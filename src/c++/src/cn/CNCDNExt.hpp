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
 * File:   CNCDNExt.hpp
 * Author: Santosh Kalsangrah
 *
 * Created on September 24, 2015, 5:58 PM
 */

#ifndef CNCDNEXT_HPP
#define	CNCDNEXT_HPP

#if ! defined(EPP_EXPORT)
#define       EPP_EXPORT
#endif

#include "EppExtension.hpp"
#include "OldValueVectorOf.hpp"

class EPP_EXPORT CNCDNExt : public EppExtension
{
public:
	CNCDNExt();
	virtual ~CNCDNExt();

	const DOMString& getOCDNPunycode() const;
	void setOCDNPunycode(const DOMString& ocdnPunycode);
	const DOMString& getSCDN() const;
	void setSCDN(const DOMString& scdn);
	const DOMString& getSCDNPunycode() const;
	void setSCDNPunycode(const DOMString& scdnPunycode);
	const DOMString& getTCDN() const;
	void setTCDN(const DOMString& tcdn);
	const DOMString& getTCDNPunycode() const;
	void setTCDNPunycode(const DOMString& tcdnPunycode);
	OldValueVectorOf<DOMString>* getVCDN() const;
	void setVCDN( OldValueVectorOf<DOMString>*& vcdn);
	void addVCDN(const DOMString& );
	OldValueVectorOf<DOMString>* getVCDNPunycode() const;
	void setVCDNPunycode( OldValueVectorOf<DOMString>*& vcdnPunycode);
	void addVCDNPunnyCode(const DOMString& );

	static EppExtension * fromXML(const DOMNode& root);
	virtual DOMElement* toXML(DOMDocument &doc, const DOMString &tag);
	virtual DOMString toString();
	virtual int getEntityType();

private:
	/**
	 * Original Puny code Domain
	 */
	DOMString OCDNPunycode;

	/**
	 * Simplified Chinese Domain
	 */
	DOMString SCDN;

	/**
	 * Simplified Puny code Domain
	 */
	DOMString SCDNPunycode;

	/**
	 * Traditional Chinese Domain
	 */
	DOMString TCDN;

	/**
	 * Traditional Puny code Domain
	 */
	DOMString TCDNPunycode;

	OldValueVectorOf<DOMString> * VCDN;
	OldValueVectorOf<DOMString> * VCDNPunycode;
} ;

#endif	/* CNCDNEXT_HPP */

