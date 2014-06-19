#ifndef _GLOBAL_INCLUDES_H_
#define _GLOBAL_INCLUDES_H_

//#include "AIDPSTower.h"
//#include "AITankTower.h"
//#include "AISupportTower.h"

//really do not spam this, AI complexity grows exponentially. Better adjust spell mods
#define TOWER_MAX_SPELL_COUNT				5

#define TOWER_TARGET_AQUISITION_INTERVAL	2000	// if target has no default target, he will try to find one
#define TOWER_REGEN_INTERVAL				5000	// these regens will keep spamming packets
#define TOWER_RESOURCE_COST_INCREASE_MULT	3		// each time you use a feature the cost will increase by this ammount

//if range is too small then Players might exploit them by attacking with ranged weapons
#define TOWER_DPS_BASE_RANGE				35
#define TOWER_TANK_BASE_RANGE				35
#define TOWER_SUPPORT_BASE_RANGE			35

#define TOWER_MIN_DISTANCE_TO_GY_SQ			(80*80)
#define TOWER_MIN_DISTANCE_TO_SPAWN_SQ		(40*40)	//should be larger then tower range. If not then check if tower tanks are stealing target from each other

#define TOWER_TANK_MAX_SHIELDED_UNIT_COUNT	15		//max 15 units are shielded
#define TOWER_TANK_HEALTH_RESTORE_PCT		75		//restore 75% of the DMG received
#define TOWER_TANK_DMG_CONVERT_PCT			50		//after restoring health to units we will apply DMG to self

#define PLAYER_SOUL_VENDOR_AURA				24828	//player may sell he's soul for instant resources
#define PLAYER_SOUL_VENDOR_REWARD_PCT		90		//give amount of resources so it fills 50% of 	

//at this update we could not find a spell to cast
#define TOWER_INVALID_SPELL_INDEX			-1
#define PLAYER_EXTENSION_ID					6173736175674	//assault
#define TOWER_INITIAL_RESOURCE_COST			150				//dmg + heal
#define PLAYER_RESOURCE_SCALEDOWN			100
#define TOWER_MAX_Z_DIFF_TO_CONSIDER_LEVEL	4

//anti hack protections
#define CAP_RESOURCE_PER_UPGRADE			0x00FFFFFF

enum SupportTowerSpellIndexes
{
	STSI_TEAM_BUFF1		= 0,
	STSI_TEAM_BUFF2		= 1,
	STSI_TEAM_HEAL		= 2,
	STSI_ENEMY_NERF1	= 3,
	STSI_ENEMY_NERF2	= 4,
};

enum TankTowerSpellIndexes
{
	TTSI_FETCH		= 0,
	TTSI_ROOT		= 1,
	TTSI_DMG		= 2,
};

struct TowerSpellDesc
{
	uint32	spell_id;
	uint32	spell_cooldown;
	uint32	spell_global_cooldown;
	uint32	cast_chance;
	uint32	spell_effect_scale_fixed[3];
	uint32	spell_duration_fixed;
	uint32	spell_tagetting_type;
	uint32	target_min_range_sq;
	uint32	target_max_range_sq;
};

struct TowerSpellDescInstance
{
	uint32 cooldown_finished;
};

enum TowerIndexes
{
	TOWER_INDEX_TANK			= 0,
	TOWER_INDEX_SUPPORT			= 1,
	TOWER_INDEX_DPS				= 2,
	TOWER_INDEX_TOTAL,
	TOWER_INDEX_TANK_UPGRADE	= 3,
	TOWER_INDEX_SUPPORT_UPGRADE	= 4,
	TOWER_INDEX_DPS_UPGRADE		= 5,
	TOWER_INDEX_TOTAL2
};

struct PlayerResourceHolder
{
	uint32	ResourceLastTime;
	uint32	ResourceNow;
	uint32	ResourceToGather[TOWER_INDEX_TOTAL*2];	//spawns + upgrades
	uint32	AssignedToInstanceID;
	uint32	TowerLevelBonuses[TOWER_INDEX_TOTAL];
	uint32	ShowingTimer;
};

/*
struct TowerLevelDesc
{
	int32			HealthBoost;				//base +
	int32			ManaBoost;					//base +
	int32			HealthRegenBoost;			//base +
	int32			ManaRegenBoost;				//base +
	int32			ResistChanceBoost;			//base +
	int32			RangeBoost;					//the range the tower will reach out to do stuff. Limited by view range
	int32			ProximityReduction;			//how close 2 towers of same type may be placed
	TowerSpellDesc	new_spells[TOWER_MAX_SPELL_GAIN_ON_LEVEL];
};*/

class GeneralTower
{
public:
	GeneralTower() 
	{ 
		target_guid = 0;
		killcount = 0;
		NextTargetUpdateStamp = 0;
		memset( SpellBook, 0 , sizeof( SpellBook ) );
		memset( SpellInstances, 0 , sizeof( SpellInstances ) );
		LastUsedSpellIndex = 0;
		cur_range_sq = 30*30;
		GlobalCoolDownFinished = 0;
		RegenStamp = 0;
		SpellCastsOnPlayers = 0;
		LastLevelAtEventUpdateCall = 0;
		in_combat_H_regen = 0;
		in_combat_P_regen = 0;
		TickNow = 0;
		NextTargetUpdateStamp = 0;
	}
	virtual void			EventLevelUp(){};
protected:
	void					EventSpellCast();
	bool					RollDiceForSpellCast( uint32 SpellBookPage );
	int32					GetNextPossibleSpellToCast(uint32 range_sq);
	uint32					LastUsedSpellIndex;
	TowerSpellDesc			SpellBook[ TOWER_MAX_SPELL_COUNT ];
	TowerSpellDescInstance	SpellInstances[ TOWER_MAX_SPELL_COUNT ];
	uint32					cur_range_sq;				//range where we may apply our spells
	uint32					killcount;					//towers could self upgrade to gain an edge ?
	uint64					target_guid;
	uint32					NextTargetUpdateStamp;
	uint32					GlobalCoolDownFinished;
	float					spawn_x,spawn_y,spawn_z;	//cause towers cannot move
	uint32					in_combat_H_regen;			
	uint32					in_combat_P_regen;			
	uint32					TickNow;					//this bugegr is used everywhere. Maybe make it global ?
	uint32					RegenStamp;					//don't spam update packets
	uint32					SpellCastsOnPlayers;		//we reward owner based on our tower contribution to fight
	uint32					LastLevelAtEventUpdateCall;
};

float Distance2DSQ(Object *o,float x, float y);
PlayerResourceHolder *FetchHolder( Player* Plr );

#endif