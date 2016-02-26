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
#if ! defined(_XOPEN_SOURCE)    /* { */
/*
 * for strptime() only
 */
#define       _XOPEN_SOURCE        1
#endif     /* _XOPEN_SOURCE */  /* } */

#include <stdio.h>
#include <string.h>
#if	   ! defined(WIN32)  	/* { */
//#include <stream.h>
#endif	/* ! defined(WIN32) */	/* } */
#include <iostream>
#include <time.h>
#include "EppEntity.hpp"
#include "EppUtil.hpp"

#if        defined(solaris)     /* { */
extern "C"
{
	extern char *strptime(const char *, const char *, struct tm *);
};
#endif	/* defined(solaris) */  /* } */

static const char * estr [6] = { "&lt;", "&gt;", "&quot;", "&amp;", "&#13;", "&#10" };

// For EPP-04
//
// #define	EPP_NAME_SPACE	"urn:iana:xml:ns:"
//
#define	EPP_NAME_SPACE	"urn:ietf:params:xml:ns:"

#define	   EPP_SUPPORT_UNSIGNED_TIME_T		   1
#if	   EPP_SUPPORT_UNSIGNED_TIME_T		/* { */
#include   "EppUtilTime.cpp"
#endif	/* EPP_SUPPORT_UNSIGNED_TIME_T	*/	/* } */

/**
 * This <code>EppWriter</code> class is specifically designed to add
 * name space prefix string inside object specific elements for fixing
 * the problem in Tucows' EPP Toolkit that it looks for hard-coded tag
 * names like "contact:roid", which is bad programming practice.
 */
class EppWriter
{
private:
	bool canonical;
	DOMString out;
	DOMString encoding;

public:

	EppWriter()
	{
		canonical = false;
		out = DOMString("");
		encoding = DOMString("UTF-8");
	};

	~EppWriter() {};

	DOMString getOutput()
	{
		return out;
	}

	void print( const DOMNode &node )
	{
		this->print(node, 0, NULLXS);
	};

	void print( const DOMNode &node, int level, DOMString prefix );

	/**
	 * Normalizes a string by escaping special characters such as
	 * ';&lt;', '&gt;', '&amp;', '&quot;', '\r', '\n'
	 */
	void normalize( DOMString& out, DOMString& s )
	{

		int len = s.length();

		if( len == 0 )
		{
			return;
		}

		for ( int i = 0; i < len; i++ )
		{
			XMLCh ch = s[i];
			if( ch == '<' )
			{
				out += estr[0];
			}
			else if( ch == '>' )
			{
				out += estr[1];
			}
			else if( ch == '"' )
			{
				out += estr[2];
			}
			else if( ch == '&' )
			{

				out += estr[3];
			}
			else if( canonical && (ch == '\r') )
			{
				out += estr[4];
			}
			else if( canonical && (ch == '\n') )
			{
				out += estr[5];
			}
			else
			{
				out += ch;
			}
		}
	};
};

void EppWriter::print( const DOMNode &node, int level, DOMString prefix )
{
	unsigned int i = 0;
	int j = 0;

	DOMNodeList* list;
	DOMNamedNodeMap* attrs;
	DOMString newPrefix;
	int type = node.getNodeType();
	switch( type )
	{
		case DOMNode::DOCUMENT_NODE:
			if( ! canonical )
			{
				out += XS("<?xml version=\"1.0\" encoding=\"")
					+ encoding + "\"?>";
				out += "\n";
			}
			list = node.getChildNodes();
			if( list != null )
			{
				for( i = 0; i < list->getLength(); i++ )
				{
					print(*list->item(i), level, prefix);
				}
			}
			break;

		case DOMNode::ELEMENT_NODE:
			for( j = 0; j < level; j++ )
			{
				out += " ";
			}
			out += "<";
			newPrefix = node.getPrefix();
			if( newPrefix.isNotNull() )
			{
				prefix = newPrefix;
			}
			if( (newPrefix.isNull()) && (prefix.isNotNull()) )
			{
				out += prefix;
				out += ":";
			}
			out += node.getNodeName();
			attrs = node.getAttributes();
			if( attrs != null )
			{
				for( i = 0; i < attrs->getLength(); i++ )
				{
					DOMNode* attr = attrs->item(i);
					out += " ";
					out += attr->getNodeName();
					out += "=\"";
					out += attr->getNodeValue();
					out += "\"";
				}
			}
			list = node.getChildNodes();
			if( (list == null) || (list->getLength() == 0) )
			{
				out += "/>\n";
			}
			else
			{
				bool cr = false;
				out += ">";
				for( i = 0; i < list->getLength(); i++ )
				{
					if( i == 0 )
					{
						if( list->item(0)->getNodeType() != DOMNode::TEXT_NODE )
						{
							out += "\n";
							cr = true;
						}
					}
					print(*list->item(i), level + 1, prefix);
				}
				if( cr )
				{
					for( j = 0; j < level; j++ )
					{
						out += " ";
					}
				}
				out += "</";
				if( (newPrefix.isNull()) && (prefix.isNotNull()) )
				{
					out += prefix;
					out += ":";
				}
				out += node.getNodeName();
				out += ">\n";
			}
			break;

		case DOMNode::TEXT_NODE:
		{
			DOMString p = node.getNodeValue();
			this->normalize(out, p);
			//			out += node.getNodeValue();
		}
			break;

		default:
			break;
	}
}

