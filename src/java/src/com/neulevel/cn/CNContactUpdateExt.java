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
package com.neulevel.cn;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.neulevel.epp.core.EppUtil;
import com.neulevel.epp.ext.EppExtension;

/**
 * 
 * @author ankit.kothana
 *
 */

public class CNContactUpdateExt extends EppExtension {
	
	private CNContact addContact;
	private CNContact remContact;
	private CNContact chgContact;

	/**
	 * @return the addContact
	 */
	public CNContact getAddContact() {
		return addContact;
	}

	/**
	 * @param cnContact the cnContact to set
	 */
	public void setAddContact(CNContact cnContact) {
		this.addContact = cnContact;
	}
	
	/**
	 * @return the remContact
	 */
	public CNContact getRemContact() {
		return remContact;
	}

	/**
	 * @param cnContactRemove the cnContactRemove to set
	 */
	public void setRemContact(CNContact cnContactRemove) {
		this.remContact = cnContactRemove;
	}

	/**
	 * @return the chgContact
	 */
	public CNContact getChgContact() {
		return chgContact;
	}

	/**
	 * @param cnContactChange the cnContactChange to set
	 */
	public void setChgContact(CNContact cnContactChange) {
		this.chgContact = cnContactChange;
	}

	public static EppExtension fromXML(final Node root)
	{
		CNContactUpdateExt cnContactUpdateExt = new CNContactUpdateExt();
		NodeList nodeList = root.getChildNodes();
		
		for (int i=0; i< nodeList.getLength(); i++) {
			Node node = nodeList.item(i);
			String name = node.getLocalName();
			if (name == null) {
				name = node.getNodeName();
			}
			
			if (name != null) {
				if (name.equals("add")) {
					cnContactUpdateExt.addContact = CNContact.fromXML(node);
					continue;
				}
				
				if (name.equals("rem")) {
					cnContactUpdateExt.remContact = CNContact.fromXML(node);
					continue;
				}
				
				if (name.equals("chg")) {
					cnContactUpdateExt.chgContact = CNContact.fromXML(node);
					continue;
				}
				
			} else {
				continue;
			}
			
		}
		
		return cnContactUpdateExt;
	}

	@Override
	public Element toXML(Document doc, String tag) {
		Element body = EppUtil.createElementNS(doc, "cnnic-contact", (tag == null) ? "update" : tag,true,"-1.0");
        if (this.addContact != null) {
        	Element elem = doc.createElement("add");
            addContact.toXML(doc, elem);
            body.appendChild(elem);
        }
        
        if (this.remContact != null) {
        	Element elem = doc.createElement("rem");
        	remContact.toXML(doc, elem);
            body.appendChild(elem);
        }
        
        if (this.chgContact != null) {
        	Element elem = doc.createElement("chg");
        	chgContact.toXML(doc, elem);
            body.appendChild(elem);
        }
        
        return body;
	}

	@Override
	public String toString() {
		return "CNContactUpdateExt [cnContactAdd=" + addContact
				+ ", cnContactRemove=" + remContact + ", cnContactChange="
				+ chgContact + "]";
	}

	
}
