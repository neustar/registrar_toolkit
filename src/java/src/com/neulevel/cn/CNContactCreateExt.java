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

import com.neulevel.epp.core.EppUtil;
import com.neulevel.epp.ext.EppExtension;

/**
 * 
 * @author ankit.kothana
 *
 */

public class CNContactCreateExt extends EppExtension {
	
	private CNContact cnContact;

	/**
	 * @return the cnContact
	 */
	public CNContact getCnContact() {
		return cnContact;
	}

	/**
	 * @param cnContact the cnContact to set
	 */
	public void setCnContact(CNContact cnContact) {
		this.cnContact = cnContact;
	}
	
	public static EppExtension fromXML(final Node root)
	{
		CNContactCreateExt cnContactCreateExt = new CNContactCreateExt();
		CNContact cnContact = CNContact.fromXML(root);
		if( cnContact != null )
		{
			cnContactCreateExt.cnContact = cnContact;
		}
		return cnContactCreateExt;
	}

	@Override
	public Element toXML(Document doc, String tag) {
		Element body = EppUtil.createElementNS(doc, "cnnic-contact", (tag == null) ? "create" : tag,true,"-1.0");
        if (this.cnContact != null) {
            cnContact.toXML(doc, body);
        }
        return body;
	}

	
	@Override
	public String toString() {
		return "CNContactCreateExt [cnContact=" + cnContact + "]";
	}

	

}