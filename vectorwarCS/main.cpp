#include <windows.h>
#include <stdio.h>
#if defined(_DEBUG)
#   include <crtdbg.h>
#endif
#include "vectorwar.h"
#include "ggpo_perfmon.h"
#include <nicenet/nicenet.h>

#ifndef MAX_INT
#  define MAX_INT          0xEFFFFFF
#endif

#ifndef MAX
#  define MAX(x, y)        (((x) > (y)) ? (x) : (y))
#endif

#ifndef MIN
#  define MIN(x, y)        (((x) < (y)) ? (x) : (y))
#endif

LRESULT CALLBACK
MainWindowProc(HWND hwnd,
               UINT uMsg,
               WPARAM wParam,
               LPARAM lParam)
{
   switch (uMsg) {
   case WM_ERASEBKGND:
      return 1;
   case WM_KEYDOWN:
      if (wParam == 'P') {
         ggpoutil_perfmon_toggle();
      } else if (wParam == VK_ESCAPE) {
         VectorWar_Exit();
		 PostQuitMessage(0);
      } else if (wParam >= VK_F1 && wParam <= VK_F12) {
         VectorWar_DisconnectPlayer((int)(wParam - VK_F1));
      }
      return 0;
   case WM_PAINT:
      VectorWar_DrawCurrentFrame();
      ValidateRect(hwnd, NULL);
      return 0;
   case WM_CLOSE:
      PostQuitMessage(0);
      break;
   }
   return CallWindowProc(DefWindowProc, hwnd, uMsg, wParam, lParam);
}

HWND
CreateMainWindow(HINSTANCE hInstance)
{
   HWND hwnd;
   WNDCLASSEXW wndclass = { 0 };
   RECT rc;
   int width = 640, height = 480;
   WCHAR titlebuf[128];

   wsprintfW(titlebuf, L"(pid:%d) ggpo sdk sample: vector war", GetCurrentProcessId());
   wndclass.cbSize = sizeof(wndclass);
   wndclass.lpfnWndProc = MainWindowProc;
   wndclass.lpszClassName = L"vwwnd";
   RegisterClassExW(&wndclass);
   hwnd = CreateWindowW(L"vwwnd",
                       titlebuf,
                       WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                       CW_USEDEFAULT, CW_USEDEFAULT,
                       width, height,
                       NULL, NULL, hInstance, NULL);

   GetClientRect(hwnd, &rc);
   SetWindowPos(hwnd, NULL, 0, 0, width + (width - (rc.right - rc.left)), height + (height - (rc.bottom - rc.top)), SWP_NOMOVE);
   return hwnd;
}

void
RunMainLoop(HWND hwnd)
{
   MSG msg = { 0 };
   int start, next, now;

   start = next = now = timeGetTime();
   while(1) {
      while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
         TranslateMessage(&msg); 
         DispatchMessage(&msg);
         if (msg.message == WM_QUIT) {
            return;
         }
      }
      now = timeGetTime();
      VectorWar_Idle(MAX(0, next - now - 1));
      if (now >= next) {
         VectorWar_RunFrame(hwnd);
         next = now + (1000 / 60);
      }
   }
}

void
Syntax()
{
   MessageBoxW(NULL, 
              L"Syntax:: vectorwar.exe <playerID> <num players> <serverIP:port> <room> ('myPlayerID' | otherPlayerID)*\n",
              L"Could not start", MB_OK);
}
#include <fcntl.h>
#include <io.h>
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPSTR,
	_In_ int)
{
	HWND hwnd = CreateMainWindow(hInstance);
	int offset = 1, local_player = 0;
	WSADATA wd = { 0 };
	wchar_t wide_ip_buffer[128];
	unsigned int wide_ip_buffer_size = (unsigned int)ARRAYSIZE(wide_ip_buffer);
	WSAStartup(MAKEWORD(2, 2), &wd);
	POINT window_offsets[] = {
		{ 64,  64 },   // player 1
		{ 740, 64 },   // player 2
		{ 64,  600 },  // player 3
		{ 740, 600 },  // player 4
	};
	nicehero::start(true);
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	if (__argc < 5) {
		Syntax();
		return 1;
	}

	std::string playerID = __argv[offset++];
	if (playerID == "")
	{
		Syntax();
		return 1;
	}
	int num_players = atoi(__argv[offset++]);
	if (num_players < 0 || __argc < offset + num_players) {
		Syntax();
		return 1;
	}
	char serverIP[32] = "";
	unsigned short serverPort = 0;
	if (sscanf(__argv[offset++], "%[^:]:%hu", serverIP, &serverPort) != 2) {
		Syntax();
		return 1;
	}
	std::string room = __argv[offset++];
	if (room == "")
	{
		Syntax();
		return 1;
	}

	if (strcmp(__argv[offset], "spectate") == 0) {
		char host_ip[128];
		unsigned short host_port;
		if (sscanf(__argv[offset+1], "%[^:]:%hu", host_ip, &host_port) != 2) {
			Syntax();
			return 1;
		}
		//wcstombs(host_ip, wide_ip_buffer, _TRUNCATE);
		//VectorWar_InitSpectator(hwnd, local_port, num_players, host_ip, host_port);
	}
	else {
		GGPOPlayer players[GGPO_MAX_SPECTATORS + GGPO_MAX_PLAYERS];
		int i;
		for (i = 0; i < num_players; i++) {
			const char *arg = __argv[offset++];

			players[i].size = sizeof(players[i]);
			players[i].player_num = i + 1;
			if (!strcmp(arg, playerID.c_str())) {
				players[i].type = GGPO_PLAYERTYPE_LOCAL;
				strcpy(players[i].u.csremote.playerID, playerID.c_str());
				local_player = i;
				MessageBoxW(NULL,L"Local\n",L"Local", MB_OK);
				continue;
			}

			players[i].type = GGPO_PLAYERTYPE_REMOTE;
			strcpy(players[i].u.csremote.playerID, arg);
			MessageBoxA(NULL,players[i].u.csremote.playerID,"REMOTE", MB_OK);
		}

		// these are spectators...
		int num_spectators = 0;
		while (offset < __argc) {
			players[i].type = GGPO_PLAYERTYPE_SPECTATOR;
			strcpy(players[i].u.csremote.playerID, __argv[offset++]);
			i++;
			num_spectators++;
		}
		if (local_player < sizeof(window_offsets) / sizeof(window_offsets[0])) {
			::SetWindowPos(hwnd, NULL, window_offsets[local_player].x, window_offsets[local_player].y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
		VectorWar_Init(hwnd, playerID,serverIP,serverPort,room, num_players, players, num_spectators);
	}
	RunMainLoop(hwnd);
	VectorWar_Exit();
	WSACleanup();
	DestroyWindow(hwnd);
	nicehero::stop();
	nicehero::joinMain();
	return 0;
}