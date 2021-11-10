/* -----------------------------------------------------------------------
 * GGPO.net (http://ggpo.net)  -  Copyright 2009 GroundStorm Studios, LLC.
 *
 * Use of this software is governed by the MIT license that can be found
 * in the LICENSE file.
 */

#include "backends/p2p.h"
#include "backends/synctest.h"
#include "backends/spectator.h"
#include "backends/csbackend.h"
#include "types.h"
#include "ggponet.h"

struct Init
{
	Init() {
		srand(Platform::GetCurrentTimeMS() + Platform::GetProcessID());
#ifdef _WIN32
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif
	}
};
static Init init;

void
ggpo_log(GGPOSession *ggpo, const char *fmt, ...)
{
   va_list args;
   va_start(args, fmt);
   ggpo_logv(ggpo, fmt, args);
   va_end(args);
}

void
ggpo_logv(GGPOSession *ggpo, const char *fmt, va_list args)
{
   if (ggpo)
      ggpo->Logv(fmt, args);
}

GGPOErrorCode
ggpo_start_session(GGPOSession **session,
                   GGPOSessionCallbacks *cb,
                   const char *game,
                   int num_players,
                   int input_size,
                   uint64_t localport)
{
	try {
	   *session= (GGPOSession *)new Peer2PeerBackend(cb,
													 game,
													 localport,
													 num_players,
													 input_size);
	   return GGPO_OK;
	} catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_start_session: %s", e.what());
	   return e.ggpoError;
	}
}

GGPOErrorCode
ggpo_add_player(GGPOSession *ggpo,
                GGPOPlayer *player,
                GGPOPlayerHandle *handle)
{
   if (!ggpo)
      return GGPO_ERRORCODE_INVALID_SESSION;
   try {
	   return ggpo->AddPlayer(player, handle);
   } catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_add_player: %s", e.what());
	   return e.ggpoError;
   }
}



GGPOErrorCode
ggpo_start_synctest(GGPOSession **ggpo,
                    GGPOSessionCallbacks *cb,
                    const char *game,
                    int num_players,
                    int input_size,
                    int frames)
{
	try {
	   *ggpo = (GGPOSession *)new SyncTestBackend(cb, game, frames, num_players);
	   return GGPO_OK;
	} catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_start_synctest: %s", e.what());
	   return e.ggpoError;
	}
}

GGPOErrorCode
ggpo_set_frame_delay(GGPOSession *ggpo,
                     GGPOPlayerHandle player,
                     int frame_delay)
{
   if (!ggpo)
      return GGPO_ERRORCODE_INVALID_SESSION;
   try {
	   return ggpo->SetFrameDelay(player, frame_delay);
   } catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_set_frame_delay: %s", e.what());
	   return e.ggpoError;
   }
}

GGPOErrorCode
ggpo_idle(GGPOSession *ggpo, int timeout)
{
   if (!ggpo)
      return GGPO_ERRORCODE_INVALID_SESSION;
   try {
	   return ggpo->DoPoll(timeout);
   } catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_idle: %s", e.what());
	   return e.ggpoError;
   }
}

GGPOErrorCode
ggpo_add_local_input(GGPOSession *ggpo,
                     GGPOPlayerHandle player,
                     void *values,
                     int size)
{
   if (!ggpo)
      return GGPO_ERRORCODE_INVALID_SESSION;
   try {
      return ggpo->AddLocalInput(player, values, size);
   } catch (const GGPOException& e) {
      Log("GGPOException in ggpo_add_local_input: %s", e.what());
      return e.ggpoError;
   }
}

GGPOErrorCode
ggpo_synchronize_input(GGPOSession *ggpo,
                       void *values,
                       int size,
                       int *disconnect_flags)
{
   if (!ggpo)
      return GGPO_ERRORCODE_INVALID_SESSION;
   try {
      return ggpo->SyncInput(values, size, disconnect_flags);
   } catch (const GGPOException& e) {
      Log("GGPOException in ggpo_synchronize_input: %s", e.what());
      return e.ggpoError;
   }
}

GGPOErrorCode ggpo_disconnect_player(GGPOSession *ggpo,
                                     GGPOPlayerHandle player)
{
   if (!ggpo)
      return GGPO_ERRORCODE_INVALID_SESSION;
   try {
	   return ggpo->DisconnectPlayer(player);
   } catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_disconnect_player: %s", e.what());
	   return e.ggpoError;
   }
}

GGPOErrorCode
ggpo_advance_frame(GGPOSession *ggpo)
{
   if (!ggpo)
      return GGPO_ERRORCODE_INVALID_SESSION;
   try {
	   return ggpo->IncrementFrame();
   } catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_advance_frame: %s", e.what());
	   return e.ggpoError;
   }
}

GGPOErrorCode
ggpo_client_chat(GGPOSession *ggpo, char *text)
{
   if (!ggpo)
      return GGPO_ERRORCODE_INVALID_SESSION;
   try {
	   return ggpo->Chat(text);
   } catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_client_chat: %s", e.what());
	   return e.ggpoError;
   }
}

