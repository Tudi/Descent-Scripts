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

//Summon Screecher Spirit - 12699
class SummonScreecherSpirit : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(SummonScreecherSpirit);
	SummonScreecherSpirit(Spell* pSpell) : SpellScript(pSpell) {}

	SpellCastError CanCast(bool tolerate)
	{
		Unit* target=_spell->GetUnitTarget();

		if(target == NULL || target->GetEntry() != 5307 || target->isAlive())
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class TagMurloc : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(TagMurloc);
	TagMurloc(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* target=_spell->GetUnitTarget();

		if(target == NULL || target->GetEntry() != 17326)
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class AdministerAntidote : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(AdministerAntidote);
	AdministerAntidote(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* target=_spell->GetUnitTarget();

		if(target == NULL || target->GetEntry() != 16880)
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class YennikusRelease : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(YennikusRelease);
	YennikusRelease(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* target=_spell->GetUnitTarget();

		if(target == NULL || target->GetEntry() != 2530)
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class RinasDiminutionPowder : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(RinasDiminutionPowder);
	RinasDiminutionPowder(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* target=_spell->GetUnitTarget();

		if(target == NULL || target->GetEntry() != 20058)
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class LiquidFire : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(LiquidFire);
	LiquidFire(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Creature *corpse = _spell->m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_spell->m_caster->GetPositionX(), _spell->m_caster->GetPositionY(), _spell->m_caster->GetPositionZ(), 18240);
		if(corpse != NULL)
			if (_spell->m_caster->CalcDistance(corpse) > 5)
				return SPELL_FAILED_NOT_HERE;

		return SPELL_CANCAST_OK;
	}
};

class Fumping : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Fumping);
	Fumping(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Creature *cleft = _spell->m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_spell->m_caster->GetPositionX(), _spell->m_caster->GetPositionY(), _spell->m_caster->GetPositionZ(), 22105);
		if(cleft == NULL || cleft->isAlive())
			return SPELL_FAILED_NOT_HERE;

		return SPELL_CANCAST_OK;
	}
};

class PlaceDraeneiBanner : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(PlaceDraeneiBanner);
	PlaceDraeneiBanner(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Creature *corpse = _spell->m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_spell->m_caster->GetPositionX(), _spell->m_caster->GetPositionY(), _spell->m_caster->GetPositionZ(), 17701);
		if(corpse != NULL)
			if (_spell->m_caster->CalcDistance(corpse) > 5  || corpse->isAlive())
				return SPELL_FAILED_NOT_HERE;

		return SPELL_CANCAST_OK;
	}
};

class DemonBroiledSurprise : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(DemonBroiledSurprise);
	DemonBroiledSurprise(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Creature *abysal = _spell->m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_spell->m_caster->GetPositionX(), _spell->p_caster->GetPositionY(), _spell->m_caster->GetPositionZ(), 19973);
		if(_spell->p_caster == NULL || abysal == NULL || abysal->isAlive())
			return SPELL_FAILED_NOT_HERE;

		if(!(_spell->p_caster->GetItemInterface()->GetItemCount(31672, 0) > 1 && _spell->p_caster->GetItemInterface()->GetItemCount(31673, 0) > 0 && _spell->p_caster->CalcDistance(abysal) < 10))
			return SPELL_FAILED_NOT_HERE;

		return SPELL_CANCAST_OK;
	}
};

class ReindeerTransformation : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(ReindeerTransformation);
	ReindeerTransformation(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (_spell->p_caster == NULL || !_spell->p_caster->IsMounted())
			return SPELL_FAILED_ONLY_MOUNTED;

		return SPELL_CANCAST_OK;
	}
};

class SmiteDemon : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(SmiteDemon);
	SmiteDemon(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL || u_target->IsPlayer() || u_target->getClass() != TARGET_TYPE_DEMON)
			return SPELL_FAILED_SPELL_UNAVAILABLE;

		return SPELL_CANCAST_OK;
	}
};

class AbsorbEyeofGrillok : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(AbsorbEyeofGrillok);
	AbsorbEyeofGrillok(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		//this is currently disabled
		return SPELL_FAILED_SPELL_UNAVAILABLE;
	}
};

class TheSeersPresence : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(TheSeersPresence);
	TheSeersPresence(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL || !u_target->IsCreature() || u_target->GetEntry() != 20132)
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class BlackwhelpNet : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(BlackwhelpNet);
	BlackwhelpNet(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL || !u_target->IsCreature() || u_target->GetEntry() != 21387)
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class ConvertingSentry : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(ConvertingSentry);
	ConvertingSentry(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL || !u_target->IsCreature() || u_target->GetEntry() != 24972)
			return SPELL_FAILED_BAD_TARGETS;

		if (!u_target->isDead())
			return SPELL_FAILED_TARGET_NOT_DEAD;

		return SPELL_CANCAST_OK;
	}
};

class CarindasRetribution : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(CarindasRetribution);
	CarindasRetribution(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL || !u_target->IsCreature() || u_target->GetEntry() != 24972)
			return SPELL_FAILED_BAD_TARGETS;

		if (!u_target->isDead())
			return SPELL_FAILED_TARGET_NOT_DEAD;

		return SPELL_CANCAST_OK;
	}
};

void SetupOldSpellCPPSuff(ScriptMgr * mgr)
{
	mgr->register_spell_script(12699, &SummonScreecherSpirit::Create);
	mgr->register_spell_script(30877, &TagMurloc::Create);
	mgr->register_spell_script(34665, &AdministerAntidote::Create);
	mgr->register_spell_script(3607, &YennikusRelease::Create);
	mgr->register_spell_script(36310, &RinasDiminutionPowder::Create);
	mgr->register_spell_script(32146, &LiquidFire::Create);
	mgr->register_spell_script(39246, &Fumping::Create);
	mgr->register_spell_script(30988, &PlaceDraeneiBanner::Create);
	mgr->register_spell_script(43723, &DemonBroiledSurprise::Create);
	mgr->register_spell_script(25860, &ReindeerTransformation::Create);
	mgr->register_spell_script(13907, &SmiteDemon::Create); //demonslayer enchant
	mgr->register_spell_script(38554, &AbsorbEyeofGrillok::Create);
	mgr->register_spell_script(36314, &TheSeersPresence::Create);
	mgr->register_spell_script(38177, &BlackwhelpNet::Create);
	mgr->register_spell_script(44997, &ConvertingSentry::Create);
	mgr->register_spell_script(30077, &CarindasRetribution::Create);
	
}