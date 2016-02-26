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
#include "BizAccount.hpp"
#include "BizLock.hpp"
#include "BizProtect.hpp"
#include "BizContact.hpp"
#include <xercesc/parsers/XercesDOMParser.hpp>

#include <string.h>
#include <stdlib.h>       
#include <iostream>     

#include "EppEntity.hpp"
#include "EppAuthInfo.hpp"
#include "EppUnspec.hpp"
#include "EppAddress.hpp"
#include "EppE164.hpp"
#include "EppPeriod.hpp"
#include "EppIpAddress.hpp"
#include "EppContactData.hpp"
#include "EppContactType.hpp"
#include "EppHello.hpp"
#include "EppStatus.hpp"
#include "EppServiceMenu.hpp"
#include "EppTransactionId.hpp"
#include "EppGreeting.hpp"
#include "EppContact.hpp"
#include "EppDomain.hpp"
#include "EppHost.hpp"
#include "EppSvcsub.hpp"
#include "EppError.hpp"
#include "EppParser.hpp"
#include "EppUtil.hpp"

int runEppTestCore(EppParser & parser);

#ifndef COMBINE_TEST

int main( int argc, char ** argv )
{
	try
	{
		XMLPlatformUtils::Initialize();
	}
	catch( const XMLException& e )
	{
		cerr	<< "Error during Xerces-c Initialization\n"
			<< "  Exception message:"
			<< DOMString(e.getMessage()) << endl;
		return 1;
	}
	EppParser *parser = new EppParser();
	runEppTestCore(*parser);

	delete 	parser;
	XMLPlatformUtils::Terminate();
}
#endif

