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

#if ! defined(EPPUTIL_HPP)    /* { */
#define       EPPUTIL_HPP        1

#include <time.h>
#include <xercesc/dom/DOM.hpp>
#include <iostream>

#include <cstring>
#include <cstdio>
#include <cstdlib>

#define MAX_VERSION_LENGTH	20

class EPP_EXPORT EppUtil
{
private:
	static char version[MAX_VERSION_LENGTH];

public:
	/**
	 * Sets the EPP XSD file version. The default value is "1.0",
	 * for EPPSchema datedon June 29, 2001.
	 */
	static void setEppVersion( const char * eppVersion );

	/**
	 * Gets the EPP XSD file version.
	 *
	 * @note value returned from this method is pointing to an internal
	 *       buffer, so there is no need to free it
	 */
	static const char * getEppVersion();

	/**
	 * Creates an <code>DOMElementNSImpl</code> object inside a
	 * <code>DOMElement</code> object. The namespace associated with the
	 * object is specific to EPP.
	 *
	 * @param doc <code>DOMElement</code> object in which the new element
	 *            to be created.
	 * @param ns  The namespace of the new element to be created.
	 * @param tag The tag name of the new element to be created.
	 *
	 * @return an <code>DOMElementNSImpl</code> object
	 */
	static DOMElement* createElementNS( DOMDocument& doc, const DOMString ns, const DOMString tag );

	/**
	 * Creates an <code>DOMElementNSImpl</code> object inside a
	 * <code>DOMElement</code> object. The namespace associated with the
	 * object is specific to EPP.
	 *
	 * @param doc <code>DOMDocument</code> object in which the new element to be
	 *            created.
	 * @param ns  The namespace of the new element to be created.
	 * @param tag The tag name of the new element to be created.
	 * @param flag The flag indicating if the default namespace is needed.
	 *
	 * @return an <code>DOMElementNSImpl</code> object
	 */
	static DOMElement* createElementNS( DOMDocument& doc, const DOMString ns, const DOMString tag, bool flag, char *_version = NULL);

	/**
	 * Creates an <code>DOMElementNSImpl</code> object inside a
	 * <code>Document</code> object. The namespace associated with the
	 * object is specific to an <I>unspec</I> object in an
	 * <code>EppServiceMenu</code> object.
	 *
	 * @param doc <code>DOMDocument</code> object in which the new element to be
	 *            created.
	 * @param unspec The string containing the information about the
	 *        <I>unspec</I> object with three tokens: name, uri, schema. For
	 *        example: "obj1ext http://custom/obj1ext-1.0 obj1ext-1.0.xsd"
	 * @param tag The tag name of the new element to be created.
	 * @param flag The flag indicating if the default namespace is needed.
	 *
	 * @return an <code>DOMElement</code> object, or null if the
	 *         <code>unspec</code> parameter is not valid.
	 */
	static DOMElement* createElementNSUnspec( DOMDocument& doc, const DOMString unspec, const DOMString tag, bool flag );

	/**
	 * Creates a <code>DOMText</code> node for converting a
	 * <code>time_t</code> value into a string of XML timeInstant type.
	 *
	 * @param doc <code>DOMElement</code> object in which the new element
	 *            to be created.
	 * @param cal The <code>time_t</code> to be converted into a XML
	 *            <code>DOMText</code> node.
	 *
	 * @note  the <code>time_t</code> value should be GMT.
	 */
	static DOMText* createTextNode( DOMDocument& doc, const time_t cal );

	/**
	 * Creates a <code>DOMText</code> node for converting a
	 * <code>time_t</code> value into a string of XML timeInstant
	 * type or XML date type.
	 *
	 * @param doc <code>DOMElement</code> object in which the new element
	 *            to be created.
	 * @param cal The <code>time_t</code> value to be converted into a XML
	 *            <code>Text</code> node.
	 * @param dateOnly true if the format is XML date type, or false
	 *                 if the format is XML timeInstant type
	 *
	 * @note  the <code>time_t</code> value should be GMT.
	 */
	static DOMText* createTextNode( DOMDocument& doc, const time_t cal, bool dateOnly );

	/**
	 * Gets the text string associated with a node
	 *
	 * @param root <code>Node</code> object from which text to be extracted.
	 *
	 * @return a string associated with the node
	 */
	static DOMString getText( const DOMNode &root );

	/**
	 * Gets the text string associated with a node as a <code>bool</code> value
	 *
	 * @param root <code>Node</code> object from which text to be extracted.
	 *
	 * @return a string associated with the node
	 */
	static bool getTextAsBool( const DOMNode &root );

	/**
	 * Converts a text string associated with a node into a
	 * <code>time_t</code> value in GMT
	 *
	 * @param root <code>Node</code> object from which text to be extracted.
	 *
	 * @return a <code>time_t</code> value, or 0 if the text string is
	 *         not a valid XML timeInstant string
	 */
	static time_t getDate( const DOMNode& root );

	/**
	 * Converts a text string associated with a node into a
	 * <code>time_t</code> value in GMT
	 *
	 * @param root <code>Node</code> object from which text to be extracted.
	 * @param dateOnly true for parsing the string in XML date format, or
	 *                 false for parsing the string in XML timeInstant
	 *                 format
	 *
	 * @return a <code>time_t</code> value, or 0 if the text string is
	 *         not a valid XML timeInstant/date string
	 */
	static time_t getDate( const DOMNode& root, bool dateOnly );

	/**
	 * Converts a string into a <code>time_t</code> value in GMT
	 *
	 * @param buf C string containing the date representation
	 * @param dateOnly true for parsing the string in XML date format, or
	 *                 false for parsing the string in XML timeInstant
	 *                 format
	 *
	 * @return a <code>time_t</code> value, or 0 if the string is
	 *         not a valid XML timeInstant/date string
	 */
	static time_t getDate( const char *buf, bool dateOnly );

	/**
	 * Converts a <code>DOMElement</code> into a formated XML string.
	 *
	 * @param doc <code>DOMElement</code> to be converted into a string.
	 *
	 * @return a string representing the formated XML document
	 */
	static DOMString toString( const DOMDocument &doc );

	/**
	 * Sleeps for the specified time period in milliseconds.
	 *
	 * @param milliseconds number of milliseconds to sleep for.
	 */
	static void msSleep(unsigned int milliseconds);

	/**
	 * Get SecDNS extension from STRING or DOMNode
	 *
	 **/
	static short getDnsSecMajorMinor(const DOMNode &root, unsigned short &_major, unsigned short &_minor);
	static short getDnsSecMajorMinor(const DOMString &root, unsigned short &_major, unsigned short &_minor);

	/**
	 * Check if a string is a unsigned integer or long
	 */

	static int isNumeric(char * str);

	static DOMString substring(char sep, const DOMString & src);

	/**
	 * Searches the search string and return start and end tags which include < and >
	 * In some searches sometime it is required to pass a space at end in the search string
	 * but it is not required to be included in endTag. If that the case pass spaceAtEnd
	 * as true.
	 **/
	static bool getStartEndTags(const char*		input,
		int			inputLen,
		const std::string&	searchString,
		std::string&	startTag,
		std::string&	endTag,
		bool		spaceAtEnd);

};

#endif     /* EPPUTIL_HPP */  /* } */
