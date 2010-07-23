#include "tl-ss-valgrind.hpp"
#include "tl-augmented-symbol.hpp"
#include "cxx-utils.h"

namespace TL
{
    SSValgrind::SSValgrind()
        : PragmaCustomCompilerPhase("css")
    {
        register_directive("start");
        register_directive("finish");
        register_directive("barrier");
        register_directive("wait");

        on_directive_post["start"].connect(functor(&SSValgrind::pragma_start, *this));
        on_directive_post["finish"].connect(functor(&SSValgrind::pragma_finish, *this));
        on_directive_post["barrier"].connect(functor(&SSValgrind::pragma_barrier, *this));
        on_directive_post["wait"].connect(functor(&SSValgrind::pragma_wait, *this));
    }

    void SSValgrind::run(DTO& dto)
    {
        PragmaCustomCompilerPhase::run(dto);

        // Now look for all function calls that we know are CSS functions

        ScopeLink sl = dto["scope_link"];
        AST_t a = dto["translation_unit"];

        ObjectList<AST_t> all_function_calls = a.depth_subtrees(PredicateAttr(LANG_IS_FUNCTION_CALL));

        for (ObjectList<AST_t>::iterator it = all_function_calls.begin();
                it != all_function_calls.end();
                it++)
        {
            Expression function_call(*it, sl);

            Expression function_called_expresion = function_call.get_called_expression();
            ObjectList<Expression> arguments = function_call.get_argument_list();
            if (!function_called_expresion.is_id_expression())
                // We do not handle indirect calls (through variables)
                continue;

            Scope sc = sl.get_scope(*it);

            AugmentedSymbol symbol = function_called_expresion.get_id_expression().get_symbol();

            AST_t decl_tree = symbol.get_point_of_declaration();

            ObjectList<ParameterDeclaration> parameter_decls;
            if (FunctionDefinition::predicate(decl_tree))
            {
                FunctionDefinition function_def(decl_tree, sl);
                DeclaredEntity entity = function_def.get_declared_entity();
                parameter_decls = entity.get_parameter_declarations();
            }
            else
            {
                Declaration declaration(decl_tree, sl);
                DeclaredEntity entity ( declaration.get_declared_entities()[0] );
                parameter_decls = entity.get_parameter_declarations();
            }

            int i = 0;
            ReplaceSrcIdExpression replace_parameters(sl);
            for (ObjectList<ParameterDeclaration>::iterator param_decl_it = parameter_decls.begin();
                    param_decl_it != parameter_decls.end();
                    param_decl_it++, i++)
            {
                replace_parameters.add_replacement(param_decl_it->get_name().get_symbol(),
                        "(" + arguments[i].prettyprint() + ")");
            }

            // This is a CSS task
            if (!symbol.is_task())
                continue;

            Source new_code, data_info;

            new_code
                << "{"
                << "start_task_valgrind(\"" << symbol.get_name() << "\");"
                << data_info
                << function_call.prettyprint() << ";"
                << "end_task_valgrind();"
                << "}"
                ;

            ObjectList<Type> parameters = symbol.get_type().nonadjusted_parameters();
            RefPtr<ParameterRegionList> parameter_region_list = symbol.get_parameter_region_list();

            i = 0;
            for (ObjectList<RegionList>::iterator region_list_it = parameter_region_list->begin();
                    region_list_it != parameter_region_list->end();
                    region_list_it++, i++)
            {
                Type base_type = parameters[i];

                if (base_type.is_pointer())
                {
                    base_type = base_type.points_to();
                }
                else if (base_type.is_reference())
                {
                    base_type = base_type.references_to();
                }
                else if (base_type.is_array())
                {
                    while (base_type.is_array())
                    {
                        base_type = base_type.array_element();
                    }
                }

                for (ObjectList<Region>::iterator reg_it = region_list_it->begin();
                        reg_it != region_list_it->end();
                        reg_it++)
                {
                    Region &region(*reg_it);
                    Source register_data, addr, base_type_size, span, called_function;

                    register_data
                        << called_function << "(" << addr << ", " << base_type_size << "," << span << ");" 
                        ;

                    switch ((int)reg_it->get_direction())
                    {
                        case Region::INPUT_DIR:
                            {
                                called_function << "task_input_valgrind";
                                break;
                            }
                        case Region::OUTPUT_DIR:
                            {
                                called_function << "task_output_valgrind";
                                break;
                            }
                        case Region::INOUT_DIR:
                            {
                                called_function << "task_inout_valgrind";
                                break;
                            }
                        case Region::UNKNOWN_DIR:
                            {
                                internal_error("Invalid directionality", 0);
                            }
                    }

                    if (region.get_dimension_count() == 0)
                    {
                        // Two cases: a scalar or a pointer if it is a scalar there is
                        // no need to state anything
                        if (parameters[i].is_pointer())
                        {
                            addr << arguments[i];
                            base_type_size 
                                << "sizeof(" << base_type.get_declaration(sc, "") << ")";
                        }
                        else if (parameters[i].is_reference())
                        {
                            addr << "&" << arguments[i];
                            base_type_size 
                                << "sizeof(" << base_type.get_declaration(sc, "") << ")";
                        }
                        else
                        {
                            // This is an awkward case
                            called_function = Source("task_input_value_valgrind");
                            addr << arguments[i];
                            base_type_size 
                                << "sizeof(" << base_type.get_declaration(sc, "") << ")";
                        }
                        span << 1;
                    }
                    else
                    {
                        Source dim_spec_src;
                        for (unsigned int j = 1;
                                j <= region.get_dimension_count();
                                j++)
                        {
                            // This list is reversed
                            Region::DimensionSpecifier &dim_spec(region[region.get_dimension_count() - j]);

                            DEBUG_CODE()
                            {
                                std::cerr 
                                    << "Region: #" << j << std::endl
                                    << " dimension_start: "  << dim_spec.get_dimension_start() << std::endl
                                    << " accessed_length: "  << dim_spec.get_accessed_length() << std::endl
                                    << " dimension_length: " << dim_spec.get_dimension_length() << std::endl;
                            }

                            // FIXME - A replace is due here
                            dim_spec_src << "[" << replace_parameters.replace(dim_spec.get_dimension_start()) << "]";
                            span.append_with_separator(
                                    replace_parameters.replace(dim_spec.get_accessed_length()),
                                    "*");
                        }
                        base_type_size 
                            << "sizeof(" << base_type.get_declaration(sc, "") << ")";
                        addr << "&(" << arguments[i] << dim_spec_src << ")";
                    }

                    data_info
                        << register_data
                        ;
                }
            }

            Statement enclosing_statement = function_call.get_enclosing_statement();

            AST_t new_tree = new_code.parse_statement(function_call.get_ast(), function_call.get_scope_link());
            enclosing_statement.get_ast().replace(new_tree);
        }
    }

