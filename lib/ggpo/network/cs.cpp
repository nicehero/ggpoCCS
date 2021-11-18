/* -----------------------------------------------------------------------
 * GGPO.net (http://ggpo.net)  -  Copyright 2009 GroundStorm Studios, LLC.
 *
 * Use of this software is governed by the MIT license that can be found
 * in the LICENSE file.
 */

#include "../types.h"
#include "cs.h"
#include <nicenet/Service.h>

void GGPOCSClient::close()
{
	TcpSessionC::close();
}

CS::CS() :
   _callbacks(NULL),
   m_receivedMsgs(1024)
{
}

CS::~CS(void)
{
	if (c)
	{
		c->m_parent = nullptr;
	}
}


void CS::InitCS(const char* ip,uint16 port,const char* room, Poll *poll, Callbacks *callbacks)
{
	_callbacks = callbacks;

	_poll = poll;
	_poll->RegisterLoop(this);
	m_room = room;
	c = std::make_shared<GGPOCSClient>();
	c->m_parent = this;
	c->connect(ip,port);
	c->init();
	c->startRead();
}


void CS::SendTo(UdpMsg *buffer, int len, int flags,const std::string& fromPlayerID,const std::string& playerID, int destlen)
{
	GGPOCSMessage msg;
	msg.room = m_room;
	msg.fromPlayerID = fromPlayerID;
	msg.playerID = playerID;
	msg.data = nicehero::Binary(ui32(len), buffer);
	if (c){
		c->sendMessage(msg);
	}
}

bool CS::OnLoopPoll(void *cookie)
{
	GGPOCSPlayerMsg d;
	while (m_receivedMsgs.pop(d)){
		_callbacks->OnMsg(d.playerID, &d.data, d.m_size);
	}

	return true;
}


const std::string& CS::getRoom()
{
	return m_room;
}

void CS::Log(const char *fmt, ...)
{
   char buf[1024];
   size_t offset;
   va_list args;

   strcpy(buf, "CS | ");
   offset = strlen(buf);
   va_start(args, fmt);
   vsnprintf(buf + offset, ARRAY_SIZE(buf) - offset - 1, fmt, args);
   buf[ARRAY_SIZE(buf)-1] = '\0';
   ::Log(buf);
   va_end(args);
}

TCP_SESSION_COMMAND(GGPOCSClient, GGPO_CS_MESSAGE_ID)
{
	GGPOCSMessage d;
	*msg >> d;
	CS* cs = dynamic_cast<GGPOCSClient&>(*session.get()).m_parent;
	if (!cs)
	{
		return true;
	}
	if (cs->getRoom() != d.room)
	{
		return true;
	}
	GGPOCSPlayerMsg d2;
	d2.playerID = d.fromPlayerID;
	d2.m_size = d.data.m_Size;
	memcpy(&d2.data,d.data.m_Data.get(),d.data.m_Size);
	cs->m_receivedMsgs.push(d2);
	return true;
}

