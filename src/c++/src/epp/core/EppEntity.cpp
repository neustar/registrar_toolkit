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
#include "EppEntity.hpp"
#include "EppUtil.hpp"

const int EppEntity::TYPE_EppEntity                      = 0   ;
const int EppEntity::TYPE_EppObject                      = 100 ;
const int EppEntity::TYPE_EppContact                     = 101 ;
const int EppEntity::TYPE_EppDomain                      = 102 ;
const int EppEntity::TYPE_EppHost                        = 103 ;
const int EppEntity::TYPE_EppSvcsub                      = 104 ;
const int EppEntity::TYPE_EppXriAuthority                = 105 ;
const int EppEntity::TYPE_EppXriNumber                   = 106 ;
const int EppEntity::TYPE_EppXriName                     = 107 ;
const int EppEntity::TYPE_EppXriService                  = 108 ;
const int EppEntity::TYPE_EppXriCodeString               = 109 ;

const int EppEntity::TYPE_EppCommand                     = 200 ;
const int EppEntity::TYPE_EppCommandPoll                 = 201 ;
const int EppEntity::TYPE_EppCommandLogin                = 202 ;
const int EppEntity::TYPE_EppCommandLogout               = 203 ;
const int EppEntity::TYPE_EppCommandCreate               = 210 ;
const int EppEntity::TYPE_EppCommandCheck                = 220 ;
const int EppEntity::TYPE_EppCommandCheckContact         = 221 ;
const int EppEntity::TYPE_EppCommandCheckDomain          = 222 ;
const int EppEntity::TYPE_EppCommandCheckHost            = 223 ;
const int EppEntity::TYPE_EppCommandCheckSvcsub          = 224 ;
const int EppEntity::TYPE_EppCommandCheckXriAuthority    = 225 ;
const int EppEntity::TYPE_EppCommandCheckXriNumber       = 226 ;
const int EppEntity::TYPE_EppCommandCheckXriName         = 227 ;
const int EppEntity::TYPE_EppCommandCheckXriService      = 228 ;
const int EppEntity::TYPE_EppCommandCheckXriCodeString   = 229 ;

const int EppEntity::TYPE_EppCommandDelete               = 230 ;
const int EppEntity::TYPE_EppCommandDeleteContact        = 231 ;
const int EppEntity::TYPE_EppCommandDeleteDomain         = 232 ;
const int EppEntity::TYPE_EppCommandDeleteHost           = 233 ;
const int EppEntity::TYPE_EppCommandDeleteSvcsub         = 234 ;
const int EppEntity::TYPE_EppCommandDeleteXriAuthority   = 235 ;
const int EppEntity::TYPE_EppCommandDeleteXriNumber      = 236 ;
const int EppEntity::TYPE_EppCommandDeleteXriName        = 237 ;
const int EppEntity::TYPE_EppCommandDeleteXriService     = 238 ;
const int EppEntity::TYPE_EppCommandDeleteXriCodeString  = 239 ;

const int EppEntity::TYPE_EppCommandInfo                 = 240 ;
const int EppEntity::TYPE_EppCommandInfoContact          = 241 ;
const int EppEntity::TYPE_EppCommandInfoDomain           = 242 ;
const int EppEntity::TYPE_EppCommandInfoHost             = 243 ;
const int EppEntity::TYPE_EppCommandInfoSvcsub           = 244 ;
const int EppEntity::TYPE_EppCommandInfoXriAuthority     = 245 ;
const int EppEntity::TYPE_EppCommandInfoXriNumber        = 246 ;
const int EppEntity::TYPE_EppCommandInfoXriName          = 247 ;
const int EppEntity::TYPE_EppCommandInfoXriService       = 248 ;
const int EppEntity::TYPE_EppCommandInfoXriCodeString    = 249 ;

const int EppEntity::TYPE_EppCommandUpdate               = 250 ;
const int EppEntity::TYPE_EppCommandUpdateContact        = 251 ;
const int EppEntity::TYPE_EppCommandUpdateDomain         = 252 ;
const int EppEntity::TYPE_EppCommandUpdateHost           = 253 ;
const int EppEntity::TYPE_EppCommandUpdateSvcsub         = 254 ;
const int EppEntity::TYPE_EppCommandUpdateXriAuthority   = 255 ;
const int EppEntity::TYPE_EppCommandUpdateXriNumber      = 256 ;
const int EppEntity::TYPE_EppCommandUpdateXriName        = 257 ;
const int EppEntity::TYPE_EppCommandUpdateXriService     = 258 ;
const int EppEntity::TYPE_EppCommandUpdateXriCodeString  = 259 ;

