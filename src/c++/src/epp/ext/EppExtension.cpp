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
#include "EppExtension.hpp"
#include "EppUtil.hpp"
#include "EppCommandCreateSecDns.hpp"
#include "EppCommandUpdateSecDns.hpp"
#include "EppResponseDataInfoSecDns.hpp"
#include "EppCommandCreateLaunchRegistration.hpp"
#include "EppResponseDataCreateLaunchRegistration.hpp"
#include "EppCommandUpdateLaunchRegistration.hpp"
#include "EppCommandDeleteLaunchRegistration.hpp"
#include "EppCommandInfoLaunchRegistration.hpp"
#include "EppResponseDataInfoLaunchRegistration.hpp"
#include "EppCommandCheckLaunchRegistration.hpp"

#include "EppCommandCheckFee.hpp"
#include "EppCommandInfoFee.hpp"
#include "EppCommandCreateFee.hpp"
#include "EppCommandRenewFee.hpp"
#include "EppCommandUpdateFee.hpp"
#include "EppCommandTransferFee.hpp"

#include "EppResponseCheckFee.hpp"
#include "EppResponseInfoFee.hpp"
#include "EppResponseCreateFee.hpp"
#include "EppResponseRenewFee.hpp"
#include "EppResponseUpdateFee.hpp"
#include "EppResponseTransferFee.hpp"
#include "EppResponseDeleteFee.hpp"

#include "EppIDNData.hpp"
#include "EppResponseDataCheckClaims.hpp"
#include "EppAllocationTokenData.hpp"
#include "EppChangePollExt.hpp"

#include "CNContactCreateExt.hpp"
#include "CNContactInfoExt.hpp"
#include "CNContactUpdateExt.hpp"
#include "CNDomainCreateExt.hpp"
#include "CNDomainInfoExt.hpp"
#include "CNDomainUpdateExt.hpp"
#include "CNHostCreateExt.hpp"
#include "CNHostInfoExt.hpp"
#include "CNHostUpdateExt.hpp"

#include "CNCDNExt.hpp"

EppExtension::~EppExtension()
{
}

