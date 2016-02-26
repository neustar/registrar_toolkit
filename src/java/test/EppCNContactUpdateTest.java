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
import org.w3c.dom.Node;

import com.neulevel.cn.CNContact;
import com.neulevel.cn.CNContactUpdateExt;
import com.neulevel.epp.core.EppAddress;
import com.neulevel.epp.core.EppAuthInfo;
import com.neulevel.epp.core.EppContact;
import com.neulevel.epp.core.EppContactData;
import com.neulevel.epp.core.EppParser;
import com.neulevel.epp.core.EppStatus;
import com.neulevel.epp.core.command.EppCommand;
import com.neulevel.epp.core.command.EppCommandUpdateContact;

/**
 * 
 * @author ankit.kothana
 *
 */
public class EppCNContactUpdateTest
{
    public static void main( String argv[] )
    {
        testContact();
    }

    private static void testContact()
    {
        Node epp;
        EppCommandUpdateContact cmd;
        EppCommandUpdateContact newcmd;

        cmd = new EppCommandUpdateContact("CONTACT-12345");
        cmd.addStatus(EppContact.STATUS_CLIENT_DELETE_PROHIBITED);
        cmd.addStatus(EppContact.STATUS_CLIENT_TRANSFER_PROHIBITED);
        cmd.addStatus(EppContact.STATUS_CLIENT_UPDATE_PROHIBITED);
        cmd.addStatus(EppContact.STATUS_LINKED);
        cmd.addStatus(new EppStatus(EppContact.STATUS_OK));
        cmd.removeStatus(new EppStatus(EppContact.STATUS_PENDING_DELETE));
        cmd.removeStatus(EppContact.STATUS_PENDING_TRANSFER);
        cmd.removeStatus(EppContact.STATUS_SERVER_DELETE_PROHIBITED);
        cmd.removeStatus(EppContact.STATUS_SERVER_TRANSFER_PROHIBITED);
        cmd.removeStatus(EppContact.STATUS_SERVER_UPDATE_PROHIBITED);

        EppContactData ascii = new EppContactData();
        ascii.setName("NEW CONTACT NAME");
        cmd.setNewAscii(ascii);

        EppContactData i15d = new EppContactData();
        i15d.setAddress(new EppAddress("100 Park Street", "Sterling", "VA", "20166", "US"));
        cmd.setNewI15d(i15d);

        cmd.setNewVoice("+1.7035551234", "1234");
        cmd.setNewFax("+1.7035551234", "4321");
        cmd.setNewEmail("foo@bar.biz");

        cmd.setNewAuthInfo(new EppAuthInfo("pw", "2fooBar", "NEULEVEL-12345"));

        cmd.setClientTransactionId("ABC-DEF-12345");
        CNContactUpdateExt cnContactUpdateExt = new CNContactUpdateExt();
        
        CNContact cc = new CNContact();
        cc.setContactID("neustar-add");
        cc.setContactIDType("QT");
        cc.setContactType("E");
        cc.setPurveyor("my-purveyor-id");
        cc.setMobile("+86.013812345678");
        
        CNContact ccChange = new CNContact();
        ccChange.setContactID("neustar-change");
        ccChange.setContactIDType("QT");
        ccChange.setContactType("E");
        ccChange.setPurveyor("my-purveyor-id");
        ccChange.setMobile("+86.013812345678");
        
        CNContact ccRem = new CNContact();
        ccRem.setContactID("neustar-rem");
        ccRem.setContactIDType("QT");
        ccRem.setContactType("E");
        ccRem.setPurveyor("my-purveyor-id");
        ccRem.setMobile("+86.013812345678");
        
        cnContactUpdateExt.setAddContact(cc);
        cnContactUpdateExt.setChgContact(ccChange);
        cnContactUpdateExt.setRemContact(ccRem);
        cmd.addExtension(cnContactUpdateExt);
        System.out.println(cmd);

        EppParser parser = new EppParser(cmd.toString());
        if( parser.hasError() )
        {
            System.out.println(parser.getResult());
            System.exit(1);
        }

        epp = parser.getRootNode();
        newcmd = (EppCommandUpdateContact) EppCommand.fromXML(epp);
        if( newcmd == null )
        {
            System.out.println("Error in fromXML");
            System.exit(1);
        }
        System.out.println(newcmd);
    }

}
