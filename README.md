# OxygenController
SAMP Oxygen Controller



- **How to use**
	```pawn
	#include <OxygenController>
	```


- **Callbacks List**
	- OnPlayerEnterInWater(playerid);
 	- OnPlayerEnterUnderWater(playerid);
	- OnPlayerOutFromWater(playerid);


			
- **Functions List**

	- IsPlayerInWater(playerid);
	- IsPlayerUnderWater(playerid);
	- SetPlayerOxygenValue(playerid, Float:Value);
	- GetPlayerOxygenValue(playerid);
	- SetPlayerMaxOxygenValue(playerid, Float:Value);
	- GetPlayerMaxOxygenValue(playerid);
	- GivePlayerOxygenValue(playerid, Float:Value);
	- GetPlayerOxygenStatus(playerid);



- **Status Definitions**
	- OXYGEN_NONE
	- OXYGEN_SPENDING
	- OXYGEN_REFILL


- **Requiments**
	- ProgressBar include ( http://forum.sa-mp.com/showthread.php?t=537468 )
 	- SKY plugin ( http://github.com/oscar-broman/sky )
	- ColAndreas plugin ( http://github.com/Pottus/ColAndreas )
