#include "StdAfx.h"

/*
Main goals of entertainment :
- masacre lots of mobs
- be challanging
- requires positioning and target selection order
- hide failures and compensate fun parts
- story
- provide the progression feeling to players

Needs to be able to  :
- measure player exitement level
- measure PCT to AI goal achievement
- scale to be competitive
- log statistics to be reviewed by human to improve 

Story :
- name : Suggar Rush endless problem
- FatboySlim is calling out for food types, do not allow him to stuff himself til explosion

How to measure if a mob is good or bad
- mobs should have goal : number of yards walked near other mobs, number of spell casts, number of miles runned, number of players nearby
- goal performance should be able to scale to achieve a close to success
- should have stat ranges, increasing 1 stat drops the other stat in PCT values

how to :
 - measure player DPS : send wave out, measure how much HP they have, measure how fast they die
 - vary difficulty : timer between mobs, number of mobs, mob HB

Player rewards :
- each wave to give a buff for speed / mana cost / cast speed / cooldown ...

Proposed Monster types :
- slow : 
	- name : Greasy Stake
	- subname : Boost Carbohidrates
	- desc : has more HP, walks slower
	- goal : walk as close as possible to target
	- bad performance : dies too fast
	- good performance : reaches at least 80% of the road. Depends on player group position
	- improve : HP + defense
	- cost : 2
- fast 
	- name : Monster Power
	- subname : The real sugga rush
	- desc : has less HP, walks fast
	- goal : walk as close as possible to target
	- bad performance : dies too fast
	- good performance : reaches at least 80% of the road. Depends on player group position
	- improve : HP + defense + speed
- swarm : 
	- name : Skittles	
	- subname : taste the rainbow
	- desc : many but with low HP
	- goal : walk as close as possible to target
	- bad performance : group dies too fast
	- good performance : reaches at least 80% of the road. Depends on player group position
	- improve : HP + defense + number of spawns
- Slow players : 
	- name : The ugly but taste 
	- subname : don't show it to others
	- desc : nearby players get slowed
	- goal : cast spell to at least one of the players
	- bad performance : does not cast enough times
	- good performance : slows enough player
	- improve : HP + defense + slow strength
- fast : 
	- name : protein booster
	- subname : friends with benefits
	- desc : nearby mobs walk faster
	- goal : cast spell to at as many as possible mob types
	- bad performance : does not cast enough times
	- good performance : bosts almost all spawns of the wave
	- improve : HP + defense + walk speed + wait time at waypoints
- give life : 
	- name : Liposucktion
	- subname : Last hope of humanity
	- desc : Adds a life point to the target
	- goal : walk as close as possible to target
	- bad performance : survives to the end
	- good performance : die as fast as possible
	- improve : HP + defense + walk speed + wait time at waypoints
- healer : 
	- name : E combo breaker
	- subname : conservant
	- desc : heals nearby mobs
	- goal : cast heal spell as many as possible targets
	- bad performance : does not heal at all
	- good performance : heals more HP then before
	- improve : HP + defense + walk speed + wait time at waypoints + heal amt
- immune : 
	- name : Chockolate cake
	- subname : can't dodge this
	- desc : gets immune when hit first time
	- goal : get as close as possible
	- bad performance : dies too fast
	- good performance : reaches at least 80% of the road. Depends on player group position
	- improve : walk speed + immune duration
- splitter : 
	- name : pizza
	- subname : take a slice
	- desc : corpse spawns pizza slices
	- goal : get as close as possible
	- bad performance : dies too fast
	- good performance : reaches at least 80% of the road. Depends on player group position
	- improve : HP + defense + number of pizza slices

Phases :
- 1 ) Tune phase : 
	- tune HP to be able to almost reach the goal : player DPS adaptation
	- tune spawn count to be able to almost reach the goal : Player AOE adaptation
	- tune cast interval for healers to almost reach the goal : player DPS adaptation
- 2 ) Combo phase :
	- combine creature types with moderate stats
- 3 ) Player killer phase
	- scale stats until players will be unable to defend themself
*/

#define EDNPCSlowID 1

