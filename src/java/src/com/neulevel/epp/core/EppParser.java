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
package com.neulevel.epp.core;

import java.io.*;
import java.util.*;
import org.w3c.dom.*;
import org.xml.sax.*;
import org.apache.xerces.parsers.*;
import com.neulevel.epp.core.response.*;

/**
 * This <code>EppParser</code> class is designed to parse and validate
 * EPP XML messages against EPP XML Schema. Apache Xerces 1.4.2 or later for Java
 * is required.
 *
 * @author Ning Zhang ning.zhang@neustar.com
 * @version $Revision: 1.10 $ $Date: 2013/06/17 15:55:21 $
 */
public class EppParser implements ErrorHandler
{
	private int       countWarnings = 0;
	private int       countErrors = 0;
	private int       countFatals = 0;
	private Vector    errorMessages = null;
	private Document  document = null;
	private DOMParser parser;

	private void init()
	{
		this.countWarnings = 0;
		this.countErrors   = 0;
		this.countFatals   = 0;
		if( this.errorMessages != null )
		{
			this.errorMessages.clear();
		}
		this.document      = null;
	}

	/**
	 * Creates an <code>EppParser</code> object
	 */
	public EppParser()
	{
		this(null);
	}

	public EppParser( String argv )
	{
		this(argv, null);
	}

