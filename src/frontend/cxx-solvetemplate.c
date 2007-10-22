/*
    Mercurium C/C++ Compiler
    Copyright (C) 2006-2007 - Roger Ferrer Ibanez <roger.ferrer@bsc.es>
    Barcelona Supercomputing Center - Centro Nacional de Supercomputacion
    Universitat Politecnica de Catalunya

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <stdio.h>
#include <stdlib.h>
#include "cxx-utils.h"
#include "cxx-cexpr.h"
#include "cxx-scope.h"
#include "cxx-solvetemplate.h"
#include "cxx-typeunif.h"
#include "cxx-typeutils.h"
#include "cxx-prettyprint.h"
#include "cxx-driver.h"
#include "cxx-instantiation.h"

static matching_pair_t* determine_more_specialized(int num_matching_set, matching_pair_t** matching_set, 
        char give_exact_match, decl_context_t decl_context);

// static
// char *template_nature_name[] =
// {
//     [TPN_UNKNOWN] = "TPN_UNKNOWN",
//     [TPN_COMPLETE_DEPENDENT] =  "TPN_COMPLETE_DEPENDENT",
//     [TPN_INCOMPLETE_INDEPENDENT] =  "TPN_INCOMPLETE_INDEPENDENT",
//     [TPN_INCOMPLETE_DEPENDENT] =  "TPN_INCOMPLETE_DEPENDENT",
//     [TPN_COMPLETE_INDEPENDENT] =  "TPN_COMPLETE_INDEPENDENT",
// };

static const char* get_template_nature_name(type_t* t)
{
    if (class_type_is_incomplete_dependent(t))
    {
        return "TPN_INCOMPLETE_DEPENDENT";
    }
    else if (class_type_is_incomplete_independent(t))
    {
        return "TPN_INCOMPLETE_INDEPENDENT";
    }
    else if (class_type_is_complete_dependent(t))
    {
        return "TPN_COMPLETE_DEPENDENT";
    }
    else if (class_type_is_complete_independent(t))
    {
        return "TPN_COMPLETE_INDEPENDENT";
    }
    else
    {
        return "TPN_UNKNOWN";
    }
}

matching_pair_t* solve_template(decl_context_t decl_context,
        scope_entry_list_t* candidate_templates, 
        template_argument_list_t* arguments, 
        char give_exact_match)
{
    DEBUG_CODE()
    {
        fprintf(stderr, "Solving template.\n - Candidate list:\n");
        scope_entry_list_t* it = candidate_templates;
        while (it != NULL)
        {
            scope_entry_t* entry = it->entry;
            fprintf(stderr, "   * Symbol : '%s' Place: '%s:%d' Status : %s\n", 
                    entry->symbol_name, 
                    entry->file,
                    entry->line,
                    get_template_nature_name(entry->type_information));
            it = it->next;
        }
        fprintf(stderr, " - End of candidate list\n");
    }
    matching_pair_t* result = NULL;

    // In the worst of the cases the chosen template will be the primary one
    scope_entry_list_t* iter = candidate_templates;

    if (iter == NULL)
    {
        internal_error("No templates were given to solve the current one", 0);
    }

    char seen_primary_template = 0;
    while (iter != NULL && !seen_primary_template)
    {
        seen_primary_template |= ((iter->entry->kind == SK_TEMPLATE_PRIMARY_CLASS)
                || (iter->entry->kind == SK_TEMPLATE_TEMPLATE_PARAMETER));
        iter = iter->next;
    }

    if (!seen_primary_template)
    {
        internal_error("No primary template was among the candidates", 0);
    }

    // Now, for every specialization try to unificate its template_argument_list with ours
    iter = candidate_templates;

    int num_matching_set = 0;

    matching_pair_t** matching_set = NULL;

    while (iter != NULL)
    {
        scope_entry_t* entry = iter->entry;

        template_argument_list_t* specialized = template_type_get_template_arguments(entry->type_information);

        // if (!give_exact_match
        //         && (entry->type_information->type->template_nature == TPN_INCOMPLETE_INDEPENDENT
        //             || entry->type_information->type->template_nature == TPN_INCOMPLETE_DEPENDENT))
        // {
        //     // If the user did not ask for an exact match, incomplete types are not eligible.
        //     iter = iter->next;
        //     continue;
        // }

        // It is supposed that this will hold in correct code
        if (arguments->num_arguments != specialized->num_arguments)
        {
            // internal_error("Template argument lists are not of equal length", 0);
            return NULL;
        }

        unification_set_t* unification_set = calloc(1, sizeof(*unification_set));
        if (match_one_template(arguments, specialized, entry, unification_set, decl_context))
        {
            matching_pair_t* match_pair = calloc(1, sizeof(*match_pair));

            match_pair->entry = entry;
            match_pair->unif_set = unification_set;

            P_LIST_ADD(matching_set, num_matching_set, match_pair);
        }

        iter = iter->next;
    }

    // There is no more than one candidate
    if (num_matching_set == 1)
    {
        result = matching_set[0];
        if (give_exact_match)
        {
            template_argument_list_t* specialized = template_type_get_template_arguments(result->entry->type_information);

            DEBUG_CODE()
            {
                fprintf(stderr, "Checking match with the unique %p %p\n", specialized, arguments);
            }

            unification_set_t* unification_set = calloc(1, sizeof(*unification_set));
            if (!match_one_template(specialized, arguments, NULL, unification_set, decl_context))
            {
                return NULL;
            }
            else
            {
                return result;
            }
        }
        else
        {
            result = matching_set[0];
        }
    }
    else if (num_matching_set > 0)
    {
        DEBUG_CODE()
        {
            fprintf(stderr, "More than one template can be selected, determining more specialized (give_exact_match=%d)\n", give_exact_match);
        }
        result = determine_more_specialized(num_matching_set, matching_set, 
                give_exact_match, decl_context);
        DEBUG_CODE()
        {
            fprintf(stderr, "More specialized determined result=%p (%s:%d)\n", result->entry,
                    result->entry->file,
                    result->entry->line);
        }
    }

    if (give_exact_match 
            && result != NULL)
    {
        // Result will be an exact match if it can be unified with the original
        template_argument_list_t* specialized = template_type_get_template_arguments(result->entry->type_information);

        DEBUG_CODE()
        {
            fprintf(stderr, "Checking match %p %p\n", specialized, arguments);
        }

        unification_set_t* unification_set = calloc(1, sizeof(*unification_set));
        if (!match_one_template(specialized, arguments, NULL, unification_set, decl_context))
        {
            return NULL;
        }
    }

    return result;
}

// This function assumes that only one minimum will exist
static matching_pair_t* determine_more_specialized(int num_matching_set, matching_pair_t** matching_set, 
        char give_exact_match, decl_context_t decl_context)
{
    matching_pair_t* min = matching_set[0];

    DEBUG_CODE()
    {
        fprintf(stderr, "Have to select the best template among %d templates\n", num_matching_set);
    }

    int i;
    for (i = 1; i < num_matching_set; i++)
    {
        matching_pair_t* current_entry = matching_set[i];

        template_argument_list_t* min_args =
            template_type_get_template_arguments(min->entry->type_information);
        template_argument_list_t* current_args =
            template_type_get_template_arguments(current_entry->entry->type_information);

        unification_set_t* unification_set = calloc(1, sizeof(*unification_set));

        DEBUG_CODE()
        {
            fprintf(stderr, "Comparing current best template %p (line %d) with %p (line %d)\n",
                    min->entry,
                    min->entry->line,
                    current_entry->entry,
                    current_entry->entry->line);
        }

        if (!match_one_template(min_args, current_args, current_entry->entry, 
                    unification_set, decl_context))
        {
            DEBUG_CODE()
            {
                fprintf(stderr, "Template %p (line %d) is more specialized than %p (line %d)\n", 
                        current_entry->entry, 
                        current_entry->entry->line,
                        min->entry,
                        min->entry->line);
            }

            min = current_entry;

            unification_set_t* unification_set_check = calloc(1, sizeof(*unification_set_check));
            // This is not correct because of cv-qualifiers 'const' and
            // 'volatile' not having an order between them. So we get that
            // "volatile T" and "const T" are both [temporarily] valid for
            // "const volatile T", even if there is a third "const volatile T"
            // specialization waiting to be matched
            //
            // We should check there is only one minimum, a way to do it would
            // be creating a topological tree with "less than" relationship and
            // watch for the deeper node, the minimum. Should be two nodes with
            // the same minimum depth, an ambiguity would occur.
            //
            // if (!give_exact_match)
            // {
            //     DEBUG_CODE()
            //     {
            //         fprintf(stderr, "Checking non ambiguous specialization\n");
            //     }
            //     if (!match_one_template(current_args, min_args, min->entry, st, 
            //                 unification_set_check, decl_context))
            //     {
            //         internal_error("Ambiguous specialization instantiation\n", 0);
            //     }
            // }
        }
    }

    DEBUG_CODE()
    {
        fprintf(stderr, "Best template selected\n");
    }

    return min;
}

char match_one_template(template_argument_list_t* arguments, 
        template_argument_list_t* specialized, scope_entry_t* specialized_entry, 
        unification_set_t* unif_set,
        decl_context_t decl_context)
{
    int i;
    // unification_set_t* unif_set = calloc(1, sizeof(*unif_set));
    
    DEBUG_CODE()
    {
        if (specialized_entry != NULL)
        {
            fprintf(stderr, "=== Starting unification with %p (%s:%d)\n", specialized_entry,
                    specialized_entry->file, specialized_entry->line);
        }
        else
        {
            // For reverse unifications
            fprintf(stderr, "=== Starting unification\n");
        }
    }

    for (i = 0; i < arguments->num_arguments; i++)
    {
        template_argument_t* spec_arg = specialized->argument_list[i];
        template_argument_t* arg = arguments->argument_list[i];

        if (spec_arg->kind == arg->kind)
        {
            switch (spec_arg->kind)
            {
                case TAK_TEMPLATE :
                case TAK_TYPE :
                    {
                        DEBUG_CODE()
                        {
                            fprintf(stderr, "=== Unificating types\n");
                        }
                        if (!unificate_two_types(spec_arg->type, arg->type, &unif_set,
                                    decl_context))
                        {
                            DEBUG_CODE()
                            {
                                fprintf(stderr, "=== Unification failed\n");
                            }
                            return 0;
                        }
                        else
                        {
                            DEBUG_CODE()
                            {
                                fprintf(stderr, "=== Types unificated\n");
                            }
                        }
                        break;
                    }
                case TAK_NONTYPE :
                    {
                        DEBUG_CODE()
                        {
                            fprintf(stderr, "==> Unificating expressions\n");
                        }
                        if (spec_arg->expression == NULL)
                        {
                            internal_error("Expected an expression value for specialized argument", 0);
                        }
                        if (arg->expression == NULL)
                        {
                            internal_error("Expected an expression value for argument", 0);
                        }

                        if (!unificate_two_expressions(&unif_set, spec_arg->expression, spec_arg->expression_context, 
                                arg->expression, arg->expression_context))
                        {
                            return 0;
                        }

                        break;
                    }
                default :
                    {
                        internal_error("Unknown template argument type %d", spec_arg->kind);
                    }
            }
        }
        else
        {
            DEBUG_CODE()
            {
                fprintf(stderr, "=== Unification failed\n");
            }
            return 0;
        }
    }

    DEBUG_CODE()
    {
        fprintf(stderr, "=== Unification succeeded!\n");
        fprintf(stderr, "=== Unification details\n");
        for (i = 0; i < unif_set->num_elems; i++)
        {
            unification_item_t* unif_item = unif_set->unif_list[i];
            fprintf(stderr, "(%s) (%d,%d) <- ",
                    unif_item->parameter_name,
                    unif_item->parameter_nesting,
                    unif_item->parameter_position);
            if (unif_item->value != NULL)
            {
                fprintf(stderr, "[type] %s", print_declarator(unif_item->value, decl_context));
            }
            else if (unif_item->expression != NULL)
            {
                fprintf(stderr, "[expr] %s", prettyprint_in_buffer(unif_item->expression));
            }
            else
            {
                fprintf(stderr, "(unknown)");
            }
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "=== End of unification details\n");
    }

    return 1;
}

scope_entry_t* get_specialization_of_template(decl_context_t template_name_context, 
        char *template_name, template_argument_list_t* template_arguments,
        int line, char *filename)
{
    /* Now get a list of candidates */
    scope_entry_list_t* candidate_templates = query_unqualified_name_str(
            template_name_context,
            template_name);

    char arguments_are_dependent = 0;
    int i;
    for (i = 0; i < template_arguments->num_arguments; i++)
    {
        template_argument_t* template_argument = template_arguments->argument_list[i];

        template_argument_t* updated_template_argument = calloc(1, sizeof(*updated_template_argument));

        switch (template_argument->kind)
        {
            case TAK_TEMPLATE :
            case TAK_TYPE :
                {
                    arguments_are_dependent |= is_dependent_type(template_argument->type, template_name_context);
                }
                break;
            case TAK_NONTYPE:
                {
                    arguments_are_dependent |= is_dependent_expression(template_argument->expression, template_argument->expression_context);
                }
                break;
            default:
                break;
        }
    }

    ERROR_CONDITION(candidate_templates == NULL, "Candidate templates not found!", 0);

    matching_pair_t* match_pair = solve_template(template_name_context,
            candidate_templates,
            template_arguments,
            /* give_exact_match */ 0);

    ERROR_CONDITION(match_pair == NULL, "Matching pair not found", 0);

    scope_entry_t *result = match_pair->entry;

    if (!arguments_are_dependent
            && !class_type_is_complete_independent(result->type_information)
            && !class_type_is_incomplete_independent(result->type_information))
    {
        // Create a holding symbol only if all are nondependent
        result = create_holding_symbol_for_template(match_pair, template_arguments, line, filename, template_name_context);
    }
    return result;
}