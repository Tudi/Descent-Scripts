#include "StdAfx.h"
#include "Setup.h"

class DeathKnightDeathGate : public GameObjectAIScript
{
public:
    DeathKnightDeathGate(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
    static GameObjectAIScript *Create(GameObject * GO) { return new DeathKnightDeathGate(GO); }

    void OnActivate(Player * pPlayer)
    {
		if( pPlayer->getClass() == DEATHKNIGHT )
			pPlayer->SafeTeleport(0, 0, 2357.95f, -5662.73f, 426.028f, 0.56313f);
    }
};

class DancingRuneWeapon : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(DancingRuneWeapon);

   float my_pct_mod;
   AI_Spell owner_spells[10];
   uint32	stored_spells;

   void SetupDefaultSpellbookSpell(uint32 page)
   {
		owner_spells[page].agent = AGENT_SPELL;
		owner_spells[page].entryId = 0;	
		owner_spells[page].global_cooldown = 2000;
		owner_spells[page].cooldown = 3000;
		owner_spells[page].initial_cooldown = 0;
		owner_spells[page].max_hp_pct = 0;
		owner_spells[page].maxrange = 30;
		owner_spells[page].min_hp_pct = 0;
		owner_spells[page].minrange = 0;
		owner_spells[page].procChance = 50;
		owner_spells[page].procCount = 0;
		owner_spells[page].shout_before_cast = NULL;
		owner_spells[page].spell = NULL;
		owner_spells[page].spelltargetType = TTYPE_SINGLETARGET;
   }
   //constructor
    DancingRuneWeapon(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 2000 );
    }
	void OnLoad()
	{
		//only handle spell created mobs
		if( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) == 0 )
			return;
		//pct mod is the spell value that created us
//		my_pct_mod = 0.5f;
		my_pct_mod = 1.0f;

		Unit *owner = _unit->GetTopOwner();
		if( owner )
		{
			int mainhandfield = PLAYER_VISIBLE_ITEM_1_ENTRYID + (EQUIPMENT_SLOT_MAINHAND * (PLAYER_VISIBLE_ITEM_2_ENTRYID - PLAYER_VISIBLE_ITEM_1_ENTRYID));
			_unit->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_ID, owner->GetUInt32Value( mainhandfield ) );
			int moffhandfield = PLAYER_VISIBLE_ITEM_1_ENTRYID + (EQUIPMENT_SLOT_OFFHAND * (PLAYER_VISIBLE_ITEM_2_ENTRYID - PLAYER_VISIBLE_ITEM_1_ENTRYID));
			_unit->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_ID_1, owner->GetUInt32Value( moffhandfield ) );
			_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, (uint32)(owner->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) * my_pct_mod) );
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			_unit->SetFloatValue( UNIT_FIELD_MAXDAMAGE, owner->GetFloatValue( UNIT_FIELD_MAXDAMAGE ) * my_pct_mod );
			_unit->SetFloatValue( UNIT_FIELD_MINDAMAGE, owner->GetFloatValue( UNIT_FIELD_MINDAMAGE ) * my_pct_mod  );
			_unit->SetFloatValue( UNIT_FIELD_MAXOFFHANDDAMAGE, owner->GetFloatValue( UNIT_FIELD_MAXOFFHANDDAMAGE ) * my_pct_mod );
			_unit->SetFloatValue( UNIT_FIELD_MINOFFHANDDAMAGE, owner->GetFloatValue( UNIT_FIELD_MINOFFHANDDAMAGE ) * my_pct_mod  );
			_unit->SetUInt32Value( UNIT_FIELD_ATTACK_POWER, owner->GetUInt32Value( UNIT_FIELD_ATTACK_POWER ) );
			_unit->SetUInt32Value( UNIT_FIELD_POWER1 + POWER_TYPE_RUNIC, owner->GetUInt32Value( UNIT_FIELD_POWER1 + POWER_TYPE_RUNIC ) );
			_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, owner->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME ) );
		}	
 		stored_spells = 0;
		SetupDefaultSpellbookSpell( stored_spells );
		owner_spells[stored_spells].spell = dbcSpell.LookupEntryForced( 49909 ); //icy touch
		stored_spells++;
	}
    void AIUpdate()
    {
		//check if casting
		if( _unit->isCasting() )
			return;
		//check if we still have an owner
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner == NULL )
			return;
		//do we have a target ? If not then try to agro owner selection :P
		Unit *target = _unit->GetAIInterface()->GetNextTarget();
		if( target == NULL && owner->GetSelection() )
		{
			target = _unit->GetMapMgr()->GetUnit( owner->GetSelection() );
			if( target && isAttackable( _unit, target ) )
				_unit->GetAIInterface()->AttackReaction( target, 1 );
		}
		if( owner->last_casted_enemy_spell != NULL 
			&& owner->last_casted_enemy_spell->Id != 49028 //do not let dancing rune weapon cast dancing rune weapon
			&& owner->last_casted_enemy_spell->Id != 49576 //Death Grip - spamming this kinda "incapacitates" the target
			)	
		{
			stored_spells = 0;
			//keep building up a spellbook
			SetupDefaultSpellbookSpell( stored_spells );
			owner_spells[ stored_spells ].global_cooldown = owner->last_casted_enemy_spell->CategoryRecoveryTime;
			owner_spells[ stored_spells ].maxrange = sqrt( owner->last_casted_enemy_spell->base_range_or_radius_sqr );
			owner_spells[ stored_spells ].spell = owner->last_casted_enemy_spell;
			owner_spells[ stored_spells ].cooldown = owner->last_casted_enemy_spell->RecoveryTime;
			stored_spells++;
		}
		if( stored_spells > 0 )
		{
			_unit->GetAIInterface()->SetNextSpell( &owner_spells[ stored_spells - 1 ] );
			stored_spells--;
		}
	}
};


