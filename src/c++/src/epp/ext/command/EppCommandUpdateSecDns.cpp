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
#include "stdio.h"
#include "stdlib.h"
#include "EppCommandUpdateSecDns.hpp"
#include "EppUtil.hpp"

const int EppCommandUpdateSecDns::OP_ADD	= 1;
const int EppCommandUpdateSecDns::OP_CHG	= 2;
const int EppCommandUpdateSecDns::OP_REM	= 3;

EppCommandUpdateSecDns * EppCommandUpdateSecDns::fromXML( const DOMNode &root )
{
	EppCommandUpdateSecDns * cmd  = new EppCommandUpdateSecDns();
	if( cmd == null )
	{
		return null;
	}

	DOMNodeList* list  = root.getChildNodes();
	for( unsigned int i = 0; i < list->getLength(); i++ )
	{
		DOMNode* node  = list->item(i);
		DOMString name = node->getLocalName();

		if( name.isNull() )
		{
			name = node->getNodeName();
		}
		if( name.isNull() )
		{
			continue;
		}
		if( name.substringData(0, 7).equals("secDNS:") )
		{
			name = name.substringData(7, name.length() - 7);
		}
		if( name.equals("add") )
		{
			cmd->fromXML(*node, EppCommandUpdateSecDns::OP_ADD);
		}
		else if( name.equals("chg") )
		{
			cmd->fromXML(*node, EppCommandUpdateSecDns::OP_CHG);
		}
		else if( name.equals("rem") )
		{
			cmd->fromXML(*node, EppCommandUpdateSecDns::OP_REM);
		}

	}
	DOMElement * elm = (DOMElement *) &root;
	DOMString flag   = elm->getAttribute(XS("urgent"));

	if( (flag.isNotNull()) && (flag.equals("true") || flag.equals("1")) )
	{
		cmd->setUrgentFlag(true);
	}

	return cmd;
}

void EppCommandUpdateSecDns::fromXML( const DOMNode &root, const int op )
{
	DOMNodeList* list  = root.getChildNodes();
	for( unsigned int i = 0; i < list->getLength(); i++ )
	{
		DOMNode* node  = list->item(i);
		DOMString name = node->getLocalName();

		if( name.isNull() )
		{
			name = node->getNodeName();
		}
		if( name.isNull() )
		{
			continue;
		}
		if( name.substringData(0, 7).equals("secDNS:") )
		{
			name = name.substringData(7, name.length() - 7);
		}
		if( name.equals("dsData") )
		{
			EppSecDnsDsData * ds = EppSecDnsDsData::fromXML(*node);
			if( ds != null )
			{
				if( op == EppCommandUpdateSecDns::OP_ADD )
				{
					this->add(*ds);
				}
				else if( op == EppCommandUpdateSecDns::OP_CHG )
				{
					this->change(*ds);
				}
				else if( op == EppCommandUpdateSecDns::OP_REM )
				{
					this->remove(*ds);
				}
				delete ds;
			}
		}
#if defined(EPPDNSSEC_COMPT)
		else if( name.equals("keyTag") )
		{
			DOMString value = EppUtil::getText(*node);

			if( value.isNotNull() && value.length() )
			{
				if( op == EppCommandUpdateSecDns::OP_REM )
				{
					char * p = value.transcode();
					int val = ::atoi(p);
					XercesString::Delete(p);
					EppSecDnsDsData * ds = new EppSecDnsDsData();
					ds->setKeyTag(val);
					this->remove(*ds);
					delete ds;
				}
			}
		}
#endif
		else if( name.equals("all") && EppCommandUpdateSecDns::OP_REM == op )
		{ /*rfc-5910*/
			this->removeAllDS = EppUtil::getTextAsBool(*node);
		}
		else if( name.equals("maxSigLife") && EppCommandUpdateSecDns::OP_CHG == op )
		{ /*rfc-5910*/
			DOMString value = EppUtil::getText(*node);
			if( value.isNotNull() && value.length() )
			{
				char * p = value.transcode();
				if( NULL != p )
				{
					this->maxSigLife = ::atoi(p);
					XercesString::Delete(p);
				}
			}
		}
		else if( name.equals("keyData") )
		{
			EppSecDnsKeyData *kd = EppSecDnsKeyData::fromXML(*node);
			if( kd != null )
			{
				if( op == EppCommandUpdateSecDns::OP_ADD )
				{
					this->add(*kd);
				}
				else if( op == EppCommandUpdateSecDns::OP_REM )
				{
					this->remove(*kd);
				}
				delete kd;
			}
			this->keyDataPresent = true;
		}
	}
}

DOMElement* EppCommandUpdateSecDns::toXML( DOMDocument& doc, const DOMString& tag )
{
	DOMElement* body = EppUtil::createElementNS(doc, "secDNS", (tag.isNull()) ? "update" : tag);
	if( this->urgent == true )
	{
		body->setAttribute(XS("urgent"), XS("true"));
	}
	if( (this->addDsDataList != null) && (this->addDsDataList->size() > 0) )
	{
		this->toXML(doc, *body, "add", this->addDsDataList, null);
	}
	if( (this->chgDsDataList != null) && (this->chgDsDataList->size() > 0) )
	{
		this->toXML(doc, *body, "chg", this->chgDsDataList, null);
	}
	if( this->maxSigLife != -1 )
	{
		DOMElement* elm;
		elm = doc.createElement(XS("maxSigLife"));
		char cBuffer[32];
		sprintf(cBuffer, "%d", this->maxSigLife);
		elm->appendChild(doc.createTextNode(XS(cBuffer)));

		body->appendChild(elm);
	}
	if( ( this->remKeyTagList != null) && (this->remKeyTagList->size() > 0) )
	{
		if( this->removeAllDS == true )
		{
			DOMElement* rem, *elm;
			rem = doc.createElement(XS("rem"));
			elm = doc.createElement(XS("all"));
			elm->appendChild(doc.createTextNode(XS("true")));
			rem->appendChild(elm);
			body->appendChild(rem);
		}
		else
		{
			this->toXML(doc, *body, "rem", this->remKeyTagList, null );
		}
	}
	return body;
}

bool EppCommandUpdateSecDns::toXML( DOMDocument& doc, DOMElement& root, const DOMString& tag, OldValueVectorOf<EppSecDnsDsData> * dsDataList, ValueVectorOf<int> * keyTagList )
{
	DOMElement* elm;
	DOMElement* body;

	if( (dsDataList != null) && (dsDataList->size() > 0) )
	{
		body = doc.createElement(tag);
		for( unsigned int i = 0; i < dsDataList->size(); i++ )
		{
			EppSecDnsDsData ds = dsDataList->elementAt(i);
			elm = ds.toXML(doc, "dsData");
			body->appendChild(elm);
		}
		root.appendChild(body);
		return true;
	}
	if( (keyTagList != null) && (keyTagList->size() > 0) )
	{
		body = doc.createElement(tag);
		for( unsigned int i = 0; i < keyTagList->size(); i++ )
		{
			char buf[32];
			int val = keyTagList->elementAt(i);
			(void) ::sprintf(buf, "%d", val);
			elm = doc.createElement(XS("keyTag"));
			elm->appendChild(doc.createTextNode(XS(buf)));
			body->appendChild(elm);
		}
		root.appendChild(body);
		return true;
	}
	return false;
}
