#include <stdlib.h>
#include <string.h>
#include "cxx-typeenviron.h"
#include "cxx-typeutils.h"
#include "cxx-cexpr.h"
#include "cxx-utils.h"

#ifdef MAX
  #warning MAX already defined here! Overriding
#endif

#undef MAX
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

static long long unsigned int _bytes_due_to_type_environment = 0;

long long unsigned int type_environment_used_memory(void)
{
    return _bytes_due_to_type_environment;
}

/* Utility functions */
static void next_offset_with_align(_size_t* current_offset, _size_t required_align)
{
    // FIXME - This is a bit stupid, it can be implemented better with
    // arithmetic, but I felt lazy when I wrote that
    while ((*current_offset) % required_align != 0)
        (*current_offset)++;
}

static int round_to_upper_byte(_size_t bit_offset)
{
    while (bit_offset % 8 == 0)
        bit_offset++;

    return bit_offset / 8;
}

/*
   Generic environment definitions
   System V mainly

   C
 */

static void system_v_array_sizeof(type_t* t)
{
    type_t* element_type = array_type_get_element_type(t);
    AST expr = array_type_get_array_size_expr(t);
    decl_context_t decl_context = array_type_get_array_size_expr_context(t);

    _size_t element_size = type_get_size(element_type);
    _size_t element_align = type_get_alignment(element_type);

    if (is_constant_expression(expr, decl_context))
    {
        literal_value_t l = evaluate_constant_expression(expr, decl_context);
        char valid = 0;

        int size = literal_value_to_uint(l, &valid);

        if (valid)
        {
            type_set_size(t, size * element_size);
            type_set_alignment(t, element_align);
            type_set_valid_size(t, 1);
            return;
        }
    }
    internal_error("Cannot compute the size of the array type '%s'!", print_declarator(t));
}

static void system_v_field_layout(scope_entry_t* field,
        _size_t *offset,
        _size_t *whole_align,
        _size_t *current_bit_within_storage,
        char *previous_was_bitfield)
{
    type_t* field_type = field->type_information;

    _size_t field_size = type_get_size(field_type);
    _size_t field_align = type_get_alignment(field_type);

    if (field->entity_specs.is_bitfield)
    {
        // Named bitfields

        // Bitfields are very special, otherwise all this stuff would be
        // extremely easy
        unsigned int bitsize = 0;
        {
            literal_value_t literal = evaluate_constant_expression(
                    field->entity_specs.bitfield_expr,
                    field->entity_specs.bitfield_expr_context);

            char valid = 0;
            bitsize = literal_value_to_uint(literal, &valid);
            if (!valid)
            {
                internal_error("Invalid bitfield expression", 0);
            }
        }

        if (bitsize == 0)
        {
            // FIXME - Frontend should check that unnamed bitfields are the only
            // ones that can have 0 width
            if (!(*previous_was_bitfield))
            {
                // If the previous was not a bitfield then we have
                // to adjust the current bit within storage just after
                // the previous field
                (*current_bit_within_storage) = ((*offset) % field_align) * 8;
            }

            // Just fill all the remaining bits
            (*current_bit_within_storage) = field_size * 8;
        }
        else
        {
            if (!(*previous_was_bitfield))
            {
                // If the previous was not a bitfield then we have
                // to adjust the current bit within storage just after
                // the previous field
                (*current_bit_within_storage) = ((*offset) % field_align) * 8;
            }

            // In System V bytes are 8 bits :)
            ERROR_CONDITION(bitsize > (field_size * 8),
                    "This bitsize (%d) is greater than the related field type (%d)", 
                    bitsize, (field_size * 8));

            // Not enough remaining bits in this storage unit, just advance as usual.
            if ((field_size * 8 - (*current_bit_within_storage)) <
                    bitsize)
            {
                // Example (assume shorts are 16 bit wide)
                // struct A 
                // {
                //    char c;
                //    short :9;
                // };
                //
                // We would start laying out the bitfield :9 just after
                // 'c', but this would mean starting at bit 8 of the
                // current storage and 8 + 9 does not fit in
                // field_size * 8. No overlapping applies in
                // System V, so advance to the next storage unit suitable
                // for the bitfield type ('short' in this example)

                next_offset_with_align(&(*offset), field_align);

                (*current_bit_within_storage) = 0;
            }

            // Now move within the current storage
            (*current_bit_within_storage) += bitsize;

            if (!field->entity_specs.is_unnamed_bitfield)
            {
                // Named bitfields DO contribute to the align of the whole struct
                // Update the whole align, this is needed for the tail padding
                if ((*whole_align) < field_align)
                    (*whole_align) = field_align;
            }
        }

        // This is a bitfield
        (*previous_was_bitfield) = 1;
    }
    else
    {
        if ((*previous_was_bitfield))
        {
            // Offset is in bytes, but for bitfields we have bits, and we have 
            // to round to the next byte
            (*offset) += round_to_upper_byte((*current_bit_within_storage));
        }

        // Update the whole align, this is needed for the tail padding
        if ((*whole_align) < field_align)
            (*whole_align) = field_align;

        // Advance the (*offset) for the current field alignment This computes
        // the "internal padding"
        next_offset_with_align(&(*offset), field_align);

        // Store the (*offset) for this field
        field->entity_specs.field_offset = (*offset);

        // Now update the (*offset), we have to advance at least field_size
        (*offset) += field_size;

        // This is not a bitfield
        (*previous_was_bitfield) = 0;
    }
}

