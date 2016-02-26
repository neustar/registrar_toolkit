package com.neulevel.cn;

import java.util.Vector;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.neulevel.epp.core.EppUtil;
import com.neulevel.epp.ext.EppExtension;

public class CNCDNExt extends EppExtension {
    
    /**
     * Original Puny code Domain
     */
    private String OCDNPunycode;

    /**
     * Simplified Chinese Domain
     */
    private String SCDN;

    /**
     * Simplified Puny code Domain
     */
    private String SCDNPunycode;

    /**
     * Traditional Chinese Domain
     */
    private String TCDN;

    /**
     * Traditional Puny code Domain
     */
    private String TCDNPunycode;

    private Vector<String> VCDN;
    
    private Vector<String> VCDNPunycode;
    
    /**
     * @return the oCDNPunycode
     */
    public String getOCDNPunycode() {
        return OCDNPunycode;
    }

    /**
     * @param oCDNPunycode the oCDNPunycode to set
     */
    public void setOCDNPunycode(String oCDNPunycode) {
        OCDNPunycode = oCDNPunycode;
    }

    /**
     * @return the sCDN
     */
    public String getSCDN() {
        return SCDN;
    }

    /**
     * @param sCDN the sCDN to set
     */
    public void setSCDN(String sCDN) {
        SCDN = sCDN;
    }

    /**
     * @return the sCDNPunycode
     */
    public String getSCDNPunycode() {
        return SCDNPunycode;
    }

    /**
     * @param sCDNPunycode the sCDNPunycode to set
     */
    public void setSCDNPunycode(String sCDNPunycode) {
        SCDNPunycode = sCDNPunycode;
    }

    /**
     * @return the tCDN
     */
    public String getTCDN() {
        return TCDN;
    }

    /**
     * @param tCDN the tCDN to set
     */
    public void setTCDN(String tCDN) {
        TCDN = tCDN;
    }

    /**
     * @return the tCDNPunycode
     */
    public String getTCDNPunycode() {
        return TCDNPunycode;
    }

    /**
     * @param tCDNPunycode the tCDNPunycode to set
     */
    public void setTCDNPunycode(String tCDNPunycode) {
        TCDNPunycode = tCDNPunycode;
    }

    /**
     * @return the vCDN
     */
    public Vector<String> getVCDN() {
        return VCDN;
    }

    /**
     * @param vCDN the vCDN to set
     */
    public void setVCDN(Vector<String> vCDN) {
        VCDN = vCDN;
    }

    /**
     * @return the vCDNPunycode
     */
    public Vector<String> getVCDNPunycode() {
        return VCDNPunycode;
    }

    /**
     * @param vCDNPunycode the vCDNPunycode to set
     */
    public void setVCDNPunycode(Vector<String> vCDNPunycode) {
        VCDNPunycode = vCDNPunycode;
    }

