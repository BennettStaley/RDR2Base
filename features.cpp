#include "stdafx.h"

namespace features {

	int onlineSelectedPlayer = 0;

	bool isGodModeEnabled = false;
	bool isInfiniteAmmoEnabled = false;

	void GodMode() {
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), isGodModeEnabled);
	}

	void WPTP() {
		Vector3 waypoint = MAP::_GET_WAYPOINT_COORDS();
		cordtp(waypoint.x, waypoint.y);
	}

	void cordtp(float x, float y) {
		float px = x;
		float py = y;
		Entity ent = PLAYER::PLAYER_PED_ID(); // get entity to teleport
		if (PED::IS_PED_ON_MOUNT(ent)) // is ped on horse?
			ent = PED::GET_MOUNT(ent);


		ENTITY::SET_ENTITY_COORDS(ent, px, py, 0, 0, 0, 0, 1);
		float ground = 300;
		float heightArray[] = { 100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0 };
		for (float height : heightArray) {
			ENTITY::SET_ENTITY_COORDS(ent, px, py, height, 0, 0, 0, 1);
			wait_for(200);
			bool isSafe = MISC::GET_GROUND_Z_AND_NORMAL_FOR_3D_COORD(px, py, height, &ground, new Vector3);
			if (isSafe) {
				ENTITY::SET_ENTITY_COORDS(ent, px, py, ground, 0, 0, 0, 1);
				break;
			}
		}
	}

	void SpawnPed() {
		PED::CLONE_PED(PLAYER::PLAYER_PED_ID(), ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 0, 1);
	}

	void InfiniteAmmo() {
		WEAPON::SET_PED_INFINITE_AMMO(PLAYER::PLAYER_PED_ID(), isInfiniteAmmoEnabled, 0);
	}
}
