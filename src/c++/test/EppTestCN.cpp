/*******************************************************************************
 * The MIT License (MIT)
 *  
 * Copyright (c)2015 Neustar Inc.
 *  
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *  
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *  
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *******************************************************************************/
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <string.h>
#include <stdlib.h>       
#include <iostream>     
#include <fstream>

#include "EppError.hpp"
#include "EppParser.hpp"
#include "EppUtil.hpp"

#include "CNContactCreateExt.hpp"
#include "CNContactInfoExt.hpp"
#include "CNContactUpdateExt.hpp"
#include "CNDomainCreateExt.hpp"
#include "CNDomainInfoExt.hpp"
#include "CNDomainUpdateExt.hpp"
#include "CNHostCreateExt.hpp"
#include "CNHostInfoExt.hpp"
#include "CNHostUpdateExt.hpp"


#include "EppNamespaceParser.hpp"
#include "EppXmlValidator.hpp"
#include "CNCDNExt.hpp"

#define TMPFILENAME "/tmp/tmp.xxmmll"

#define SEPERATOR "\n-----------------------------------------------------------------------------\n"


int runEppTestCN(EppParser & parser);

#ifndef COMBINE_TEST

EppXmlValidator* getValidator()
{
	static EppXmlValidator* validator = new EppXmlValidator();
	return validator;
}

void makeFile(DOMString in)
{
	ofstream myfile;
	myfile.open (TMPFILENAME);
	myfile << in;
	myfile.close();
}

void makeValidate(DOMString in, EppXmlValidator& validator)
{
	makeFile(in);
	if ( !validator.parseFile(TMPFILENAME) )
	{
		cout<<endl<<"PASSED: XSD Validation";
	}
	else
	{
		cout<<endl<<"FAILED: XSD Validation";
	}
}

void doValidate(XS orig, XS again, const char* methodName)
{
	std::cout<<methodName<<"::"<<endl;
	if( XMLON )
	{
		std::cout<<orig;
	}

	if ( orig.equals(again) )
	{
		std::cout<<endl<<"Result of toXML and fromXML matches"<<endl;
	}
	else
	{
		std::cout<<endl<<"****************ERROR************ "
			"Result of toXML and fromXML does NOT match"
			"Again=\n"<<again<<endl;
	}
	makeValidate(orig, *getValidator());
	//makeValidate(again, getValidator());

}

int main( int argc, char ** argv )
{
	try
	{
		XMLPlatformUtils::Initialize();
	}
	catch( const XMLException& e )
	{
		cerr<< "Error during Xerces-c Initialization\n"
			<< "  Exception message:"
			<< DOMString(e.getMessage()) << endl;
		return 1;
	}

	if ( false == validateArguments(argc, argv) )
	{
		usage("EXE-NAME: ");
	}

	EppParser *parser = new EppParser();

	EppXmlValidator* validator = getValidator();

	if ( 1 == argc )
	{
		validator->loadXSD(getenv("PWD"));
	}
	else if ( (2 == argc) && (0 == strcmp(argv[1], ".")) )
	{
		validator->loadXSD(getenv("PWD"));
	}
	else
	{
		validator->loadXSD(argv[1]);
	}

	runEppTestCN(*parser);

	delete parser;
	delete validator;
	XMLPlatformUtils::Terminate();
}
#else

void doValidate(XS orig, XS again, const char* methodName){ }
#endif

int CNNICContactCreate(EppParser& parser, DOMNode* n)
{
	std::cout<<__FUNCTION__<<std::endl;

	CNContactCreateExt cnExt;
	CNContact * cc = new CNContact();
	cc->setMobile("8889188838");
	cc->setContactType("E");
	cc->setContactID("CNContactCreateExt-1234567890");
	cc->setContactIDType("OSF");
	cc->setPurveyor("CNContactCreateExt-TestPurveyor");

	cnExt.setContact(cc);
	DOMString orig = cnExt.toString();
	std::cout<<std::endl<<orig;

	parser.parse(orig);

	n = parser.getDocument().getDocumentElement();

	EppExtension * newRsp = EppExtension::fromXML(*n);
	DOMString again = newRsp->toString();

	doValidate(orig, again, __FUNCTION__);

	return 0;
}

