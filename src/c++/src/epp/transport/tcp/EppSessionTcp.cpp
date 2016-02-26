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

#if       defined(win32)        /* { */

#include <windows.h>
#include <winsock.h>
#include <io.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#ifndef	   EINPROGRESS		/* { */
#define	   EINPROGRESS		WSAEINPROGRESS
#endif	/* EINPROGRESS	*/ 	/* } */
#ifndef	   EALREADY		/* { */
#define	   EALREADY		WSAEALREADY
#endif	/* EALREADY	*/ 	/* } */
#ifndef	   EISCONN		/* { */
#define	   EISCONN		WSAEISCONN
#endif	/* EISCONN	*/ 	/* } */

#define	CLOSESOCKET(_s_)	::closesocket(_s_)

#else  /* defined(win32) */     /* } { */

#define	CLOSESOCKET(_s_)	::close(_s_)

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#endif /* defined(win32) */     /* } */

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>

#include <xercesc/dom/DOM.hpp>

#include "EppHello.hpp"
#include "EppGreeting.hpp"
#include "EppParser.hpp"
#include "EppSessionTcp.hpp"
#include "EppChannelTcp.hpp"
#include "EppMessageUtil.hpp"
#include "EppUtil.hpp"

EppSessionTcp::EppSessionTcp()
{
	this->host = null;
	this->port = 0;
	this->channel = null;
	this->useTls = true;
	this->sock = -1;
	this->sslctx = null;
	this->sslssl = null;
	this->sotimeout = 0;

	this->privateKeyFileName = null;
	this->privateKeyFileType = SSL_FILETYPE_PEM;
	this->publicKeyFileName = null;
	this->publicKeyFileType = SSL_FILETYPE_PEM;
	this->remoteKeyFileName = null;
};

EppSessionTcp::EppSessionTcp( bool useTLS )
{
	this->host = null;
	this->port = 0;
	this->channel = null;
	this->useTls = useTLS;
	this->sock = -1;
	this->sslctx = null;
	this->sslssl = null;
	this->sotimeout = 0;

	this->privateKeyFileName = null;
	this->privateKeyFileType = SSL_FILETYPE_PEM;
	this->publicKeyFileName = null;
	this->publicKeyFileType = SSL_FILETYPE_PEM;
	this->remoteKeyFileName = null;
}

EppSessionTcp::~EppSessionTcp()
{
	this->cleanup();
}

void EppSessionTcp::cleanup()
{
	if( this->host != null )
	{
		delete [] this->host;
		this->host = null;
	}
	this->port = 0;
	if( this->privateKeyFileName != null )
	{
		delete [] this->privateKeyFileName;
		this->privateKeyFileName = null;
	}
	this->privateKeyFileType = SSL_FILETYPE_PEM;
	if( this->publicKeyFileName != null )
	{
		delete [] this->publicKeyFileName;
		this->publicKeyFileName = null;
	}
	this->publicKeyFileType = SSL_FILETYPE_PEM;
	if( this->remoteKeyFileName != null )
	{
		delete [] this->remoteKeyFileName;
		this->remoteKeyFileName = null;
	}
	if( this->channel != null )
	{
		this->channel->terminate();
		delete this->channel;
		this->channel = null;
	}
	if( this->useTls )
	{
		if( this->sslssl != null )
		{
			SSL_shutdown(this->sslssl);
			SSL_free(this->sslssl);
			this->sslssl = null;
		}
		if( this->sslctx != null )
		{
			SSL_CTX_free(this->sslctx);
			this->sslctx = null;
		}
	}
	resetSocket(true);
}

