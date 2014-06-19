#include "StdAfx.h"
#include "Setup.h"
#include "global.h"

//	Fast attacking
//	Medium Defense
//	Medium Damage
//	Medium Regen

class AI_DPS_TOWER : public GeneralTower, public AIInterface
{
public:
	void            Init(Unit *un, AIType at, MovementType mt);
	void            Update(uint32 p_time);
private:
	void			EventLevelUp();
	bool			MyIsAttackable( Object *a, Object *b)
	{
		if( a && a->m_faction && b && b->m_faction && a->m_faction->ID != b->m_faction->ID )
			return true;
		return false;
	}
};

void AI_DPS_TOWER::Init(Unit *un, AIType at, MovementType mt)
{
	//run the original init and we add our custom stuff too
	AIInterface::Init(un,at,mt);
	//mark where we was spawned. Towers do not move
	spawn_x = un->GetPositionX();
	spawn_y = un->GetPositionY();
	spawn_z = un->GetPositionZ();
	//set level specific info
	EventLevelUp();
	cur_range_sq = TOWER_DPS_BASE_RANGE * TOWER_DPS_BASE_RANGE;
	//somehow signal that we are a tower
	un->m_special_state |= UNIT_STATE_BANISH;
}

void AI_DPS_TOWER::Update(uint32 p_time)
{
	//in case they managed somehow to move the tower then just put it back. Ex : deathgrip
	if( spawn_x != m_Unit->GetPositionX() )
	{
		m_Unit->SetPosition( spawn_x,spawn_y,spawn_z,m_Unit->GetOrientation(), false );
		WorldPacket data;
		m_Unit->BuildHeartBeatMsg( &data );
		m_Unit->SendMessageToSet( &data, false );
	}

	//if we are already casting something then ignore trying to cast again
//	if( m_Unit->isCasting() )
//		return;

	//let compiler optimize this. Function calls cannot be always optimized
	TickNow = GetTickCount();

	//constant Regen
	if( RegenStamp < TickNow )
	{
		//check for owner, if there is none then we may despawn
		Player *p = m_Unit->GetMapMgr()->GetPlayer( m_Unit->GetUInt64Value( UNIT_FIELD_CREATEDBY ) );
		//we may also despawn if we died
		if( ( ( p == NULL && m_Unit->GetUInt64Value( UNIT_FIELD_CREATEDBY ) ) || m_Unit->GetUInt32Value( UNIT_FIELD_HEALTH ) == 0 ) && m_Unit->IsCreature() )
		{
			static_cast<Creature *>( m_Unit )->Despawn( 0, 0 );
			RegenStamp = 0xFFFFFFFF;				//hopefully no more updates
			GlobalCoolDownFinished = 0xFFFFFFFF;	//hopefully no more updates
			return;
		}
		//award owner for spawning us
		if( p )
		{
			p->ModUnsigned32Value( PLAYER_FIELD_COINAGE, SpellCastsOnPlayers * sWorld.getRate(RATE_MONEY) * 10 );
			SpellCastsOnPlayers = 0;
		}

		//check if we got upgraded 
		EventLevelUp();

		RegenStamp = TickNow + TOWER_REGEN_INTERVAL;
		m_Unit->ModUnsigned32Value( UNIT_FIELD_HEALTH, in_combat_H_regen );
		if( m_Unit->GetUInt32Value( UNIT_FIELD_HEALTH ) > m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) )
			m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH, m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
//		m_Unit->ModUnsigned32Value( UNIT_FIELD_POWER1, in_combat_P_regen );
//		if( m_Unit->GetUInt32Value( UNIT_FIELD_POWER1 ) > m_Unit->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) )
//			m_Unit->SetUInt32Value( UNIT_FIELD_POWER1, m_Unit->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );
	}

	//we are still waiting to get a new chance to cast a spell
	if( GlobalCoolDownFinished >= TickNow )
		return;

	Unit *TargetNow;

	//see if in previous update we managed to find a target that is still valid
	if( target_guid != 0 )
	{
		TargetNow = m_Unit->GetMapMgr()->GetUnit( target_guid );
		//target is not valid anymore
		if( TargetNow == NULL )
			target_guid = 0;
	}

	//see if we can quickly target someone from agro list
	if( target_guid == 0 )
	{
		TargetNow = GetMostHated();
		if( TargetNow && IsInrange( TargetNow, m_Unit, cur_range_sq ) )
			target_guid = TargetNow->GetGUID();
	}

	//check if our target is still ok to attack
	if( TargetNow && 
		( IsInrange( TargetNow, m_Unit, cur_range_sq ) == false
			|| TargetNow->GetUInt32Value( UNIT_FIELD_HEALTH ) == 0 
			|| TargetNow->isAlive() == false
			|| TargetNow->GetEntry() == 123461	//dummy tower should not be targeted
//			|| ( TargetNow->m_special_state & UNIT_STATE_BANISH ) != 0	// does not attack other towers
			|| abs( TargetNow->GetPositionZ() - m_Unit->GetPositionZ() ) > TOWER_MAX_Z_DIFF_TO_CONSIDER_LEVEL
			)
		)
	{
		target_guid = 0;
		TargetNow = NULL;
		RemoveThreatByPtr( TargetNow );
	}

	//see if we could find a new target to agro
	if( target_guid == 0 && NextTargetUpdateStamp < TickNow )
	{
		//do not spam search cause it might be slow
		NextTargetUpdateStamp = TickNow + TOWER_TARGET_AQUISITION_INTERVAL;

		//our max visible list
		for( set<Object*>::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
		{
			if( (*itr) == NULL || (*itr)->IsInWorld() == false || (*itr)->IsUnit() == false 
				|| (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH ) == 0 || static_cast<Unit *>(*itr)->isAlive() == false
				|| (*itr)->GetEntry() == 123461	//dummy tower should not be targeted
				|| abs( (*itr)->GetPositionZ() - m_Unit->GetPositionZ() ) > TOWER_MAX_Z_DIFF_TO_CONSIDER_LEVEL
				|| static_cast<Unit *>(*itr)->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLUS_MOB | UNIT_FLAG_NOT_ATTACKABLE_9 ) == true //spirit guide
				)
				continue;

			if( MyIsAttackable( m_Unit, (*itr) ) == false )
				continue;

			if( IsInrange( m_Unit, (*itr), cur_range_sq ) == false )
				continue;

			//found one
			TargetNow = static_cast<Unit*>(*itr);
			target_guid = (*itr)->GetGUID();
			break;
		}
		_UpdateTargets();	//requered to clean out hate list
	}
	
	//if we still have no target then we can just give up
	if( target_guid == 0 )
		return;

	//get a target and try to cast spell on it
	float distx=m_Unit->GetPositionX()-TargetNow->GetPositionX();
	float disty=m_Unit->GetPositionY()-TargetNow->GetPositionY();
	int32 dist = distx*distx + disty*disty; 

	int32 SpellBookPage = GetNextPossibleSpellToCast( dist );

	//can we cast a spell ?
	if( SpellBookPage != TOWER_INVALID_SPELL_INDEX )
	{
		SetNextTarget( target_guid );
		AttackReaction( TargetNow, 1, 0 );

        SpellCastTargets targets( target_guid );
		SpellEntry *spellInfo = dbcSpell.LookupEntry( SpellBook[ SpellBookPage ].spell_id );
		if (spellInfo)
		{
			Spell *spell = SpellPool.PooledNew();
			spell->Init( m_Unit, spellInfo ,true, NULL);
			if( SpellBook[ SpellBookPage ].spell_effect_scale_fixed[ 0 ] ) 
				spell->forced_basepoints[0] = SpellBook[ SpellBookPage ].spell_effect_scale_fixed[ 0 ];
			if( SpellBook[ SpellBookPage ].spell_effect_scale_fixed[ 1 ] ) 
				spell->forced_basepoints[1] = SpellBook[ SpellBookPage ].spell_effect_scale_fixed[ 1 ];
			spell->prepare(&targets);
//			m_Unit->ModUnsigned32Value( UNIT_FIELD_POWER1, m_Unit->GetUInt32Value( UNIT_FIELD_POWER1 ) * spellInfo->ManaCostPercentage );
		}

		//count tower contribution to fight
		if( TargetNow && TargetNow->IsPlayer() )
			SpellCastsOnPlayers++;

		GlobalCoolDownFinished = TickNow + SpellBook[ SpellBookPage ].spell_global_cooldown;
		SpellInstances[ SpellBookPage ].cooldown_finished = TickNow + SpellBook[ SpellBookPage ].spell_cooldown;
		LastUsedSpellIndex  = SpellBookPage;
	}
}

