/*******************************************************************************
 * The MIT License (MIT)
 *  
 * Copyright (c) 2015 Neustar Inc.
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

/* 
 * File:   CNDomainUpdateExt.cpp
 * Author: Santosh Kalsangrah
 * 
 * Created on August 25, 2015, 5:58 PM
 */

#include "CNDomainUpdateExt.hpp"
#include "EppUtil.hpp"
#include "EppEntity.hpp"

CNDomainUpdateExt::CNDomainUpdateExt()
{
	DomainChg = null;
}

CNDomainUpdateExt::~CNDomainUpdateExt()
{
	if ( null != DomainChg )
	{
		delete DomainChg;
		DomainChg = null;
	}
}

void CNDomainUpdateExt::setChgDomain( CNDomain* const domain )
{
	if ( null != DomainChg )
	{
		delete DomainChg;
	}
	this->DomainChg = domain;
}

const CNDomain* CNDomainUpdateExt::getChgDomain() const
{
	return this->DomainChg;
}

EppExtension* CNDomainUpdateExt::fromXML(const DOMNode& root)
{
	DOMNodeList* list = root.getChildNodes();

	if ( NULL == list )
	{
		return null;
	}

	CNDomainUpdateExt * cnExt = new CNDomainUpdateExt();

	for ( unsigned int i = 0; i < list->getLength(); i++ )
	{
		DOMNode* node = list->item(i);

		if ( NULL == node )
		{
			continue;
		}
		DOMString name = node->getLocalName();
		if ( name.isNull() )
		{
			name = node->getNodeName();
		}
		if ( name.isNull() )
		{
			continue;
		}

		if ( name.equals("chg") || name.equals("cnnic-domain:chg") )
		{
			CNDomain* cd = CNDomain::fromXML(*node);
			if( cd != null )
			{
				cnExt->setChgDomain(cd);
			}
			continue;
		}
	}
	return cnExt;
}

DOMElement* CNDomainUpdateExt::toXML(DOMDocument& doc, const DOMString& tag)
{
	DOMElement* body = EppUtil::createElementNS(doc, "cnnic-domain", "update", true, (char*) "-1.0");

	if ( null != DomainChg )
	{
		DOMElement* elm = doc.createElement(XS("chg"));
		DomainChg->toXML(doc, elm);
		body->appendChild(elm);
	}
	return body;
}

DOMString CNDomainUpdateExt::toString()
{
	return EppEntity::toString(DOMString("cnnic-domain:update"));
}

int CNDomainUpdateExt::getEntityType()
{
	return EppEntity::TYPE_CNDomainUpdateExt;
}

