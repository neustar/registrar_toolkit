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

#if ! defined(EppResponseDataInfoLaunchRegistration_HPP)    /* { */
#define       EppResponseDataInfoLaunchRegistration_HPP 1
#include "EppExtension.hpp"
#include "EppMarkMark.hpp"
#include "EppStatus.hpp"
#include "EppLaunchPhase.hpp"

class EPP_EXPORT EppResponseDataInfoLaunchRegistration : public EppExtension
{
public:
	EppResponseDataInfoLaunchRegistration();
	EppResponseDataInfoLaunchRegistration(const EppResponseDataInfoLaunchRegistration&);
	EppResponseDataInfoLaunchRegistration& operator=(const EppResponseDataInfoLaunchRegistration&);
	virtual ~EppResponseDataInfoLaunchRegistration();
	void phase(const EppLaunchPhase&);
	EppLaunchPhase phase();
	void applicationId(const DOMString &);
	DOMString applicationId(void);
	void status(const DOMString& _s);
	DOMString status(void);
	void mark(EppMarkMark* _mark);
	EppMarkMark* mark(void);
	static EppResponseDataInfoLaunchRegistration* fromXML( const DOMNode& root );
	virtual DOMElement* toXML( DOMDocument& doc, const DOMString& tag );
	virtual DOMString toString();
	virtual int getEntityType();
private:
	EppLaunchPhase  _phase;
	DOMString _appId;
	EppStatus _status;
	EppMarkMark *_mark;
	bool isStatusSet;
};

#endif
