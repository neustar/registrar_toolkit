/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
 * reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 * 
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache\@apache.org.
 * 
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log: OldValueVectorOf.hpp,v $
 * Revision 1.1  2013/04/05 17:28:00  achowdhu
 * initial checkin
 *
 * Revision 1.4  2000/03/02 19:54:47  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/24 20:05:26  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/06 07:48:05  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:33  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:19  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(VALUEVECTOROF_HPP)
#define VALUEVECTOROF_HPP

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/ArrayIndexOutOfBoundsException.hpp>
#include <xercesc/util/XMLEnumerator.hpp>


template <class TElem> class OldValueVectorOf
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    OldValueVectorOf(const unsigned int maxElems);
    OldValueVectorOf(const OldValueVectorOf<TElem>& toCopy);
    ~OldValueVectorOf();


    // -----------------------------------------------------------------------
    //  Operators
    // -----------------------------------------------------------------------
    OldValueVectorOf<TElem>& operator=(const OldValueVectorOf<TElem>& toAssign);

    
    // -----------------------------------------------------------------------
    //  Element management
    // -----------------------------------------------------------------------
    void addElement(const TElem& toAdd);
    void setElementAt(const TElem& toSet, const unsigned int setAt);
    void insertElementAt(const TElem& toInsert, const unsigned int insertAt);
    void removeElementAt(const unsigned int removeAt);
    void removeAllElements();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const TElem& elementAt(const unsigned int getAt) const;
    TElem& elementAt(const unsigned int getAt);
    unsigned int curCapacity() const;
    unsigned int size() const;


    // -----------------------------------------------------------------------
    //  Miscellaneous
    // -----------------------------------------------------------------------
    void ensureExtraCapacity(const unsigned int length);
    const TElem* rawData() const;


private:
    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fCurCount
    //      The count of values current added to the vector, which may be
    //      less than the internal capacity.
    //
    //  fMaxCount
    //      The current capacity of the vector.
    //
    //  fElemList
    //      The list of elements, which is dynamically allocated to the needed
    //      size.
    // -----------------------------------------------------------------------
    unsigned int    fCurCount;
    unsigned int    fMaxCount;
    TElem*          fElemList;
};


#if !defined(XERCES_TMPLSINC)
#include <OldValueVectorOf.c>
#endif

#endif
