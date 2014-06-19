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

bool Refocus(uint32 i, Spell * pSpell)
{
    Player * playerTarget = pSpell->GetPlayerTarget();
    if(playerTarget == 0) return true;

    SpellSet::const_iterator itr = playerTarget->mSpells.begin();
    for(; itr != playerTarget->mSpells.end(); ++itr)
    {
        if((*itr) == 24531)       // skip calling spell.. otherwise spammies! :D
            continue;

        if((*itr) == 19434 || (*itr) == 20900 || (*itr) == 20901 || (*itr) == 20902 || (*itr) == 20903 || (*itr) == 20904 || (*itr) == 27632
            || (*itr) == 2643 || (*itr) == 14288|| (*itr) == 14289|| (*itr) == 14290 || (*itr) == 25294 || (*itr) == 14443 || (*itr) == 18651 || (*itr) == 20735 || (*itr) == 21390
            || (*itr) == 1510 || (*itr) == 14294 || (*itr) == 14295 || (*itr) == 1540 || (*itr) == 22908
            || (*itr) == 3044 || (*itr) == 14281 || (*itr) == 14282 || (*itr) == 14283 || (*itr) == 14284 || (*itr) == 14285 || (*itr) == 14286 || (*itr) == 14287)
            playerTarget->ClearCooldownForSpell((*itr));
    }
    return true;
}

bool Readiness(uint32 i, Spell * pSpell)
{
    if(!pSpell->p_caster)
		return true;
    pSpell->p_caster->ClearCooldownsOnLine(50 , pSpell->GetProto()->Id);//Beast Mastery
    pSpell->p_caster->ClearCooldownsOnLine(163, pSpell->GetProto()->Id);//Marksmanship
    pSpell->p_caster->ClearCooldownsOnLine(51 , pSpell->GetProto()->Id);//Survival
    return true;
}

class SnakeTrapViper : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(SnakeTrapViper);

   //constructor
    SnakeTrapViper(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//only handle spell created mobs
		if( pCreature->GetUInt32Value( UNIT_FIELD_CREATEDBY ) == 0 )
			return;

		pCreature->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 107);
		pCreature->SetUInt32Value( UNIT_FIELD_HEALTH, 107);
		pCreature->SetFloatValue( UNIT_FIELD_MAXDAMAGE, 53);
		pCreature->SetFloatValue( UNIT_FIELD_MINDAMAGE, 38);
		pCreature->BaseDamage[0] = 38;
		pCreature->BaseDamage[1] = 58;
		for(uint32 i=0;i<SCHOOL_COUNT;i++)
			pCreature->ModDamageDone[i] = 0;
		pCreature->SetUInt32Value( UNIT_FIELD_RESISTANCES, 9729);
		pCreature->BaseAttackType = SCHOOL_NATURE;
		pCreature->SetInt32Value(UNIT_FIELD_ATTACK_POWER, 0 );
		pCreature->CalcDamage();
		//cripling poison
		SpellEntry *sp = dbcSpell.LookupEntryForced( 3409 );	
		ProcTriggerSpell pts( sp, NULL );
		pts.caster = _unit->GetGUID();
		pts.spellId = sp->Id;	
		pts.procFlags = PROC_ON_MELEE_ATTACK;
		pts.created_with_value = sp->eff[0].EffectBasePoints + 1;
		pts.procCharges = 0;
		pts.procChance = 50;	//4 snakes
		pts.procInterval = 2000;	//same as attack interval ?
		_unit->RegisterScriptProcStruct(pts);
		//Mind-numbing Poison
		sp = dbcSpell.LookupEntryForced( 25810 );	
		ProcTriggerSpell pts2( sp, NULL );
		pts2.caster = _unit->GetGUID();
		pts2.spellId = sp->Id;	
		pts2.procFlags = PROC_ON_MELEE_ATTACK;
		pts2.created_with_value = sp->eff[0].EffectBasePoints + 1;
		pts2.procCharges = 0;
		pts2.procChance = 50;	//4 snakes
		pts2.procInterval = 2000;	//same as attack interval ?
		_unit->RegisterScriptProcStruct(pts2);
		//Deadly Poison
		sp = dbcSpell.LookupEntryForced( 38520 );	
		if( sp )
		{
			ProcTriggerSpell pts3( sp, NULL );
			pts3.caster = _unit->GetGUID();
			pts3.spellId = sp->Id;	
			pts3.procFlags = PROC_ON_MELEE_ATTACK;
			pts3.created_with_value = sp->eff[0].EffectBasePoints + 1;
			pts3.procCharges = 0;
			pts3.procChance = 30;	//4 snakes
			pts3.procInterval = 2000;	//same as attack interval ?
			_unit->RegisterScriptProcStruct(pts3);
		}
   }
};

