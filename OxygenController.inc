/*
	__________________________________

	        Oxygen Controller      

	   Real SAMP Oxygen Controller
	__________________________________


	
	***** -> Created by <- *****
		Ino

	***** -> Credits <- *****	
		ColAndreas Team			(ColAndreas plugin)
		Slice  					(SKY plugin)
		Toribio / Southclaw 	(Progress Bar include)

	***** -> Version <- ***** 	
		v1.1 - beta

	***** -> Build <- *****		
		3

	***** -> Released  Date <- ***** 		
			   02/05/2016


			
	***** -> Changes <- *****

		* -> v1.0
		     Release

		* -> v1.1
		     Fixed bugs with health bar
		     Support last CA version




	***** -> Callbacks <- *****

		OnPlayerEnterInWater(playerid);
		OnPlayerEnterUnderWater(playerid);
		OnPlayerOutFromWater(playerid);


			
	***** -> Functions <- *****

		IsPlayerInWater(playerid);
		IsPlayerUnderWater(playerid);
		SetPlayerOxygenValue(playerid, Float:Value);
		GetPlayerOxygenValue(playerid);
		SetPlayerMaxOxygenValue(playerid, Float:Value);
		GetPlayerMaxOxygenValue(playerid);
		GivePlayerOxygenValue(playerid, Float:Value);
		GetPlayerOxygenStatus(playerid);



	***** -> Status Definitions <- *****
			
		OXYGEN_NONE
		OXYGEN_SPENDING
		OXYGEN_REFILL

*/


#include <a_samp>

#if defined _INC_OXYGEN
	#endinput
#endif
#define _INC_OXYGEN

#if !defined _INC_SKY
	#tryinclude <SKY>

	#if !defined _INC_SKY
		#error The SKY plugin is required, get it here: github.com/oscar-broman/sky
	#endif
#endif

#if !defined COLANDREAS
	#tryinclude <colandreas>

	#if !defined COLANDREAS
		#error The ColAndreas plugin is required, get it here: github.com/Pottus/ColAndreas
	#endif
#endif

#if !defined _progress2_included
	#tryinclude <progress2>

	#if !defined _progress2_included
		#error The Progress include is required, get it here: forum.sa-mp.com/showthread.php?t=537468
	#endif
#endif


// Defines
#define MAX_OXYGEN      (5000.00)
#define MIN_OXYGEN      (100.00)

// Defined status
#define OXYGEN_NONE     (0000)
#define OXYGEN_SPENDING (0001)
#define OXYGEN_REFILL   (0002)

// Debug (in progress)
//#define PRINT_DEBUG

// Variables

// Textdraws
static 
	PlayerBar:s_hpbar[MAX_PLAYERS],
	PlayerText:s_hpbcg[MAX_PLAYERS],
	PlayerBar:s_bar[MAX_PLAYERS],
	PlayerText:s_barbcg[MAX_PLAYERS];

// Floats / Bools
static 
	Float:s_max[MAX_PLAYERS],
	Float:s_hp[MAX_PLAYERS],
	Float:s_rememberhp[MAX_PLAYERS],
	bool:s_empty[MAX_PLAYERS],
	bool:s_callin[MAX_PLAYERS],
	bool:s_callun[MAX_PLAYERS],
	bool:s_callout[MAX_PLAYERS],
	bool:s_death[MAX_PLAYERS];

static s_status[MAX_PLAYERS];


// Callbacks
forward OnPlayerEnterInWater(playerid);
forward OnPlayerEnterUnderWater(playerid);
forward OnPlayerOutFromWater(playerid);


// Hooked Callbacks
public OnFilterScriptInit()
{
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		OxygenInit(i);
	}

	SetTimer("_oxygen_timer", 200, true);

	print("loaded from filterscript");

	#if defined oc_OnFilterScriptInit
		return oc_OnFilterScriptInit();
	#else
		return (true);
	#endif
}



public OnGameModeInit()
{
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		OxygenInit(i);
	}

	SetTimer("_oxygen_timer", 200, true);

	print("loaded from gamemode");

	#if defined oc_OnGameModeInit
		return oc_OnGameModeInit();
	#else
		return (true);
	#endif
}



public OnFilterScriptExit()
{
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		OxygenExit(i);
	}

	print("unloaded from filterscript");

	#if defined oc_OnFilterScriptExit
		return oc_OnFilterScriptExit();
	#else
		return (true);
	#endif
}



