#include <a_samp>
#include <OxygenController>
#include <zcmd>

// Variables
new bool:oxstatus[MAX_PLAYERS];



// Callbacks

// Main
public OnFilterScriptInit()
{
	SetTimer("ostatus", 200, true);
	return (true);
}

// Oxygen
public OnPlayerEnterInWater(playerid)
{
	SendClientMessage(playerid, -1, "WATER: Player enter in water");
	return (true);
}

public OnPlayerEnterUnderWater(playerid)
{
	SendClientMessage(playerid, -1, "WATER: Player enter under water");
	return (true);
}

public OnPlayerOutFromWater(playerid)
{
	SendClientMessage(playerid, -1, "WATER: Player out from water");
	return (true);
}

// Timer
forward ostatus();
public ostatus()
{


	new 
		tdstring[128];

	for(new i; i < MAX_PLAYERS; i++)
	{
	    if(oxstatus[i])
	    {
	    	switch (GetPlayerOxygenStatus(i))
	    	{
	    		case OXYGEN_NONE:
	    		{	    			
	    			format (tdstring, sizeof(tdstring), "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~oxygen~n~~g~~h~~h~full~n~~w~status: %.2f/%.2f", GetPlayerOxygenValue(i), GetPlayerMaxOxygenValue(i));
	    			GameTextForPlayer(i, tdstring, 1000, 3);
	    		}
	    		case OXYGEN_REFILL:
	    		{
	    			format (tdstring, sizeof(tdstring), "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~oxygen~n~~b~~h~~h~refill~n~~w~status: %.2f/%.2f", GetPlayerOxygenValue(i), GetPlayerMaxOxygenValue(i));
	    			GameTextForPlayer(i, tdstring, 1000, 3);
	    		}
	    		case OXYGEN_SPENDING:
	    		{
	    			format (tdstring, sizeof(tdstring), "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~oxygen~n~~r~~h~~h~spending~n~~w~status: %.2f/%.2f", GetPlayerOxygenValue(i), GetPlayerMaxOxygenValue(i));
	    			GameTextForPlayer(i, tdstring, 1000, 3);
	    		}
	    	}
		}
	}
	return (true);
}


// Commands

CMD:status(playerid, params[])
{
	oxstatus[playerid] = !oxstatus[playerid];
	return (true);
}

CMD:max100(playerid, params[])
{
	SetPlayerMaxOxygenValue(playerid, 100.0);
	return (true);
}

CMD:max500(playerid, params[])
{
	SetPlayerMaxOxygenValue(playerid, 500.0);
	return (true);
}

CMD:max1000(playerid, params[])
{
	SetPlayerMaxOxygenValue(playerid, 1000.0);
	return (true);
}

CMD:give(playerid, params[])
{
	GivePlayerOxygenValue(playerid, 5.0);
	return (true);
}

CMD:full(playerid, params[])
{
	SetPlayerOxygenValue(playerid, GetPlayerMaxOxygenValue(playerid));
	return (true);
}