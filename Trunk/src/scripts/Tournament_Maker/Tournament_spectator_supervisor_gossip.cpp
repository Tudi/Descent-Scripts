#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"

//this NPC needs to handle players that wish to join the Tournament as spectators
class SCRIPT_DECL TSTNPCGOSSIP : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
    void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
    void GossipEnd(Object * pObject, Player* Plr);
	void Destroy()	{		delete this;	}
	void send_MainPage(Object * pObject, Player* Plr)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		Menu->AddItem(0, TOURNAMENT_SPECTATOR_TELEPORT_NPC_GOSSIP, 2);
		Menu->SendTo(Plr);
	}
};

void TSTNPCGOSSIP::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
	if( AutoSend && tournament_started == 1 )
		send_MainPage(pObject,Plr);
	else if( Plr )
		Plr->BroadcastMessage( TOURNAMENT_SPECTATOR_TELEPORT_NPC_REFUSE_TOURNAMENT );
}

void TSTNPCGOSSIP::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
		return;
	switch(IntId)
    {
	    case 1:	
		default:
			if( tournament_started == 1 )
				send_MainPage(pObject,Plr);						
			else 
				Plr->BroadcastMessage( TOURNAMENT_SPECTATOR_TELEPORT_NPC_REFUSE_TOURNAMENT );
			break;

		case 2:	
		{
			//check if anyone left their seet to find empty ones
			RefreshSpectators();

			//check if we have open seets
			uint32 valid_spectators = 0;
			for( uint32 i = 0; i < NUMBER_OF_SPECTATOR_SEETS; i++ )
				if( spectator_guids[i] != 0 )
					valid_spectators++;

			if( valid_spectators >= NUMBER_OF_SPECTATOR_SEETS )
			{
				Plr->BroadcastMessage( TOURNAMENT_SPECTATOR_TELEPORT_NPC_REFUSE_SEAT );
				break;
			}

			//we can add a new spectator
			MakeUnitSpectator( Plr , 1);

			//close that menu
			Plr->CloseGossip();
		}break;
	}
}

void TSTNPCGOSSIP::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

//NPC spawned all around the world to be able to handle Tournament teleports
class TSTNPCAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TSTNPCAI);

	//constructor
    TSTNPCAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//make us friendly
		_unit->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, SUPERVISOR_FACION );
		_unit->_setFaction();

		//make us imune to every spell we know of :P
		for(uint32 i=0;i<7;i++)
			_unit->SchoolImmunityList[i] = 999;
		for(uint32 i=0;i<31;i++)
			_unit->MechanicsDispels[i] = 999;

		//make him an NPC who can talk
		_unit->SetUInt32Value( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP );
    }
};

void SetupTournamentSpectatorSupervisorGossip(ScriptMgr * mgr)
{
	//make him have a gossip
	GossipScript * gs = (GossipScript*) new TSTNPCGOSSIP();
    mgr->register_gossip_script(TOURNAMENT_SPECTATOR_TELPORT_NPC_ENTRY, gs);

	//make him not react to stuff
    mgr->register_creature_script(TOURNAMENT_SPECTATOR_TELPORT_NPC_ENTRY, &TSTNPCAI::Create);
}