static void system_v_union_sizeof(type_t* class_type)
{
    // offset is used only for non-bitfields and when bitfields
    // cause an effective advance of the offset, otherwise bitfields only
    // use current_bit_within_storage
    _size_t offset = 0;
    _size_t whole_align = 1;

    // bitfields do not use offset except as needed
    _size_t current_bit_within_storage = 0;

    char previous_was_bitfield = 0;

    int i;
    int num_fields = class_type_get_num_nonstatic_data_members(class_type);

    for (i = 0; i < num_fields; i++)
    {
        // For a union its fields are always in offset 0
        offset = 0;
        current_bit_within_storage = 0;
        previous_was_bitfield = 0;
        
        scope_entry_t* field = class_type_get_nonstatic_data_member_num(class_type, i);
        system_v_field_layout(field,
                &offset,
                &whole_align,
                &current_bit_within_storage,
                &previous_was_bitfield);
    }

    // Round the offset to upper byte if the last field was a bitfield
    if (previous_was_bitfield)
    {
        offset += round_to_upper_byte(current_bit_within_storage);
    }
     
    // Compute tail padding, just ensure that the next laid out entity
    // will satisfy the alignment 
    next_offset_with_align(&offset, whole_align);

    // If it remains 0, it means that the struct was empty
    // Make it like an int
    if (offset == 0)
    {
        offset = CURRENT_CONFIGURATION(type_environment)->sizeof_signed_int;
        whole_align = CURRENT_CONFIGURATION(type_environment)->alignof_signed_int;
    }

    type_set_size(class_type, offset);
    type_set_alignment(class_type, whole_align);
    type_set_valid_size(class_type, 1);
}

static void system_v_struct_sizeof(type_t* class_type)
{
    // offset is used only for non-bitfields and when bitfields
    // cause an effective advance of the offset, otherwise bitfields only
    // use current_bit_within_storage
    _size_t offset = 0;
    _size_t whole_align = 1;

    // bitfields do not use offset except as needed
    _size_t current_bit_within_storage = 0;

    char previous_was_bitfield = 0;

    int i;
    int num_fields = class_type_get_num_nonstatic_data_members(class_type);

    for (i = 0; i < num_fields; i++)
    {
        scope_entry_t* field = class_type_get_nonstatic_data_member_num(class_type, i);
        system_v_field_layout(field,
                &offset,
                &whole_align,
                &current_bit_within_storage,
                &previous_was_bitfield);
    }

    // Round the offset to upper byte if the last field was a bitfield
    if (previous_was_bitfield)
    {
        offset += round_to_upper_byte(current_bit_within_storage);
    }
     
    // Compute tail padding, just ensure that the next laid out entity
    // would satisfy the alignment 
    next_offset_with_align(&offset, whole_align);

    // If it remains 0, it means that the struct was empty
    // In C++ classes can be empty but have sizeof 1
    // In C, structs cannot be empty, so leave them as being sizeof(X) == 0 
    CXX_LANGUAGE()
    {
        if (offset == 0)
        {
            offset = 1;
            whole_align = 1;
        }
    }

    type_set_size(class_type, offset);
    type_set_alignment(class_type, whole_align);
    type_set_valid_size(class_type, 1);
}

static void system_v_generic_sizeof(type_t* t)
{
    // FIXME - Named types like in the case below are not well handled
    // typedef __attribute__((aligned(16))) int T;
    if (is_array_type(t))
    {
        system_v_array_sizeof(t);
    }
    else if (is_named_class_type(t))
    {
        type_t* class_type = get_actual_class_type(t);
        system_v_generic_sizeof(class_type);

        type_set_size(t, type_get_size(class_type));
        type_set_alignment(t, type_get_alignment(class_type));
        type_set_valid_size(t, 1);
    }
    // It must be unnamed
    else if (is_union_type(t))
    {
        system_v_union_sizeof(t);
    }
    else if (is_unnamed_class_type(t))
    {
        system_v_struct_sizeof(t);
    }
    else
    {
        internal_error("Unhandled type size '%s'", print_declarator(t));
    }
}

/* 
   Itanium ABI for C++ (the one followed by most vendors of C++)
 */
