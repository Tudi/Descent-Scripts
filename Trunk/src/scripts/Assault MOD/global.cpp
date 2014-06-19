#include "StdAfx.h"
#include "Setup.h"
#include "global.h"

bool GeneralTower::RollDiceForSpellCast( uint32 SpellBookPage )
{
	if( RandomUInt() % 100 > SpellBook[ SpellBookPage ].cast_chance )
		return false;
	return true;
}

int32 GeneralTower::GetNextPossibleSpellToCast(uint32 range_sq)
{
	uint32 TickNow = GetTickCount();

	if( GlobalCoolDownFinished >= TickNow )
		return TOWER_INVALID_SPELL_INDEX;

	for(uint32 i=LastUsedSpellIndex+1;i<LastUsedSpellIndex+TOWER_MAX_SPELL_COUNT+1;i++)
	{
		//pick a valid index
		uint32 ind_now = i % TOWER_MAX_SPELL_COUNT;

		//do we even have a spell in spellbook for this page ?
		if( SpellBook[ ind_now ].spell_id == 0 )
			continue;

		//cooldown finished ?
		if( SpellInstances[ ind_now ].cooldown_finished >= TickNow )
			continue;

		//distance check
//		if( SpellBook[ ind_now ].target_min_range_sq <= range_sq )
		if( SpellBook[ ind_now ].target_min_range_sq > range_sq )
			continue;
//		if( SpellBook[ ind_now ].target_max_range_sq >= range_sq )
		if( SpellBook[ ind_now ].target_max_range_sq < range_sq )
			continue;

		//cast chance
		if( RollDiceForSpellCast( ind_now ) == false )
			continue;

		//usable and taken
		return ind_now;
	}

	//seems like no spells are usable atm
	return TOWER_INVALID_SPELL_INDEX;
}

float Distance2DSQ(Object *o,float x, float y)
{
	float dist_x = o->GetPositionX() - x;
	float dist_y = o->GetPositionY() - y;
	float dist_sq = dist_x*dist_x + dist_y*dist_y;
	return dist_sq;
}
