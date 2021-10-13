/* -----------------------------------------------------------------------
 * GGPO.net (http://ggpo.net)  -  Copyright 2009 GroundStorm Studios, LLC.
 *
 * Use of this software is governed by the MIT license that can be found
 * in the LICENSE file.
 */

#ifndef _CSBACKEND_H
#define _CSBACKEND_H

#include "../types.h"
#include "../poll.h"
#include "../sync.h"
#include "backend.h"
#include "../timesync.h"
#include "../network/cs_proto.h"

class CSBackend : public IQuarkBackend, IPollSink, CS::Callbacks {
public:
   CSBackend(GGPOSessionCallbacks *cb, const char *gamename,const char* room,const std::string playerID,const char* ip, unsigned short serverPort, int num_players, int input_size);
   virtual ~CSBackend();


public:
   virtual GGPOErrorCode DoPoll(int timeout);
   virtual GGPOErrorCode AddPlayer(GGPOPlayer *player, GGPOPlayerHandle *handle);
   virtual GGPOErrorCode AddLocalInput(GGPOPlayerHandle player, void *values, int size);
   virtual GGPOErrorCode SyncInput(void *values, int size, int *disconnect_flags);
   virtual GGPOErrorCode IncrementFrame(void);
   virtual GGPOErrorCode DisconnectPlayer(GGPOPlayerHandle handle);
   virtual GGPOErrorCode GetNetworkStats(GGPONetworkStats *stats, GGPOPlayerHandle handle);
   virtual GGPOErrorCode SetFrameDelay(GGPOPlayerHandle player, int delay);
   virtual GGPOErrorCode SetDisconnectTimeout(int timeout);
   virtual GGPOErrorCode SetDisconnectNotifyStart(int timeout);

public:
   virtual void OnMsg(const std::string& from, UdpMsg *msg, int len);
   bool isServerConnected();
protected:
   GGPOErrorCode PlayerHandleToQueue(GGPOPlayerHandle player, int *queue);
   GGPOPlayerHandle QueueToPlayerHandle(int queue) { return (GGPOPlayerHandle)(queue + 1); }
   GGPOPlayerHandle QueueToSpectatorHandle(int queue) { return (GGPOPlayerHandle)(queue + 1000); } /* out of range of the player array, basically */
   void DisconnectPlayerQueue(int queue, int syncto);
   void PollSyncEvents(void);
   void PollCSProtocolEvents(void);
   void CheckInitialSync(void);
   int Poll2Players(int current_frame);
   int PollNPlayers(int current_frame);
   void AddRemotePlayer(const std::string& playerID, int queue);
   GGPOErrorCode AddSpectator(const std::string& playerID);
   virtual void OnSyncEvent(Sync::Event &e) { }
   virtual void OnCSProtocolEvent(CSProtocol::Event &e, GGPOPlayerHandle handle);
   virtual void OnCSProtocolPeerEvent(CSProtocol::Event &e, int queue);
   virtual void OnCSProtocolSpectatorEvent(CSProtocol::Event &e, int queue);

protected:
   GGPOSessionCallbacks  _callbacks;
   Poll                  _poll;
   Sync                  _sync;
   CS                   _cs;
   CSProtocol           *_endpoints;
   CSProtocol           _spectators[GGPO_MAX_SPECTATORS];
   int                   _num_spectators;
   int                   _input_size;

   bool                  _synchronizing;
   int                   _num_players;
   int                   _next_recommended_sleep;

   int                   _next_spectator_frame;
   int                   _disconnect_timeout;
   int                   _disconnect_notify_start;
	std::string			m_playerID;
   UdpMsg::connect_status _local_connect_status[UDP_MSG_MAX_PLAYERS];
};

#endif