void EppSessionTcp::initTLS()
{
	static bool tlsInit = false;
	if( tlsInit == false )
	{
		SSL_load_error_strings();
		SSL_library_init();
		OpenSSL_add_ssl_algorithms();

		tlsInit = true;
	}
	resetSocket(true);
	if( this->sslssl != null )
	{
		SSL_shutdown(this->sslssl);
		SSL_free(this->sslssl);
		this->sslssl = null;
	}
	if( this->sslctx != null )
	{
		SSL_CTX_free(this->sslctx);
		this->sslctx = null;
	}
	initSocket();
	if( this->sock < 0 )
	{
		// error
		return;
	}
	this->sslctx = SSL_CTX_new(TLSv1_client_method());
	//this->sslctx = SSL_CTX_new(SSLv23_client_method());
	if( this->sslctx == null )
	{
		this->setException("Cannot initialize OpenSSL: SSL_CTX_new()");
		return;
	}
	if( this->publicKeyFileName != null )
	{
		//if( SSL_CTX_use_certificate_file(this->sslctx, this->publicKeyFileName, this->publicKeyFileType) < 0 )
		if( SSL_CTX_use_certificate_chain_file(this->sslctx, this->publicKeyFileName) <= 0 )
		{
			this->setException("Cannot load public key file");
			return;
		}
	}
	if( this->privateKeyFileName != null )
	{
		if( SSL_CTX_use_PrivateKey_file(this->sslctx, this->privateKeyFileName, this->privateKeyFileType) < 0 )
		{
			this->setException("Cannot load private key file");
			return;
		}
	}
	if( this->remoteKeyFileName != null )
	{
		if( SSL_CTX_load_verify_locations(this->sslctx, this->remoteKeyFileName, null) < 0 )
		{
			this->setException("Cannot load remote public key file");
			return;
		}
	}
	if( SSL_CTX_set_default_verify_paths(this->sslctx) < 0 )
	{
		this->setException("SSL_CTX_set_default_verify_paths failed");
		return;
	}
	SSL_CTX_set_verify(this->sslctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, null);
	this->sslssl = SSL_new(this->sslctx);
	if( this->sslssl == null )
	{
		this->setException("Cannot initialize OpenSSL: SSL_new()");
		return;
	}

	// set socket to non-blocking
	int ofcmode = fcntl(this->sock, F_GETFL, 0);
	if ( fcntl(this->sock, F_SETFL, ofcmode | O_NONBLOCK) )
	{
		::shutdown(this->sock, 2);
		CLOSESOCKET(this->sock);
		this->sock = -1;
		this->setException("Cannot set socket to non-blocking");
		return;
	}

	SSL_set_fd(this->sslssl, this->sock);
	time_t tStarted = time(0);
	while ( true )
	{
		int n = SSL_connect(this->sslssl);
		if ( n > 0 )
			break; // success

		if( EppMessageUtil::shouldSSLRetry(SSL_get_error(this->sslssl, n), tStarted, this->getSocketTimeout()) )
		{
			continue;
		}

		// error
		SSL_shutdown(this->sslssl);
		SSL_free(this->sslssl);
		this->sslssl = null;
		this->setException("Cannot initialize OpenSSL: SSL_connect()");
		return;
	}
}

void EppSessionTcp::initSocket()
{
	struct addrinfo hints;
	struct addrinfo *servInfoList = NULL;
	struct addrinfo *addr = NULL;

	resetSocket(true);

	::memset(&hints, 0, sizeof hints);
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	char portStr[32];
	::sprintf(portStr, "%d", this->port);

	if( getaddrinfo(this->host, portStr, &hints, &servInfoList) != 0 )
	{
		this->setException("Cannot find host: getaddrinfo()");
	}
	else
	{
		// loop through all the results and connect to the one we can	
		for( addr = servInfoList; NULL != addr; addr = addr->ai_next )
		{
			this->sock = ::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

			if( this->sock < 0 )
			{
				this->setException("Cannot get socket: socket()");
				continue; // Try on next address if any
			}

			setSocketOption();

			// Try connecting now
			if( connectToAddress(addr) == true )
			{
				// Connection established
				this->setException(NULL);
				break;
			}

			resetSocket(false);
			this->setException("Cannot connect to the server: connect()");
		}
	}
	if( NULL != servInfoList )
	{
		freeaddrinfo(servInfoList);
	}
}

void EppSessionTcp::setSocketOption()
{
	if( this->sotimeout > 0 )
	{
		(void) ::setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO,
			(char *) &this->sotimeout, sizeof(this->sotimeout));
		(void) ::setsockopt(this->sock, SOL_SOCKET, SO_SNDTIMEO,
			(char *) &this->sotimeout, sizeof(this->sotimeout));
	}
}

void EppSessionTcp::resetSocket( bool bShutdown )
{
	if( this->sock >= 0 )
	{
		if( bShutdown )
		{
			::shutdown(this->sock, 2);
		}
		::close(this->sock);
		this->sock = -1;
	}
}

