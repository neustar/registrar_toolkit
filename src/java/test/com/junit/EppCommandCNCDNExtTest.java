package com.junit;

import java.util.Vector;

import junit.framework.Assert;

import org.junit.Test;
import org.w3c.dom.Node;

import com.neulevel.cn.CNCDNExt;
import com.neulevel.epp.core.EppAuthInfo;
import com.neulevel.epp.core.EppParser;
import com.neulevel.epp.core.command.EppCommand;
import com.neulevel.epp.core.command.EppCommandInfoDomain;

public class EppCommandCNCDNExtTest 
{
	@Test 
	public void testDomain()
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
        
        String rootDir = System.getProperty("user.dir");
		rootDir= rootDir.replace("\\","//");
        EppParser parser = new EppParser(cmd.toString(),rootDir+"//xsd");
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
        Assert.assertEquals(cmd.toString(), newcmd.toString());
    }
	
	
	
	@Test 
	public void testDomainChineseSCDNPunnyCode()
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
        cdnExt.setSCDNPunycode("导演.中国");
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
        
        String rootDir = System.getProperty("user.dir");
		rootDir= rootDir.replace("\\", "//");
        EppParser parser = new EppParser(cmd.toString(),rootDir + "//xsd");
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
        Assert.assertEquals(cmd.toString(), newcmd.toString());
    }


}
