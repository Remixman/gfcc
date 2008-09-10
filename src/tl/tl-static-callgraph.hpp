#ifndef TL_STATIC_CFG_HPP
#define TL_STATIC_CFG_HPP

#include "tl-object.hpp"
#include "tl-objectlist.hpp"
#include "tl-ast.hpp"
#include "tl-scopelink.hpp"
#include "tl-symbol.hpp"
#include "tl-langconstruct.hpp"
#include "tl-predicate.hpp"
#include <map>
#include <utility>

namespace TL
{
    class StaticCallGraph : Object
    {
        private:
            ObjectList<Symbol> _explicitly_called;
            ObjectList<Symbol> _implicitly_called;
            typedef std::pair<ObjectList<Symbol>, ObjectList<Symbol> > explicit_implicit_pair_t;
            std::map<Symbol, explicit_implicit_pair_t> _call_map;

            void empty_data();

            void compute_call_graph(AST_t a, 
                    ObjectList<Symbol> &explicitly_called,
                    ObjectList<Symbol> &implicitly_called);
        public:
            StaticCallGraph() { }

            void compute_global(AST_t translation_unit, ScopeLink scope_link);

            void compute_statement(Statement stmt);
            void compute_expression(Expression expr);

            ObjectList<Symbol> get_all_called_functions();
            ObjectList<Symbol> get_all_explicitly_called_functions();
            ObjectList<Symbol> get_all_implicitly_called_functions();

            ObjectList<Symbol> get_all_called_functions_from(Symbol function);
            ObjectList<Symbol> get_all_explicitly_called_functions_from(Symbol function);
            ObjectList<Symbol> get_all_implicitly_called_functions_from(Symbol function);

            ObjectList<Symbol> get_all_functions_calling_to(Symbol function);
            ObjectList<Symbol> get_all_functions_calling_explicitly_to(Symbol function);
            ObjectList<Symbol> get_all_functions_calling_implicitly_to(Symbol function);
    };
}

#endif