bool EppSessionTcp::connectToAddress( struct addrinfo* addr )
{
	// connect to the server
	int status = ::connect(this->sock, addr->ai_addr, addr->ai_addrlen);
	if( (status < 0) && (errno != EINPROGRESS) )
	{
		return false;
	}
	else if( status >= 0 )
	{
		// already connected, done
		return true;
	}

	/*
	 * try to connect until we have a timeout, or the connection is
	 * successful, or the user hits the stop button
	 */
	fd_set			  fds;
	struct timeval		  timeout;
	int secs = 0;

	while( (secs + secs) < MAX_NUM_OF_DELAY_SECONDS )
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET(this->sock, &fds);
		int rv = ::select(this->sock + 1, (fd_set *) NULL, &fds,
						  (fd_set *) NULL, &timeout);
		secs++;

		// for a non-blocking socket, if the previous connection
		// has not been completed, EALREADY will be set in errno;
		// otherwise, we have a real error

		if( (rv < 0) && (errno != EALREADY) )
		{
			return false;
		}
		else if( rv > 0 )
		{
			// the connection is OK, add an extra check here for
			// connection success, if we try to connect again,
			// and get EISCONN, it means that we have a good
			// connection

			status = ::connect(this->sock, addr->ai_addr, addr->ai_addrlen);
			if( (status < 0) && (errno == EISCONN) )
			{
				return true;
			}
			// else, we got a real error
			else
			{
				return false;
			}
		}
		// else, the select() says we are not ready yet. try to
		// connect again and make make sure. if we don't get
		// EALREADY or EISCONN, something is wrong
		status = ::connect(this->sock, addr->ai_addr, addr->ai_addrlen);
		if(    (status < 0)
		    && (errno != EALREADY)
		    && (errno != EISCONN) )
		{
			return false;
		}
	}

	return false;
}

void EppSessionTcp::init( void * object )
{
	// nothing to do for now
	if( object != null )
	{
		char ** argv = (char **) object;
		setPrivateKeyFile(argv[0], SSL_FILETYPE_PEM);
		setPublicKeyFile(argv[1], SSL_FILETYPE_PEM);
		setRemoteKeyFile(argv[2]);
	}
}

void EppSessionTcp::setPrivateKeyFile( char * name, int type )
{
	this->privateKeyFileType = type;
	if( this->privateKeyFileName != null )
	{
		delete this->privateKeyFileName;
		this->privateKeyFileName = null;
	}
	if( name != null )
	{
		this->privateKeyFileName = new char[strlen(name) + 1];
		if( this->privateKeyFileName != null )
		{
			(void) strcpy(this->privateKeyFileName, name);
		}
	}
}

void EppSessionTcp::setPublicKeyFile( char * name, int type )
{
	this->publicKeyFileType = type;
	if( this->publicKeyFileName != null )
	{
		delete this->publicKeyFileName;
		this->publicKeyFileName = null;
	}
	if( name != null )
	{
		this->publicKeyFileName = new char[strlen(name) + 1];
		if( this->publicKeyFileName != null )
		{
			(void) strcpy(this->publicKeyFileName, name);
		}
	}
}

void EppSessionTcp::setRemoteKeyFile( char * name )
{
	if( this->remoteKeyFileName != null )
	{
		delete this->remoteKeyFileName;
	}
	if( name != null )
	{
		this->remoteKeyFileName = new char[strlen(name) + 1];
		if( this->remoteKeyFileName != null )
		{
			(void) strcpy(this->remoteKeyFileName, name);
		}
	}
}

EppGreeting * EppSessionTcp::connect( const char * host, const int port )
{
	if( this->host != null )
	{
		delete [] this->host;
	}
	this->host = new char[strlen(host) + 1];
	(void) strcpy(this->host, host);
	this->port = port;
	this->setException(null);
	this->setMessage(null);

	char * str = null;
	int    len = 0;

	if( ! this->useTls )
	{
		initSocket();
		if( this->sock < 0 )
		{
			return null;
		}
		str = EppMessageUtil::getEppPayload(this->sock, &len);
		if( len == 0 )
		{
			this->setException(str);
			str = null;
		}
	}
	else
	{
		initTLS();
		if( this->sslssl == null )
		{
			return null;
		}
		str = EppMessageUtil::getEppPayload(this->sslssl, &len, this->getSocketTimeout());
		if( len == 0 )
		{
			this->setException(str);
			str = null;
		}
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

void EppSessionTcp::close()
{
	this->cleanup();
}

EppChannel * EppSessionTcp::getChannel()
{
	if( this->channel == null )
	{
		this->channel = new EppChannelTcp(this);
	}
	return this->channel;
}

EppGreeting * EppSessionTcp::hello()
{
	char * str = null;
	int    len = 0;
	EppHello hello;

	if( this->useTls )
	{
		if( this->sslssl == null )
		{
			return null;
		}
		str = EppMessageUtil::send(this->sslssl, hello.toString(), &len, this->sotimeout);
	}
	else
	{
		if( this->sock < 0 )
		{
			return null;
		}
		str = EppMessageUtil::send(this->sock, hello.toString(), &len);
	}
	this->setMessage(str);
	if( str == null )
	{
		this->setException("Error in EppSessionTcp::hello()");
		return null;
	}
	EppParser parser(str, len);
	if( parser.hasError() )
	{
		return null;
	}
	return EppGreeting::fromXML(*parser.getRootNode());
}