enum EDModableCreatureStats
{
	SkippThisAsZeroValue ,
	Health ,
	SpawnCount ,
	MoveSpeed ,
	WaypointWait ,
	CastInterval ,
	EffectDuration ,
};

enum EDMobTypes
{
	ED_NPC_SLOW	= 1,
	ED_NPC_FAST,
	ED_NPC_SWARM,
	ED_NPC_IMMUNE,
	ED_NPC_SPLITTER,

	ED_NPC_PLAYERSLOW,
	ED_NPC_NPCSPEED,
	ED_NPC_NPCHEAL,

	ED_NPC_TYPE_COUNT,
	ED_NPC_BONUSLIFE,	//special
	ED_NPC_TYPE_COUNT_WITH_SPECIAL,
};

struct StatPairs
{
	int Stat[2];
};

struct EDMapVariables
{
	int32	PlayerDPSOnMobType[ED_NPC_TYPE_COUNT];				//adjust health so they live long enough
	int32	PlayerDPSOnMobTypeSampleCount[ED_NPC_TYPE_COUNT];
//	int32	PlayerDPSOnLastWave;								//avg DPS
//	int32	LastWaveStartTime;
//	int32	LastWaveEndTime;
//	int32	LastWaveMobTypeCounts[ED_NPC_TYPE_COUNT];
//	int32	CurActiveSpawnCount;
	int32	CurWaveMobTypeCountsRemaining[ED_NPC_TYPE_COUNT];
//	int32	CurWaveMobTypeSpawnDelay[ED_NPC_TYPE_COUNT];
	int32	WaypointAssignIndex;	//mobs should not walk exactly the same road. This is our randomization
};

struct Simple3DCordStore
{
	float x,y,z;
};

#define EDDISTANCE_TO_WALK_TO_REACH_GOAL	500
#define ED_EXTENSION_ID						"Endless Defense Ext"
#define ED_MAP_ID							1

//wave compositions
struct Wave1SpawnComposition
{
	uint32	Entry;
	uint32	SpawnSleep;
	uint32	SpawnLocationIndex;
};

#define POSSIBLE_PREDEFINED_WAVES	2
Wave1SpawnComposition WavesComposition[POSSIBLE_PREDEFINED_WAVES][100] = { { EDNPCSlowID, 500, 0 } };

#define ED_TIME_BETWEEN_WAVES		5000
#define ED_SPAWN_LOCATION_COUNT		1
Simple3DCordStore SpawnLocations[ED_SPAWN_LOCATION_COUNT] = { 0, 0, 0 };

#define POSSIBLE_WAYPOINT_COUNT		2
WayPointVect Waypoints[ED_SPAWN_LOCATION_COUNT][POSSIBLE_WAYPOINT_COUNT];


class EDNPCFightController : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EDNPCFightController);

	uint32	CurrentWaveCount;
	uint32	CurrentWaveProgress;
	uint32	NextSpawnStamp;
	int32	LivesRemaining;

	EDNPCFightController(Creature* pCreature) : CreatureAIScript(pCreature) {}
	void OnLoad()
	{
		if( _unit->GetMapId() != ED_MAP_ID )
			return;
		EDMapVariables *MapVars = new EDMapVariables;
		memset( MapVars, 0, sizeof( MapVars ) ); 
		_unit->GetMapMgr()->SetExtension( ED_EXTENSION_ID, MapVars );			
		CurrentWaveCount = 0;
		CurrentWaveProgress = 0;
		NextSpawnStamp = 0;
		LivesRemaining = 100;
		_unit->SetMaxHealth( LivesRemaining );
		_unit->SetHealth( LivesRemaining );
		_unit->m_canRegenerateHP = false;
	}
    void AIUpdate()
    {
		if( LivesRemaining > 0 )
		{
			//wave ended
			uint32 WaveLoop = CurrentWaveCount / POSSIBLE_PREDEFINED_WAVES;
			uint32 WaveCount = CurrentWaveCount % POSSIBLE_PREDEFINED_WAVES;
			uint32 CurrentStamp = GetTickCount();
			if( WavesComposition[WaveCount][CurrentWaveProgress].Entry == 0 )
			{
				CurrentWaveCount++;
				CurrentWaveProgress = 0;
				NextSpawnStamp = CurrentStamp + ED_TIME_BETWEEN_WAVES;

				char msg[50];
				sprintf( msg, " Wave %d ended", CurrentWaveCount );
				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg );
			}
			if( NextSpawnStamp < CurrentStamp )
			{
				Creature *c = _unit->GetMapMgr()->CreateCreature( WavesComposition[WaveCount][CurrentWaveProgress].Entry, true, SpawnLocations[ WavesComposition[WaveCount][CurrentWaveProgress].SpawnLocationIndex].x , SpawnLocations[ WavesComposition[WaveCount][CurrentWaveProgress].SpawnLocationIndex].y, SpawnLocations[ WavesComposition[WaveCount][CurrentWaveProgress].SpawnLocationIndex].z );
				NextSpawnStamp = CurrentStamp + WavesComposition[WaveCount][CurrentWaveProgress].SpawnSleep;			
			}
		}
	}
	void OnDied(Unit *mKiller)
	{
		//continue running update script !
		//mark that people lost the match
		LivesRemaining = 0;
    }
}; 

