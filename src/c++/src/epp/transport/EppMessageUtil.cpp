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
#include <string.h>
#include <memory.h>
#if       ! defined(win32)      /* { */
#include <sys/time.h>
#endif /* ! defined(win32) */   /* } */
#include "EppMessageUtil.hpp"
#include "EppUtil.hpp"

#ifndef	BUFSIZ
#define	BUFSIZ	4096
#endif


bool EppMessageUtil::writeMessageSize = true;
bool EppMessageUtil::fetchMessageSize = true;

char * EppMessageUtil::send( int socket, const DOMString& str, int * length )
{
	*length = 0;
	char * p = EppMessageUtil::putEppPayload(socket, str);
	if( p != null )
	{
		return p;
	}
	return EppMessageUtil::getEppPayload(socket, length);
}

char * EppMessageUtil::getEppPayload( int socket, int * length )
{
	bool quit = false;
	return EppMessageUtil::getEppPayload(socket, length, &quit, 0);
}

char * EppMessageUtil::getEppPayload( int socket, int * length, bool * quit, int maxsize )
{
	*length = 0;
	int    i = 0;
	int    len = BUFSIZ;
	char * buf = new char[BUFSIZ];

	if( buf == null )
		return buf; // no mem

	bool endFound = false;
	bool eppFound = false;
	bool eppStart = false;
	int  eppSize  = 0;

	if( fetchMessageSize == true )
	{
		eppSize = EppMessageUtil::getEppPayloadSize(socket, quit, buf, BUFSIZ);
		if( eppSize <= 0 )
		{
			return buf;
		}
	}

	while( ! endFound )
	{
		char c;
		int n;
		bool redo = false;

		do
		{
#if       defined(win32)        /* { */
			n = ::recv(socket, &c, 1, 0);
#else /* defined(win32) */     /* } */
			n = ::read(socket, &c, 1);
#endif /* defined(win32) */     /* } */
			if( n < 0 && errno == EINTR )
				redo = true;

			//EppUtil::msSleep(1000);
			if( *quit == true )
			{
				strncpy(buf, "Error in EppMessageUtil::getEppPayload() timeout/quit", BUFSIZ);
				buf[BUFSIZ-1] = '\0';
				return buf;
			}
		} while( redo );

		if( n <= 0 )
		{
			// error
			*quit = true;
			strncpy(buf, "Error in EppMessageUtil::getEppPayload() read()", BUFSIZ);
			buf[BUFSIZ-1] = '\0';
			return buf;
		}
		if( eppStart == false )
		{
			if( c != '<' )
			{
				continue;
			}
			eppStart = true;
		}

		// EPP payload rarely exceeds 8KB, so this
		// doubling-up should not occur very often

		if( i == (len - 1) )
		{
			// check if the maxsize is reached

			if( (maxsize > 0) && (len >= maxsize) )
			{
				*length = -len;
				strncpy(buf, "EPP message payload size exceeds the maximum limit", BUFSIZ);
				buf[BUFSIZ-1] = '\0';
				return buf;
			}

			char * newbuf = new char[len + BUFSIZ];
			if( newbuf == null )
			{
				strncpy(buf, "Error in EppMessageUtil::getEppPayload() no memory", BUFSIZ);
				buf[BUFSIZ-1] = '\0';
				return buf;
			}
			(void) memcpy(newbuf, buf, len);
			delete [] buf;
			buf = newbuf;
			len = len + BUFSIZ;
		}
		buf[i++] = c;

		// check if we have got the "</epp" tag

		if(    ! eppFound
			&& (i >= 5)
			&& (buf[i - 5] == '<')
			&& (buf[i - 4] == '/')
			&& (buf[i - 3] == 'e')
			&& (buf[i - 2] == 'p')
			&& (buf[i - 1] == 'p') )
		{
			eppFound = true;
		}
		if( eppFound && (buf[i - 1] == '>') )
		{
			endFound = true;
		}

		// check if we have reached the size

		if( fetchMessageSize == true )
		{
			if( i == eppSize )
			{
				endFound = true;
			}
			else
			{
				endFound = false;
			}
		}
	}

	// ok, we have got a buffer, either with a full EPP payload, or
	// end of the input stream

	buf[i] = 0;
	*length = i;

	// trim the buffer

	return buf;
}

