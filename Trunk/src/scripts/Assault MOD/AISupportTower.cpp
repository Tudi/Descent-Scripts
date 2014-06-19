#include "StdAfx.h"
#include "Setup.h"
#include "global.h"

//	Does not Damage
//	Adds support buffs to friendly players
//	Heals neighbours
//	Nerfs Enemy
//	Fast Regen
//	Weak Unit

class AI_SUPPORT_TOWER : public GeneralTower, public AIInterface
{
public:
	void            Init(Unit *un, AIType at, MovementType mt);
	void            Update(uint32 p_time);
private:
	void			EventLevelUp();
	bool			TryBeneficCast( Unit *target );
	bool			TryNerfCast( Unit *target );
	void			CastSpellBookSpell( uint32 SpellBookPage, Unit *target );
	bool			MyIsAttackable( Object *a, Object *b)
	{
		if( a && a->m_faction && b && b->m_faction && a->m_faction->ID != b->m_faction->ID )
			return true;
		return false;
	}
};

void AI_SUPPORT_TOWER::Init(Unit *un, AIType at, MovementType mt)
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
	//somehow signal that we are a tower
	un->m_special_state |= UNIT_STATE_BANISH;
}

void AI_SUPPORT_TOWER::CastSpellBookSpell( uint32 SpellBookPage, Unit *target )
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

bool AI_SUPPORT_TOWER::TryBeneficCast( Unit *target )
{
	if( target->IsPlayer() && TickNow > SpellInstances[ STSI_TEAM_BUFF1 ].cooldown_finished )
	{
		Aura *a = target->FindAura( SpellBook[ STSI_TEAM_BUFF1 ].spell_id );
		if( a != NULL && a->GetDuration() < SpellBook[ STSI_TEAM_BUFF1 ].spell_duration_fixed )
		{
			a->Remove();
			a = NULL;
		}
		if( a == NULL )
		{
			CastSpellBookSpell( STSI_TEAM_BUFF1, target );
			return true;
		}
	}
	if( target->IsPlayer() && TickNow > SpellInstances[ STSI_TEAM_BUFF2 ].cooldown_finished )
	{
		Aura *a = target->FindAura( SpellBook[ STSI_TEAM_BUFF2 ].spell_id );
		if( a != NULL && a->GetDuration() < SpellBook[ STSI_TEAM_BUFF2 ].spell_duration_fixed )
		{
			a->Remove();
			a = NULL;
		}
		if( a == NULL )
		{
			CastSpellBookSpell( STSI_TEAM_BUFF2, target );
			return true;
		}
	}
	if( target->GetUInt32Value( UNIT_FIELD_HEALTH ) <  target->GetUInt32Value( UNIT_FIELD_MAXHEALTH )&& TickNow > SpellInstances[ STSI_TEAM_HEAL ].cooldown_finished )
	{
		Aura *a = target->FindAura( SpellBook[ STSI_TEAM_HEAL ].spell_id );
		if( a != NULL && a->GetDuration() < SpellBook[ STSI_TEAM_HEAL ].spell_duration_fixed )
		{
			a->Remove();
			a = NULL;
		}
		if( a == NULL )
		{
			CastSpellBookSpell( STSI_TEAM_HEAL, target );
			return true;
		}
	}
	//try to remove nerf so only weakest would be active on our players near us
//	target->RemoveAura( 14535 );
	return false;
}

bool AI_SUPPORT_TOWER::TryNerfCast( Unit *target )
{
//	TickNow = GetTickCount();
	if( target->IsPlayer() && TickNow > SpellInstances[ STSI_ENEMY_NERF1 ].cooldown_finished 
		)
	{
		Aura *a = target->FindAura( SpellBook[ STSI_ENEMY_NERF1 ].spell_id );
		if( a != NULL && a->GetDuration() < SpellBook[ STSI_ENEMY_NERF1 ].spell_duration_fixed )
		{
			a->Remove();
			a = NULL;
		}
		if( a == NULL )
		{
			CastSpellBookSpell( STSI_ENEMY_NERF1, target );
			return true;
		}
	}
	if( target->IsPlayer()	&& TickNow > SpellInstances[ STSI_ENEMY_NERF2 ].cooldown_finished 
		&& RollDiceForSpellCast( STSI_ENEMY_NERF2 ) == true 
		)
	{
		Aura *a = target->FindAura( SpellBook[ STSI_ENEMY_NERF2 ].spell_id );
		if( a != NULL && a->GetDuration() < SpellBook[ STSI_ENEMY_NERF1 ].spell_duration_fixed )
		{
			a->Remove();
			a = NULL;
		}
		if( a == NULL )
		{
			CastSpellBookSpell( STSI_ENEMY_NERF2, target );
			return true;
		}
	}
	return false;
}

