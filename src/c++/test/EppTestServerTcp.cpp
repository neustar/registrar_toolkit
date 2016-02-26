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

#define WIN32_NO_STATUS
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

#define	sleep(_x)	Sleep(1000 * (_x))
#define	CLOSESOCKET(_s_)	::closesocket(_s_)

#else  /* defined(win32) */      /* } { */

#define	CLOSESOCKET(_s_)	::close(_s_)

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>

#endif /* defined(win32) */      /* } */

#if       defined(win32) || defined(hpux)       /* { */
#define   socklen_t int
#endif /* defined(win32) || defined(hpux) */    /* } */

#include <xercesc/parsers/XercesDOMParser.hpp>

#include "openssl/rand.h"

#include "EppSessionTcp.hpp"
#include "EppChannelTcp.hpp"
#include "EppAddress.hpp"
#include "EppContactData.hpp"
#include "EppContactType.hpp"
#include "EppCommandCreate.hpp"
#include "EppCommandPoll.hpp"
#include "EppResponse.hpp"
#include "EppContact.hpp"
#include "EppDomain.hpp"
#include "EppHost.hpp"
#include "EppSvcsub.hpp"
#include "EppMessageUtil.hpp"
#include "EppResponseDataCreate.hpp"
#include "EppResponseDataCreateContact.hpp"
#include "EppResponseDataCreateDomain.hpp"
#include "EppResponseDataCreateHost.hpp"
#include "EppResponseDataCreateSvcsub.hpp"
#include "EppResponseDataCheck.hpp"
#include "EppResponseDataCheckContact.hpp"
#include "EppResponseDataCheckDomain.hpp"
#include "EppResponseDataCheckHost.hpp"
#include "EppResponseDataCheckSvcsub.hpp"
#include "EppResponseDataTransfer.hpp"
#include "EppResponseDataTransferContact.hpp"
#include "EppResponseDataTransferDomain.hpp"
#include "EppResponseDataTransferSvcsub.hpp"

#ifdef XRI_TEST
#include "EppXriAuthority.hpp"
#include "EppXriName.hpp"
#include "EppXriNumber.hpp"
#include "EppXriService.hpp"
#include "EppResponseDataCreateXriAuthority.hpp"
#include "EppResponseDataCreateXriName.hpp"
#include "EppResponseDataCreateXriNumber.hpp"
#include "EppResponseDataCreateXriService.hpp"
#include "EppResponseDataCheckXriAuthority.hpp"
#include "EppResponseDataCheckXriName.hpp"
#include "EppResponseDataCheckXriNumber.hpp"
#include "EppResponseDataCheckXriService.hpp"
#include "EppResponseDataTransferXriAuthority.hpp"
#include "EppResponseDataTransferXriName.hpp"
#include "EppCommandRenewXriName.hpp"
#include "EppCommandRenewXriNumber.hpp"
#include "EppCommandRenewXriService.hpp"
#include "EppCommandTransferXriAuthority.hpp"
#include "EppCommandTransferXriName.hpp"
#endif

#include "EppHello.hpp"
#include "EppGreeting.hpp"
#include "EppParser.hpp"

#define	MAX_NUM_OF_THREADS	256

bool instanceof( EppEntity * entity, int type )
{
	return entity->instanceOf(type);
}

static int verify_callback( int ok, X509_STORE_CTX *ctx )
{
	char *s,buf[256];

	s = X509_NAME_oneline(X509_get_subject_name(ctx->current_cert), buf, 256);
	if( s != null )
	{
		if( ok != 0 )
		{
			fprintf(stderr, "depth=%d %s\n",ctx->error_depth, buf);
		}
		else
		{
			fprintf(stderr, "depth=%d error=%d %s\n",
				ctx->error_depth, ctx->error, buf);
		}
	}

	if( ok == 0 )
	{
		switch (ctx->error)
		{
		case X509_V_ERR_CERT_NOT_YET_VALID:
		case X509_V_ERR_CERT_HAS_EXPIRED:
		case X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT:
			ok = 1;
			break;
		default:
			break;
		}
	}

	return(ok);
}

class EppTestServerTcpThread
{
private:
	int   sock;
	SSL * ssl;
	bool  hasLogin;
	bool  quit;

	int write( EppEntity * s );
	int write( char * buf, int len );

