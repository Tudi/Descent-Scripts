/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

// Cassa Crimsonwing
#define GOSSIP_CASSA_CRIMSONWING    "Lady Jaina told me to speak to yo about using a gryphon to survey Alcaz Island."

class CassaCrimsonwing_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 11224, plr);
        if(plr->GetQuestLogForEntry(11142) != NULL){
			Menu->AddItem( 0, GOSSIP_CASSA_CRIMSONWING, 1);
		};
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pCreature==NULL)
			return;

        switch(IntId)
        {
        case 1:
			{
				plr->GetQuestLogForEntry(11142)->SendQuestComplete();
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 724 ); // Flight Path
				plr->TaxiStart( path, 1147, 0 ); // Gryph
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

// Captain Garran Vimes
#define GOSSIP_CAPTAIN_GARRAN_VIMES    "What have you heard of the Shady Rest Inn?"

class CaptainGarranVimes_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1793, plr);
			
			Menu->AddItem( 0, GOSSIP_CAPTAIN_GARRAN_VIMES, 1);

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;

		GossipMenu * Menu;
        switch(IntId)
        {
        case 1:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1794, plr);
				Menu->SendTo(plr);
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

void SetupTheramoreGossip(ScriptMgr * mgr)
{
	GossipScript * CassaCrimsonwingGossip = (GossipScript*) new CassaCrimsonwing_Gossip;
	GossipScript * CaptainGarranVimesGossip = (GossipScript*) new CaptainGarranVimes_Gossip;
	
	mgr->register_gossip_script(23704, CassaCrimsonwingGossip); // Cassa Crimsonwing
	mgr->register_gossip_script(4944, CaptainGarranVimesGossip); // Captain Garran Vimes
}
