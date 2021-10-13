/* -----------------------------------------------------------------------
 * GGPO.net (http://ggpo.net)  -  Copyright 2009 GroundStorm Studios, LLC.
 *
 * Use of this software is governed by the MIT license that can be found
 * in the LICENSE file.
 */

#ifndef _GGPOCS_H
#define _GGPOCS_H

#include "../poll.h"
#include "udp_msg.h"
#include <ggponet.h>
#include <GGPOCSClient.h>
#include <GGPOCSMessage.hpp>
#include "../ring_buffer.h"
#include <nicenet/Service.h>
#include <nicenet/Tcp.h>
#include <string>
#include <kfifo>


struct GGPOCSPlayerMsg
{
	std::string playerID;
	ui32 m_size;
	UdpMsg data;
};
class CS : public IPollSink
{
public:
   struct Stats {
      int      bytes_sent;
      int      packets_sent;
      float    kbps_sent;
   };

   struct Callbacks {
      virtual ~Callbacks() { }
      virtual void OnMsg(const std::string& playerID, UdpMsg *msg, int len) = 0;
   };


protected:
   void Log(const char *fmt, ...);

public:
   CS();

   void InitCS(const char* ip,uint16 port,const char* room, Poll *p, Callbacks *callbacks);
   
   void SendTo(UdpMsg *buffer, int len, int flags, const std::string& fromPlayerID,const std::string& playerID, int destlen);

   virtual bool OnLoopPoll(void *cookie);
   const std::string& getRoom();
public:
   ~CS(void);
	kfifo<GGPOCSPlayerMsg> m_receivedMsgs;
	std::shared_ptr<GGPOCSClient> c;
protected:

   // state management
   Callbacks      *_callbacks;
   Poll           *_poll;
   std::string m_room;
};

#endif
