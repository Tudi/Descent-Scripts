/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Fliks Frog			#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class SCRIPT_DECL FlikFrog : public GossipScript
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

void FlikFrog::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
    {
        GossipMenu *Menu;
		
		int RandomText;
			RandomText=rand()%2;
			switch (RandomText)
			{
			case 0:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60011, Plr);
				break;
			case 1:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60012, Plr);
				break;
			}
                        
        if(AutoSend)
            Menu->SendTo(Plr);
	}

void FlikFrog::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
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

void FlikFrog::GossipEnd(Object * pObject, Player* Plr)
	{
		GossipScript::GossipEnd(pObject, Plr);
	}

void SetupFlikFrog(ScriptMgr * mgr)
	{
		GossipScript * gs = (GossipScript*) new FlikFrog();
    	mgr->register_gossip_script(14866, gs);
	}