static void cxx_abi_array_sizeof(type_t* t)
{
    type_t* element_type = array_type_get_element_type(t);
    AST expr = array_type_get_array_size_expr(t);
    decl_context_t decl_context = array_type_get_array_size_expr_context(t);

    _size_t element_size = type_get_size(element_type);
    _size_t element_align = type_get_alignment(element_type);

    if (is_constant_expression(expr, decl_context))
    {
        literal_value_t l = evaluate_constant_expression(expr, decl_context);
        char valid = 0;

        int size = literal_value_to_uint(l, &valid);

        if (valid)
        {
            type_set_size(t, size * element_size);
            type_set_alignment(t, element_align);
            type_set_valid_size(t, 1);
            return;
        }
    }
    internal_error("Cannot compute the size of the array type '%s'!", print_declarator(t));
}

static void cxx_abi_union_sizeof(type_t* t)
{
    system_v_union_sizeof(t);
}

#define MAX_BASES 32

typedef
struct base_info_preorder_tag
{
    scope_entry_t* entry;
    char is_virtual;
} base_info_preorder_t;

static void class_type_preorder_all_bases_rec(type_t* t,
        base_info_preorder_t base_info[MAX_BASES], int *num_elems)
{
    type_t* class_type = get_actual_class_type(t);
    ERROR_CONDITION((*num_elems) == MAX_BASES, 
            "Too many bases (%d)\n", (*num_elems));

    int num_bases = class_type_get_num_bases(class_type);
    int i;

    for (i = 0; i < num_bases; i++)
    {
        char is_virtual = 0;
        scope_entry_t* current_base = 
            class_type_get_base_num(class_type, i, &is_virtual);

        char is_found = 0;
        int j;
        for (j = 0; (j < (*num_elems)) && !is_found; j++)
        {
            if (base_info[j].entry == current_base
                    && base_info[j].is_virtual 
                    && is_virtual)
                is_found = 1;
        }

        if (!is_found)
        {
            base_info[(*num_elems)].entry = current_base;
            base_info[(*num_elems)].is_virtual = is_virtual;
            (*num_elems)++;

            class_type_preorder_all_bases_rec(current_base->type_information,
                    base_info, num_elems);
        }
    }
}

static void class_type_preorder_all_bases(type_t* t,
        base_info_preorder_t base_info[MAX_BASES],
        int *num_elems)
{
    *num_elems = 0;
    class_type_preorder_all_bases_rec(t, base_info, num_elems);
}

static scope_entry_t* cxx_abi_class_type_get_primary_base_class(type_t* t, char *is_virtual);

static void class_type_get_indirect_virtual_primary_bases(
        base_info_preorder_t bases[MAX_BASES],
        int num_bases, 
        base_info_preorder_t indirect_primary_bases[MAX_BASES],
        int *num_primaries)
{
    (*num_primaries) = 0;

    int i;
    for (i = 0; i < num_bases; i++)
    {
        char current_base_is_virtual = 0;
        // Recursion
        scope_entry_t* entry = cxx_abi_class_type_get_primary_base_class(
                bases[i].entry->type_information, 
                &current_base_is_virtual);

        if (entry != NULL
                && current_base_is_virtual)
        {
            char is_found = 0;
            int j;
            for (j = 0; (j < (*num_primaries)) && !is_found; j++)
            {
                if (indirect_primary_bases[j].entry == entry
                        && indirect_primary_bases[j].is_virtual
                        /* && current_base_is_virtual */)
                    is_found = 1;
            }

            if (!is_found)
            {
                ERROR_CONDITION((*num_primaries) == MAX_BASES,
                        "Too many primaries (%d)!", (*num_primaries));
                indirect_primary_bases[(*num_primaries)].entry = entry;
                // This is always 1 since we are only interested in virtual ones
                indirect_primary_bases[(*num_primaries)].is_virtual = 1;
                (*num_primaries)++;
            }
        }
    }

}

// Returns null if no primary base class. is_virtual is only valid if non null
// was returned
static scope_entry_t* cxx_abi_class_type_get_primary_base_class(type_t* t, char *is_virtual)
{
    type_t* class_type = get_actual_class_type(t);

    int num_bases = 0;
    base_info_preorder_t base_info[MAX_BASES];

    class_type_preorder_all_bases(class_type, base_info, &num_bases);

    // 1. Identify all _virtual_ base classes, direct or indirect, that are
    // primary base classes for some other direct or indirect base class

    int num_primaries = 0;
    base_info_preorder_t indirect_primary_bases[MAX_BASES];

    class_type_get_indirect_virtual_primary_bases(base_info, num_bases,
            // indirect primary bases will be stored in 'indirect_primary_bases'
            indirect_primary_bases, &num_primaries);

    // 2. If C has a dynamic base class, attempt to choose a primary base classB.
    // a) It is the first (in direct base class order) non-virtual dynamic base class
    int i;
    int num_direct_bases = class_type_get_num_bases(class_type);
    for (i = 0; i < num_direct_bases; i++)
    {
        char current_base_is_virtual = 0;
        scope_entry_t* current_direct_base = class_type_get_base_num(class_type, i, 
                &current_base_is_virtual);

        if (!current_base_is_virtual
                && class_type_is_dynamic(current_direct_base->type_information))
        {
            *is_virtual = 0;
            return current_direct_base;
        }
    }

    // b) Otherwise it is a nearly empty virtual base class, the first one in (preorder)
    // inheritance graph which is not an indirect primary base class if any exist or 
    // just the first one if they are all indirect primaries
    *is_virtual = 1;

    scope_entry_t* first_ne_virtual_base = NULL;

    for (i = 0; i < num_bases; i++)
    {
        if (base_info[i].is_virtual
                && class_type_is_nearly_empty(base_info[i].entry->type_information))
        {
            if (first_ne_virtual_base == NULL)
            {
                first_ne_virtual_base = base_info[i].entry;
            }

            char is_found = 0;
            int j;
            for (j = 0; (j < num_primaries) && !is_found; j++)
            {
                if (indirect_primary_bases[j].entry == base_info[i].entry)
                    is_found = 1;
            }

            if (!is_found)
            {
                return base_info[i].entry;
            }
        }
    }

    // Return the first one. It might be null if neither a) nor b) returned anything
    return first_ne_virtual_base;
}

