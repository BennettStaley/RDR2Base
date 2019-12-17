#pragma once
#define DISPLAY_VERSION "v1.0.0"
#include "menu.h"

const std::string modDir = "\\RDRMenu";

namespace MainMenu {
	extern NativeMenu::Menu menu;
	extern int textureBgId;
	extern int textureBgId2;
	extern void start_menu();
	extern void update_menu();
	extern void update_game();
};
