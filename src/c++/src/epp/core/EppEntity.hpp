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

#if ! defined(EPPENTITY_HPP)    /* { */
#define       EPPENTITY_HPP        1

// new gcc prefers <iostream>
// #include <iostream.h>

#include <iostream>
using namespace std;

#include <xercesc/dom/DOM.hpp>
#include "XercesString.hpp"
#include "OldValueVectorOf.hpp"

using namespace xercesc;

#if ! defined(EPP_EXPORT)    /* { */
#define       EPP_EXPORT
#endif     /* EPP_EXPORT) */ /* } */

/**
 * The <code>EppEntity</code> class is the base class for all entities/objects
 * defined for EPP. All subclasses of <code>EppEntity</code> need to implement
 * the following three methods:
 * <UL>
 * <LI><code>fromXML</code></LI>
 * <LI><code>toXML</code></LI>
 * <LI><code>toString</code></LI>
 * </UL>
 *
 * @author Ning Zhang ning.zhang@neustar.com
 * @version $Revision: 1.14 $ $Date: 2013/07/08 21:43:25 $
 */
class EPP_EXPORT EppEntity
{
public:
	/**@name TYPE_EppEntity
	 * Defines EPP Entity type constants to be used for determining the
	 * EPP object type at run-time. Each EPP Entity type correspondes to
	 * the same class name in the foramt of <I>TYPE_EppClassName</I>.
	 * If an EPP Entity is not defined below, its default type is
	 * <I>TYPE_EppEntity</I>. If a constant does not corrspondes to an
	 * EPP Entity, it is used internally. These constants are used in the
	 * <code>getEntityType()</code> and <code>instanceOf()</code> methods.
	 *
	 * @see getEntityType
	 * @see instanceOf
	 *
	 * <pre>
	static const int TYPE_EppEntity                      
	static const int TYPE_EppObject                      
	static const int TYPE_EppContact                     
	static const int TYPE_EppDomain                      
	static const int TYPE_EppHost                        
	static const int TYPE_EppSvcsub                      
	static const int TYPE_EppXriAuthority                      
	static const int TYPE_EppXriNumber                      
	static const int TYPE_EppXriName                   
	static const int TYPE_EppXriService

	static const int TYPE_EppCommand                     
	static const int TYPE_EppCommandPoll                 
	static const int TYPE_EppCommandLogin                
	static const int TYPE_EppCommandLogout               
	static const int TYPE_EppCommandCreate               
	static const int TYPE_EppCommandCheck                
	static const int TYPE_EppCommandCheckContact         
	static const int TYPE_EppCommandCheckDomain          
	static const int TYPE_EppCommandCheckHost            
	static const int TYPE_EppCommandCheckSvcsub          
	static const int TYPE_EppCommandCheckXriAuthority        
	static const int TYPE_EppCommandCheckXriNumber        
	static const int TYPE_EppCommandCheckXriName 
	static const int TYPE_EppCommandDelete               
	static const int TYPE_EppCommandDeleteContact        
	static const int TYPE_EppCommandDeleteDomain         
	static const int TYPE_EppCommandDeleteHost           
	static const int TYPE_EppCommandDeleteSvcsub         
	static const int TYPE_EppCommandDeleteXriAuthority        
	static const int TYPE_EppCommandDeleteXriNumber        
	static const int TYPE_EppCommandDeleteXriName 
	static const int TYPE_EppCommandInfo                 
	static const int TYPE_EppCommandInfoContact          
	static const int TYPE_EppCommandInfoDomain           
	static const int TYPE_EppCommandInfoHost             
	static const int TYPE_EppCommandInfoSvcsub           
	static const int TYPE_EppCommandInfoXriAuthority        
	static const int TYPE_EppCommandInfoXriNumber        
	static const int TYPE_EppCommandInfoXriName 
	static const int TYPE_EppCommandInfo                 
	static const int TYPE_EppCommandInfoContact          
	static const int TYPE_EppCommandUpdate               
	static const int TYPE_EppCommandUpdateContact        
	static const int TYPE_EppCommandUpdateDomain         
	static const int TYPE_EppCommandUpdateHost           
	static const int TYPE_EppCommandUpdateSvcsub         
	static const int TYPE_EppCommandUpdateXriAuthority        
	static const int TYPE_EppCommandUpdateXriNumber        
	static const int TYPE_EppCommandUpdateXriName 
	static const int TYPE_EppCommandRenew                
	static const int TYPE_EppCommandRenewContact         
	static const int TYPE_EppCommandRenewDomain          
	static const int TYPE_EppCommandRenewHost            
	static const int TYPE_EppCommandRenewSvcsub          
	static const int TYPE_EppCommandRenewXriAuthority        
	static const int TYPE_EppCommandRenewXriNumber        
	static const int TYPE_EppCommandRenewXriName 
	static const int TYPE_EppCommandTransfer             
	static const int TYPE_EppCommandTransferContact      
	static const int TYPE_EppCommandTransferDomain       
	static const int TYPE_EppCommandTransferHost         
	static const int TYPE_EppCommandTransferSvcsub       
	static const int TYPE_EppCommandTransferXriAuthority        
	static const int TYPE_EppCommandTransferXriNumber        
	static const int TYPE_EppCommandTransferXriName 

	static const int TYPE_EppResponse                    

	static const int TYPE_EppResponseData                
	static const int TYPE_EppResponseDataPoll            
	static const int TYPE_EppResponseDataInfo            
	static const int TYPE_EppResponseDataCreate          
	static const int TYPE_EppResponseDataCreateContact   
	static const int TYPE_EppResponseDataCreateDomain    
	static const int TYPE_EppResponseDataCreateHost      
	static const int TYPE_EppResponseDataCreateSvcsub    
	static const int TYPE_EppResponseDataCreateXriAuthority    
	static const int TYPE_EppResponseDataCreateXriNumber    
	static const int TYPE_EppResponseDataCreateXriName    
	static const int TYPE_EppResponseDataCheck           
	static const int TYPE_EppResponseDataCheckContact    
	static const int TYPE_EppResponseDataCheckDomain     
	static const int TYPE_EppResponseDataCheckHost       
	static const int TYPE_EppResponseDataCheckSvcsub     
	static const int TYPE_EppResponseDataCheckXriAuthority    
	static const int TYPE_EppResponseDataCheckXriNumber    
	static const int TYPE_EppResponseDataCheckXriName    
	static const int TYPE_EppResponseDataTransfer        
	static const int TYPE_EppResponseDataTransferContact 
	static const int TYPE_EppResponseDataTransferDomain  
	static const int TYPE_EppResponseDataTransferHost    
	static const int TYPE_EppResponseDataTransferSvcsub  
	static const int TYPE_EppResponseDataTransferXriAuthority    
	static const int TYPE_EppResponseDataTransferXriNumber    
	static const int TYPE_EppResponseDataTransferXriName    
	static const int TYPE_EppResponseDataRenew           
	static const int TYPE_EppResponseDataRenewContact    
	static const int TYPE_EppResponseDataRenewDomain    
	static const int TYPE_EppResponseDataRenewHost      
	static const int TYPE_EppResponseDataRenewSvcsub    
	static const int TYPE_EppResponseDataRenewXriAuthority    
	static const int TYPE_EppResponseDataRenewXriNumber    
	static const int TYPE_EppResponseDataRenewXriName    
	static const int TYPE_EppResponseDataPending    
	static const int TYPE_EppResponseDataPendingContact 
	static const int TYPE_EppResponseDataPendingDomain  
	static const int TYPE_EppResponseDataPendingHost    
	static const int TYPE_EppResponseDataPendingSvcsub  
	static const int TYPE_EppResponseDataPendingXriAuthority    
	static const int TYPE_EppResponseDataPendingXriNumber    
	static const int TYPE_EppResponseDataPendingXriName    

	static const int TYPE_EppExtension                    
	static const int TYPE_EppExtensionUnspec              
	static const int TYPE_EppSecDnsDsData
	static const int TYPE_EppSecDnsKeyData
	static const int TYPE_EppSignedMarkData
	static const int TYPE_EppCommandCreateSecDns
	static const int TYPE_EppCommandUpdateSecDns
	static const int TYPE_EppResponseDataInfoSecDns
	static const int TYPE_EppCommandCreateLaunchRegistration
	static const int TYPE_EppResponseDataCreateLaunchRegistration
	static const int TYPE_EppIDN
	static const int TYPE_EppEncodedSignedMarkData
	static const int TYPE_EppResponseDataCheckClaims           
	    </pre>
	 */
	//@{
	static const int TYPE_EppEntity                      ;
	static const int TYPE_EppObject                      ;
	static const int TYPE_EppContact                     ;
	static const int TYPE_EppDomain                      ;
	static const int TYPE_EppHost                        ;
	static const int TYPE_EppSvcsub                      ;
	static const int TYPE_EppXriAuthority                ;
	static const int TYPE_EppXriNumber                   ;
	static const int TYPE_EppXriName                     ;
	static const int TYPE_EppXriService                  ;
	static const int TYPE_EppXriCodeString               ;