	void setTransactionId( EppResponse * rsp );
	void setTransactionId( EppResponse * rsp, EppCommand * cmd );
	void setResponseDataCreate( EppCommand * cmd, EppResponse * rsp );
	void setResponseDataCheck( EppCommand * cmd, EppResponse * rsp );
	void setResponseDataRenew( EppCommand * cmd, EppResponse * rsp );
	void setResponseDataTransfer( EppCommand * cmd, EppResponse * rsp );
	EppEntity * processLogin( DOMNode& root );
	EppEntity * process( char * msg, int len );

public:
	EppTestServerTcpThread();
	EppTestServerTcpThread( int sock, SSL * ssl );
	~EppTestServerTcpThread();
	void run();
#if       defined(win32)        /* { */
	unsigned long thread_id;
	HANDLE thread_handle;
#else  /* defined(win32) */     /* } { */
	pthread_t thread_id;
#endif /* defined(win32) */     /* } */
};

EppTestServerTcpThread::EppTestServerTcpThread()
{
	this->sock = -1;
	this->ssl  = null;
	this->hasLogin = false;
	this->quit = false;
	this->thread_id = 0;
#if       defined(win32)        /* { */
	this->thread_handle = null;
#endif /* defined(win32) */     /* } */
}

EppTestServerTcpThread::EppTestServerTcpThread( int sock, SSL * ssl )
{
	this->sock = sock;
	this->ssl  = ssl;
	this->hasLogin = false;
	this->quit = false;
	this->thread_id = 0;
#if       defined(win32)        /* { */
	this->thread_handle = null;
#endif /* defined(win32) */     /* } */
}

EppTestServerTcpThread::~EppTestServerTcpThread()
{
	if( this->sock >= 0 )
	{
		(void) ::shutdown(this->sock, 2);
		(void) CLOSESOCKET(this->sock);
		this->sock = -1;
	}
	if( this->ssl != null )
	{
		SSL_shutdown(this->ssl);
		SSL_free(this->ssl);
		this->ssl = null;
	}
	if( this->thread_id >= 0 )
	{
	}
}

void EppTestServerTcpThread::setTransactionId( EppResponse * rsp )
{
	EppTransactionId xid("NO-CLIENT-ID", "SERVER-ID-9999");
	rsp->setTransactionId(xid);
}

void EppTestServerTcpThread::setTransactionId( EppResponse * rsp, EppCommand * cmd )
{
	EppTransactionId xid(cmd->getClientTransactionId(), "SERVER-ID-9999");
	rsp->setTransactionId(xid);
}

void EppTestServerTcpThread::setResponseDataCreate( EppCommand * cmd, EppResponse * rsp )
{
	EppObject * obj = ((EppCommandCreate *) cmd)->getObject();
	EppResponseDataCreate * data = null;
	if( instanceof(obj, EppEntity::TYPE_EppContact) )
	{
		DOMString id = ((EppContact *) obj)->getId();
		data = new EppResponseDataCreateContact(id);
	}
	else if( instanceof(obj, EppEntity::TYPE_EppDomain) )
	{
		DOMString name = ((EppDomain *) obj)->getName();
		data = new EppResponseDataCreateDomain(name, time(0));
	}
	else if( instanceof(obj, EppEntity::TYPE_EppHost) )
	{
		DOMString name = ((EppHost *) obj)->getName();
		data = new EppResponseDataCreateHost(name);
	}
	else if( instanceof(obj, EppEntity::TYPE_EppSvcsub) )
	{
		DOMString id = ((EppSvcsub *) obj)->getId();
		data = new EppResponseDataCreateSvcsub(id, time(0));
	}

#ifdef XRI_TEST
	else if( instanceof(obj, EppEntity::TYPE_EppXriAuthority) )
	{
		DOMString id = ((EppXriAuthority *) obj)->getAuthorityId();
		data = new EppResponseDataCreateXriAuthority(id, time(0));
	}
	else if( instanceof(obj, EppEntity::TYPE_EppXriName) )
	{
		DOMString name = ((EppXriName *) obj)->getIName();
		data = new EppResponseDataCreateXriName(name, time(0));
	}
	else if( instanceof(obj, EppEntity::TYPE_EppXriNumber) )
	{
		DOMString num = ((EppXriNumber *) obj)->getINumber();
		data = new EppResponseDataCreateXriNumber(num, time(0));
	}
	else if( instanceof(obj, EppEntity::TYPE_EppXriService) )
	{
		DOMString id = ((EppXriService *) obj)->getId();
		data = new EppResponseDataCreateXriService(id, time(0));
	}
#endif

	if( data != null )
	{
		rsp->setResponseData(data);
	}
}