EppExtension * EppExtension::fromXML( const DOMNode &root )
{
	DOMString prefix = root.getPrefix();
	DOMString name = root.getLocalName();
	if( name.isNull() )
	{
		name = root.getNodeName();
	}
	if( name.isNull() )
	{
		return null;
	}

	EppExtension * entity = null;

	if(    ((prefix.isNull()) && name.equals("secDNS:create"))
	    || ((prefix.isNotNull()) && prefix.equals("secDNS") && name.equals("create")) )
	{
		unsigned short major, minor;
		major = minor = 0;
		entity = EppCommandCreateSecDns::fromXML(root);
		if( 0 == EppUtil::getDnsSecMajorMinor( root, major, minor) )
		{
			entity->secDnsVersion = setDnsSecVersion( major, minor );
		}
	}
	else if(    ((prefix.isNull()) && name.equals("secDNS:update"))
		 || ((prefix.isNotNull()) && prefix.equals("secDNS") && name.equals("update")) )
	{
		unsigned short  major, minor;
		major = minor = 0;
		entity = EppCommandUpdateSecDns::fromXML(root);
		if( 0 == EppUtil::getDnsSecMajorMinor( root, major, minor) )
		{
			entity->secDnsVersion = setDnsSecVersion( major, minor );
		}
	}
	else if(    ((prefix.isNull()) && name.equals("secDNS:infData"))
		|| ((prefix.isNotNull()) && prefix.equals("secDNS") && name.equals("infData")) )
	{
		unsigned short  major, minor;
		major = minor = 0;
		entity = EppResponseDataInfoSecDns::fromXML(root);
		if( 0 == EppUtil::getDnsSecMajorMinor( root, major, minor) )
		{
			entity->secDnsVersion = setDnsSecVersion( major, minor );
		}
	}
	else if(    ((prefix.isNull()) && name.equals("launch:create"))
		|| ((prefix.isNotNull()) && prefix.equals("launch") && name.equals("create")) )
	{
		entity = EppCommandCreateLaunchRegistration::fromXML(root);
	}
	else if(    ((prefix.isNull()) && name.equals("launch:creData"))
		|| ((prefix.isNotNull()) && prefix.equals("launch") && name.equals("creData")) )
	{
		entity = EppResponseDataCreateLaunchRegistration::fromXML(root);
	}
	else if(    ((prefix.isNull()) && name.equals("launch:update"))
		|| ((prefix.isNotNull()) && prefix.equals("launch") && name.equals("update")) )
	{
		entity = EppCommandUpdateLaunchRegistration::fromXML(root);
	}
	else if(    ((prefix.isNull()) && name.equals("launch:delete"))
		|| ((prefix.isNotNull()) && prefix.equals("launch") && name.equals("delete")) )
	{
		entity = EppCommandDeleteLaunchRegistration::fromXML(root);
	}
	else if(    ((prefix.isNull()) && name.equals("launch:info"))
		|| ((prefix.isNotNull()) && prefix.equals("launch") && name.equals("info")) )
	{
		entity = EppCommandInfoLaunchRegistration::fromXML(root);
	}
	else if(    ((prefix.isNull()) && name.equals("launch:infData"))
		|| ((prefix.isNotNull()) && prefix.equals("launch") && name.equals("infData")) )
	{
		entity = EppResponseDataInfoLaunchRegistration::fromXML(root);
	}
	else if(    ((prefix.isNull()) && name.equals("launch:check"))
		|| ((prefix.isNotNull()) && prefix.equals("launch") && name.equals("check")) )
	{
		entity = EppCommandCheckLaunchRegistration::fromXML(root); // TODO: One day we would need to pass correct ns name here
	}
	else if ( ((prefix.isNull()) && name.equals("idn:data"))
		|| ((prefix.isNotNull()) && prefix.equals("idn") && name.equals("data")) )
	{
		entity = EppIDNData::fromXML(root);
	}
	else if(    ((prefix.isNull()) && name.equals("launch:chkData"))
		|| ((prefix.isNotNull()) && prefix.equals("launch") && name.equals("chkData")) )
	{
		entity = EppResponseDataCheckClaims::fromXML(root);
	}
	else if(    ((prefix.isNull()) && name.equals("allocationToken:allocationToken"))
		|| ((prefix.isNotNull()) && prefix.equals("allocationToken") && name.equals("allocationToken")) )
	{
		entity = EppAllocationTokenData::fromXML(root);
	}
	else if(    ((prefix.isNull()) && name.equals("allocationToken:info"))
		|| ((prefix.isNotNull()) && prefix.equals("allocationToken") && name.equals("info")) )
	{
		entity = EppAllocationTokenData::fromXML(root);
	}

	//----------------Fee Extsnsion--------------------------
	else if(    (prefix.isNull() && name.equals("fee:check")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("check")) )
	{
		entity = EppCommandCheckFee::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("fee:chkData")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("chkData")) )
	{
		entity = EppResponseCheckFee::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("fee:info")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("info")) )
	{
		entity = EppCommandInfoFee::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("fee:infData")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("infData")) )
	{
		entity = EppResponseInfoFee::fromXML(root);
	}
	// Nothing for transfer query request
	else if(    (prefix.isNull() && name.equals("fee:create")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("create")) )
	{
		entity = EppCommandCreateFee::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("fee:creData")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("creData")) )
	{
		entity = EppResponseCreateFee::fromXML(root);
	}
	// nothing in delete request
	else if(    (prefix.isNull() && name.equals("fee:renew")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("renew")) )
	{
		entity = EppCommandRenewFee::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("fee:renData")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("renData")) )
	{
		entity = EppResponseRenewFee::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("fee:transfer")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("transfer")) )
	{
		entity = EppCommandTransferFee::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("fee:trnData")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("trnData")) )
	{
		entity = EppResponseTransferFee::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("fee:update")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("update")) )
	{
		entity = EppCommandUpdateFee::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("fee:updData")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("updData")) )
	{
		entity = EppResponseUpdateFee::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("fee:delData")) ||
		(prefix.isNotNull() && prefix.equals("fee") && name.equals("delData")) )
	{
		entity = EppResponseDeleteFee::fromXML(root);
	}
	//----------------Fee Extsnsion-end----------------------
        else if(    (prefix.isNull() && name.equals("changePoll:changeData")) ||
                (prefix.isNotNull() && prefix.equals("changePoll") && name.equals("changeData")) )
        {
                entity = EppChangePollExt::fromXML(root);
        }
	//----------------CN Extsnsion-----------------------
	else if(    (prefix.isNull() && name.equals("cnnic-domain:create")) ||
		(prefix.isNotNull() && prefix.equals("cnnic-domain") && name.equals("create")) )
	{
		entity = CNDomainCreateExt::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("cnnic-domain:infData")) ||
		(prefix.isNotNull() && prefix.equals("cnnic-domain") && name.equals("infData")) )
	{
		entity = CNDomainInfoExt::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("cnnic-domain:update")) ||
		(prefix.isNotNull() && prefix.equals("cnnic-domain") && name.equals("update")) )
	{
		entity = CNDomainUpdateExt::fromXML(root);
	}

	else if(    (prefix.isNull() && name.equals("cnnic-contact:create")) ||
		(prefix.isNotNull() && prefix.equals("cnnic-contact") && name.equals("create")) )
	{
		entity = CNContactCreateExt::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("cnnic-contact:infData")) ||
		(prefix.isNotNull() && prefix.equals("cnnic-contact") && name.equals("infData")) )
	{
		entity = CNContactInfoExt::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("cnnic-contact:update")) ||
		(prefix.isNotNull() && prefix.equals("cnnic-contact") && name.equals("update")) )
	{
		entity = CNContactUpdateExt::fromXML(root);
	}

	else if(    (prefix.isNull() && name.equals("cnnic-host:create")) ||
		(prefix.isNotNull() && prefix.equals("cnnic-host") && name.equals("create")) )
	{
		entity = CNHostCreateExt::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("cnnic-host:infData")) ||
		(prefix.isNotNull() && prefix.equals("cnnic-host") && name.equals("infData")) )
	{
		entity = CNHostInfoExt::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("cnnic-host:update")) ||
		(prefix.isNotNull() && prefix.equals("cnnic-host") && name.equals("update")) )
	{
		entity = CNHostUpdateExt::fromXML(root);
	}
	else if(    (prefix.isNull() && name.equals("cdn:infData")) ||
		(prefix.isNotNull() && prefix.equals("cdn") && name.equals("infData")) )
	{
		entity = CNCDNExt::fromXML(root);
	}
	return entity;
}