const int EppEntity::TYPE_EppCommandRenew                = 260 ;
const int EppEntity::TYPE_EppCommandRenewContact         = 261 ;
const int EppEntity::TYPE_EppCommandRenewDomain          = 262 ;
const int EppEntity::TYPE_EppCommandRenewHost            = 263 ;
const int EppEntity::TYPE_EppCommandRenewSvcsub          = 264 ;
const int EppEntity::TYPE_EppCommandRenewXriAuthority    = 265 ;
const int EppEntity::TYPE_EppCommandRenewXriNumber       = 266 ;
const int EppEntity::TYPE_EppCommandRenewXriName         = 267 ;
const int EppEntity::TYPE_EppCommandRenewXriService      = 268 ;
const int EppEntity::TYPE_EppCommandRenewXriCodeString   = 269 ;

const int EppEntity::TYPE_EppCommandTransfer             = 270 ;
const int EppEntity::TYPE_EppCommandTransferContact      = 271 ;
const int EppEntity::TYPE_EppCommandTransferDomain       = 272 ;
const int EppEntity::TYPE_EppCommandTransferHost         = 273 ;
const int EppEntity::TYPE_EppCommandTransferSvcsub       = 274 ;
const int EppEntity::TYPE_EppCommandTransferXriAuthority = 275 ;
const int EppEntity::TYPE_EppCommandTransferXriNumber    = 276 ;
const int EppEntity::TYPE_EppCommandTransferXriName      = 277 ;
const int EppEntity::TYPE_EppCommandTransferXriService   = 278 ;
const int EppEntity::TYPE_EppCommandTransferXriCodeString = 279 ;

const int EppEntity::TYPE_EppResponse                    = 300 ;

const int EppEntity::TYPE_EppResponseData                = 400 ;
const int EppEntity::TYPE_EppResponseDataPoll            = 410 ;
const int EppEntity::TYPE_EppResponseDataInfo            = 420 ;
const int EppEntity::TYPE_EppResponseDataCreate          = 430 ;
const int EppEntity::TYPE_EppResponseDataCreateContact   = 431 ;
const int EppEntity::TYPE_EppResponseDataCreateDomain    = 432 ;
const int EppEntity::TYPE_EppResponseDataCreateHost      = 433 ;
const int EppEntity::TYPE_EppResponseDataCreateSvcsub    = 434 ;
const int EppEntity::TYPE_EppResponseDataCreateXriAuthority = 435 ;
const int EppEntity::TYPE_EppResponseDataCreateXriNumber   = 436 ;
const int EppEntity::TYPE_EppResponseDataCreateXriName     = 437 ;
const int EppEntity::TYPE_EppResponseDataCreateXriService  = 438 ;
const int EppEntity::TYPE_EppResponseDataCreateXriCodeString = 439 ;

const int EppEntity::TYPE_EppResponseDataCheck           = 440 ;
const int EppEntity::TYPE_EppResponseDataCheckContact    = 441 ;
const int EppEntity::TYPE_EppResponseDataCheckDomain     = 442 ;
const int EppEntity::TYPE_EppResponseDataCheckHost       = 443 ;
const int EppEntity::TYPE_EppResponseDataCheckSvcsub     = 444 ;
const int EppEntity::TYPE_EppResponseDataCheckXriAuthority = 445 ;
const int EppEntity::TYPE_EppResponseDataCheckXriNumber   = 446 ;
const int EppEntity::TYPE_EppResponseDataCheckXriName     = 447 ;
const int EppEntity::TYPE_EppResponseDataCheckXriService  = 448 ;
const int EppEntity::TYPE_EppResponseDataCheckXriCodeString = 449 ;

const int EppEntity::TYPE_EppResponseDataTransfer        = 450 ;
const int EppEntity::TYPE_EppResponseDataTransferContact = 451 ;
const int EppEntity::TYPE_EppResponseDataTransferDomain  = 452 ;
const int EppEntity::TYPE_EppResponseDataTransferHost    = 453 ;
const int EppEntity::TYPE_EppResponseDataTransferSvcsub  = 454 ;
const int EppEntity::TYPE_EppResponseDataTransferXriAuthority = 455 ;
const int EppEntity::TYPE_EppResponseDataTransferXriNumber   = 456 ;
const int EppEntity::TYPE_EppResponseDataTransferXriName     = 457 ;
const int EppEntity::TYPE_EppResponseDataTransferXriService  = 458 ;
const int EppEntity::TYPE_EppResponseDataTransferXriCodeString = 459 ;

