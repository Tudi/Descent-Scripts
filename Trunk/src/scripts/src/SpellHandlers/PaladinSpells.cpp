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

/*
	How to add a new paladin spell to the dummy spell handler:

		1) Add a new function to handle the spell at the end of this file but before the
		   SetupPaladinSpells() function. SetupPaladinSpells() must always be the last function.

		2) Register the dummy spell by adding a new line to the end of the list in the
		   SetupPaladinSpells() function.

	Please look at how the other spells are handled and try to use the
	same variable names and formatting style in your new spell handler.
*/

// *****************************************************************************
// -----------------------------------------------------------------------------

bool HolyShock(uint32 i, Spell *pSpell)
{
	Unit *target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target) return true;

	uint32 newspell = 0;

	if(isAttackable(pSpell->p_caster,target)) // if its an enemy
		newspell = 25912;
	else // if its friendly
		newspell = 25914;

	if( newspell )
	{
		SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
		if(!spInfo) 
			return true;
		pSpell->p_caster->CastSpell(target, spInfo, true);
	}
	return true;
}

/*
class Consecration : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Consecration);

    Consecration(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner == NULL )
			return;
		RegisterAIUpdateEvent( 500 );
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
		}
		//tif we enable this mob to be unselectable then he will not be able to cast the spell on others
//		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_DEAD);
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
		_unit->_setFaction( );
    }
    void AIUpdate()
    {
		//Consecrates the land beneath the Paladin, doing (8 * (0 + 0.04 * holy power.04 * AP)) Holy damage over 10 sec to enemies who enter the area.
		//0 damage every 5 seconds.
		const int tick_count = (10 / 5) * 10; //20 ticks over 10 seconds
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner == NULL )
			return;
		uint32 value_total = 8 * float2int32( ( (owner->GetDamageDoneMod( SCHOOL_HOLY ) + 0.04f) * 0.04f * owner->GetAP() ) );
		uint32 value = value_total / tick_count;

		SpellCastTargets targets( 0 );
		targets.m_targetMask |= TARGET_FLAG_DEST_LOCATION | TARGET_FLAG_SOURCE_LOCATION;
		targets.m_srcX = targets.m_destX = _unit->GetPositionX();
		targets.m_srcY = targets.m_destY = _unit->GetPositionY();
		targets.m_srcZ = targets.m_destZ = _unit->GetPositionZ();

		SpellEntry *spellInfo = dbcSpell.LookupEntry( 81297 );//not dead sure about the spell
		Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
		spell->Init( _unit, spellInfo ,true, NULL);
		spell->forced_basepoints[0] = value;
		spell->prepare(&targets);

//		_unit->CastSpell( _unit, 81297, true );	//not dead sure about the spell
	}
};
*/

class GuardianOfAncientKingsHoly : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(GuardianOfAncientKingsHoly);

   //constructor
    GuardianOfAncientKingsHoly(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//only handle spell created mobs
		if( pCreature->GetUInt32Value( UNIT_FIELD_CREATEDBY ) == 0 )
			return;

		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->Init( _unit, AITYPE_PET, MOVEMENTTYPE_NONE, NULL );	//do not follow owner
			_unit->GetAIInterface()->disable_melee = true;
//			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
		}
		_unit->bInvincible = true;
	    _unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner != NULL )
		{
			//register spell that will regen health for caster on melee hit
			SpellEntry *sp = dbcSpell.LookupEntryForced( 86678 );	//Light of the Ancient Kings
			ProcTriggerSpell pts( sp, NULL );
			pts.caster = _unit->GetGUID();
			pts.spellId = sp->Id;	
			pts.procFlags = PROC_ON_HEAL_SPELL | PROC_ON_NO_DOT;
			pts.created_with_value = sp->eff[0].EffectBasePoints + 1;
			pts.procCharges = 5;	//max 5 spells
			pts.procChance = 100;
			pts.spellId = 86678;	//self proc :(
			owner->RegisterScriptProcStruct(pts);
		}
   }
	void OnRemoveFromWorld()
	{
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner != NULL )
		{
			CommitPointerListNode<ProcTriggerSpell> *itr;
			for(itr = owner->m_procSpells.begin();itr != owner->m_procSpells.end();itr = itr->Next() )
				if( itr->data->origId == 50454 && itr->data->caster == _unit->GetGUID() && !itr->data->deleted )
				{
					itr->data->deleted = true;
					break;
				}
		}
	}
};

class GuardianOfAncientKingsRetribution : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(GuardianOfAncientKingsRetribution);

   //constructor
    GuardianOfAncientKingsRetribution(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//only handle spell created mobs
		if( pCreature->GetUInt32Value( UNIT_FIELD_CREATEDBY ) == 0 )
			return;

		_unit->bInvincible = true;
	    _unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );

		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