public OnGameModeExit()
{
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		OxygenExit(i);
	}

	print("unloaded from gamemode");

	#if defined oc_OnGameModeExit
		return oc_OnGameModeExit();
	#else
		return (true);
	#endif
}



public OnPlayerConnect(playerid)
{

	OxygenInit(playerid);


	#if defined oc_OnPlayerConnect
		return oc_OnPlayerConnect(playerid);
	#else
		return (true);
	#endif
}



public OnPlayerDisconnect(playerid, reason)
{

	OxygenExit(playerid);


	#if defined oc_OnPlayerDisconnect
		return oc_OnPlayerDisconnect(playerid, reason);
	#else
		return (true);
	#endif
}



public OnPlayerUpdate(playerid)
{
	if (!s_callin[playerid])
	{
		if (IsPlayerInWater(playerid))
		{
			CallLocalFunction("OnPlayerEnterInWater", "i", playerid);
			s_callin[playerid] = true;
			s_callout[playerid] = false;
			s_callun[playerid] = false;
		}
	}

	if (!s_callun[playerid])
	{
		if (IsPlayerUnderWater(playerid))
		{
			CallLocalFunction("OnPlayerEnterUnderWater", "i", playerid);
			s_callun[playerid] = true;
			s_callout[playerid] = false;
			s_callin[playerid] = false;
		}
	}

	if (!s_callout[playerid])
	{
		if (!IsPlayerUnderWater(playerid) && !IsPlayerInWater(playerid))
		{
			CallLocalFunction("OnPlayerOutFromWater", "i", playerid);
			s_callout[playerid] = true;
			s_callun[playerid] = false;
			s_callin[playerid] = false;
		}
	}


	#if defined oc_OnPlayerUpdate
		return oc_OnPlayerUpdate(playerid);
	#else
		return (true);
	#endif
}


public OnPlayerSpawn(playerid)
{
	SetPlayerHealth(playerid, 800000.0);
	s_hp[playerid] = 100.0;
	s_rememberhp[playerid] = 100.0;
	ShowPlayerProgressBar(playerid, s_hpbar[playerid]);
  	PlayerTextDrawShow(playerid, s_hpbcg[playerid]);
	UpdatePlayerHealth(playerid);	

	s_empty[playerid] = false;
	SetPlayerProgressBarMaxValue(playerid, s_bar[playerid], s_max[playerid]);
  	SetPlayerProgressBarValue(playerid, s_bar[playerid], s_max[playerid]);
  	HidePlayerProgressBar(playerid, s_bar[playerid]);
  	s_status[playerid] = OXYGEN_NONE;

  	s_death[playerid] = false;


	#if defined oc_OnPlayerSpawn
		return oc_OnPlayerSpawn(playerid);
	#else
		return (true);
	#endif
}


public OnPlayerDeath(playerid, killerid, reason)
{
	s_hp[playerid] = 0.0;
	UpdatePlayerHealth(playerid);
	s_death[playerid] = true;

	#if defined oc_OnPlayerDeath
		return oc_OnPlayerDeath(playerid, killerid, reason);
	#else
		return (true);
	#endif
}


public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid)
{
	if (s_callun[playerid] && !s_empty[playerid])
	{
		if (issuerid != INVALID_PLAYER_ID)
		{
			s_rememberhp[playerid] -= amount;
		}

		oc_SetPlayerHealth(playerid, 8000000.0);
		s_hp[playerid] = s_rememberhp[playerid];
		UpdatePlayerHealth(playerid);
	}

	if (!s_empty[playerid] && s_callout[playerid])
	{
		if (issuerid == INVALID_PLAYER_ID)
		{
			s_rememberhp[playerid] -= amount;
			s_hp[playerid] = s_rememberhp[playerid];
			UpdatePlayerHealth(playerid);
		}
	}

	if (s_callun[playerid] && s_empty[playerid])
	{
		SetPlayerHealth(playerid, s_hp[playerid]);
	}

	#if defined oc_OnPlayerTakeDamage
		return oc_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid);
	#else
		return (true);
	#endif
}

// Hooked Functions

stock oc_GetPlayerHealth(playerid, &Float:health)
{
	health = s_hp[playerid];
	return (true);
}


stock oc_SetPlayerHealth(playerid, Float:amount)
{
  	s_hp[playerid] = amount;
  	UpdatePlayerHealth(playerid);
  	SetPlayerHealth(playerid, amount);
   	return (true);
}


