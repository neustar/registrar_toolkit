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

#include "EppDomain.hpp"
#include "EppCommandCreate.hpp"
#include "EppCommandUpdateDomain.hpp"
#include "EppSecDnsDsData.hpp"
#include "EppCommandCreateSecDns.hpp"
#include "EppCommandUpdateSecDns.hpp"

#include "EppResponse.hpp"
#include "EppResponseDataInfo.hpp"
#include "EppResponseDataInfoSecDns.hpp"
#include "EppIDNData.hpp"
#include "EppCommandCheckFee.hpp"
#include "EppResponseCheckFee.hpp"
#include "EppResponseDataCheckDomain.hpp"
#include "EppCommandInfoFee.hpp"
#include "EppResponseInfoFee.hpp"
#include "EppCommandCreateFee.hpp"
#include "EppResponseCreateFee.hpp"
#include "EppCommandRenewFee.hpp"
#include "EppResponseRenewFee.hpp"
#include "EppCommandUpdateFee.hpp"
#include "EppResponseUpdateFee.hpp"
#include "EppResponseDataCreateDomain.hpp"
#include "EppResponseDataRenewDomain.hpp"
#include "EppCommandTransferFee.hpp"
#include "EppResponseTransferFee.hpp"
#include "EppResponseDeleteFee.hpp"

#include "EppNamespaceParser.hpp"
#include "EppXmlValidator.hpp"

#define TMPFILENAME "/tmp/tmp.xxmmll"

#define SEPERATOR "\n-----------------------------------------------------------------------------\n"


int runEppTestFee(EppParser & parser);

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
    if (!validator.parseFile(TMPFILENAME))
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
	
	if (orig.equals(again))
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

	if ( false == validateArguments(argc, argv))
	{
		usage("EXE-NAME: ");
		//exit (99);
	}

	EppParser *parser = new EppParser();

	EppXmlValidator* validator = getValidator();
	
	if (1 == argc)
	{
		validator->loadXSD(getenv("PWD"));
	}
	else if ((2 == argc) && (0 == strcmp(argv[1], ".")))
	{
		validator->loadXSD(getenv("PWD"));
	}
	else
	{
		validator->loadXSD(argv[1]);
	}


	runEppTestFee(*parser);

	delete parser;

	delete validator;
	XMLPlatformUtils::Terminate();
}
#else
void doValidate(XS orig, XS again, const char* methodName)
{
}
#endif




int InfoRequestTest(EppParser & parser, DOMNode* n)
{
	EppCommandInfoDomain eppCommand("INFO.DOMAIN", "ABC-DEF-XXXX-ID");

	EppCommandInfoFee* feeExt = new EppCommandInfoFee();

	feeExt->setCurrency("CAD");

	EppFeeCommand* command = new EppFeeCommand();
	command->setCommand("NEWEPPCOMMAND");
	command->setPhase("sunrise");
	feeExt->setCommand(command);

	EppPeriod* period = new EppPeriod();
	period->setUnit('y');
	period->setValue(1);
	feeExt->setPeriod(period);

	eppCommand.addExtension(feeExt);


	DOMString orig = eppCommand.toString();

	parser.parse(orig);
	n = parser.getRootNode();
	EppCommandInfo * eppCommandNew = (EppCommandInfo *) EppCommand::fromXML(*n);
	DOMString again = eppCommandNew->toString();

	delete eppCommandNew;
	doValidate(orig, again, __FUNCTION__);
	return 0;
}

