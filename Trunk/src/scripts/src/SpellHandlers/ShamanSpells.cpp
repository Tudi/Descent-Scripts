/****************************************************************************
 *
 * SpellHandler Plugin
 * Copyright (c) 2007 Team arcemu
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons,
 * 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
 *
 * EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU
 * ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES
 * ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 */

#include "StdAfx.h"
#include "Setup.h"

/************************************************************************/
/* Spell Defs                                                           */
/************************************************************************/

bool RockbiterWeapon(uint32 i, Spell* pSpell)
{
    uint32 enchantment_entry = 0;
    switch(pSpell->GetProto()->RankNumber)
    {
    case 1:
        enchantment_entry = 3021;
        break;
    case 2:
        enchantment_entry = 3024;
        break;
    case 3:
        enchantment_entry = 3027;
        break;
    case 4:
        enchantment_entry = 3030;
        break;
    case 5:
        enchantment_entry = 3033;
        break;
    case 6:
        enchantment_entry = 3036;
        break;
    case 7:
        enchantment_entry = 3039;
        break;
    case 8:
        enchantment_entry = 3042;
        break;
    case 9:
        enchantment_entry = 3018;
        break;
    }

    if(!enchantment_entry || !pSpell->p_caster)
        return true;

    Item * item = pSpell->p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
    EnchantEntry * enchant = dbcEnchant.LookupEntry(enchantment_entry);
    if(!item || !enchant)
        return true;

    int32 Slot = item->HasEnchantment(enchant->Id);
    if(Slot >= 0)
        item->ModifyEnchantmentTime(Slot, 1800);
    else
    {
		//check if enchantment slot 1 is taken. If there was no enchantment there function will quit
		item->RemoveEnchantment(1);
		//this will also apply bonuses for us
        Slot = item->AddEnchantment(enchant, 1800, false, true, false, 1);   // 5min
        if(Slot < 0) return true;
    }

    sLog.outDebug("ShamanSpells.cpp :: Rockbiter Weapon Rank %u, enchant %u, slot %u", pSpell->GetProto()->RankNumber, enchantment_entry, Slot);
    
    return true;
}

class StoneClawTotem : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(StoneClawTotem);
    StoneClawTotem(Creature* pCreature) : CreatureAIScript(pCreature)   {   }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner->IsPlayer() == false )
			return;
		//get all other totems of the owner and place a shield on them
		for(uint32 i=0;i<4;i++)
			if( SafePlayerCast( owner )->m_TotemSlots[i] != NULL && SafePlayerCast( owner )->m_TotemSlots[i] != _unit )
				_unit->CastSpell( owner->m_TotemSlots[i], 55328, true );
	}
};

class EarthElementalTotem : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EarthElementalTotem);
	uint64 MySummon;
    EarthElementalTotem(Creature* pCreature) : CreatureAIScript(pCreature)   {   }
	void OnLoad()
	{
		Unit *caster = _unit->GetTopOwner();
		if( caster == _unit )	//wtf self cast ? looping much ?
			return;
		//timer should not reach this value thus not cast this spell again
		_unit->GetAIInterface()->spell_global_cooldown_stamp = 0xEFFFFFFF;
		uint32 summon_duration = 120000;
		if( caster->GetCurrentSpell() )
			summon_duration = caster->GetCurrentSpell()->GetDuration();
		Unit *ourslave=_unit->create_guardian( 15352, summon_duration );
		if(ourslave)
		{
			SafeCreatureCast(ourslave)->ResistanceModPct[NATURE_DAMAGE]=666;//we should be imune to nature dmg. This can be also set in db
			SafeCreatureCast(ourslave)->SchoolImmunityAntiEnemy[NATURE_DAMAGE]++;
			SafeCreatureCast(ourslave)->BaseAttackType = SCHOOL_NATURE;
			//- Earth Stun (37982)
			//- taunt
			// we want the elemental to have the same pvp flag as the shaman who popped the totem
			if( caster->IsPvPFlagged() )
				ourslave->SetPvPFlag();
			if( caster->IsFFAPvPFlagged() )
				ourslave->SetFFAPvPFlag();
			MySummon = ourslave->GetGUID();
		}
		_unit->GetAIInterface()->totemspell = NULL;
	}
	void OnRemoveFromWorld()
	{
		Unit *ourslave = _unit->GetMapMgr()->GetUnit( MySummon );
		if( ourslave && ourslave->IsCreature() )
			SafeCreatureCast( ourslave )->SummonExpire();
	}
};

