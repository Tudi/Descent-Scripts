#include "StdAfx.h"
#include "Setup.h"
#include "global.h"

// very strong as health
// almost no Regen
// shields a few nearby units by absorbing dmg and applying to self
// slow attack
// strong dmg
// multiple targetting
// pull and root a target to attack

struct UnitHealthStateStore
{
	Unit	*unit;				//to identify who we saved state for
	uint32	SavedHealthState;	//last time the health was like this
};

class AI_TANK_TOWER : public GeneralTower, public AIInterface
{
public:
	void			Init(Unit *un, AIType at, MovementType mt);
	void			Update(uint32 p_time);
private:
	void			EventLevelUp();
	bool			MyIsAttackable( Object *a, Object *b)
	{
		if( a && a->m_faction && b && b->m_faction && a->m_faction->ID != b->m_faction->ID )
			return true;
		return false;
	}
	void			CastSpellBookSpell( uint32 SpellBookPage, Unit *target );
	UnitHealthStateStore	ShieldedUnits[ TOWER_TANK_MAX_SHIELDED_UNIT_COUNT ];
};

void AI_TANK_TOWER::Init(Unit *un, AIType at, MovementType mt)
{
	//run the original init and we add our custom stuff too
	AIInterface::Init(un,at,mt);
	//mark where we was spawned. Towers do not move
	spawn_x = un->GetPositionX();
	spawn_y = un->GetPositionY();
	spawn_z = un->GetPositionZ();
	//set level specific info
	EventLevelUp();
	cur_range_sq = TOWER_SUPPORT_BASE_RANGE * TOWER_SUPPORT_BASE_RANGE;
	memset( ShieldedUnits, 0, sizeof( ShieldedUnits ) );
	//somehow signal that we are a tower
	un->m_special_state |= UNIT_STATE_BANISH;
}

void AI_TANK_TOWER::CastSpellBookSpell( uint32 SpellBookPage, Unit *target )
{
	if( SpellBookPage != TOWER_INVALID_SPELL_INDEX )
	{
		uint64 target_guid = target->GetGUID();
		SetNextTarget( target_guid );

        SpellCastTargets targets( target_guid );
		SpellEntry *spellInfo = dbcSpell.LookupEntry( SpellBook[ SpellBookPage ].spell_id );
		if (spellInfo)
		{
			Spell *spell = SpellPool.PooledNew();
			spell->Init( m_Unit, spellInfo ,true, NULL);
			spell->forced_basepoints[0] = SpellBook[ SpellBookPage ].spell_effect_scale_fixed[ 0 ];
			if( SpellBook[ SpellBookPage ].spell_effect_scale_fixed[ 0 ] ) 
				spell->forced_basepoints[0] = SpellBook[ SpellBookPage ].spell_effect_scale_fixed[ 0 ];
			if( SpellBook[ SpellBookPage ].spell_duration_fixed )
				spell->forced_duration = SpellBook[ SpellBookPage ].spell_duration_fixed;
//			m_Unit->ModUnsigned32Value( UNIT_FIELD_POWER1, m_Unit->GetUInt32Value( UNIT_FIELD_POWER1 ) * spellInfo->ManaCostPercentage );
			spell->prepare(&targets);
		}

		GlobalCoolDownFinished = TickNow + SpellBook[ SpellBookPage ].spell_global_cooldown;
		SpellInstances[ SpellBookPage ].cooldown_finished = TickNow + SpellBook[ SpellBookPage ].spell_cooldown;
		LastUsedSpellIndex  = SpellBookPage;
	}
}


