#ifndef _GGPOCSMessage_H
#define _GGPOCSMessage_H

#include <nicenet/Service.h>
#include <nicenet/Tcp.h>

#define GGPO_CS_MESSAGE_ID					48888

struct GGPOCSMessage :
	public nicehero::Serializable
{
public:
	std::string room;
	std::string fromPlayerID;
	std::string playerID;
	nicehero::Binary data;

	ui32 getSize() const;

	ui16 getID() const;
	void serializeTo(nicehero::Message& msg) const;
	void unserializeFrom(nicehero::Message& msg);
};

inline ui32 GGPOCSMessage::getSize() const
{
	ui32 s = 0;
	s += nicehero::Serializable::getSize(room);
	s += nicehero::Serializable::getSize(fromPlayerID);
	s += nicehero::Serializable::getSize(playerID);
	s += nicehero::Serializable::getSize(data);
	return s;
}
inline nicehero::Message & operator << (nicehero::Message &m, const GGPOCSMessage& p)
{
	m << p.room;
	m << p.fromPlayerID;
	m << p.playerID;
	m << p.data;
	return m;
}

inline nicehero::Message & operator >> (nicehero::Message &m, GGPOCSMessage& p)
{
	m >> p.room;
	m >> p.fromPlayerID;
	m >> p.playerID;
	m >> p.data;
	return m;
}
inline void GGPOCSMessage::serializeTo(nicehero::Message& msg) const
{
	msg << (*this);
}

inline void GGPOCSMessage::unserializeFrom(nicehero::Message& msg)
{
	msg >> (*this);
}

inline ui16 GGPOCSMessage::getID() const
{
	return GGPO_CS_MESSAGE_ID;
}


#endif