int InfoResponseTest(EppParser& parser, DOMNode* n)
{
	EppDomain domain("INFO-RESPONSE-TEST.BIZ");
	domain.setRoid("ROID-ROID");

	EppStatus st(EppDomain::STATUS_CLIENT_UPDATE_PROHIBITED);
	domain.addStatus(st);

	domain.setClientId("123");

	EppResult result(EppError::CODE_NO_ERROR);
	//EppValueReason reason0("Value0", "Reason0", "en_US");
	EppValueReason reason0("Value0", "Reason0", "en");
	EppValueReason reason4("Value4");
	result.addValue(reason0);
	result.addValue("Value1");
	result.addValue("Value2");
	result.addValue("Value3");
	result.addValue(reason4);
	EppTransactionId trid("CLIENT-XID", "SERVER-ID");

	EppResponse rsp;
	rsp.addResult(result);
	rsp.setTransactionId(trid);
	EppResponseDataInfo infData(&domain);
	rsp.setResponseData(&infData);

	EppResponseInfoFee* feeExt = new EppResponseInfoFee();
	feeExt->setCurrency("RND");

	EppFeeCommand* command = new EppFeeCommand();
	command->setCommand("NEWEPPCOMMAND");
	command->setPhase("new-sunrise");
	feeExt->setCommand(command);

	EppPeriod* period = new EppPeriod();
	period->setUnit('m');
	period->setValue(11);
	feeExt->setPeriod(period);

	EppFeeFee *fee2 = new EppFeeFee();
	fee2->setDescription("SOME Fee");
	fee2->setRefundable("1");
	fee2->setFee(5.01);
	feeExt->addFee(fee2);

	rsp.addExtension(feeExt);

	DOMString orig = rsp.toString();

	parser.parse(orig);
	n = parser.getRootNode();

	EppResponse * newRsp = EppResponse::fromXML(*n);
	DOMString again = newRsp->toString();

	delete newRsp;
	
	doValidate(orig, again, __FUNCTION__);
	return 0;
}

int CheckRequestTest(EppParser& parser, DOMNode* n)
{
	EppCommandCheckDomain eppCommand;
	eppCommand.setClientTransactionId("CLINET-XID");
	eppCommand.add("CHECK-DOMAIN1.CLUB");
	eppCommand.add("CHECK-DOMAIN2.CLUB");
	eppCommand.add("CHECK-DOMAIN3.CLUB");

	EppCommandCheckFee* feeExt = new EppCommandCheckFee();
	{
		EppCommandCheckFeeType* element = new  EppCommandCheckFeeType();

		element->setName("CHECK-FEE-DOMAIN1.CLUB");
		element->setCurrency("USD");

		EppFeeCommand* command = new EppFeeCommand();
		command->setCommand("create");
		command->setPhase("sunrise");
		element->setCommand(command);

		EppPeriod* period = new EppPeriod();
		period->setUnit('y');
		period->setValue(1);
		element->setPeriod(period);
		
		feeExt->add(element);
	}
	{
		EppCommandCheckFeeType* element = new  EppCommandCheckFeeType();

		element->setName("CHECK-FEE-DOMAIN2.CLUB");
		element->setCurrency("EUR");

		EppFeeCommand* command = new EppFeeCommand();
		command->setCommand("create");
		command->setPhase("claims");
		command->setSubPhase("landrush");
		element->setCommand(command);

		EppPeriod* period = new EppPeriod();
		period->setUnit('y');
		period->setValue(2);
		element->setPeriod(period);
		
		feeExt->add(element);
	}
	{
		EppCommandCheckFeeType* element = new  EppCommandCheckFeeType();

		element->setName("CHECK-FEE-DOMAIN3.CLUB");
		element->setCurrency("EUR");

		EppFeeCommand* command = new EppFeeCommand();
		command->setCommand("transfer");
		element->setCommand(command);

		feeExt->add(element);
	}
	{
		EppCommandCheckFeeType* element = new  EppCommandCheckFeeType();

		element->setName("CHECK-FEE-DOMAIN4.CLUB");

		EppFeeCommand* command = new EppFeeCommand();
		command->setCommand("restore");
		element->setCommand(command);

		EppPeriod* period = new EppPeriod();
		period->setUnit('m');
		period->setValue(23);
		element->setPeriod(period);
		
		feeExt->add(element);
	}	

	eppCommand.addExtension(feeExt);

	DOMString orig = eppCommand.toString();
	
	parser.parse(orig);
	n = parser.getRootNode();
	EppCommandCheckDomain * eppCommandNew = (EppCommandCheckDomain *) EppCommand::fromXML(*n);
	DOMString again = eppCommandNew->toString();

	delete eppCommandNew;
	
	doValidate(orig, again, __FUNCTION__);
	return 0;
}

