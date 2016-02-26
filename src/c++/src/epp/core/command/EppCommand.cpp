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
#include "EppCommand.hpp"
#include "EppCommandLogin.hpp"
#include "EppCommandLogout.hpp"
#include "EppCommandPoll.hpp"
#include "EppCommandCreate.hpp"
#include "EppCommandDelete.hpp"
#include "EppCommandDeleteContact.hpp"
#include "EppCommandDeleteDomain.hpp"
#include "EppCommandDeleteHost.hpp"
#include "EppCommandDeleteSvcsub.hpp"
#if defined(RTK_SUPPORT_XRI)
#include "EppCommandDeleteXriAuthority.hpp"
#include "EppCommandDeleteXriName.hpp"
#include "EppCommandDeleteXriNumber.hpp"
#include "EppCommandDeleteXriService.hpp"
#endif
#include "EppCommandInfo.hpp"
#include "EppCommandInfoContact.hpp"
#include "EppCommandInfoDomain.hpp"
#include "EppCommandInfoHost.hpp"
#include "EppCommandInfoSvcsub.hpp"
#if defined(RTK_SUPPORT_XRI)
#include "EppCommandInfoXriAuthority.hpp"
#include "EppCommandInfoXriName.hpp"
#include "EppCommandInfoXriNumber.hpp"
#include "EppCommandInfoXriService.hpp"
#include "EppCommandInfoXriCodeString.hpp"
#endif
#include "EppCommandCheck.hpp"
#include "EppCommandCheckContact.hpp"
#include "EppCommandCheckDomain.hpp"
#include "EppCommandCheckHost.hpp"
#include "EppCommandCheckSvcsub.hpp"
#if defined(RTK_SUPPORT_XRI)
#include "EppCommandCheckXriAuthority.hpp"
#include "EppCommandCheckXriName.hpp"
#include "EppCommandCheckXriNumber.hpp"
#include "EppCommandCheckXriService.hpp"
#include "EppCommandCheckXriCodeString.hpp"
#endif
#include "EppCommandRenew.hpp"
#include "EppCommandRenewDomain.hpp"
#include "EppCommandRenewSvcsub.hpp"
#if defined(RTK_SUPPORT_XRI)
#include "EppCommandRenewXriName.hpp"
#include "EppCommandRenewXriNumber.hpp"
#include "EppCommandRenewXriService.hpp"
#endif
#include "EppCommandTransfer.hpp"
#include "EppCommandTransferContact.hpp"
#include "EppCommandTransferDomain.hpp"
#include "EppCommandTransferSvcsub.hpp"
#if defined(RTK_SUPPORT_XRI)
#include "EppCommandTransferXriAuthority.hpp"
#include "EppCommandTransferXriName.hpp"
#endif
#include "EppCommandUpdate.hpp"
#include "EppCommandUpdateContact.hpp"
#include "EppCommandUpdateDomain.hpp"
#include "EppCommandUpdateHost.hpp"
#include "EppCommandUpdateSvcsub.hpp"
#if defined(RTK_SUPPORT_XRI)
#include "EppCommandUpdateXriAuthority.hpp"
#include "EppCommandUpdateXriName.hpp"
#include "EppCommandUpdateXriNumber.hpp"
#include "EppCommandUpdateXriService.hpp"
#include "EppCommandUpdateXriCodeString.hpp"
#endif
#include "EppUtil.hpp"
#include "EppExtension.hpp"

EppCommand::~EppCommand()
{
	if( this->creds != null )
	{
		delete this->creds;
		this->creds = null;
	}

	if( this->extension != null )
	{
		for( unsigned int i = 0; i < this->extension->size(); i++ )
		{
			EppEntity * obj = this->extension->elementAt(i);
			if( obj != null )
			{
				delete obj;
			}
		}
		delete this->extension;
		this->extension = null;
	}

}

void EppCommand::setCreds( EppCreds& creds )
{
	if( this->creds != null )
	{
		delete this->creds;
	}
	this->creds = new EppCreds(creds.getClientId(),
		creds.getPassword(),
		creds.getNewPassword());
	EppCredsOptions * opt = creds.getOptions();
	if( opt != null )
	{
		this->creds->setOptions(*opt);
	}
}

DOMElement* EppCommand::toXMLCommon( DOMDocument& doc, const DOMString command, DOMElement& element )
{
	return toXMLCommon(doc, command, &element, (EppEntity *) null, command, (ValueVectorOf<DOMAttr*> *) null);
}

DOMElement* EppCommand::toXMLCommon( DOMDocument& doc, const DOMString command, DOMElement* element, ValueVectorOf<DOMAttr*> * attrList )
{
	return toXMLCommon(doc, command, element, null, command, attrList);
}

