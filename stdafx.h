#pragma once
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <windows.h>
#include <stdio.h>
#include <string>
#include <Psapi.h>
#include <map>

#include "MinHook/MinHook.h"
#include "simpleini/SimpleIni.h"
#pragma comment(lib, "MinHook/libMinHook.x64.lib")

#include "crossmap.h"
#include "invoker.h"
#include "natives.h"
#include "globals.h"
#include "hooks.h"
#include "memory.h"
#include "features.h"
#include "menukeyboard.h"
#include "menusettings.h"
#include "menumemutils.hpp"
#include "menuutils.h"
#include "mainmenu.h"

struct rgba {
    int r, g, b, a;
};