	/**
	 * Creates an <code>EppParser</code> object, given an EPP XML payload
	 * string and a URI in which XSD files may be found.
	 */
	public EppParser( String argv, String xsdFilesLocation )
	{
		parser = new DOMParser();
		try
		{
			parser.setFeature("http://xml.org/sax/features/validation", true);
			parser.setFeature("http://xml.org/sax/features/namespaces", true);
			parser.setFeature("http://apache.org/xml/features/validation/schema", true);
			parser.setFeature("http://apache.org/xml/features/dom/defer-node-expansion", true);
			parser.setFeature("http://apache.org/xml/features/dom/include-ignorable-whitespace", false);

			if (xsdFilesLocation != null) {
				// construct pairs of namespaceURI-location values
			    // TAKE CARE TO FOLLOW THE "<targetnamespace> <file:///xsd-location> <targetnamespace> <file:///xsd-location> <targetnamespace> <file:///xsd-location>"
				StringBuffer sl = new StringBuffer();
				sl.append(EppUtil.NAMESPACEEPP09).append("epp-1.0 ").append(xsdFilesLocation).append("/epp-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("domain-1.0 ").append(xsdFilesLocation).append("/domain-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("contact-1.0 ").append(xsdFilesLocation).append("/contact-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("eppcom-1.0 ").append(xsdFilesLocation).append("/eppcom-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("host-1.0 ").append(xsdFilesLocation).append("/host-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("neulevel-1.0 ").append(xsdFilesLocation).append("/neulevel-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("neustar-1.0 ").append(xsdFilesLocation).append("/neustar-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("secDNS-1.0 ").append(xsdFilesLocation).append("/secDNS-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("secDNS-1.1 ").append(xsdFilesLocation).append("/secDNS-1.1.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("svcsub-1.0 ").append(xsdFilesLocation).append("/svcsub-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("xriAU-1.0 ").append(xsdFilesLocation).append("/xriAU-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("xriCommon-1.0 ").append(xsdFilesLocation).append("/xriCommon-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("xriINA-1.0 ").append(xsdFilesLocation).append("/xriINA-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("xriINU-1.0 ").append(xsdFilesLocation).append("/xriINU-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("xriISV-1.0 ").append(xsdFilesLocation).append("/xriISV-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("xriCS-1.0 ").append(xsdFilesLocation).append("/xriCS-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("signedMark-1.0 ").append(xsdFilesLocation).append("/signedMark-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("mark-1.0 ").append(xsdFilesLocation).append("/mark-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("launch-1.0 ").append(xsdFilesLocation).append("/launch-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("xmldsig-core-schema ").append(xsdFilesLocation).append("/xmldsig-core-schema.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("idn-1.0 ").append(xsdFilesLocation).append("/idn-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("allocationToken-1.0 ").append(xsdFilesLocation).append("/allocationToken-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("fee-0.6 ").append(xsdFilesLocation).append("/fee-0.6.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("changePoll-1.0 ").append(xsdFilesLocation).append("/changePoll-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("cnnic-contact-1.0 ").append(xsdFilesLocation).append("/cnnic-contact-1.0.xsd ");
				sl.append(EppUtil.NAMESPACEEPP09).append("cdn-1.0 ").append(xsdFilesLocation).append("/cdn-1.0.xsd ");
				parser.setProperty("http://apache.org/xml/properties/schema/external-schemaLocation", sl.toString());
			}

			parser.setErrorHandler(this);
			if( argv != null )
			{
				parser.parse(new InputSource(new StringReader(argv)));
				document = parser.getDocument();
			}
		}
		catch( Exception ex )
		{
			this.countFatals++;
			this.addMessage("[Unknwon Error] " + ex.getMessage());
		}
	}

	/**
	 * Gets the DOM Parser used for parsing EPP messages
	 */
	public DOMParser getParser()
	{
		return this.parser;
	}

	/**
	 * Parses an EPP message string
	 */
	public void parse( String argv )
	{
		this.init();
		try
		{
			this.parser.reset();
			this.parser.parse(new InputSource(new StringReader(argv)));
			document = parser.getDocument();
		}
		catch( Exception ex )
		{
			this.countFatals++;
			this.addMessage("[Unknwon Error] " + ex.getMessage());
		}
	}

	/**
	 * Parses an EPP message stored in a file
	 */
	public void parseFile( String file )
	{
		this.init();
		try
		{
			this.parser.reset();
			this.parser.parse(file);
			document = parser.getDocument();
		}
		catch( Exception ex )
		{
			this.countFatals++;
			this.addMessage("[Unknwon Error] " + ex.getMessage());
		}
	}

	/**
	 * Gets <code>Document</code> object after parsing
	 */
	public Document getDocument()
	{
		return this.document;
	}

	/**
	 * Checks if there is any errors
	 */
	public boolean hasError()
	{
		return (this.countWarnings != 0) ||
		       (this.countErrors != 0) || (this.countFatals != 0);
	}

	/**
	 * Gets parsing result formatted as plain text.
	 */
	public String getErrorMessage()
	{
		StringBuffer sb = new StringBuffer();

		if( this.errorMessages != null )
		{
			for( int i = 0; i < this.errorMessages.size(); i++ )
			{
				sb.append((String) this.errorMessages.get(i));
				sb.append("\n");
			}
		}

		return sb.toString();
	}

	/**
	 * Gets parsing result as an <code>EppResult</code> object
	 */
	public EppResult getResult()
	{
		if( ! this.hasError() )
		{
			return new EppResult(EppError.CODE_NO_ERROR, EppError.TEXT_NO_ERROR);
		}

		EppResult result = new EppResult(EppError.CODE_COMMAND_SYNTAX_ERROR, EppError.TEXT_COMMAND_SYNTAX_ERROR);
		if( this.errorMessages != null )
		{
			for( int i = 0; i < this.errorMessages.size(); i++ )
			{
				result.addValue((String) this.errorMessages.get(i));
			}
		}

		return result;
	}

	/**
	 * Gets the root "epp" node of parsing result as an <code>Node</code>
	 * object. If there is any error, null is returned.
	 */
	public Node getRootNode()
	{
		if( this.hasError() )
		{
			return null;
		}
		Document doc = this.getDocument();
		NodeList list = doc.getElementsByTagName("epp");
		if( (list == null) || (list.getLength() == 0) )
		{
			return null;
		}
		return list.item(0);
	}

	//
	// ErrorHandler methods
	//

	//
	// Warning
	//
	public void warning( SAXParseException ex )
	{
		this.addMessage("[Warning] " + getLoc(ex) + ": " + ex.getMessage());
		this.countWarnings++;
	}

	//
	// Error
	//
	public void error( SAXParseException ex )
	{
		this.addMessage("[Error] " + getLoc(ex) + ": "+ ex.getMessage());
		this.countErrors++;
	}

	//
	// Fatal error
	//
	public void fatalError( SAXParseException ex ) throws SAXException
	{
		this.addMessage("[Fatal Error] " + getLoc(ex) + ": "+ ex.getMessage());
		this.countFatals++;
		throw ex;
	}

	/**
	 * Parses a <code>SAXParseException</code> message to find out the exact
	 * location of an error
	 */
	private String getLoc( SAXParseException ex )
	{
		StringBuffer buf = new StringBuffer();

		String id = ex.getSystemId();
		if( id != null )
		{
			int index = id.lastIndexOf('/');
			if( index != -1 )
			{
				id = id.substring(index + 1);
			}
			buf.append(id);
		}
		buf.append(':');
		buf.append(ex.getLineNumber());
		buf.append(':');
		buf.append(ex.getColumnNumber());

		return buf.toString();
	}

	/**
	 * Adds an error message
	 */
	private void addMessage( String message )
	{
		if( this.errorMessages == null )
		{
			this.errorMessages = new Vector();
		}
		this.errorMessages.addElement(message);
	}

	public String toString()
	{
		return "EppParser: warnings=" + this.countWarnings
			+ " errors=" + this.countErrors + " fatals=" + this.countFatals;
	}
}