char * EppMessageUtil::send( SSL * ssl, const DOMString& str, int * length, int timeout )
{
	*length = 0;
	char * p = EppMessageUtil::putEppPayload(ssl, str, timeout);
	if( p != null )
	{
		return p;
	}
	return EppMessageUtil::getEppPayload(ssl, length, timeout);
}

char * EppMessageUtil::getEppPayload( SSL * ssl, int * length, int timeout )
{
	bool quit = false;
	return EppMessageUtil::getEppPayload(ssl, length, &quit, 0, timeout);
}

char * EppMessageUtil::getEppPayload( SSL * ssl, int * length, bool * quit, int maxsize, int timeout )
{
	*length = 0;
	int    i = 0;
	int    len = BUFSIZ;
	char * buf = new char[BUFSIZ];

	if( buf == null )
		return buf; // no mem

	bool endFound = false;
	bool eppFound = false;
	bool eppStart = false;
	int  eppSize  = 0;

	if( fetchMessageSize == true )
	{
		eppSize = EppMessageUtil::getEppPayloadSize(ssl, quit, buf, BUFSIZ, timeout);
		if( eppSize <= 0 )
		{
			return buf;
		}
	}

	while( ! endFound )
	{
		char c;
		int n;
		time_t tStarted= time(0);
		while ( true )
		{
			if ( *quit == true )
			{
				strncpy(buf, "Error in EppMessageUtil::getEppPayload() timeout/quit", BUFSIZ);
				buf[BUFSIZ-1] = '\0';
				return buf;
			}

			ERR_clear_error();
			n = SSL_read(ssl, (char *)&c, 1);

			if ( n > 0 ) // we read something
				break;

			if ( n < 0 )
			{
				if ( EppMessageUtil::shouldSSLRetry( SSL_get_error(ssl, n), tStarted, timeout ) )
				{
					continue;
				}

				snprintf(buf, BUFSIZ, "Error in EppMessageUtil::getEppPayload() SSL_read error [rv=%d, ssl_err=%d, errno=%d]", n, SSL_get_error(ssl, n), errno);
			}
			else
			{ // n == 0
				strncpy(buf, "Error in EppMessageUtil::getEppPayload() connection closed", BUFSIZ);
			}
			buf[BUFSIZ-1] = '\0';

			// error - disconnected or other error
			*quit = true;

			return buf;
		}

		//cout << c;
		if( eppStart == false )
		{
			if( c != '<' )
			{
				continue;
			}
			eppStart = true;
		}

		// EPP payload rarely exceeds 8KB, so this
		// doubling-up should not occur very often

		if( i == (len - 1) )
		{
			// check if the maxsize is reached

			if( (maxsize > 0) && (len >= maxsize) )
			{
				*length = -len;
				strncpy(buf, "EPP message payload size exceeds the maximum limit", BUFSIZ);
				buf[BUFSIZ-1] = '\0';
				return buf;
			}

			char * newbuf = new char[len + BUFSIZ];
			if( newbuf == null )
			{
				strncpy(buf, "Error in EppMessageUtil::getEppPayload() no memory", BUFSIZ);
				buf[BUFSIZ-1] = '\0';
				return buf;
			}
			(void) memcpy(newbuf, buf, len);
			delete [] buf;
			buf = newbuf;
			len = len + BUFSIZ;
		}
		buf[i++] = c;

		// check if we have got the "</epp" tag

		if(    ! eppFound
			&& (i >= 5)
			&& (buf[i - 5] == '<')
			&& (buf[i - 4] == '/')
			&& (buf[i - 3] == 'e')
			&& (buf[i - 2] == 'p')
			&& (buf[i - 1] == 'p') )
		{
			eppFound = true;
		}
		if( eppFound && (buf[i - 1] == '>') )
		{
			endFound = true;
		}

		// check if we have reached the size

		if( fetchMessageSize == true )
		{
			if( i == eppSize )
			{
				endFound = true;
			}
			else
			{
				endFound = false;
			}
		}
	}

	// ok, we have got a buffer, either with a full EPP payload, or
	// end of the input stream

	buf[i] = 0;
	*length = i;

	// trim the buffer

	return buf;
}

