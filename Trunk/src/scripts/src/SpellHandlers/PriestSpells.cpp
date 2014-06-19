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

class ShadowFiend : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(ShadowFiend);

   //constructor
   ShadowFiend(Creature* pCreature) : CreatureAIScript(pCreature){}
	void OnLoad()
    {
		//only handle spell created mobs
		if( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) == 0 )
			return;

		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner )
		{
			uint32 parent_bonus;
			if( owner->IsPlayer() )
				parent_bonus = owner->GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + SCHOOL_SHADOW ) * 37 / 100;
			else
				parent_bonus = 2300 + owner->GetDamageDoneMod(SCHOOL_SHADOW)*65/100;
			for(uint32 i=0;i<SCHOOL_COUNT;i++)
				_unit->ModDamageDone[ i ] = owner->GetDamageDoneMod( i ) * 40 / 100;
			_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, 1500 );
			_unit->SetFloatValue(UNIT_FIELD_MINDAMAGE, (float)parent_bonus);
			_unit->SetFloatValue(UNIT_FIELD_MAXDAMAGE, (float)parent_bonus);
			_unit->BaseDamage[0] = (float)parent_bonus;
			_unit->BaseDamage[1] = (float)parent_bonus;
			_unit->BaseAttackType = SCHOOL_SHADOW;
			if( owner->HasAuraWithNameHash( SPELL_HASH_ITEM___PRIEST_T13_SHADOW_4P_BONUS__SHADOWFIEND_AND_SHADOWY_APPARITION_, 0, AURA_SEARCH_PASSIVE ) )
			{
				SpellEntry *sp = dbcSpell.LookupEntryForced( 77487 );	//Shadow Orb
				ProcTriggerSpell pts( sp, NULL );
				pts.caster = _unit->GetGUID();
				pts.spellId = sp->Id;
				pts.procFlags = PROC_ON_MELEE_ATTACK;
				pts.created_with_value = sp->eff[0].EffectBasePoints + 1;
				pts.procCharges = 0;
				pts.procChance = 100;
				//register it 3 times ( yes, 3 times )
				_unit->RegisterScriptProcStruct( pts );
				_unit->RegisterScriptProcStruct( pts );
				_unit->RegisterScriptProcStruct( pts );
			}
		}	
		//register spell that will regen mana for caster on melee hit
		SpellEntry *sp = dbcSpell.LookupEntryForced( 34650 );	//Mana Leech
		ProcTriggerSpell pts( sp, NULL );
		pts.caster = _unit->GetGUID();
		pts.spellId = sp->Id;
		pts.procFlags = PROC_ON_MELEE_ATTACK;
		pts.created_with_value = sp->eff[0].EffectBasePoints + 1;
		pts.procCharges = 0;
		pts.procChance = 100;
		_unit->RegisterScriptProcStruct( pts );
   }
};

class LightWell : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(LightWell);

   //constructor
    LightWell(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		//only handle spell created mobs
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			uint32 new_faction = owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE);
//			uint32 new_faction = owner->GetTeam()?1:2;
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, new_faction);
			_unit->_setFaction( );
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
		}
		_unit->CastSpell(_unit,59907,true);
	}
};

class PowerWordBarrier : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(PowerWordBarrier);

   bool has_glyph;
    PowerWordBarrier(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner == NULL )
			return;
		if( owner->HasGlyphWithID( GLYPH_PRIEST_POWERWORDBARRIER ) )
			has_glyph = true;
		else
			has_glyph = false;
		RegisterAIUpdateEvent( 400 );
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
		}
		//tif we enable this mob to be unselectable then he will not be able to cast the spell on others
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PLAYER_CONTROLLED);
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
		_unit->_setFaction( );
		_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, _unit->GetGUID() );	//we need a cast target even if it not that valid :P
		_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 81781 );	
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686 );	
		_unit->SetUInt32Value(UNIT_CREATED_BY_SPELL, 62618 );	
    }
    void AIUpdate()
    {
		_unit->CastSpell( _unit, 81782, true );
		if( has_glyph == true )
			_unit->CastSpell( _unit, 90785, true );
	}
};