int runEppTestCore(EppParser & parser)
{
	DOMString ss;
	if( ss.isNull() )
	{
		cout << "ss is null\n";
	}

	cout << "EppError " << EppError::CODE_NO_ERROR << " "
	     << EppError::TEXT_NO_ERROR << endl;
	cout << "EppError " << EppError::CODE_UNKNOWN_COMMAND << " "
	     << EppError::getText(EppError::CODE_UNKNOWN_COMMAND) << endl;

	DOMNode* n;

	{
		EppUnspec x;
		DOMString y = x.toString();

		cout << y << endl;
		cout << x.toString() << endl;
	}
	{
		EppAddress addr("city", "province", NULLXS, "cc");
		addr.setStreet(0, "addr1");
		addr.setStreet(1, "addr2");
		addr.setStreet(2, "addr3");
		cout << addr.toString() << endl;

		parser.parse(addr.toString());
		cout << parser.toString() << endl;

		n = parser.getDocument().getElementsByTagName(XS("address"))->item(0);
		EppAddress * newAddr = EppAddress::fromXML(*n);
		cout << newAddr->toString() << endl;
		newAddr->setPostalCode("postalcode");
		cout << newAddr->toString() << endl;
	}
	{
		EppAuthInfo auth(EppAuthInfo::TYPE_PW, "password", NULLXS);
		cout << auth.toString() << endl;
		parser.parse(auth.toString());

		n = parser.getDocument().getElementsByTagName(XS("authInfo"))->item(0);
		EppAuthInfo * newAuth = EppAuthInfo::fromXML(*n);
		cout << newAuth->toString() << endl;
		delete newAuth;
	}
	{
		EppE164 e164("+12345", "000");
		cout << e164.toString() << endl;
		parser.parse(e164.toString());
		n = parser.getDocument().getElementsByTagName(XS("e164"))->item(0);
		EppE164 * newE164 = EppE164::fromXML(*n);
		cout << newE164->toString() << endl;
		delete newE164;
	}
	{
		EppPeriod period(99, EppPeriod::UNIT_YEAR);
		cout << period.toString() << endl;
		parser.parse(period.toString());
		n = parser.getDocument().getElementsByTagName(XS("period"))->item(0);
		EppPeriod * newPeriod = EppPeriod::fromXML(*n);
		cout << newPeriod->toString() << endl;
		delete newPeriod;
	}
	{
		EppIpAddress ipAddress("192.123.1.1");
		cout << ipAddress.toString() << endl;
		parser.parse(ipAddress.toString());
		n = parser.getDocument().getElementsByTagName(XS("address"))->item(0);
		EppIpAddress * newIpAddress = EppIpAddress::fromXML(*n);
		cout << newIpAddress->toString() << endl;
		newIpAddress->setType(EppIpAddress::TYPE_V6);
		cout << newIpAddress->toString() << endl;
		delete newIpAddress;
	}
	{
		EppAddress addr("city", "province", NULLXS, "cc");
		addr.setStreet(0, "addr1");
		addr.setStreet(1, "addr2");
		addr.setStreet(2, "addr3");

		parser.parse(addr.toString());

		n = parser.getDocument().getElementsByTagName(XS("address"))->item(0);
		EppAddress * newAddr = EppAddress::fromXML(*n);
		cout << newAddr->toString() << endl;
		newAddr->setPostalCode("postalcode");
		cout << newAddr->toString() << endl;
		
		
		EppContactData contactData("Foo Bar", "Foo Bar Inc", addr);
		cout << contactData.toString() << endl;
		parser.parse(contactData.toString());
		n = parser.getDocument().getElementsByTagName(XS("contact-data"))->item(0);
		EppContactData * newContactData = EppContactData::fromXML(*n);
		cout << newContactData->toString() << endl;
		newContactData->setAddress(*newAddr);
		cout << newContactData->toString() << endl;
		delete newContactData;
	}
	{
		EppStatus status("not-ok");
		cout << status.toString() << endl;
		parser.parse(status.toString());
		n = parser.getDocument().getElementsByTagName(XS("status"))->item(0);
		EppStatus * newStatus = EppStatus::fromXML(*n);
		cout << newStatus->toString() << endl;
		newStatus->setLanguage("XYZ");
		newStatus->setMessage("Ba bla");
		cout << newStatus->toString() << endl;
		delete newStatus;
	}
	{
		EppContactType contactType("ROID", "TYPE");
		cout << contactType.toString() << endl;
		parser.parse(contactType.toString());
		n = parser.getDocument().getElementsByTagName(XS("contact"))->item(0);
		EppContactType * newContactType = EppContactType::fromXML(*n);
		cout << newContactType->toString() << endl;
		newContactType->setType("NEW TYPE");
		cout << newContactType->toString() << endl;
		delete newContactType;
	}
	{
		EppTransactionId transId("CL-ID", "SV-ID");
		cout << transId.toString() << endl;
		parser.parse(transId.toString());
		n = parser.getDocument().getElementsByTagName(XS("trID"))->item(0);
		EppTransactionId * newTransId = EppTransactionId::fromXML(*n);
		cout << newTransId->toString() << endl;
		newTransId->setClientTransactionId("NEW CL-ID");
		cout << newTransId->toString() << endl;
		delete newTransId;
	}
	{
		EppServiceMenu menu;
		menu.addVersion("1.0");
		menu.addVersion("1.1");
		menu.addVersion("1.2");
		menu.addVersion("1.3");
		menu.addVersion("1.4");
		menu.addVersion("1.5");
		menu.addVersion("1.6");
		menu.addLanguage("en");
		menu.addLanguage("zh-GB");
		menu.addLanguage("zh-BIG5");
		menu.addLanguage("fr");
		menu.addLanguage("de");
		menu.addLanguage("kr");
		menu.addLanguage("jp");
		menu.addService("contact");
		menu.addService("domain");
		menu.addService("host");
		cout << menu.toString() << endl;
		parser.parse(menu.toString());
		n = parser.getDocument().getElementsByTagName(XS("svcMenu"))->item(0);
		EppServiceMenu * newMenu = EppServiceMenu::fromXML(*n);
		cout << newMenu->toString() << endl;
		newMenu->addService("extension");
		cout << newMenu->toString() << endl;
		delete newMenu;
	}
	{
		EppHello hello;
		cout << hello.toString() << endl;
		parser.parse(hello.toString());
		n = parser.getRootNode();
		EppHello * newHello = EppHello::fromXML(*n);
		cout << newHello->toString() << endl;
		delete newHello;
	}
	{	
		EppServiceMenu newMenu;
		newMenu.addVersion("1.0");
		newMenu.addVersion("1.1");
		newMenu.addLanguage("en");
		newMenu.addLanguage("de");
		newMenu.addLanguage("kr");
		newMenu.addService("contact");
		newMenu.addService("domain");
		newMenu.addService("host");
		
		EppGreeting greeting;
		cout << greeting.toString() << endl;
		parser.parse(greeting.toString());
		n = parser.getRootNode();
		EppGreeting * newGreeting = EppGreeting::fromXML(*n);
		cout << newGreeting->toString() << endl;
		newGreeting->setServiceMenu(newMenu);
		cout << newGreeting->toString() << endl;
		delete newGreeting;
	}
	{
		EppHost host("ns.abc.com");
		host.addAddress(EppIpAddress("192.9.1.1"));
		host.addAddress(EppIpAddress("192.9.1.2"));
		host.addAddress(EppIpAddress("192.9.1.3"));
		cout << host.toString() << endl;
		parser.parse(host.toString());
		n = parser.getDocument().getElementsByTagName(XS("host:host"))->item(0);
		EppHost * newHost = EppHost::fromXML(*n);
		cout << newHost->toString() << endl;
		EppObject * newHost1 = EppObject::fromXML(*n);
		cout << newHost1->toString() << endl;
		delete newHost;
		delete newHost1;
	}
	{
		EppDomain domain("ns.abc.com");
		domain.addHost("ns1.abc.com");
		domain.addHost("ns2.abc.com");
		domain.addHost("ns3.abc.com");
		domain.addNameServer("ns1.xxx.com");
		domain.addNameServer("ns2.xxx.com");
		domain.addNameServer("ns3.xxx.com");
		domain.addNameServer(new EppHostAttribute("ns4.xxx.com"));
		domain.addNameServer(new EppHostAttribute("ns5.xxx.com"));
		domain.addNameServer(new EppHostAttribute("ns6.xxx.com"));
		domain.addContact("ROID-CONTACT1", EppDomain::CONTACT_TYPE_ADMIN);
		domain.addContact("ROID-CONTACT2", EppDomain::CONTACT_TYPE_TECH);
		domain.addContact("ROID-CONTACT3", EppDomain::CONTACT_TYPE_BILLING);
		domain.setRegistrant("ABC-COM-INC");
		domain.setPeriod(EppPeriod(9, EppPeriod::UNIT_YEAR));
		cout << domain.toString() << endl;
		parser.parse(domain.toString());
		n = parser.getDocument().getElementsByTagName(XS("domain:domain"))->item(0);
		EppDomain * newDomain = EppDomain::fromXML(*n);
		cout << newDomain->toString() << endl;
		EppObject * newDomain1 = EppObject::fromXML(*n);
		cout << newDomain1->toString() << endl;
		delete newDomain;
		delete newDomain1;
	}
	{
		EppSvcsub svcsub("BIZACCOUNT-001");
		svcsub.setService(BizAccount::ID);
		svcsub.addParam(BizLock::ID, "BIZLOCK-123");
		svcsub.addParam(BizLock::ID, "BIZLOCK-234");
		svcsub.addParam(BizAccount::PARAM_SVCLIST, "xyz@abc.com");
		svcsub.addParam(BizAccount::PARAM_SVCLIST, "abc@xyz.biz");
		svcsub.addParam(BizAccount::PARAM_LANGPREF, "Chinese");

		svcsub.addContact("ROID-CONTACT1", EppSvcsub::CONTACT_TYPE_ADMIN);
		svcsub.addContact("ROID-CONTACT2", EppSvcsub::CONTACT_TYPE_TECH);
		svcsub.addContact("ROID-CONTACT3", EppSvcsub::CONTACT_TYPE_BILLING);
		svcsub.addContact("ROID-CONTACT4", EppSvcsub::CONTACT_TYPE_PRIMARY);
		svcsub.addContact("ROID-CONTACT5", EppSvcsub::CONTACT_TYPE_SECONDARY);
		svcsub.addContact("ROID-CONTACT6", EppSvcsub::CONTACT_TYPE_BIZLOCK);
		svcsub.setRegistrant("ABC-COM-INC");
		svcsub.setPeriod(EppPeriod(9, EppPeriod::UNIT_YEAR));
		cout << svcsub.toString() << endl;
		parser.parse(svcsub.toString());
		n = parser.getDocument().getElementsByTagName(XS("svcsub:svcsub"))->item(0);
		EppSvcsub * newSvcsub = EppSvcsub::fromXML(*n);
		cout << newSvcsub->toString() << endl;
		EppObject * newSvcsub1 = EppObject::fromXML(*n);
		cout << newSvcsub1->toString() << endl;
		delete newSvcsub;
		delete newSvcsub1;
	}
	{
		EppAddress addr("city", "province", NULLXS, "cc");
		addr.setStreet(0, "addr1");
		addr.setStreet(1, "addr2");
		addr.setStreet(2, "addr3");
		
		EppContactData contactData("Foo Bar", "Foo Bar Inc", addr);
		
		EppContact contact("CONTACT-ID");
		contact.setContactDataAscii(contactData);
		contact.setVoice("+1234567", "123");
		contact.setFax("+xxxx", "999");
		contact.setEmail("xxx@foo.com");
		cout << contact.toString() << endl;
		parser.parse(contact.toString());
		n = parser.getDocument().getElementsByTagName(XS("contact:contact"))->item(0);
		EppContact * newContact = EppContact::fromXML(*n);
		cout << newContact->toString() << endl;
		EppObject * newContact1 = EppObject::fromXML(*n);
		cout << newContact1->toString() << endl;
		delete newContact;
		delete newContact1;
	}
	EppResult * result = parser.getResult();
	cout << result->toString() << endl;
	delete result;

	return 0;
}