int CheckResponseTest(EppParser& parser, DOMNode* n)
{
	EppResponse rsp;
	

	EppResult result(EppError::CODE_NO_ERROR);
	//EppValueReason reason0("Value0", "Reason0", "en_US");
	EppValueReason reason0("Value0", "Reason0", "en");
	EppValueReason reason4("Value4");
	result.addValue(reason0);
	result.addValue("Value1");
	result.addValue("Value2");
	result.addValue("Value3");
	result.addValue(reason4);
	EppTransactionId trid("CLIENT-XID", "SERVER-ID");

	rsp.addResult(result);
	rsp.setTransactionId(trid);

	EppResponseDataCheckDomain eppCommand;
	eppCommand.add("DOMAIN1.CLUB", DOMString(EppResponseDataCheck::FOUND));
	eppCommand.add("DOMAIN2.CLUB", DOMString(EppResponseDataCheck::NOT_FOUND));
	eppCommand.add("DOMAIN3.CLUB", DOMString(EppResponseDataCheck::FOUND));
	rsp.setResponseData(&eppCommand);


	EppResponseCheckFee* feeExt = new EppResponseCheckFee();
	{
		{
			EppResponseCheckFeeType* element = new  EppResponseCheckFeeType();
			element->setName("CHECK-RESPOSNSE-FEE-DOMAIN1.CLUB");
			element->setCurrency("USD");

			EppFeeCommand* command = new EppFeeCommand();
			command->setCommand("create");
			command->setPhase("sunrise");
			element->setCommand(command);
			
			EppPeriod* period = new EppPeriod();
			period->setUnit('y');
			period->setValue(1);
			element->setPeriod(period);
		
			EppFeeFee *fee1 = new EppFeeFee();
			fee1->setApplied("delayed");
			fee1->setDescription("Application Fee");
			fee1->setRefundable("0");
			fee1->setFee(5.002);
			fee1->setGracePeriod("P15D");
			element->addFee(fee1);
			
			EppFeeFee *fee2 = new EppFeeFee();
			fee2->setDescription("Registration Fee");
			fee2->setRefundable("1");
			fee2->setFee(5.003);
			element->addFee(fee2);
			feeExt->add(element);
		}
		{
			EppResponseCheckFeeType* element = new  EppResponseCheckFeeType();
			element->setName("CHECK-RESPOSNSE-FEE-DOMAIN2.CLUB");
			element->setCurrency("EUR");

			EppFeeCommand* command = new EppFeeCommand();
			command->setCommand("create");
			command->setPhase("claims");
			command->setSubPhase("landrush");
			element->setCommand(command);
			
			EppPeriod* period = new EppPeriod();
			period->setUnit('y');
			period->setValue(2);
			element->setPeriod(period);
		
			EppFeeFee *fee1 = new EppFeeFee();
			fee1->setFee(5.004);
			element->addFee(fee1);
			feeExt->add(element);
		}
		{
			EppResponseCheckFeeType* element = new  EppResponseCheckFeeType();
			element->setName("CHECK-RESPOSNSE-FEE-DOMAIN3.CLUB");
			element->setCurrency("EUR");

			EppFeeCommand* command = new EppFeeCommand();
			command->setCommand("transfer");
			element->setCommand(command);
			
			EppPeriod* period = new EppPeriod();
			period->setUnit('y');
			period->setValue(2);
			element->setPeriod(period);
		
			EppFeeFee *fee1 = new EppFeeFee();
			fee1->setApplied("immediate");
			fee1->setDescription("Transfer Fee");
			fee1->setFee(2.505);
			element->addFee(fee1);
			
			EppFeeFee *fee2 = new EppFeeFee();
			fee2->setDescription("Renewal Fee");
			fee2->setFee(10.00);
			element->addFee(fee2);
			feeExt->add(element);
		}
		
		{
			EppResponseCheckFeeType* element = new  EppResponseCheckFeeType();
			element->setName("CHECK-RESPOSNSE-FEE-DOMAIN4.CLUB");
			element->setCurrency("GBP");

			EppFeeCommand* command = new EppFeeCommand();
			command->setCommand("restore");
			element->setCommand(command);
			
			EppPeriod* period = new EppPeriod();
			period->setUnit('y');
			period->setValue(1);
			element->setPeriod(period);
		
			EppFeeFee *fee1 = new EppFeeFee();
			fee1->setDescription("Restore Fee");
			fee1->setFee(25);
			element->addFee(fee1);
			
			EppFeeFee *fee2 = new EppFeeFee();
			fee2->setDescription("Renewal Fee");
			fee2->setFee(5.00);
			element->addFee(fee2);
			
			element->setFeeClass("premium-tier1");
			feeExt->add(element);
		}
	}

	rsp.addExtension(feeExt);

	DOMString orig = rsp.toString();

	parser.parse(orig);


	n = parser.getRootNode();
	EppResponse * newRsp = (EppResponse *) EppResponse::fromXML(*n);
	DOMString again = newRsp->toString();

	delete newRsp;
	

	doValidate(orig, again, __FUNCTION__);
	return 0;
}

