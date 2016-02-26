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
package com.junit;

import java.util.Calendar;

import junit.framework.Assert;

import org.junit.Test;
import org.w3c.dom.Node;

import com.neulevel.cn.CNContact;
import com.neulevel.cn.CNContactCreateExt;
import com.neulevel.epp.core.EppError;
import com.neulevel.epp.core.EppParser;
import com.neulevel.epp.core.EppTransactionId;
import com.neulevel.epp.core.response.EppResponse;
import com.neulevel.epp.core.response.EppResult;
import com.neulevel.epp.core.response.EppValueReason;

/**
 * 
 * @author Anshul kothari/s_anskot
 *
 */
public class EppResponseCNCreateContact 
{
	@Test
	public void test()
	{
        Node epp;
        EppResponse res;
        EppResponse newres;
        EppResult result;
        EppTransactionId xid;

        res = new EppResponse();
        result = new EppResult(EppError.CODE_NO_ERROR, EppError.TEXT_NO_ERROR);
        result.addValue("Value 1");
        result.addValue("Value 2");
        result.addValue("Value 3");
        result.addValue("Value 4");
        result.addValue(new EppValueReason("Value 5", "Reason 5","en"));
        result.addValue("Value 6");
        result.addValue(new EppValueReason("Value 7", "Reason 7","en"));
        res.addResult(result);
        xid = new EppTransactionId("ABC-DEF-12345", "SERVER-ID-9999");
        res.setTransactionId(xid);
        res.setMessageId("ABC123XYZ");
        res.setMessageQueued(99);
        res.setMessageQueueUpdated(Calendar.getInstance());
        CNContactCreateExt cnContactCreateExt = new CNContactCreateExt();

        CNContact cc = new CNContact();
        cc.setContactID("neustarResponse");
        cc.setContactIDType("QT");
        cc.setContactType("E");
        cc.setPurveyor("my-purveyor-id");
        cc.setMobile("+86.013812345678");

        cnContactCreateExt.setCnContact(cc);
        res.addExtension(cnContactCreateExt);
        System.out.println(res);
        String rootDir = System.getProperty("user.dir");
		rootDir= rootDir.replace("\\", "//");
        EppParser parser = new EppParser(res.toString(),rootDir + "//xsd");
        if (parser.hasError())
        {
            System.out.println(parser.getResult());
            System.exit(1);
        }

        epp = parser.getRootNode();
        newres = (EppResponse) EppResponse.fromXML(epp);
        if (newres == null) {
            System.out.println("Error in fromXML");
        }
        System.out.println(newres);
        
        Assert.assertEquals(res.toString(), newres.toString());
    }
	
}
