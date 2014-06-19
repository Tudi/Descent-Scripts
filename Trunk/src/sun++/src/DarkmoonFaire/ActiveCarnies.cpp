/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Active Carnies		#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class ActiveCarnies : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ActiveCarnies);
	ActiveCarnies(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// Speaks 1:10
		RegisterAIUpdateEvent(70000);
	}

	void AIUpdate()
	{
		int RandomText;
			RandomText=rand()%4;
			switch (RandomText)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Having a good time?" );
			break;

			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Sure are a lot of litter bugs around here." );
			break;

			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Don't forget to buy refreshments and souvenirs!" );
			break;

			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The Darkmoon Faire is the greatest show on all of Azeroth!" );
			break;

			}
	}

};


class SCRIPT_DECL ActiveCarniesGossip : public GossipScript
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

void ActiveCarniesGossip::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
    {
        GossipMenu *Menu;
		
		int RandomText;
			RandomText=rand()%4;
			switch (RandomText)
			{
			case 0:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60007, Plr);
				break;
			case 1:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60008, Plr);
				break;
			case 2:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60009, Plr);
				break;
			case 3:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60010, Plr);
				break;
			}
                        
        if(AutoSend)
            Menu->SendTo(Plr);
	}

void ActiveCarniesGossip::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
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

void ActiveCarniesGossip::GossipEnd(Object * pObject, Player* Plr)
	{
		GossipScript::GossipEnd(pObject, Plr);
	}

void SetupActiveCarniesGossip(ScriptMgr * mgr)
	{
		GossipScript * gs = (GossipScript*) new ActiveCarniesGossip();
    	mgr->register_gossip_script(14849, gs);
	}

void SetupActiveCarnies(ScriptMgr *mgr)
{
	mgr->register_creature_script(14849, &ActiveCarnies::Create);
}