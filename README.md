# OxygenController
SAMP Oxygen Controller

Callbacks List;

			OnPlayerEnterInWater(playerid);
			OnPlayerEnterUnderWater(playerid);
			OnPlayerOutFromWater(playerid);


			
			Functions List;

			IsPlayerInWater(playerid);
			IsPlayerUnderWater(playerid);
			SetPlayerOxygenValue(playerid, Float:Value);
			GetPlayerOxygenValue(playerid);
			SetPlayerMaxOxygenValue(playerid, Float:Value);
			GetPlayerMaxOxygenValue(playerid);
			GivePlayerOxygenValue(playerid, Float:Value);
			GetPlayerOxygenStatus(playerid);



			Status Definitions;
			OXYGEN_NONE
			OXYGEN_SPENDING
			OXYGEN_REFILL