void EppTestServerTcpThread::setResponseDataCheck( EppCommand * cmd, EppResponse * rsp )
{
	EppResponseDataCheck * data = null;
	if( instanceof(cmd, EppEntity::TYPE_EppCommandCheckContact) )
	{
		data = new EppResponseDataCheckContact();
	}
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandCheckDomain) )
	{
		data = new EppResponseDataCheckDomain();
	}
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandCheckHost) )
	{
		data = new EppResponseDataCheckHost();
	}
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandCheckSvcsub) )
	{
		data = new EppResponseDataCheckSvcsub();
	}

#ifdef XRI_TEST
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandCheckXriAuthority) )
	{
		data = new EppResponseDataCheckXriAuthority();
	}
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandCheckXriName) )
	{
		data = new EppResponseDataCheckXriName();
	}
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandCheckXriNumber) )
	{
		data = new EppResponseDataCheckXriNumber();
	}
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandCheckXriService) )
	{
		data = new EppResponseDataCheckXriService();
	}
#endif

	OldValueVectorOf<DOMString> * list = ((EppCommandCheck *) cmd)->get();
	for( unsigned int i = 0; i < list->size(); i++ )
	{
		DOMString id = list->elementAt(i);
		data->add(id,  ((i % 2) == 0) ?
			DOMString(EppResponseDataCheck::FOUND) :
			DOMString(EppResponseDataCheck::NOT_FOUND));
	}

	rsp->setResponseData(data);
}

void EppTestServerTcpThread::setResponseDataRenew( EppCommand * cmd, EppResponse * rsp )
{
	EppResponseData * data = null;
	if( instanceof(cmd, EppEntity::TYPE_EppCommandRenewDomain) )
	{
		DOMString name = ((EppCommandRenewDomain *) cmd)->getName();
		data = new EppResponseDataCreateDomain(name, time(0));
	}
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandRenewSvcsub) )
	{
		DOMString id = ((EppCommandRenewSvcsub *) cmd)->getId();
		data = new EppResponseDataCreateSvcsub(id, time(0));
	}

#ifdef XRI_TEST
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandRenewXriName) )
	{
		DOMString name = ((EppCommandRenewXriName *) cmd)->getIName();
		data = new EppResponseDataCreateXriName(name, time(0));
	}
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandRenewXriNumber) )
	{
		DOMString num = ((EppCommandRenewXriNumber *) cmd)->getINumber();
		data = new EppResponseDataCreateXriNumber(num, time(0));
	}
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandRenewXriService) )
	{
		DOMString id = ((EppCommandRenewXriService *) cmd)->getId();
		data = new EppResponseDataCreateXriService(id, time(0));
	}
#endif

	rsp->setResponseData(data);
}

void EppTestServerTcpThread::setResponseDataTransfer( EppCommand * cmd, EppResponse * rsp )
{
	EppResponseDataTransfer * data = null;
	if( instanceof(cmd, EppEntity::TYPE_EppCommandTransferContact) )
	{
		DOMString id = ((EppCommandTransferContact *) cmd)->getId();
		data = new EppResponseDataTransferContact(id);
	}
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandTransferDomain) )
	{
		DOMString name = ((EppCommandTransferDomain *) cmd)->getName();
		data = new EppResponseDataTransferDomain(name);
		((EppResponseDataTransferDomain *) data)->setDateExpired(time(0));
	}
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandTransferSvcsub) )
	{
		DOMString id = ((EppCommandTransferSvcsub *) cmd)->getId();
		data = new EppResponseDataTransferSvcsub(id);
		((EppResponseDataTransferSvcsub *) data)->setDateExpired(time(0));
	}

#ifdef XRI_TEST
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandTransferXriAuthority) )
	{
		DOMString id = ((EppCommandTransferXriAuthority *) cmd)->getAuthorityId();
		DOMString target = ((EppCommandTransferXriAuthority *) cmd)->getTargetAuthorityId();
		data = new EppResponseDataTransferXriAuthority(id);
		((EppResponseDataTransferXriAuthority *)data)->setSourceAuthorityId(id);
		((EppResponseDataTransferXriAuthority *)data)->setTargetAuthorityId(target);
	}
	else if( instanceof(cmd, EppEntity::TYPE_EppCommandTransferXriName) )
	{
		DOMString name = ((EppCommandTransferXriName *) cmd)->getIName();
		data = new EppResponseDataTransferXriName(name);
		((EppResponseDataTransferXriName *) data)->setDateExpired(time(0));
	}
