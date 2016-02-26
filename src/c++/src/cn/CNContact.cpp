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
 * File:   CNContact.cpp
 * Author: Santosh Kalsangrah
 * 
 * Created on August 25, 2015, 5:57 PM
 */

#include "CNContact.hpp"
#include "EppUtil.hpp"

CNContact::CNContact()
{
	this->contactType = null;
	this->contactID = null;
	this->contactIDType = null;
	this->mobile = null;
	this->purveyor = null;
}

CNContact::~CNContact(){ }

const DOMString& CNContact::getContactType() const
{
	return this->contactType;
}

void CNContact::setContactType(const DOMString& cType)
{
	this->contactType = cType;
}

const DOMString& CNContact::getContactID() const
{
	return this->contactID;
}

void CNContact::setContactID(const DOMString& id)
{
	this->contactID = id;
}

const DOMString& CNContact::getContactIDType() const
{
	return this->contactIDType;
}

void CNContact::setContactIDType(const DOMString& idType)
{
	this->contactIDType = idType;
}

const DOMString& CNContact::getMobile() const
{
	return this->mobile;
}

void CNContact::setMobile(const DOMString& mobile)
{
	this->mobile = mobile;
}

const DOMString& CNContact::getPurveyor() const
{
	return this->purveyor;
}

void CNContact::setPurveyor(const DOMString& pur)
{
	this->purveyor = pur;
}
const bool CNContact::isEmpty() const
{
	return not ( contactType.isNotNull() 
			|| contactID.isNotNull() 
			|| contactIDType.isNotNull() 
			|| mobile.isNotNull() 
			|| purveyor.isNotNull() );
}

CNContact * CNContact::fromXML( const DOMNode& root )
{
	DOMNodeList* list = root.getChildNodes();

	if ( NULL == list )
	{
		return null;
	}

	CNContact * cc = new CNContact();
	
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

		if ( name.equals("type") || name.equals("cnnic-contact:type") )
		{
			cc->setContactType(EppUtil::getText(*node));
			continue;
		}

		if ( name.equals("mobile") || name.equals("cnnic-contact:mobile") )
		{
			cc->setMobile(EppUtil::getText(*node));
			continue;
		}

		if ( name.equals("purveyor") || name.equals("cnnic-contact:purveyor") )
		{
			cc->setPurveyor(EppUtil::getText(*node));
			continue;
		}

		if ( name.equals("contact") || name.equals("cnnic-contact:contact") )
		{
			cc->setContactID(EppUtil::getText(*node));
			cc->setContactIDType(((DOMElement*) node)->getAttribute(XS("type")));
			continue;
		}
	}
	return cc;
}

void CNContact::toXML( DOMDocument &doc, DOMElement* inElement )
{
	if ( NULL == inElement )
	{
		return;
	}

	if ( this->getContactType().length() > 0 )
	{
		DOMElement* elm = doc.createElement(XS("type"));
		elm->appendChild(doc.createTextNode(this->getContactType()));
		inElement->appendChild(elm);
	}
	if ( this->getContactID().length() > 0 )
	{
		DOMElement* elm = doc.createElement(XS("contact"));
		elm->appendChild(doc.createTextNode(this->getContactID()));
		if ( this->getContactIDType().length() > 0 )
		{
			elm->setAttribute(XS("type"), this->getContactIDType());
		}
		inElement->appendChild(elm);
	}
	if ( this->getPurveyor().length() > 0 )
	{
		DOMElement* elm = doc.createElement(XS("purveyor"));
		elm->appendChild(doc.createTextNode(this->getPurveyor()));
		inElement->appendChild(elm);
	}
	if ( this->getMobile().length() > 0 )
	{

		DOMElement* elm = doc.createElement(XS("mobile"));
		elm->appendChild(doc.createTextNode(this->getMobile()));
		inElement->appendChild(elm);
	}
}