typedef
struct offset_info_tag
{
    _size_t offset;
    scope_entry_list_t* subobjects;
    struct offset_info_tag* next;
} offset_info_t;

typedef
struct layout_info_tag
{
    _size_t dsize;
    _size_t size;
    _size_t align;
    _size_t nvalign;
    _size_t nvsize;
    offset_info_t* offsets;

    char previous_was_bitfield;
    _size_t bit_within_bitfield;

    char previous_was_base;
    scope_entry_t* previous_base;
} layout_info_t;

static void cxx_abi_register_entity_offset(layout_info_t* layout_info,
        scope_entry_t* entry,
        _size_t offset)
{
    offset_info_t* previous_offset = NULL;
    offset_info_t* current_offset = layout_info->offsets;

    while (current_offset != NULL)
    {
        if (current_offset->offset == offset)
        {
            scope_entry_list_t* new_entry_list
                = counted_calloc(1, sizeof(*new_entry_list), &_bytes_due_to_type_environment);

            new_entry_list->entry = entry;
            new_entry_list->next = current_offset->subobjects;
            current_offset->subobjects = new_entry_list;

            // Do nothing else
            return;
        }
        else if (current_offset->offset > offset)
        {
            // This offset does not exist!, we have to add it
            break;
        }

        previous_offset = current_offset;
        current_offset = current_offset->next;
    }

    // Cases: previous_offset == NULL means we are at the beginning
    // current_offset == NULL means we are the largest offset

    offset_info_t* new_offset_info = counted_calloc(1, 
            sizeof(*new_offset_info), &_bytes_due_to_type_environment);
    new_offset_info->offset = offset;

    scope_entry_list_t* new_entry_list
        = counted_calloc(1, sizeof(*new_entry_list), &_bytes_due_to_type_environment);
    new_entry_list->entry = entry;
    new_offset_info->subobjects = new_entry_list;

    new_offset_info->next = current_offset;

    if (previous_offset == NULL)
    {
        layout_info->offsets = new_offset_info;
    }
    else
    {
        previous_offset->next = new_offset_info;
    }
}

#if 0
static void cxx_abi_print_layout(layout_info_t* layout_info)
{
    fprintf(stderr, "--- Layout of class ---\n");

    offset_info_t* current_offset = layout_info->offsets;
    while (current_offset != NULL)
    {
        scope_entry_list_t* subobjects = current_offset->subobjects;


        while (subobjects != NULL)
        {
            scope_entry_t* entry = subobjects->entry;

            char is_dependent = 0;
            int max_qualif_level = 0;
            fprintf(stderr, "@%04llu:", current_offset->offset);

            fprintf(stderr, " %s\n", get_fully_qualified_symbol_name(entry, 
                        entry->decl_context, &is_dependent, &max_qualif_level));

            subobjects = subobjects->next;
        }

        current_offset = current_offset->next;
    }

    fprintf(stderr, "--- End of layout ---\n");
}
#endif

static void cxx_abi_register_subobject_offset(layout_info_t* layout_info,
        scope_entry_t* member,
        _size_t chosen_offset)
{
    cxx_abi_register_entity_offset(layout_info, member, chosen_offset);

    if (is_class_type(member->type_information))
    {
        type_t* class_type = get_actual_class_type(member->type_information);

        int num_bases = class_type_get_num_bases(class_type);
        int i;

        for (i = 0; i < num_bases; i++)
        {
            char is_virtual;
            scope_entry_t* base_class = class_type_get_base_num(class_type, i, &is_virtual);

            if (!is_virtual)
            {
                _size_t base_offset = class_type_get_offset_base_num(class_type, i);

                // Recurse
                cxx_abi_register_subobject_offset(layout_info, 
                        base_class,
                        chosen_offset + base_offset);
            }
        }

        // Nonstatic data members
        int num_nonstatic_data_members 
            = class_type_get_num_nonstatic_data_members(class_type);

        for (i = 0; i < num_nonstatic_data_members; i++)
        {
            scope_entry_t* nonstatic_data_member 
                = class_type_get_nonstatic_data_member_num(class_type, i);

            // Bitfields do not have offset!
            if (!nonstatic_data_member->entity_specs.is_bitfield)
            {
                _size_t field_offset = nonstatic_data_member->entity_specs.field_offset;

                // Recurse
                cxx_abi_register_subobject_offset(layout_info,
                        nonstatic_data_member,
                        chosen_offset + field_offset);
            }
        }
    }
}

