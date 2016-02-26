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
 * File:   CNDomain.cpp
 * Author: Santosh Kalsangrah
 * 
 * Created on August 25, 2015, 5:57 PM
 */

#include "CNDomain.hpp"
#include "EppUtil.hpp"

CNDomain::CNDomain()
{
	this->domainType = null;
	this->purveyor   = null;
}

CNDomain::~CNDomain(){ }

const DOMString& CNDomain::getDomainType() const
{
	return this->domainType;
}

void CNDomain::setDomainType(const DOMString& dType)
{
	this->domainType = dType;
}

const DOMString& CNDomain::getPurveyor() const
{
	return this->purveyor;
}

void CNDomain::setPurveyor(const DOMString& pur)
{
	this->purveyor = pur;
}

CNDomain * CNDomain::fromXML( const DOMNode& root )
{
	DOMNodeList* list = root.getChildNodes();

	if ( NULL == list )
	{
		return null;
	}
	
	CNDomain * cd = new CNDomain();
	
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

		if ( name.equals("type") || name.equals("cnnic-domain:type") )
		{
			cd->setDomainType(EppUtil::getText(*node));
			continue;
		}

		if ( name.equals("purveyor") || name.equals("cnnic-domain:purveyor") )
		{
			cd->setPurveyor(EppUtil::getText(*node));
			continue;
		}
	}
	return cd;
}

void CNDomain::toXML( DOMDocument &doc, DOMElement* inElement )
{
	if ( NULL == inElement )
	{
		return;
	}

	if ( this->getDomainType().length() > 0 )
	{
		DOMElement* elm = doc.createElement(XS("type"));
		elm->appendChild(doc.createTextNode(this->getDomainType()));
		inElement->appendChild(elm);
	}

	if ( this->getPurveyor().length() > 0 )
	{
		DOMElement* elm = doc.createElement(XS("purveyor"));
		elm->appendChild(doc.createTextNode(this->getPurveyor()));
		inElement->appendChild(elm);
	}
}