class ArmyOfDead : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(ArmyOfDead);

   //constructor
    ArmyOfDead(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//only handle spell created mobs
		if( pCreature->GetUInt32Value( UNIT_FIELD_CREATEDBY ) == 0 )
			return;

		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner == NULL )
			return;
//		float parent_bonus = 5.0f + owner->GetAP() * 6.0f / 100.0f;	//someone said so
		float parent_bonus = 5.0f + owner->GetAP() * 5.0f / 100.0f;	//someone said so
		if( parent_bonus < 50 )
			parent_bonus = 50;
		pCreature->SetFloatValue(UNIT_FIELD_MINDAMAGE, parent_bonus);
		pCreature->SetFloatValue(UNIT_FIELD_MAXDAMAGE, parent_bonus);
		pCreature->BaseAttackType = SCHOOL_SHADOW;
		float x,y,z;
		x = owner->GetPositionX()+rand()%10-rand()%10;
		y = owner->GetPositionY()+rand()%10-rand()%10;
		z = owner->GetPositionZ();
		pCreature->SetPosition(x,y,z,0.0f,true);
//		pCreature->GetAIInterface()->SetFollowDistance(3.0f + (rand()%20)/10.0f);	//randomized a bit ok ?
		uint64 guid = pCreature->GetGUID();
		Unit *t = pCreature;
		//make it so spell cast happens after push to world
//		sEventMgr.AddEvent(t, &Unit::EventCastSpell2, guid, (uint32)50142,	EVENT_CREATURE_UPDATE, 2000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
//		pCreature->CastSpell(pCreature,50142,true);
   }
	void OnLoad()
	{
		_unit->CastSpell( _unit, 50142, true );
	}
};

class BloodWorms : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(BloodWorms);

   //constructor
    BloodWorms(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//only handle spell created mobs
		if( pCreature->GetUInt32Value( UNIT_FIELD_CREATEDBY ) == 0 )
			return;

		//health from wowhead
		uint32 health_per_level = 300/61;
		pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH, health_per_level * pCreature->getLevel() + 100 );
		pCreature->SetUInt32Value(UNIT_FIELD_HEALTH,pCreature->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		//low dmg
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner != NULL )
		{
//			float base_dmg = 5.0f + owner->GetAP() * 6.0f / 1000.0f * 169.0f / 100.0f;	//formula from wowhead;
			float base_dmg = 5.0f + owner->GetAP() * 3.0f / 1000.0f * 169.0f / 100.0f;	//formula from wowhead;
			pCreature->SetFloatValue( UNIT_FIELD_MINDAMAGE, base_dmg );
			pCreature->SetFloatValue( UNIT_FIELD_MAXDAMAGE, base_dmg * 3 / 2 );
		}
		pCreature->BaseAttackType = SCHOOL_SHADOW;
		//register spell that will regen health for caster on melee hit
		SpellEntry *sp = dbcSpell.LookupEntryForced( 50454 );	//Health Leech
		ProcTriggerSpell pts( sp, NULL );
		pts.caster = _unit->GetGUID();
		pts.spellId = sp->Id;	
		pts.procFlags = PROC_ON_MELEE_ATTACK;
		pts.created_with_value = sp->eff[0].EffectBasePoints + 1;
		pts.procCharges = 0;
		pts.procChance = 25;
		_unit->RegisterScriptProcStruct(pts);
   }
};

#define TOWER_SUPPORT_BASE_RANGE 14
#if 0
#define HEALTH_SHIELD_HEALTH_RESTORE_PCT 75
#define HEALTH_SHIELD_DMG_CONVERT_PCT 100
#define HEALTH_SHIELD_MAX_SHIELDED_UNIT_COUNT 45