class FireElementalTotem : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FireElementalTotem);
	uint64 MySummon;
    FireElementalTotem(Creature* pCreature) : CreatureAIScript(pCreature)  {   }
	void OnLoad()
	{
		Unit *caster = _unit->GetTopOwner();
		if( caster == _unit )	//wtf self cast ? looping much ?
			return;
		//timer should not reach this value thus not cast this spell again
		_unit->GetAIInterface()->spell_global_cooldown_stamp = 0xEFFFFFFF;
		uint32 summon_duration = 120000;
		if( caster->GetCurrentSpell() )
			summon_duration = caster->GetCurrentSpell()->GetDuration();
		Unit *ourslave=_unit->create_guardian( 15438, summon_duration );
		if(ourslave)
		{
			SafeCreatureCast(ourslave)->ResistanceModPct[FIRE_DAMAGE]=666;//we should be imune to nature dmg. This can be also set in db
			SafeCreatureCast(ourslave)->SchoolImmunityAntiEnemy[FIRE_DAMAGE]++;
			SafeCreatureCast(ourslave)->BaseAttackType = SCHOOL_FIRE;
//			- also : select * from dbc_spell where name like "%fire blast%"
//			- also : select * from dbc_spell where name like "%fire nova"
			// we want the elemental to have the same pvp flag as the shaman who popped the totem
			if( caster->IsPvPFlagged() )
				ourslave->SetPvPFlag();
			if( caster->IsFFAPvPFlagged() )
				ourslave->SetFFAPvPFlag();
			MySummon = ourslave->GetGUID();
			float LevelScale = caster->getLevel() / 85.0f;
			ourslave->SetFloatValue(UNIT_FIELD_MINDAMAGE, 4000 * LevelScale );
			ourslave->SetFloatValue(UNIT_FIELD_MAXDAMAGE, 6000 * LevelScale );
			ourslave->SetMaxHealth( float2int32( 60000 * LevelScale ) );
			ourslave->SetHealth( ourslave->GetMaxHealth() );
		}
		_unit->GetAIInterface()->totemspell = NULL;
	}
	void OnRemoveFromWorld()
	{
		Unit *ourslave = _unit->GetMapMgr()->GetUnit( MySummon );
		if( ourslave && ourslave->IsCreature() )
			SafeCreatureCast( ourslave )->SummonExpire();
	}
};

class FireElemental : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(FireElemental);

    FireElemental(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 7000 );
    }
	void AIUpdate()
	{
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		if( RandChance( 50 ) )
			_unit->CastSpell( tank, 12470 , false );	//Fire Nova
		else if( RandChance( 25 ) )
			_unit->CastSpell( tank, 57984 , false );	//Fire Blast
	}
};

class EarthElemental : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(EarthElemental);

    EarthElemental(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 5000 );
    }
	void AIUpdate()
	{
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		_unit->CastSpell( tank, 36213 , false );	//Angered Earth
	}
};

class HealingStreamTotem : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HealingStreamTotem);
    HealingStreamTotem(Creature* pCreature) : CreatureAIScript(pCreature) {}
	void OnLoad()
	{
		Unit *caster = _unit->GetTopOwner();
		if( caster == _unit )	//wtf self cast ? looping much ?
			return;
		if( caster->IsPlayer() == false )
			return;
		if( SafePlayerCast( caster )->HasGlyphWithID( GLYPH_SHAMAN_HEALING_STREAM_TOTEM ) )
			_unit->CastSpell( _unit, 8185, true ); //Elemental Resistance
	}
};