class SnakeTrapVenomSnake : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(SnakeTrapVenomSnake);

   //constructor
    SnakeTrapVenomSnake(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//only handle spell created mobs
		if( pCreature->GetUInt32Value( UNIT_FIELD_CREATEDBY ) == 0 )
			return;

		pCreature->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 107);
		pCreature->SetUInt32Value( UNIT_FIELD_HEALTH, 107);
		pCreature->SetFloatValue( UNIT_FIELD_MAXDAMAGE, 24);
		pCreature->SetFloatValue( UNIT_FIELD_MINDAMAGE, 16);
		pCreature->BaseDamage[0] = 16;
		pCreature->BaseDamage[1] = 24;
		for(uint32 i=0;i<SCHOOL_COUNT;i++)
			pCreature->ModDamageDone[i] = 0;
		pCreature->SetUInt32Value( UNIT_FIELD_RESISTANCES, 9729);
		pCreature->BaseAttackType = SCHOOL_NATURE;
		pCreature->SetInt32Value(UNIT_FIELD_ATTACK_POWER, 0 );
		pCreature->CalcDamage();
		//cripling poison
		SpellEntry *sp = dbcSpell.LookupEntryForced( 3409 );	
		ProcTriggerSpell pts( sp, NULL );
		pts.caster = _unit->GetGUID();
		pts.spellId = sp->Id;	
		pts.procFlags = PROC_ON_MELEE_ATTACK;
		pts.created_with_value = sp->eff[0].EffectBasePoints + 1;
		pts.procCharges = 0;
		pts.procChance = 50;	//4 snakes
		pts.procInterval = 2000;	//same as attack interval ?
		_unit->RegisterScriptProcStruct(pts);
		//Mind-numbing Poison
		sp = dbcSpell.LookupEntryForced( 25810 );	
		ProcTriggerSpell pts2( sp, NULL );
		pts2.caster = _unit->GetGUID();
		pts2.spellId = sp->Id;	
		pts2.procFlags = PROC_ON_MELEE_ATTACK;
		pts2.created_with_value = sp->eff[0].EffectBasePoints + 1;
		pts2.procCharges = 0;
		pts2.procChance = 50;	//4 snakes
		pts2.procInterval = 2000;	//same as attack interval ?
		_unit->RegisterScriptProcStruct(pts2);
		//Deadly Poison
		sp = dbcSpell.LookupEntryForced( 38520 );	
		if( sp )
		{
			ProcTriggerSpell pts3( sp, NULL );
			pts3.caster = _unit->GetGUID();
			pts3.spellId = sp->Id;	
			pts3.procFlags = PROC_ON_MELEE_ATTACK;
			pts3.created_with_value = sp->eff[0].EffectBasePoints + 1;
			pts3.procCharges = 0;
			pts3.procChance = 30;	//4 snakes
			pts3.procInterval = 2000;	//same as attack interval ?
			_unit->RegisterScriptProcStruct(pts3);
		}
   }
};
void SetupHunterSpells(ScriptMgr * mgr)
{
    mgr->register_dummy_spell(24531, &Refocus);
    mgr->register_dummy_spell(23989, &Readiness);
	mgr->register_creature_script( 19921, &SnakeTrapViper::Create);			//snake trap viper
	mgr->register_creature_script( 19833, &SnakeTrapVenomSnake::Create);	//snake trap snake
}