struct UnitHealthStateStore
{
   Unit   *unit;            //to identify who we saved state for
   uint32   SavedHealthState;   //last time the health was like this
};

class HealthShield : public AIInterface
{
public:
   void         Init(Unit *un, AIType at, MovementType mt);
   void         Update(uint32 p_time);
private:
   UnitHealthStateStore   ShieldedUnits[ HEALTH_SHIELD_MAX_SHIELDED_UNIT_COUNT ];
   float		spawn_x,spawn_y,spawn_z;
   float 		cur_range_sq;
   uint32		next_update;	
};

void HealthShield::Init(Unit *un, AIType at, MovementType mt)
{
   //run the original init and we add our custom stuff too
   AIInterface::Init(un,at,mt);
   //mark where we was spawned. Towers do not move
   spawn_x = un->GetPositionX();
   spawn_y = un->GetPositionY();
   spawn_z = un->GetPositionZ();
   cur_range_sq = TOWER_SUPPORT_BASE_RANGE * TOWER_SUPPORT_BASE_RANGE;
   memset( ShieldedUnits, 0, sizeof( ShieldedUnits ) );
   //somehow signal that we are a tower
   next_update = 0;
}

void HealthShield::Update(uint32 p_time)
{
	//it is not called when i made this but better be safe
	if( m_Unit->IsDead() )
		return;

	//spam update protection
	if( next_update > GetTickCount() )
		return;
	next_update = GetTickCount() + 1000;

	//sanity check
	if( m_Unit->GetMapMgr() == NULL )
		return;

	Player *owner = m_Unit->GetMapMgr()->GetPlayer( m_Unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
	if( owner == NULL )
	{
		if( m_Unit->IsCreature() )
			static_cast<Creature*>(m_Unit)->Despawn( 0, 0 );
		return;
	}

   //in case they managed somehow to move the tower then just put it back. Ex : deathgrip
   if( spawn_x != m_Unit->GetPositionX() )
   {
      m_Unit->SetPosition( spawn_x,spawn_y,spawn_z,m_Unit->GetOrientation(), false );
      WorldPacket data;
      m_Unit->BuildHeartBeatMsg( &data );
      m_Unit->SendMessageToSet( &data, false );
   }

	uint32 updated_slots[ HEALTH_SHIELD_MAX_SHIELDED_UNIT_COUNT ];
	memset( updated_slots, 0, sizeof( updated_slots ) );
	//our max visible list
	for( InRangeSet::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
	{
		if( (*itr) == NULL || (*itr)->IsInWorld() == false || (*itr)->IsUnit() == false 
		|| (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH ) == 0 || static_cast<Unit *>(*itr)->isAlive() == false
		)
		continue;

		//no self targetting
		if( (*itr) == m_Unit )
			continue;

		if( IsInrange( m_Unit, (*itr), cur_range_sq ) == false )
			continue;

		//check if we are shielding this unit
		if( 
			(*itr) == owner
			|| ( isAttackable( m_Unit, (*itr) ) == false 
				//must be raid member
				&& (*itr)->IsPlayer() && owner->GetGroup() && owner->GetGroup()->HasMember( static_cast<Player*>(*itr) )
				)
			)
		{
			bool foundit = false;
			for( uint32 i=0;i<HEALTH_SHIELD_MAX_SHIELDED_UNIT_COUNT;i++ )
				if( ShieldedUnits[i].unit == (*itr) )
				{
					//check if we need to restore health to him and apply DMG to us
					if( (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH ) < ShieldedUnits[i].SavedHealthState )
					{
						 uint32   health_diff = ShieldedUnits[i].SavedHealthState - (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH );
						 uint32   restore = health_diff * HEALTH_SHIELD_HEALTH_RESTORE_PCT / 100;
						 (*itr)->ModUnsigned32Value( UNIT_FIELD_HEALTH, restore );
						 uint32   DMG_self = health_diff * HEALTH_SHIELD_DMG_CONVERT_PCT / 100;
						 m_Unit->DealDamage( m_Unit, DMG_self, 0, 0, 0, true );
						 updated_slots[ i ] = 1;
					}
					ShieldedUnits[i].SavedHealthState = (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH );
					foundit = true;
					break;
				}
			//not yet shielded -> add it
			if( foundit == false )
			   for( uint32 i=0;i<HEALTH_SHIELD_MAX_SHIELDED_UNIT_COUNT;i++)
				  if( ShieldedUnits[i].unit == NULL )
				  {
					 ShieldedUnits[i].unit = static_cast<Unit*>(*itr);
					 ShieldedUnits[i].SavedHealthState = (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH );
					 updated_slots[ i ] = 1;
					 break;
				  }
		}
	}
	//remove units from our list that we are not shielded anymore
	for( uint32 i=0;i<HEALTH_SHIELD_MAX_SHIELDED_UNIT_COUNT;i++)
		if( updated_slots[i] != 1 )
			ShieldedUnits[i].unit = NULL;
}
#endif

//this NPC is spawned in the midle of the arena to do everything :P
class AIHealthShieldReplacer : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(AIHealthShieldReplacer);

   //constructor
    AIHealthShieldReplacer(Creature* pCreature) : CreatureAIScript(pCreature)
    {
      //we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
		_unit->GetAIInterface()->Init(_unit,AITYPE_SOCIAL,MOVEMENTTYPE_NONE);
		_unit->CastSpell( _unit, 50461, true );
//		HealthShield *new_interface = new HealthShield;
//		new_interface->Init(_unit,AITYPE_SOCIAL,MOVEMENTTYPE_NONE);
//		_unit->ReplaceAIInterface( (AIInterface *) new_interface );
		_unit->m_canRegenerateHP = false;
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
		_unit->bInvincible = true;
	    _unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_DEAD);
	    _unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 10000 );
	    _unit->SetUInt32Value( UNIT_FIELD_HEALTH, 10000 );
	    _unit->SetUInt32Value( UNIT_CHANNEL_SPELL, 50461 );
	    _unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, 11686 );
	    _unit->SetFloatValue( OBJECT_FIELD_SCALE_X, TOWER_SUPPORT_BASE_RANGE / 7.0f );
   }
};