char EppUtil::version[MAX_VERSION_LENGTH] = { '-', '1', '.', '0', 0};

void EppUtil::setEppVersion( const char * eppVersion )
{
	if( eppVersion == null )
	{
		version[0] = 0;
	}
	else if( ::strlen(eppVersion) == 0 )
	{
		version[0] = 0;
	}
	else if( eppVersion[0] == '-' )
	{
		(void) ::strncpy(version, eppVersion, MAX_VERSION_LENGTH - 1);
		EppUtil::version[MAX_VERSION_LENGTH - 1] = 0;
	}
	else
	{
		version[0] = '-';
		(void) ::strncpy(&version[1], eppVersion, MAX_VERSION_LENGTH - 2);
		EppUtil::version[MAX_VERSION_LENGTH - 1] = 0;
	}
}

const char * EppUtil::getEppVersion()
{
	if( version[0] == 0 )
	{
		return (const char *) &version[0];
	}
	else
	{
		return (const char *) &version[1];
	}
}

DOMElement* EppUtil::createElementNS( DOMDocument& doc, const DOMString ns, const DOMString tag )
{
	return EppUtil::createElementNS(doc, ns, tag, true);
}

DOMElement* EppUtil::createElementNS( DOMDocument& doc, const DOMString ns, const DOMString tag, bool flag , char *_version)
{
	DOMElement* elm;
	DOMString prefix(EPP_NAME_SPACE);
	char *use_version = version;

	if( NULL != _version ) /*param provided*/
		use_version = _version;

	if( tag.isNull() )
	{
		elm = doc.createElementNS(prefix + ns + use_version , ns);
		elm->setAttribute(XS("xmlns"), XS(EPP_NAME_SPACE) + ns + use_version );
		if( use_version[0] == 0 )
		{
			// old EPP-02 schema
			elm->setAttribute(XS("xmlns:xsi"), XS("http://www.w3.org/2000/10/XMLSchema-instance"));
		}
		else
		{
			elm->setAttribute(XS("xmlns:xsi"), XS("http://www.w3.org/2001/XMLSchema-instance"));
		}
	}
	else
	{
		elm = doc.createElementNS(prefix + ns + use_version, tag);
		elm->setPrefix(ns);
		if( flag )
		{
			elm->setAttribute(XS("xmlns"), prefix + ns + use_version);
		}
		elm->setAttribute(XS("xmlns:") + ns, prefix + ns + use_version);
	}
	elm->setAttribute(XS("xsi:schemaLocation"), prefix + ns + use_version + " " + ns + use_version + ".xsd");

	return elm;
}

