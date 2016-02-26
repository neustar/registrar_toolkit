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
#include "EppCommandUpdateDomain.hpp"
#include "EppUtil.hpp"

DOMElement* EppCommandUpdateDomain::toXML( DOMDocument& doc, const DOMString& tag )
{
	DOMElement* elm;
	DOMElement* body = EppUtil::createElementNS(doc, "domain", tag);

	if( name.isNotNull() )
	{
		elm = doc.createElement(XS("name"));
		elm->appendChild(doc.createTextNode(name));
		body->appendChild(elm);
	}
	if(    ((nsAdded != null) && (nsAdded->size() > 0))
		|| ((nsAttrAdded != null) && (nsAttrAdded->size() > 0))
		|| (contactAdded->size() > 0)
		|| (statusAdded->size() > 0) )
	{
		elm = doc.createElement(XS("add"));
		if( (nsAdded != null) && (nsAdded->size() > 0) )
		{
			nameServerToXML(doc, *elm, nsAdded);
		}
		else if( (nsAttrAdded != null) && (nsAttrAdded->size() > 0) )
		{
			nameServerToXML(doc, *elm, nsAttrAdded);
		}
		if( contactAdded->size() > 0 )
		{
			contactToXML(doc, *elm, contactAdded);
		}
		if( statusAdded->size() > 0 )
		{
			statusToXML(doc, *elm, statusAdded);
		}
		body->appendChild(elm);
	}
	if(    ((nsRemoved != null) && (nsRemoved->size() > 0) )
		|| ((nsAttrRemoved != null) && (nsAttrRemoved->size() > 0) )
		|| (contactRemoved->size() > 0)
		|| (statusRemoved->size() > 0) )
	{
		elm = doc.createElement(XS("rem"));
		if( (nsRemoved != null) && (nsRemoved->size() > 0) )
		{
			nameServerToXML(doc, *elm, nsRemoved);
		}
		else if( (nsAttrRemoved != null) && (nsAttrRemoved->size() > 0) )
		{
			nameServerToXML(doc, *elm, nsAttrRemoved);
		}
		if( contactRemoved->size() > 0 )
		{
			contactToXML(doc, *elm, contactRemoved);
		}
		if( statusRemoved->size() > 0 )
		{
			statusToXML(doc, *elm, statusRemoved);
		}
		body->appendChild(elm);
	}
	if( (newRegistrant.isNotNull()) || (newAuthInfo != null) )
	{
		elm = doc.createElement(XS("chg"));
		if( newRegistrant.isNotNull() )
		{
			DOMElement* name = doc.createElement(XS("registrant"));
			name->appendChild(doc.createTextNode(newRegistrant));
			elm->appendChild(name);
		}
		if( newAuthInfo != null )
		{
			elm->appendChild(newAuthInfo->toXML(doc, "authInfo"));
		}
		body->appendChild(elm);
	}

	return toXMLCommon(doc, tag, *body);
}

void EppCommandUpdateDomain::nameServerToXML( DOMDocument& doc, DOMElement& body, OldValueVectorOf<DOMString> * list )
{
	if( (list != null) && (list->size() > 0) )
	{
		DOMElement* elm = doc.createElement(XS("ns"));
		body.appendChild(elm);
		for( unsigned int i = 0; i < list->size(); i++ )
		{
			DOMString ns = list->elementAt(i);
			DOMElement* ens = doc.createElement(XS("hostObj"));
			ens->appendChild(doc.createTextNode(ns));
			elm->appendChild(ens);
		}
	}
}

void EppCommandUpdateDomain::nameServerToXML( DOMDocument& doc, DOMElement& body, OldValueVectorOf<EppHostAttribute *> * list )
{
	if( (list != null) && (list->size() > 0) )
	{
		bool first = true;
		DOMElement* elm = null;
		for( unsigned int i = 0; i < list->size(); i++ )
		{
			EppHostAttribute * ns = list->elementAt(i);
			if( ns != null )
			{
				if( first == true )
				{
					elm = doc.createElement(XS("ns"));
					body.appendChild(elm);
					first = false;
				}
				if( elm == null )
				{
					continue;
				}
				DOMElement* ens = ns->toXML(doc, "hostAttr");
				elm->appendChild(ens);
			}
		}
	}
}

