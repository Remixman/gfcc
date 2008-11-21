/*
    SMP superscalar Compiler
    Copyright (C) 2008 Barcelona Supercomputing Center

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef TL_PARAMETER_REGION_LIST_HPP
#define TL_PARAMETER_REGION_LIST_HPP

#include <cstddef>

#include "tl-object.hpp"

#include "tl-region-list.hpp"


namespace TL {
	class ParameterRegionList : public Object
	{
		private:
			ObjectList<RegionList> *_list;
			
		public:
			typedef ObjectList<RegionList>::iterator iterator;
			typedef ObjectList<RegionList>::const_iterator const_iterator;
			
			ParameterRegionList()
				: Object(), _list(NULL)
			{
			}
			
			ParameterRegionList(RefPtr<Object> object)
			{
				RefPtr<ParameterRegionList> parameter_region_list = RefPtr<ParameterRegionList>::cast_dynamic(object);
				if (parameter_region_list.get_pointer() != NULL)
				{
					_list = parameter_region_list->_list;
				}
				else
				{
					if (typeid(*object.get_pointer()) != typeid(Undefined))
					{
						std::cerr << "Bad initialization for ParameterRegionList" << std::endl;
					}
					_list = NULL;
				}
			}
			
			ParameterRegionList(ParameterRegionList const &other)
				: Object(other), _list(other._list)
			{
			}
			
			~ParameterRegionList()
			{
			}
			
			void initialize()
			{
				_list = new ObjectList<RegionList>();
			}
			
			ObjectList<RegionList>::const_iterator begin() const
			{
				return _list->begin();
			}
			
			ObjectList<RegionList>::iterator begin()
			{
				return _list->begin();
			}
			
			ObjectList<RegionList>::const_iterator end() const
			{
				return _list->end();
			}
			
			RegionList const &operator[](int index) const
			{
				return (*_list)[index];
			}
			
			RegionList &operator[](int index)
			{
				return (*_list)[index];
			}
			
			size_t size() const
			{
				return (*_list).size();
			}
			
			void add(RegionList const &region)
			{
				if (_list == NULL)
				{
					initialize();
				}
				(*_list).push_back(region);
			}
			
			bool operator==(ParameterRegionList const &other) const
			{
				if ((_list == NULL) != (other._list == NULL))
				{
					return false;
				}
				
				if (_list->size() != other._list->size())
				{
					return false;
				}
				
				for (unsigned int i=0; i < _list->size(); i++)
				{
					if ((*_list)[i] != (*other._list)[i])
					{
						return false;
					}
				}
				
				return true;
			}
			
			void reserve(size_t size)
			{
				_list->reserve(size);
				// Looks like reserve does not actually create default objects
				for (unsigned int i = 0 ; i < size; i++)
				{
					_list->push_back(RegionList());
				}
			}
			
	};
}


#endif // TL_PARAMETER_REGION_LIST_HPP