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

#if ! defined(EPPCOMMAND_HPP)    /* { */
#define       EPPCOMMAND_HPP        1

#include "EppEntity.hpp"
#include "EppCreds.hpp"
#include "EppUnspec.hpp"

#include "EppObject.hpp"

#define	MAX_NUM_OF_EXTENSIONS	4

class EppCommandCreate;
class EppCommandCheck;
class EppCommandDelete;
class EppCommandInfo;
class EppCommandUpdate;
class EppCommandTransfer;
class EppCommandRenew;

/**
 * This <code>EppCommand</code> class implements EPP Command entity.
 *
 * @author Ning Zhang ning.zhang@neustar.com
 * @version $Revision: 1.2 $ $Date: 2006/03/01 01:35:37 $
 */
class EPP_EXPORT EppCommand : public EppEntity
{
protected:
	/**
	 * The EPP Credentials associated with the EPP Command (optional)
	 *  - for backward compatibility, not used any more in EPP-1.0
	 */
	EppCreds  * creds;	// optional
	/**
	 * Any extension data associated with the EPP Command (optional)
	 */
	OldValueVectorOf<EppEntity *> * extension;	// optional
	/**
	 * The client transaction id associated with the EPP Command
	 */
	DOMString   clTRID;

	/**
	 * Converts the shared <code>EppCommand</code> components into an XML
	 * element. The command must be object specific commands such as
	 * create/delete/query, etc.
	 *
	 * @param doc the XML <code>DOMElement</code> object
	 * @param command the tag/element name for the <code>EppCommand</code>
	 *                object
	 * @param element the <code>DOMElement</code> associated with the command
	 *
	 * @return an <code>DOMElement</code> object
	 */
	DOMElement* toXMLCommon( DOMDocument& doc, const DOMString command, DOMElement& element );

	/**
	 * Converts the shared <code>EppCommand</code> components into an XML
	 * element. The command must be object specific commands such as
	 * create/delete/query, etc.
	 *
	 * @param doc the XML <code>DOMElement</code> object
	 * @param command  the tag/element name for the <code>EppCommand</code>
	 *                 object
	 * @param element  the <code>DOMElement</code> associated with the command
	 * @param attrList the <code>Attr</code> list associated with the
	 *		   command
	 *
	 * @return an <code>DOMElement</code> object
	 */
	DOMElement* toXMLCommon( DOMDocument& doc, const DOMString command, DOMElement* element, ValueVectorOf<DOMAttr*> * attrList );

	/**
	 * Converts the shared <code>EppCommand</code> components into an XML
	 * element. The command must be object specific commands such as
	 * create/delete/query, etc.
	 *
	 * @param doc the XML <code>DOMElement</code> object
	 * @param command the tag/element name for the <code>EppCommand</code>
	 *                object
	 * @param object the <code>EppObject</code> associated with the command.
	 *		 the type of the object should be either
	 *		 <code>EppEntity</code> or <code>DOMElement</code>
	 *
	 * @return an <code>DOMElement</code> object
	 */
	DOMElement* toXMLCommon( DOMDocument& doc, const DOMString command, EppEntity * object );

	/**
	 * Converts the shared <code>EppCommand</code> components into an XML
	 * element
	 *
	 * @param doc the XML <code>DOMElement</code> object
	 * @param command the tag/element name for the <code>EppCommand</code>
	 *                object
	 * @param element the <code>DOMElement</code> associated with the command.
	 * @param object  the <code>EppEntity</code> associated with the command.
	 * @param tag      the tag indicating if the command is generic, such
	 *                 as login/logout
	 * @param attrList the <code>Attr</code> list associated with the
	 *		   command
	 *
	 * @return an <code>DOMElement</code> object
	 *
	 * @note the element and object parameters should be mutually exclusive,
	 *       i.e. if object is not null, element will not be used, or if
	 *       object is null, the element should be used
	 */
	DOMElement* toXMLCommon( DOMDocument& doc, const DOMString command, DOMElement* element, EppEntity * object, const DOMString tag, ValueVectorOf<DOMAttr*> * attrList );

public:

	/**
	 * Creates <code>EppCommand</code> object
	 */
	EppCommand()
	{
		this->creds = null;
		this->clTRID = NULLXS;
		this->extension = null;
	};

	/**
	 * Destructor
	 */
	virtual ~EppCommand();

	/**
	 * Returns the run-time type of an EppEntity object
	 */
	virtual int getEntityType()
	{
		return EppEntity::TYPE_EppCommand;
	};

	/**
	 * Gets command credentials  - for backward compatibility, not used any more in EPP-1.0
	 */
	virtual EppCreds * getCreds()
	{
		return this->creds;
	};

	/**
	 * Sets command credentials - for backward compatibility, not used any more in EPP-1.0
	 */
	virtual void setCreds( EppCreds& creds );

	/**
	 * Gets optional unspecified object
	 */
	EppUnspec * getUnspec()
	{
		if( this->extension == null )
		{
			return null;
		}
		for( unsigned int i = 0; i < this->extension->size(); i++ )
		{
			EppEntity * obj = this->extension->elementAt(i);
			if( (obj != null) && (obj->getEntityType() == EppEntity::TYPE_EppExtensionUnspec) )
			{
				return (EppUnspec *) obj;
			}
		}
		return null;
	};

	/**
	 * Sets optional unspecified object
	 */
	void setUnspec( EppUnspec& unspec )
	{
		if( this->extension == null )
		{
			this->extension = new OldValueVectorOf<EppEntity *>(MAX_NUM_OF_EXTENSIONS);
		}
		if( this->extension == null )
		{
			return;
		}
		EppUnspec * p = new EppUnspec();
		if( p == null )
		{
			return;
		}
		*p = unspec;
		this->extension->addElement(p);
	};

	/**
	 * Gets transaction id from the registrar client
	 */
	DOMString getClientTransactionId()
	{
		return this->clTRID;
	};

	/**
	 * Sets transaction id from the registrar client
	 */
	void setClientTransactionId( DOMString xid )
	{
		this->clTRID = xid;
	};

	/**
	 * Gets the list of extension objects, either EppUnspec, or DOM Element
	 */
	OldValueVectorOf<EppEntity *> * getExtension()
	{
		return this->extension;
	}

	/**
	 * Adds an optional extension object
	 */
	void addExtension( EppEntity * ext )
	{
		if( ext == null )
		{
			return;
		}
		if( this->extension == null )
		{
			this->extension = new OldValueVectorOf<EppEntity *>(MAX_NUM_OF_EXTENSIONS);
		}
		if( this->extension == null )
		{
			return;
		}
		this->extension->addElement(ext);
	};

	/**
	 * Converts an XML element into an <code>EppCommand</code> object.
	 * The caller of this method must make sure that the root node is of
	 * an EPP Command entity.
	 *
	 * @param root root node for an <code>EppCommand</code> object in XML
	 *             format
	 *
	 * @return an <code>EppEntity</code> object, or null if the node
	 *         is invalid
	 */
	static EppCommand * fromXML( const DOMNode& root );

	/**
	 * Creates an <code>EppCommandCreate</code> object. This is a
	 * convience method for creating an EPP Object in the registry.
	 *
	 * @param object object to be created in the registry
	 * @param xid    client transaction id associated with the operation
	 *
	 * @return an <code>EppCommandCreate</code> object, which could
	 *         be one of the subclass of <code>EppCommandCreate</code>
	 */
	static EppCommandCreate create( EppObject * object, DOMString xid );

