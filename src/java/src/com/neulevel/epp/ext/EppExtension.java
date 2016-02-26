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
package com.neulevel.epp.ext;

import org.w3c.dom.Node;

import com.neulevel.cn.CNCDNExt;
import com.neulevel.cn.CNContactCreateExt;
import com.neulevel.cn.CNContactInfoExt;
import com.neulevel.cn.CNContactUpdateExt;
import com.neulevel.epp.core.EppEntity;
import com.neulevel.epp.core.EppUtil;
import com.neulevel.epp.ext.command.EppCommandCheckLaunchRegistration;
import com.neulevel.epp.ext.command.EppCommandCreateLaunchRegistration;
import com.neulevel.epp.ext.command.EppCommandCreateSecDns;
import com.neulevel.epp.ext.command.EppCommandDeleteLaunchRegistration;
import com.neulevel.epp.ext.command.EppCommandInfoLaunchRegistration;
import com.neulevel.epp.ext.command.EppCommandUpdateLaunchRegistration;
import com.neulevel.epp.ext.command.EppCommandUpdateSecDns;
import com.neulevel.epp.ext.fee.command.EppCommandCheckFee;
import com.neulevel.epp.ext.fee.command.EppCommandCreateFee;
import com.neulevel.epp.ext.fee.command.EppCommandInfoFee;
import com.neulevel.epp.ext.fee.command.EppCommandRenewFee;
import com.neulevel.epp.ext.fee.command.EppCommandTransferFee;
import com.neulevel.epp.ext.fee.response.EppResponseCheckFee;
import com.neulevel.epp.ext.fee.response.EppResponseCreateFee;
import com.neulevel.epp.ext.fee.response.EppResponseInfoFee;
import com.neulevel.epp.ext.fee.response.EppResponseRenewFee;
import com.neulevel.epp.ext.fee.response.EppResponseTransferFee;
import com.neulevel.epp.ext.response.EppResponseDataCheckClaims;
import com.neulevel.epp.ext.response.EppResponseDataCreateLaunchRegistration;
import com.neulevel.epp.ext.response.EppResponseDataInfoLaunchRegistration;
import com.neulevel.epp.ext.response.EppResponseDataInfoSecDns;

/**
 * This <code>EppExtension</code> class implements EPP Extension entity.
 * 
 * @author Ning Zhang ning.zhang@neustar.com
 * @version $Revision: 1.9 $ $Date: 2013/07/08 21:36:29 $
 */