	static const int TYPE_EppCommand                     ;
	static const int TYPE_EppCommandPoll                 ;
	static const int TYPE_EppCommandLogin                ;
	static const int TYPE_EppCommandLogout               ;
	static const int TYPE_EppCommandCreate               ;
	static const int TYPE_EppCommandCheck                ;
	static const int TYPE_EppCommandCheckContact         ;
	static const int TYPE_EppCommandCheckDomain          ;
	static const int TYPE_EppCommandCheckHost            ;
	static const int TYPE_EppCommandCheckSvcsub          ;
	static const int TYPE_EppCommandCheckXriAuthority    ;
	static const int TYPE_EppCommandCheckXriNumber      ;
	static const int TYPE_EppCommandCheckXriName        ;
	static const int TYPE_EppCommandCheckXriService     ;
	static const int TYPE_EppCommandCheckXriCodeString  ;
	static const int TYPE_EppCommandDelete               ;
	static const int TYPE_EppCommandDeleteContact        ;
	static const int TYPE_EppCommandDeleteDomain         ;
	static const int TYPE_EppCommandDeleteHost           ;
	static const int TYPE_EppCommandDeleteSvcsub         ;
	static const int TYPE_EppCommandDeleteXriAuthority   ;
	static const int TYPE_EppCommandDeleteXriNumber     ;
	static const int TYPE_EppCommandDeleteXriName       ;
	static const int TYPE_EppCommandDeleteXriService    ;
	static const int TYPE_EppCommandDeleteXriCodeString ;
	static const int TYPE_EppCommandInfo                 ;
	static const int TYPE_EppCommandInfoContact          ;
	static const int TYPE_EppCommandInfoDomain           ;
	static const int TYPE_EppCommandInfoHost             ;
	static const int TYPE_EppCommandInfoSvcsub           ;
	static const int TYPE_EppCommandInfoXriAuthority     ;
	static const int TYPE_EppCommandInfoXriNumber       ;
	static const int TYPE_EppCommandInfoXriName         ;
	static const int TYPE_EppCommandInfoXriService      ;
	static const int TYPE_EppCommandInfoXriCodeString   ;
	static const int TYPE_EppCommandUpdate               ;
	static const int TYPE_EppCommandUpdateContact        ;
	static const int TYPE_EppCommandUpdateDomain         ;
	static const int TYPE_EppCommandUpdateHost           ;
	static const int TYPE_EppCommandUpdateSvcsub         ;
	static const int TYPE_EppCommandUpdateXriAuthority   ;
	static const int TYPE_EppCommandUpdateXriNumber     ;
	static const int TYPE_EppCommandUpdateXriName       ;
	static const int TYPE_EppCommandUpdateXriService    ;
	static const int TYPE_EppCommandUpdateXriCodeString ;
	static const int TYPE_EppCommandRenew                ;
	static const int TYPE_EppCommandRenewContact         ;
	static const int TYPE_EppCommandRenewDomain          ;
	static const int TYPE_EppCommandRenewHost            ;
	static const int TYPE_EppCommandRenewSvcsub          ;
	static const int TYPE_EppCommandRenewXriAuthority    ;
	static const int TYPE_EppCommandRenewXriNumber      ;
	static const int TYPE_EppCommandRenewXriName        ;
	static const int TYPE_EppCommandRenewXriService     ;
	static const int TYPE_EppCommandRenewXriCodeString  ;
	static const int TYPE_EppCommandTransfer             ;
	static const int TYPE_EppCommandTransferContact      ;
	static const int TYPE_EppCommandTransferDomain       ;
	static const int TYPE_EppCommandTransferHost         ;
	static const int TYPE_EppCommandTransferSvcsub       ;
	static const int TYPE_EppCommandTransferXriAuthority ;
	static const int TYPE_EppCommandTransferXriNumber   ;
	static const int TYPE_EppCommandTransferXriName     ;
	static const int TYPE_EppCommandTransferXriService  ;
	static const int TYPE_EppCommandTransferXriCodeString ;