//as towers gain level they get stronger and stronger
void AI_DPS_TOWER::EventLevelUp()
{
	uint32 level_now = m_Unit->GetUInt32Value( UNIT_FIELD_LEVEL );
	if( LastLevelAtEventUpdateCall == level_now )
		return;
	LastLevelAtEventUpdateCall = level_now;
	//mana boost is large
//	uint32 power_new = 5000 + level_now*1000;
//	m_Unit->SetUInt32Value( UNIT_FIELD_MAXPOWER1, power_new );
//	m_Unit->SetUInt32Value( UNIT_FIELD_POWER1, power_new );
	//spell_awarding
	uint32 small_level_index = (level_now % 10) + 1;	//cause each BG has top 10 level bracket

	//health regen is moderate
	in_combat_H_regen = small_level_index * 20;		//x HP / 500 Msec even in combat
	//power regen is moderate
//	in_combat_P_regen = small_level_index * 50;		//y P / 500 Msec

	//health boost / level is small
	float health_now_pct = (float)m_Unit->GetUInt32Value( UNIT_FIELD_HEALTH ) / m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH );
	uint32 health_new = m_Unit->GetUInt32Value( UNIT_FIELD_BASE_HEALTH ) + small_level_index * 15000;
	m_Unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, health_new );
	m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH, health_new * health_now_pct );

	SpellBook[ 0 ].spell_cooldown = 4000 - small_level_index*100;					//slow attack to leave space for other
	SpellBook[ 0 ].spell_global_cooldown = 2000 - small_level_index*10;				//general attack timer
	SpellBook[ 1 ].spell_cooldown = 4000 - small_level_index*100;					//slow attack to leave space for other
	SpellBook[ 1 ].spell_global_cooldown = 2000 - small_level_index*10;				//general attack timer
	SpellBook[ 2 ].spell_cooldown = 7000 - small_level_index*200;					//slow attack to leave space for other
	SpellBook[ 2 ].spell_global_cooldown = 2000 - small_level_index*10;				//general attack timer
	SpellBook[ 3 ].spell_cooldown = 7000 - small_level_index*200;					//slow attack to leave space for other
	SpellBook[ 3 ].spell_global_cooldown = 2000 - small_level_index*10;				//general attack timer
	if( small_level_index >= 2 )
	{
		SpellBook[ 0 ].spell_id = 116;								//frostbolt to slow enemy and deal small dmg
		SpellBook[ 0 ].cast_chance = 100;
		SpellBook[ 0 ].target_min_range_sq = 15;
		SpellBook[ 0 ].target_min_range_sq *= SpellBook[ 0 ].target_min_range_sq;
		SpellBook[ 0 ].target_max_range_sq = TOWER_DPS_BASE_RANGE;
		SpellBook[ 0 ].target_max_range_sq *= SpellBook[ 0 ].target_max_range_sq;
		SpellBook[ 0 ].spell_effect_scale_fixed[0] = 10 + small_level_index * 5;
		SpellBook[ 0 ].spell_effect_scale_fixed[1] = 450 + small_level_index * 140;

		SpellBook[ 1 ].spell_id = 122;								//frost nova AOE for close combat
		SpellBook[ 1 ].cast_chance = 100;
		SpellBook[ 1 ].target_min_range_sq = 0;
		SpellBook[ 1 ].target_min_range_sq *= SpellBook[ 1 ].target_min_range_sq;
		SpellBook[ 1 ].target_max_range_sq = 15;
		SpellBook[ 1 ].target_max_range_sq *= SpellBook[ 1 ].target_max_range_sq;
		SpellBook[ 1 ].spell_effect_scale_fixed[0] = 700 + small_level_index * 210;
	}
