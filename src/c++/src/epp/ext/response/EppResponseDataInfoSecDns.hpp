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

#if ! defined(EPPRESPONSEDATASECDNS_HPP)    /* { */
#define       EPPRESPONSEDATASECDNS_HPP        1

#include "EppExtension.hpp"
#include "EppSecDnsDsData.hpp"
#include <xercesc/util/ValueVectorOf.hpp>

#define MAX_NUM_OF_DS_DATA	4

/**
 * This <code>EppCommandSecDns</code> class implements EPP SecDNS infData entity.
 *
 * @author Ning Zhang ning.zhang@neustar.com
 * @version $Revision: 1.4 $ $Date: 2012/07/12 16:30:49 $
 */
class EPP_EXPORT EppResponseDataInfoSecDns : public EppExtension
{
private:
	bool keyDataPresent;
	int maxSigLife;
	OldValueVectorOf<EppSecDnsDsData> *dsDataList;
	OldValueVectorOf<EppSecDnsKeyData> *keyDataList;

public:

	/**
	 * Creates an <code>EppCommandSecDns</code> object
	 */
	EppResponseDataInfoSecDns(uint8_t ver = EPPSECDNS_11):keyDataPresent(false)
	{
		maxSigLife = -1;
		this->dsDataList = new OldValueVectorOf<EppSecDnsDsData>(MAX_NUM_OF_DS_DATA);
		this->keyDataList = new OldValueVectorOf<EppSecDnsKeyData>(MAX_NUM_OF_DS_DATA);
		secDnsVersion = ver;
	};

	/**
	 * Destructor
	 */
	~EppResponseDataInfoSecDns()
	{
		if( this->dsDataList != null )
		{
			delete this->dsDataList;
			this->dsDataList = null;
		}
		if( this->keyDataList != null )
		{
			delete this->keyDataList;
			this->keyDataList = null;
		}
	};

	/**
	 * Returns the run-time type of an EppEntity object
	 */
	virtual int getEntityType()
	{
		return EppEntity::TYPE_EppResponseDataInfoSecDns;
	};

	/**
	 * Adds DS Data to the list to be attached to a domain name
	 */
	void add( EppSecDnsDsData& dsData )
	{
		this->dsDataList->addElement(dsData);
	};

	void add( EppSecDnsKeyData& kdData )
	{
		keyDataPresent = true;
		this->keyDataList->addElement(kdData);
	}

	/**
	 * Gets the list of DS data to be attached to a domain name
	 */
	OldValueVectorOf<EppSecDnsDsData> * getDsData()
	{
		return this->dsDataList;
	};

	OldValueVectorOf<EppSecDnsKeyData> * getKeyData()
	{
		return this->keyDataList;
	};

	int getMaxSigLife()
	{
		return this->maxSigLife;
	}

	void setMaxSigLife( int life )
	{
		this->maxSigLife = life;
	}

	bool isKeyDataPresent()
	{
		return this->keyDataPresent;
	}


	/**
	 * Converts an XML element into an <code>EppResponseDataInfoSecDns</code> object.
	 * The caller of this method must make sure that the root node is of
	 * EPP SECDNS createType
	 *
	 * @param root root node for an <code>EppResponseDataInfoSecDns</code> object in XML format
	 *
	 * @return an <code>EppResponseDataInfoSecDns</code> object, or null if the node is invalid
	 */
	static EppResponseDataInfoSecDns * fromXML( const DOMNode& root );

	/**
	 * Converts the <code>EppResponseDataInfoSecDns</code> object into an XML element
	 *
	 * @param doc the XML <code>Document</code> object
	 * @param tag the tag/element name for the <code>EppResponseDataInfoSecDns</code> object
	 *
	 * @return an <code>DOMElement</code> object
	 */
	DOMElement* toXML( DOMDocument& doc, const DOMString& tag );

	DOMString toString()
	{
		return EppEntity::toString(DOMString("infData"));
	};
};

#endif     /* EPPRESPONSEDATASECDNS_HPP */  /* } */