const int EppEntity::TYPE_EppResponseDataRenew           = 550 ;
const int EppEntity::TYPE_EppResponseDataRenewContact    = 551 ;
const int EppEntity::TYPE_EppResponseDataRenewDomain     = 552 ;
const int EppEntity::TYPE_EppResponseDataRenewHost       = 553 ;
const int EppEntity::TYPE_EppResponseDataRenewSvcsub     = 554 ;
const int EppEntity::TYPE_EppResponseDataRenewXriAuthority = 555 ;
const int EppEntity::TYPE_EppResponseDataRenewXriNumber   = 556 ;
const int EppEntity::TYPE_EppResponseDataRenewXriName     = 557 ;
const int EppEntity::TYPE_EppResponseDataRenewXriService  = 558 ;
const int EppEntity::TYPE_EppResponseDataRenewXriCodeString = 559 ;

const int EppEntity::TYPE_EppResponseDataPending         = 650 ;
const int EppEntity::TYPE_EppResponseDataPendingContact  = 651 ;
const int EppEntity::TYPE_EppResponseDataPendingDomain   = 652 ;
const int EppEntity::TYPE_EppResponseDataPendingHost     = 653 ;
const int EppEntity::TYPE_EppResponseDataPendingSvcsub   = 654 ;
const int EppEntity::TYPE_EppResponseDataPendingXriAuthority = 655 ;
const int EppEntity::TYPE_EppResponseDataPendingXriNumber   = 656 ;
const int EppEntity::TYPE_EppResponseDataPendingXriName     = 657 ;
const int EppEntity::TYPE_EppResponseDataPendingXriService  = 658 ;
const int EppEntity::TYPE_EppResponseDataPendingXriCodeString = 659 ;

const int EppEntity::TYPE_EppExtension                   = 900 ;
const int EppEntity::TYPE_EppExtensionUnspec             = 901 ;
const int EppEntity::TYPE_EppSecDnsDsData                = 902 ;
const int EppEntity::TYPE_EppSecDnsKeyData               = 903 ;
const int EppEntity::TYPE_EppCommandCreateSecDns         = 904 ;
const int EppEntity::TYPE_EppCommandUpdateSecDns         = 905 ;
const int EppEntity::TYPE_EppResponseDataInfoSecDns      = 906 ;

const int EppEntity::TYPE_EppSignedMarkData                       = 907;
const int EppEntity::TYPE_EppCommandCreateLaunchRegistration      = 908;
const int EppEntity::TYPE_EppResponseDataCreateLaunchRegistration = 909;
const int EppEntity::TYPE_EppCommandUpdateLaunchRegistration      = 910;
const int EppEntity::TYPE_EppCommandDeleteLaunchRegistration      = 911;
const int EppEntity::TYPE_EppCommandInfoLaunchRegistration        = 912;
const int EppEntity::TYPE_EppMarkAddress                          = 913;
const int EppEntity::TYPE_EppResponseDataInfoLaunchRegistration   = 914;
const int EppEntity::TYPE_EppCommandCheckLaunchRegistration       = 915;
const int EppEntity::TYPE_EppEncodedSignedMarkData       	  = 916;
const int EppEntity::TYPE_EppResponseDataCheckClaims              = 917;
const int EppEntity::TYPE_EppAllocationToken                      = 918;

const int EppEntity::TYPE_EppCommandInfoFee			  = 919;
const int EppEntity::TYPE_EppCommandCheckFee			  = 920;
const int EppEntity::TYPE_EppCommandTransferFee			  = 921;
const int EppEntity::TYPE_EppCommandCreateFee			  = 922;
const int EppEntity::TYPE_EppCommandRenewFee			  = 923;
const int EppEntity::TYPE_EppCommandUpdateFee			  = 924;

const int EppEntity::TYPE_EppResponseInfoFee			  = 925;
const int EppEntity::TYPE_EppResponseCheckFee			  = 926;
const int EppEntity::TYPE_EppResponseTransferFee		  = 927;
const int EppEntity::TYPE_EppResponseCreateFee			  = 928;
const int EppEntity::TYPE_EppResponseRenewFee			  = 929;
const int EppEntity::TYPE_EppResponseUpdateFee			  = 930;
const int EppEntity::TYPE_EppResponseDeleteFee			  = 931;

const int EppEntity::TYPE_EppChangePollExt			  = 932;

const int EppEntity::TYPE_CNDomainCreateExt			  = 933;
const int EppEntity::TYPE_CNDomainInfoExt			  = 934;
const int EppEntity::TYPE_CNDomainUpdateExt			  = 935;
const int EppEntity::TYPE_CNContactCreateExt			  = 936;
const int EppEntity::TYPE_CNContactInfoExt			  = 937;
const int EppEntity::TYPE_CNContactUpdateExt			  = 938;
const int EppEntity::TYPE_CNHostCreateExt			  = 939;
const int EppEntity::TYPE_CNHostInfoExt				  = 940;
const int EppEntity::TYPE_CNHostUpdateExt			  = 941;