void AI_SUPPORT_TOWER::Update(uint32 p_time)
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

	//see if we could find a new target
	if( NextTargetUpdateStamp < TickNow )
	{
		//do not spam search cause it might be slow
		NextTargetUpdateStamp = TickNow + TOWER_TARGET_AQUISITION_INTERVAL;

		bool self_heal = true;
		//our max visible list
		for( set<Object*>::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
		{
			if( (*itr) == NULL || (*itr)->IsInWorld() == false || (*itr)->IsUnit() == false 
				|| (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH ) == 0 || static_cast<Unit *>(*itr)->isAlive() == false
				|| (*itr)->GetEntry() == 123461	//dummy tower should not be targeted
				)
				continue;

			if( IsInrange( m_Unit, (*itr), cur_range_sq ) == false )
				continue;

//			if( isAttackable( m_Unit, (*itr) ) == true )
			if( MyIsAttackable( m_Unit, (*itr) ) == true )
			{
				if( TryNerfCast( static_cast<Unit*>(*itr) ) )
				{
					//count tower contribution to fight
					if( (*itr)->IsPlayer() )
						SpellCastsOnPlayers+=20;
					self_heal = false;
					break;
				}
			}
			else
			{
				if( TryBeneficCast( static_cast<Unit*>(*itr) ) )
				{
					//count tower contribution to fight
//					if( (*itr)->IsPlayer() )
//						SpellCastsOnPlayers+=20;
					self_heal = false;
					break;
				}
				if( static_cast<Unit*>(*itr)->GetPowerType() == POWER_TYPE_MANA )
				{
					(*itr)->ModUnsigned32Value( UNIT_FIELD_POWER1 , in_combat_P_regen );
					if( (*itr)->GetUInt32Value( UNIT_FIELD_POWER1 ) > (*itr)->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) )
						(*itr)->SetUInt32Value( UNIT_FIELD_POWER1, (*itr)->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );
				}
			}
		}
		_UpdateTargets();	//requered to clean out hate list

		if( self_heal == true )
			TryBeneficCast( m_Unit );
	}
}