//			float base_dmg = owner->GetFloatValue( UNIT_FIELD_MINDAMAGE ) * 1.2f;	
			float base_dmg = owner->GetFloatValue( UNIT_FIELD_MINDAMAGE ) * 0.6f;	
			pCreature->SetFloatValue( UNIT_FIELD_MINDAMAGE, base_dmg );
			pCreature->SetFloatValue( UNIT_FIELD_MAXDAMAGE, base_dmg * 1.5f );
			pCreature->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, owner->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME ) * 3 / 4 );
			pCreature->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME_1, owner->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME_1 ) * 3 / 4 );
			pCreature->SetUInt32Value( UNIT_FIELD_RANGEDATTACKTIME, owner->GetUInt32Value( UNIT_FIELD_RANGEDATTACKTIME ) * 3 / 4 );
		}
		SpellEntry *sp = dbcSpell.LookupEntryForced( 86700 );	//Ancient Power
		ProcTriggerSpell pts( sp, NULL );
		pts.caster = _unit->GetGUID();
		pts.spellId = sp->Id;
		pts.procFlags = PROC_ON_MELEE_ATTACK;
		pts.created_with_value = sp->eff[0].EffectBasePoints + 1;
		pts.procCharges = 0;
		pts.procChance = 100;
		pts.procInterval = 2000;	//no idea about default value. making sure it is 2 sec
		_unit->RegisterScriptProcStruct( pts );
		owner->RegisterScriptProcStruct( pts );
		_unit->m_stealthDetectBonus = 50000; //can see stealthed players acording to wowhead. Tough i think it's a bug
		//should not be CCable ?
		_unit->MechanicsDispels[MECHANIC_STUNNED] = 9001;
		_unit->MechanicsDispels[MECHANIC_CHARMED] = 9001;
		_unit->MechanicsDispels[MECHANIC_DISORIENTED] = 9001;
		_unit->MechanicsDispels[MECHANIC_DISTRACED] = 9001;
		_unit->MechanicsDispels[MECHANIC_FLEEING] = 9001;
		_unit->MechanicsDispels[MECHANIC_ROOTED] = 9001;
		_unit->MechanicsDispels[MECHANIC_SLOWED] = 9001;
		_unit->MechanicsDispels[MECHANIC_ASLEEP] = 9001;
		_unit->MechanicsDispels[MECHANIC_STUNNED] = 9001;
		_unit->MechanicsDispels[MECHANIC_POLYMORPHED] = 9001;
		_unit->MechanicsDispels[MECHANIC_HORRIFIED] = 9001;
		_unit->GetAIInterface()->setOutOfCombatRange( 90 * 90 );	//not sure it will work tough :(
    }
	void OnDied(Unit *mKiller)
	{
		OnRemoveFromWorld();
	}
	void OnRemoveFromWorld()
	{
		Unit *owner = _unit->GetTopOwner();
		uint32 count = owner->RemoveAura( 86700, 0, AURA_SEARCH_ALL, MAX_PASSIVE_AURAS );
		if( count )
		{
//			owner->CastSpell( owner, 86704, true ); // Ancient Fury
			SpellEntry* TriggeredSpell = dbcSpell.LookupEntryForced( 86704 );
			if( TriggeredSpell != NULL )
			{
				Spell *sp2=SpellPool.PooledNew( __FILE__, __LINE__ );
				sp2->Init( owner,TriggeredSpell,true,NULL);
				sp2->forced_pct_mod[0] = count*100;	
				SpellCastTargets tgt( owner->GetGUID() );
				sp2->prepare(&tgt);
			}
		}
	}
};

class GuardianOfAncientKingsProtection : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(GuardianOfAncientKingsProtection);

    GuardianOfAncientKingsProtection(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//only handle spell created mobs
		if( pCreature->GetUInt32Value( UNIT_FIELD_CREATEDBY ) == 0 )
			return;

		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->Init( _unit, AITYPE_PET, MOVEMENTTYPE_NONE, NULL );	//do not follow owner
			_unit->GetAIInterface()->disable_melee = true;
//			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
		}
		_unit->bInvincible = true;
	    _unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );
		//low dmg
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner != NULL )
		{
//			for(uint32 i=0;i<SCHOOL_COUNT;i++)
//				owner->DamageTakenPctMod[i] -= 0.5f;
			//Ancient Guardian
			owner->CastSpell( owner, 86657, true );
		}
   }
	void OnRemoveFromWorld()
	{
		Player *owner =objmgr.GetPlayer( _unit->GetUInt64Value( UNIT_FIELD_CREATEDBY ) );
		if( owner != NULL )
		{
//			for(uint32 i=0;i<SCHOOL_COUNT;i++)
//				owner->DamageTakenPctMod[i] += 0.5f;
			//Ancient Guardian
			owner->RemoveAura( 86657 );
		}
	}
	void OnDied(Unit *mKiller)
	{
		Player *owner =objmgr.GetPlayer( _unit->GetUInt64Value( UNIT_FIELD_CREATEDBY ) );
		if( owner != NULL )
		{
			//Ancient Guardian
			owner->RemoveAura( 86657 );
		}
	}
};

void SetupPaladinSpells(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(20473, &HolyShock);           // Holy Shock rank 1
//	mgr->register_creature_script( 43499, &Consecration::Create);	//Consecration
	mgr->register_creature_script( 46490, &GuardianOfAncientKingsHoly::Create);			//Guardian of Ancient Kings
	mgr->register_creature_script( 46499, &GuardianOfAncientKingsProtection::Create);	//Guardian of Ancient Kings
	mgr->register_creature_script( 46506, &GuardianOfAncientKingsRetribution::Create);	//Guardian of Ancient Kings
}