    @Override
    public Element toXML(Document doc, String tag) {
        Element body = EppUtil.createElementNS(doc, "cdn", (tag == null) ? "infData" : tag,true,"-1.0");
        if (OCDNPunycode != null && OCDNPunycode.length() > 0) {
            Element elem = doc.createElement("OCDNPunycode");
            elem.appendChild(doc.createTextNode(this.OCDNPunycode));
            body.appendChild(elem);
        }
        
        if (SCDN != null && SCDN.length() > 0) {
            Element elem = doc.createElement("SCDN");
            elem.appendChild(doc.createTextNode(this.SCDN));
            body.appendChild(elem);
        }
        
        if (SCDNPunycode != null && SCDNPunycode.length() > 0) {
            Element elem = doc.createElement("SCDNPunycode");
            elem.appendChild(doc.createTextNode(this.SCDNPunycode));
            body.appendChild(elem);
        }
        
        if (TCDN != null && TCDN.length() > 0) {
            Element elem = doc.createElement("TCDN");
            elem.appendChild(doc.createTextNode(this.TCDN));
            body.appendChild(elem);
        }
        
        if (TCDNPunycode != null && TCDNPunycode.length() > 0) {
            Element elem = doc.createElement("TCDNPunycode");
            elem.appendChild(doc.createTextNode(this.TCDNPunycode));
            body.appendChild(elem);
        }
        
        Element vCDNElem = null;
        
        if (VCDN != null && VCDN.size() > 0) {
            vCDNElem = doc.createElement("VCDNList");
            for (int i = 0; i < VCDN.size(); i++) {
                Element elem = doc.createElement("VCDN");
                elem.appendChild(doc.createTextNode(this.VCDN.get(i)));
                vCDNElem.appendChild(elem);
            }
        }
        
        if (VCDNPunycode != null && VCDNPunycode.size() > 0) {
            if (vCDNElem == null) {
                vCDNElem = doc.createElement("VCDNList");
            }
            for (int i = 0; i < VCDNPunycode.size(); i++) {
                Element elem = doc.createElement("VCDNPunycode");
                elem.appendChild(doc.createTextNode(this.VCDNPunycode.get(i)));
                vCDNElem.appendChild(elem);
            }
        }
        
        if (vCDNElem != null) {
            body.appendChild(vCDNElem);
        }
        
        return body;
    }
    
    public static CNCDNExt fromXML(Node root) {
        CNCDNExt returnCNCDNExt = new CNCDNExt();
        NodeList nodeList = root.getChildNodes();
        
        for (int i=0; i< nodeList.getLength(); i++) {
            Node node = nodeList.item(i);
            String name = node.getLocalName();
            if (name == null) {
                name = node.getNodeName();
            }
            
            if (name != null) {
                if (name.equals("OCDNPunycode")) {
                    returnCNCDNExt.OCDNPunycode = EppUtil.getText(node);
                    continue;
                }
                
                if (name.equals("SCDN")) {
                    returnCNCDNExt.SCDN = EppUtil.getText(node);
                    continue;
                }
                
                if (name.equals("SCDNPunycode")) {
                    returnCNCDNExt.SCDNPunycode = EppUtil.getText(node);
                    continue;
                }
                
                if (name.equals("TCDN")) {
                    returnCNCDNExt.TCDN = EppUtil.getText(node);
                    continue;
                }
                
                if (name.equals("TCDNPunycode")) {
                    returnCNCDNExt.TCDNPunycode = EppUtil.getText(node);
                    continue;
                }
                
                if (name.equals("VCDNList")) {
                    NodeList vNodelist = node.getChildNodes();
                    for (int j=0; j < vNodelist.getLength(); j++) {
                        Node vNode = vNodelist.item(j);
                        String vName = vNode.getLocalName();
                        if (vName == null) {
                            vName = vNode.getNodeName();
                        }
                        
                        if (vName != null) {
                            if (vName.equals("VCDN")) {
                                if (returnCNCDNExt.VCDN == null) {
                                    returnCNCDNExt.VCDN = new Vector<String>();
                                } 
                                
                                returnCNCDNExt.VCDN.add(EppUtil.getText(vNode));
                                continue;
                            }
                            
                            if (vName.equals("VCDNPunycode")) {
                                if (returnCNCDNExt.VCDNPunycode == null) {
                                    returnCNCDNExt.VCDNPunycode = new Vector<String>();
                                } 
                                
                                returnCNCDNExt.VCDNPunycode.add(EppUtil.getText(vNode));
                            }
                        }
                    }
                }
                
                
                
            } else {
                continue;
            }
            
        }
        return returnCNCDNExt;
    }

    @Override
    public String toString() {
        return "CNCDNExt [OCDNPunycode=" + OCDNPunycode + ", SCDN=" + SCDN
                + ", SCDNPunycode=" + SCDNPunycode + ", TCDN=" + TCDN
                + ", TCDNPunycode=" + TCDNPunycode + ", VCDN=" + VCDN
                + ", VCDNPunycode=" + VCDNPunycode + "]";
    }

}