static char cxx_abi_conflicting_member(layout_info_t* layout_info,
        scope_entry_t* member,
        _size_t candidate_offset)
{
    offset_info_t* current_offset = layout_info->offsets;

    while (current_offset != NULL)
    {
        /*
           Things to consider: Conflicts arise because of empty bases. They
           cannot arise because of anything else. Nonempty bases, like data
           members (even if they are empty classes) have nonempty space
           allocated for them. In fact conflicts only happen when laying bases
           or when all bases have ben laid out and we start with data members.

           It will never happen that we are checking whether there is a
           conflict for a nonempty subobject (i.e.: a data member or nonempty
           base class) and offsets bigger than the candidate are found. Should
           this happen it would mean we are allocating something on potentially
           the same allocation of another thing!
           */
        if (current_offset->offset == candidate_offset)
        {
            // Check all elements linked to this offset (there can be more than
            // one if these are empty bases)
            scope_entry_list_t* current_subobject = current_offset->subobjects;
            while (current_subobject != NULL)
            {
                scope_entry_t* subobject_entry = current_subobject->entry;

                if (equivalent_types(subobject_entry->type_information,
                            member->type_information))
                {
                    return 1;
                }

                current_subobject = current_subobject->next;
            }
        }
        current_offset = current_offset->next;
    }

    return 0;
}

static type_t* get_largest_integral_with_bits(unsigned int bits)
{
    // Try in order: unsigned long long, signed long long, unsigned long,
    // signed long, unsigned int, int, unsigned short, short, unsigned char, char

    type_t* integral_types[] = {
        get_unsigned_long_long_int_type(),
        get_signed_long_long_int_type(),
        get_unsigned_long_int_type(),
        get_signed_long_int_type(),
        get_unsigned_int_type(),
        get_signed_int_type(),
        get_unsigned_short_int_type(),
        get_signed_short_int_type(),
        get_unsigned_char_type(),
        get_signed_char_type(),
    };

    unsigned int i;
    unsigned int max_elems = STATIC_ARRAY_LENGTH(integral_types);
    for (i = 0; i < max_elems; i++)
    {
        if ((type_get_size(integral_types[i]) * 8) <= bits)
                return integral_types[i];
    }

    // if we reach here what? :)
    return get_char_type();
}

static void cxx_abi_lay_bitfield(type_t* t UNUSED_PARAMETER, 
        scope_entry_t* member, 
        layout_info_t* layout_info)
{
    unsigned int bitsize = 0;
    {
        literal_value_t literal = evaluate_constant_expression(
                member->entity_specs.bitfield_expr,
                member->entity_specs.bitfield_expr_context);

        char valid = 0;
        bitsize = literal_value_to_uint(literal, &valid);
        if (!valid)
        {
            internal_error("Invalid bitfield expression", 0);
        }
    }

    _size_t size_of_member = type_get_size(member->type_information);
    _size_t current_bit_within_storage = 0;

    // At least one byte is always used
    _size_t used_bytes = 0;

    // Starting offset, we may move it if it is not suitable
    // for the bitfield being laid out
    _size_t offset = layout_info->dsize;

    if (size_of_member * 8 >= bitsize)
    {
        _size_t member_align = type_get_alignment(member->type_information);

        if (layout_info->previous_was_base)
        {
            // Allocate like the underlying ABI says constrained that a bitfield
            // is never placed in the tail padding of a base class of C
            next_offset_with_align(&offset, 
                   type_get_alignment(layout_info->previous_base->type_information));
        }

        if (layout_info->previous_was_bitfield)
        {
            // Next available bits in this offset
            // if dsize(C) > 0 and dsize(C) - 1 is partially filled by a
            // bitfield and that bitfield is also a data member declared in C, 
            // next available bits are the unfilled bits in dsize(C) - 1,
            if (offset > 0)
            {
                // Fix the offset
                offset = offset - 1;
            }

            current_bit_within_storage = layout_info->bit_within_bitfield;
        }
        else
        {
            // otherwise bits available start at the offset
            current_bit_within_storage = (offset % member_align) * 8;
        }

        if (bitsize == 0)
        {
            // Just fill all the remaining bits
            current_bit_within_storage = size_of_member * 8;
        }
        else
        {
            if ((size_of_member * 8 - current_bit_within_storage) <
                    bitsize)
            {
                // No overlapping in SysV
                next_offset_with_align(&offset, member_align);
                current_bit_within_storage = 0;
            }

            current_bit_within_storage += bitsize;
        }

        // Update align if not unnamed
        if (!member->entity_specs.is_unnamed_bitfield)
            layout_info->align = MAX(layout_info->align, member_align);
    }
    else
    {
        // GCC gives a warning for these cases 
        type_t* align_integral_type = get_largest_integral_with_bits(bitsize);
        _size_t integral_type_align = type_get_alignment(align_integral_type);

        next_offset_with_align(&offset, integral_type_align);

        // Advance the required amount of bytes
        used_bytes += bitsize / 8;
        // And we save the bit in the last (partially) filled byte
        current_bit_within_storage = bitsize % 8;

        // Update align if not unnamed
        if (!member->entity_specs.is_unnamed_bitfield)
            layout_info->align = MAX(layout_info->align, integral_type_align);
    }

    // It may be zero only for bitfields bigger than their base type
    used_bytes += round_to_upper_byte(current_bit_within_storage);

    layout_info->dsize = offset + used_bytes;
    layout_info->size = MAX(layout_info->size, layout_info->dsize);

    layout_info->previous_was_bitfield = 1;
    layout_info->bit_within_bitfield = current_bit_within_storage;
}

