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
package com.neulevel.epp.core.response;

import java.util.*;
import com.neulevel.epp.core.*;
import org.w3c.dom.*;
import org.apache.xerces.dom.*;

/**
 * This <code>EppResponseDataPendingContact</code> class implements EPP
 * Response Data entity for EPP Pending Actions of EPP Contact objects.
 *
 * @author Ning Zhang ning.zhang@neustar.com
 * @version $Revision: 1.2 $ $Date: 2006/03/01 01:35:38 $
 */
public class EppResponseDataPendingContact extends EppResponseDataPending
{
	private String   id;

	/**
	 * Creates an <code>EppResponseDataPendingContact</code> object
	 *
	 * @param id the id of the <code>EppContact</code> object associated with the pending action
	 */
	public EppResponseDataPendingContact( String id )
	{
		this.id = id;
		this.paResult = false;
		this.paTRID = null;
		this.paDate = null;
	}

	/**
	 * Creates an <code>EppResponseDataPendingContact</code> object
	 *
	 * @param id the id of the <code>EppContact</code> object associated with the pending action
	 * @param result the boolean flag indicating if the pending action is a success or a failure
	 */
	public EppResponseDataPendingContact( String id, boolean result )
	{
		this.id = id;
		this.paResult = result;
		this.paTRID = null;
		this.paDate = null;
	}

	/**
	 * Gets the id of the contact object associated with the pending action
	*/
	public String getId()
	{
		return this.id;
	}

	/**
	 * Sets the id of the contact object associated with the pending action
	 */
	public void setId( String id )
	{
		this.id = id;
	}

	/**
	 * Converts an XML element into an
	 * <code>EppResponseDataPendingContact</code> object. The caller of this
	 * method must make sure that the root node is the resData element of
	 * EPP responseType for creating an EPP Contact object
	 *
	 * @param root root node for an <code>EppResponseDataPendingContact</code>
	 *             object in XML format
	 *
	 * @return an <code>EppResponseDataPendingContact</code> object,
	 *         or null if the node is invalid
	 */
	public static EppEntity fromXML( Node root )
	{
		EppResponseDataPendingContact res = null;
		NodeList list = root.getChildNodes();
		for( int i = 0; i < list.getLength(); i++ )
		{
			Node node = list.item(i);
			String name = node.getLocalName();
			if( name == null )
			{
				continue;
			}
			else if( name.equals("id") )
			{
				String id = EppUtil.getText(node);
				if( res == null )
				{
					res = new EppResponseDataPendingContact(id);
					String cd = ((Element) node).getAttribute("paResult");
					if(    (cd != null)
					    && (    cd.equals("0")
						 || cd.equalsIgnoreCase("f")
						 || cd.equalsIgnoreCase("false") ) )
					{
						res.setResult(false);
					}
					else
					{
						res.setResult(true);
					}
				}
			}
			else if( res != null )
			{
				res.fromXMLCommon(node, name);
			}
		}

		return res;
	}

	/**
	 * Converts an <code>EppResponseDataPendingContact</code> object
	 * into an XML element.
	 *
	 * @param doc the XML <code>Document</code> object
	 * @param tag the tag/element name for the
	 *        <code>EppResponseDataPendingContact</code> object
	 *
	 * @return an <code>Element</code> object
	 */
	public Element toXML( Document doc, String tag )
	{
		Element elm;
		Element body = doc.createElement(tag);
		ElementNSImpl data = EppUtil.createElementNS(doc, "contact", "panData");
		body.appendChild(data);

		if( id != null )
		{
			elm = doc.createElement("id");
			elm.appendChild(doc.createTextNode(id));
			if( paResult == true )
			{
				elm.setAttribute("paResult", "1");
			}
			else
			{
				elm.setAttribute("paResult", "0");
			}
			data.appendChild(elm);
		}

		toXMLCommon(doc, data);

		return body;
	}
}