GGPOErrorCode
ggpo_get_network_stats(GGPOSession *ggpo,
                       GGPOPlayerHandle player,
                       GGPONetworkStats *stats)
{
   if (!ggpo)
      return GGPO_ERRORCODE_INVALID_SESSION;
   try {
	   return ggpo->GetNetworkStats(stats, player);
   } catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_get_network_stats: %s", e.what());
	   return e.ggpoError;
   }
}


GGPOErrorCode
ggpo_close_session(GGPOSession *ggpo)
{
   if (!ggpo) {
      return GGPO_ERRORCODE_INVALID_SESSION;
   }
   delete ggpo;
   return GGPO_OK;
}

GGPOErrorCode
ggpo_set_disconnect_timeout(GGPOSession *ggpo, int timeout)
{
   if (!ggpo)
      return GGPO_ERRORCODE_INVALID_SESSION;
   try {
	   return ggpo->SetDisconnectTimeout(timeout);
   } catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_set_disconnect_timeout: %s", e.what());
	   return e.ggpoError;
   }
}

GGPOErrorCode
ggpo_set_disconnect_notify_start(GGPOSession *ggpo, int timeout)
{
   if (!ggpo)
      return GGPO_ERRORCODE_INVALID_SESSION;
   try {
	   return ggpo->SetDisconnectNotifyStart(timeout);
   } catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_set_disconnect_notify_start: %s", e.what());
	   return e.ggpoError;
   }
}

GGPOErrorCode ggpo_start_spectating(GGPOSession **session,
                                    GGPOSessionCallbacks *cb,
                                    const char *game,
                                    int num_players,
                                    int input_size,
                                    unsigned short local_port,
                                    char *host_ip,
                                    unsigned short host_port)
{
	try {
	   *session= (GGPOSession *)new SpectatorBackend(cb,
													 game,
													 local_port,
													 num_players,
													 input_size,
													 host_ip,
													 host_port);
	   return GGPO_OK;
	} catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_start_spectating: %s", e.what());
	   return e.ggpoError;
	}
}

std::shared_ptr<GGPOCSClient> ggpo_get_cs_client_obj(GGPOSession *session)
{
	CSBackend* backend = dynamic_cast<CSBackend *>(session);
	if (!backend)
	{
		return std::shared_ptr<GGPOCSClient>();
	}
	return backend->getClientObj();
}

GGPOErrorCode
ggpo_start_cssession(GGPOSession **session,
                   GGPOSessionCallbacks *cb,
                   const char *game,
                   int num_players,
                   int input_size,
                   const char* room,
                   const char* playerID,
				   const char* serverIP,
				   unsigned short serverPort)
{
	try {
		CSBackend* backend = new CSBackend(cb, game, room, playerID, serverIP, serverPort, num_players, input_size);
		if (!backend->isServerConnected())
		{
			delete backend;
			*session = nullptr;
			return GGPO_ERRORCODE_CS_CONNECT_FAILED;
		}
		*session = (GGPOSession *)backend;
	   return GGPO_OK;
	} catch (const GGPOException& e) {
	   Log("GGPOException in ggpo_start_session: %s", e.what());
	   return e.ggpoError;
	}
}