static void cxx_abi_lay_member_out(type_t* t, 
        scope_entry_t* member, 
        layout_info_t* layout_info,
        char is_base_class,
        // If num_base_class < 0 then do not set its offset
        int num_base_class)
{
    if (member->entity_specs.is_bitfield)
    {
        cxx_abi_lay_bitfield(t, member, layout_info);
    }
    else
    {
        if (is_base_class
                && class_type_is_empty(member->type_information))
        {
            // Start attempting to put this class at offset zero
            _size_t offset = 0;
            /*
               Surprisingly, even an empty class can have a sizeof different than 1

               Example

               struct A { };
               struct B : A { };
               struct C : A { };
               struct D : B, C { };

               D is empty but its sizeof(D) is 2 since there are two A's that must be allocated
               in different offsets:

               D          <-- 0
               D::B       <-- 0
               D::B::A    <-- 0
               D::C       <-- 1
               D::C::A    <-- 1 (this causes the conflict)
             */

            _size_t size_of_base = type_get_size(member->type_information);
            _size_t non_virtual_align = class_type_get_non_virtual_align(member->type_information);

            if (cxx_abi_conflicting_member(layout_info, member, offset))
            {
                // If unsuccessful due to a component type conflict, proceed
                // with attempts at dsize(C)
                offset = layout_info->dsize;

                // Increment by nvalign(D)
                while (cxx_abi_conflicting_member(layout_info, member, offset))
                {
                    offset += non_virtual_align;
                }
            }

            if (num_base_class >= 0)
            {
                class_type_set_offset_base_num(t, num_base_class, offset);
                cxx_abi_register_subobject_offset(layout_info, member, offset);
            }

            // Once offset(D) has been chosen, update sizeof(C) to max(sizeof(C), offset(D) + sizeof(D))
            layout_info->size = MAX(layout_info->size, offset + size_of_base);
        }
        else // Plain data member or non empty base class
        {
            // Start at offset dsize(C) incremented for alignment to nvalign(D)
            // or align(D) for data members
            _size_t offset = layout_info->dsize;
            // Compute this this to ensure that this member has its sizeof already computed
            _size_t sizeof_member = type_get_size(member->type_information);
            if (is_base_class)
            {
                next_offset_with_align(&offset, 
                        class_type_get_non_virtual_align(member->type_information));
            }
            else
            {
                next_offset_with_align(&offset, 
                        type_get_alignment(member->type_information));
            }

            while (cxx_abi_conflicting_member(layout_info, member, offset))
            {
                if (is_base_class)
                {
                    offset += class_type_get_non_virtual_align(member->type_information);
                }
                else
                {
                    offset += type_get_alignment(member->type_information);
                }
            }

            if (is_base_class)
            {
                // If D is a base class update sizeof(C) to max (sizeof(C), offset(D) + nvsize(D))
                _size_t non_virtual_size = class_type_get_non_virtual_size(member->type_information);
                _size_t non_virtual_align = class_type_get_non_virtual_align(member->type_information);
                layout_info->size = MAX(layout_info->size, offset + non_virtual_size);

                if (num_base_class >= 0)
                {
                    class_type_set_offset_base_num(t, num_base_class, offset);
                    cxx_abi_register_subobject_offset(layout_info, member, offset);
                }

                // Update dsize(C) to offset(D) + nvsize(D) and align(C) to max(align(C), nvalign(D))
                layout_info->dsize = offset + non_virtual_size;
                layout_info->align = MAX(layout_info->align, non_virtual_align);
            }
            else
            {
                member->entity_specs.field_offset = offset;
                cxx_abi_register_subobject_offset(layout_info, member, offset);

                // Otherwise, if D is a data member, update sizeof(C) to max(sizeof(C), offset(D) + sizeof(D))
                layout_info->size = MAX(layout_info->size, offset + sizeof_member);

                _size_t alignment = type_get_alignment(member->type_information);

                // Update dsize(C) to offset(D) + sizeof(D), align(C) to max(align(C), align(D))
                layout_info->dsize = offset + sizeof_member;
                layout_info->align = MAX(layout_info->align, alignment);
            }
        }

        // This is not a bitfield
        layout_info->previous_was_bitfield = 0;
    }

    // This is needed for bitfields
    layout_info->previous_was_base = is_base_class;
    layout_info->previous_base = is_base_class ? member : NULL;
}

