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
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.neulevel.epp.core.EppUtil;

/**
 * 
 * @author ankit.kothana
 *
 */

public class CNContact {
	private String contactType; //E or I
	private String contactID;
	private String contactIDType;
	private String mobile;
	private String purveyor;
	
	/**
	 * @return the contactType
	 */
	public String getContactType() {
		return contactType;
	}
	/**
	 * @param contactType the contactType to set
	 */
	public void setContactType(String contactType) {
		this.contactType = contactType;
	}
	/**
	 * @return the contactID
	 */
	public String getContactID() {
		return contactID;
	}
	/**
	 * @param contactID the contactID to set
	 */
	public void setContactID(String contactID) {
		this.contactID = contactID;
	}
	/**
	 * @return the contactIDType
	 */
	public String getContactIDType() {
		return contactIDType;
	}
	/**
	 * @param contactIDType the contactIDType to set
	 */
	public void setContactIDType(String contactIDType) {
		this.contactIDType = contactIDType;
	}
	/**
	 * @return the mobile
	 */
	public String getMobile() {
		return mobile;
	}
	/**
	 * @param mobile the mobile to set
	 */
	public void setMobile(String mobile) {
		this.mobile = mobile;
	}
	/**
	 * @return the purveyor
	 */
	public String getPurveyor() {
		return purveyor;
	}
	/**
	 * @param purveyor the purveyor to set
	 */
	public void setPurveyor(String purveyor) {
		this.purveyor = purveyor;
	}
	
	public static CNContact fromXML(Node root) {
		CNContact cnContact = new CNContact();
		NodeList nodeList = root.getChildNodes();
		
		for (int i=0; i< nodeList.getLength(); i++) {
			Node node = nodeList.item(i);
			String name = node.getLocalName();
			if (name == null) {
				name = node.getNodeName();
			}
			
			if (name != null) {
				if (name.equals("type")) {
					cnContact.contactType = EppUtil.getText(node);
					continue;
				}
				
				if (name.equals("contact")) {
					cnContact.contactID = EppUtil.getText(node);
					NamedNodeMap attrs = node.getAttributes();
					for(int j = 0; j< attrs.getLength() ; j++) {
						Node attr = attrs.item(j);
						String attrValue = attr.getNodeValue();
						if( attr.getNodeName().equals("type") ) {
							cnContact.contactIDType = attrValue;
							continue;
						} 
						
					}
					continue;
				}
				
				if (name.equals("mobile")) {
					cnContact.mobile = EppUtil.getText(node);
					continue;
				}
				
				if (name.equals("purveyor")) {
					cnContact.purveyor = EppUtil.getText(node);
					continue;
				}
				
			} else {
				continue;
			}
			
		}
		
		return cnContact;
	}
	
	public void toXML(Document doc, Element inElement) {
        if (contactType != null && contactType.length() > 0) {
        	Element elem = doc.createElement("type");
        	elem.appendChild(doc.createTextNode(this.contactType));
        	inElement.appendChild(elem);
        }
        
        if (contactID != null && contactID.length() > 0) {
        	Element elem = doc.createElement("contact");
        	elem.appendChild(doc.createTextNode(this.contactID));
        	if (this.contactIDType != null && this.contactIDType.length() > 0) {
        		elem.setAttribute("type", this.contactIDType);
        	}
        	inElement.appendChild(elem);
        }
        
        if (purveyor != null && purveyor.length() > 0) {
        	Element elem = doc.createElement("purveyor");
        	elem.appendChild(doc.createTextNode(this.purveyor));
        	inElement.appendChild(elem);
        }
        
        if (mobile != null && mobile.length() > 0) {
        	Element elem = doc.createElement("mobile");
        	elem.appendChild(doc.createTextNode(this.mobile));
        	inElement.appendChild(elem);
        }
        
    }
}