DOMElement* EppUtil::createElementNSUnspec( DOMDocument& doc, const DOMString unspec, const DOMString tag, bool flag )
{
	unsigned int len = unspec.length();
	unsigned int index_ns;
	unsigned int index_uri;
	unsigned int index;

	DOMString ns;
	DOMString uri;
	DOMString loc;

	for( index = 0; index < len; index++ )
	{
		if( unspec.charAt(index) == ' ' )
		{
			ns = unspec.substringData(0, index);
			index_ns = index;
			break;
		}
	}
	for( index = index_ns + 1; index < len; index++ )
	{
		if( unspec.charAt(index) == ' ' )
		{
			uri = unspec.substringData(index_ns + 1, index - index_ns - 1);
			index_uri = index;
			break;
		}
	}
	if( index < len )
	{
		loc = unspec.substringData(index_uri + 1, len - index_uri - 1);
	}
	if( (ns.isNull()) || (uri.isNull()) || (loc.isNull()) )
	{
		return NULL;
		//return DOMElement();
	}

	loc = uri + " " + loc;

	DOMElement* elm;
	if( tag.isNull() )
	{
		elm = doc.createElementNS(uri, ns);
		elm->setAttribute(XS("xmlns"), uri);
		if( version[0] == 0 )
		{
			// old EPP-02 schema
			elm->setAttribute(XS("xmlns:xsi"), XS("http://www.w3.org/2000/10/XMLSchema-instance"));
		}
		else
		{
			elm->setAttribute(XS("xmlns:xsi"), XS("http://www.w3.org/2001/XMLSchema-instance"));
		}
	}
	else
	{
		elm = doc.createElementNS(uri, tag);
		elm->setPrefix(ns);
		if( flag )
		{
			elm->setAttribute(XS("xmlns"), uri);
		}
		elm->setAttribute(XS("xmlns:") + ns, uri);
	}
	elm->setAttribute(XS("xsi:schemaLocation"), loc);

	return elm;
}

DOMText* EppUtil::createTextNode( DOMDocument& doc, const time_t cal )
{
	return EppUtil::createTextNode(doc, cal, false);
}

DOMText* EppUtil::createTextNode( DOMDocument& doc, const time_t cal, bool dateOnly )
{
	char buf[128];
	struct tm tmp;

#if	   EPP_SUPPORT_UNSIGNED_TIME_T		/* { */

	long long t = cal;
	(void) gmtime64_r(&t, &tmp);

#else	/* EPP_SUPPORT_UNSIGNED_TIME_T	*/	/* } */

#if       defined(win32)      /* { */
	struct tm * p = gmtime(&cal);
	tmp = *p;
#else  /* defined(win32) */   /* } { */
	(void) gmtime_r(&cal, &tmp);
#endif /* defined(win32) */   /* } */

#endif	/* EPP_SUPPORT_UNSIGNED_TIME_T	*/	/* } */

	const char * fmt = dateOnly ? "%Y-%m-%d" : "%Y-%m-%dT%H:%M:%S.0Z";
	buf[0] = '\0';
	(void) strftime(buf, sizeof(buf), fmt, &tmp);

	return doc.createTextNode(XS(buf));
}

DOMString EppUtil::getText( const DOMNode &root )
{
	DOMNodeList* list = root.getChildNodes();
	if( list->getLength() == 0 )
	{
		return root.getNodeValue();
	}

	DOMString str = DOMString("");
	for( unsigned int i = 0; i < list->getLength(); i++ )
	{
		DOMNode* node = list->item(i);
		if( (node->getNodeType() == DOMNode::TEXT_NODE)  || (node->getNodeType() == DOMNode::CDATA_SECTION_NODE) )
		{
			DOMString val = node->getNodeValue();
			str += val;
		}
	}
	return str;
}

bool EppUtil::getTextAsBool( const DOMNode &root )
{
	DOMString str = getText(root);

	unsigned int length = str.length();
	if ( length == 1 && (str.charAt(0) == (XMLCh)'1') )
	{
		return true;
	}
	else if ( length == 4 )
	{
		XMLCh c;

		c = str.charAt(0);
		if ( c != (XMLCh)'t' && c != (XMLCh)'T' )
			return false;

		c = str.charAt(1);
		if ( c != (XMLCh)'r' && c != (XMLCh)'R' )
			return false;

		c = str.charAt(2);
		if ( c != (XMLCh)'u' && c != (XMLCh)'U' )
			return false;

		c = str.charAt(3);
		if ( c != (XMLCh)'e' && c != (XMLCh)'E' )
			return false;

		return true;
	}
	return false;
}

time_t EppUtil::getDate( const DOMNode& root )
{
	return EppUtil::getDate(root, false);
}

