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
import java.util.Vector;

import org.w3c.dom.Node;

import com.neulevel.cn.CNCDNExt;
import com.neulevel.epp.core.EppAuthInfo;
import com.neulevel.epp.core.EppParser;
import com.neulevel.epp.core.command.EppCommand;
import com.neulevel.epp.core.command.EppCommandInfoDomain;

/**
 * 
 * @author ankit.kothana
 *
 */
public class EppCNCDNExtTest
{
    public static void main( String argv[] )
    {
        testDomain();
    }

    private static void testDomain()
    {
        Node epp;
        EppCommand cmd;
        EppCommand newcmd;

        cmd = new EppCommandInfoDomain("example.cn");
        cmd.setClientTransactionId("ABC-DEF-12345");
        ((EppCommandInfoDomain) cmd).setAuthInfo(new EppAuthInfo(EppAuthInfo.TYPE_PW, "2fooBar"));
        ((EppCommandInfoDomain) cmd).setHostsControl(EppCommandInfoDomain.HOSTS_NONE);
        CNCDNExt cdnExt = new CNCDNExt();
        cdnExt.setOCDNPunycode("oCDNPunnyCode");
        cdnExt.setSCDN("scdn");
        cdnExt.setSCDNPunycode("sdnPunnyCode");
        cdnExt.setTCDN("tcdn");
        cdnExt.setTCDNPunycode("tdnPunnyCode");
        Vector<String> vcdn = new Vector<String>();
        vcdn.add("vcdn1");
        vcdn.add("vcdn2");
        Vector<String> vcdnPunnyCode = new Vector<String>();
        vcdnPunnyCode.add("vcdnPunnyCode1");
        vcdnPunnyCode.add("vcdnPunnyCode2");
        vcdnPunnyCode.add("vcdnPunnyCode3");
        cdnExt.setVCDN(vcdn);
        cdnExt.setVCDNPunycode(vcdnPunnyCode);
        cmd.addExtension(cdnExt);
        System.out.println(cmd);
        EppParser parser = new EppParser(cmd.toString());
        if( parser.hasError() )
        {
            System.out.println(parser.getResult());
            System.exit(1);
        }

        epp = parser.getRootNode();
        newcmd = (EppCommand) EppCommand.fromXML(epp);
        if( newcmd == null )
        {
            System.out.println("Error in fromXML");
            System.exit(1);
        }
        System.out.println(newcmd);
    }

}
