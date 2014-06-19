#include "StdAfx.h"
#include "Setup.h"

/* 
Created by: Nexis
Script: Darkmoon Faire Carnies
Revision: 10

Notes: This is far from complete but this is stage 1. I was forced to make custom
NPCs in order to make the Speech random throughout them. This will be my attempt
at making it based around a single NPC (blizzlike). All that really needs to be done
is to make the register timer random and once it hooks, to randomly apply to the npcs.
Long story short, have the NPCs randomly speak (not at the same time) using a single
NPC entry... Good luck to me, cause I'll need that shit!

Random Timer > Roll Guid Chance > Execute
*/

//Darkmoon Faire Carnies Speak
class SetupCarnie : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SetupCarnie);
	SetupCarnie(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// Random between 1:30 - 3:00
		RegisterAIUpdateEvent(120000);
	}

	void AIUpdate()
	{
		int RandomSpeech;
			RandomSpeech=rand()%4;
			switch (RandomSpeech)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Faire's a coming!" );
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Won't be long now until the Darkmoon Faire opens. Come back later and check to see if we're done." );
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Can't you see I've got work to do here?" );
				break;
			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "That's right friend! The Darkmoon Faire is going to be right here. Just as soon as I unload those huge wagons, put up all the tents, erect the zoo and pour the drinks. Make sure you come back!" );
				break;
			}
	}
};



// Darkmoon Faire Carnies Talk
class SCRIPT_DECL DMFCarniesTalk : public GossipScript
	{
	public:
		void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
		void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
		void GossipEnd(Object * pObject, Player* Plr);
		void Destroy()
    {
        delete this;
    }
};


void DMFCarniesTalk::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
    {
        GossipMenu *Menu;
		
		int RandomText;
			RandomText=rand()%4;
			switch (RandomText)
			{
			case 0:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 50000, Plr);
				break;
			case 1:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 50001, Plr);
				break;
			case 2:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 50002, Plr);
				break;
			case 3:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 50003, Plr);
				break;
			}
                        
        if(AutoSend)
            Menu->SendTo(Plr);
	}

void DMFCarniesTalk::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;

		if(pCreature==NULL)
			return;

        switch(IntId)
		{
		case 0:
			GossipHello(pCreature, Plr, true);
		break;
		}
}

void SetupSetupCarnies(ScriptMgr *mgr)
{
	mgr->register_creature_script(14849, &SetupCarnie::Create);
}

void DMFCarniesTalk::GossipEnd(Object * pObject, Player* Plr)
	{
		GossipScript::GossipEnd(pObject, Plr);
	}

void SetupSetupCarniesSpeech(ScriptMgr * mgr)
	{
		GossipScript * gs = (GossipScript*) new DSetupCarniesSpeech();
    	mgr->register_gossip_script(14849, gs);
	}
