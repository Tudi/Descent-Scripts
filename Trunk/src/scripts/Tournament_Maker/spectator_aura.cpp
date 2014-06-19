/*
#include "StdAfx.h"
#include "Setup.h"
#include "global.h"
bool TurnPlayerToSpectator(uint32 i, Aura* pAura, bool apply)
{
	Unit *caster = GetUnitCaster();
	if( !caster || !caster->IsPlayer())
		return;
	Player *pcaster = (Player*)caster;

	if(apply)
	{
		//set the sight to the supervisor so they can all see something
		if( m_target )
			caster->SetUInt64Value(PLAYER_FARSIGHT, m_target->GetGUID() );
		//add flags so player cannot move
		caster->SetUInt32Value( UNIT_FIELD_FLAGS, caster->GetUInt32Value( UNIT_FIELD_FLAGS ) | SET_SPECTATOR_UNIT_FLAGS );
		pcaster->EventTeleport(TOURNAMENT_SPECTATOR_TEL_MAP, TOURNAMENT_SPECTATOR_TEL_AREA_X, TOURNAMENT_SPECTATOR_TEL_AREA_Y, TOURNAMENT_SPECTATOR_TEL_AREA_Z );		
	}
	else
	{
		//unchain bind vision if we even chained it
		caster->SetUInt64Value(PLAYER_FARSIGHT, 0 );

		//remove flags
		caster->SetUInt32Value( UNIT_FIELD_FLAGS, caster->GetUInt32Value( UNIT_FIELD_FLAGS ) & ~SET_SPECTATOR_UNIT_FLAGS );

		//teleport him to some location : maybe shattrath ?
		pcaster->EventTeleport(TOURNAMENT_SPECTATOR_RET_AREA_MAP, TOURNAMENT_SPECTATOR_RET_AREA_X, TOURNAMENT_SPECTATOR_RET_AREA_Y, TOURNAMENT_SPECTATOR_RET_AREA_Z );		
	}
}

void SetupTSTNPCSPECTATOR(ScriptMgr * mgr)
{
	mgr->register_dummy_aura(SPECTATOR_AURA_ID, &TurnPlayerToSpectator);
}
*/