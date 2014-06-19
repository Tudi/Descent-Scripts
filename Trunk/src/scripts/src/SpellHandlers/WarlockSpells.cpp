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
int32 GetSpellDmgBonusNoNerf(Unit *caster, Unit *pVictim, SpellEntry *spellInfo,int32 base_dmg)
{
	int32 plus_damage = 0;
	uint32 school = spellInfo->School;

//------------------------------by school---------------------------------------------------
	plus_damage += caster->GetDamageDoneMod(school);
//------------------------------by victim type----------------------------------------------
	if(pVictim->IsCreature() && static_cast< Creature*>(pVictim)->GetCreatureInfo() && caster->IsPlayer() )
		plus_damage += static_cast< Player* >(caster)->IncreaseDamageByType[static_cast<Creature*>(pVictim)->GetCreatureInfo()->Type]; //this might create a crash. Leaving it like this to detect new creature types :)
//==============================Bonus Adding To Main Damage=================================
	int32 bonus_damage = plus_damage;
	if((spellInfo->c_is_flags & SPELL_FLAG_IS_REQUIRING_SM_MOD) && caster->SM_Mods)
	{
		SM_FIValue(caster->SM_Mods->SM_FDamageBonus, &bonus_damage, spellInfo->SpellGroupType);
		int dmg_bonus_pct=0;
		SM_FIValue(caster->SM_Mods->SM_PDamageBonus,&dmg_bonus_pct,spellInfo->SpellGroupType);
		bonus_damage += (base_dmg+bonus_damage)*dmg_bonus_pct/100;
	}
//------------------------------by school----------------------------------------------
	float summaryPCTmod = caster->GetDamageDonePctMod(school)-1; //value is initialized with 1
//	summaryPCTmod += caster->DamageDoneModPCT[school];	// BURLEX FIXME
	int32 res = (int32)((base_dmg+bonus_damage)*summaryPCTmod + bonus_damage);
	return res;
}*/

class FieryImp : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(FieryImp);

    FieryImp(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 2500 );
		if( _unit->GetAIInterface() )
			_unit->GetAIInterface()->disable_melee = true;
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
		}
	}
    void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		_unit->CastSpell( tank, 99226, false );		//Flame Blast
	}
};
/*
class HandOfGulDanVisual : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(HandOfGulDanVisual);
    HandOfGulDanVisual(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner == NULL )
			return;
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
		}
	   _unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_DEAD);
	   _unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 9000000 );
	   _unit->SetUInt32Value( UNIT_FIELD_HEALTH, 10000 );
	   _unit->SetUInt32Value( UNIT_CHANNEL_SPELL, 0 );	//no idea about this one :( 86000 ? 86041 ? 85526 ? 71521 ?
	   _unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, 11686 );
    }
}; */

void SetupWarlockSpells(ScriptMgr * mgr)
{
	mgr->register_creature_script( 53491, &FieryImp::Create);	
	// 45884 - Hand of Gul'dan Area Effect
//	mgr->register_creature_script( 46157, &HandOfGulDanVisual::Create);	//Hand of Gul'dan
}

