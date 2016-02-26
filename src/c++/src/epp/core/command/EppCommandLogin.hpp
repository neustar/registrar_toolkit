/*******************************************************************************
 * The MIT License (MIT)
 *  
 * Copyright (c)2015 Neustar Inc.
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

#if ! defined(EPPCOMMANDLOGIN_HPP)    /* { */
#define       EPPCOMMANDLOGIN_HPP        1

#include "EppCommand.hpp"
#include "EppServiceMenu.hpp"

/**
 * This <code>EppCommandLogin</code> class implements EPP Command Login
 * entity.
 *
 * @author Ning Zhang ning.zhang@neustar.com
 * @version $Revision: 1.2 $ $Date: 2006/03/01 01:35:37 $
 */
class EPP_EXPORT EppCommandLogin : public EppCommand
{
private:
	EppServiceMenu * svcs;
	DOMString	 clID;
	DOMString	 pw;
	DOMString	 newPW;
	EppCredsOptions* options;

public:

	/**
	 * Creates an <code>EppCommandLogin</code> object
	 */
	EppCommandLogin()
	{
		this->svcs = null;
		this->options = null;
	};

	/**
	 * Creates an <code>EppCommandLogin</code> object
	 *
	 * @param serviceMenu the menu of services to be supported
	 */
	EppCommandLogin( EppServiceMenu& serviceMenu )
	{
		this->svcs = null;
		setServiceMenu(serviceMenu);
		this->clID = null;
		this->pw   = null;
		this->newPW = null;
		this->options = new EppCredsOptions("1.0", "en-US");
	};

	/**
	 * Destructor
	 */
	~EppCommandLogin()
	{
		if( this->svcs != null )
		{
			delete this->svcs;
			this->svcs = null;
		}
		if( this->options != null )
		{
			delete this->options;
			this->options = null;
		}
	};

	/**
	 * Returns the run-time type of an EppEntity object
	 */
	virtual int getEntityType()
	{
		return EppEntity::TYPE_EppCommandLogin;
	};

	/**
	 * Gets the menu of services to be supported
	 */
	EppServiceMenu * getServiceMenu()
	{
		return this->svcs;
	};

	/**
	 * Sets the menu of services to be supported
	 */
	void setServiceMenu( EppServiceMenu& serviceMenu );

	/**
	 * Gets registrar client id
	 */
	DOMString getClientId()
	{
		return this->clID;
	};

	/**
	 * Sets registrar client id
	 */
	void setClientId( DOMString clientId )
	{
		this->clID = clientId;
	};

	/**
	 * Gets registrar client's password
	 */
	DOMString getPassword()
	{
		return this->pw;
	};

	/**
	 * Sets registrar client's password
	 */
	void setPassword( DOMString password )
	{
		this->pw = password;
	};

	/**
	 * Gets registrar client's new password
	 */
	DOMString getNewPassword()
	{
		return this->newPW;
	};

	/**
	 * Sets registrar client's new password
	 */
	void setNewPassword( DOMString newPassword )
	{
		this->newPW = newPassword;
	};

	/**
	 * Gets credentials options
	 */
	EppCredsOptions * getOptions()
	{
		return this->options;
	};

	/**
	 * Sets credentials options
	 */
	void setOptions( EppCredsOptions& opt )
	{
		if( this->options == null )
		{
			this->options = new EppCredsOptions("1.0", "en-US");
		}
		this->options->setVersion(opt.getVersion());
		this->options->setLanguage(opt.getLanguage());
	};

	/**
	 * Sets the login credentials - for backward compatibility
	 */
	void setCreds( EppCreds& creds )
	{
		this->clID    = creds.getClientId();
		this->pw      = creds.getPassword();
		this->newPW   = creds.getNewPassword();

		EppCredsOptions * opt = creds.getOptions();
		if( opt != null )
		{
			EppCredsOptions tmp = *opt;
			this->setOptions(tmp);
		}
	};

	/**
	 * Gets the login credentials - for backward compatibility
	 *
	 * <P><B>Note:</B> Caller must free the object returned.
	 */
	EppCreds * getCreds()
	{
		EppCreds * creds = new EppCreds(this->clID, this->pw, this->newPW);
		if( creds != null )
		{
			if( this->options != null )
			{
				EppCredsOptions opt = *(this->options);
				creds->setOptions(opt);
			}
		}
		return creds;
	};

	/**
	 * Converts the <code>EppCommandLogin</code> object into an XML element
	 *
	 * @param doc the XML <code>DOMElement</code> object
	 * @param tag the tag/element name for the <code>EppCommandLogin</code>
	 *            object
	 *
	 * @return an <code>DOMElement</code> object
	 */
	DOMElement* toXML( DOMDocument& doc, const DOMString& tag );

	/**
	 * Converts an XML element into an <code>EppCommandLogin</code> object.
	 * The caller of this method must make sure that the root node is of an
	 * EPP Command Create entity.
	 *
	 * @param root root node for an <code>EppCommandLogin</code> object
	 *             in XML format
	 *
	 * @return an <code>EppCommandLogin</code> object, or null if the node
	 *         is invalid
	 */
	static EppCommandLogin * fromXML( const DOMNode& root );

	DOMString toString()
	{
		return EppEntity::toString(DOMString("login"));
	};
};

#endif     /* EPPCOMMANDLOGIN_HPP */  /* } */