DOMElement* EppCommand::toXMLCommon( DOMDocument& doc, const DOMString command, EppEntity * object )
{
	return toXMLCommon(doc, command, (DOMElement*) null, object, command, (ValueVectorOf<DOMAttr*> *) null);
}

DOMElement* EppCommand::toXMLCommon( DOMDocument& doc, const DOMString command, DOMElement* element, EppEntity * object, const DOMString tag, ValueVectorOf<DOMAttr*> * attrList )
{
	DOMElement* elm = null;
	DOMElement* epp = EppUtil::createElementNS(doc, "epp", NULLXS);
	DOMElement* body = doc.createElement(XS("command"));
	epp->appendChild(body);

	//if( creds != null )
	//{
	//	body->appendChild(creds->toXML(doc, "creds"));
	//}

	if( this->getEntityType() == EppEntity::TYPE_EppCommandLogin )
	{
		elm = element;
	}
	else
	{
		elm = doc.createElement(command);
	}
	if( elm != null )
	{
	if( attrList != null )
	{
		for( unsigned int i = 0; i < attrList->size(); i++ )
		{
			DOMAttr* obj = attrList->elementAt(i);
			elm->setAttributeNode(obj);
		}
	}
	if( object != null )
	{
		elm->appendChild(object->toXML(doc, tag));
	}
		else if( element != null )
	{
		if( this->getEntityType() != EppEntity::TYPE_EppCommandLogin )
		{
				elm->appendChild(element);
		}
	}
	body->appendChild(elm);
	}

	if( extension != null )
	{
		elm = null;
		for( unsigned int i = 0; i < extension->size(); i++ )
		{
			EppEntity * obj = extension->elementAt(i);
			DOMElement* ext;
			if( obj == null )
			{
				continue;
			}
			if( obj->getEntityType() == EppEntity::TYPE_EppExtensionUnspec )
			{
				EppUnspec * unspec = (EppUnspec *) obj;
				ext = unspec->toXML(doc, "extension");
			}
			else if( obj->instanceOf(EppEntity::TYPE_EppExtension) == true )
			{
				EppExtension * extobj = (EppExtension *) obj;
				ext = extobj->toXML(doc, NULLXS);
			}
			else
			{
				ext = obj->toXML(doc, "extension");
			}
			if( ext != null )
			{
				if( elm == null )
				{
					elm = doc.createElement(XS("extension"));
					body->appendChild(elm);
				}
				elm->appendChild(ext);
			}
		}
	}

	if( clTRID.isNotNull() )
	{
		elm = doc.createElement(XS("clTRID"));
		elm->appendChild(doc.createTextNode(clTRID));
		body->appendChild(elm);
	}

	return epp;
}

EppCommand * EppCommand::fromXML( const DOMNode& root )
{
	unsigned int i;
	EppCommand * cmd = null;
	DOMNode* command;
	bool found = false;

	DOMNodeList* list = root.getChildNodes();
	for( i = 0; i < list->getLength(); i++ )
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
		if( name.equals("command") )
		{
			command = node;
			found = true;
			break;
		}
	}

	if( found == false )
	{
		return null;
	}

	list = command->getChildNodes();
	for( i = 0; i < list->getLength(); i++ )
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
		if( name.equals("login") )
		{
			cmd = EppCommandLogin::fromXML(*node);
		}
		else if( name.equals("logout") )
		{
			cmd = EppCommandLogout::fromXML(*node);
		}
		else if( name.equals("poll") )
		{
			cmd = EppCommandPoll::fromXML(*node);
		}
		else if( name.equals("create") )
		{
			cmd = EppCommandCreate::fromXML(*node);
		}
		else if( name.equals("delete") )
		{
			cmd = EppCommandDelete::fromXML(*node);
		}
		else if( name.equals("info") )
		{
			cmd = EppCommandInfo::fromXML(*node);
		}
		else if( name.equals("check") )
		{
			cmd = EppCommandCheck::fromXML(*node);
		}
		else if( name.equals("renew") )
		{
			cmd = EppCommandRenew::fromXML(*node);
		}
		else if( name.equals("transfer") )
		{
			cmd = EppCommandTransfer::fromXML(*node);
		}
		else if( name.equals("update") )
		{
			cmd = EppCommandUpdate::fromXML(*node);
		}
		/*
		 * other commands
		 */
		if( cmd != null )
		{
			break;
		}
	}
	if( cmd == null )
	{
		return null;
	}
	for( i = 0; i < list->getLength(); i++ )
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
		if( name.equals("creds") )
		{
			if( cmd->creds == null )
			{
				//cmd->creds = EppCreds::fromXML(node);
			}
		}
		else if( name.equals("extension") )
		{
			DOMNodeList* clist = node->getChildNodes();
			for( unsigned int j = 0; j < clist->getLength(); j++ )
			{
				DOMNode* cnode   = clist->item(j);
				DOMString prefix = cnode->getPrefix();
				DOMString cname  = cnode->getLocalName();
				DOMString nsURI  = cnode->getNamespaceURI();
				if( cname.isNull() )
				{
					cname = cnode->getNodeName();
				}
				if( cname.isNull() )
				{
					continue;
				}
				if(     cname.equals("neulevel:extension")
					|| (    (prefix.isNotNull()) && prefix.equals("neulevel")
					&& (cname.isNotNull()) && cname.equals("extension") )
					|| cname.equals("ex01:extension") )
				{
					EppUnspec * unspec = EppUnspec::fromXML(*cnode);
					if( unspec != null )
					{
						cmd->addExtension(unspec);
					}
				}
				else if( cname.isNotNull() )
				{
					EppExtension * ext = EppExtension::fromXML(*cnode);
					if( ext != null )
					{
						cmd->addExtension(ext);
					}
					// else other extensions
				}
				else
				{
					// other extensions
				}
			}
		}
		else if( name.equals("clTRID") )
		{
			cmd->clTRID = EppUtil::getText(*node);
		}
	}

	return cmd;
}