	static const int TYPE_EppResponse                    ;

	static const int TYPE_EppResponseData                ;
	static const int TYPE_EppResponseDataPoll            ;
	static const int TYPE_EppResponseDataInfo            ;
	static const int TYPE_EppResponseDataCreate          ;
	static const int TYPE_EppResponseDataCreateContact   ;
	static const int TYPE_EppResponseDataCreateDomain    ;
	static const int TYPE_EppResponseDataCreateHost      ;
	static const int TYPE_EppResponseDataCreateSvcsub    ;
	static const int TYPE_EppResponseDataCreateXriAuthority;
	static const int TYPE_EppResponseDataCreateXriNumber  ;
	static const int TYPE_EppResponseDataCreateXriName    ;
	static const int TYPE_EppResponseDataCreateXriService ;
	static const int TYPE_EppResponseDataCreateXriCodeString;
	static const int TYPE_EppResponseDataCheck           ;
	static const int TYPE_EppResponseDataCheckContact    ;
	static const int TYPE_EppResponseDataCheckDomain     ;
	static const int TYPE_EppResponseDataCheckHost       ;
	static const int TYPE_EppResponseDataCheckSvcsub     ;
	static const int TYPE_EppResponseDataCheckXriAuthority;
	static const int TYPE_EppResponseDataCheckXriNumber  ;
	static const int TYPE_EppResponseDataCheckXriName    ;
	static const int TYPE_EppResponseDataCheckXriService ;
	static const int TYPE_EppResponseDataCheckXriCodeString;
	static const int TYPE_EppResponseDataTransfer        ;
	static const int TYPE_EppResponseDataTransferContact ;
	static const int TYPE_EppResponseDataTransferDomain  ;
	static const int TYPE_EppResponseDataTransferHost    ;
	static const int TYPE_EppResponseDataTransferSvcsub  ;
	static const int TYPE_EppResponseDataTransferXriAuthority;
	static const int TYPE_EppResponseDataTransferXriNumber  ;
	static const int TYPE_EppResponseDataTransferXriName    ;
	static const int TYPE_EppResponseDataTransferXriService ;
	static const int TYPE_EppResponseDataTransferXriCodeString;
	static const int TYPE_EppResponseDataRenew           ;
	static const int TYPE_EppResponseDataRenewContact    ;
	static const int TYPE_EppResponseDataRenewDomain     ;
	static const int TYPE_EppResponseDataRenewHost       ;
	static const int TYPE_EppResponseDataRenewSvcsub     ;
	static const int TYPE_EppResponseDataRenewXriAuthority;
	static const int TYPE_EppResponseDataRenewXriNumber  ;
	static const int TYPE_EppResponseDataRenewXriName    ;
	static const int TYPE_EppResponseDataRenewXriService ;
	static const int TYPE_EppResponseDataRenewXriCodeString;
	static const int TYPE_EppResponseDataPending         ;
	static const int TYPE_EppResponseDataPendingContact  ;
	static const int TYPE_EppResponseDataPendingDomain   ;
	static const int TYPE_EppResponseDataPendingHost     ;
	static const int TYPE_EppResponseDataPendingSvcsub   ;
	static const int TYPE_EppResponseDataPendingXriAuthority;
	static const int TYPE_EppResponseDataPendingXriNumber  ;
	static const int TYPE_EppResponseDataPendingXriName    ;
	static const int TYPE_EppResponseDataPendingXriService ;
	static const int TYPE_EppResponseDataPendingXriCodeString;

