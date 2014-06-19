/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Burth					#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class SCRIPT_DECL Burth : public GossipScript
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

void Burth::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
    {
        GossipMenu *Menu;
		
		int RandomText;
			RandomText=rand()%8;
			switch (RandomText)
			{
			case 0:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60000, Plr);
				break;
			case 1:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60001, Plr);
				break;
			case 2:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60002, Plr);
				break;
			case 3:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60003, Plr);
				break;
			case 4:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60004, Plr);
				break;
			case 5:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60005, Plr);
				break;
			case 6:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60006, Plr);
				break;
			case 7:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60050, Plr);
				break;
			}
                        
        if(AutoSend)
            Menu->SendTo(Plr);
	}

void Burth::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
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

void Burth::GossipEnd(Object * pObject, Player* Plr)
	{
		GossipScript::GossipEnd(pObject, Plr);
	}

void SetupBurth(ScriptMgr * mgr)
	{
		GossipScript * gs = (GossipScript*) new Burth();
    	mgr->register_gossip_script(14827, gs);
	}