static void cxx_abi_class_sizeof(type_t* class_type)
{
    // Initialization: sizeof to 0, align to 1, dsize to 0
    layout_info_t layout_info;
    memset(&layout_info, 0, sizeof(layout_info));

    layout_info.size = 0;
    layout_info.align = 1;
    layout_info.dsize = 0;

    // For each data component D (first the primary base of C, if any, then
    // the non-primary non-virtual direct base classes in declaration
    // orderd, then the non-static data members and unnamed bitfields in
    // declaration order

    scope_entry_t* primary_base_class = NULL;

    if (class_type_is_dynamic(class_type))
    {
        char primary_base_class_is_virtual = 0;
        primary_base_class = 
            cxx_abi_class_type_get_primary_base_class(class_type, 
                    &primary_base_class_is_virtual);

        if (primary_base_class == NULL)
        {
            // c) If C has no primary base class, allocate the virtual table pointer
            // for C at offset zero and set sizeof(C), align(C) and dsize(C) to the
            // appropriate values for a pointer
            layout_info.size = CURRENT_CONFIGURATION(type_environment)->sizeof_pointer;
            layout_info.align = CURRENT_CONFIGURATION(type_environment)->alignof_pointer;
            layout_info.dsize = CURRENT_CONFIGURATION(type_environment)->sizeof_pointer;

        }
        else
        {
            // First primary base class (if any)
            cxx_abi_lay_member_out(class_type,
                    primary_base_class,
                    &layout_info, 
                    /* is_base_class */ 1,
                    // No need to set the offset since it is 0
                    /* num_base_class */ -1);
        }
    }

    // Non primary non virtual direct bases
    {
        int num_bases = class_type_get_num_bases(class_type);
        int i;

        for (i = 0; i < num_bases; i++)
        {
            char is_virtual;
            scope_entry_t* base_class = class_type_get_base_num(class_type, i, &is_virtual);

            if (!is_virtual
                    && primary_base_class != base_class)
            {
                cxx_abi_lay_member_out(class_type,
                        base_class,
                        &layout_info, 
                        /* is_base_class */ 1,
                        /* num_base_class */ i);

                // class_type_set_offset_base_num(class_type, i, 
            }
        }
    }

    // Non static data members and unnamed bitfields
    {
        // Nonstatic data members
        int i;
        int num_nonstatic_data_members 
            = class_type_get_num_nonstatic_data_members(class_type);

        for (i = 0; i < num_nonstatic_data_members; i++)
        {
            scope_entry_t* nonstatic_data_member 
                = class_type_get_nonstatic_data_member_num(class_type, i);

            cxx_abi_lay_member_out(class_type,
                    nonstatic_data_member,
                    &layout_info, 
                    /* is_base_class */ 0,
                    /* num_base_class */ -1);
        }
    }

    // After all such components have been allocated set nvalign(C) = align(C)
    layout_info.nvalign = layout_info.align;
    layout_info.nvsize = layout_info.size;

    {
        // Virtual bases allocation
        int num_bases = 0;
        base_info_preorder_t base_info[MAX_BASES];

        class_type_preorder_all_bases_rec(class_type, base_info, &num_bases);

        // Get all the indirect primary bases since we won'class_type allocate these here
        int num_primaries = 0;
        base_info_preorder_t indirect_primary_bases[MAX_BASES];

        class_type_get_indirect_virtual_primary_bases(base_info, num_bases,
                // indirect primary bases will be stored in 'indirect_primary_bases'
                indirect_primary_bases, &num_primaries);

        int i;
        for (i = 0; i < num_bases; i++)
        {
            if (base_info[i].is_virtual)
            {
                char found = 0;
                int j;
                for (j = 0; (j < num_primaries) && !found; j++)
                {
                    if ((base_info[i].entry == indirect_primary_bases[i].entry)
                            || (base_info[i].entry == primary_base_class))
                        found = 1;
                }

                // It is not an indirect primary base, allocate it
                if (!found)
                {
                    cxx_abi_lay_member_out(class_type, base_info[i].entry,
                            &layout_info, 
                            /* is_base_class */ 1,
                            // Do not set the offset of this class since it might not be a direct one
                            /* num_base_class */ -1);
                }
            }
        }
    }

    // dsize
    type_set_data_size(class_type, layout_info.size);

    // Finalization, round sizeof(C) up to a non zero multiple of align(C)
    next_offset_with_align(&layout_info.size, layout_info.align);

    if (layout_info.size == 0)
        layout_info.size = 1;

    // If it is POD, but not for the purpose of layout
    // set nvsize(C) = sizeof(C)
    if (is_pod_type(class_type)
            && !is_pod_type_layout(class_type))
    {
        layout_info.nvsize = layout_info.size;
    }

    // sizeof
    type_set_size(class_type, layout_info.size);
    // align
    type_set_alignment(class_type, layout_info.align);

    // nvsize
    class_type_set_non_virtual_size(class_type, layout_info.nvsize);
    // nvalign
    class_type_set_non_virtual_align(class_type, layout_info.nvalign);

    // Valid size
    type_set_valid_size(class_type, 1);

    // cxx_abi_print_layout(&layout_info);
}