time_t EppUtil::getDate( const DOMNode& root, bool dateOnly )
{
	time_t cal = 0;

	DOMString s = EppUtil::getText(root);
	if( s.isNotNull() && s.length() > 0 )
	{
		char * buf = s.transcode();
		if( buf != null )
		{
			cal = EppUtil::getDate(buf, dateOnly);
			XercesString::Delete(buf);
		}
	}

	return cal;
}

time_t EppUtil::getDate( const char * s, bool dateOnly )
{
	time_t cal = 0;
	struct tm tmp;
#if       defined(win32)      /* { */
	const char * fmt = dateOnly ? "%4d-%2d-%2d" : "%4d-%2d-%2dT%2d:%2d:%2d.0Z";
#else  /* defined(win32) */   /* } { */
#if       defined(hpux)       /* { */
	const char * fmt = dateOnly ? "%Y-%m-%d" : "%Y-%m-%d %H:%M:%S.0Z";
#else  /* defined(hpux) */    /* } { */
	const char * fmt = dateOnly ? "%Y-%m-%d" : "%Y-%m-%dT%H:%M:%S.0Z";
#endif /* defined(hpux) */    /* } */
#endif /* defined(win32) */   /* } */

	if( (s != null) && (*s != '\0') )
	{
		char * buf = ::strdup(s);
		if( buf != null )
		{
#if       defined(hpux)       /* { */
			char * cp = buf;
			while( *cp != '\0' )
			{
				if( *cp == 'T' )
				{
					*cp = ' ';
					break;
				}
				cp++;
			}
#endif /* defined(hpux) */    /* } */

			(void) memset((char *) &tmp, '\0', sizeof(tmp));

#if       defined(win32)      /* { */

			char * c = "";
			if( dateOnly )
			{
				if( sscanf(buf, fmt, &tmp.tm_year, &tmp.tm_mon,
					&tmp.tm_mday) != 3 )
				{
					c = null;
				}
			}
			else
			{
				if( sscanf(buf, fmt, &tmp.tm_year, &tmp.tm_mon,
					&tmp.tm_mday, &tmp.tm_hour, &tmp.tm_min,
					&tmp.tm_sec) != 6 )
				{
					c = null;
				}
			}
			if( c != null )
			{
				tmp.tm_year -= 1900;
				tmp.tm_mon--;
			}

#else  /* defined(win32) */   /* } { */
			if ( dateOnly )
			{
				strptime(buf, fmt, &tmp);
			}
			else
			{
				// Parse the GMT offset if present.
				struct tm junk; // Values inside are not used. Defined this instead of defining many ints.
				char tStr[32];

				(void) memset(tStr, '\0', sizeof(tStr));
				(void) memset((char *) &junk, '\0', sizeof(junk));

				// Quick check on whats inside....
				if ( sscanf(buf, "%u-%u-%uT%u:%u:%s",
					&junk.tm_year, &junk.tm_mon, &junk.tm_mday, &junk.tm_hour, &junk.tm_min,
					tStr) )
				{
					const char * fmt2 = "%Y-%m-%dT%H:%M:%S%z";

					char* subPtr;
					if ( subPtr = strchr(tStr, 'Z') )
					{
						strptime(buf, fmt, &tmp);
					}
					else if ( NULL == strchr(tStr, ':') ) // timezone offset is something like +/-hhmm
					{
						strptime(buf, fmt2, &tmp);
					}
					else // timezone offset is something like +/-hh:mm
					{
						std::string input(buf);
						std::string subString(tStr);

						for ( int index = input.find(subString);
							index <input.length();
							index++ )
						{
							if ( input[index]==':' )
							{
								input.erase(index, 1);
							}
						}
						strptime(input.c_str(), fmt2, &tmp);
					}
				}
			}
#endif /* defined(win32) */   /* } */

#if	   EPP_SUPPORT_UNSIGNED_TIME_T		/* { */

			cal = mktime64(&tmp) - timezone + daylight * 3600;

#else	/* EPP_SUPPORT_UNSIGNED_TIME_T	*/	/* } */

			//			if( strptime(buf, fmt, &tmp) == null )
			//			if( (c == null) || (*c == '\0') )
			{
				cal = mktime(&tmp) - timezone + daylight * 3600;
			}

#endif	/* EPP_SUPPORT_UNSIGNED_TIME_T	*/	/* } */

			(void) ::free(buf);
		}
	}

	return cal;
}

