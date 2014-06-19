#include "StdAfx.h"
#include "Setup.h"
#include "global.h"

// Does not Damage
// constant health
// provodes skill up and chance for players to gain resource

class AI_DUMMY_TOWER : public GeneralTower, public AIInterface
{
public:
	void            Init(Unit *un, AIType at, MovementType mt);
	void            Update(uint32 p_time);
private:
};

void AI_DUMMY_TOWER::Init(Unit *un, AIType at, MovementType mt)
{
	//run the original init and we add our custom stuff too
	AIInterface::Init(un,at,mt);
	//mark where we was spawned. Towers do not move
	spawn_x = un->GetPositionX();
	spawn_y = un->GetPositionY();
	spawn_z = un->GetPositionZ();

	un->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 500000*2 );
	un->SetUInt32Value( UNIT_FIELD_MAXPOWER1, 500000*2 );

	un->SetPowerType( POWER_TYPE_HAPPINESS );	//no packet spams to client

	//somehow signal that we are a tower
	un->m_special_state |= UNIT_STATE_BANISH;
}

void AI_DUMMY_TOWER::Update(uint32 p_time)
{
	//in case they managed somehow to move the tower then just put it back. Ex : deathgrip
	if( spawn_x != m_Unit->GetPositionX() )
	{
		m_Unit->SetPosition( spawn_x,spawn_y,spawn_z,m_Unit->GetOrientation(), false );
		WorldPacket data;
		m_Unit->BuildHeartBeatMsg( &data );
		m_Unit->SendMessageToSet( &data, false );
	}

	//let compiler optimize this. Function calls cannot be always optimized
	TickNow = GetTickCount();

	//constant Regen
	if( RegenStamp < TickNow )
	{
		m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH, 500000 );
		m_Unit->SetUInt32Value( UNIT_FIELD_POWER1, 500000 );

		RegenStamp = TickNow + TOWER_REGEN_INTERVAL;

		WipeTargetList();	//we will never get to fight back :(

		//provide resource for those that are inrange
		for( set<Object*>::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
		{
			if( (*itr) == NULL || (*itr)->IsInWorld() == false || (*itr)->IsPlayer() == false 
				|| (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH ) == 0 || static_cast<Unit *>(*itr)->isAlive() == false
				)
				continue;

			if( IsInrange( m_Unit, (*itr), cur_range_sq ) == false )
				continue;

			PlayerResourceHolder *h = FetchHolder( static_cast<Player*>(*itr) );
			h->ResourceNow += ( TOWER_REGEN_INTERVAL / PLAYER_RESOURCE_SCALEDOWN );
		}
	}
}

//this NPC is spawned in the midle of the arena to do everything :P
class AIDummyTowerReplacer : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AIDummyTowerReplacer);

	//constructor
    AIDummyTowerReplacer(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
		AI_DUMMY_TOWER *new_interface = new AI_DUMMY_TOWER;
		new_interface->Init(_unit,AITYPE_SOCIAL,MOVEMENTTYPE_NONE);
		_unit->ReplaceAIInterface( (AIInterface *) new_interface );
		_unit->m_canRegenerateHP = false;
	}
};

void RegisterAIDummyTower(ScriptMgr * mgr)
{
	 mgr->register_creature_script( 123461, &AIDummyTowerReplacer::Create );
}