EppCommandCreate EppCommand::create( EppObject * object, DOMString xid )
{
	return EppCommandCreate(object, xid);
}

EppCommandCheck * EppCommand::check( int objectType, DOMString xid )
{
	EppCommandCheck * cmd = null;

	if( objectType == EppObject::CONTACT )
	{
		cmd = new EppCommandCheckContact(xid);
	}
	else if( objectType == EppObject::DOMAIN )
	{
		cmd = new EppCommandCheckDomain(xid);
	}
	else if( objectType == EppObject::HOST )
	{
		cmd = new EppCommandCheckHost(xid);
	}
	else if( objectType == EppObject::SVCSUB )
	{
		cmd = new EppCommandCheckSvcsub(xid);
	}
#if defined(RTK_SUPPORT_XRI)	
	else if( objectType == EppObject::XRI_AUTHORITY )
	{
		cmd = new EppCommandCheckXriAuthority(xid);
	}
	else if( objectType == EppObject::XRI_INAME )
	{
		cmd = new EppCommandCheckXriName(xid);
	}
	else if( objectType == EppObject::XRI_INUMBER )
	{
		cmd = new EppCommandCheckXriNumber(xid);
	}
	else if( objectType == EppObject::XRI_ISERVICE )
	{
		cmd = new EppCommandCheckXriService(xid);
	}
	else if( objectType == EppObject::XRI_CODESTRING )
	{
		cmd = new EppCommandCheckXriCodeString(xid);
	}
#endif
	return cmd;
}

EppCommandDelete * EppCommand::remove( int objectType, DOMString objectId, DOMString xid )
{
	EppCommandDelete * cmd = null;

	if( objectType == EppObject::CONTACT )
	{
		cmd = new EppCommandDeleteContact(objectId, xid);
	}
	else if( objectType == EppObject::DOMAIN )
	{
		cmd = new EppCommandDeleteDomain(objectId, xid);
	}
	else if( objectType == EppObject::HOST )
	{
		cmd = new EppCommandDeleteHost(objectId, xid);
	}
	else if( objectType == EppObject::SVCSUB )
	{
		cmd = new EppCommandDeleteSvcsub(objectId, xid);
	}
#if defined(RTK_SUPPORT_XRI)
	else if( objectType == EppObject::XRI_AUTHORITY )
	{
		cmd = new EppCommandDeleteXriAuthority(objectId, xid);
	}
	else if( objectType == EppObject::XRI_INAME )
	{
		cmd = new EppCommandDeleteXriName(objectId, xid);
	}
	else if( objectType == EppObject::XRI_INUMBER )
	{
		cmd = new EppCommandDeleteXriNumber(objectId, xid);
	}
	else if( objectType == EppObject::XRI_ISERVICE )
	{
		cmd = new EppCommandDeleteXriService(objectId, xid);
	}
#endif
	return cmd;
}