abstract public class EppExtension extends EppEntity
{
	/**
	 * Converts an XML element into an <code>EppExtension</code> object. The
	 * caller of this method must make sure that the root node is a child node
	 * of an EPP extension tag
	 * 
	 * @param root
	 *            root node for an <code>EppExtension</code> object in XML
	 *            format
	 * 
	 * @return an <code>EppEntity</code> object, or null if the node is invalid
	 */
	public static EppEntity fromXML(Node root)
	{
		if (root == null)
		{
			return null;
		}

		String prefix = root.getPrefix();
		String name = root.getLocalName();

		if ((prefix == null) || (name == null))
		{
			return null;
		}

		EppExtension entity = null;

		if (prefix.equals("secDNS"))
		{
			if (name.equals("create"))
			{
				entity = (EppExtension) EppCommandCreateSecDns.fromXML(root);
				if (null != entity)
				{
					String maj_min[] = EppUtil.getDnsSecMajorMinor(root);
					if (null != maj_min)
					{
						entity.secDnsMajor = maj_min[0];
						entity.secDnsMinor = maj_min[1];
					}
				}
			} else if (name.equals("update"))
			{
				entity = (EppExtension) EppCommandUpdateSecDns.fromXML(root);
				if (null != entity)
				{
					String maj_min[] = EppUtil.getDnsSecMajorMinor(root);
					if (null != maj_min)
					{
						entity.secDnsMajor = maj_min[0];
						entity.secDnsMinor = maj_min[1];
					}
				}
			} else if (name.equals("infData"))
			{
				entity = (EppExtension) EppResponseDataInfoSecDns.fromXML(root);
				if (null != entity)
				{
					String maj_min[] = EppUtil.getDnsSecMajorMinor(root);
					if (null != maj_min)
					{
						entity.secDnsMajor = maj_min[0];
						entity.secDnsMinor = maj_min[1];
					}
				}
			}
		} else if (prefix.equals("launch"))
		{
			if (name.equals("check"))
			{
				entity = (EppExtension) EppCommandCheckLaunchRegistration
						.fromXML(root);
			} else if (name.equals("create"))
			{
				entity = (EppExtension) EppCommandCreateLaunchRegistration
						.fromXML(root);
			} else if (name.equals("update"))
			{
				entity = (EppExtension) EppCommandUpdateLaunchRegistration
						.fromXML(root);
			} else if (name.equals("delete"))
			{
				entity = (EppExtension) EppCommandDeleteLaunchRegistration
						.fromXML(root);
			} else if (name.equals("info"))
			{
				entity = (EppExtension) EppCommandInfoLaunchRegistration
						.fromXML(root);
			} else if (name.equals("infData"))
			{
				entity = (EppExtension) EppResponseDataInfoLaunchRegistration
						.fromXML(root);
			} else if (name.equals("creData"))
			{
				entity = (EppExtension) EppResponseDataCreateLaunchRegistration
						.fromXML(root);
			} else if (name.equals("chkData"))
			{
				entity = (EppExtension) EppResponseDataCheckClaims
						.fromXML(root);
			}
		} else if (prefix.equals("idn"))
		{
			if (name.equals("data"))
			{
				entity = (EppExtension) EppIDNData.fromXML(root);
			}
		} else if (prefix.equals("allocationToken"))
		{
			if (name.equals("allocationToken"))
			{
				entity = (EppExtension) EppAllocationToken.fromXML(root);
			}
		} else if (prefix.equals("fee"))
		{
			if (name.equals("info")) {
				entity = (EppExtension) EppCommandInfoFee.fromXML(root);
			} else if (name.equals("infData")) {
				entity = (EppExtension) EppResponseInfoFee.fromXML(root);
			} else if (name.equals("check")) {
				entity = (EppExtension) EppCommandCheckFee.fromXML(root);
			} else if (name.equals("chkData")){
				entity = (EppExtension) EppResponseCheckFee.fromXML(root);
			} else if (name.equals("create")) {
				entity = (EppExtension) EppCommandCreateFee.fromXML(root);
			} else if (name.equals("creData")) {
				entity = (EppExtension) EppResponseCreateFee.fromXML(root);
			} else if (name.equals("transfer")) {
				entity = (EppExtension) EppCommandTransferFee.fromXML(root);
			} else if (name.equals("trnData")) {
				entity = (EppExtension) EppResponseTransferFee.fromXML(root);
			} else if (name.equals("renew")) {
				entity = (EppExtension) EppCommandRenewFee.fromXML(root);
			} else if (name.equals("renData")) {
				entity = (EppExtension) EppResponseRenewFee.fromXML(root);
			}
		} else if (prefix.equals("changePoll") && name.equals("changeData") ) 
		{
			entity = (EppExtension) EppChangePollExt.fromXML(root);
			
		} else if (prefix.equals("cnnic-contact") && name.equals("create") ) 
		{
			entity = (EppExtension) CNContactCreateExt.fromXML(root);
			
		} else if (prefix.equals("cnnic-contact") && name.equals("infData") ) 
		{
			entity = (EppExtension) CNContactInfoExt.fromXML(root);
			
		} else if (prefix.equals("cnnic-contact") && name.equals("update") ) 
		{
			entity = (EppExtension) CNContactUpdateExt.fromXML(root);
			
		} else if (prefix.equals("cdn") && name.equals("infData") ) 
		{
			entity = (EppExtension) CNCDNExt.fromXML(root);
			
		}
		return (EppEntity) entity;
	}

	protected String secDnsMajor = "1";
	protected String secDnsMinor = "1";
}