#include <nicenet/nicenet.h>
#ifdef _WIN32
#include <windows.h>
#endif
extern "C"{
	GGPOSession* g_ggpo = nullptr;
	std::string g_playerID;
	void unity_ggpo_start(){
		nicehero::start(true);
	}
	void unity_ggpo_stop(){
		if (g_ggpo)
		{
			ggpo_close_session(g_ggpo);
			g_ggpo = nullptr;
			g_playerID = "";
		}
		nicehero::stop();
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
		nicehero::joinMain();
	}
	int UggSetDisconnectNotifyStart(int timeout)
	{
		return ggpo_set_disconnect_notify_start(g_ggpo, timeout);
	}

	int UggSetDisconnectTimeout(int timeout)
	{
		return ggpo_set_disconnect_timeout(g_ggpo, timeout);
	}

	int UggSynchronizeInput(uint64_t* inputs, int length, int& disconnect_flags)
	{
		return ggpo_synchronize_input(g_ggpo, inputs, sizeof(uint64_t) * length, &disconnect_flags);
	}

	int UggAddLocalInput(int local_player_handle, uint64_t input)
	{
		return ggpo_add_local_input(g_ggpo, local_player_handle, &input, sizeof(uint64_t));
	}

	int UggCloseSession()
	{
		if (g_ggpo)
		{
			ggpo_close_session(g_ggpo);
			g_ggpo = nullptr;
			g_playerID = "";
		}
		return 0;
	}

	int UggIdle(int timeout)
	{
		return ggpo_idle(g_ggpo, timeout);
	}

	typedef void (*LogDelegate)(const char* text);
	typedef bool (*BeginGameDelegate)(const char* text);
	typedef bool (*AdvanceFrameDelegate)(int flags);
	typedef bool (*LoadGameStateDelegate)(unsigned char* buffer, int length);
	typedef bool (*LogGameStateDelegate)(char* text, unsigned char* buffer, int length);
	typedef bool (*SaveGameStateDelegate)(unsigned char** buffer, int* len, int* checksum, int frame);
	typedef void (*FreeBufferDelegate)(void* buffer);
	typedef bool (*OnEventDelegate)(GGPOEvent* info);
	

	int unity_ggpo_game_init(
	const char* playerID
	, const char* serverIP, int serverPort
	, const char* room
	, int num_players, int num_spectators,    
	BeginGameDelegate beginGame,
    AdvanceFrameDelegate advanceFrame,
    LoadGameStateDelegate loadGameState,
    LogGameStateDelegate logGameState,
    SaveGameStateDelegate saveGameState,
    FreeBufferDelegate freeBuffer,
    OnEventDelegate onEvent)
	{
	   GGPOErrorCode result;

	   GGPOSessionCallbacks cb = { 0 };
	   cb.begin_game      = beginGame;
	   cb.advance_frame	 = advanceFrame;
	   cb.load_game_state = loadGameState;
	   cb.save_game_state = saveGameState;
	   cb.free_buffer     = freeBuffer;
	   cb.on_event        = onEvent;
	   cb.log_game_state  = logGameState;

	   result = ggpo_start_cssession(&g_ggpo, &cb, "vectorwar", num_players, sizeof(int),room,playerID,serverIP,(unsigned short)serverPort);
	   if (result == GGPO_ERRORCODE_CS_CONNECT_FAILED)
	   {
		   int retryConnect = 0;
			while (ggpo_start_cssession(
				&g_ggpo, &cb, "vectorwar", num_players, sizeof(int), room, playerID, serverIP, (unsigned short)serverPort
			) == GGPO_ERRORCODE_CS_CONNECT_FAILED) 
			{
				if (retryConnect > 4)
				{
					return result;
				}
#ifdef _WIN32
				Sleep(100);
#else
				usleep(100000);
#endif
				++retryConnect;
			}
	   }
	   // automatically disconnect clients after 3000 ms and start our count-down timer
	   // for disconnects after 1000 ms.   To completely disable disconnects, simply use
	   // a value of 0 for ggpo_set_disconnect_timeout.
	   ggpo_set_disconnect_timeout(g_ggpo, 3000);
	   ggpo_set_disconnect_notify_start(g_ggpo, 1000);
	   g_playerID = playerID;
/*
	   int i;
	   for (i = 0; i < num_players + num_spectators; i++) {
		  GGPOPlayerHandle handle;
		  result = ggpo_add_player(g_ggpo, players + i, &handle);
		  if (result != 0) {
			  return result;
		  }
		  ngs.players[i].handle = handle;
		  ngs.players[i].type = players[i].type;
		  if (players[i].type == GGPO_PLAYERTYPE_LOCAL) {
			 ngs.players[i].connect_progress = 100;
			 ngs.local_player_handle = handle;
			 ngs.SetConnectState(handle, Connecting);
			 ggpo_set_frame_delay(g_ggpo, handle, FRAME_DELAY);
		  } else {
			 ngs.players[i].connect_progress = 0;
		  }
	   }
*/
	   return result;
	}
	int unity_ggpo_game_add_player(
		int player_type,
		int player_num,
		const char* playerID,
		int& phandle)
	{
		if (!g_ggpo){
			return -1;
		}
		GGPOPlayer player;
		player.size = sizeof(GGPOPlayer);
		player.type = (GGPOPlayerType)player_type;
		player.player_num = player_num;
		strcpy(player.u.csremote.playerID, playerID);
		int r = ggpo_add_player(g_ggpo, &player, &phandle);
		if (r != 0) {
			return r;
		}
		if (g_playerID == playerID){
			ggpo_set_frame_delay(g_ggpo, phandle, 2);
		}
		return r;
	}
	int UggDisconnectPlayer(int phandle)
	{
		return ggpo_disconnect_player(g_ggpo, phandle);
	}

	int UggSetFrameDelay(int phandle, int frame_delay)
	{
		return ggpo_set_frame_delay(g_ggpo, phandle, frame_delay);
	}

	int UggAdvanceFrame()
	{
		return ggpo_advance_frame(g_ggpo);
	}

	void UggLog(const char* text)
	{
		ggpo_log(g_ggpo, text);
	}

	int UggGetNetworkStats(int phandle,
		int& send_queue_len,
		int& recv_queue_len,
		int& ping,
		int& kbps_sent,
		int& local_frames_behind,
		int& remote_frames_behind)
	{
		GGPONetworkStats stats;
		auto result = ggpo_get_network_stats(g_ggpo, phandle, &stats);
		send_queue_len = stats.network.send_queue_len;
		recv_queue_len = stats.network.recv_queue_len;
		ping = stats.network.ping;
		kbps_sent = stats.network.kbps_sent;
		local_frames_behind = stats.timesync.local_frames_behind;
		remote_frames_behind = stats.timesync.remote_frames_behind;
		return result;
	}
}

