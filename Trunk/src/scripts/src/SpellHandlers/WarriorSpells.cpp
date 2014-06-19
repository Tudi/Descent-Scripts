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
/*
bool Charge(uint32 i, Spell* pSpell)
{
    uint32 rage_to_gen;
    switch(pSpell->GetProto()->Id)
    {
    case 100:   // Charge Rank 1
        rage_to_gen = 90;
		if(pSpell->p_caster)
		{
			for(set<uint32>::iterator itr = pSpell->p_caster->mSpells.begin(); itr != pSpell->p_caster->mSpells.end(); ++itr)
			{
				if(*itr == 12697)
					rage_to_gen += 60;
				if(*itr == 12285)
					rage_to_gen += 30;
			}
		}		
        break;

    case 6178:  // Charge Rank 2
        rage_to_gen = 120;
		if(pSpell->p_caster)
		{
			for(set<uint32>::iterator itr = pSpell->p_caster->mSpells.begin(); itr != pSpell->p_caster->mSpells.end(); ++itr)
			{
				if(*itr == 12697)
					rage_to_gen += 60;
				if(*itr == 12285)
					rage_to_gen += 30;
			}
		}
        break;

    default:    // Charge Rank 3 +
        rage_to_gen = 150;
		if(pSpell->p_caster)
		{
			for(set<uint32>::iterator itr = pSpell->p_caster->mSpells.begin(); itr != pSpell->p_caster->mSpells.end(); ++itr)
			{
				if(*itr == 12697)
					rage_to_gen += 60;
				if(*itr == 12285)
					rage_to_gen += 30;
			}
		}

        break;
    }

    // Add the rage to the caster
    if(!pSpell->u_caster)
        return true;

    uint32 val = pSpell->u_caster->GetUInt32Value(UNIT_FIELD_POWER2);
    uint32 max = pSpell->u_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER2);
    val += rage_to_gen;
    if(val > max)
        val = max;
    
    pSpell->u_caster->SetUInt32Value(UNIT_FIELD_POWER2, val);
  //  script_debuglog("WarriorSpells.cpp :: Charge generate %u rage on "I64FMT, rage_to_gen, pSpell->u_caster->GetGUID());
    return true;
}/**/

#if 0
bool Execute(uint32 i, Spell* pSpell)
{
    //uint32 uSpellId = pSpell->GetProto()->Id;
    int32 max_rage_convert = pSpell->damage * 10;
    /*
	Attempt to finish off a wounded foe, causing (10 + AP * 0.437 * 100 / 100) physical damage and 
	consumes up to 20 additional rage to deal up to ($ap * 0.874 * 100 / 100 - 1) additional damage.  
	Only usable on enemies that have less than 20% health.

	Attempt to finish off a wounded foe, causing ${10+$AP*0.25} physical damage and 
	consumes up to $m2 additional rage to deal up to ${$ap*0.5-1} additional damage.  
	Only usable on enemies that have less than 20% health.
	*/

    Unit * target = pSpell->GetUnitTarget();
    if(!target || !pSpell->u_caster) 
		return true;

	//1 proc of this
	pSpell->u_caster->RemoveAuraByNameHash( SPELL_HASH_SUDDEN_DEATH );

    // some auras might set state !
	if( !target->HasAuraStateFlag(AURASTATE_HEALTH_BELOW_20_PCT)
		&& ( pSpell->p_caster == NULL || pSpell->p_caster->IgnoreSpellSpecialCasterStateRequirement==0 )
		)
    {
        // send failed
        pSpell->SendCastResult(SPELL_FAILED_BAD_TARGETS);
        return true;
    }

    float add_damage = 10 + pSpell->u_caster->GetAP() * 0.437f ;
	int32 rage_consume;
	if( pSpell->u_caster->GetPower( POWER_TYPE_RAGE ) > max_rage_convert )
		rage_consume = max_rage_convert;
	else
		rage_consume = pSpell->u_caster->GetPower( POWER_TYPE_RAGE );
	add_damage += rage_consume * 0.874f;   
//printf("!!!! %u, %u, %f\n",base_dmg,pSpell->u_caster->GetAP() * 20 / 100,rage_consume * pSpell->GetProto()->dmg_multiplier[0]);
    // send spell damage log
	//pSpell->u_caster->SpellNonMeleeDamageLog(target, 20647, base_dmg + add_damage, false);
	SpellEntry *sp_for_the_logs = dbcSpell.LookupEntry(20647);
	pSpell->u_caster->Strike( target, MELEE, sp_for_the_logs, float2int32( add_damage ), 0, 0, true, true );
	// zero rage
	pSpell->u_caster->ModPower( POWER_TYPE_RAGE, -rage_consume );
    return true;
}
#endif

bool Bloodrage(uint32 i, Spell* pSpell)
{
  // Put the player in Combat (gotta find when to put him ooc)


  return true;
}

/* Module info */
void SetupWarriorSpells(ScriptMgr * mgr)
{
    /**** Charge ****/
//    mgr->register_dummy_spell(100, &Charge);      // Rank 1
//    mgr->register_dummy_spell(6178, &Charge);     // Rank 2
//    mgr->register_dummy_spell(11578, &Charge);    // Rank 3

    /**** Execute ****/
    /* log isn't working */
    /*
      WORLD: got cast spell packet, spellId - 25236, data length = 9
      Spell::cast 25236, Unit: 2
      WORLD: Spell effect id = 3, damage = 925
      WORLD: Recvd CMSG_ATTACKSWING Message
      WORLD: Sent SMSG_ATTACKSTART
      Player::Update:  No valid current selection to attack, stopping attack
    */
//    mgr->register_dummy_spell(5308, &Execute);     // Rank 1

    /**** Bloodrage ****/
    /* server debug when blood rages is cast */
    /*

    WORLD: got cast spell packet, spellId - 2687, data length = 6
    Spell::cast 2687, Unit: 2
    WORLD: Spell effect id = 30, damage = 100
    WORLD: Spell effect id = 64, damage = 1
    Spell::cast 29131, Unit: 2
    WORLD: Spell effect id = 6, damage = 10
    WORLD: Spell effect id = 6, damage = 1
    WORLD: target = 2 , Spell Aura id = 24, SpellId  = 29131, i = 0, apply = true
    WORLD: target = 2 , Spell Aura id = 94, SpellId  = 29131, i = 1, apply = true
    WORLD: target = 2 , Spell Aura id = 24, SpellId  = 29131, i = 0, apply = false
    WORLD: target = 2 , Spell Aura id = 94, SpellId  = 29131, i = 1, apply = false
    
    */
    mgr->register_dummy_spell(2687, &Bloodrage);    // Bloodrage
    mgr->register_dummy_spell(29131, &Bloodrage);   // Bloodrage this is also called
}
