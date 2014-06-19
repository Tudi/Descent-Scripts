#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"

//this NPC needs to handle players that wish to join the Tournament as spectators
//only GMs can select and talk to him
class SCRIPT_DECL TSNPCGOSSIP : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
    void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
    void GossipEnd(Object * pObject, Player* Plr);
	void Destroy()	{		delete this;	}
	void send_MainPage(Object * pObject, Player* Plr)
	{
		if( !Plr || !Plr->GetSession() || !Plr->GetSession()->HasGMPermissions() )
			return; //GM only menu

		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		char msg[ MAX_STRING_LENGTH ];
		Menu->AddItem(0, SUPERVISOR_GOSSIPGM_MENU_START, 2);
		if( team1name[0] != 0 )
		{
			Menu->AddItem(0, SUPERVISOR_GOSSIPGM_MENU_ABORT, 3);
			sprintf(msg,SUPERVISOR_GOSSIPGM_MENU_FORCE_LOOSE1,team1name);
			Menu->AddItem(0, msg, 4);
			sprintf(msg,SUPERVISOR_GOSSIPGM_MENU_FORCE_LOOSE2,team2name);
			Menu->AddItem(0, msg, 5);
			sprintf(msg,SUPERVISOR_GOSSIPGM_MENU_FORCE_WIN1,team1name);
			Menu->AddItem(0, msg, 6);
			sprintf(msg,SUPERVISOR_GOSSIPGM_MENU_FORCE_WIN2,team2name);
			Menu->AddItem(0, msg, 7);
			sprintf(msg,SUPERVISOR_GOSSIPGM_MENU_FORCE_KICK1,team1name);
			Menu->AddItem(0, msg, 8);
			sprintf(msg,SUPERVISOR_GOSSIPGM_MENU_FORCE_KICK2,team2name);
			Menu->AddItem(0, msg, 9);
		}
		Menu->SendTo(Plr);
	}
};

void TSNPCGOSSIP::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
	if( AutoSend )
		send_MainPage(pObject,Plr);
}

void TSNPCGOSSIP::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
		return;
	switch(IntId)
    {
	    case 1:		send_MainPage(pObject,Plr);													break;

		case 2:		
			pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_START);	
			Plr->CloseGossip();
			break;
		case 3:		
			pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_ABORT);		
			Plr->CloseGossip();
			break;
		case 4:		
			pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_LOOSE1);	
			Plr->CloseGossip();
			break;
		case 5:		
			pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_LOOSE2);	
			Plr->CloseGossip();
			break;
		case 6:		
			pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_WIN1);		
			Plr->CloseGossip();
			break;
		case 7:		
			pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_WIN2);		
			Plr->CloseGossip();
			break;
		case 8:		
			pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_KICK1);		
			Plr->CloseGossip();
			break;
		case 9:		
			pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_KICK2);
			Plr->CloseGossip();
			break;

		default:	send_MainPage(pObject,Plr);													break;
	}
}

void TSNPCGOSSIP::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupTournamentSupervisorGossip(ScriptMgr * mgr)
{
	//make him have a gossip
	GossipScript * gs = (GossipScript*) new TSNPCGOSSIP();
    mgr->register_gossip_script(CREATURE_ENTRY_FOR_SPECTATOR_SUPERVIZOR, gs);
}