#endif

	if( data != null )
	{
		data->setTransferStatus(EppResponseDataTransfer::STATUS_PENDING);
		data->setRequestClientId("CLIENT-ID-REQ");
		data->setActionClientId("CLIENT-ID-ACT");
		data->setRequestDate(time(0));
		data->setActionDate(time(0));
	}

	rsp->setResponseData(data);
}

EppEntity * EppTestServerTcpThread::processLogin( DOMNode& root )
{
	// check if it is a hello

	EppHello * hello = EppHello::fromXML(root);
	if( hello != null )
	{
		delete hello;
		return new EppGreeting();
	}

	// check if it is login command

	EppCommand * cmd = EppCommand::fromXML(root);
	if( cmd == null )
	{
		EppResponse * res = new EppResponse();
		EppResult p(EppError::CODE_UNKNOWN_COMMAND);
		res->addResult(p);
		this->setTransactionId(res);
		return res;
	}

	if( instanceof(cmd, EppEntity::TYPE_EppCommandLogin) )
	{
		EppResponse * res = new EppResponse();
		EppResult p(EppError::CODE_NO_ERROR);
		res->addResult(p);
		this->setTransactionId(res, cmd);
		this->hasLogin = true;
		delete cmd;
		return res;
	}

	// it is an invalid command sequence

	EppResponse * res = new EppResponse();
	EppResult p(EppError::CODE_COMMAND_USE_ERROR);
	res->addResult(p);
	this->setTransactionId(res, cmd);
	delete cmd;
	return res;
}

EppEntity * EppTestServerTcpThread::process( char * msg, int len )
{
	// parse the message first

	EppParser parser(msg, len);
	if( parser.hasError() )
	{
		EppResponse * rsp = new EppResponse();
		EppResult * p = parser.getResult();
		rsp->addResult(p);
		this->setTransactionId(rsp);
		return rsp;
	}

	DOMNode* root = parser.getRootNode();
	if( root == null )
	{
		EppResponse * res = new EppResponse();
		EppResult p(EppError::CODE_UNKNOWN_COMMAND);
		res->addResult(p);
		this->setTransactionId(res);
		return res;
	}

	if( this->hasLogin == false )
	{
		return this->processLogin(*root);
	}

	EppCommand * cmd = EppCommand::fromXML(*root);
	if( cmd == null )
	{
		EppHello * hello = EppHello::fromXML(*root);
		if( hello != null )
		{
			delete hello;
			return new EppGreeting();
		}

		EppResponse * rsp = new EppResponse();
		EppResult p(EppError::CODE_UNKNOWN_COMMAND);
		rsp->addResult(p);
		this->setTransactionId(rsp);
		return rsp;
	}

	if( instanceof(cmd, EppEntity::TYPE_EppCommandLogin) )
	{
		EppResponse * rsp = new EppResponse();
		EppResult p(EppError::CODE_COMMAND_USE_ERROR);
		rsp->addResult(p);
		this->setTransactionId(rsp, cmd);
		delete cmd;
		return rsp;
	}

	if( instanceof(cmd, EppEntity::TYPE_EppCommandLogout) )
	{
		EppResponse * rsp = new EppResponse();
		EppResult p(EppError::CODE_NO_ERROR_ENDING_SESSION);
		rsp->addResult(p);
		this->setTransactionId(rsp, cmd);
		this->hasLogin = false;
		this->quit = true;
		delete cmd;
		return rsp;
	}

	if(    instanceof(cmd, EppEntity::TYPE_EppCommandDelete)
	    || instanceof(cmd, EppEntity::TYPE_EppCommandUpdate) )
	{
		EppResponse * rsp = new EppResponse();
		EppResult p (EppError::CODE_NO_ERROR);
		rsp->addResult(p);
		this->setTransactionId(rsp, cmd);
		delete cmd;
		return rsp;
	}

	if( instanceof(cmd, EppEntity::TYPE_EppCommandCreate) )
	{
		EppResponse * rsp = new EppResponse();
		EppResult p (EppError::CODE_NO_ERROR);
		rsp->addResult(p);
		this->setTransactionId(rsp, cmd);
		this->setResponseDataCreate(cmd, rsp);
		delete cmd;
		return rsp;
	}

	if( instanceof(cmd, EppEntity::TYPE_EppCommandCheck) )
	{
		EppResponse * rsp = new EppResponse();
		EppResult p (EppError::CODE_NO_ERROR);
		rsp->addResult(p);
		this->setTransactionId(rsp, cmd);
		this->setResponseDataCheck(cmd, rsp);
		delete cmd;
		return rsp;
	}

	if( instanceof(cmd, EppEntity::TYPE_EppCommandRenew) )
	{
		EppResponse * rsp = new EppResponse();
		EppResult p (EppError::CODE_NO_ERROR);
		rsp->addResult(p);
		this->setTransactionId(rsp, cmd);
		this->setResponseDataRenew(cmd, rsp);
		delete cmd;
		return rsp;
	}

	if( instanceof(cmd, EppEntity::TYPE_EppCommandTransfer) )
	{
		EppResponse * rsp = new EppResponse();
		EppResult p (EppError::CODE_NO_ERROR);
		rsp->addResult(p);
		this->setTransactionId(rsp, cmd);
		this->setResponseDataTransfer(cmd, rsp);
		delete cmd;
		return rsp;
	}

	if( instanceof(cmd, EppEntity::TYPE_EppCommandInfo) )
	{
		EppResponse * rsp = new EppResponse();
		EppResult p(EppError::CODE_OBJECT_DOES_NOT_EXIST);
		rsp->addResult(p);
		this->setTransactionId(rsp, cmd);
		delete cmd;
		return rsp;
	}

	if( instanceof(cmd, EppEntity::TYPE_EppCommandPoll) )
	{
		EppResponse * rsp = new EppResponse();
		EppResult p(EppError::CODE_NO_ERROR_NO_MESSAGE);
		rsp->addResult(p);
		this->setTransactionId(rsp, cmd);
		delete cmd;
		return rsp;
	}

	EppResponse * rsp = new EppResponse();
	EppResult p(EppError::CODE_UNKNOWN_COMMAND);
	rsp->addResult(p);
	this->setTransactionId(rsp, cmd);
	delete cmd;
	return rsp;
}

