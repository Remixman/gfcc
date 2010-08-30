#include "tl-devices.hpp"
#include "nanox-smp_numa.hpp"

using namespace TL;
using namespace TL::Nanox;

static std::string smp_outline_name(const std::string &task_name)
{
    return "_smp_numa_" + task_name;
}

static Type compute_replacement_type_for_vla(Type type, 
        ObjectList<Source>::iterator dim_names_begin,
        ObjectList<Source>::iterator dim_names_end)
{
    Type new_type(NULL);
    if (type.is_array())
    {
        new_type = compute_replacement_type_for_vla(type.array_element(), dim_names_begin + 1, dim_names_end);

        if (dim_names_begin == dim_names_end)
        {
            internal_error("Invalid dimension list", 0);
        }

        new_type = new_type.get_array_to(*dim_names_begin);
    }
    else if (type.is_pointer())
    {
        new_type = compute_replacement_type_for_vla(type.points_to(), dim_names_begin, dim_names_end);
        new_type = new_type.get_pointer_to();
    }
    else
    {
        new_type = type;
    }

    return new_type;
}

static void do_smp_numa_outline_replacements(
        AST_t body,
        ScopeLink scope_link,
        const DataEnvironInfo& data_env_info,
        Source &initial_code,
        Source &replaced_outline)
{
    Source copy_setup;
    Scope sc = scope_link.get_scope(body);

    initial_code
        << copy_setup
        ;

    ReplaceSrcIdExpression replace_src(scope_link);

    bool err_declared = false;

    ObjectList<DataEnvironItem> data_env_items = data_env_info.get_items();
    for (ObjectList<DataEnvironItem>::iterator it = data_env_items.begin();
            it != data_env_items.end();
            it++)
    {
        DataEnvironItem& data_env_item(*it);

        Symbol sym = data_env_item.get_symbol();
        const std::string field_name = data_env_item.get_field_name();

        if (data_env_item.is_private())
            continue;

        if (data_env_item.is_copy())
        {
        	replace_src.add_replacement(sym, "_args->" + field_name);
        }
    }

    ObjectList<OpenMP::CopyItem> copies = data_env_info.get_copy_items();
    unsigned int j = 0;
    for (ObjectList<OpenMP::CopyItem>::iterator it = copies.begin();
            it != copies.end();
            it++, j++)
    {
        DataReference data_ref = it->get_copy_expression();
        Symbol sym = data_ref.get_base_symbol();
        Type type = sym.get_type();

        bool requires_indirect = false;

        if (type.is_array())
        {
            type = type.array_element().get_pointer_to();
        }
        else
        {
            requires_indirect = true;
            type = type.get_pointer_to();
        }

        // There are some problems with the typesystem currently
        // that require these workarounds
        if (data_ref.is_shaping_expression())
        {
            // Shaping expressions ([e] a)  have a type of array but we do not
            // want the array but the related pointer
            type = data_ref.get_data_type();
            requires_indirect = false;
        }
        else if (data_ref.is_array_section())
        {
            // Array sections have a scalar type, but the data type will be array
            // See ticket #290
            type = data_ref.get_data_type().array_element().get_pointer_to();
            requires_indirect = false;
        }

        std::string copy_name = "_cp_" + sym.get_name();

        if (!err_declared)
        {
            copy_setup
                << "nanos_err_t cp_err;"
                ;
            err_declared = true;
        }

        DataEnvironItem data_env_item = data_env_info.get_data_of_symbol(sym);

        ERROR_CONDITION(!data_env_item.get_symbol().is_valid(),
                "Invalid data for copy symbol", 0);

        std::string field_addr = "_args->" + data_env_item.get_field_name();

        copy_setup
            << type.get_declaration(sc, copy_name) << ";"
            << "cp_err = nanos_get_addr(" << j << ", (void**)&" << copy_name << ");"
            << "if (cp_err != NANOS_OK) nanos_handle_error(cp_err);"
            ;

        if (!requires_indirect)
        {
            replace_src.add_replacement(sym, copy_name);
        }
        else
        {
            replace_src.add_replacement(sym, "(*" + copy_name + ")");
        }
    }

    replaced_outline << replace_src.replace(body);
}

