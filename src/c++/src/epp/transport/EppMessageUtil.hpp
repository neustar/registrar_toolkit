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

#if ! defined(EPPMESSAGEUTIL_HPP)    /* { */
#define       EPPMESSAGEUTIL_HPP        1

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

#else  /* defined(win32) */     /* } { */

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
#include "XercesString.hpp"

#if ! defined(EPP_EXPORT)    /* { */
#define       EPP_EXPORT
#endif     /* EPP_EXPORT) */ /* } */

/**
 * This <code>EppSessionUtil</code> class contains some utility methods for
 * handling message exchanges between an EPP Server and an EPP Client.
 *
 * @author Ning Zhang ning.zhang@neustar.com
 * @version $Revision: 1.3 $ $Date: 2007/09/26 20:41:43 $
 */
class EPP_EXPORT EppMessageUtil
{
private:
	static bool writeMessageSize;
	static bool fetchMessageSize;

	/**
	 * Gets the EPP message payload size from an input stream
	 *
	 * @param socket the socket from which the EPP
	 *           message payload size is retrieved
	 * @param quit the flag indicating if the thread should quit without
	 *        completing the reading. The flag is set by another thread
	 *	  to notify the thread to exit.
	 * @param errbuf buffer into which error message is returned (if method returns &lt; 0).
	 * @param bufsiz size of buffer.
	 */
	static int getEppPayloadSize( int socket, bool * quit, char * errbuf, size_t bufsiz );

	/**
	 * Gets the EPP message payload size from an input stream
	 *
	 * @param ssl the <code>SSL</code> object of OPENSSL from which the EPP
	 *           message payload size is retrieved
	 * @param quit the flag indicating if the thread should quit without
	 *        completing the reading. The flag is set by another thread
	 *	  to notify the thread to exit.
	 * @param Max number of seconds after which operation of this method would timeout
	 *        this is mainly for SSL_read and SSL_write operations. It is responsibility 
         *        of caller to choose correct value of this param.
	 *
	 * @param errbuf buffer into which error message is returned (if method returns &lt; 0).
	 * @param bufsiz size of buffer.
	 */
	static int getEppPayloadSize( SSL * ssl, bool * quit, char * errbuf, size_t bufsiz, int timeout );

public:

	/**
	 * Sets the flags for handling EPP message size
	 */
	static void setEppMessageSizeFlags( bool write, bool fetch )
	{
		writeMessageSize = write;
		fetchMessageSize = fetch;
	};

	/**
	 * Gets the flag for outputing the size of an outgoing EPP message
	 */
	static bool getEppMessageSizeWriteFlag()
	{
		return writeMessageSize;
	};

	/**
	 * Gets the flag for extracting the size of an incoming EPP message
	 */
	static bool getEppMessageSizeFetchFlag()
	{
		return fetchMessageSize;
	};

	/**
	 * Sends a string over a socket and reads a string back from
	 * the socket
	 *
	 * @param socket the socket used for sending the string
	 * @param str    the string to be sent over the socket
	 * @param length the length of the string to be returned
	 *
	 * @return a string containing the EPP message payload, or an
	 *         error message if there is any error associated with
	 *         the connection to the server or running out of memory
	 *
	 * @note the caller should free the memory containing the EPP message
	 *       payload, if there is no error. If there is any error, the
	 *       length parameter would be set to 0 and a string containing
	 *       the error message should not be freed by the caller.
	 */
	static char * send( int socket, const DOMString& str, int * length );

	/**
	 * Sends a string over a socket and reads a string back from
	 * the socket
	 *
	 * @param ssl    the <code>SSL</code> object of OPENSSL used for
	 *               sending the string
	 * @param str    the string to be sent over the socket
	 * @param length the length of the string to be returned
	 *
	 * @param Max number of seconds after which operation of this method would timeout
	 *        this is mainly for SSL_read and SSL_write operations. It is responsibility 
         *        of caller to choose correct value of this param.
	 *
	 * @return a string containing the EPP message payload, or an
	 *         error message if there is any error associated with
	 *         the connection to the server or running out of memory
	 *
	 * @note the caller should free the memory containing the EPP message
	 *       payload, if there is no error. If there is any error, the
	 *       length parameter would be set to 0 and a string containing
	 *       the error message should not be freed by the caller.
	 */
	static char * send( SSL * ssl, const DOMString& str, int * length , int timeout);

	/**
	 * Sends out an outgoing EPP message
	 *
	 * @param ssl the <code>SSL</code> object of OPENSSL to which the EPP
	 *           message payload is sent
	 *
	 * @param str    the string to be sent out
	 *
	 * @return null if there is no error, or an error message
	 */
	static char * putEppPayload( int socket, const DOMString& str );