int CreateRequestTest(EppParser& parser, DOMNode* n)
{
	EppDomain domain("CREATE-FEE.BIZ");
	EppAuthInfo authInfo(EppAuthInfo::TYPE_PW, "3fooBar");
	domain.setAuthInfo(authInfo);
	EppCommandCreate eppCommand(&domain, "ABC-DEF-XXXX-ID");

	EppCommandCreateFee* feeExt = new EppCommandCreateFee();

	feeExt->setCurrency("INR");
	EppFeeFee *fee1 = new EppFeeFee();
	fee1->setApplied("immediate");
	fee1->setDescription("Application Fee");
	fee1->setRefundable("1");
	fee1->setFee(5.00);
	fee1->setGracePeriod("P30D");
	feeExt->addFee(fee1);

	eppCommand.addExtension(feeExt);


	DOMString orig = eppCommand.toString();

	parser.parse(orig);
	n = parser.getRootNode();
	EppCommandCreate * eppCommandNew = (EppCommandCreate *) EppCommand::fromXML(*n);
	DOMString again = eppCommandNew->toString();

	delete eppCommandNew;

	doValidate(orig, again, __FUNCTION__);
	return 0;

}
int CreateResponseTest(EppParser& parser, DOMNode* n)
{
	EppDomain domain("CREATE-RESPONSE-TEST.BIZ");
	EppAuthInfo authInfo(EppAuthInfo::TYPE_PW, "3fooBar");
	domain.setAuthInfo(authInfo);

	EppResult result(EppError::CODE_NO_ERROR);
	EppValueReason reason0("Value0", "Reason0", "en");
	EppValueReason reason4("Value4");
	result.addValue(reason0);
	result.addValue("Value1");
	result.addValue("Value2");
	result.addValue("Value3");
	result.addValue(reason4);
	EppTransactionId trid("CLIENT-XID", "SERVER-ID");

	EppResponse rsp;
	rsp.addResult(result);
	rsp.setTransactionId(trid);

	EppResponseDataCreateDomain creDomain("CREATE-RESPONSE-TEST.BIZ", time(0));
	rsp.setResponseData(&creDomain);

	EppResponseCreateFee* feeExt = new EppResponseCreateFee();
	feeExt->setCurrency("RND");

	EppFeeFee *fee2 = new EppFeeFee();
	fee2->setDescription("SOME Fee");
	fee2->setRefundable("1");
	fee2->setFee(5.00);
	feeExt->addFee(fee2);

	feeExt->setBalance("1000");
	feeExt->setCreditLimit("5000");
	rsp.addExtension(feeExt);

	DOMString orig = rsp.toString();

	parser.parse(orig);
	n = parser.getRootNode();

	EppResponse * newRsp = EppResponse::fromXML(*n);
	DOMString again = newRsp->toString();

	delete newRsp;
	
	doValidate(orig, again, __FUNCTION__);
	return 0;

}