const int EppEntity::TYPE_CNCDNExt				  = 942;

const int EppEntity::TYPE_EppIDN   = 1000;

EppEntity::~EppEntity() {}

EppEntity * EppEntity::fromXML( const DOMNode &root )
{
	return 0;
}

DOMString EppEntity::toString( const DOMString &tag )
{
	DOMImplementation* impl = DOMImplementation::getImplementation();
	DOMDocument* doc = impl->createDocument();

	DOMElement* elm = toXML(*doc, tag);
	doc->appendChild(elm);
	DOMString str = EppUtil::toString(*doc);
	delete doc;
	return str;
}

bool EppEntity::instanceOf( const int type )
{
	int obj = this->getEntityType();
	if( obj == type )
	{
		return true;
	}

	if( type == EppEntity::TYPE_EppEntity )
	{
		return true;
	}

	switch( type )
	{
		case EppEntity::TYPE_EppObject:
			return  (obj == EppEntity::TYPE_EppObject      ) ||
				(obj == EppEntity::TYPE_EppContact     ) ||
				(obj == EppEntity::TYPE_EppDomain      ) ||
				(obj == EppEntity::TYPE_EppHost        ) ||
				(obj == EppEntity::TYPE_EppSvcsub      ) ||
				(obj == EppEntity::TYPE_EppXriAuthority) ||
				(obj == EppEntity::TYPE_EppXriNumber  ) ||
				(obj == EppEntity::TYPE_EppXriName    ) ||
				(obj == EppEntity::TYPE_EppXriService ) ||
				(obj == EppEntity::TYPE_EppXriCodeString );

		case EppEntity::TYPE_EppCommand:
			return  (obj >= EppEntity::TYPE_EppCommand) &&
				(obj <  EppEntity::TYPE_EppResponse);

		case EppEntity::TYPE_EppCommandCheck:
			return  (obj >= EppEntity::TYPE_EppCommandCheck) &&
				(obj <= EppEntity::TYPE_EppCommandCheckXriCodeString );

		case EppEntity::TYPE_EppCommandDelete:
			return  (obj >= EppEntity::TYPE_EppCommandDelete) &&
				(obj <= EppEntity::TYPE_EppCommandDeleteXriCodeString );

		case EppEntity::TYPE_EppCommandInfo:
			return  (obj >= EppEntity::TYPE_EppCommandInfo) &&
				(obj <= EppEntity::TYPE_EppCommandInfoXriCodeString );

		case EppEntity::TYPE_EppCommandRenew:
			return  (obj >= EppEntity::TYPE_EppCommandRenew) &&
				(obj <= EppEntity::TYPE_EppCommandRenewXriCodeString );

		case EppEntity::TYPE_EppCommandTransfer:
			return  (obj >= EppEntity::TYPE_EppCommandTransfer) &&
				(obj <= EppEntity::TYPE_EppCommandTransferXriCodeString );

		case EppEntity::TYPE_EppCommandUpdate:
			return  (obj >= EppEntity::TYPE_EppCommandUpdate) &&
				(obj <= EppEntity::TYPE_EppCommandUpdateXriCodeString );

		case EppEntity::TYPE_EppResponseData:
			return  (obj >= EppEntity::TYPE_EppResponseData) &&
				(obj <= EppEntity::TYPE_EppResponseDataPendingXriCodeString );

		case EppEntity::TYPE_EppResponseDataCreate:
			return  (obj >= EppEntity::TYPE_EppResponseDataCreate) &&
				(obj <= EppEntity::TYPE_EppResponseDataCreateXriCodeString );

		case EppEntity::TYPE_EppResponseDataCheck:
			return  (obj >= EppEntity::TYPE_EppResponseDataCheck) &&
				(obj <= EppEntity::TYPE_EppResponseDataCheckXriCodeString );

		case EppEntity::TYPE_EppResponseDataTransfer:
			return  (obj >= EppEntity::TYPE_EppResponseDataTransfer) &&
				(obj <= EppEntity::TYPE_EppResponseDataTransferXriCodeString );

		case EppEntity::TYPE_EppResponseDataRenew:
			return  (obj >= EppEntity::TYPE_EppResponseDataRenew) &&
				(obj <= EppEntity::TYPE_EppResponseDataRenewXriCodeString );

		case EppEntity::TYPE_EppResponseDataPending:
			return  (obj >= EppEntity::TYPE_EppResponseDataPending) &&
				(obj <= EppEntity::TYPE_EppResponseDataPendingXriCodeString );

		case EppEntity::TYPE_EppExtension:
			return  (obj >= EppEntity::TYPE_EppExtension) &&
				(obj <= EppEntity::TYPE_EppIDN);

		default:
			break;
	}

	return false;
}
