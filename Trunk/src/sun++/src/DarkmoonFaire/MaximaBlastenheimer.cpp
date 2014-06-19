/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Maxima Mlastenheimer	#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

/*
Created by: Nexis
Script: Maxima Blastenheimer
*/

class SCRIPT_DECL MaximaBlastenheimer : public GossipScript{
public:
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
	void GossipEnd(Object * pObject, Player* Plr){ GossipScript::GossipEnd(pObject, Plr); }
	void Destroy(){ delete this; }
};

void MaximaBlastenheimer::GossipHello(Object * pObject, Player* Plr, bool AutoSend){
	GossipMenu *Menu;

	// Main text
	objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60048, Plr);
	Menu->AddItem(0, "Tell me how to use the Blastenheimer 5000 Ultra Cannon.", 1);

	if(AutoSend) 
		Menu->SendTo(Plr);
}

void MaximaBlastenheimer::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code){
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;

	if(pCreature==NULL)
		return;

    GossipMenu * Menu;
		switch(IntId)
		
		{
			case 0: 
				GossipHello(pCreature, Plr, true);
			break;

			case 1:
			// Second text
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60049, Plr);
			
			Menu->SendTo(Plr);
			break;
		}
}

void SetupMaximaBlastenheimer(ScriptMgr * mgr)
	{
		GossipScript * gs = (GossipScript*) new MaximaBlastenheimer();
    	mgr->register_gossip_script(15303, gs);
	}