	/**
	 * Creates an <code>EppCommandCheck</code> object based on the
	 * object type.
	 *
	 * @param objectType the type of objects to be checked. The value
	 *                   must be defined in <code>EppObject</code>, as
	 *                   one of the followings:
	 *                   <UL><LI>EppObject::CONTACT</LI>
	 *                       <LI>EppObject::DOMAIN</LI>
	 *                       <LI>EppObject::HOST</LI>
	 *                       <LI>EppObject::SVCSUB</LI>
	 *                       <LI>EppObject::XRI_AUTHORITY</LI>
	 *                       <LI>EppObject::XRI_INAME</LI>
	 *                       <LI>EppObject::XRI_INUMBER</LI>
	 *                       <LI>EppObject::XRI_ISERVICE</LI>
	 *                   </UL>
	 * @param xid client transaction id associated with the operation
	 *
	 * @return an <code>EppCommandCheck</code> object, which could
	 *         be one of the subclass of <code>EppCommandCheck</code>;
	 *         or null if the object type is invalid.
	 *
	 * @note the memory associated with the command should be freed by the
	 *       caller
	 */
	static EppCommandCheck * check( int objectType, DOMString xid );

	/**
	 * Creates an <code>EppCommandDelete</code> object based on the
	 * object type.
	 *
	 * @param objectType the type of object to be deleted. The value
	 *                   must be defined in <code>EppObject</code>, as
	 *                   one of the followings:
	 *                   <UL><LI>EppObject::CONTACT</LI>
	 *                       <LI>EppObject::DOMAIN</LI>
	 *                       <LI>EppObject::HOST</LI>
	 *                       <LI>EppObject::SVCSUB</LI>
	 *                       <LI>EppObject::XRI_AUTHORITY</LI>
	 *                       <LI>EppObject::XRI_INAME</LI>
	 *                       <LI>EppObject::XRI_INUMBER</LI>
	 *                       <LI>EppObject::XRI_ISERVICE</LI>
	 *                   </UL>
	 * @param objectId the id associated with object to be deleted.
	 *                 The value is id for an EPP Contact object,
	 *                 domain name for an EPP Domain object, or host
	 *                 name for an EPP Host object.
	 * @param xid client transaction id associated with the operation
	 *
	 * @return an <code>EppCommandDelete</code> object, which could
	 *         be one of the subclass of <code>EppCommandDelete</code>;
	 *         or null if the object type is invalid.
	 *
	 * @note the memory associated with the command should be freed by the
	 *       caller
	 *
	 * @note this method is named as <code>delete</code> in its Java API,
	 *       but we could not use <code>delete</code> as it is a C++ keyword.
	 */
	static EppCommandDelete * remove( int objectType, DOMString objectId, DOMString xid );

	/**
	 * Creates an <code>EppCommandInfo</code> object based on the
	 * object type.
	 *
	 * @param objectType the type of object to be queried. The value
	 *                   must be defined in <code>EppObject</code>, as
	 *                   one of the followings:
	 *                   <UL><LI>EppObject::CONTACT</LI>
	 *                       <LI>EppObject::DOMAIN</LI>
	 *                       <LI>EppObject::HOST</LI>
	 *                       <LI>EppObject::SVCSUB</LI>
	 *                       <LI>EppObject::XRI_AUTHORITY</LI>
	 *                       <LI>EppObject::XRI_INAME</LI>
	 *                       <LI>EppObject::XRI_INUMBER</LI>
	 *                       <LI>EppObject::XRI_ISERVICE</LI>
	 *                   </UL>
	 * @param objectId the id associated with object to be queried.
	 *                 The value is id for an EPP Contact object,
	 *                 domain name for an EPP Domain object, or host
	 *                 name for an EPP Host object.
	 * @param xid client transaction id associated with the operation
	 *
	 * @return an <code>EppCommandInfo</code> object, which could
	 *         be one of the subclass of <code>EppCommandInfo</code>;
	 *         or null if the object type is invalid.
	 *
	 * @note the memory associated with the command should be freed by the
	 *       caller
	 */
	static EppCommandInfo * info( int objectType, DOMString objectId, DOMString xid );