int RenewRequestTest(EppParser& parser, DOMNode* n)
{
	EppCommandRenewDomain eppCommand("RENEW-FEE.BIZ", "ABC-DEF-XXXX-ID");

	EppCommandRenewFee* feeExt = new EppCommandRenewFee();

	feeExt->setCurrency("INR");
			EppFeeFee *fee1 = new EppFeeFee();
			fee1->setApplied("delayed");
			fee1->setDescription("Application Fee");
			fee1->setRefundable("1");
			fee1->setFee(5.00);
			fee1->setGracePeriod("P25D");
	feeExt->addFee(fee1);

	eppCommand.addExtension(feeExt);


	DOMString orig = eppCommand.toString();

	parser.parse(orig);
	n = parser.getRootNode();
	EppCommandCreate * eppCommandNew = (EppCommandCreate *) EppCommand::fromXML(*n);
	DOMString again = eppCommandNew->toString();

	delete eppCommandNew;

	doValidate(orig, again, __FUNCTION__);
	return 0;

}
int RenewResponseTest(EppParser& parser, DOMNode* n)
{
	EppDomain domain("RENEW-RESPONSE-TEST.BIZ");
	EppAuthInfo authInfo(EppAuthInfo::TYPE_PW, "3fooBar");
	domain.setAuthInfo(authInfo);

	EppResult result(EppError::CODE_NO_ERROR);
	EppValueReason reason0("Value0", "Reason0", "en");
	EppValueReason reason4("Value4");
	result.addValue(reason0);
	result.addValue("Value1");
	result.addValue("Value2");
	result.addValue("Value3");
	result.addValue(reason4);
	EppTransactionId trid("CLIENT-XID", "SERVER-ID");

	EppResponse rsp;
	rsp.addResult(result);
	rsp.setTransactionId(trid);

	EppResponseDataRenewDomain eppCommand("RENEW-RESPONSE-TEST.BIZ", time(0));
	rsp.setResponseData(&eppCommand);

	EppResponseRenewFee* feeExt = new EppResponseRenewFee();
	feeExt->setCurrency("RND");

	EppFeeFee *fee2 = new EppFeeFee();
	fee2->setDescription("SOME Fee");
	fee2->setRefundable("1");
	fee2->setFee(5.00);
	feeExt->addFee(fee2);

	feeExt->setBalance("1000");
	feeExt->setCreditLimit("5000");
	rsp.addExtension(feeExt);

	DOMString orig = rsp.toString();

	parser.parse(orig);
	n = parser.getRootNode();

	EppResponse * newRsp = EppResponse::fromXML(*n);
	DOMString again = newRsp->toString();

	delete newRsp;
	
	doValidate(orig, again, __FUNCTION__);
	return 0;

}

