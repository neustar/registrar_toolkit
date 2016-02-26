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
 * File:   CNCDNExt.cpp
 * Author: Santosh Kalsangrah
 * 
 * Created on September 24, 2015, 5:58 PM
 */

#include "CNCDNExt.hpp"
#include "EppUtil.hpp"
#include "EppEntity.hpp"

CNCDNExt::CNCDNExt()
{
	this->VCDN = null;
	this->VCDNPunycode = null;
}

CNCDNExt::~CNCDNExt()
{
	if ( this->VCDN != null )
	{
		delete this->VCDN;
		this->VCDN = null;
	}
	if ( this->VCDNPunycode != null )
	{
		delete this->VCDNPunycode;
		this->VCDNPunycode = null;
	}
}

const DOMString& CNCDNExt::getOCDNPunycode() const
{
	return this->OCDNPunycode;
}

void CNCDNExt::setOCDNPunycode(const DOMString& ocdnPunycode)
{
	this->OCDNPunycode = ocdnPunycode;
}

const DOMString& CNCDNExt::getSCDN() const
{
	return this->SCDN;
}

void CNCDNExt::setSCDN(const DOMString& scdn)
{
	this->SCDN = scdn;
}

const DOMString& CNCDNExt::getSCDNPunycode() const
{
	return this->SCDNPunycode;
}

void CNCDNExt::setSCDNPunycode(const DOMString& scdnPunycode)
{
	this->SCDNPunycode = scdnPunycode;
}

const DOMString& CNCDNExt::getTCDN() const
{
	return this->TCDN;
}

void CNCDNExt::setTCDN(const DOMString& tcdn)
{
	this->TCDN = tcdn;
}

const DOMString& CNCDNExt::getTCDNPunycode() const
{
	return this->TCDNPunycode;
}

void CNCDNExt::setTCDNPunycode(const DOMString& tcdnPunycode)
{
	this->TCDNPunycode = tcdnPunycode;
}

OldValueVectorOf<DOMString>* CNCDNExt::getVCDN() const
{
	return this->VCDN;
}

void CNCDNExt::setVCDN( OldValueVectorOf<DOMString>*& vcdn)
{
	this->VCDN = vcdn;
}

void CNCDNExt::addVCDN(const DOMString& add)
{
	if ( this->VCDN == null )
	{
		this->VCDN = new OldValueVectorOf<DOMString>(1);
	}
	this->VCDN->addElement(add);
}

OldValueVectorOf<DOMString>* CNCDNExt::getVCDNPunycode() const
{
	return this->VCDNPunycode;
}

void CNCDNExt::setVCDNPunycode( OldValueVectorOf<DOMString>*& vcdnPunycode)
{
	this->VCDNPunycode = vcdnPunycode;
}

void CNCDNExt::addVCDNPunnyCode(const DOMString& add)
{
	if ( this->VCDNPunycode == null )
	{
		this->VCDNPunycode = new OldValueVectorOf<DOMString>(1);
	}
	this->VCDNPunycode->addElement(add);
}

