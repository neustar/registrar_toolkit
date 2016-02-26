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
 * File:   CNDomainCreateExt.cpp
 * Author: Santosh Kalsangrah
 * 
 * Created on August 25, 2015, 5:58 PM
 */

#include "CNDomainCreateExt.hpp"
#include "EppUtil.hpp"
#include "EppEntity.hpp"

CNDomainCreateExt::CNDomainCreateExt()
{
	domain = null;
}

CNDomainCreateExt::~CNDomainCreateExt()
{
	if( domain != null )
	{
		delete domain;
		domain = null;
	}
}

void CNDomainCreateExt::setDomain( CNDomain* const cd)
{
	if( this->domain != null )
	{
		delete domain;
	}
	this->domain = cd;
}

CNDomain* CNDomainCreateExt::getDdomain() const
{
	return this->domain;
}

EppExtension* CNDomainCreateExt::fromXML(const DOMNode& root)
{
	CNDomainCreateExt * cnExt = new CNDomainCreateExt();
	CNDomain * cd = CNDomain::fromXML(root);
	if( cd != null )
	{
		cnExt->setDomain(cd);
	}
	return cnExt;
}

DOMElement* CNDomainCreateExt::toXML(DOMDocument& doc, const DOMString& tag)
{
	DOMElement* body = EppUtil::createElementNS(doc, "cnnic-domain", "create", true, (char*) "-1.0");
	if( domain != null )
	{
		domain->toXML(doc, body);
	}
	return body;
}

DOMString CNDomainCreateExt::toString()
{
	return EppEntity::toString(DOMString("cnnic-domain:create"));
}

int CNDomainCreateExt::getEntityType()
{
	return EppEntity::TYPE_CNDomainCreateExt;
}