class SummonGargoyle : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(SummonGargoyle);

   AI_Spell our_attack_spell;

   //constructor
    SummonGargoyle(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//only handle spell created mobs
		if( pCreature->GetUInt32Value( UNIT_FIELD_CREATEDBY ) == 0 )
			return;

		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->disable_ranged = true;
		}

		our_attack_spell.agent = AGENT_SPELL;
		our_attack_spell.entryId = 0;	
		our_attack_spell.global_cooldown = 2000;
		our_attack_spell.cooldown = 2000;
		our_attack_spell.initial_cooldown = 0;
		our_attack_spell.max_hp_pct = 0;
		our_attack_spell.maxrange = 30;
		our_attack_spell.min_hp_pct = 0;
		our_attack_spell.minrange = 0;
		our_attack_spell.procChance = 100;
		our_attack_spell.procCount = 0;
		our_attack_spell.shout_before_cast = NULL;
		our_attack_spell.spell = dbcSpell.LookupEntryForced(51963);	//not sure about this value exactly
		our_attack_spell.spelltargetType = TTYPE_SINGLETARGET;

		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner )
		{
			for( uint32 x = 0; x < 7; x++ )
				SafeCreatureCast( _unit )->ModDamageDone[x] += 100 + float2int32( owner->GetUInt32Value( UNIT_FIELD_ATTACK_POWER ) * 0.42f * 2.0f);	// the 2 comes from the spell coefficient to be nullified
			//inherit 30% of stamina
			uint32 OwnerStaminaInherit = owner->GetStat( STAMINA ) * 30 / 100;
			uint32 OwnerInheritHP = float2int32(OwnerStaminaInherit * 13.9088f);
			_unit->SetMaxHealth( _unit->GetMaxHealth() + OwnerInheritHP );
			_unit->SetHealth( _unit->GetMaxHealth() );
		}

		RegisterAIUpdateEvent( 2000 );	//instead of melee attack we will cast spells

   }
    void AIUpdate()
    {
		//check if casting
		if( _unit->isCasting() )
			return;
		//check if we still have an owner
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner == NULL )
			return;
		//do we have a target ? If not then try to agro owner selection :P
		Unit *target = _unit->GetAIInterface()->GetNextTarget();
		if( target == NULL && owner->GetSelection() )
		{
			target = _unit->GetMapMgr()->GetUnit( owner->GetSelection() );
			if( target && isAttackable( _unit, target ) )
				_unit->GetAIInterface()->AttackReaction( target, 1 );
		}
		if( target && CheckLOS( _unit->GetMapId(), _unit->GetPositionNC(), target->GetPositionNC() ) ) 
			_unit->GetAIInterface()->SetNextSpell( &our_attack_spell );
	}
};

void SetupDKSpells(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(190942, &DeathKnightDeathGate::Create);
	mgr->register_creature_script( 27893, &DancingRuneWeapon::Create);	//mirror image
	mgr->register_creature_script( 28017, &BloodWorms::Create);	//bloodworms
	mgr->register_creature_script( 24207, &ArmyOfDead::Create);	//army of the dead 
	mgr->register_creature_script( 28306, &AIHealthShieldReplacer::Create);	//Anti-Magic Zone
	mgr->register_creature_script( 27829, &SummonGargoyle::Create);	//Summon Gargoyle
}