void EppTestServerTcpThread::run()
{
	EppGreeting greeting;
	char * msg = null;
	int len = 0;

	(void) fprintf(stderr, "thread %lX started\n", (long) this->thread_id);
	(void) fflush(stderr);

	if( write(&greeting) < 0 )
	{
		(void) fprintf(stderr, "Cannot send greeting\n");
		(void) fflush(stderr);
		return;
	}

	while( ! this->quit )
	{
		// wait for a message
	
		msg = null;
		len = 0;

		if( this->ssl != null )
		{
			msg = EppMessageUtil::getEppPayload(this->ssl, &len, 60);
		}
		else if( this->sock >= 0 )
		{
			msg = EppMessageUtil::getEppPayload(this->sock, &len);
		}
		if( len == 0 )
		{
			(void) fprintf(stderr, "Error is %s\n", msg);
			(void) fflush(stderr);
			msg = null;
		}
		if( msg == null )
		{
			(void) fprintf(stderr, "Cannot get EPP message\n");
			(void) fflush(stderr);
			break;
		}
		(void) fprintf(stderr, "EPP Message:\n%s", msg);
		(void) fflush(stderr);
		EppEntity * res = this->process(msg, len);
		if( res == null )
		{
			continue;
		}
		(void) fprintf(stderr, "EPP Response:\n");
		cerr << res << endl;
		(void) fflush(stderr);

		if( this->write(res) < 0 )
		{
			(void) fprintf(stderr, "Write error\n");
			(void) fflush(stderr);
		}
		EppResponseData * data = null;

		if( instanceof(res, EppEntity::TYPE_EppResponse) )
		{
 			data = ((EppResponse *) res)->getResponseData();
		}
		delete res;
		if( data != null )
		{
			delete data;
		}
		if( msg != null )
		{
			delete [] msg;
		}
	}

	(void) fprintf(stderr, "Closing connection...");
	(void) fflush(stderr);

	sleep(3);
	if( this->sock >= 0 )
	{
		(void) ::shutdown(this->sock, 2);
		(void) CLOSESOCKET(this->sock);
		this->sock = -1;
	}
	if( this->ssl != null )
	{
		SSL_shutdown(ssl);
		SSL_free(this->ssl);
		this->ssl = null;
	}
	(void) fprintf(stderr, "DONE\n");
	(void) fprintf(stderr, "thread %lX terminated\n", (long) this->thread_id);
	(void) fflush(stderr);
	return;
}

int EppTestServerTcpThread::write( EppEntity * s )
{
	int n = 0;

	if( s != null )
	{
		DOMString t = s->toString();
		char * err = null;
		if( this->ssl != null )
		{
			err = EppMessageUtil::putEppPayload(this->ssl, t, 60);
			if( err == null )
			{
				n = t.length();
			}
		}
		else if( this->sock >= 0 )
		{
			err = EppMessageUtil::putEppPayload(this->sock, t);
			if( err == null )
			{
				n = t.length();
			}
		}
	}
	return n;
}