    void SSValgrind::pragma_start(PragmaCustomConstruct ctr)
    {
        Source src;
        src
            << "start_css_valgrind();"
            ;

        AST_t new_code = src.parse_statement(ctr.get_ast(), ctr.get_scope_link());
        ctr.get_ast().replace(new_code);
    }

    void SSValgrind::pragma_finish(PragmaCustomConstruct ctr)
    {
        Source src;
        src
            << "end_css_valgrind();"
            ;

        AST_t new_code = src.parse_statement(ctr.get_ast(), ctr.get_scope_link());
        ctr.get_ast().replace(new_code);
    }

    void SSValgrind::pragma_barrier(PragmaCustomConstruct ctr)
    {
        Source src;
        src
            << "barrier_css_valgrind();"
            ;

        AST_t new_code = src.parse_statement(ctr.get_ast(), ctr.get_scope_link());
        ctr.get_ast().replace(new_code);
    }

    void SSValgrind::pragma_wait(PragmaCustomConstruct ctr)
    {
        PragmaCustomClause clause = ctr.get_clause("on");

        if (clause.is_defined())
        {
            ObjectList<Expression> expression_list = clause.get_expression_list();

            Source src;
            for (ObjectList<Expression>::iterator it = expression_list.begin();
                    it != expression_list.end();
                    it++)
            {
                src
                    << "wait_on_valgrind(" << *it << ");"
                    ;
            }

            AST_t new_code = src.parse_statement(ctr.get_ast(), ctr.get_scope_link());
            ctr.get_ast().replace(new_code);
        }
    }
}

EXPORT_PHASE(TL::SSValgrind);