class SearingTotem : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(SearingTotem);

    SearingTotem(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 3000 );
		AIUpdate();
	}
	void AIUpdate()
	{
		//check if our owner has flame shock on nearby targets and try to agro him the most
		InrangeLoopExitAutoCallback AutoLock;
		for(InRangeSetRecProt::iterator itr = _unit->GetInRangeSetBegin( AutoLock ); itr != _unit->GetInRangeSetEnd(); ++itr)
		{
			Unit *target;
			if( (*itr)->IsUnit() )
				target = (Unit*)*itr;
			else
				continue;

			if( target->isAlive() == false || isAttackable( _unit, target) == false )
				continue;

			Aura *a = target->HasAuraWithNameHash( SPELL_HASH_FLAME_SHOCK, _unit->GetUInt64Value( UNIT_FIELD_CREATEDBY ), AURA_SEARCH_NEGATIVE );
			if( a )
			{
				uint32 Threath = 32000;
				Unit *tank = _unit->GetAIInterface()->GetNextTarget();
				if( tank != NULL )
					Threath += _unit->GetAIInterface()->getThreatByPtr( tank );
				//agro this new guy that has flame shock on it
				_unit->GetAIInterface()->AttackReaction( target, Threath );
				break;
			}
		}
	}
};

class ManaTideTotem : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(ManaTideTotem);

    ManaTideTotem(Creature* pCreature) : CreatureAIScript(pCreature)    { }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		_unit->SetMaxHealth( owner->GetMaxHealth() * 10 / 100 );
		_unit->SetHealth( _unit->GetMaxHealth() );
	}
};

class GroundingTotem : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(GroundingTotem);

    GroundingTotem(Creature* pCreature) : CreatureAIScript(pCreature)    { }
	void RemoveBuffFromTargets()
	{
		InrangeLoopExitAutoCallback AutoLock;
		for(InRangeSetRecProt::iterator itr = _unit->GetInRangeSetBegin( AutoLock ); itr != _unit->GetInRangeSetEnd(); ++itr)
		{
			Unit *target;
			if( (*itr)->IsUnit() )
				target = (Unit*)*itr;
			else
				continue;

			if( target->isAlive() == false || isAttackable( _unit, target) == true )
				continue;

			target->RemoveAuraByNameHash( SPELL_HASH_GROUNDING_TOTEM_EFFECT, _unit->GetGUID(), AURA_SEARCH_POSITIVE );
		}
	}
	void OnDied(Unit *mKiller)
	{
		RemoveBuffFromTargets();
		RemoveAIUpdateEvent();
	}
	void OnRemoveFromWorld()
	{
		RemoveBuffFromTargets();
	}
};


void SetupShamanSpells(ScriptMgr * mgr)
{
	mgr->register_creature_script( 3579, &StoneClawTotem::Create);			//StoneClas Totem
	mgr->register_creature_script( 15439, &FireElementalTotem::Create);		//Fire Elemental Totem
	mgr->register_creature_script( 15430, &EarthElementalTotem::Create);	//Earth Elemental Totem
	mgr->register_creature_script( 15352, &EarthElemental::Create);			//Earth Elemental
	mgr->register_creature_script( 15438, &FireElemental::Create);			//Fire Elemental
	mgr->register_creature_script( 3527, &HealingStreamTotem::Create);		//Healing stream totem
	mgr->register_creature_script( 2523, &SearingTotem::Create);			//Searing Totem
	mgr->register_creature_script( 10467, &ManaTideTotem::Create);			//ManaTide Totem
	mgr->register_creature_script( 5925, &GroundingTotem::Create);			//Grounding Totem

    mgr->register_dummy_spell(8017, &RockbiterWeapon); // rank 1
    mgr->register_dummy_spell(8018, &RockbiterWeapon); // rank 2
    mgr->register_dummy_spell(8019, &RockbiterWeapon); // rank 3
    mgr->register_dummy_spell(10399, &RockbiterWeapon);// rank 4
    mgr->register_dummy_spell(16314, &RockbiterWeapon);// rank 5
    mgr->register_dummy_spell(16315, &RockbiterWeapon);// rank 6
    mgr->register_dummy_spell(16316, &RockbiterWeapon);// rank 7
	mgr->register_dummy_spell(25479, &RockbiterWeapon);// rank 8
    mgr->register_dummy_spell(25485, &RockbiterWeapon);// rank 9
}
