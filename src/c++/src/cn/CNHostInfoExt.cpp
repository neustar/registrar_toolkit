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
 * File:   CNHostInfoExt.cpp
 * Author: Santosh Kalsangrah
 * 
 * Created on August 25, 2015, 5:58 PM
 */

#include "CNHostInfoExt.hpp"
#include "EppUtil.hpp"
#include "EppEntity.hpp"

CNHostInfoExt::CNHostInfoExt()
{
	purveyor = null;
}

CNHostInfoExt::~CNHostInfoExt(){ }

const DOMString& CNHostInfoExt::getPurveyor() const
{
	return this->purveyor;
}

void CNHostInfoExt::setPurveyor(const DOMString& pur)
{
	this->purveyor = pur;
}

EppExtension* CNHostInfoExt::fromXML(const DOMNode& root)
{
	DOMNodeList* list = root.getChildNodes();

	if ( NULL == list )
	{
		return null;
	}
	CNHostInfoExt * cnExt = new CNHostInfoExt();

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

		if ( name.equals("purveyor") || name.equals("cnnic-host:purveyor") )
		{
			cnExt->setPurveyor(EppUtil::getText(*node));
			continue;
		}
	}
	return cnExt;
}

DOMElement* CNHostInfoExt::toXML(DOMDocument& doc, const DOMString& tag)
{
	DOMElement* body = EppUtil::createElementNS(doc, "cnnic-host", "infData", true, (char*) "-1.0");
	if ( this->getPurveyor().length() > 0 )
	{
		DOMElement* elm = doc.createElement(XS("purveyor"));
		elm->appendChild(doc.createTextNode(this->getPurveyor()));
		body->appendChild(elm);
	}
	return body;
}

DOMString CNHostInfoExt::toString()
{
	return EppEntity::toString(DOMString("cnnic-host:infData"));
}

int CNHostInfoExt::getEntityType()
{
	return EppEntity::TYPE_CNHostInfoExt;
}

