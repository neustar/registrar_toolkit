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
#include "EppChannelTcp.hpp"
#include "EppParser.hpp"
#include "EppHello.hpp"
#include "EppCommandLogout.hpp"
#include "EppMessageUtil.hpp"
#include "EppSessionTcp.hpp"
//#include "xercesc/dom/DomMemDebug.hpp"

EppResponse * EppChannelTcp::send( EppCommand& command )
{
	int socket = -1;
	SSL * ssl = null;
	bool usetls = this->useTLS();

	if( usetls )
	{
		ssl = this->getSSL();
		if( ssl == null )
		{
			return null;
		}
	}
	else
	{
		socket = this->getSocket();
		if( socket < 0 )
		{
			return null;
		}
	}

	// first make sure that the command is valid, if not, we
	// will not send the command out at all

	this->setException(null);
	this->setMessage(null);

	DOMString cmd = command.toString();
	EppParser parser(cmd);
	if( parser.hasError() )
	{
		EppResponse * rsp = new EppResponse();
		EppResult * result = parser.getResult();
		rsp->addResult(result);
		EppTransactionId xid(command.getClientTransactionId(),
				     command.getClientTransactionId());
		rsp->setTransactionId(xid);
		return rsp;
	}

	// ok, we are sure that the command syntax is OK, send the
	// the command string in XML format

	char * msg = null;
	int    len = 0;
	if( usetls )
	{
		int timeout = 0;
		EppSessionTcp *tSession = dynamic_cast<EppSessionTcp*> (this->getSession());
		if( tSession != NULL )
		{
			timeout = tSession->getSocketTimeout();
		}
		msg = EppMessageUtil::send(ssl, cmd, &len, timeout);
	}
	else
	{
		msg = EppMessageUtil::send(socket, cmd, &len);
	}
	if( len == 0 )
	{
		this->setException(msg);
		msg = null;
	}
	this->setMessage(msg);
	if( msg == null )
	{
		return null;
	}
	parser.parse(msg, len);
	delete [] msg;

	if( parser.hasError() )
	{
		// there is syntax error with the response

		EppResponse * rsp = new EppResponse();
		EppResult * result = parser.getResult();
		rsp->addResult(result);
		EppTransactionId xid(command.getClientTransactionId(),
				     command.getClientTransactionId());
		rsp->setTransactionId(xid);
		return rsp;
	}

	return EppResponse::fromXML(*parser.getRootNode());
}

EppResponse * EppChannelTcp::start( EppCommandLogin& login )
{
	EppResponse * rsp = this->send(login);
	if( (rsp != null) && rsp->success() )
	{
		if( this->status == STATUS_INIT )
		{
			this->status = STATUS_START;
		}
	}
	return rsp;
}

EppGreeting * EppChannelTcp::hello()
{
	int socket = -1;
	SSL * ssl = null;
	bool usetls = this->useTLS();

	if( usetls )
	{
		ssl = this->getSSL();
		if( ssl == null )
		{
			return null;
		}
	}
	else
	{
		socket = this->getSocket();
		if( socket < 0 )
		{
			return null;
		}
	}

	this->setException(null);
	this->setMessage(null);

	EppHello hello;

	char * str = null;
	int    len = 0;
	if( usetls )
	{
		int timeout = 0;
		EppSessionTcp *tSession = dynamic_cast<EppSessionTcp*> (this->getSession());
		if( tSession != NULL )
		{
			timeout = tSession->getSocketTimeout();
		}		
		str = EppMessageUtil::send(ssl, hello.toString(), &len, timeout);
	}
	else
	{
		str = EppMessageUtil::send(socket, hello.toString(), &len);
	}
	if( len == null )
	{
		this->setException(str);
		str = null;
	}
	this->setMessage(str);
	if( str == null )
	{
		return null;
	}
	EppParser parser(str, len);
	delete [] str;

	if( parser.hasError() )
	{
		return null;
	}
	return EppGreeting::fromXML(*parser.getRootNode());
}

EppResponse * EppChannelTcp::terminate()
{
	if( this->status != STATUS_START )
	{
		return null;
	}
	EppCommandLogout logout;
	EppResponse * rsp = this->send(logout);
	if( (rsp != null) && rsp->success() )
	{
		this->status = STATUS_CLOSED;
	}
	return rsp;
}