DeviceSMP_NUMA::DeviceSMP_NUMA()
    : DeviceProvider(/* needs_copies */ true)
{
    DeviceHandler &device_handler(DeviceHandler::get_device_handler());

    device_handler.register_device("smp_numa", this);

    set_phase_name("Nanox SMP NUMA support");
    set_phase_description("This phase is used by Nanox phases to implement SMP NUMA device support");
}

void DeviceSMP_NUMA::create_outline(
        const std::string& task_name,
        const std::string& struct_typename,
        DataEnvironInfo &data_environ,
        const OutlineFlags& outline_flags,
        AST_t reference_tree,
        ScopeLink sl,
        Source initial_setup,
        Source outline_body)
{
    AST_t function_def_tree = reference_tree.get_enclosing_function_definition();
    FunctionDefinition enclosing_function(function_def_tree, sl);

    Source result, body, outline_name, parameter_list;

    Source forward_declaration;
    Symbol function_symbol = enclosing_function.get_function_symbol();

    if (!function_symbol.is_member())
    {
        Source template_header;

        IdExpression function_name = enclosing_function.get_function_name();
        Declaration point_of_decl = function_name.get_declaration();
        DeclarationSpec decl_specs = point_of_decl.get_declaration_specifiers();
        ObjectList<DeclaredEntity> declared_entities = point_of_decl.get_declared_entities();
        DeclaredEntity declared_entity = *(declared_entities.begin());

        forward_declaration 
            << template_header
            << decl_specs.prettyprint()
            << " "
            << declared_entity.prettyprint()
            << ";";
    }


    result
        << forward_declaration
        << "static void " << outline_name << "(" << parameter_list << ")"
        << "{"
        << body
        << "}"
        ;

    parameter_list
        << struct_typename << "* _args"
        ;

    outline_name
        << smp_outline_name(task_name)
        ;

    Source private_vars, final_code;

    body
        << private_vars
        << initial_setup
        << outline_body
        << final_code
        ;

    ObjectList<DataEnvironItem> data_env_items = data_environ.get_items();

    for (ObjectList<DataEnvironItem>::iterator it = data_env_items.begin();
            it != data_env_items.end();
            it++)
    {
        if (!it->is_private())
            continue;

        Symbol sym = it->get_symbol();
        Type type = sym.get_type();

        private_vars
            << type.get_declaration(sym.get_scope(), sym.get_name()) << ";"
            ;
    }

    if (outline_flags.barrier_at_end)
    {
        final_code
            << "nanos_team_barrier();"
            ;
    }

    if (outline_flags.leave_team)
    {
        final_code
            << "nanos_leave_team();"
            ;
    }

    // Parse it in a sibling function context
    AST_t outline_code_tree
        = result.parse_declaration(enclosing_function.get_ast(), sl);
    reference_tree.prepend_sibling_function(outline_code_tree);
}

void DeviceSMP_NUMA::get_device_descriptor(const std::string& task_name,
        DataEnvironInfo &data_environ,
        const OutlineFlags& outline_flags,
        Source &ancillary_device_description,
        Source &device_descriptor)
{
    Source outline_name;
    if (!outline_flags.implemented_outline)
    {
        outline_name
            << smp_outline_name(task_name);
        ;
    }
    else
    {
        outline_name << task_name;
    }

    ancillary_device_description
        << comment("SMP device descriptor")
        << "nanos_smp_args_t " << task_name << "_smp_numa_args = { (void(*)(void*))" << outline_name << "};"
        ;

    device_descriptor
        << "{ nanos_smp_factory, nanos_smp_dd_size, &" << task_name << "_smp_numa_args },"
        ;
}

void DeviceSMP_NUMA::do_replacements(DataEnvironInfo& data_environ,
        AST_t body,
        ScopeLink scope_link,
        Source &initial_setup,
        Source &replaced_src)
{
    do_smp_numa_outline_replacements(body,
            scope_link,
            data_environ,
            initial_setup,
            replaced_src);
}

EXPORT_PHASE(TL::Nanox::DeviceSMP_NUMA);
