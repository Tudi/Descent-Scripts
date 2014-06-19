/****************************************************************************
 *
 * SpellHandler Plugin
 * Copyright (c) 2007 Team Ascent
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

class MindControl : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(MindControl);
	MindControl(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();
		if (!_spell->u_caster == NULL || u_target == NULL || u_target->getLevel() > _spell->m_spellInfo->EffectBasePoints[0]+1 + int32(_spell->u_caster->getLevel() - _spell->m_spellInfo->spellLevel))
			return SPELL_FAILED_HIGHLEVEL;

		if (u_target->IsCreature() && static_cast<Creature*>(u_target)->GetCreatureName() != NULL || static_cast<Creature*>(u_target)->GetCreatureName()->Rank > ELITE_ELITE)
			return SPELL_FAILED_HIGHLEVEL;

		return SPELL_CANCAST_OK;
	}
};

class PowerWordShield : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(PowerWordShield);
	Unit* u_target;
	PowerWordShield(Spell* pSpell) : SpellScript(pSpell) { u_target = NULL; }
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();
		if (u_target == NULL || u_target->HasAura(6788))
			return SPELL_FAILED_DAMAGE_IMMUNE;

		return SPELL_CANCAST_OK;
	}

	void OnCast()
	{
		if (_spell->u_caster != NULL && u_target != NULL)
			_spell->u_caster->CastSpell(u_target, 6788, true);
	}
};

class PrayerOfMending : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(PrayerOfMending);
	PrayerOfMending(Spell* pSpell) : SpellScript(pSpell) {}
};

void SetupPriestSpells(ScriptMgr * mgr)
{
	mgr->register_spell_script(605, &MindControl::Create);
	mgr->register_spell_script(10911, &MindControl::Create);
	mgr->register_spell_script(10912, &MindControl::Create);
	mgr->register_spell_script(17, &PowerWordShield::Create);
	mgr->register_spell_script(592, &PowerWordShield::Create);
	mgr->register_spell_script(600, &PowerWordShield::Create);
	mgr->register_spell_script(3747, &PowerWordShield::Create);
	mgr->register_spell_script(6065, &PowerWordShield::Create);
	mgr->register_spell_script(6066, &PowerWordShield::Create);
	mgr->register_spell_script(10898, &PowerWordShield::Create);
	mgr->register_spell_script(10899, &PowerWordShield::Create);
	mgr->register_spell_script(10900, &PowerWordShield::Create);
	mgr->register_spell_script(10901, &PowerWordShield::Create);
	mgr->register_spell_script(25217, &PowerWordShield::Create);
	mgr->register_spell_script(25218, &PowerWordShield::Create);
	mgr->register_spell_script(41635, &PrayerOfMending::Create);
}