void AI_TANK_TOWER::Update(uint32 p_time)
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
		/*
		m_Unit->ModUnsigned32Value( UNIT_FIELD_HEALTH, in_combat_H_regen );
		if( m_Unit->GetUInt32Value( UNIT_FIELD_HEALTH ) > m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) )
			m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH, m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			*/
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
		if( TargetNow )
			target_guid = TargetNow->GetGUID();
	}

	//check if our target is still ok to attack
	if( TargetNow && 
		( IsInrange( TargetNow, m_Unit, cur_range_sq ) == false
			|| TargetNow->GetUInt32Value( UNIT_FIELD_HEALTH ) == 0 
			|| TargetNow->isAlive() == false
			|| ( TargetNow->m_special_state & UNIT_STATE_BANISH ) != 0	// does not attack other towers
			|| abs( TargetNow->GetPositionZ() - m_Unit->GetPositionZ() ) > TOWER_MAX_Z_DIFF_TO_CONSIDER_LEVEL
			)
		)
	{
		target_guid = 0;
		TargetNow = NULL;
		RemoveThreatByPtr( TargetNow );
	}

	//see if we could find a new target to agro
	if( NextTargetUpdateStamp < TickNow )
	{
		//do not spam search cause it might be slow
		NextTargetUpdateStamp = TickNow + TOWER_TARGET_AQUISITION_INTERVAL;

		uint32 updated_slots[ TOWER_TANK_MAX_SHIELDED_UNIT_COUNT ];
		memset( updated_slots, 0, sizeof( updated_slots ) );
		//our max visible list
		for( set<Object*>::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
		{
			if( (*itr) == NULL || (*itr)->IsInWorld() == false || (*itr)->IsUnit() == false 
				|| (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH ) == 0 || static_cast<Unit *>(*itr)->isAlive() == false
//				|| ( TargetNow->m_special_state & UNIT_STATE_BANISH ) != 0	// does not attack other towers but it does support them
				)
				continue;

			//no self targetting
			if( (*itr) == m_Unit )
				continue;

			if( IsInrange( m_Unit, (*itr), cur_range_sq ) == false )
				continue;

			//check if we are shielding this unit
			if( MyIsAttackable( m_Unit, (*itr) ) == false )
			{
				bool foundit = false;
				for( uint32 i=0;i<TOWER_TANK_MAX_SHIELDED_UNIT_COUNT;i++)
					if( ShieldedUnits[i].unit == (*itr) )
					{
						//check if we need to restore health to him and apply DMG to us
						if( (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH ) < ShieldedUnits[i].SavedHealthState )
						{
							uint32	health_diff = ShieldedUnits[i].SavedHealthState - (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH );
							uint32	restore = health_diff * TOWER_TANK_HEALTH_RESTORE_PCT / 100;
							(*itr)->ModUnsigned32Value( UNIT_FIELD_HEALTH, restore );
							uint32	DMG_self = health_diff * TOWER_TANK_DMG_CONVERT_PCT / 100;
							m_Unit->DealDamage( m_Unit, DMG_self, 0, 0, 0, true );
							updated_slots[ i ] = 1;
							if( (*itr)->IsPlayer() ) 
								SpellCastsOnPlayers++;
						}
						ShieldedUnits[i].SavedHealthState = (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH );
						foundit = true;
						break;
					}
				//not yet shielded -> add it
				if( foundit == false )
					for( uint32 i=0;i<TOWER_TANK_MAX_SHIELDED_UNIT_COUNT;i++)
						if( ShieldedUnits[i].unit == NULL )
						{
							ShieldedUnits[i].unit = static_cast<Unit*>(*itr);
							ShieldedUnits[i].SavedHealthState = (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH );
							updated_slots[ i ] = 1;
							break;
						}
			}
			//possible target to be attacked
			else if( target_guid == 0 
				&& ( static_cast<Unit *>(*itr)->m_special_state & UNIT_STATE_BANISH ) == 0	//does not attack other towers
				&& abs( (*itr)->GetPositionZ() - m_Unit->GetPositionZ() ) < TOWER_MAX_Z_DIFF_TO_CONSIDER_LEVEL
				&& static_cast<Unit *>(*itr)->m_rooted == 0	// do not target already rooted / neted targets
				&& static_cast<Unit *>(*itr)->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLUS_MOB | UNIT_FLAG_NOT_ATTACKABLE_9) == false //spirit guide
				)
			{
				TargetNow = static_cast<Unit*>(*itr);
				target_guid = (*itr)->GetGUID();
			}
		}
		//remove units from our list that we are not shielded anymore
		for( uint32 i=0;i<TOWER_TANK_MAX_SHIELDED_UNIT_COUNT;i++)
			if( updated_slots[i] != 1 )
				ShieldedUnits[i].unit = NULL;
		_UpdateTargets();	//requered to clean out hate list
	}
	
	//if we still have no target then we can just give up
	if( target_guid == 0 )
		return;

	// 2 steps : 
	//	a) pull + root
	//	b) start hitting as much as possible

	//get a target and try to cast spell on it
	float distx=m_Unit->GetPositionX()-TargetNow->GetPositionX();
	float disty=m_Unit->GetPositionY()-TargetNow->GetPositionY();
	int32 dist = distx*distx + disty*disty; 

	// a) fetch
	if( dist > 5*5 && dist < (( TOWER_TANK_BASE_RANGE*TOWER_TANK_BASE_RANGE )-(10*10)) && SpellInstances[ TTSI_FETCH ].cooldown_finished < TickNow )
	{
		CastSpellBookSpell( TTSI_FETCH, TargetNow );
		CastSpellBookSpell( TTSI_ROOT, TargetNow );
		// gold per second
		if( TargetNow && TargetNow->IsPlayer() )
			SpellCastsOnPlayers += 2;
	}
	else if( dist < 5*5 && SpellInstances[ TTSI_DMG ].cooldown_finished < TickNow )
	{
		CastSpellBookSpell( TTSI_DMG, TargetNow );
		// gold per second
		if( TargetNow && TargetNow->IsPlayer() )
			SpellCastsOnPlayers += 7;
	}
}