	/**
	 * Creates an <code>EppCommandUpdate</code> object based on the
	 * object type.
	 *
	 * @param objectType the type of object to be updated. The value
	 *                   must be defined in <code>EppObject</code>, as
	 *                   one of the followings:
	 *                   <UL><LI>EppObject::CONTACT</LI>
	 *                       <LI>EppObject::DOMAIN</LI>
	 *                       <LI>EppObject::HOST</LI>
	 *                       <LI>EppObject::SVCSUB</LI>
	 *                       <LI>EppObject::XRI_AUTHORITY</LI>
	 *                       <LI>EppObject::XRI_INAME</LI>
	 *                       <LI>EppObject::XRI_INUMBER</LI>
	 *                       <LI>EppObject::XRI_ISERVICE</LI>
	 *                   </UL>
	 * @param objectId the id associated with object to be updated.
	 *                 The value is id for an EPP Contact object,
	 *                 domain name for an EPP Domain object, or host
	 *                 name for an EPP Host object.
	 * @param xid client transaction id associated with the operation
	 *
	 * @return an <code>EppCommandUpdate</code> object, which could
	 *         be one of the subclass of <code>EppCommandUpdate</code>;
	 *         or null if the object type is invalid.
	 *
	 * @note the memory associated with the command should be freed by the
	 *       caller
	 */
	static EppCommandUpdate * update( int objectType, DOMString objectId, DOMString xid );

	/**
	 * Creates an <code>EppCommandTransfer</code> object based on the
	 * object type.
	 *
	 * @param objectType the type of object to be transferred. The value
	 *                   must be defined in <code>EppObject</code>, as
	 *                   one of the followings:
	 *                   <UL><LI>EppObject::CONTACT</LI>
	 *                       <LI>EppObject::DOMAIN</LI>
	 *                       <LI>EppObject::SVCSUB</LI>
	 *                       <LI>EppObject::XRI_AUTHORITY</LI>
	 *                       <LI>EppObject::XRI_INAME</LI>
	 *                   </UL>
	 * @param objectId the id associated with object to be transferred.
	 *                 The value is id for an EPP Contact object,
	 *                 domain name for an EPP Domain object.
	 * @param xid client transaction id associated with the operation
	 *
	 * @return an <code>EppCommandTransfer</code> object, which could
	 *         be one of the subclass of <code>EppCommandTransfer</code>;
	 *         or null if the object type is invalid.
	 *
	 * @note the memory associated with the command should be freed by the
	 *       caller
	 */
	static EppCommandTransfer * transfer( int objectType, DOMString objectId, DOMString xid );

	/**
	 * Creates an <code>EppCommandRenew</code> object based on the
	 * object type.
	 *
	 * @param objectType the type of object to be renewed. The value
	 *                   must be defined in <code>EppObject</code>, as
	 *                   one of the followings:
	 *                   <UL><LI>EppObject::DOMAIN</LI>
	 *                       <LI>EppObject::SVCSUB</LI>
	 *                       <LI>EppObject::XRI_INAME</LI>
	 *                       <LI>EppObject::XRI_INUMBER</LI>
	 *                       <LI>EppObject::XRI_ISERVICE</LI>
	 *                   </UL>
	 * @param objectId the id associated with object to be renewed.
	 *                 The value is the domain name for an EPP Domain
	 *                 object.
	 * @param xid client transaction id associated with the operation
	 *
	 * @return an <code>EppCommandRenew</code> object, which could
	 *         be one of the subclass of <code>EppCommandRenew</code>;
	 *         or null if the object type is invalid.
	 *
	 * @note   As the current expiration date of the object is default
	 *         to the current date, and the default value of the new
	 *         registration period is specified by the registry, the
	 *         the caller to this method may have to use
	 *         <code>setCurrentExpiratin</code> and <code>setPeriod</code>
	 *         to change these default values.
	 */
	static EppCommandRenew * renew( int objectType, DOMString objectId, DOMString xid );
};

#endif     /* EPPCOMMAND_HPP */  /* } */
