/* 
#####################################
#  The Darkmoon Faire Project		#
# ---------------------------------	#
# Author: Nexis						#
# Script: Professor Thaddeus Paleo	#
#####################################
*/

#include "StdAfx.h"
#include "Setup.h"

class SCRIPT_DECL ProfessorThaddeusPaleo : public GossipScript
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

void ProfessorThaddeusPaleo::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
    GossipMenu *Menu;
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60016, Plr);

    if( pObject->GetUInt32Value(UNIT_NPC_FLAGS) & UNIT_NPC_FLAG_VENDOR )
        Menu->AddItem( 1, "Let me browse your goods.", 1 );
		Menu->AddItem( 0, "Tell me more about these Darkmoon Cards.", 2 );

    if(AutoSend)
        Menu->SendTo(Plr);
}

void ProfessorThaddeusPaleo::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
		return;

    switch(IntId)
    {
    case 1:
		Plr->GetSession()->SendInventoryList(pCreature);
    break;

    case 2:
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60017, Plr);

		Menu->AddItem( 0, "Tell me about the Beasts Deck.", 5 );
		Menu->AddItem( 0, "Tell me about the Portals Deck.", 6 );
		Menu->AddItem( 0, "Tell me about the Elementals Deck.", 7 );
		Menu->AddItem( 0, "Tell me about the Warlords Deck.", 8 );
		Menu->AddItem( 0, "Tell me about the Furies Deck.", 9 );
		Menu->AddItem( 0, "Tell me about the Lunacy Deck.", 10 );
		Menu->AddItem( 0, "Tell me about the Blessings Deck.", 11 );
		Menu->AddItem( 0, "Tell me about the Storms Deck.", 12 );
		
		Menu->SendTo(Plr);
    break;

	case 5:
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60018, Plr);
		Menu->SendTo(Plr);
    break;

	case 6:
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60019, Plr);
		Menu->SendTo(Plr);
    break;

	case 7:
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60020, Plr);
		Menu->SendTo(Plr);
    break;

	case 8:
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60021, Plr);
		Menu->SendTo(Plr);
    break;

	case 9:
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60022, Plr);
		Menu->SendTo(Plr);
    break;

	case 10:
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60023, Plr);
		Menu->SendTo(Plr);
    break;

	case 11:
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60024, Plr);
		Menu->SendTo(Plr);
    break;

	case 12:
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60025, Plr);
		Menu->SendTo(Plr);
    break;

    }
}

class ProfessorThaddeusPaleoTalk : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ProfessorThaddeusPaleoTalk);
	ProfessorThaddeusPaleoTalk(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// Speaks 3:30
		RegisterAIUpdateEvent(210000);
	}

	void AIUpdate()
	{
		int RandomText;
			RandomText=rand()%4;
			switch (RandomText)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "If you wait to come see me, there may be nothing left!" );
			break;

			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Have you any Darkmoon Faire Cards? Come speak with me to learn more about them if you dare!" );
			break;

			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Welcome one and all to the greatest show on all of Azeroth... the Darkmoon Faire! Adventure and exitement await, don't be shy! Mysteries and prizes for both the young and the old are here for the taking. And be sure to speak with the professor about any Darkmoon Cards that you have come across during your travels!" );
			break;

			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come speak with me if you are a purveyor of the unusual, or if you have any Darkmoon Faire cards." );
			break;

			}
	}

};

void SetupProfessorThaddeusPaleoTalk(ScriptMgr *mgr)
{
	mgr->register_creature_script(14847, &ProfessorThaddeusPaleoTalk::Create);
}

void ProfessorThaddeusPaleo::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupProfessorThaddeusPaleo(ScriptMgr * mgr)
{
	GossipScript * gs = (GossipScript*) new ProfessorThaddeusPaleo();
    mgr->register_gossip_script(14847, gs);
}