//as towers gain level they get stronger and stronger
void AI_TANK_TOWER::EventLevelUp()
{
	uint32 level_now = m_Unit->GetUInt32Value( UNIT_FIELD_LEVEL );
	if( LastLevelAtEventUpdateCall == level_now )
		return;
	LastLevelAtEventUpdateCall = level_now;
	//mana boost is none
	//health regen is none
//	in_combat_H_regen = 0;	//x HP / 2 sec even in combat
	//power regen is none
//	in_combat_P_regen = 0;		//y P / 2 sec
	//spell_awarding
	uint32 small_level_index = (level_now % 10)+1;	//cause each BG has top 10 level bracket

	//health boost / level is big
	float health_now_pct = (float)m_Unit->GetUInt32Value( UNIT_FIELD_HEALTH ) / m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH );
	uint32 health_new = m_Unit->GetUInt32Value( UNIT_FIELD_BASE_HEALTH ) + small_level_index * 20000;
	m_Unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, health_new );
	m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH, health_new * health_now_pct );
	//dmg
	m_Unit->SetUInt32Value( UNIT_FIELD_MAXDAMAGE, m_Unit->GetUInt32Value( UNIT_FIELD_MINDAMAGE ) + small_level_index * 200 );

	SpellBook[ TTSI_FETCH ].spell_id = 49576;											//death grip
	SpellBook[ TTSI_FETCH ].spell_cooldown = 30000;						
	SpellBook[ TTSI_FETCH ].spell_global_cooldown = 2000;				
	SpellBook[ TTSI_FETCH ].cast_chance = 100;
	SpellBook[ TTSI_FETCH ].target_min_range_sq = 5;
	SpellBook[ TTSI_FETCH ].target_min_range_sq *= SpellBook[ 0 ].target_min_range_sq;
	SpellBook[ TTSI_FETCH ].target_max_range_sq = TOWER_TANK_BASE_RANGE-5;
	SpellBook[ TTSI_FETCH ].target_max_range_sq *= SpellBook[ 0 ].target_max_range_sq;

	SpellBook[ TTSI_ROOT ].spell_id = 49092;											//root
	SpellBook[ TTSI_ROOT ].spell_global_cooldown = 2000;				
	SpellBook[ TTSI_ROOT ].cast_chance = 100;
	SpellBook[ TTSI_ROOT ].target_min_range_sq = 0;
	SpellBook[ TTSI_ROOT ].target_min_range_sq *= SpellBook[ 1 ].target_min_range_sq;
	SpellBook[ TTSI_ROOT ].target_max_range_sq = 5;
	SpellBook[ TTSI_ROOT ].target_max_range_sq *= SpellBook[ 1 ].target_max_range_sq;
	SpellBook[ TTSI_ROOT ].spell_duration_fixed = 20 + 2*1000*small_level_index;
	SpellBook[ TTSI_ROOT ].spell_cooldown = 20000 + SpellBook[ TTSI_ROOT ].spell_duration_fixed;						
	// !! twin spells test through first one
	SpellBook[ TTSI_FETCH ].spell_cooldown = 20000 + SpellBook[ TTSI_ROOT ].spell_duration_fixed;						

	SpellBook[ TTSI_DMG ].spell_id = 39047;											//cleave
	SpellBook[ TTSI_DMG ].spell_cooldown = 7000 - small_level_index*100;						
	SpellBook[ TTSI_DMG ].spell_global_cooldown = 2000;				
	SpellBook[ TTSI_DMG ].cast_chance = 100;
	SpellBook[ TTSI_DMG ].target_min_range_sq = 0;
	SpellBook[ TTSI_DMG ].target_min_range_sq *= SpellBook[ 2 ].target_min_range_sq;
	SpellBook[ TTSI_DMG ].target_max_range_sq = 7;
	SpellBook[ TTSI_DMG ].target_max_range_sq *= SpellBook[ 2 ].target_max_range_sq;
//	SpellBook[ TTSI_DMG ].spell_effect_scale_fixed[0] = small_level_index * 100 / 4;
}

//this NPC is spawned in the midle of the arena to do everything :P
class AITankTowerReplacer : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AITankTowerReplacer);

	//constructor
    AITankTowerReplacer(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
		AI_TANK_TOWER *new_interface = new AI_TANK_TOWER;
		new_interface->Init(_unit,AITYPE_SOCIAL,MOVEMENTTYPE_NONE);
		_unit->ReplaceAIInterface( (AIInterface *) new_interface );
		_unit->m_canRegenerateHP = false;
	}
};

void RegisterAITankTower(ScriptMgr * mgr)
{
	 mgr->register_creature_script( 123466, &AITankTowerReplacer::Create );
	 mgr->register_creature_script( 123467, &AITankTowerReplacer::Create );
	 //adjust stun targetting. Very bad move to do :p
	 SpellEntry *sp = dbcSpell.LookupEntryForced( 46383 );
	 if( sp )
	 {
		 sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
		 sp->Effect[ 1 ] = SPELL_EFFECT_NULL;
	 }
}