EppExtension* CNCDNExt::fromXML(const DOMNode& root)
{
	DOMNodeList* list = root.getChildNodes();
	if ( NULL == list )
	{
		return null;
	}
	CNCDNExt * cnExt = new CNCDNExt();

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
		if ( name.equals("OCDNPunycode") || name.equals("cdn:OCDNPunycode") )
		{
			cnExt->setOCDNPunycode(EppUtil::getText(*node));
			continue;
		}
		if ( name.equals("SCDN") || name.equals("cdn:SCDN") )
		{
			cnExt->setSCDN(EppUtil::getText(*node));
			continue;
		}
		if ( name.equals("SCDNPunycode") || name.equals("cdn:SCDNPunycode") )
		{
			cnExt->setSCDNPunycode(EppUtil::getText(*node));
			continue;
		}
		if ( name.equals("TCDN") || name.equals("cdn:TCDN") )
		{
			cnExt->setTCDN(EppUtil::getText(*node));
			continue;
		}
		if ( name.equals("TCDNPunycode") || name.equals("cdn:TCDNPunycode") )
		{
			cnExt->setTCDNPunycode(EppUtil::getText(*node));
			continue;
		}
		if ( name.equals("VCDNList") || name.equals("cdn:VCDNList") )
		{
			DOMNodeList* vList = node->getChildNodes();
			if ( NULL == vList )
			{
				continue;
			}
			for ( unsigned int i = 0; i < vList->getLength(); i++ )
			{
				DOMNode* vNode = vList->item(i);
				if ( NULL == vNode )
				{
					continue;
				}
				DOMString vName = vNode->getLocalName();
				if ( vName.isNull() )
				{
					vName = vNode->getNodeName();
				}
				if ( vName.isNull() )
				{
					continue;
				}
				if ( vName.equals("VCDN") || vName.equals("cdn:VCDN") )
				{
					if ( null == cnExt->VCDN )
					{
						cnExt->VCDN = new OldValueVectorOf<DOMString>(1);
					}
					cnExt->VCDN->addElement(EppUtil::getText(*vNode));
					continue;
				}
				if ( vName.equals("VCDNPunycode") || vName.equals("cdn:VCDNPunycode") )
				{
					if ( null == cnExt->VCDNPunycode )
					{
						cnExt->VCDNPunycode = new OldValueVectorOf<DOMString>(1);
					}
					cnExt->VCDNPunycode->addElement(EppUtil::getText(*vNode));
					continue;
				}
			}
			continue;
		}
	}
	return cnExt;

}

DOMElement* CNCDNExt::toXML(DOMDocument& doc, const DOMString& tag)
{
	DOMElement* body = EppUtil::createElementNS(doc, "cdn", "infData", true, (char*) "-1.0");

	if ( this->getOCDNPunycode().length() > 0 )
	{
		DOMElement* elm = doc.createElement(XS("OCDNPunycode"));
		elm->appendChild(doc.createTextNode(this->getOCDNPunycode()));
		body->appendChild(elm);
	}
	if ( this->getSCDN().length() > 0 )
	{
		DOMElement* elm = doc.createElement(XS("SCDN"));
		elm->appendChild(doc.createTextNode(this->getSCDN()));
		body->appendChild(elm);
	}
	if ( this->getSCDNPunycode().length() > 0 )
	{
		DOMElement* elm = doc.createElement(XS("SCDNPunycode"));
		elm->appendChild(doc.createTextNode(this->getSCDNPunycode()));
		body->appendChild(elm);
	}
	if ( this->getTCDN().length() > 0 )
	{
		DOMElement* elm = doc.createElement(XS("TCDN"));
		elm->appendChild(doc.createTextNode(this->getTCDN()));
		body->appendChild(elm);
	}
	if ( this->getTCDNPunycode().length() > 0 )
	{
		DOMElement* elm = doc.createElement(XS("TCDNPunycode"));
		elm->appendChild(doc.createTextNode(this->getTCDNPunycode()));
		body->appendChild(elm);
	}

	DOMElement* vCDNListElm = null;


	OldValueVectorOf<DOMString>* vCDN = this->getVCDN();
	if ( vCDN != null )
	{
		if ( vCDN->size() > 0 )
		{
			vCDNListElm = doc.createElement(XS("VCDNList"));
			for ( unsigned int i = 0; i < vCDN->size(); i++ )
			{
				DOMElement* elm = doc.createElement(XS("VCDN"));
				elm->appendChild(doc.createTextNode(vCDN->elementAt(i)));
				vCDNListElm->appendChild(elm);
			}
		}
	}
	OldValueVectorOf<DOMString>* vCDNP = this->getVCDNPunycode();
	if ( vCDNP != null )
	{
		if ( vCDNP->size() > 0 )
		{
			if ( vCDNListElm == null )
			{
				vCDNListElm = doc.createElement(XS("VCDNList"));
			}
			for ( unsigned int i = 0; i < vCDN->size(); i++ )
			{
				DOMElement* elm = doc.createElement(XS("VCDNPunycode"));
				elm->appendChild(doc.createTextNode(vCDNP->elementAt(i)));
				vCDNListElm->appendChild(elm);
			}
		}
	}
	if ( vCDNListElm != null )
	{
		body->appendChild(vCDNListElm);
	}
	return body;
}

DOMString CNCDNExt::toString()
{
	return EppEntity::toString(DOMString("cdn:infData"));
}

int CNCDNExt::getEntityType()
{
	return EppEntity::TYPE_CNCDNExt;
}