class EDNPCSlow : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EDNPCSlow);
	uint32 SpawnedAtStamp;

	EDNPCSlow(Creature* pCreature) : CreatureAIScript(pCreature) {}
	void OnLoad()
	{
		if( _unit->GetMapId() != ED_MAP_ID )
			return;
		_unit->m_walkSpeed = PLAYER_NORMAL_RUN_SPEED - 2.0f;
		_unit->m_runSpeed = _unit->m_walkSpeed;	
		_unit->m_flySpeed = _unit->m_walkSpeed;	
		SpawnedAtStamp = GetTickCount();
		EDMapVariables *MapVars = (EDMapVariables*)_unit->GetMapMgr()->GetExtension( ED_EXTENSION_ID );			

		_unit->GetAIInterface()->SetWaypointMap( &Waypoints[ MapVars->WaypointAssignIndex ] );
		MapVars->WaypointAssignIndex = ( MapVars->WaypointAssignIndex + 1 ) % POSSIBLE_WAYPOINT_COUNT;

	}
	void OnDied(Unit *mKiller)
	{
		if( _unit->GetMapId() != ED_MAP_ID )
			return;

		EDMapVariables *MapVars = (EDMapVariables*)_unit->GetMapMgr()->GetExtension( ED_EXTENSION_ID );			
		uint32 TimeLived = 1 + GetTickCount() - SpawnedAtStamp;
		uint32 PlayerDPSOnUs = _unit->GetMaxHealth() * 1000 / TimeLived;
		MapVars->PlayerDPSOnMobType[ ED_NPC_SLOW ] += PlayerDPSOnUs;
		MapVars->PlayerDPSOnMobTypeSampleCount[ ED_NPC_SLOW ]++;

		RemoveAIUpdateEvent();
    }
}; 

void RegisterEndlessDefense(ScriptMgr* mgr)
{
	Simple3DCordStore WapointPoints[POSSIBLE_WAYPOINT_COUNT][1] =
	{ 
		{0,0,0},
		{0,0,0}
	};
	for( uint32 i=0;i<POSSIBLE_WAYPOINT_COUNT;i++)
	{
		//count number of points we have here
		uint32 PointCount = 0;
		for( uint32 j=0;WapointPoints[i][j].x != 0.0f && j < 10000;j++)
			PointCount++;
		Waypoints[i].Resize( PointCount );
		for( uint32 j=0;WapointPoints[i][j].x != 0.0f && j < 10000;j++)
		{
			WayPoint *wp = new WayPoint;
			memset( wp, 0, sizeof( wp ) );
			wp->x = WapointPoints[i][j].x;
			wp->y = WapointPoints[i][j].y;
			wp->z = WapointPoints[i][j].z;
			Waypoints[i].push_back( wp );
		}
	}

	mgr->register_creature_script( EDNPCSlowID, &EDNPCSlow::Create);
}
