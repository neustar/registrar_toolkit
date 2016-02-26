#******************************************************************************
# The MIT License (MIT)
#  
# Copyright (c) 2015 Neustar Inc.
#  
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#  
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#  
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#******************************************************************************
#
# $Id: Makefile,v 1.4 2007/08/01 23:20:03 wtan Exp $
#
# This is the Makefile for building EPP C++ Implementation
#
# Top Makefile

TOP=.
DIRS=
include $(TOP)/src/Makefile.plat
TARGETS=compile_src lib test docs package
include $(TOP)/src/Makefile.incl

package:: package_bin package_src

compile_src::
	@echo Building Source
	${MAKE} -C src

lib::
	${MAKE} -C src lib

docs::
	@echo Building apiDocs in docs directory.
	${MKDIR} docs/apiDocs
	${MAKE} -C src apiDocs
	tar --exclude CVS -zcf Epp-C++-doc.${PRODUCTVERSION_DOT}.tar.gz docs
	zip -r Epp-C++-doc.${PRODUCTVERSION_DOT}.zip docs -x \*CVS\*

Epp-C++-doc.${PRODUCTVERSION_DOT}.zip::
	zip -r Epp-C++-doc.${PRODUCTVERSION_DOT}.zip docs -x \*CVS\*

Epp-C++-doc.${PRODUCTVERSION_DOT}.tar.gz::
	tar --exclude CVS -zcf Epp-C++-doc.${PRODUCTVERSION_DOT}.tar.gz docs

dist:: Epp-C++-doc.${PRODUCTVERSION_DOT}.zip Epp-C++-doc.${PRODUCTVERSION_DOT}.tar.gz ${BINTARGETDIR}.zip ${BINTARGETDIR}.tar.gz
	cp Epp-C++-doc.${PRODUCTVERSION_DOT}.{tar.gz,zip} ../../doc/
	cp Epp-C++-doc.${PRODUCTVERSION_DOT}.{tar.gz,zip} ../../bin/c++/
	cp ${BINTARGETDIR}.{tar.gz,zip} ../../bin/c++/


package_bin:: 
	@echo Building the binary release package
	${MAKE} -C src package_bin

package_src::
	@echo Building the source release package
	${MAKE} -C src package_src

test:: compile_src
	${MAKE} -C test

clean::
	${MAKE} -C src clean
	${MAKE} -C test clean
	${RM} -rf bin include source docs/apiDocs docs/html lib/libepp-c$(PRODUCTVERSION)*
	${RM} -rf $(EPP_OBJ_DIR) $(BINTARGETDIR) $(BINTARGETDIR).*
	${RM} -f Epp-C++-doc.${PRODUCTVERSION_DOT}.{tar.gz,zip}
	find . -name '*~' -exec rm \{\} \;
	find . -name 'Template*.DB' -exec rm -rf \{\} \;