DOMString EppUtil::toString( const DOMDocument &doc )
{
	EppWriter out = EppWriter();
	out.print(doc);
	return out.getOutput();
}

void EppUtil::msSleep(unsigned int milliseconds)
{
#if       defined(WIN32)       /* { */

	::Sleep(milliseconds);

#else  /* defined(WIN32) */    /* } { */

	struct timespec req, rem;

	int seconds = milliseconds / 1000;
	milliseconds -= seconds * 1000;

	req.tv_sec = seconds;
	req.tv_nsec = milliseconds * 1000;

	::nanosleep(&req, &rem);
#endif /* defined(WIN32) */    /* } */
}

short
EppUtil::getDnsSecMajorMinor(const DOMNode &root,
	unsigned short &_major, unsigned short &_minor)
{
	DOMString value = ((DOMElement*)&root)->getAttribute(XS("xmlns:secDNS"));
	if( value.isNull() )
	{
		return -1;
	}
	EppUtil::getDnsSecMajorMinor( value, _major, _minor );
	return 0;
}

short
EppUtil::getDnsSecMajorMinor(const DOMString &root,
	unsigned short &_major, unsigned short &_minor)
{
	char major[4] = "\0";
	char minor[4] = "\0";
	int8_t iter = 0;

	if( 0 >= root.length() || root.isNull() )
		return -1;

	char *str_val = root.transcode();
	if( null == str_val )
		return -1;

	char *needle = strrchr(str_val, '-');
	if( NULL == needle )
	{
		delete []str_val;
	}
	needle++;/*we already have '-' in it */
	char *part = major;
	while( *needle != '\0' )
	{
		if( *needle == '.' )
		{
			needle++;
			part = minor;
			iter = 0;
			continue;
		}
		if( iter > 2 )
		{
			iter = -1;
			break;
		}
		part[iter] = *needle;
		iter++;
		needle++;
	}
	XercesString::Delete(str_val);
	if( iter != -1 )
	{
		_major = (unsigned short)atoi(major);
		_minor = (unsigned short)atoi(minor);
		return 0;
	}
	return -1;
}

int EppUtil::isNumeric(char *str)
{
	if( strlen(str) == 0 )
	{
		return 0;
	}
	while( *str )
	{
		if( !isdigit(*str) )
			return 0;
		str++;
	}

	return 1;
}

DOMString EppUtil::substring(char sep , const DOMString & src)
{
	DOMString name;
	if ( src.isNull() ) return name;
	name = src;
	char * nameStr = name.transcode();
	if ( nameStr == null ) return name;
	int totalLen = name.length();
	char * indexPtr = strchr(nameStr, sep);
	if ( indexPtr != null )
	{
		int len = indexPtr - nameStr;
		name = name.substringData(len+1, totalLen);
	}
	XercesString::Delete(nameStr);
	return name;
}

bool EppUtil::getStartEndTags(const char*     input,
	int             inputLen,
	const string&   searchString,
	string&         startTag,
	string&         endTag,
	bool            spaceAtEnd)
{

	if ( inputLen <= 0 )
	{
		return false;
	}

	// Search for searchString
	const char *start = strstr(input, searchString.c_str());

	if ( NULL == start )
	{
		// search string is not present
		return false;
	}

	// Calculate the start index of search string in input string.
	int startindex = start - input;

	// Now move back char by char to see opening angle 
	// limit to available number of chars.
	int loopIndex = 0;
	bool found    = false;

	for ( loopIndex = startindex - 1; loopIndex >= 0; loopIndex-- )
	{
		// Adjust the start pointer to recent start
		start--;
		if ( '<' == input[loopIndex] )
		{
			found = true;
			break;
		}
	}

	if ( false == found )
	{
		return false;
	}

	startTag.insert(0, start, startindex - loopIndex);
	startTag.append(searchString);

	endTag = "</";
	endTag.insert(endTag.size(), ++start, startindex - loopIndex - 1);
	endTag.append(searchString);
	if ( spaceAtEnd )
	{
		// Replace the space with a ">"
		endTag.replace(endTag.size() - 1, 1, ">");
	}
	else
	{
		endTag.append(">");
	}

	return true;
}

