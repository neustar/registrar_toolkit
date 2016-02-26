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
 * File:   CNDomain.hpp
 * Author: Santosh Kalsangrah
 *
 * Created on August 25, 2015, 5:57 PM
 */

#ifndef CNDOMAIN_HPP
#define	CNDOMAIN_HPP

#if ! defined(EPP_EXPORT)
#define       EPP_EXPORT
#endif

#include "EppEntity.hpp"

class EPP_EXPORT CNDomain
{
public:
	CNDomain();
	virtual ~CNDomain();

	const DOMString& getDomainType() const;
	void setDomainType(const DOMString& dType);
	
	const DOMString& getPurveyor() const;
	void setPurveyor(const DOMString& pur);
	
	static CNDomain * fromXML( const DOMNode& root );
	void toXML( DOMDocument &doc, DOMElement* inElement );
private:

	DOMString domainType;
	DOMString purveyor;
} ;

#endif	/* CNDOMAIN_HPP */