int EppTestServerTcpThread::write( char * buf, int len )
{
	int n = -1;
	if( this->ssl != null )
	{
		n = SSL_write(this->ssl, buf, len);
		if( n < 0 )
		{
			ERR_print_errors_fp(stderr);
		}
	}
	else if( this->sock >= 0 )
	{
#if       defined(win32)        /* { */
		n = ::send(this->sock, buf, len, 0);
#else  /* defined(win32) */	/* } { */
		n = ::write(this->sock, buf, len);
#endif /* defined(win32) */	/* } */
		if( n < 0 )
		{
			perror("write");
		}
	}
	return n;
}

class EppTestServerTcp
{
private:
	int                  port;
	bool                 useTLS;
	bool                 clientAuth;
	char               * privateKeyFileName;
	int                  privateKeyFileType;
	char               * publicKeyFileName;
	int                  publicKeyFileType;
	char               * remoteKeyFileName;
	int                  sock;
	SSL_CTX            * sslctx;
	struct sockaddr_in   sa_serv;
	EppTestServerTcpThread * threads[MAX_NUM_OF_THREADS];
	int                    num_of_threads;

	void initTLS();
	void initSocket();
	void dumpSSL( SSL * ssl );

public:
	EppTestServerTcp();
	EppTestServerTcp( int port );
	~EppTestServerTcp();
	void setPort( int port );
	void setUseTLS( bool useTLS );
	void setClientAuth( bool clientAuth );
	void setPublicKeyFile( char * name, int type );
	void setPrivateKeyFile( char * name, int type );
	void setRemoteKeyFile( char * name );
	void run();
};

EppTestServerTcp::EppTestServerTcp()
{
	this->port = -1;
	this->useTLS = false;
	this->clientAuth = false;
	this->remoteKeyFileName = null;
	this->publicKeyFileName = null;
	this->privateKeyFileName = null;
	this->privateKeyFileType = SSL_FILETYPE_PEM;
	this->publicKeyFileType  = SSL_FILETYPE_PEM;
	this->sock = -1;
	this->sslctx = null;
	this->num_of_threads = 0;
	for( int i = 0; i < MAX_NUM_OF_THREADS; i++ )
	{
		this->threads[i] = null;
	}
}

EppTestServerTcp::EppTestServerTcp( int port )
{
	this->port = port;
	this->useTLS = false;
	this->clientAuth = false;
	this->remoteKeyFileName = null;
	this->publicKeyFileName = null;
	this->privateKeyFileName = null;
	this->privateKeyFileType = SSL_FILETYPE_PEM;
	this->publicKeyFileType  = SSL_FILETYPE_PEM;
	this->sock = -1;
	this->sslctx = null;
	this->num_of_threads = 0;
	for( int i = 0; i < MAX_NUM_OF_THREADS; i++ )
	{
		this->threads[i] = null;
	}
}

EppTestServerTcp::~EppTestServerTcp()
{
	if( this->remoteKeyFileName != null )
	{
		delete [] this->remoteKeyFileName;
	}
	if( this->publicKeyFileName != null )
	{
		delete [] this->publicKeyFileName;
	}
	if( this->privateKeyFileName != null )
	{
		delete [] this->privateKeyFileName;
	}
	if( this->sock != null )
	{
		(void) CLOSESOCKET(this->sock);
		this->sock = null;
	}
	if( this->sslctx != null )
	{
		SSL_CTX_free(this->sslctx);
		this->sslctx = null;
	}
	for( int i = 0; i < this->num_of_threads; i++ )
	{
		if( this->threads[i] != null )
		{
			delete this->threads[i];
			this->threads[i] = null;
		}
	}
}

void EppTestServerTcp::setPort( int port )
{
	this->port = port;
}

void EppTestServerTcp::setUseTLS( bool useTLS )
{
	this->useTLS = useTLS;
}

void EppTestServerTcp::setClientAuth( bool clientAuth )
{
	this->clientAuth = clientAuth;
}

void EppTestServerTcp::setRemoteKeyFile( char * name )
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

void EppTestServerTcp::setPublicKeyFile( char * name, int type )
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

void EppTestServerTcp::setPrivateKeyFile( char * name, int type )
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