EppCommandInfo * EppCommand::info( int objectType, DOMString objectId, DOMString xid )
{
	EppCommandInfo * cmd = null;

	if( objectType == EppObject::CONTACT )
	{
		cmd = new EppCommandInfoContact(objectId, xid);
	}
	else if( objectType == EppObject::DOMAIN )
	{
		cmd = new EppCommandInfoDomain(objectId, xid);
	}
	else if( objectType == EppObject::HOST )
	{
		cmd = new EppCommandInfoHost(objectId, xid);
	}
	else if( objectType == EppObject::SVCSUB )
	{
		cmd = new EppCommandInfoSvcsub(objectId, xid);
	}
#if defined(RTK_SUPPORT_XRI)	
	else if( objectType == EppObject::XRI_AUTHORITY )
	{
		cmd = new EppCommandInfoXriAuthority(objectId, xid);
	}
	else if( objectType == EppObject::XRI_INAME )
	{
		cmd = new EppCommandInfoXriName(objectId, xid);
	}
	else if( objectType == EppObject::XRI_INUMBER )
	{
		cmd = new EppCommandInfoXriNumber(objectId, xid);
	}
	else if( objectType == EppObject::XRI_ISERVICE )
	{
		cmd = new EppCommandInfoXriService(objectId, xid);
	}
	else if( objectType == EppObject::XRI_CODESTRING )
	{
		cmd = new EppCommandInfoXriCodeString(objectId, xid);
	}
#endif
	return cmd;
}

EppCommandUpdate * EppCommand::update( int objectType, DOMString objectId, DOMString xid )
{
	EppCommandUpdate * cmd = null;

	if( objectType == EppObject::CONTACT )
	{
		cmd = new EppCommandUpdateContact(objectId, xid);
	}
	else if( objectType == EppObject::DOMAIN )
	{
		cmd = new EppCommandUpdateDomain(objectId, xid);
	}
	else if( objectType == EppObject::HOST )
	{
		cmd = new EppCommandUpdateHost(objectId, xid);
	}
	else if( objectType == EppObject::SVCSUB )
	{
		cmd = new EppCommandUpdateSvcsub(objectId, xid);
	}
#if defined(RTK_SUPPORT_XRI)	
	else if( objectType == EppObject::XRI_AUTHORITY )
	{
		cmd = new EppCommandUpdateXriAuthority(objectId, xid);
	}
	else if( objectType == EppObject::XRI_INAME )
	{
		cmd = new EppCommandUpdateXriName(objectId, xid);
	}
	else if( objectType == EppObject::XRI_INUMBER )
	{
		cmd = new EppCommandUpdateXriNumber(objectId, xid);
	}
	else if( objectType == EppObject::XRI_ISERVICE )
	{
		cmd = new EppCommandUpdateXriService(objectId, xid);
	}
	else if( objectType == EppObject::XRI_CODESTRING )
	{
		cmd = new EppCommandUpdateXriCodeString(objectId, xid);
	}
#endif
	return cmd;
}

EppCommandTransfer * EppCommand::transfer( int objectType, DOMString objectId, DOMString xid )
{
	EppCommandTransfer * cmd = null;

	if( objectType == EppObject::CONTACT )
	{
		cmd = new EppCommandTransferContact(objectId, xid);
	}
	else if( objectType == EppObject::DOMAIN )
	{
		cmd = new EppCommandTransferDomain(objectId, xid);
	}
	else if( objectType == EppObject::SVCSUB )
	{
		cmd = new EppCommandTransferSvcsub(objectId, xid);
	}
#if defined(RTK_SUPPORT_XRI)	
	else if( objectType == EppObject::XRI_AUTHORITY )
	{
		cmd = new EppCommandTransferXriAuthority(objectId, xid);
	}
	else if( objectType == EppObject::XRI_INAME )
	{
		cmd = new EppCommandTransferXriName(objectId, xid);
	}
#endif
	return cmd;
}

EppCommandRenew * EppCommand::renew( int objectType, DOMString objectId, DOMString xid )
{
	EppCommandRenew * cmd = null;

	if( objectType == EppObject::DOMAIN )
	{
		cmd = new EppCommandRenewDomain(objectId, xid);
	}
	else if( objectType == EppObject::SVCSUB )
	{
		cmd = new EppCommandRenewSvcsub(objectId, xid);
	}
#if defined(RTK_SUPPORT_XRI)	
	else if( objectType == EppObject::XRI_INAME )
	{
		cmd = new EppCommandRenewXriName(objectId, xid);
	}
	else if( objectType == EppObject::XRI_INUMBER )
	{
		cmd = new EppCommandRenewXriNumber(objectId, xid);
	}
	else if( objectType == EppObject::XRI_ISERVICE )
	{
		cmd = new EppCommandRenewXriService(objectId, xid);
	}
#endif
	return cmd;
}