	static const int TYPE_EppExtension                   ;
	static const int TYPE_EppExtensionUnspec             ;
	static const int TYPE_EppSecDnsDsData                ;
	static const int TYPE_EppSecDnsKeyData               ;
	static const int TYPE_EppSignedMarkData              ;
	static const int TYPE_EppCommandCreateSecDns         ;
	static const int TYPE_EppCommandUpdateSecDns         ;
	static const int TYPE_EppResponseDataInfoSecDns      ;
	static const int TYPE_EppCommandCreateLaunchRegistration ;
	static const int TYPE_EppResponseDataCreateLaunchRegistration ;
	static const int TYPE_EppCommandUpdateLaunchRegistration;
	static const int TYPE_EppCommandDeleteLaunchRegistration;
	static const int TYPE_EppCommandInfoLaunchRegistration;
	static const int TYPE_EppMarkAddress;
	static const int TYPE_EppResponseDataInfoLaunchRegistration;
	static const int TYPE_EppCommandCheckLaunchRegistration ;
	static const int TYPE_EppIDN ;
	static const int TYPE_EppEncodedSignedMarkData		;
	static const int TYPE_EppResponseDataCheckClaims         ;
	static const int TYPE_EppAllocationToken;

	static const int TYPE_EppCommandInfoFee;
	static const int TYPE_EppCommandCheckFee;
	static const int TYPE_EppCommandTransferFee;
	static const int TYPE_EppCommandCreateFee;
	static const int TYPE_EppCommandRenewFee;
	static const int TYPE_EppCommandUpdateFee;