//as towers gain level they get stronger and stronger
void AI_SUPPORT_TOWER::EventLevelUp()
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
	uint32 small_level_index = (level_now % 10)+1;	//cause each BG has top 10 level bracket

	//health regen is good
	in_combat_H_regen = small_level_index * 40;		//x HP / 2 sec even in combat
	//power regen is good
	in_combat_P_regen = small_level_index * 150;	//y P / 2 sec

	//health boost / level is small
	float health_now_pct = (float)m_Unit->GetUInt32Value( UNIT_FIELD_HEALTH ) / m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH );
	uint32 health_new = m_Unit->GetUInt32Value( UNIT_FIELD_BASE_HEALTH ) + small_level_index * 10000;
	m_Unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, health_new );
	m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH, health_new * health_now_pct );

	SpellBook[ STSI_TEAM_BUFF1 ].spell_id = 26393;								//Elune's Blessing
	SpellBook[ STSI_TEAM_BUFF1 ].spell_cooldown = 20000 / small_level_index;						
	SpellBook[ STSI_TEAM_BUFF1 ].spell_global_cooldown = 10000 / small_level_index;				
	SpellBook[ STSI_TEAM_BUFF1 ].cast_chance = 100;
	SpellBook[ STSI_TEAM_BUFF1 ].spell_effect_scale_fixed[0] = 20 + small_level_index*5;	
	SpellBook[ STSI_TEAM_BUFF1 ].spell_duration_fixed = 5*60*1000*small_level_index;	

	SpellBook[ STSI_TEAM_BUFF2 ].spell_id = 467;								//Thorns
	SpellBook[ STSI_TEAM_BUFF2 ].spell_cooldown = 20000 / small_level_index;						
	SpellBook[ STSI_TEAM_BUFF2 ].spell_global_cooldown = 10000 / small_level_index;				
	SpellBook[ STSI_TEAM_BUFF2 ].cast_chance = 100;
	SpellBook[ STSI_TEAM_BUFF2 ].spell_effect_scale_fixed[0] = 250 + small_level_index * 70;
	SpellBook[ STSI_TEAM_BUFF2 ].spell_duration_fixed = 5*60*1000*small_level_index;	

	SpellBook[ STSI_TEAM_HEAL ].spell_id = 139;									//Renew
	SpellBook[ STSI_TEAM_HEAL ].spell_cooldown = 20000 / small_level_index;						
	SpellBook[ STSI_TEAM_HEAL ].spell_global_cooldown = 10000 / small_level_index;				
	SpellBook[ STSI_TEAM_HEAL ].cast_chance = 100;
	SpellBook[ STSI_TEAM_HEAL ].spell_effect_scale_fixed[0] = 25 + small_level_index * 45;
	SpellBook[ STSI_TEAM_HEAL ].spell_duration_fixed = 2*60*1000*small_level_index;

	SpellBook[ STSI_ENEMY_NERF1 ].spell_id = 14535;								//Dredge Sickness
	SpellBook[ STSI_ENEMY_NERF1 ].spell_cooldown = 60000 / small_level_index;						
	SpellBook[ STSI_ENEMY_NERF1 ].spell_global_cooldown = 10000 / small_level_index;				
	SpellBook[ STSI_ENEMY_NERF1 ].cast_chance = 100;
	SpellBook[ STSI_ENEMY_NERF1 ].spell_effect_scale_fixed[0] = -30 - small_level_index*5;		//stronger then pro buff
	SpellBook[ STSI_ENEMY_NERF1 ].spell_duration_fixed = 5*60*1000*small_level_index;	

	SpellBook[ STSI_ENEMY_NERF2 ].spell_id = 5782;								//fear
	SpellBook[ STSI_ENEMY_NERF2 ].spell_cooldown = 40000 - small_level_index*500;						
	SpellBook[ STSI_ENEMY_NERF2 ].spell_global_cooldown = 10000 / small_level_index;				
	SpellBook[ STSI_ENEMY_NERF2 ].cast_chance = 5;
	SpellBook[ STSI_ENEMY_NERF2 ].spell_duration_fixed = 5*1000*small_level_index;	
}

//this NPC is spawned in the midle of the arena to do everything :P
class AISupportTowerReplacer : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AISupportTowerReplacer);

	//constructor
    AISupportTowerReplacer(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
		AI_SUPPORT_TOWER *new_interface = new AI_SUPPORT_TOWER;
		new_interface->Init(_unit,AITYPE_SOCIAL,MOVEMENTTYPE_NONE);
		_unit->ReplaceAIInterface( (AIInterface *) new_interface );
		_unit->m_canRegenerateHP = false;
	}
};

void RegisterAISupportTower(ScriptMgr * mgr)
{
	 mgr->register_creature_script( 123464, &AISupportTowerReplacer::Create );
	 mgr->register_creature_script( 123465, &AISupportTowerReplacer::Create );
	 //adjust dredge sickness targetting. Very bad move to do :p
	 SpellEntry *sp = dbcSpell.LookupEntryForced( 14535 );
	 if( sp )
		 sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
	 //adjust Elune's Blessing targetting. Very bad move to do :p
	 sp = dbcSpell.LookupEntryForced( 26393 );
	 if( sp )
		 sp->EffectImplicitTargetA[0] = EFF_TARGET_DUEL;
	 //adjust Thorns targetting. Very bad move to do :p
	 sp = dbcSpell.LookupEntryForced( 467 );
	 if( sp )
		 sp->EffectImplicitTargetA[0] = EFF_TARGET_DUEL;
	 //adjust Renew targetting. Very bad move to do :p
	 sp = dbcSpell.LookupEntryForced( 139 );
	 if( sp )
		 sp->EffectImplicitTargetA[0] = EFF_TARGET_DUEL;
}