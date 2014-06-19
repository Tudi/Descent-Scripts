/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Selina Dourman		#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class SCRIPT_DECL SelinaDourman : public GossipScript{
public:
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
	void GossipEnd(Object * pObject, Player* Plr){ GossipScript::GossipEnd(pObject, Plr); }
	void Destroy(){ delete this; }
};

void SelinaDourman::GossipHello(Object * pObject, Player* Plr, bool AutoSend){
	GossipMenu *Menu;

	// Main text
	objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60035, Plr);
	Menu->AddItem(0, "What can I purchase?", 1);
	Menu->AddItem(0, "What are Darkmoon Faire Prize Tickets and how do I get them?", 2);
	Menu->AddItem(0, "What are Darkmoon Cards?", 3);
	Menu->AddItem(0, "What other things can I do at the faire?", 4);

	if(AutoSend) 
		Menu->SendTo(Plr);
}

void SelinaDourman::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code){
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;

	if(pCreature==NULL)
		return;

    GossipMenu * Menu;
		switch(IntId)
		
		{
			case 1:
				// What can I purchase?
				objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60036, Plr);
				Menu->SendTo(Plr);
			break;
		
			case 2:
				// What are Darkmoon Faire Prize Tickets and how do I get them?
				objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60037, Plr);
				Menu->SendTo(Plr);
			break;
		
			case 3:
				// What are Darkmoon Cards?
				objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60038, Plr);
				Menu->AddItem(0, "<more>", 10);
				Menu->SendTo(Plr);
			break;

			case 4:
				// What other things can I do at the faire?
				objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60040, Plr);
				Menu->AddItem(0, "What are these Tonk Control Consoles?", 20);
				Menu->AddItem(0, "Tell me about the cannon.", 21);
				Menu->SendTo(Plr);
			break;

			case 10:
				// What are Darkmoon Cards? <more>
				objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60039, Plr);
				Menu->SendTo(Plr);
			break;

			case 20:
				// What are these Tonk Control Consoles?
				objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60041, Plr);
				Menu->SendTo(Plr);
			break;

			case 21:
				// Tell me about the cannon.
				objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60042, Plr);
				Menu->SendTo(Plr);
			break;
		}
}

void SetupSelinaDourman(ScriptMgr * mgr)
	{
		GossipScript * gs = (GossipScript*) new SelinaDourman();
    	mgr->register_gossip_script(10445, gs);
	}