void EppTestServerTcp::initSocket()
{
	this->sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if( this->sock < 0 )
	{
		perror("socket");
		return;
	}
	memset(&this->sa_serv, '\0', sizeof(this->sa_serv));
	this->sa_serv.sin_family      = AF_INET;
	this->sa_serv.sin_addr.s_addr = INADDR_ANY;
	this->sa_serv.sin_port        = htons(port);

	if( bind(this->sock, (struct sockaddr *) &this->sa_serv, sizeof(this->sa_serv)) < 0 )
	{
		perror("bind");
		(void) CLOSESOCKET(this->sock);
		this->sock = -1;
		return;
	}

	if( listen(this->sock, 5) < 0 )
	{
		perror("listen");
		(void) CLOSESOCKET(this->sock);
		this->sock = -1;
		return;
	}
}


void EppTestServerTcp::initTLS()
{
	static bool tlsInit = false;
	if( tlsInit == false )
	{
		SSL_load_error_strings();
		SSL_library_init();
//		OpenSSL_add_all_algorithms();
		OpenSSL_add_ssl_algorithms();

		tlsInit = true;
	}
//	this->sslctx = SSL_CTX_new(TLSv1_server_method());
	this->sslctx = SSL_CTX_new(SSLv23_server_method());
	if( this->sslctx == null )
	{
		ERR_print_errors_fp(stderr);
		(void) fprintf(stderr, "Cannot initialize OpenSSL: SSL_CTX_new()");
		return;
	}
	if( SSL_CTX_use_certificate_file(this->sslctx, this->publicKeyFileName, this->publicKeyFileType) <= 0 )
	{
		ERR_print_errors_fp(stderr);
		SSL_CTX_free(this->sslctx);
		this->sslctx = null;
		return;
	}
	if( SSL_CTX_use_PrivateKey_file(this->sslctx, this->privateKeyFileName, this->privateKeyFileType) <= 0 )
	{
		ERR_print_errors_fp(stderr);
		SSL_CTX_free(this->sslctx);
		this->sslctx = null;
		return;
	}
	if( ! SSL_CTX_check_private_key(this->sslctx) )
	{
		(void) fprintf(stderr, "Private key does not match the certificate public key\n");
		SSL_CTX_free(this->sslctx);
		this->sslctx = null;
		return;
	}
	if(  SSL_CTX_load_verify_locations(this->sslctx, this->remoteKeyFileName, null ) < 0 )
	{
		(void) fprintf(stderr, "Cannot load remote client public key\n");
		SSL_CTX_free(this->sslctx);
		this->sslctx = null;
		return;
	}
	if( SSL_CTX_set_default_verify_paths(this->sslctx) < 0 )
	{
		(void) fprintf(stderr, "SSL_CTX_set_default_verify_paths failed\n");
		SSL_CTX_free(this->sslctx);
		this->sslctx = null;
		return;
	}
	SSL_CTX_set_verify(this->sslctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, &verify_callback);
}

void EppTestServerTcp::dumpSSL( SSL * ssl )
{
	cerr << "SSL/TLS: Version " << SSL_get_version(ssl) << endl;

	SSL_CIPHER * ciph = (SSL_CIPHER *)SSL_get_current_cipher(ssl);
	if( ciph == null )
	{
		cerr << "Cipher cannot be found" << endl;
	}
	else
	{
		cerr << "Cipher : Version " << SSL_CIPHER_get_version(ciph);
		cerr << " " << SSL_CIPHER_get_name(ciph) << endl;
	}
	X509 * cert = SSL_get_peer_certificate(ssl);
	if( cert == null )
	{
		cerr << "Cannot get peer certificate" << endl;
		return;
	}
	char * str;
	str = X509_NAME_oneline (X509_get_subject_name(cert), 0, 0);
	if( str == null )
	{
		cerr << "Subject: " << "none" << endl;
	}
	else
	{
		cerr << "Subject: " << str << endl;
		(void) free(str);
	}
	str = X509_NAME_oneline (X509_get_issuer_name(cert), 0, 0);
	if( str == null )
	{
		cerr << "Issuer : " << "none" << endl;
	}
	else
	{
		cerr << "Issuer : " << str << endl;
		(void) free(str);
	}
	EVP_PKEY * pkey = X509_get_pubkey(cert);
	if( pkey == null )
	{
		cerr << "Pubkey : " << "none" << endl;
	}
	else if( (pkey->type == EVP_PKEY_RSA) && (pkey->pkey.rsa != null) )
	{
		cerr << "Pubkey : " << BN_num_bits(pkey->pkey.rsa->n);
		cerr << " bit RSA" << endl;
	}
	else if( (pkey->type == EVP_PKEY_DSA) && (pkey->pkey.dsa != null) )
	{
		cerr << "Pubkey : " << BN_num_bits(pkey->pkey.dsa->p);
		cerr << " bit DSA" << endl;
	}
	EVP_PKEY_free(pkey);
	X509_free(cert);
}

