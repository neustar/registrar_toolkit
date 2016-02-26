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
# $Id: Makefile,v 1.4 2007/08/01 23:20:05 wtan Exp $
#
# This is the Makefile for building EPP Java Implementation
#
# Makefile for directory . (src)

TARGETS =
DIRS = com
TOP = ..
include $(TOP)/src/Makefile.incl

apidocs: ../docs/apiDocs/index.html

../docs/apiDocs/index.html: classfiles_updated
	$(JAVADOC) -classpath "$(CLPATH)" \
		-windowtitle "EPP ${PRODUCTVERSION_DOT} API" \
		-splitindex -header "EPP ${PRODUCTVERSION_DOT}" \
		-bottom "Copyright &copy; 2001-2006 NeuStar, Inc. All Rights Reserved." \
		-doctitle "<h2>NeuStar EPP-1.0 Java API, Version ${PRODUCTVERSION_DOT}</h2>" \
		-version -author \
		-d ../docs/apiDocs ${PACKAGES}

clean::
	${RM} classfiles_updated

jars: source

source: ../bin/${JARFILE} ../bin/${JARFILE}.md5

../bin/${JARFILE}.md5: ../bin/${JARFILE}
	md5sum ${<} > ${@}

../bin/${JARFILE}: classfiles_updated
#
# Copy the .class under src
#
# src/com/neulevel/epp/core
#
	${MKDIR} ../class/com/neulevel/epp/core
	${CP}      ../src/com/neulevel/epp/core/*.class \
	         ../class/com/neulevel/epp/core
#
# src/com/neulevel/epp/core/command
#
	${MKDIR} ../class/com/neulevel/epp/core/command
	${CP}      ../src/com/neulevel/epp/core/command/*.class \
	         ../class/com/neulevel/epp/core/command
#
# src/com/neulevel/epp/core/response
#
	${MKDIR} ../class/com/neulevel/epp/core/response
	${CP}      ../src/com/neulevel/epp/core/response/*.class \
	         ../class/com/neulevel/epp/core/response
#
# src/com/neulevel/epp/ext
#
	${MKDIR} ../class/com/neulevel/epp/ext
	${CP}      ../src/com/neulevel/epp/ext/*.class \
	         ../class/com/neulevel/epp/ext
#
# src/com/neulevel/epp/ext/command
#
	${MKDIR} ../class/com/neulevel/epp/ext/command
	${CP}      ../src/com/neulevel/epp/ext/command/*.class \
	         ../class/com/neulevel/epp/ext/command
#
# src/com/neulevel/epp/ext/response
#
	${MKDIR} ../class/com/neulevel/epp/ext/response
	${CP}      ../src/com/neulevel/epp/ext/response/*.class \
	         ../class/com/neulevel/epp/ext/response
#
# src/com/neulevel/epp/ext/fee
#
	${MKDIR} ../class/com/neulevel/epp/ext/fee
	${CP}      ../src/com/neulevel/epp/ext/fee/*.class \
                 ../class/com/neulevel/epp/ext/fee
#
# src/com/neulevel/epp/ext/command
#
	${MKDIR} ../class/com/neulevel/epp/ext/fee/command
	${CP}      ../src/com/neulevel/epp/ext/fee/command/*.class \
                 ../class/com/neulevel/epp/ext/fee/command
#
# src/com/neulevel/epp/ext/fee/response
#
	${MKDIR} ../class/com/neulevel/epp/ext/fee/response
	${CP}      ../src/com/neulevel/epp/ext/fee/response/*.class \
                 ../class/com/neulevel/epp/ext/fee/response

#
# src/com/neulevel/epp/transport
#
	${MKDIR} ../class/com/neulevel/epp/transport
	${CP}      ../src/com/neulevel/epp/transport/*.class \
	         ../class/com/neulevel/epp/transport
#
# src/com/neulevel/epp/transport/beep
#
#	${MKDIR} ../class/com/neulevel/epp/transport/beep
#	${CP}      ../src/com/neulevel/epp/transport/beep/*.class \
#	         ../class/com/neulevel/epp/transport/beep
#
# src/com/neulevel/epp/transport/tcp
#
	${MKDIR} ../class/com/neulevel/epp/transport/tcp
	${CP}      ../src/com/neulevel/epp/transport/tcp/*.class \
	         ../class/com/neulevel/epp/transport/tcp
#
# src/com/neulevel/epp/xri
#
	${MKDIR} ../class/com/neulevel/epp/xri
	${CP}      ../src/com/neulevel/epp/xri/*.class \
	         ../class/com/neulevel/epp/xri
#
# src/com/neulevel/epp/xri/command
#
	${MKDIR} ../class/com/neulevel/epp/xri/command
	${CP}      ../src/com/neulevel/epp/xri/command/*.class \
	         ../class/com/neulevel/epp/xri/command
#
# src/com/neulevel/epp/xri/response
#
	${MKDIR} ../class/com/neulevel/epp/xri/response
	${CP}      ../src/com/neulevel/epp/xri/response/*.class \
	         ../class/com/neulevel/epp/xri/response
#
# src/com/neulevel/biz
#
	${MKDIR} ../class/com/neulevel/biz
	${CP}      ../src/com/neulevel/biz/*.class \
	         ../class/com/neulevel/biz
#
# src/com/neulevel/us
#
	${MKDIR} ../class/com/neulevel/us
	${CP}      ../src/com/neulevel/us/*.class \
	         ../class/com/neulevel/us
#
# src/com/neulevel/cn
#
	${MKDIR} ../class/com/neulevel/cn
	${CP}      ../src/com/neulevel/cn/*.class \
		../class/com/neulevel/cn
#
# Make the jar file
#
	echo ${PRODUCTVERSION} > ../class/Epp_Ver_${PRODUCTIONVESION}.info
	cd ../class; $(JAR) c0vf ../bin/${JARFILE} *.info com; cd ../src

package_src:
#
# Copy the .java/.html files under src
#
# src/com/neulevel/epp/core
#
	${MKDIR} ../source/src/com/neulevel/epp/core
	${CP}           ../src/com/neulevel/epp/core/*.{java,html} \
	         ../source/src/com/neulevel/epp/core
#
# src/com/neulevel/epp/core/command
#
	${MKDIR} ../source/src/com/neulevel/epp/core/command
	${CP}           ../src/com/neulevel/epp/core/command/*.{java,html} \
	         ../source/src/com/neulevel/epp/core/command
#
# src/com/neulevel/epp/core/response
#
	${MKDIR} ../source/src/com/neulevel/epp/core/response
	${CP}           ../src/com/neulevel/epp/core/response/*.{java,html} \
	         ../source/src/com/neulevel/epp/core/response
#
# src/com/neulevel/epp/ext
#
	${MKDIR} ../source/src/com/neulevel/epp/ext
	${CP}           ../src/com/neulevel/epp/ext/*.{java,html} \
	         ../source/src/com/neulevel/epp/ext
#
# src/com/neulevel/epp/ext/command
#
	${MKDIR} ../source/src/com/neulevel/epp/ext/command
	${CP}           ../src/com/neulevel/epp/ext/command/*.{java,html} \
	         ../source/src/com/neulevel/epp/ext/command
#
# src/com/neulevel/epp/ext/response
#
	${MKDIR} ../source/src/com/neulevel/epp/ext/response
	${CP}           ../src/com/neulevel/epp/ext/response/*.{java,html} \
	         ../source/src/com/neulevel/epp/ext/response
#
# src/com/neulevel/epp/ext/fee
#
	${MKDIR} ../source/src/com/neulevel/epp/ext/fee
	${CP}           ../src/com/neulevel/epp/ext/fee/*.{java,html} \
                 ../source/src/com/neulevel/epp/ext/fee
#
# src/com/neulevel/epp/ext/fee/command
#
	${MKDIR} ../source/src/com/neulevel/epp/ext/fee/command
	${CP}           ../src/com/neulevel/epp/ext/fee/command/*.{java,html} \
                 ../source/src/com/neulevel/epp/ext/fee/command
#
# src/com/neulevel/epp/ext/fee/response
#
	${MKDIR} ../source/src/com/neulevel/epp/ext/fee/response
	${CP}           ../src/com/neulevel/epp/ext/fee/response/*.{java,html} \
                 ../source/src/com/neulevel/epp/ext/fee/response
#
# src/com/neulevel/epp/transport
#
	${MKDIR} ../source/src/com/neulevel/epp/transport
	${CP}           ../src/com/neulevel/epp/transport/*.{java,html} \
	         ../source/src/com/neulevel/epp/transport
#
# src/com/neulevel/epp/transport/beep
#
#	${MKDIR} ../source/src/com/neulevel/epp/transport/beep
#	${CP}           ../src/com/neulevel/epp/transport/beep/*.{java,html} \
#	         ../source/src/com/neulevel/epp/transport/beep
#
# src/com/neulevel/epp/transport/tcp
#
	${MKDIR} ../source/src/com/neulevel/epp/transport/tcp
	${CP}           ../src/com/neulevel/epp/transport/tcp/*.{java,html} \
	         ../source/src/com/neulevel/epp/transport/tcp
#
# src/com/neulevel/epp/xri
#
	${MKDIR} ../source/src/com/neulevel/epp/xri
	${CP}           ../src/com/neulevel/epp/xri/*.{java,html} \
	         ../source/src/com/neulevel/epp/xri
#
# src/com/neulevel/epp/xri/command
#
	${MKDIR} ../source/src/com/neulevel/epp/xri/command
	${CP}           ../src/com/neulevel/epp/xri/command/*.{java,html} \
	         ../source/src/com/neulevel/epp/xri/command
#
# src/com/neulevel/epp/xri/response
#
	${MKDIR} ../source/src/com/neulevel/epp/xri/response
	${CP}           ../src/com/neulevel/epp/xri/response/*.{java,html} \
	         ../source/src/com/neulevel/epp/xri/response
#
# src/com/neulevel/biz
#
	${MKDIR} ../source/src/com/neulevel/biz
	${CP}           ../src/com/neulevel/biz/*.{java,html} \
	         ../source/src/com/neulevel/biz
#
# src/com/neulevel/us
#
	${MKDIR} ../source/src/com/neulevel/us
	${CP}           ../src/com/neulevel/us/*.{java,html} \
	         ../source/src/com/neulevel/us
#
# src/com/neulevel/cn
#
	${MKDIR} ../source/src/com/neulevel/cn
	${CP}           ../src/com/neulevel/cn/*.{java,html} \
		../source/src/com/neulevel/cn
#
# Copy the Makefiles under src
#
# src/com/neulevel/epp/core
#
	${CP}     ../src/com/neulevel/epp/core/Makefile \
	   ../source/src/com/neulevel/epp/core
#
# src/com/neulevel/epp/core/command
#
	${CP}     ../src/com/neulevel/epp/core/command/Makefile \
	   ../source/src/com/neulevel/epp/core/command
#
# src/com/neulevel/epp/core/response
#
	${CP}     ../src/com/neulevel/epp/core/response/Makefile \
	   ../source/src/com/neulevel/epp/core/response
#
# src/com/neulevel/epp/transport
#
	${CP}     ../src/com/neulevel/epp/transport/Makefile \
	   ../source/src/com/neulevel/epp/transport
#
# src/com/neulevel/epp/transport/beep
#
#	${CP}     ../src/com/neulevel/epp/transport/beep/Makefile \
#	   ../source/src/com/neulevel/epp/transport/beep
#
# src/com/neulevel/epp/transport/tcp
#
	${CP}     ../src/com/neulevel/epp/transport/tcp/Makefile \
	   ../source/src/com/neulevel/epp/transport/tcp
#
# src/com/neulevel/epp/ext
#
	${CP}     ../src/com/neulevel/epp/ext/Makefile \
	   ../source/src/com/neulevel/epp/ext
#
# src/com/neulevel/epp/ext/command
#
	${CP}     ../src/com/neulevel/epp/ext/command/Makefile \
	   ../source/src/com/neulevel/epp/ext/command
#
# src/com/neulevel/epp/ext/response
#
	${CP}     ../src/com/neulevel/epp/ext/response/Makefile \
	   ../source/src/com/neulevel/epp/ext/response
#
# src/com/neulevel/epp/ext/fee
#
	${CP}     ../src/com/neulevel/epp/ext/fee/Makefile \
	../source/src/com/neulevel/epp/ext/fee/
#
# src/com/neulevel/epp/ext/fee/command
#
	${CP}     ../src/com/neulevel/epp/ext/fee/command/Makefile \
	../source/src/com/neulevel/epp/ext/fee/command
#
# src/com/neulevel/epp/ext/fee/response
#
	${CP}     ../src/com/neulevel/epp/ext/fee/response/Makefile \
	../source/src/com/neulevel/epp/ext/fee/response
#
# src/com/neulevel/epp/xri
#
	${CP}     ../src/com/neulevel/epp/xri/Makefile \
	   ../source/src/com/neulevel/epp/xri
#
# src/com/neulevel/epp/xri/command
#
	${CP}     ../src/com/neulevel/epp/xri/command/Makefile \
	   ../source/src/com/neulevel/epp/xri/command
#
# src/com/neulevel/epp/xri/response
#
	${CP}     ../src/com/neulevel/epp/xri/response/Makefile \
	   ../source/src/com/neulevel/epp/xri/response
#
# src/com/neulevel/epp
#
	${CP}     ../src/com/neulevel/epp/Makefile \
	   ../source/src/com/neulevel/epp
#
# src/com/neulevel/biz
#
	${CP}     ../src/com/neulevel/biz/Makefile \
	   ../source/src/com/neulevel/biz
#
# src/com/neulevel/us
#
	${CP}     ../src/com/neulevel/us/Makefile \
	   ../source/src/com/neulevel/us
#
# src/com/neulevel/cn
#
	${CP}     ../src/com/neulevel/cn/Makefile \
	../source/src/com/neulevel/cn
#
# src/com/neulevel
#
	${CP}     ../src/com/neulevel/Makefile \
	   ../source/src/com/neulevel
#
# Copy the Makefiles at higher levels
#
	${CP}  ../Makefile                       ../source
	${CP}  ../src/Makefile                   ../source/src
	${CP}  ../src/Makefile.incl              ../source/src
	${CP}  ../src/com/Makefile               ../source/src/com
#
# Copy the EPP Schema files in the xsd directory
#
	${MKDIR} ../source/xsd
	${MKDIR} ../source/xsd/xml
	${CP}    ../xsd/*.xsd                    ../source/xsd
	${CP}    ../xsd/xml/*.xsd                ../source/xsd/xml
	${CP}    ../xsd/xml/*.xml                ../source/xsd/xml
#
# Copy the files in the test directory
#
	${MKDIR} ../source/test
	${CP} ../test/Makefile                   ../source/test
	${CP} ../test/*.java                     ../source/test
	${CP} ../test/testkeys.*                 ../source/test
#	${CP} ../test/config.xml                 ../source/test
	${CP} ../test/RunTestTcp                 ../source/test
#	${CP} ../test/RunTestBeep                ../source/test
#
# Copy the license, Status file, and BUILD, etc
#
	${CP}  ../BUILD ../README.html ../STATUS ../LICENSE \
	       ../CHANGELOG ../EPP-1.0-MIGRATION-GUIDE \
	       ../source
