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
#include "EppCommandCheckDomain.hpp"
#include "EppUtil.hpp"

DOMElement* EppCommandCheckDomain::toXML( DOMDocument& doc, const DOMString& tag )
{
	DOMElement* elm;
	DOMElement* body = EppUtil::createElementNS(doc, "domain", tag);

	if( names != null )
	{
		for( unsigned int i = 0; i < names->size(); i++ )
		{
			DOMString name = names->elementAt(i);
			elm = doc.createElement(XS("name"));
			elm->appendChild(doc.createTextNode(name));
			body->appendChild(elm);
		}
	}

	return toXMLCommon(doc, tag, *body);
}

EppCommandCheckDomain * EppCommandCheckDomain::fromXML( const DOMNode& root )
{
	EppCommandCheckDomain * cmd = new EppCommandCheckDomain();
	DOMNodeList* list = root.getChildNodes();

	for( unsigned int i = 0; i < list->getLength(); i++ )
	{
		DOMNode* node = list->item(i);
		DOMString name = node->getLocalName();

		if( name.isNull() )
		{
			name = node->getNodeName();
		}
		if( name.isNull() )
		{
			continue;
		}
		//if( name.equals("name") )
		if( name.equals("name") || name.equals("domain:name") )
		{
			DOMString domain = EppUtil::getText(*node);
			cmd->addName(domain);
		}
	}

	return cmd;
}