//	if( small_level_index >= 3 )
	{
		SpellBook[ 2 ].spell_id = 145;								//fireball to slow enemy and deal small dmg
		SpellBook[ 2 ].cast_chance = 10;
		SpellBook[ 2 ].target_min_range_sq = 15;
		SpellBook[ 2 ].target_min_range_sq *= SpellBook[ 2 ].target_min_range_sq;
		SpellBook[ 2 ].target_max_range_sq = TOWER_DPS_BASE_RANGE;
		SpellBook[ 2 ].target_max_range_sq *= SpellBook[ 2 ].target_max_range_sq;
		SpellBook[ 2 ].spell_effect_scale_fixed[0] = 1200 + small_level_index * 170;

		SpellBook[ 3 ].spell_id = 30941;							//fire nova AOE for close combat
		SpellBook[ 3 ].cast_chance = 10;
		SpellBook[ 3 ].target_min_range_sq = 0;
		SpellBook[ 3 ].target_min_range_sq *= SpellBook[ 3 ].target_min_range_sq;
		SpellBook[ 3 ].target_max_range_sq = 15;
		SpellBook[ 3 ].target_max_range_sq *= SpellBook[ 3 ].target_max_range_sq;
		SpellBook[ 3 ].spell_effect_scale_fixed[0] = 1500 + small_level_index * 190;
	}
}

//this NPC is spawned in the midle of the arena to do everything :P
class AIDPSTowerReplacer : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AIDPSTowerReplacer);

	//constructor
    AIDPSTowerReplacer(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
		AI_DPS_TOWER *new_interface = new AI_DPS_TOWER;
		new_interface->Init(_unit,AITYPE_SOCIAL,MOVEMENTTYPE_NONE);
		_unit->ReplaceAIInterface( (AIInterface *) new_interface );
		_unit->m_canRegenerateHP = false;
	}
};

void RegisterAIDPSTower(ScriptMgr * mgr)
{
	 mgr->register_creature_script( 123462, &AIDPSTowerReplacer::Create );
	 mgr->register_creature_script( 123463, &AIDPSTowerReplacer::Create );
}