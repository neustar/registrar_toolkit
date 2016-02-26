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
 * File:   CNContactUpdateExt.cpp
 * Author: Santosh Kalsangrah
 * 
 * Created on August 25, 2015, 5:57 PM
 */

#include "CNContactUpdateExt.hpp"
#include "EppUtil.hpp"
#include "EppEntity.hpp"
#include "CNContact.hpp"

CNContactUpdateExt::CNContactUpdateExt()
{
	contactAdd = null;
	contactRem = null;
	contactChg = null;
}

CNContactUpdateExt::~CNContactUpdateExt()
{
	if ( null != contactAdd )
	{
		delete contactAdd;
		contactAdd = null;
	}

	if ( null != contactRem )
	{
		delete contactRem;
		contactRem = null;
	}

	if ( null != contactChg )
	{
		delete contactChg;
		contactChg = null;
	}
}

void CNContactUpdateExt::setAddContact( CNContact* const contact)
{
	if ( null != contactAdd )
	{
		delete contactAdd;
	}
	this->contactAdd = contact;
}

void CNContactUpdateExt::setRemContact( CNContact* const contact)
{
	if ( null != contactRem )
	{
		delete contactRem;
	}
	this->contactRem = contact;
}

void CNContactUpdateExt::setChgContact( CNContact* const contact)
{
	if ( null != contactChg )
	{
		delete contactChg;
	}
	this->contactChg = contact;
}

CNContact* CNContactUpdateExt::getAddContact() const
{
	return this->contactAdd;
}

CNContact* CNContactUpdateExt::getRemContact() const
{
	return this->contactRem;
}

CNContact* CNContactUpdateExt::getChgContact() const
{
	return this->contactChg;
}

EppExtension* CNContactUpdateExt::fromXML(const DOMNode& root)
{
	DOMNodeList* list = root.getChildNodes();

	if ( NULL == list )
	{
		return null;
	}

	CNContactUpdateExt * cnExt = new CNContactUpdateExt();

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

		if ( name.equals("add") || name.equals("cnnic-contact:add") )
		{
			CNContact * cc = CNContact::fromXML(*node);
			if( cc != null )
			{
				cnExt->setAddContact(cc);
			}
			continue;
		}
		if ( name.equals("rem") || name.equals("cnnic-contact:rem") )
		{
			CNContact * cc = CNContact::fromXML(*node);
			if( cc != null )
			{
				cnExt->setRemContact(cc);
			}
			continue;
		}
		if ( name.equals("chg") || name.equals("cnnic-contact:chg") )
		{
			CNContact * cc = CNContact::fromXML(*node);
			if( cc != null )
			{
				cnExt->setChgContact(cc);
			}
			continue;
		}
	}

	return cnExt;
}

DOMElement* CNContactUpdateExt::toXML(DOMDocument& doc, const DOMString& tag)
{
	DOMElement* body = EppUtil::createElementNS(doc, "cnnic-contact", "update", true, (char*) "-1.0");
	if ( null != contactAdd )
	{
		DOMElement* elm = doc.createElement(XS("add"));
		contactAdd->toXML(doc, elm);
		body->appendChild(elm);
	}
	if ( null != contactRem )
	{
		DOMElement* elm = doc.createElement(XS("rem"));
		contactRem->toXML(doc, elm);
		body->appendChild(elm);
	}
	if ( null != contactChg )
	{
		DOMElement* elm = doc.createElement(XS("chg"));
		contactChg->toXML(doc, elm);
		body->appendChild(elm);
	}
	return body;
}

DOMString CNContactUpdateExt::toString()
{
	return EppEntity::toString(DOMString("cnnic-contact:update"));
}

int CNContactUpdateExt::getEntityType()
{
	return EppEntity::TYPE_CNContactUpdateExt;
}
