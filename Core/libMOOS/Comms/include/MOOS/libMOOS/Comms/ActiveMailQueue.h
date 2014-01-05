///////////////////////////////////////////////////////////////////////////
//
//   This file is part of the MOOS project
//
//   MOOS : Mission Oriented Operating Suite A suit of
//   Applications and Libraries for Mobile Robotics Research
//   Copyright (C) Paul Newman
//
//   This software was written by Paul Newman at MIT 2001-2002 and
//   the University of Oxford 2003-2013
//
//   email: pnewman@robots.ox.ac.uk.
//
//   This source code and the accompanying materials
//   are made available under the terms of the GNU Lesser Public License v2.1
//   which accompanies this distribution, and is available at
//   http://www.gnu.org/licenses/lgpl.txt
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
////////////////////////////////////////////////////////////////////////////
/*
 * ActiveMailQueue.h
 *
 *  Created on: Dec 6, 2012
 *      Author: pnewman
 */

#ifndef ACTIVEMAILQUEUE_H_
#define ACTIVEMAILQUEUE_H_
#include "MOOS/libMOOS/Comms/MOOSMsg.h"
#include "MOOS/libMOOS/Utils/MOOSThread.h"
#include "MOOS/libMOOS/Utils/SafeList.h"
#include "MOOS/libMOOS/Comms/MessageFunction.h"


namespace MOOS {

/** \internal
	@brief provides a queue (serviced by a thread and callback) of CMOOSMsg's

	Used in CommsClient classes to support AddMessageCallback function

    @author Paul Newman

	\endinternal
*/

class ActiveMailQueue
{
public:

	//install a callback onto the queue C style
	void SetCallback(bool (*pfn)(CMOOSMsg &M, void * pParamCaller), void * pCallerParam);

	//install a callback onto the queue but supply a instance of a class and a member function
	//which takes single message as a parameter
	template <class T>
    void SetCallback(T* Instance,bool (T::*memfunc)(CMOOSMsg &));

    ActiveMailQueue(const std::string &sName);
    	virtual ~ActiveMailQueue();

	//us this to push work onto the queue
	bool Push(const CMOOSMsg & M);

    bool DoWork();
    bool Stop();
    bool Start();
    std::string GetName();


protected:
	MOOS::SafeList<CMOOSMsg> queue_;

    /** the user supplied Callback*/
    bool (*pfn_)(CMOOSMsg &M, void* pParam);
    void * caller_param_;

    //or using the fancy class member functionality
    //given by MessageFunction.h
    MOOS::MsgFunctor* pClassMemberFunctionCallback_;

    CMOOSThread thread_;

    //this is a mick name for the Queue
    std::string Name_;

};


template <class T>
   void ActiveMailQueue::SetCallback(T* Instance,bool (T::*memfunc)(CMOOSMsg &))
   {
   	pfn_=NULL;
   	caller_param_ = NULL;

   	pClassMemberFunctionCallback_ = MOOS::BindMsgFunctor<T>(Instance,memfunc);
   }

}

#endif /* ACTIVEMAILQUEUE_H_ */
