##########################################################################
#  (C) Copyright 2006-2011 Barcelona Supercomputing Center               #
#                          Centro Nacional de Supercomputacion           #
#                                                                        #
#  This file is part of Mercurium C/C++ source-to-source compiler.       #
#                                                                        #
#  See AUTHORS file in the top level directory for information           #
#  regarding developers and contributors.                                #
#                                                                        #
#  This library is free software; you can redistribute it and/or         #
#  modify it under the terms of the GNU Lesser General Public            #
#  License as published by the Free Software Foundation; either          #
#  version 3 of the License, or (at your option) any later version.      #
#                                                                        #
#  Mercurium C/C++ source-to-source compiler is distributed in the hope  #
#  that it will be useful, but WITHOUT ANY WARRANTY; without even the    #
#  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR       #
#  PURPOSE.  See the GNU Lesser General Public License for more          #
#  details.                                                              #
#                                                                        #
#  You should have received a copy of the GNU Lesser General Public      #
#  License along with Mercurium C/C++ source-to-source compiler; if      #
#  not, write to the Free Software Foundation, Inc., 675 Mass Ave,       #
#  Cambridge, MA 02139, USA.                                             #
##########################################################################

lib_LTLIBRARIES = libtl-gfn.la 
phases_LTLIBRARIES = libtl-gfn-pragma.la

include ../Makefile.phases

END=
 
COMMON_CFLAGS=-DLIBGFN_DLL_EXPORT -I $(srcdir)/../omp/core \
-I /usr/lib/openmpi/include \
-I /usr/include

libtl_gfn_la_CFLAGS=$(tl_cflags) $(COMMON_CFLAGS) 
libtl_gfn_la_CXXFLAGS=$(tl_cxxflags) $(COMMON_CFLAGS)
libtl_gfn_la_LIBADD=$(tl_libadd) 
libtl_gfn_la_LDFLAGS=$(tl_ldflags)

libtl_gfn_la_SOURCES = \
      gfn-common.hpp \
      gfn-common.cpp \
      gfn-config.hpp \
      gfn-config.cpp \
      gfn-kernel_core.hpp \
      gfn-kernel_core.cpp \
      gfn-transform.hpp \
      gfn-transform.cpp \
      gfn-exception.hpp \
      gfn-exception.cpp \
      gfn-atomic.hpp \
      gfn-atomic.cpp \
      gfn-parallel_for.hpp \
      gfn-parallel_for.cpp \
      gfn-data.hpp \
      gfn-data.cpp \
     $(END)

libtl_gfn_pragma_la_CFLAGS=$(tl_cflags) $(COMMON_CFLAGS)
libtl_gfn_pragma_la_CXXFLAGS=$(tl_cxxflags) $(COMMON_CFLAGS)
libtl_gfn_pragma_la_LIBADD=$(tl_libadd) libtl-gfn.la
libtl_gfn_pragma_la_LDFLAGS=$(tl_ldflags) 

libtl_gfn_pragma_la_SOURCES = \
      gfn-pragma.cpp \
      gfn-pragma.hpp \
      $(END)