int EppMessageUtil::checkSocket( int sock, int timeout )
{
	int rtn;
	fd_set fi;
	fd_set fo;
	fd_set fe;
	struct timeval tp;

	FD_ZERO(&fi);
	FD_ZERO(&fo);
	FD_ZERO(&fe);

	FD_SET(sock, &fi);
	//FD_SET(sock, &fo);
	FD_SET(sock, &fe);

	tp.tv_sec = timeout;
	tp.tv_usec = 0;

	rtn = ::select( sock + 1, &fi, &fo, &fe, &tp);
	if( rtn < 0 )
	{
		return -1;
	}
	if( rtn == 0 )
	{
		return 0;
	}
	if( FD_ISSET(sock, &fi) )
	{
		return +1;
	}
	if( FD_ISSET(sock, &fe) )
	{
		return -1;
	}
	return 0;
}

char * EppMessageUtil::putEppPayload( int socket, const DOMString& str )
{
	char	* p = str.transcode();
	int	  n;
	if( p == null )
	{
		return "Error in EppMessageUtil::send() no memory";
	}
	int l = ::strlen(p);
	if( writeMessageSize == true )
	{
		uint32_t len = htonl(l + 4);
		if( l <= (BUFSIZ - 4) )
		{
			char tmp_buf[BUFSIZ];
			(void) ::memcpy(tmp_buf    , (char *) &len, 4);
			(void) ::memcpy(tmp_buf + 4, (char *) p   , l);
#if       defined(win32)        /* { */
			n = ::send(socket, tmp_buf, l + 4, 0);
#else /* defined(win32) */     /* } */
			n = ::write(socket, tmp_buf, l + 4);
#endif /* defined(win32) */     /* } */
			delete [] p;
			if( n != (l + 4) )
			{
				return "Error in EppMessageUtil::putEppPayload() write error";
			}
			return null;
		}
#if       defined(win32)        /* { */
		n = ::send(socket, (char *) &len, sizeof(uint32_t), 0);
#else /* defined(win32) */     /* } */
		n = ::write(socket, (char *) &len, sizeof(uint32_t));
#endif /* defined(win32) */     /* } */
		if( n != sizeof(uint32_t) )
		{
			return "Error in EppMessageUtil::putEppPayload() write error";
		}
	}
#if       defined(win32)        /* { */
	n = ::send(socket, p, l, 0);
#else /* defined(win32) */     /* } */
	n = ::write(socket, p, l);
#endif /* defined(win32) */     /* } */
	XercesString::Delete(p);
	if( n != l )
	{
		return "Error in EppMessageUtil::putEppPayload() write error";
	}
	return null;
}

char * EppMessageUtil::putEppPayload( SSL * ssl, const DOMString& str, int timeout)
{
	char	* p = str.transcode();
	int	  n;
	if( p == null )
		return null; // no mem

	int l = strlen(p);
	if( writeMessageSize == true )
	{
		uint32_t len = htonl(l + 4);
		if( l <= (BUFSIZ - 4) )
		{
			char tmp_buf[BUFSIZ];
			(void) ::memcpy(tmp_buf    , (char *) &len, 4);
			(void) ::memcpy(tmp_buf + 4, (char *) p   , l);
			delete [] p;
			time_t tStarted = time(0);
			while ( true )
			{
				ERR_clear_error();
				n = SSL_write(ssl, tmp_buf, l + 4);
				if ( n < 0 )
				{
					if ( EppMessageUtil::shouldSSLRetry( SSL_get_error(ssl, n), tStarted, timeout ) )
					{
						continue;
					}
				}
				break; // either success, or connection closed, or error
			}
			if( n != (l + 4) )
			{
				return "Error in EppMessageUtil::putEppPayload() write error";
			}
			return null;
		}

		time_t tStarted = time(0);
		while ( true )
		{
			ERR_clear_error();
			n = SSL_write(ssl, (char *) &len, sizeof(uint32_t));
			if ( n < 0 )
			{
				if ( EppMessageUtil::shouldSSLRetry( SSL_get_error(ssl, n), tStarted, timeout ) )
				{
					continue;
				}
			}
			break; // either success, or connection closed, or error
		}
		if( n != sizeof(uint32_t) )
		{
			return "Error in EppMessageUtil::putEppPayload() write error";
		}
	}

	time_t tStarted = time(0);
	while ( true )
	{
		ERR_clear_error();
		n = SSL_write(ssl, p, l);
		if ( n < 0 )
		{
			if ( EppMessageUtil::shouldSSLRetry( SSL_get_error(ssl, n), tStarted, timeout ) )
			{
				continue;
			}
		}
		break; // either success, or connection closed, or error
	}
	XercesString::Delete(p);
	if( n != l )
	{
		return "Error in EppMessageUtil::putEppPayload() write error";
	}
	return null;
}

