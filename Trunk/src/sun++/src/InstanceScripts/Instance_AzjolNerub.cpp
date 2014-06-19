/*
* Sun++ Scripts for Ascent Based MMORPG Server
* Copyright (C) 2008 Sun++ Team <http://www.sunplusplus.info/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"

//DUMMYS: Web Wrap - NPC ID: 28619 - Should spawn at Root-ed target
//Krikthir The Gatewatcher BOSS
#define BOSS_KRIKTHIR 28684

#define KRIKTHIR_MINDFLAY 52586
#define KRIKTHIR_CURSEOFFATIGUE 52592
#define KRIKTHIR_ENRAGE 28747

class KrikthirAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(KrikthirAI, MoonScriptCreatureAI);
	KrikthirAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		
		AddSpell(KRIKTHIR_CURSEOFFATIGUE, Target_Self, 100, 0, 10);
		AddSpell(KRIKTHIR_MINDFLAY, Target_RandomPlayer, 100, 0, 7, 0, 30);
		if ( _unit->GetHealthPct() > 10 )
		AddSpell(KRIKTHIR_ENRAGE, Target_Self, 0, 0, 0);
		//EMOTES
		AddEmote(Event_OnCombatStart, "This kingdom belongs to the Scourge! Only the dead may enter.", Text_Yell);
		AddEmote(Event_OnTargetDied, "You were foolish to come.", Text_Yell);
		AddEmote(Event_OnTargetDied, "As Anub'Arak commands!", Text_Yell);
		AddEmote(Event_OnDied, "I should be grateful. But I long ago lost the capacity.", Text_Yell);
	}
};

//boss Hadronox
#define BOSS_HADRONOX 28921

#define HADRONOX_WEBGRAB 53406
#define HADRONOX_PIERCEARMOR 53418 
#define HADRONOX_LEECHPOISON 53030
#define HADRONOX_ACIDCLOUD 53400

class HadronoxAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(HadronoxAI, MoonScriptCreatureAI);
	HadronoxAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(HADRONOX_WEBGRAB, Target_RandomPlayer, 22, 0, 14, 0, 0);
		AddSpell(HADRONOX_PIERCEARMOR, Target_ClosestPlayer, 20, 0, 5, 0, 0);
		AddSpell(HADRONOX_LEECHPOISON, Target_Self, 14, 0, 25, 0, 20);
		AddSpell(HADRONOX_ACIDCLOUD, Target_RandomPlayer, 18, 0, 20, 0, 60);
	}
};	
//to-do: trash and other bosses.

//Watcher Gashra.
#define CN_GASHRA 28730

#define GASHRA_WEBWRAP 52086
#define GASHRA_INFECTEDBITE 52469
#define GASHRA_ENRAGE 52470

class GashraAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(GashraAI, MoonScriptCreatureAI);
	GashraAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(GASHRA_WEBWRAP, Target_RandomPlayer, 22, 0, 35, 0, 0);
		AddSpell(GASHRA_INFECTEDBITE, Target_ClosestPlayer, 35, 0, 12, 0, 0);
	}
};	

//Watcher Narjil
#define CN_NARJIL 28729

#define NARJIL_WEBWRAP 52086
#define NARJIL_INFECTEDBITE 52469
#define NARJIL_BLINDINGWEBS 52524

class NarjilAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(NarjilAI, MoonScriptCreatureAI);
	NarjilAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(NARJIL_WEBWRAP, Target_RandomPlayer, 22, 0, 35, 0, 0);
		AddSpell(NARJIL_INFECTEDBITE, Target_ClosestPlayer, 35, 0, 12, 0, 0);
		AddSpell(NARJIL_BLINDINGWEBS, Target_ClosestPlayer, 16, 0, 9, 0, 0);
	}
};	

//Watcher Silthik
#define CN_SILTHIK 28731

#define SILTHIK_WEBWRAP 52086
#define SILTHIK_INFECTEDBITE 52469
#define SILTHIK_POISONSPRAY 52493

class SilthikAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SilthikAI, MoonScriptCreatureAI);
	SilthikAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(NARJIL_WEBWRAP, Target_RandomPlayer, 22, 0, 35, 0, 0);
		AddSpell(NARJIL_INFECTEDBITE, Target_ClosestPlayer, 35, 0, 12, 0, 0);
		AddSpell(SILTHIK_POISONSPRAY, Target_RandomPlayer, 30, 0, 15, 0, 0);
	}
};	

//Anub'ar Shadowcaster (anub shadowcaster)
#define CN_ANUB_SHADOWCASTER 28733

#define SHADOWCASTER_SHADOWBOLT 52534
#define SHADOWCASTER_SHADOW_NOVA 52535

class AnubShadowcasterAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(AnubShadowcasterAI, MoonScriptCreatureAI);
	AnubShadowcasterAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWCASTER_SHADOWBOLT, Target_RandomPlayer, 36, 0, 8);
		AddSpell(SHADOWCASTER_SHADOW_NOVA, Target_Self, 22, 0, 15);
	}
};	

//Anub'ar Skirmisher (anub skirmisher)
#define CN_ANUB_SKIRMISHER 28734

#define SKIRMISHER_BACKSTAB 52540

class AnubSkirmisherAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(AnubSkirmisherAI, MoonScriptCreatureAI);
	AnubSkirmisherAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SKIRMISHER_BACKSTAB, Target_ClosestPlayer, 36, 0, 8);
	}
};	

//Anub'ar Venomancer (anub venomancer)
#define CN_ANUB_VENOMANCER 29217

#define VENOMANCER_POISONBOLT 53617

class AnubVenomancerAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(AnubVenomancerAI, MoonScriptCreatureAI);
	AnubVenomancerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(VENOMANCER_POISONBOLT, Target_RandomPlayer, 25, 0, 6);
	}
};	

//Anub'ar Warrior (anub warrior)
#define CN_ANUB_WARRIOR 28732

#define WARRIOR_CLAVE 49806
#define WARRIOR_STRIKE 52532

class AnubWarriorAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(AnubWarriorAI, MoonScriptCreatureAI);
	AnubWarriorAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(WARRIOR_CLAVE, Target_ClosestPlayer, 35, 0, 7);
		AddSpell(WARRIOR_STRIKE, Target_ClosestPlayer, 10, 0, 10);
	}
};	

//Anub'ar Webspinner (anub webspinner)
#define CN_ANUB_WEBSPINNER 29335

#define WEBSPINNER_WEBSHOT 54290
#define WEBSPINNER_WEBWRAP 52086

class AnubWebspinnerAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(AnubWebspinnerAI, MoonScriptCreatureAI);
	AnubWebspinnerAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(WEBSPINNER_WEBSHOT, Target_RandomPlayer, 30, 0, 5);
		AddSpell(WEBSPINNER_WEBWRAP, Target_RandomPlayer, 20, 0, 12);
	}
};	
void SetupAzjolNerub(ScriptMgr* pScriptMgr) 
{
	pScriptMgr->register_creature_script(BOSS_KRIKTHIR, &KrikthirAI::Create);
	pScriptMgr->register_creature_script(BOSS_HADRONOX, &HadronoxAI::Create);
	pScriptMgr->register_creature_script(CN_GASHRA, &GashraAI::Create);
	pScriptMgr->register_creature_script(CN_NARJIL, &NarjilAI::Create);
	pScriptMgr->register_creature_script(CN_SILTHIK, &SilthikAI::Create);
	pScriptMgr->register_creature_script(CN_ANUB_SHADOWCASTER, &AnubShadowcasterAI::Create);
	pScriptMgr->register_creature_script(CN_ANUB_SKIRMISHER, &AnubSkirmisherAI::Create);
	pScriptMgr->register_creature_script(CN_ANUB_VENOMANCER, &AnubVenomancerAI::Create);
	pScriptMgr->register_creature_script(CN_ANUB_WARRIOR, &AnubWarriorAI::Create);
	pScriptMgr->register_creature_script(CN_ANUB_WEBSPINNER, &AnubWebspinnerAI::Create);
}
	