void EppCommandUpdateDomain::contactToXML( DOMDocument& doc, DOMElement& body, OldValueVectorOf<EppContactType> *list )
{
	DOMElement* elm;
	if( list != null )
	{
		for( unsigned int i = 0; i < list->size(); i++ )
		{
			EppContactType ct  = list->elementAt(i);
			elm = ct.toXML(doc, "contact");
			body.appendChild(elm);
		}
	}
}

EppCommandUpdateDomain * EppCommandUpdateDomain::fromXML( const DOMNode& root )
{
	EppCommandUpdateDomain * cmd = null;
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
			if( cmd == null )
			{
				DOMString host = EppUtil::getText(*node);
				cmd = new EppCommandUpdateDomain(host);
			}
		}
		//else if( name.equals("add") )
		else if( name.equals("add") || name.equals("domain:add") )
		{
			if( cmd != null )
			{
				cmd->nameServerFromXML(*node, true);
				cmd->contactFromXML(*node, cmd->contactAdded);
				cmd->statusFromXML(*node, cmd->statusAdded);
			}
		}
		//else if( name.equals("rem") )
		else if( name.equals("rem") || name.equals("domain:rem") )
		{
			if( cmd != null )
			{
				cmd->nameServerFromXML(*node, false);
				cmd->contactFromXML(*node, cmd->contactRemoved);
				cmd->statusFromXML(*node, cmd->statusRemoved);
			}
		}
		//else if( name.equals("chg") )
		else if( name.equals("chg") || name.equals("domain:chg") )
		{
			DOMNodeList* tlist = node->getChildNodes();
			for( unsigned int j = 0; j < tlist->getLength(); j++ )
			{
				DOMNode* tnode = tlist->item(j);
				DOMString tname = tnode->getLocalName();

				if( tname.isNull() )
				{
					tname = tnode->getNodeName();
				}
				if( tname.isNull() )
				{
					continue;
				}
				//if( tname.equals("registrant") )
				if( tname.equals("registrant") || tname.equals("domain:registrant") )
				{
					DOMString id = EppUtil::getText(*tnode);
					if( cmd != null )
					{
						cmd->setNewRegistrant(id);
					}
				}
				//else if( tname.equals("authInfo") )
				else if( tname.equals("authInfo") || tname.equals("domain:authInfo") )
				{
					EppAuthInfo * authInfo = EppAuthInfo::fromXML(*tnode);
					if( (cmd != null) && (authInfo != null) )
					{
						cmd->setNewAuthInfo(*authInfo);
						delete authInfo;
					}
				}
			}
		}
	}
	return cmd;
}

void EppCommandUpdateDomain::nameServerFromXML( const DOMNode& root, bool addOrRem )
{
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
		//if( name.equals("ns") == false )
		if( (name.equals("ns") == false) && (name.equals("domain:ns") == false) )
		{
			continue;
		}
		DOMNodeList* clist = node->getChildNodes();
		if( clist == null )
		{
			continue;
		}
		for( unsigned int j = 0; j < clist->getLength(); j++ )
		{
			DOMNode* cnode = clist->item(j);
			DOMString cname = cnode->getLocalName();
			if( cname.isNull() )
			{
				cname = cnode->getNodeName();
			}
			if( cname.isNull() )
			{
				continue;
			}
			if( cname.equals("hostObj") || cname.equals("domain:hostObj") )
			{
				DOMString ns = EppUtil::getText(*cnode);
				if( ns.isNotNull() && ns.length() > 0 )
				{
					if( addOrRem == true )
					{
						this->addNameServer(ns);
					}
					else
					{
						this->removeNameServer(ns);
					}
				}
			}
			else if( cname.equals("hostAttr") || cname.equals("domain:hostAttr") )
			{
				EppHostAttribute * ns;
				ns = EppHostAttribute::fromXML(*cnode);
				if( ns != null )
				{
					if( addOrRem == true )
					{
						this->addNameServer(ns);
					}
					else
					{
						this->removeNameServer(ns);
					}
				}
			}
		}
	}
}

void EppCommandUpdateDomain::contactFromXML( const DOMNode& root, OldValueVectorOf<EppContactType> * contactList )
{
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
		//if( name.equals("contact") )
		if( name.equals("contact") || name.equals("domain:contact") )
		{
			EppContactType * ct = EppContactType::fromXML(*node);
			if( ct != null )
			{
				contactList->addElement(*ct);
				delete ct;
			}
		}
	}
}