// Oxygen Main Timer
forward _oxygen_timer();
public _oxygen_timer()
{
	
	for(new i; i < MAX_PLAYERS; i++)
	{
		if (s_callun[i])
		{
			SetPlayerProgressBarValue(i, s_bar[i], (GetPlayerProgressBarValue(i, s_bar[i])-0.50));
			PlayerTextDrawShow(i, s_barbcg[i]);
			s_status[i] = OXYGEN_SPENDING;
		}

		if (!s_callun[i] && GetPlayerProgressBarValue(i, s_bar[i]) < GetPlayerProgressBarMaxValue(i, s_bar[i]))
		{
			SetPlayerProgressBarValue(i, s_bar[i], (GetPlayerProgressBarValue(i, s_bar[i])+0.40));
		    PlayerTextDrawShow(i, s_barbcg[i]);
		    s_status[i] = OXYGEN_REFILL;
		}

		if (GetPlayerProgressBarValue(i, s_bar[i]) >= GetPlayerProgressBarMaxValue(i, s_bar[i]))
		{
			HidePlayerProgressBar(i, s_bar[i]);
			PlayerTextDrawHide(i, s_barbcg[i]);
			s_status[i] = OXYGEN_NONE;
		}

		if (GetPlayerProgressBarValue(i, s_bar[i]) <= 0.00)
		{
			if (!s_death[i])
			{
				s_empty[i] = true;
				SetPlayerHealth(i, 0.0);
				UpdatePlayerHealth(i);
			}
		}
		else
		{
		    s_empty[i] = false;
		}

	}
	return (true);
}


// Functions for Include

static OxygenInit(playerid)
{
	s_barbcg[playerid] = CreatePlayerTextDraw(playerid,609.000000, 58.000000, "_");
	PlayerTextDrawBackgroundColor(playerid,s_barbcg[playerid], 255);
	PlayerTextDrawFont(playerid,s_barbcg[playerid], 1);
	PlayerTextDrawLetterSize(playerid,s_barbcg[playerid], 0.500000, 0.599999);
	PlayerTextDrawColor(playerid,s_barbcg[playerid], -1);
	PlayerTextDrawSetOutline(playerid,s_barbcg[playerid], 0);
	PlayerTextDrawSetProportional(playerid,s_barbcg[playerid], 1);
	PlayerTextDrawSetShadow(playerid,s_barbcg[playerid], 1);
	PlayerTextDrawUseBox(playerid,s_barbcg[playerid], 1);
	PlayerTextDrawBoxColor(playerid,s_barbcg[playerid], 255);
	PlayerTextDrawTextSize(playerid,s_barbcg[playerid], 544.500000, 1.000000);
	PlayerTextDrawSetSelectable(playerid,s_barbcg[playerid], 0);

	s_hpbcg[playerid] = CreatePlayerTextDraw(playerid,609.000000, 68.550000, "_");
	PlayerTextDrawBackgroundColor(playerid,s_hpbcg[playerid], 255);
	PlayerTextDrawFont(playerid,s_hpbcg[playerid], 1);
	PlayerTextDrawLetterSize(playerid,s_hpbcg[playerid], 0.500000, 0.599999);
	PlayerTextDrawColor(playerid,s_hpbcg[playerid], -1);
	PlayerTextDrawSetOutline(playerid,s_hpbcg[playerid], 0);
	PlayerTextDrawSetProportional(playerid,s_hpbcg[playerid], 1);
	PlayerTextDrawSetShadow(playerid,s_hpbcg[playerid], 1);
	PlayerTextDrawUseBox(playerid,s_hpbcg[playerid], 1);
	PlayerTextDrawBoxColor(playerid,s_hpbcg[playerid], 255);
	PlayerTextDrawTextSize(playerid,s_hpbcg[playerid], 544.500000, 1.000000);
	PlayerTextDrawSetSelectable(playerid,s_hpbcg[playerid], 0);


	DestroyPlayerProgressBar(playerid, s_hpbar[playerid]);
	DestroyPlayerProgressBar(playerid, s_bar[playerid]);


    s_bar[playerid] = CreatePlayerProgressBar(playerid, 548.50, 58.00, 61.00, 4.80, 2899046911, 100.0, BAR_DIRECTION_RIGHT);
    SetPlayerProgressBarValue(playerid, s_bar[playerid], 100.0);
    s_max[playerid] = 100.0;
	PlayerTextDrawHide(playerid, s_barbcg[playerid]);

	
    s_hpbar[playerid] = CreatePlayerProgressBar(playerid, 548.50, 68.55, 61.00, 4.80, 3021544959, 100.0, BAR_DIRECTION_RIGHT);
    SetPlayerProgressBarValue(playerid, s_hpbar[playerid], 100.0);
	PlayerTextDrawHide(playerid, s_hpbcg[playerid]);


	s_callin[playerid] = false;
	s_callout[playerid] = true;
	s_callun[playerid] = false;

	HidePlayerProgressBar(playerid, s_hpbar[playerid]);
  	PlayerTextDrawHide(playerid, s_hpbcg[playerid]);

  	HidePlayerProgressBar(playerid, s_bar[playerid]);
  	PlayerTextDrawHide(playerid, s_barbcg[playerid]);


  	//

  	SetPlayerHealth(playerid, 800000.0);
	s_hp[playerid] = 100.0;
	s_rememberhp[playerid] = 100.0;
	ShowPlayerProgressBar(playerid, s_hpbar[playerid]);
  	PlayerTextDrawShow(playerid, s_hpbcg[playerid]);
	UpdatePlayerHealth(playerid);	

	s_empty[playerid] = false;
	SetPlayerProgressBarMaxValue(playerid, s_bar[playerid], s_max[playerid]);
  	SetPlayerProgressBarValue(playerid, s_bar[playerid], s_max[playerid]);
  	HidePlayerProgressBar(playerid, s_bar[playerid]);
  	s_status[playerid] = OXYGEN_NONE;
	return (true);
}