class ShadowyAparition : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(ShadowyAparition);

   bool casted_visual_spells;
   ShadowyAparition(Creature* pCreature) : CreatureAIScript(pCreature){}
	void OnLoad()
    {
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt64Value( UNIT_FIELD_CREATEDBY ) );
		if( owner )
		{
/*
started creating object with type TYPEID_UNIT with Hguid 4113610602 and Lguid 4206378 -> 2A 2F 40 00 6A B7 30 F5  = F7 2A 2F 40 6A B7 30 F5 
	Debug Datareader : Masklen(24) for mask 06 at position 281 
	 OBJECT_FIELD_GUID = 4206378 
	 OBJECT_FIELD_GUID_1 = 4113610602 
	 OBJECT_FIELD_TYPE = 65545 
	 OBJECT_FIELD_ENTRY = 46954 
	 OBJECT_FIELD_SCALE_X = 1.000000 
	 OBJECT_FIELD_DATA = 12618975 
	 OBJECT_FIELD_DATA_1 = 535887872 
	 UNIT_FIELD_CREATEDBY = 54126938 
	 UNIT_FIELD_CREATEDBY_1 = 16777216 
	 UNIT_FIELD_BYTES_0 = 16908544 
	 UNIT_FIELD_HEALTH = 5 
	 UNIT_FIELD_POWER3 = 100 
	 UNIT_FIELD_POWER4 = 100 
	 UNIT_FIELD_POWER5 = 1000000 
	 UNIT_FIELD_MAXHEALTH = 5 
	 UNIT_FIELD_MAXPOWER3 = 100 
	 UNIT_FIELD_MAXPOWER4 = 100 
	 UNIT_FIELD_MAXPOWER5 = 1000000 
	 UNIT_FIELD_LEVEL = 85 
	 UNIT_FIELD_FACTIONTEMPLATE = 116 
	 UNIT_FIELD_FLAGS = 32776 
	 UNIT_FIELD_FLAGS_2 = 2048 
	 UNIT_FIELD_BASEATTACKTIME = 2000 
	 UNIT_FIELD_BASEATTACKTIME_1 = 2000 
	 UNIT_FIELD_BOUNDINGRADIUS = 0.500000 
	 UNIT_FIELD_COMBATREACH = 1.000000 
	 UNIT_FIELD_DISPLAYID = 11686 
	 UNIT_FIELD_NATIVEDISPLAYID = 11686 
	 UNIT_MOD_CAST_SPEED = 1.000000 
	 UNIT_CREATED_BY_SPELL = 87426 
	 UNIT_FIELD_BYTES_2 = 1 
	 UNIT_FIELD_HOVERHEIGHT = 1.000000 

	Debug Datareader : Masklen(24) for mask 06 at position 16 
	 UNIT_FIELD_TARGET = 54056937 
	 UNIT_FIELD_TARGET_1 = 16777216 
	 UNIT_FIELD_BYTES_0 = 16777472 
	 UNIT_FIELD_FLAGS = 34824 
	 UNIT_FIELD_FLAGS_2 = 2064 
	 UNIT_FIELD_BOUNDINGRADIUS = 0.306000 
	 UNIT_FIELD_COMBATREACH = 1.500000 
	 UNIT_FIELD_DISPLAYID = 1478 
*/
			_unit->SetUInt32Value( UNIT_FIELD_BYTES_0, owner->GetUInt32Value( UNIT_FIELD_BYTES_0 ) ); 
			_unit->SetUInt32Value( UNIT_FIELD_FLAGS, 8 );
			_unit->SetUInt32Value( UNIT_FIELD_FLAGS_2, 2064 );
			_unit->SetUInt32Value( UNIT_CREATED_BY_SPELL, 87426 );
			_unit->SetUInt32Value( UNIT_FIELD_BYTES_2, owner->GetUInt32Value( UNIT_FIELD_BYTES_2 ) );//weapon sheath and rest
			_unit->SetByte( UNIT_FIELD_BYTES_2, 0, 1 );//animation frozen, just to be blizzlike
//			_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, owner->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
//			_unit->SetUInt32Value( UNIT_FIELD_MAXPOWER1, owner->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );
			_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, owner->GetUInt32Value( UNIT_FIELD_DISPLAYID ) );
			_unit->SetUInt64Value( UNIT_FIELD_CREATEDBY, owner->GetGUID() );
			_unit->SetUInt64Value( OBJECT_FIELD_DATA, owner->GetUInt32Value( OBJECT_FIELD_DATA ) );
			_unit->SetFloatValue( UNIT_FIELD_BOUNDINGRADIUS, 0.383000f );
			_unit->SetFloatValue( UNIT_FIELD_COMBATREACH, 1.500000f );
			int mainhandfield = PLAYER_VISIBLE_ITEM_1_ENTRYID + (EQUIPMENT_SLOT_MAINHAND * (PLAYER_VISIBLE_ITEM_2_ENTRYID - PLAYER_VISIBLE_ITEM_1_ENTRYID));
			_unit->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_ID, owner->GetUInt32Value( mainhandfield ) );
			int moffhandfield = PLAYER_VISIBLE_ITEM_1_ENTRYID + (EQUIPMENT_SLOT_OFFHAND * (PLAYER_VISIBLE_ITEM_2_ENTRYID - PLAYER_VISIBLE_ITEM_1_ENTRYID));
			_unit->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_ID_1, owner->GetUInt32Value( moffhandfield ) );
			//target the guy that just procced this spell (SWP)
			if( owner->m_currentSpellAny )
				_unit->SetUInt64Value( UNIT_FIELD_TARGET, owner->m_currentSpellAny->m_targets.m_unitTarget );
			else if( owner->GetUInt64Value( UNIT_FIELD_TARGET ) )
				_unit->SetUInt64Value( UNIT_FIELD_TARGET, owner->GetUInt64Value( UNIT_FIELD_TARGET ) );

			_unit->ModDamageDone[ SCHOOL_SHADOW ] = owner->GetDamageDoneMod( SCHOOL_SHADOW );

			if( owner->HasAuraWithNameHash( SPELL_HASH_ITEM___PRIEST_T13_SHADOW_4P_BONUS__SHADOWFIEND_AND_SHADOWY_APPARITION_, 0, AURA_SEARCH_PASSIVE ) )
			{
				SpellEntry *sp = dbcSpell.LookupEntryForced( 77487 );	//Shadow Orb
				ProcTriggerSpell pts( sp, NULL );
				pts.caster = _unit->GetGUID();
				pts.spellId = sp->Id;
				pts.procFlags = PROC_ON_MELEE_ATTACK;
				pts.created_with_value = sp->eff[0].EffectBasePoints + 1;
				pts.procCharges = 0;
				pts.procChance = 100;
				//register it 3 times ( yes, 3 times )
				_unit->RegisterScriptProcStruct( pts );
				_unit->RegisterScriptProcStruct( pts );
				_unit->RegisterScriptProcStruct( pts );
			}

		}

		RegisterAIUpdateEvent( 2000 );
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
			_unit->m_runSpeed = _unit->m_walkSpeed;	
			_unit->m_flySpeed = _unit->m_walkSpeed;	
			_unit->GetAIInterface()->Init( _unit, AITYPE_LONER, MOVEMENTTYPE_NONE, NULL );	//do not follow owner
		}
		casted_visual_spells = false;
    }
    void AIUpdate()
    {
		if( casted_visual_spells == false && _unit->IsInWorld() )
		{
			casted_visual_spells = true;
			Unit *TopO = _unit->GetTopOwner();
//			TopO->CastSpell( _unit, 45204, false ); //Clone Me!
			TopO->CastSpell( _unit, 87213, true );	//Summon Shadowy Apparition Spawn Effect
			_unit->CastSpell( _unit, 87427, true );	//Shadowy Apparition Visual
			_unit->GetAIInterface()->SetUnitToFollow( NULL );
		}
		//try to go to our target location
		Unit *ourtarget = _unit->GetMapMgr()->GetUnit( _unit->GetUInt64Value( UNIT_FIELD_TARGET ) );
		if( ourtarget )
		{
			float dist = _unit->GetDistance2dSq( ourtarget );
			if( dist < ( DISTANCE_TO_SMALL_TO_WALK * DISTANCE_TO_SMALL_TO_WALK + 2.0f * 2.0f ) )
			{
				//explode
				_unit->CastSpell( _unit, 87529, true );	//Shadowy Apparition Death Visual
				_unit->GetTopOwner()->CastSpell( ourtarget, 87532, true );	//Shadowy Apparition - deal damage
				_unit->SafeDelete();
			}
			else //move closer
			{
				_unit->GetAIInterface()->MoveTo( ourtarget->GetPositionX(), ourtarget->GetPositionY(), ourtarget->GetPositionZ(), 0.0f );
			}
		}
	}
};