int UpdateRequestTest(EppParser& parser, DOMNode* n)
{
	EppCommandUpdateDomain eppCommand("UPDATE-FEE.BIZ", "ABC-DEF-XXXX-ID");

	EppCommandUpdateFee* feeExt = new EppCommandUpdateFee();

	feeExt->setCurrency("INR");
			EppFeeFee *fee1 = new EppFeeFee();
			fee1->setApplied("delayed");
			fee1->setDescription("Application Fee");
			fee1->setRefundable("1");
			fee1->setFee(5.00);
			fee1->setGracePeriod("P20D");
	feeExt->addFee(fee1);

	eppCommand.addExtension(feeExt);


	DOMString orig = eppCommand.toString();

	parser.parse(orig);
	n = parser.getRootNode();
	EppCommandCreate * eppCommandNew = (EppCommandCreate *) EppCommand::fromXML(*n);
	DOMString again = eppCommandNew->toString();

	delete eppCommandNew;

	doValidate(orig, again, __FUNCTION__);
	return 0;

}
int UpdateResponseTest(EppParser& parser, DOMNode* n)
{
	EppDomain domain("UPDATE-RESPONSE-TEST.BIZ");
	EppAuthInfo authInfo(EppAuthInfo::TYPE_PW, "3fooBar");
	domain.setAuthInfo(authInfo);

	EppResult result(EppError::CODE_NO_ERROR);
	EppValueReason reason0("Value0", "Reason0", "en");
	EppValueReason reason4("Value4");
	result.addValue(reason0);
	result.addValue("Value1");
	result.addValue("Value2");
	result.addValue("Value3");
	result.addValue(reason4);
	EppTransactionId trid("CLIENT-XID", "SERVER-ID");

	EppResponse rsp;
	rsp.addResult(result);
	rsp.setTransactionId(trid);

	//EppResponseDataUpdateDomain eppCommand("RENEW-RESPONSE-TEST.BIZ", time(0));
	//rsp.setResponseData(&eppCommand);


	EppResponseDataRenewDomain eppCommand("UPDATE-RESPONSE-TEST.BIZ", time(0));
	rsp.setResponseData(&eppCommand);

	
	EppResponseUpdateFee* feeExt = new EppResponseUpdateFee();
	feeExt->setCurrency("RND");

	EppFeeFee *fee2 = new EppFeeFee();
	fee2->setDescription("SOME Fee");
	fee2->setRefundable("1");
	fee2->setFee(5.00);
	feeExt->addFee(fee2);

	feeExt->setBalance("1000");
	feeExt->setCreditLimit("5000");
	rsp.addExtension(feeExt);

	DOMString orig = rsp.toString();

	parser.parse(orig);
	n = parser.getRootNode();

	EppResponse * newRsp = EppResponse::fromXML(*n);
	DOMString again = newRsp->toString();

	delete newRsp;
	
	doValidate(orig, again, __FUNCTION__);
	return 0;

}


int TransferRequestTest(EppParser& parser, DOMNode* n)
{
	EppCommandUpdateDomain eppCommand("TRANSFER-FEE.BIZ", "ABC-DEF-XXXX-ID");

	EppCommandTransferFee* feeExt = new EppCommandTransferFee();

	feeExt->setCurrency("INR");
			EppFeeFee *fee1 = new EppFeeFee();
			fee1->setApplied("immediate");
			fee1->setDescription("Application Fee");
			fee1->setRefundable("1");
			fee1->setFee(5.00);
			fee1->setGracePeriod("P22D");
	feeExt->addFee(fee1);

	eppCommand.addExtension(feeExt);


	DOMString orig = eppCommand.toString();

	parser.parse(orig);
	n = parser.getRootNode();
	EppCommandCreate * eppCommandNew = (EppCommandCreate *) EppCommand::fromXML(*n);
	DOMString again = eppCommandNew->toString();

	delete eppCommandNew;

	doValidate(orig, again, __FUNCTION__);
	return 0;

}
int TransferResponseTest(EppParser& parser, DOMNode* n)
{
	EppDomain domain("TRANSFER-RESPONSE-TEST.BIZ");
	EppAuthInfo authInfo(EppAuthInfo::TYPE_PW, "3fooBar");
	domain.setAuthInfo(authInfo);

	EppResult result(EppError::CODE_NO_ERROR);
	EppValueReason reason0("Value0", "Reason0", "en");
	EppValueReason reason4("Value4");
	result.addValue(reason0);
	result.addValue("Value1");
	result.addValue("Value2");
	result.addValue("Value3");
	result.addValue(reason4);
	EppTransactionId trid("CLIENT-XID", "SERVER-ID");

	EppResponse rsp;
	rsp.addResult(result);
	rsp.setTransactionId(trid);

	//EppResponseDataUpdateDomain eppCommand("RENEW-RESPONSE-TEST.BIZ", time(0));
	//rsp.setResponseData(&eppCommand);


	EppResponseDataRenewDomain eppCommand("TRANSFER-RESPONSE-TEST.BIZ", time(0));
	rsp.setResponseData(&eppCommand);

	
	EppResponseTransferFee* feeExt = new EppResponseTransferFee();
	feeExt->setCurrency("RND");

	EppFeeFee *fee2 = new EppFeeFee();
	fee2->setDescription("SOME Fee");
	fee2->setRefundable("1");
	fee2->setFee(5.00);
	feeExt->addFee(fee2);

// Come in response to transfer query command only
	EppPeriod* period = new EppPeriod();
	period->setUnit('y');
	period->setValue(21);
	feeExt->setPeriod(period);
//

	feeExt->setBalance("1000");
	feeExt->setCreditLimit("5000");
	rsp.addExtension(feeExt);

	DOMString orig = rsp.toString();

	parser.parse(orig);
	n = parser.getRootNode();

	EppResponse * newRsp = EppResponse::fromXML(*n);
	DOMString again = newRsp->toString();

	delete newRsp;
	
	doValidate(orig, again, __FUNCTION__);
	return 0;

}