static OxygenExit(playerid)
{
	PlayerTextDrawDestroy(playerid, s_hpbcg[playerid]);
	PlayerTextDrawDestroy(playerid, s_barbcg[playerid]);
	DestroyPlayerProgressBar(playerid, s_hpbar[playerid]);
	DestroyPlayerProgressBar(playerid, s_bar[playerid]);
	return (true);
}

static UpdatePlayerHealth(playerid)
{
	SetFakeHealth(playerid, (floatround(s_hp[playerid], floatround_ceil)));
	UpdatePlayerSyncData(playerid);

	if (s_hp[playerid] <= 0.0)
	{
		SetPlayerHealth(playerid, 0.0);
		SetPlayerProgressBarValue(playerid, s_hpbar[playerid], s_hp[playerid]);
	}
	
	if (!s_empty[playerid])
	{
		SetPlayerProgressBarValue(playerid, s_hpbar[playerid], s_hp[playerid]);
	}
	else
	{
		HidePlayerProgressBar(playerid, s_hpbar[playerid]);
  		PlayerTextDrawHide(playerid, s_hpbcg[playerid]);
	}
	return (true);
}

// by Slice
static UpdatePlayerSyncData(playerid)
{
	// Currently re-sending onfoot data is only supported
	if (!IsPlayerConnected(playerid) || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) {
		return;
	}

	for (new i = 0; i < MAX_PLAYERS; i++) {
		if (i != playerid && IsPlayerConnected(i) && IsPlayerStreamedIn(playerid, i)) {
			SendLastSyncData(playerid, i);
		}
	}
}



// Available Functions
stock IsPlayerInWater(playerid)
{
	new Float:x, Float:y, Float:z, Float:retx[2], Float:rety[2], Float:retz[2], Float: retdist[2], modelids[2];
	GetPlayerPos(playerid, x, y, z);
	new collisions = CA_RayCastMultiLine(x, y, z+0.7, x, y, z-0.7, retx, rety, retz, retdist, modelids, 2);

	if (collisions)
	{
		for(new i = 0; i < collisions; i++)
		{
		    if (modelids[i] == WATER_OBJECT)
		    {
		    	return (true);
		    }
		}
	}
	return (false);
}


stock IsPlayerUnderWater(playerid)
{
	new Float:x, Float:y, Float:z, Float:retx[10], Float:rety[10], Float:retz[10], Float: retdist[10], modelids[10];
	GetPlayerPos(playerid, x, y, z);
	new collisions = CA_RayCastMultiLine(x, y, z+0.7, x, y, z+1000.0, retx, rety, retz, retdist, modelids, 10);

	if (collisions)
	{
		for(new i = 0; i < collisions; i++)
		{
		    if (modelids[i] == WATER_OBJECT)
		    {
		    	return (true);
		    }
		}
	}
	return (false);
}

stock GetPlayerOxygenValue(playerid)
{
    new Float:Value = GetPlayerProgressBarValue(playerid, s_bar[playerid]);
	return _:Value;
}