class CauterizingFlame : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(CauterizingFlame);

   //constructor
    CauterizingFlame(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		//only handle spell created mobs
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
		}
		_unit->CastSpell(_unit,102129,true);	//Cauterizing Flame - visual
		_unit->CastSpell(_unit,99153,true);	//Cauterizing Flame - periodic heal
	}
};


void SetupPriestSpells(ScriptMgr * mgr)
{
	mgr->register_creature_script( 46954, &ShadowyAparition::Create);	//Shadowy Apparition
	mgr->register_creature_script( 33248, &PowerWordBarrier::Create);	//Power Word: Barrier
//	mgr->register_creature_script( 19668, &ShadowFiend::Create);	//shadowfiend
	mgr->register_pet_script( 19668, &ShadowFiend::Create);	//shadowfiend
//	mgr->register_creature_script( 31893, &LightWell::Create);		//lightwell
//	mgr->register_creature_script( 31894, &LightWell::Create);		//lightwell
//	mgr->register_creature_script( 31895, &LightWell::Create);		//lightwell
//	mgr->register_creature_script( 31896, &LightWell::Create);		//lightwell
	mgr->register_creature_script( 31897, &LightWell::Create);		//lightwell
//	mgr->register_creature_script( 31883, &LightWell::Create);		//lightwell
	mgr->register_creature_script( 53475, &CauterizingFlame::Create);		//Cauterizing Flame
}
