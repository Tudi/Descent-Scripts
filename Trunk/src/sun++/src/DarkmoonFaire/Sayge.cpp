/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Sayge					#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class SCRIPT_DECL Sayge : public GossipScript{
public:
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
	void GossipEnd(Object * pObject, Player* Plr);
	void Destroy()
    {
        delete this;
    }
};

void Sayge::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
	GossipMenu *Menu;

	// Check to see if the player already has a buff from Sayge.
	if (Plr->HasAura(23768) || Plr->HasAura(23769) || Plr->HasAura(23767) || Plr->HasAura(23738) || Plr->HasAura(23766) || Plr->HasAura(23737) || Plr->HasAura(23735) || Plr->HasAura(23736))
	{
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60034, Plr); // They have the buff, denied.
	} else {
		// They don't have the buff, continue.
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60026, Plr);
		Menu->AddItem(0, "I am ready to discover where my fortune lies!", 1);
	}
                        
	if(AutoSend)
		Menu->SendTo(Plr);
}

void Sayge::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;

	if(pCreature==NULL)
		return;

	GossipMenu * Menu;
	switch(IntId)

	{
		case 1:		// Question 1 (Initial question, always the same)
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60027, Plr);

            Menu->AddItem(0, "I slay the man on the spot as my liege would expect me to do, as he is nothing more than a thief and a liar.", 10);
			Menu->AddItem(0, "I turn over the man to my liege for punishment, as he has broken the law of the land and it is my sworn duty to enforce it.", 11);
			Menu->AddItem(0, "I confiscate the corn he has stolen, warn him that stealing is a path towards doom and destruction, but I let him go to return to his family.", 12);
			Menu->AddItem(0, "I allow the man to take enough corn to feed his family for a couple of days, encouraging him to leave the land.", 13);
            
            Menu->SendTo(Plr);
		break;

        case 10:	// Question 2 (First Answer = 1)
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60028, Plr);

            Menu->AddItem(0, "I execute him as per my liege's instructions, and do it in such a manner that he suffers painfully before he dies a retribution for his crimes against my people.", 14);
			Menu->AddItem(0, "I execute him as per my liege's instructions, but doing so in as painless of a way as possible. Justice must be served, but I can show some compassion.", 15);
			Menu->AddItem(0, "I risk my own life and free him so that he may prove his innocence. If I can, I'll help him in any way.", 16);
			            
            Menu->SendTo(Plr);
		break;

		case 11:     // Question 2 (First Answer = 2)
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60029, Plr);

            Menu->AddItem(0, "I confront the ruler on his malicious behavior, upholding my liege's honor at risk of any future diplomacy.", 17);
			Menu->AddItem(0, "I not-so-quietly ignore the insult, hoping to instill a fear in the ruler that he may have gaffed. I then inform my liege of the insult when I return.", 18);
			Menu->AddItem(0, "I quietly ignore the insult. I will not tell my liege, as I am to secure peace at all costs. It's only an insult - not a declaration of war.", 19);
			            
            Menu->SendTo(Plr);
		break;

		case 12:     // Question 2 (First Answer = 3)
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60030, Plr);

            Menu->AddItem(0, "I would speak against my brother joining the order, rushing a permanent breech in our relationship. He would be a danger to himself and those around him, and that is too great a risk hoping he would improve over time.", 20);
			Menu->AddItem(0, "I would speak for my brother joining the order, potentially risking the safety of the order. I could help him with the order's regimens, and I'd have faith in his ability to adapt and learn.", 21);
			Menu->AddItem(0, "I would create some surreptitious means to keep my brother out of the order. I can keep him out without him being any bit wiser, thereby saving our familial bonds.", 22);
			            
            Menu->SendTo(Plr);
		break;

		case 13:     // Question 2 (First Answer = 4)
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60031, Plr);

            Menu->AddItem(0, "I would show my liege the beast's ear and claim the beast's death as my own, taking the reward for my own use. It is wrong to claim a deed as your own that someone else in fact did.", 23);
			Menu->AddItem(0, "I would show my liege the beast's ear and claim the beast's death as my own - after all, I did slay it. I would then offer some of the reward to the destitute knight to help his family.", 24);
			Menu->AddItem(0, "I would remain silent about the kill and allow the knight to claim the reward to aid his family.", 25);
			            
            Menu->SendTo(Plr);
		break;

		// Answers 1-#
		case 14:     // Answers: 1-1
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, Plr);

            Menu->AddItem(0, "I'd love to get one of those written fortunes you mentioned! I've got the space in my inventory for it.", 30);
            Menu->SendTo(Plr);

			pCreature->CastSpell(Plr, 23768, true);
		break;

		case 15:     // Answers: 1-2
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, Plr);

            Menu->AddItem(0, "I'd love to get one of those written fortunes you mentioned! I've got the space in my inventory for it.", 30);
            Menu->SendTo(Plr);
					
			pCreature->CastSpell(Plr, 23769, true);
		break;

		case 16:     // Answers: 1-3
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, Plr);

            Menu->AddItem(0, "I'd love to get one of those written fortunes you mentioned! I've got the space in my inventory for it.", 30);
            Menu->SendTo(Plr);
					
			pCreature->CastSpell(Plr, 23767, true);
		break;

		// Answers 2-#
        case 17:     // Answers: 2-1
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, Plr);

            Menu->AddItem(0, "I'd love to get one of those written fortunes you mentioned! I've got the space in my inventory for it.", 30);
            Menu->SendTo(Plr);
					
			pCreature->CastSpell(Plr, 23738, true);
		break;

		case 18:     // Answers: 2-2
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, Plr);

            Menu->AddItem(0, "I'd love to get one of those written fortunes you mentioned! I've got the space in my inventory for it.", 30);
            Menu->SendTo(Plr);
					
			pCreature->CastSpell(Plr, 23766, true);	
		break;

		case 19:     // Answers: 2-3
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, Plr);

            Menu->AddItem(0, "I'd love to get one of those written fortunes you mentioned! I've got the space in my inventory for it.", 30);
            Menu->SendTo(Plr);
					
			pCreature->CastSpell(Plr, 23769, true);
		break;

		// Answers 3-#
        case 20:     // Answers: 3-1
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, Plr);

            Menu->AddItem(0, "I'd love to get one of those written fortunes you mentioned! I've got the space in my inventory for it.", 30);
            Menu->SendTo(Plr);
					
			pCreature->CastSpell(Plr, 23737, true);	
		break;

		case 21:     // Answers: 3-2
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, Plr);

            Menu->AddItem(0, "I'd love to get one of those written fortunes you mentioned! I've got the space in my inventory for it.", 30);
            Menu->SendTo(Plr);
					
			pCreature->CastSpell(Plr, 23735, true);	
		break;

		case 22:     // Answers: 3-3
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, Plr);

            Menu->AddItem(0, "I'd love to get one of those written fortunes you mentioned! I've got the space in my inventory for it.", 30);
            Menu->SendTo(Plr);
					
			pCreature->CastSpell(Plr, 23736, true);
		break;

		// Answers 4-#
        case 23:     // Answers: 4-1
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, Plr);

            Menu->AddItem(0, "I'd love to get one of those written fortunes you mentioned! I've got the space in my inventory for it.", 30);
            Menu->SendTo(Plr);
					
			pCreature->CastSpell(Plr, 23766, true);		
		break;

		case 24:     // Answers: 4-2
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, Plr);

            Menu->AddItem(0, "I'd love to get one of those written fortunes you mentioned! I've got the space in my inventory for it.", 30);
            Menu->SendTo(Plr);
					
			pCreature->CastSpell(Plr, 23738, true);
		break;

		case 25:     // Answers: 4-3
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60032, Plr);

            Menu->AddItem(0, "I'd love to get one of those written fortunes you mentioned! I've got the space in my inventory for it.", 30);        
            Menu->SendTo(Plr);
					
			pCreature->CastSpell(Plr, 23737, true);	
		break;

		case 30:
			objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60033, Plr);
			Menu->SendTo(Plr);

			// Cast the fortune into the player's inventory - Not working?
			pCreature->CastSpell(Plr, 23765, true);

			// TEMP fix for spell not adding item to  player's inventory.
			if (Plr->GetItemInterface()->CalculateFreeSlots(ItemPrototypeStorage.LookupEntry(19422)))			
			{				
				Item *titem = objmgr.CreateItem(19422, Plr);
				Plr->GetItemInterface()->AddItemToFreeSlot(&titem); //Darkmoon Faire Fortune				
			} else	{				
				sChatHandler.SystemMessage(Plr->GetSession(), "You do not have enough free slots for your fortune!");			
			}
		break;

	}
}

class SaygeTalk : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SaygeTalk);
	SaygeTalk(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent(135000);
	}

	void AIUpdate()
	{
		int RandomText;
		RandomText=rand()%4;
		switch (RandomText)
		{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Fear not! All fortunes can be discerned for those who know the way." );
			break;

			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The longer you wait, the less future you have for me to foretell." );
			break;

			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come speak with me, and what once was cloudy shall become crystal clear." );
			break;

			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come to me true believers, and see what the future holds for you!" );
			break;
		}
	}
};

void SetupSaygeTalk(ScriptMgr *mgr)
{
	mgr->register_creature_script(14822, &SaygeTalk::Create);
}

void Sayge::GossipEnd(Object * pObject, Player* Plr)
{
	GossipScript::GossipEnd(pObject, Plr);
}

void SetupSayge(ScriptMgr * mgr)
{
	GossipScript * gs = (GossipScript*) new Sayge();
    mgr->register_gossip_script(14822, gs);
}