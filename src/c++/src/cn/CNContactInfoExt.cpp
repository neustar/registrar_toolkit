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
 * File:   CNContactInfoExt.cpp
 * Author: Santosh Kalsangrah
 * 
 * Created on August 25, 2015, 5:57 PM
 */

#include "CNContactInfoExt.hpp"
#include "EppUtil.hpp"
#include "EppEntity.hpp"

CNContactInfoExt::CNContactInfoExt()
{
	contact = null;
}

CNContactInfoExt::~CNContactInfoExt()
{
	if( contact != null )
	{
		delete contact;
		contact = null;
	}
}

void CNContactInfoExt::setContact( CNContact* const cc)
{
	if( this->contact != null )
	{
		delete contact;
	}
	this->contact = cc;
}

CNContact* CNContactInfoExt::getContact() const
{
	return this->contact;	
}

EppExtension* CNContactInfoExt::fromXML(const DOMNode& root)
{
	CNContactInfoExt * cnExt = new CNContactInfoExt();
	CNContact * cc = CNContact::fromXML(root);
	if( cc != null )
	{
		cnExt->setContact(cc);
	}
	return cnExt;
}

DOMElement* CNContactInfoExt::toXML(DOMDocument& doc, const DOMString& tag)
{
	if( this->contact->isEmpty() )
	{
		return null;
	}
	
	DOMElement* body = EppUtil::createElementNS(doc, "cnnic-contact", "infData", true, (char*) "-1.0");
	if( contact != null )
	{
		contact->toXML(doc, body);
	}
	return body;
}

DOMString CNContactInfoExt::toString()
{
	return EppEntity::toString(DOMString("cnnic-contact:infData"));
}

int CNContactInfoExt::getEntityType()
{
	return EppEntity::TYPE_CNContactInfoExt;
}
