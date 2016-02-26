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
# $Id: Makefile,v 1.5 2013/06/12 17:23:54 achowdhu Exp $
#
# This is the Makefile for building EPP C++ Implementation
#
# Makefile for directory . (test)

TOP=..
DIRS=
include $(TOP)/src/Makefile.incl
include $(TOP)/src/Makefile.plat

HDRS=\

SRCS=\
	EppTestCore.cpp		\
	EppTestCommand.cpp	\
	EppTestResponse.cpp	\
	EppTestSSL.cpp	\
	EppTestServer.cpp	\
	EppTestSecDns.cpp	\
	EppTestIDN.cpp 		\
	EppTestFee.cpp 		\
	EppTestChangePoll.cpp 		\
	EppTestCN.cpp 		\

OBJS=\
	EppTestCore.$(O)	\
	EppTestCommand.$(O)	\
	EppTestResponse.$(O)	\
	EppTestSSL.$(O)	\
	EppTestServer.$(O)	\
	EppTestSecDns.$(O)	\
	EppTestIDN.$(O) 	\
	EppTestFee.$(O) 	\
	EppTestChangePoll.$(O) 	\
	EppTestCN.$(O) 	\

BINS=\
	EppTestCore$(EXE)	\
	EppTestCommand$(EXE)	\
	EppTestResponse$(EXE)	\
	EppTestServerTcp$(EXE)	\
	EppTestSSL$(EXE)	\
	EppTestSecDns$(EXE)	\
	EppTestIDN$(EXE) \
	EppTestFee$(EXE) \
	EppTestChangePoll$(EXE) \
	EppTestCN$(EXE) \

TESTS=\
	EppTestCore	\
	EppTestCommand	\
	EppTestResponse	\
	EppTestSSL	\
	EppTestSecDns	\
	EppTestXriCommand \
	EppTestIDN \
	EppTestFee \
	EppTestChangePoll \
	EppTestCN \

LIBNAME=libepp-c
THISLIB=${EPP_LIB_DIR}/${LIBNAME}${PRODUCTVERSION}${SHLIBSUFFIX}
LIBS=${THISLIB} ${EPPAPI_LIB} ${XERCES_LIB} ${OPENSSL_LIB} -ldl

TARGETS=$(BINS)

include $(TOP)/src/Makefile.incl

ifeq (${PLATFORM}, win32)
DEBUG_OPT = /Zi /Od
DDLS=$(XERCES_C_DLL) $(OPENSSL_DDL)
else
DEBUG_OPT = -g
DDLS=
endif

XSDS = epp-1.0.xsd eppcom-1.0.xsd contact-1.0.xsd domain-1.0.xsd host-1.0.xsd svcsub-1.0.xsd idn-1.0.xsd

#
# Common rules (test only)
#

ifeq (${PLATFORM}, solaris)
CC2 =	$(CC1) $(DEBUG_OPT) -lpthread
else
CC2 =	$(CC1) $(DEBUG_OPT)
endif

clean::
	${RM} -rf *.$(O) *.xsd xml ${BINS} *.pdb *.ilk

EppTestCore$(EXE): EppTestCore.$(O)
	$(CC2) -o $(@) $(<) ${ALLLIBS}

EppTestCommand$(EXE): EppTestCommand.$(O)
	$(CC2) -o $(@) $(<) ${ALLLIBS}

EppTestResponse$(EXE): EppTestResponse.$(O)
	$(CC2) -o $(@) $(<) ${ALLLIBS}

EppTestSSL$(EXE): EppTestSSL.$(O)
	$(CC2) -o $(@) $(<) ${ALLLIBS}

EppTestServerTcp$(EXE): EppTestServerTcp.$(O)
	$(CC2) -o $(@) $(<) ${ALLLIBS}

EppTestSecDns$(EXE): EppTestSecDns.$(O)
	$(CC2) -o $(@) $(<) ${ALLLIBS}

EppTestIDN$(EXE): EppTestIDN.$(O)
	$(CC2) -o $(@) $(<) ${ALLLIBS}

EppTestFee$(EXE): EppTestFee.$(O)
	$(CC2) -o $(@) $(<) ${ALLLIBS}

EppTestChangePoll$(EXE): EppTestChangePoll.$(O)
	$(CC2) -o $(@) $(<) ${ALLLIBS}

EppTestCN$(EXE): EppTestCN.$(O)
	$(CC2) -o $(@) $(<) ${ALLLIBS}

#
# Copy EPP Schema files from ../xsd
#
epp-1.0.xsd:: ../xsd/epp-1.0.xsd
	${CP} -p $< $@

eppcom-1.0.xsd:: ../xsd/eppcom-1.0.xsd
	${CP} -p $< $@

contact-1.0.xsd:: ../xsd/contact-1.0.xsd
	${CP} -p $< $@

domain-1.0.xsd:: ../xsd/domain-1.0.xsd
	${CP} -p $< $@

host-1.0.xsd:: ../xsd/host-1.0.xsd
	${CP} -p $< $@

svcsub-1.0.xsd:: ../xsd/svcsub-1.0.xsd
	${CP} -p $< $@

idn-1.0.xsd:: ../xsd/idn-1.0.xsd
	${CP} -p $< $@

epp.xml::
	${RM} -rf xml
	${CP} -pr ../xsd/xml .
#
# Test all
#
testall:	$(TARGETS) $(XSDS)
	echo "Test all..."
	@for i in $(TESTS) ; do \
		echo "Performing $$i ..." ; \
		./$$i ; \
	done
