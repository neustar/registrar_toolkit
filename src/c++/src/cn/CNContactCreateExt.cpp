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
 * File:   CNContactCreateExt.cpp
 * Author: Santosh Kalsangrah
 * 
 * Created on August 25, 2015, 5:57 PM
 */

#include "CNContactCreateExt.hpp"
#include "EppUtil.hpp"
#include "EppEntity.hpp"

CNContactCreateExt::CNContactCreateExt()
{
	contact = null;
}

CNContactCreateExt::~CNContactCreateExt()
{
	if( contact != null )
	{
		delete contact;
		contact = null;
	}
}

void CNContactCreateExt::setContact( CNContact* const cc)
{
	if( this->contact != null )
	{
		delete contact;
	}
	this->contact = cc;
}

CNContact* CNContactCreateExt::getContact() const
{
	return this->contact;	
}

EppExtension* CNContactCreateExt::fromXML(const DOMNode& root)
{
	CNContactCreateExt * cnExt = new CNContactCreateExt();
	CNContact * cc = CNContact::fromXML(root);
	if( cc != null )
	{
		cnExt->setContact(cc);
	}
	return cnExt;
}

DOMElement* CNContactCreateExt::toXML(DOMDocument& doc, const DOMString& tag)
{
	DOMElement* body = EppUtil::createElementNS(doc, "cnnic-contact", "create", true, (char*) "-1.0");
	if( contact != null )
	{
		contact->toXML(doc, body);
	}
	return body;
}

DOMString CNContactCreateExt::toString()
{
	return EppEntity::toString(DOMString("cnnic-contact:create"));
}

int CNContactCreateExt::getEntityType()
{
	return EppEntity::TYPE_CNContactCreateExt;
}