int CNNICContactInfo(EppParser& parser, DOMNode* n)
{
	std::cout<<__FUNCTION__<<std::endl;

	CNContactInfoExt cnExt;
	CNContact * cc = new CNContact();
	cc->setMobile("8889188838");
	cc->setContactType("E");
	cc->setContactID("CNNICContactInfo-1234567890");
	cc->setContactIDType("OSF");
	cc->setPurveyor("CNNICContactInfo-TestPurveyor");

	cnExt.setContact(cc);

	DOMString orig = cnExt.toString();
	std::cout<<std::endl<<orig;

	parser.parse(orig);

	n = parser.getDocument().getDocumentElement();

	EppExtension * newRsp = EppExtension::fromXML(*n);
	DOMString again = newRsp->toString();

	doValidate(orig, again, __FUNCTION__);

	return 0;
}

int CNNICContactUpdate(EppParser& parser, DOMNode* n)
{
	std::cout<<__FUNCTION__<<std::endl;

	CNContactUpdateExt cnExt;

	CNContact* c1 = new CNContact();
	c1->setMobile("1_8889188838");
	c1->setContactType("E");
	c1->setContactID("1_1234567890");
	c1->setContactIDType("OSF");
	c1->setPurveyor("1_TestPurveyor");
	cnExt.setAddContact(c1);

	CNContact* c2 = new CNContact();
	c2->setMobile("2_8889188838");
	c2->setContactType("E");
	c2->setContactID("2_1234567890");
	c2->setContactIDType("OSF");
	c2->setPurveyor("2_TestPurveyor");
	cnExt.setRemContact(c2);

	CNContact* c3 = new CNContact();
	c3->setMobile("3_8889188838");
	c3->setContactType("E");
	c3->setContactID("3_1234567890");
	c3->setContactIDType("OSF");
	c3->setPurveyor("3_TestPurveyor");
	cnExt.setChgContact(c3);

	DOMString orig = cnExt.toString();
	std::cout<<std::endl<<orig;

	parser.parse(orig);

	n = parser.getDocument().getDocumentElement();

	EppExtension * newRsp = EppExtension::fromXML(*n);
	DOMString again = newRsp->toString();

	doValidate(orig, again, __FUNCTION__);

	return 0;
}

int CNNICDomainCreate(EppParser& parser, DOMNode* n)
{
	std::cout<<__FUNCTION__<<std::endl;

	CNDomainCreateExt cnExt;
	CNDomain * cd = new CNDomain();
	cd->setDomainType("E");
	cd->setPurveyor("CNNICDomainCreate-TestPurveyor");
	cnExt.setDomain(cd);

	DOMString orig = cnExt.toString();
	std::cout<<std::endl<<orig;

	parser.parse(orig);

	n = parser.getDocument().getDocumentElement();

	EppExtension * newRsp = EppExtension::fromXML(*n);
	DOMString again = newRsp->toString();

	doValidate(orig, again, __FUNCTION__);

	return 0;
}

int CNNICDomainInfo(EppParser& parser, DOMNode* n)
{
	std::cout<<__FUNCTION__<<std::endl;

	CNDomainInfoExt cnExt;
	CNDomain * cd = new CNDomain();
	cd->setDomainType("E");
	cd->setPurveyor("CNNICDomainInfo-TestPurveyor");
	cnExt.setDomain(cd);

	DOMString orig = cnExt.toString();
	std::cout<<std::endl<<orig;

	parser.parse(orig);

	n = parser.getDocument().getDocumentElement();

	EppExtension * newRsp = EppExtension::fromXML(*n);
	DOMString again = newRsp->toString();

	doValidate(orig, again, __FUNCTION__);

	return 0;
}

int CNNICDomainUpdate(EppParser& parser, DOMNode* n)
{
	std::cout<<__FUNCTION__<<std::endl;

	CNDomainUpdateExt cnExt;
	CNDomain *domain = new CNDomain();
	domain->setDomainType("E");
	domain->setPurveyor("CNNICDomainUpdate-TestPurveyor");

	cnExt.setChgDomain(domain);

	DOMString orig = cnExt.toString();
	std::cout<<std::endl<<orig;

	parser.parse(orig);

	n = parser.getDocument().getDocumentElement();

	EppExtension * newRsp = EppExtension::fromXML(*n);
	DOMString again = newRsp->toString();

	doValidate(orig, again, __FUNCTION__);

	return 0;
}