	/**
	 * Sends out an outgoing EPP message
	 *
	 * @param socket the SSL socket to which the EPP message payload is sent
	 *
	 * @param str    the string to be sent out
	 *
	 * @param Max number of seconds after which operation of this method would timeout
	 *        this is mainly for SSL_read and SSL_write operations. It is responsibility 
         *        of caller to choose correct value of this param.
	 *
	 * @return null if there is no error, or an error message
	 */
	static char * putEppPayload( SSL * ssl, const DOMString& str, int timeout );

	/**
	 * Gets the EPP message payload from an input stream
	 *
	 * @param socket the socket from which the EPP
	 *           message payload is retrieved
	 * @param length the length of the string to be returned
	 *
	 * @return a string containing the EPP message payload, or an
	 *         error message if there is any error associated with
	 *         the connection to the server or running out of memory
	 *
	 * @note the caller should free the memory containing the EPP message
	 *       payload, if there is no error. If there is any error, the
	 *       length parameter would be set to 0 and a string containing
	 *       the error message should not be freed by the caller.
	 */
	static char * getEppPayload( int socket, int * length );

	/**
	 * Gets the EPP message payload from an input stream
	 *
	 * @param socket the socket from which the EPP
	 *           message payload is retrieved
	 * @param length the length of the string to be returned
	 * @param quit the flag indicating if the thread should quit without
	 *        completing the reading. The flag is set by another thread
	 *	  to notify the thread to exit.
	 * @param the maximum EPP message payload size, or 0 if there is no limit
	 *
	 * @return a string containing the EPP message payload, or an
	 *         error message if there is any error associated with
	 *         the connection to the server or running out of memory
	 *
	 * @note the caller should free the memory containing the EPP message
	 *       payload, if there is no error. If there is any error, the
	 *       length parameter would be set to 0 and a string containing
	 *       the error message should not be freed by the caller. if
	 *
	 * @note if the maximum size is reached, the length parameter is set to
	 *       the negative of message length received
	 */
	static char * getEppPayload( int socket, int * length, bool * quit, int maxsize );

	/**
	 * Gets the EPP message payload from an input stream
	 *
	 * @param ssl the <code>SSL</code> object of OPENSSL from which the EPP
	 *           message payload is retrieved
	 * @param length the length of the string to be returned
	 *
	 * @return a string containing the EPP message payload, or an
	 *         error message if there is any error associated with
	 *         the connection to the server or running out of memory
	 * @param Max number of seconds after which operation of this method would timeout
	 *        this is mainly for SSL_read and SSL_write operations. It is responsibility 
         *        of caller to choose correct value of this param.
	 *
	 * @note the caller should free the memory containing the EPP message
	 *       payload, if there is no error. If there is any error, the
	 *       length parameter would be set to 0 and a string containing
	 *       the error message should not be freed by the caller.
	 */
	static char * getEppPayload( SSL * ssl, int * length, int timeout );

	/**
	 * Gets the EPP message payload from an input stream
	 *
	 * @param ssl the <code>SSL</code> object of OPENSSL from which the EPP
	 *           message payload is retrieved
	 * @param length the length of the string to be returned
	 * @param quit the flag indicating if the thread should quit without
	 *        completing the reading. The flag is set by another thread
	 *	  to notify the thread to exit.
	 * @param the maximum EPP message payload size, or 0 if there is no limit
	 * @param Max number of seconds after which operation of this method would timeout
	 *        this is mainly for SSL_read and SSL_write operations. It is responsibility 
         *        of caller to choose correct value of this param.
	 *
	 * @return a string containing the EPP message payload, or an
	 *         error message if there is any error associated with
	 *         the connection to the server or running out of memory
	 *
	 * @note the caller should free the memory containing the EPP message
	 *       payload, if there is no error. If there is any error, the
	 *       length parameter would be set to 0 and a string containing
	 *       the error message should not be freed by the caller.
	 *
	 * @note if the maximum size is reached, the length parameter is set to
	 *       the negative of message length received
	 */
	static char * getEppPayload( SSL * ssl, int * length, bool * quit, int maxsize, int timeout );

	/**
	 * Checks if there is timeout/error/input in a socket
	 * @param sock socket to be checked
	 * @param timeout timeout in seconds
	 *
	 * @return 1 for input, 0 for timeout, -1 for error
	 */
	static int checkSocket( int sock, int timeout );
	/**
	 * This method tried to see if SSL operation which return sslErrNum should be retried or not.
	 * 
         * @param sslErrNum: SSL error number return while performing SSL_read/write/accept/connect
         * @param tStarted: time when the SSL operation was initially started.
         * @param secTimeout: Timeout value in seconds. Operation would not timeout if this param is set to 0
         * @return true if SSL operation should be re-tried false otherwise
         */
	static bool shouldSSLRetry( int sslErrNum, time_t tStarted, int secTimeout );
};

#endif     /* EPPMESSAGEUTIL_HPP */  /* } */
