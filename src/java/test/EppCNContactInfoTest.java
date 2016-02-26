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
import java.util.Calendar;

import org.w3c.dom.Node;

import com.neulevel.cn.CNContact;
import com.neulevel.cn.CNContactInfoExt;
import com.neulevel.epp.core.EppAddress;
import com.neulevel.epp.core.EppAuthInfo;
import com.neulevel.epp.core.EppContact;
import com.neulevel.epp.core.EppContactData;
import com.neulevel.epp.core.EppError;
import com.neulevel.epp.core.EppParser;
import com.neulevel.epp.core.EppStatus;
import com.neulevel.epp.core.EppTransactionId;
import com.neulevel.epp.core.command.EppCommandCreate;
import com.neulevel.epp.core.response.EppResponse;
import com.neulevel.epp.core.response.EppResponseDataInfo;
import com.neulevel.epp.core.response.EppResult;

/**
 * 
 * @author ankit.kothana
 *
 */
public class EppCNContactInfoTest
{
    public static void main( String argv[] )
    {
        testContact();
    }

    private static void testContact()
    {
        EppAddress addr = new EppAddress();
        addr.setStreet(0, "101 Park Street");
        addr.setStreet(1, "Suite 101");
        addr.setCity("Sterling");
        addr.setState("VA");
        addr.setPostalCode("20166");
        addr.setCountryCode("US");

        EppContactData ascii = new EppContactData("FooBar, Inc,", null, addr);
        EppContactData i15d  = new EppContactData("Foo Bar", "FooBar, Inc.", addr);

        EppContact contact = new EppContact("CONTACT-ID");
        contact.setContactDataAscii(ascii);
        contact.setContactDataI15d(i15d);
        contact.setVoice("+1.7035551234", "9999");
        contact.setEmail("mail@foobar.com");
        contact.setAuthInfo(new EppAuthInfo("pw", "NEULEVEL-123"));

        EppCommandCreate cmd = new EppCommandCreate(contact);
        cmd.setClientTransactionId("ABC-DEF-12345");
        
        contact.setDateTransferred(Calendar.getInstance());
        contact.setAuthInfo(new EppAuthInfo("pw", "2fooBar"));
        
        contact.setRoid("OBJECTID-12345");
        contact.setClientId("REGISTRAR-123");
        contact.setClientIdCreated("REGISTRAR-000");
        contact.setDateCreated(Calendar.getInstance());
        contact.setClientIdCreated("REGISTRAR-111");
        contact.setDateUpdated(Calendar.getInstance());
        contact.setClientIdUpdated("REGISTRAR-222");
        contact.addStatus(new EppStatus("ok"));
        contact.addStatus(new EppStatus("pendingDelete"));

        Node epp;
        EppResponse res;
        EppResponse newres;
        EppResult result;
        EppTransactionId xid;

        res = new EppResponse();
        result = new EppResult(EppError.CODE_NO_ERROR,
                               EppError.TEXT_NO_ERROR);
        res.addResult(result);
        xid = new EppTransactionId("ABC-DEF-12345", "SERVER-ID-9999");
        res.setTransactionId(xid);
        res.setResponseData(new EppResponseDataInfo(contact));
        CNContactInfoExt infoExt = new CNContactInfoExt();
        CNContact cc = new CNContact();
        cc.setContactID("neustar");
        cc.setContactIDType("QT");
        cc.setContactType("E");
        cc.setPurveyor("my-purveyor-id");
        cc.setMobile("+86.013812345678");
        infoExt.setContact(cc);
        res.addExtension(infoExt);
        
        System.out.println(res);

        EppParser parser = new EppParser(res.toString());
        if( parser.hasError() )
        {
            System.out.println(parser.getResult());
            System.exit(1);
        }

        epp = parser.getRootNode();
        newres = (EppResponse) EppResponse.fromXML(epp);
        if( newres == null )
        {
            System.out.println("Error in fromXML");
        }
        System.out.println(newres);
    }

}