int DeleteResponseTest(EppParser& parser, DOMNode* n)
{
	EppDomain domain("DELETE-RESPONSE-TEST.BIZ");
	EppAuthInfo authInfo(EppAuthInfo::TYPE_PW, "3fooBar");
	domain.setAuthInfo(authInfo);

	EppResult result(EppError::CODE_NO_ERROR);
	EppValueReason reason0("Value0", "Reason0", "en");
	EppValueReason reason4("Value4");
	result.addValue(reason0);
	result.addValue("Value1");
	result.addValue("Value2");
	result.addValue("Value3");
	result.addValue(reason4);
	EppTransactionId trid("CLIENT-XID", "SERVER-ID");

	EppResponse rsp;
	rsp.addResult(result);
	rsp.setTransactionId(trid);

	//EppResponseDataUpdateDomain eppCommand("RENEW-RESPONSE-TEST.BIZ", time(0));
	//rsp.setResponseData(&eppCommand);


	EppResponseDataRenewDomain eppCommand("DELETE-RESPONSE-TEST.BIZ", time(0));
	rsp.setResponseData(&eppCommand);

	
	EppResponseDeleteFee* feeExt = new EppResponseDeleteFee();
	feeExt->setCurrency("RND");

	{
		EppFeeCredit *cr1 = new EppFeeCredit();
		cr1->setDescription("SOME Credit 1");
		cr1->setCredit("-5.00");
		feeExt->addCredit(cr1);
	}
	{
		EppFeeCredit *cr2 = new EppFeeCredit();
		cr2->setDescription("SOME Credit 2");
		cr2->setCredit("-15.00");
		feeExt->addCredit(cr2);
	}
	

	feeExt->setBalance("1000");
	feeExt->setCreditLimit("5000");
	rsp.addExtension(feeExt);

	DOMString orig = rsp.toString();

	parser.parse(orig);
	n = parser.getRootNode();

	EppResponse * newRsp = EppResponse::fromXML(*n);
	DOMString again = newRsp->toString();

	delete newRsp;
	
	doValidate(orig, again, __FUNCTION__);
	return 0;

}

int runEppTestFee(EppParser& parser)
{
	DOMNode* n = NULL;

	cout<<endl;
	cout<<SEPERATOR<<endl;

	InfoRequestTest(parser, n);
	cout<<SEPERATOR<<endl;
	
	InfoResponseTest(parser, n);	
	cout<<SEPERATOR<<endl;
	
	CheckRequestTest(parser, n);	
	cout<<SEPERATOR<<endl;
	
	CheckResponseTest(parser, n);	
	cout<<SEPERATOR<<endl;
	
	CreateRequestTest(parser, n);	
	cout<<SEPERATOR<<endl;
	
	CreateResponseTest(parser, n);	
	cout<<SEPERATOR<<endl;
	
	RenewRequestTest(parser, n);	
	cout<<SEPERATOR<<endl;
	
	RenewResponseTest(parser, n);	
	cout<<SEPERATOR<<endl;
	
	UpdateRequestTest(parser, n);	
	cout<<SEPERATOR<<endl;
	
	UpdateResponseTest(parser, n);	
	cout<<SEPERATOR<<endl;
	
	TransferRequestTest(parser, n);	
	cout<<SEPERATOR<<endl;
	
	TransferResponseTest(parser, n);	
	cout<<SEPERATOR<<endl;
	
	DeleteResponseTest(parser, n);	
	cout<<SEPERATOR<<endl;
	
	return 0;
}