static void cxx_abi_generic_sizeof(type_t* t)
{
    // FIXME - Named types like in the case below are not well handled
    // typedef __attribute__((aligned(16))) int T;
    if (is_array_type(t))
    {
        cxx_abi_array_sizeof(t);

        // This is like a POD
        type_set_data_size(t, type_get_size(t));
    }
    else if (is_lvalue_reference_type(t)
            || is_rvalue_reference_type(t))
    {
        // Use the referenced type
        type_t* referenced_type = reference_type_get_referenced_type(t);

        // align
        type_set_alignment(t, type_get_alignment(referenced_type));
        // sizeof
        type_set_size(t, type_get_size(referenced_type));
        // dsize
        type_set_data_size(t, type_get_size(referenced_type));
        // Valid size
        type_set_valid_size(t, 1);
    }
    else if (is_named_class_type(t))
    {
        type_t* class_type = get_actual_class_type(t);

        // Use the underlying class type
        cxx_abi_generic_sizeof(class_type);

        // And propagate
        type_set_size(t, type_get_size(class_type));
        type_set_data_size(t, type_get_data_size(class_type));
        type_set_alignment(t, type_get_alignment(class_type));

        type_set_valid_size(t, 1);
    }
    // Unnamed from now
    else if (is_union_type(t))
    {
        cxx_abi_union_sizeof(t);

        // This should be a POD all the time
        type_set_data_size(t, type_get_size(t));
        // nvsize
        class_type_set_non_virtual_size(t, type_get_size(t));
        // nvalign
        class_type_set_non_virtual_align(t, type_get_alignment(t));
    }
    else if (is_class_type(t))
    {
        cxx_abi_class_sizeof(t);
    }
    else
    {
        internal_error("Unhandled type size '%s'", print_declarator(t));
    }
}

void generic_system_v_sizeof(type_t* t)
{
    C_LANGUAGE()
    {
        system_v_generic_sizeof(t);
        return;
    }

    CXX_LANGUAGE()
    {
        cxx_abi_generic_sizeof(t);
        return;
    }

    internal_error("Unreachable code", 0);
}

/*
   Specific architecture environment definitions and routines
 */
// Linux IA-32
// Nothing is aligned more than 4 here
static type_environment_t type_environment_linux_ia32_ = 
{
    .environ_name = "Linux IA32",

    // '_Bool' in C99
    // 'bool' in C++
    .sizeof_bool = 1,
    .alignof_bool = 1,

    // ?? Check this one with itanium abi
    .sizeof_wchar_t = 2,
    .alignof_wchar_t = 2,

    .sizeof_unsigned_short = 2,
    .alignof_unsigned_short = 2,

    .sizeof_signed_short = 2,
    .alignof_signed_short = 2,

    .sizeof_unsigned_int = 4,
    .alignof_unsigned_int = 4,

    .sizeof_signed_int = 4,
    .alignof_signed_int = 4,

    .sizeof_unsigned_long = 4,
    .alignof_unsigned_long = 4,

    .sizeof_signed_long = 4,
    .alignof_signed_long = 4,

    .sizeof_unsigned_long_long = 8,
    .alignof_unsigned_long_long = 4,

    .sizeof_signed_long_long = 8,
    .alignof_signed_long_long = 4, 

    .sizeof_float = 4,
    .alignof_float = 4,

    .sizeof_double = 8,
    .alignof_double = 4,

    .sizeof_long_double = 12,
    .alignof_long_double = 4,

    .sizeof_pointer = 4,
    .alignof_pointer = 4,

    // One 'ptrdiff_t'
    .sizeof_pointer_to_data_member = 4,
    .alignof_pointer_to_data_member = 4,

    .sizeof_function_pointer = 4,
    .alignof_function_pointer = 4,

    // Two 'ptrdiff_t'
    .sizeof_pointer_to_member_function = 8,
    .alignof_pointer_to_member_function = 4,

    // Valid both for C and C++
    .compute_sizeof = generic_system_v_sizeof,

    .type_of_sizeof = get_unsigned_int_type,

    // In IA32 'char' == 'signed char'
    .char_type = get_signed_char_type
};

type_environment_t* type_environment_linux_ia32 = &type_environment_linux_ia32_;