#if       defined(win32)        /* { */
long WINAPI runThread( void * arg )
#else  /* defined(win32) */	/* } { */
void * runThread( void * arg )
#endif /* defined(win32) */	/* } */
{
	EppTestServerTcpThread * t = (EppTestServerTcpThread *) arg;
	t->run();
	return null;
}

void EppTestServerTcp::run()
{
	if( this->sock >= 0 )
	{
		CLOSESOCKET(this->sock);
		this->sock = -1;
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
	if( this->useTLS )
	{
		initTLS();
		if( this->sslctx == null )
		{
			// error
			return;
		}
	}

	(void) fprintf(stderr, "Server started, listening to port %d\n", this->port);
	(void) fflush(stderr);

	while( 1 )
	{
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int client_sock = accept(this->sock, (struct sockaddr *) &client_addr, &client_addr_len);
		if( client_sock < 0 )
		{
			perror("accept");
			return;		
		}
		(void) fprintf(stderr, "Connection from %lx, port %x\n",
			(unsigned long) client_addr.sin_addr.s_addr, client_addr.sin_port);
		(void) fflush(stderr);
		if( num_of_threads >= MAX_NUM_OF_THREADS )
		{
			(void) fprintf(stderr, "No more threads, max %d\n", MAX_NUM_OF_THREADS);
			(void) fflush(stderr);
			(void) ::shutdown(client_sock, 2);
			(void) CLOSESOCKET(client_sock);
			continue;
		}

		SSL * ssl = null;
		if( this->useTLS )
		{
			ssl = SSL_new(this->sslctx);
			if( ssl == null )
			{
				(void) fprintf(stderr, "SSL_new() failure\n");
				CLOSESOCKET(client_sock);
				continue;
			}
			SSL_set_fd(ssl, client_sock);
			if( SSL_accept(ssl) < 0 )
			{
				ERR_print_errors_fp(stderr);
				SSL_shutdown(ssl);
				SSL_free(ssl);
				CLOSESOCKET(client_sock);
				continue;
			}
			dumpSSL(ssl);
			(void) fflush(stderr);
		}

		EppTestServerTcpThread * t = new EppTestServerTcpThread(client_sock, ssl);
		if( t == null )
		{
			(void) fprintf(stderr, "No memory for thread\n");
			SSL_shutdown(ssl);
			SSL_free(ssl);
			CLOSESOCKET(client_sock);
			continue;
		}

#if       defined(win32)        /* { */
		t->thread_handle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)
				runThread, (void *) t, 0, &t->thread_id);
		if( t->thread_handle == null )
#else  /* defined(win32) */	/* } { */
		if( pthread_create(&t->thread_id, null, &runThread, (void *) t) != 0 )
#endif /* defined(win32) */	/* } */
		{
			perror("thread_create");
			delete t;
			continue;
		}
		this->threads[this->num_of_threads++] = t;
	}
}

void domInit()
{
	try
	{
		XMLPlatformUtils::Initialize();
	}
	catch( const XMLException& e )
	{
                cerr    << "Error during Xerces-c Initialization\n"
                        << "  Exception message:"
                        << DOMString(e.getMessage()) << endl;
		exit(0);
	}
}

void domFinish()
{
	XMLPlatformUtils::Terminate();
}

int main( int argc, char * argv[] )
{
	int port = 10288;
	bool useTLS = true;

	if( argc > 1 )
	{
		port = atoi(argv[1]);
	}
	if( argc > 2 )
	{
		if( strcmp("notls", argv[2]) == 0 )
		{
			useTLS = false;
		}
	}

#if	   defined(win32)       /* { */
	static WSADATA wsa_data;
	static int     wsa_status;

	wsa_status = WSAStartup(MAKEWORD(0x1, 0x1), &wsa_data);
	if( wsa_status == 0 )
	{
#endif	/* defined(win32) */	/* } */

	domInit();

	EppTestServerTcp server(port);
	server.setUseTLS(useTLS);
	server.setRemoteKeyFile("testkeys.client.public.pem");
	server.setPublicKeyFile("testkeys.server.public.pem", SSL_FILETYPE_PEM);
	server.setPrivateKeyFile("testkeys.server.private.pem", SSL_FILETYPE_PEM);
	server.run();

	domFinish();

#if	   defined(win32)       /* { */
	WSACleanup();
	}
#endif	/* defined(win32) */	/* } */

	return 0;
}