int EppMessageUtil::getEppPayloadSize( int socket, bool * quit, char *errbuf, size_t bufsiz )
{
	int size = 0;
	for( int i = 0; i < 4; i++ )
	{
		unsigned char c;
		int n;
		if( 0 == socket )
		{
			*quit = true;
			strncpy(errbuf, "Error in EppMessageUtil::getEppPayloadSize():connection closed", bufsiz);
			errbuf[bufsiz-1] = '\0';
			return -2;
		}
		bool redo = false;
		do
		{
#if       defined(win32)        /* { */
			n = ::recv(socket, (char *) &c, 1, 0);
#else /* defined(win32) */     /* } */
			n = ::read(socket, &c, 1);
#endif /* defined(win32) */     /* } */
			if( n < 0 && errno == EINTR )
				redo = true;
			if( *quit == true )
			{
				strncpy(errbuf, "Error in EppMessageUtil::getEppPayloadSize() timeout/quit", bufsiz);
				errbuf[bufsiz-1] = '\0';
				return -2;
			}
		} while( redo );

		if( n <= 0 )
		{
			// error
			snprintf(errbuf, bufsiz, "Error in EppMessageUtil::getEppPayloadSize() recv error=%d", errno);
			errbuf[bufsiz-1] = '\0';
			*quit = true;
			return -2;
		}
		size = size << 8;
		size |= (unsigned int) c;
	}

	size -= 4;
	return size;
}

int EppMessageUtil::getEppPayloadSize( SSL * ssl, bool * quit, char *errbuf, size_t bufsiz, int timeout )
{
	int size = 0;
	for( int i = 0; i < 4; i++ )
	{
		unsigned char c;
		int n;

		int tStarted = time(0);
		while( true )
		{
			if ( *quit == true )
			{
				strncpy(errbuf, "Error in EppMessageUtil::getEppPayloadSize() timeout/quit", bufsiz);
				errbuf[bufsiz-1] = '\0';
				return -2;
			}

			ERR_clear_error();
			n = SSL_read(ssl, (char *)&c, 1);
			if ( n > 0 )
				break; // successful read

			if ( n < 0 )
			{
				if ( EppMessageUtil::shouldSSLRetry( SSL_get_error(ssl, n), tStarted, timeout ) )
				{
					continue;
				}

				snprintf(errbuf, bufsiz, "Error in EppMessageUtil::getEppPayloadSize() SSL_read error timeout[%d] [rv=%d, ssl_err=%d, errno=%d]", timeout, n, SSL_get_error(ssl, n), errno);
			}
			else
			{ // n == 0
				strncpy(errbuf, "Error in EppMessageUtil::getEppPayloadSize() connection closed", bufsiz);
			}
			errbuf[bufsiz-1] = '\0';

			// error - disconnected or other error
			*quit = true;

			return ((n < 0) ? -4 // syscall
				: -6); // other errors
		}
		size = size << 8;
		size |= (unsigned int) c;
	}

	size -= 4;
	return size;
}

bool EppMessageUtil::shouldSSLRetry( int sslErrNum, time_t tStarted, int secTimeout )
{
	if ( sslErrNum == SSL_ERROR_WANT_READ || sslErrNum == SSL_ERROR_WANT_WRITE )
	{
		if ( ( (time(0) - tStarted) < secTimeout ) || secTimeout == 0 )
		{
			EppUtil::msSleep(50);
			return true;
		}
	}
	return false;
}