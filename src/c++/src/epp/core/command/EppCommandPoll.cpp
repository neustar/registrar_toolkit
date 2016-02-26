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
#include "EppCommandPoll.hpp"
#include "EppUtil.hpp"

const char * EppCommandPoll::OPTYPE_REQ = "req";
const char * EppCommandPoll::OPTYPE_ACK = "ack";

EppCommandPoll::EppCommandPoll()
{
	this->op = OPTYPE_REQ;
	this->msgID = null;
	this->clTRID = null;
}

EppCommandPoll::EppCommandPoll( DOMString xid )
{
	this->op = OPTYPE_REQ;
	this->msgID = null;
	this->clTRID = xid;
}

EppCommandPoll::EppCommandPoll( DOMString operation, DOMString messageId )
{
	this->op = operation;
	this->msgID = messageId;
	this->clTRID = null;
}

EppCommandPoll::EppCommandPoll( DOMString operation, DOMString messageId, DOMString xid )
{
	this->op = operation;
	this->msgID = messageId;
	this->clTRID = xid;
}

DOMElement* EppCommandPoll::toXML( DOMDocument& doc, const DOMString& tag )
{
	DOMAttr* attr;
	ValueVectorOf<DOMAttr*> attrList(3);

	if( op.isNotNull() )
	{
		attr = doc.createAttribute(XS("op"));
		attr->setValue(op);
		attrList.addElement(attr);
	}
	if( msgID.isNotNull() )
	{
		attr = doc.createAttribute(XS("msgID"));
		attr->setValue(msgID);
		attrList.addElement(attr);
	}

	return toXMLCommon(doc, tag, (DOMElement*) null, &attrList);
}

EppCommandPoll * EppCommandPoll::fromXML( const DOMNode& root )
{
	DOMElement * elm = (DOMElement *) &root;

	DOMString operation = elm->getAttribute(XS("op"));
	DOMString messageId = elm->getAttribute(XS("msgID"));

	//if( messageId.isNull() )
	//{
	//	messageId = null;
	//}

	return new EppCommandPoll(operation, messageId);
}
