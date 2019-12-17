#include "stdafx.h"
 // You'll only need to include <menu.h> to start using it,
 // after setting paths up properly.
#include "menu.h"
#include "features.h"

// You can ignore these includes as they're only used to resolve
// paths, log things to a file or to provide nice wrappers for natives.
#include "mainmenu.h"


HANDLE mainFiber;
DWORD wakeAt;

#pragma comment(lib, "winmm.lib")

static HANDLE main_fiber;
static DWORD time_to_wake_up;


void wait_for(DWORD ms) {
	time_to_wake_up = timeGetTime() + ms;
	SwitchToFiber(main_fiber);
}


void __stdcall fiber_thread(LPVOID params) {
	srand(GetTickCount());
	while (true) {
		MainMenu::update_menu();
		MainMenu::update_game();
		wait_for(0);
	}
}

void on_tick() {
	if (!main_fiber)
		main_fiber = ConvertThreadToFiber(nullptr);

	if (time_to_wake_up > timeGetTime())
		return;

	static HANDLE fiber_handle = nullptr;
	if (fiber_handle) {
		SwitchToFiber(fiber_handle);
	}
	else {
		fiber_handle = CreateFiber(NULL, fiber_thread, nullptr);
	}
}

namespace hooks {
	typedef int(__fastcall* get_player_ped_fn)(Player player);

	namespace original
	{
		get_player_ped_fn o_get_player_ped;
	}

	int __fastcall get_player_ped_hook(Player player) {
		on_tick();
		return original::o_get_player_ped(player);
	}

	
	void Init() {
		if (AllocConsole()) {
			freopen("CONIN$", "r", stdin);
			freopen("CONOUT$", "w", stdout);
			freopen("CONOUT$", "w", stderr);
		}

		MainMenu::start_menu();

		globals::baseAddress = uintptr_t(GetModuleHandleA(0));
		auto hwnd_ = FindWindowA(0, "Red Dead Redemption 2");
		GetWindowRect(hwnd_, &globals::resolution);

		auto status = MH_Initialize();
		printf("init_status : %s\n", std::string(MH_StatusToString(status)).c_str());

		auto get_player_ped = memory::find_signature(0, "\x40\x53\x48\x83\xEC\x20\x33\xDB\x81\xF9", "xxxxxxxxxx");
		status = MH_CreateHook((PVOID)get_player_ped, get_player_ped_hook, reinterpret_cast<void**>(&original::o_get_player_ped));
		printf("create_status : %s\n", std::string(MH_StatusToString(status)).c_str());
		status = MH_EnableHook((PVOID)get_player_ped);
		printf("enable_status : %s\n\n", std::string(MH_StatusToString(status)).c_str());
		printf("get_player_ped: %I64X\n", get_player_ped);

		Beep(220, 100);

		do {
			Sleep(100);
			if (GetAsyncKeyState(VK_INSERT) & 0x1) {
				MainMenu::start_menu();
			}
		} while (!(GetAsyncKeyState(VK_F12) & 0x1));

		MH_DisableHook(get_player_ped_hook);

		MH_Uninitialize();

		Beep(220, 100);

		FreeLibraryAndExitThread(globals::hModule, 0);
	
	}
}