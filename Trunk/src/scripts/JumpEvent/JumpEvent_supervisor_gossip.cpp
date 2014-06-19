#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"


//this NPC needs to handle players that wish to join the Tournament as spectators
//only GMs can select and talk to him
class SCRIPT_DECL JESUPGOSSIP : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
    void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
    void GossipEnd(Object * pObject, Player* Plr);
	void Destroy()	{		delete this;	}
	void send_MainPage(Object * pObject, Player* Plr)
	{
		if( Plr == false )
			return; 

		//check if player is already participant in one of the events
		uint64 playerguid = Plr->GetGUID();
		uint32 stampnow = GetTickCount();
		for(uint32 i=0;i<MAX_NUMBER_OF_CONCURENT_JUMP_EVENT_HOLDERS;i++)
			if( JumpEventHolders[i].PlayerGUID == playerguid 
				&& stampnow < JumpEventHolders[i].EventExpireStamp
				)
			{
				Plr->BroadcastMessage("You are already part of a jumpevent. You need to wait until it will expire to be able to start a new one\n");
				return;
			}

		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		Menu->AddItem( 0, "I wish to have a new speed record",3);
		Menu->AddItem( 0, "I give up on my Jump Event",4);
		Menu->SendTo(Plr);
	}
};

void JESUPGOSSIP::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
	if( AutoSend )
		send_MainPage(pObject,Plr);
}

void JESUPGOSSIP::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
		return;
	switch(IntId)
    {
		// info menu
	    case 1:		send_MainPage(pObject,Plr);													break;
		// if he is redirected here then menu should be closed
	    case 2:		Plr->CloseGossip();															break;
		// start a jump event
		case 3:		
			StartJumpEvent( Plr, pCreature->GetGUID() );
			Plr->CloseGossip();
			break;
		//stop a jump event
		case 4:		
			EarlyEndJumpEvent( Plr, pCreature->GetGUID() );
			Plr->CloseGossip();
			break;
		default:	send_MainPage(pObject,Plr);													break;
	}
}

void JESUPGOSSIP::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void JumpEventSupervisorGossipIngame(ScriptMgr * mgr)
{
	//make our initialization
	memset(&JumpEventHolders, 0, sizeof( JumpEventHolders ) );

	//make him have a gossip
	GossipScript * gs = (GossipScript*) new JESUPGOSSIP();
    mgr->register_gossip_script(CREATURE_ENTRY_FOR_SUPERVIZOR, gs);
}
