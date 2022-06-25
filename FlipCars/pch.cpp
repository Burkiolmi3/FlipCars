#include "pch.h"
#include <Windows.h>
#include <winbase.h>


bool CanWeUse(Entity entity)
{
	return entity != NULL && ENTITY::DOES_ENTITY_EXIST(entity);
}

void update()
{
	char szKeyValue[32];
   

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	int Key = GetPrivateProfileInt(L"Keys", L"Key", 0x71, L"./FlipCars.ini");
	bool disablemidair = GetPrivateProfileInt(L"Options", L"Mid-Air AutoCorrection disabled", false, L"./FlipCars.ini");

	if (IsKeyDown(Key))
	{
		Vector3 pedcoords = ENTITY::GET_ENTITY_COORDS(playerPed, true);
		Vehicle veh = VEHICLE::GET_CLOSEST_VEHICLE(pedcoords.x, pedcoords.y, pedcoords.z, 5.0f, 0, 70);

		Vector3 vehcoords = ENTITY::GET_ENTITY_COORDS(veh, true);
		float dist = (MISC::GET_DISTANCE_BETWEEN_COORDS(pedcoords.x, pedcoords.y, pedcoords.z, vehcoords.x, vehcoords.y, vehcoords.z, true));

		if (dist <= 3 && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
		{ 
			
			STREAMING::REQUEST_ANIM_DICT("missfinale_c2ig_11");
			while (!STREAMING::HAS_ANIM_DICT_LOADED("missfinale_c2ig_11"))
			{
				WAIT(10);
			}
			TASK::TASK_PLAY_ANIM(playerPed, "missfinale_c2ig_11", "pushcar_offcliff_m", 2.0, -8.0, -1, 35, 0, 0, 0, 0);
			WAIT(3 * 1000);
			Vector3 carCoords = ENTITY::GET_ENTITY_ROTATION(veh, 2);

			ENTITY::SET_ENTITY_ROTATION(veh, carCoords.y, carCoords.x, carCoords.z, 1, true);
			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 5.0f);
			TASK::CLEAR_PED_TASKS(playerPed);
		}
		else 
		{
			HUD::BEGIN_TEXT_COMMAND_PRINT("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Vehicle is too far away");
			HUD::END_TEXT_COMMAND_PRINT(4000, 1);
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed,true)) 
		{
			HUD::BEGIN_TEXT_COMMAND_PRINT("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("You have to be on foot to flip your vehicle");
			HUD::END_TEXT_COMMAND_PRINT(4000, 1);
		}
		

		
	}

	if (disablemidair == true) 
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		if (CanWeUse(veh) && VEHICLE::IS_THIS_MODEL_A_CAR(veh))
		{
			VEHICLE::SET_VEHICLE_OUT_OF_CONTROL(veh, false, false);
		}
	}


}




void main()
{
	while (true)
	{
		update();
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