	static const int TYPE_EppResponseInfoFee;
	static const int TYPE_EppResponseCheckFee;
	static const int TYPE_EppResponseTransferFee;
	static const int TYPE_EppResponseCreateFee;
	static const int TYPE_EppResponseRenewFee;
	static const int TYPE_EppResponseUpdateFee;
	static const int TYPE_EppResponseDeleteFee;
        static const int TYPE_EppChangePollExt;

	static const int TYPE_CNDomainCreateExt;
	static const int TYPE_CNDomainInfoExt;
	static const int TYPE_CNDomainUpdateExt;
	static const int TYPE_CNContactCreateExt;
	static const int TYPE_CNContactInfoExt;
	static const int TYPE_CNContactUpdateExt;
	static const int TYPE_CNHostCreateExt;
	static const int TYPE_CNHostInfoExt;
	static const int TYPE_CNHostUpdateExt;
	static const int TYPE_CNCDNExt;

	//@}

	/**
	 * Constructor
	 */
	EppEntity() {};

	/**
	 * Destructor
	 */
	virtual ~EppEntity() = 0;

	/**
	 * Returns the run-time type of an EppEntity object
	 *
	 * @see TYPE_EppEntity
	 */
	virtual int getEntityType()
	{
		return EppEntity::TYPE_EppEntity;
	};

	/**
	 * Checks if an EppEntity object is an instance of an specific object
	 * type
	 *
	 * @see TYPE_EppEntity
	 */
	bool instanceOf( const int type );

	/**
	 * Converts an XML element into an <code>EppEntity</code> object.
	 * The caller of this method must make sure that the root node is
	 * of an EPP entity.
	 *
	 * @param root root node for an <code>EppEntity</code> object in
	 *             XML format
	 *
	 * @return a pointer to an <code>EppEntity</code> object, or null
	 *         if the node is invalid
	 *
	 * @note the caller should free the object
	 */
	static EppEntity * fromXML( const DOMNode &root );

	/**
	 * Converts the <code>EppEntity</code> object into an XML element
	 *
	 * @param doc the XML <code>DOMElement</code> object
	 * @param tag the tag/element name for the <code>EppEntity</code> object
	 *
	 * @return an <code>DOMElement</code> object
	 */
	virtual DOMElement* toXML( DOMDocument &doc, const DOMString &tag ) = 0;

	/**
	 * Converts the <code>EppEntity</code> object into plain XML text string
	 * by using the default root tag name
	 *
	 * @return a text string representing the <code>EppEntity</code> object
	 *         in XML format
	 */
	virtual DOMString toString() = 0;

protected:
	/**
	 * Converts the <code>EppEntity</code> object into plain XML text string
	 *
	 * @param tag XML tag name for the root node
	 *
	 * @return a text string representing the <code>EppEntity</code> object
	 *         in XML format
	 */
	DOMString toString( const DOMString &tag );
};


// This is already defined in XercesString.cpp/hpp
//inline ostream& operator<<(ostream& target, const DOMString& s)
//{
//	char * p = s.transcode();
//	target << p;
//	delete [] p;
//	return target;
//};

inline ostream& operator<<(ostream& target, EppEntity& s)
{
	target << s.toString();
	return target;
};

inline ostream& operator<<(ostream& target, EppEntity * s)
{
	if( s != null )
	{
		DOMString t = s->toString();
		char * p = t.transcode();
		target << p;
		XercesString::Delete(p);
	}
	else
	{
		target << "null";
	}
	return target;
};

#endif     /* EPPENTITY_HPP */  /* } */