stock SetPlayerOxygenValue(playerid, Float:Value)
{
	if (Value < MIN_OXYGEN)
		return (false);

	if (Value > s_max[playerid])
	{
	    SetPlayerProgressBarValue(playerid, s_bar[playerid], s_max[playerid]);
	}
	else
	{
        SetPlayerProgressBarValue(playerid, s_bar[playerid], Value);
	}
	return (true);
}

stock GetPlayerMaxOxygenValue(playerid)
{
    new Float:Value = s_max[playerid];
	return _:Value;
}

stock SetPlayerMaxOxygenValue(playerid, Float:Value)
{
	if (Value < MIN_OXYGEN)
		return (false);

	if (Value > MAX_OXYGEN)
	{
	    SetPlayerProgressBarMaxValue(playerid, s_bar[playerid], MAX_OXYGEN);
    	s_max[playerid] = GetPlayerProgressBarMaxValue(playerid, s_bar[playerid]);
	}
	else
	{
        SetPlayerProgressBarMaxValue(playerid, s_bar[playerid], Value);
    	s_max[playerid] = GetPlayerProgressBarMaxValue(playerid, s_bar[playerid]);
	}
	return (true);
}

stock GivePlayerOxygenValue(playerid, Float:Value)
{
	if (Value < 1.0)
		return (false);

	if ((GetPlayerProgressBarValue(playerid, s_bar[playerid])+Value) > s_max[playerid])
	{
	    SetPlayerProgressBarValue(playerid, s_bar[playerid], s_max[playerid]);
	}
	else
	{
	    SetPlayerProgressBarValue(playerid, s_bar[playerid], (GetPlayerProgressBarValue(playerid, s_bar[playerid])+Value));
	}
	return (true);
}

stock GetPlayerOxygenStatus(playerid)
{
	return s_status[playerid];
}


// ALS callbacks


// OnPlayerTakeDamage
#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif
 
#define OnPlayerTakeDamage oc_OnPlayerTakeDamage
#if defined oc_OnPlayerTakeDamage
	forward oc_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid);
#endif



// OnPlayerUpdate
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
 
#define OnPlayerUpdate oc_OnPlayerUpdate
#if defined oc_OnPlayerUpdate
	forward oc_OnPlayerUpdate(playerid);
#endif



// OnPlayerDeath
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif
 
#define OnPlayerDeath oc_OnPlayerDeath
#if defined oc_OnPlayerDeath
	forward oc_OnPlayerDeath(playerid, killerid, reason);
#endif



// OnPlayerSpawn
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
 
#define OnPlayerSpawn oc_OnPlayerSpawn
#if defined oc_OnPlayerSpawn
	forward oc_OnPlayerSpawn(playerid);
#endif



// OnPlayerDisconnect
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
 
#define OnPlayerDisconnect oc_OnPlayerDisconnect
#if defined oc_OnPlayerDisconnect
	forward oc_OnPlayerDisconnect(playerid, reason);
#endif



// OnPlayerConnect
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
 
#define OnPlayerConnect oc_OnPlayerConnect
#if defined oc_OnPlayerConnect
	forward oc_OnPlayerConnect(playerid);
#endif



// OnGameModeExit
#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
 
#define OnGameModeExit oc_OnGameModeExit
#if defined oc_OnGameModeExit
	forward oc_OnGameModeExit();
#endif



// OnFilterScriptExit
#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif
 
#define OnFilterScriptExit oc_OnFilterScriptExit
#if defined oc_OnFilterScriptExit
	forward oc_OnFilterScriptExit();
#endif



// OnGameModeInit
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
 
#define OnGameModeInit oc_OnGameModeInit
#if defined oc_OnGameModeInit
	forward oc_OnGameModeInit();
#endif



// OnFilterScriptInit
#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif
 
#define OnFilterScriptInit oc_OnFilterScriptInit
#if defined oc_OnFilterScriptInit
	forward oc_OnFilterScriptInit();
#endif


// --------------------------------------------
// ALS Functions


// GetPlayerHealth
#if defined _ALoc_GetPlayerHealth
		#undef GetPlayerHealth
#else
		#define _ALoc_GetPlayerHealth
#endif
#define GetPlayerHealth oc_GetPlayerHealth



// SetPlayerHealth
#if defined _ALS_SetPlayerHealth
	#undef SetPlayerHealth
#else
#define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth oc_SetPlayerHealth