int CNNICHostCreate(EppParser& parser, DOMNode* n)
{
	std::cout<<__FUNCTION__<<std::endl;

	CNHostCreateExt cnExt;
	cnExt.setPurveyor("CNNICHostCreate-TestPurveyor");

	DOMString orig = cnExt.toString();
	std::cout<<std::endl<<orig;

	parser.parse(orig);

	n = parser.getDocument().getDocumentElement();

	EppExtension * newRsp = EppExtension::fromXML(*n);
	DOMString again = newRsp->toString();

	doValidate(orig, again, __FUNCTION__);

	return 0;
}

int CNNICHostInfo(EppParser& parser, DOMNode* n)
{
	std::cout<<__FUNCTION__<<std::endl;

	CNHostInfoExt cnExt;
	cnExt.setPurveyor("CNNICHostInfo-TestPurveyor");

	DOMString orig = cnExt.toString();
	std::cout<<std::endl<<orig;

	parser.parse(orig);

	n = parser.getDocument().getDocumentElement();

	EppExtension * newRsp = EppExtension::fromXML(*n);
	DOMString again = newRsp->toString();

	doValidate(orig, again, __FUNCTION__);

	return 0;
}

int CNNICHostUpdate(EppParser& parser, DOMNode* n)
{
	std::cout<<__FUNCTION__<<std::endl;

	CNHostUpdateExt cnExt;
	cnExt.setPurveyor("CNNICHostUpdate-TestPurveyor");

	DOMString orig = cnExt.toString();
	std::cout<<std::endl<<orig;

	parser.parse(orig);

	n = parser.getDocument().getDocumentElement();

	EppExtension * newRsp = EppExtension::fromXML(*n);
	DOMString again = newRsp->toString();

	doValidate(orig, again, __FUNCTION__);

	return 0;
}

int TestCNCDNExt(EppParser& parser, DOMNode* n)
{
	std::cout<<__FUNCTION__<<std::endl;

	CNCDNExt cnExt;
	cnExt.setOCDNPunycode("XN--OCDN");
	cnExt.setSCDN("SCDNValue");
	cnExt.setSCDNPunycode("XN--SCDNVALUE");
	cnExt.setTCDN("TCDNValue");
	cnExt.setTCDNPunycode("XN--TCDNValue");

	cnExt.addVCDN("VCDN1");
	cnExt.addVCDN("VCDN2");
	cnExt.addVCDN("VCDN3");

	cnExt.addVCDNPunnyCode("VCDNPUNY1");
	cnExt.addVCDNPunnyCode("VCDNPUNY2");
	cnExt.addVCDNPunnyCode("VCDNPUNY3");
	cnExt.addVCDNPunnyCode("VCDNPUNY4");

	DOMString orig = cnExt.toString();
	std::cout<<std::endl<<orig;

	parser.parse(orig);

	n = parser.getDocument().getDocumentElement();

	EppExtension * newRsp = EppExtension::fromXML(*n);
	DOMString again = newRsp->toString();

	doValidate(orig, again, __FUNCTION__);

	return 0;
}

int runEppTestCN(EppParser& parser)
{
	DOMNode* n = NULL;

	cout<<endl;
	cout<<SEPERATOR<<endl;

	CNNICContactCreate(parser, n);
	cout<<SEPERATOR<<endl;

	CNNICContactInfo(parser, n);
	cout<<SEPERATOR<<endl;

	CNNICContactUpdate(parser, n);
	cout<<SEPERATOR<<endl;

	CNNICDomainCreate(parser, n);
	cout<<SEPERATOR<<endl;

	CNNICDomainInfo(parser, n);
	cout<<SEPERATOR<<endl;

	CNNICDomainUpdate(parser, n);
	cout<<SEPERATOR<<endl;

	CNNICHostCreate(parser, n);
	cout<<SEPERATOR<<endl;

	CNNICHostInfo(parser, n);
	cout<<SEPERATOR<<endl;

	CNNICHostUpdate(parser, n);
	cout<<SEPERATOR<<endl;

	TestCNCDNExt(parser, n);
	cout<<SEPERATOR<<endl;
	return 0;
}
