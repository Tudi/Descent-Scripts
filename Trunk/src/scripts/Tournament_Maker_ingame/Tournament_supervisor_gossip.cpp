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
		if( Plr == false )
			return; 

		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		Menu->SetTextID( 100000 );
		Menu->AddItem( 0, "Queue for a Tournament",2);
		Menu->SendTo(Plr);
	}
	void send_StatusPage(Object * pObject, Player* Plr)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

		bool isleader;
		if( Plr->IsGroupLeader() )
			isleader = true;
		else
			isleader = false;
		for( int32 i = 0; i < TOURNAMENT_TYPE_COUNT; i++)
		{
			char tstr[500];
			uint32 teamsize = Tournaments[ i ].max_players_in_team;

			if( Tournaments[ i ].status != TS_NOT_STARTED )
			{
				sprintf( tstr,"%s Tournament started. Come back later.", TournamentTypeNames[ i ] );
				Menu->AddItem( 0, tstr,2);
			}
			else
			{
				//count teams registered so far
				uint32 teams_registered_so_far = 0;
				for(uint32 j=0;j<MAX_TEAMS_PER_TOURNAMENT;j++)
					if( Tournaments[ i ].teams_registered[ j ].is_used != 0 )
						teams_registered_so_far++;

				sprintf( tstr,"Queue for teamsize %s Tournament(%u/%u|%u->%u)", TournamentTypeNames[ i ],teams_registered_so_far,MAX_TEAMS_PER_TOURNAMENT,Tournaments[ i ].MinLevel,Tournaments[ i ].MaxLevel);
				Menu->AddMenuItem( 0, tstr,0,0,"Input your unique team name to join/create",0,1,3+i);
				if( isleader )
				{
					sprintf( tstr,"Queue for teamsize %s Tournament as group(%u/%u|%u->%u)", TournamentTypeNames[ i ],teams_registered_so_far,MAX_TEAMS_PER_TOURNAMENT,Tournaments[ i ].MinLevel,Tournaments[ i ].MaxLevel);
					Menu->AddMenuItem( 0, tstr,0,0,"Input your unique team name to join/create",0,1,3+TOURNAMENT_TYPE_COUNT+i);
				}
			}
		}
		// leaving so soon ?
		Menu->AddItem( 0, "Leave all tournament queues",100);
		//special menu for GMs
		if( Plr->GetSession() && Plr->GetSession()->HasGMPermissions() )
		{
			Menu->AddItem( 0, "Force start all tournaments",660);
		}
		Menu->SendTo(Plr);
	}
};

void TSNPCGOSSIP::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
	if( AutoSend )
		send_MainPage(pObject,Plr);
}

bool  RemovePlayerFromTournamentQueue( Player *plr )
{
	uint64 guid_to_remove = plr->GetGUID();
	for(uint32 i=0;i<TOURNAMENT_TYPE_COUNT;i++)
		for(uint32 j=0;j<MAX_TEAMS_PER_TOURNAMENT;j++)
			if( Tournaments[ i ].teams_registered[ j ].is_used != 0 )
				for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
					if( Tournaments[ i ].teams_registered[ j ].player_guids[ k ] == guid_to_remove )
					{
						Tournaments[ i ].teams_registered[ j ].player_guids[ k ] = 0;
						return true;
					}
	return false;
}

void QueuePlayerForTournament( Player *plr, uint32 TournamentType,const char *team_name)
{
	if( TournamentType >= TOURNAMENT_TYPE_COUNT )
	{
		plr->BroadcastMessage(" Invalid tournament type. Report to devs. ",TournamentTypeNames[ TournamentType ] );
		return;
	}
	if( Tournaments[ TournamentType ].status != TS_NOT_STARTED )
	{
		plr->BroadcastMessage(" Tournament %s Started. Wait to finish ",TournamentTypeNames[ TournamentType ] );
		return;
	}
	if( team_name == NULL || strlen( team_name ) == 0 || strlen( team_name ) >= MAX_TOURNAMENT_TEAM_NAME_LEN )
	{
		plr->BroadcastMessage(" Team name is inapropriate to register. Min len 1, max len %u ",MAX_TOURNAMENT_TEAM_NAME_LEN);
		return;
	}

	UpdateMutex.Acquire();
	//make sure he is not already registered
	RemovePlayerFromTournamentQueue( plr );

	bool needs_team_create=true;
	//try to see if we find this team
	for(uint32 j=0;j<MAX_TEAMS_PER_TOURNAMENT;j++)
		if( Tournaments[ TournamentType ].teams_registered[ j ].is_used != 0 
			&& strstr( team_name,Tournaments[ TournamentType ].teams_registered[ j ].name ) == team_name )
		{
			//check if team has open positions
			uint32 found_players = 0, open_slot;
			for(uint32 i = 0;i< Tournaments[ TournamentType ].max_players_in_team; i++)
				if( Tournaments[ TournamentType ].teams_registered[ j ].player_guids[ i ] != 0 )
					found_players++;
				else 
					open_slot = i;
			if( found_players >= Tournaments[ TournamentType ].max_players_in_team )
			{
				plr->BroadcastMessage(" Team with name '%s' is full ",team_name );
				needs_team_create =  false;
				break;
			}
			Tournaments[ TournamentType ].teams_registered[ j ].player_guids[ open_slot ] = plr->GetGUID();
			if( found_players == 0 )
				Tournaments[ TournamentType ].teams_registered[ j ].leader_guid = plr->GetGUID();
			plr->BroadcastMessage(" Added you to tournament queue inside team '%s'",team_name);
			needs_team_create = false;
			break;
		}
	if( needs_team_create == true )
	{
		//add a new team to our list
		uint32 slot = MAX_TEAMS_PER_TOURNAMENT;
		for(uint32 j=0;j<MAX_TEAMS_PER_TOURNAMENT;j++)
			if( Tournaments[ TournamentType ].teams_registered[ j ].is_used == 0 )
			{
				slot = j;
				break;
			}
		if( slot < MAX_TEAMS_PER_TOURNAMENT )
		{
			Team_Info *freespot = &Tournaments[ TournamentType ].teams_registered[ slot ];
			freespot->id = cur_team_id++;
			freespot->leader_guid = plr->GetGUID();
			freespot->matches_won_total = 0;
			freespot->matches_played_total = 0;
			strcpy( freespot->name, team_name );
			freespot->player_guids[ 0 ] = plr->GetGUID();
			freespot->is_used = 1;
			plr->BroadcastMessage(" Created new toutnament team with name '%s'",team_name);
		}
		else
		{
			plr->BroadcastMessage(" This tournament %s is full. Wait until it is finished ",TournamentTypeNames[ TournamentType ]);
		}
	}
	UpdateMutex.Release();
}

void TSNPCGOSSIP::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
		return;
	switch(IntId)
    {
		// info menu
	    case 1:		send_MainPage(pObject,Plr);													break;
	    case 2:		send_StatusPage(pObject,Plr);												break;
		//one of the tournament types
		case 3:		
		case 4:		
		case 5:		
		case 6:		
			QueuePlayerForTournament( Plr, IntId - 3, Code );
			Plr->CloseGossip();
			break;
		//one of the tournament types as group leader
		case (3+TOURNAMENT_TYPE_COUNT):		
		case (4+TOURNAMENT_TYPE_COUNT):		
		case (5+TOURNAMENT_TYPE_COUNT):		
		case (6+TOURNAMENT_TYPE_COUNT):		
			if( Plr->IsGroupLeader() == false || Plr->GetGroup() == NULL )
				Plr->BroadcastMessage("You are not a group leader");
			else
			{
				//button pusher is leader and has group
				QueuePlayerForTournament( Plr, IntId - 3 - TOURNAMENT_TYPE_COUNT, Code );
				Group * group = Plr->GetGroup(); 
				uint32 maxcount = IntId - 3 - TOURNAMENT_TYPE_COUNT - 1 + 2;	//-1 cause we added leader
				// Loop through each raid group.
				Plr->GetGroup()->Lock();
				uint32 count = group->GetSubGroupCount();
				for( uint8 k = 0; k < count; k++ )
				{
					SubGroup * subgroup = group->GetSubGroup( k );
					if( subgroup != NULL )
					{				
						for(GroupMembersSet::iterator itr = subgroup->GetGroupMembersBegin(); itr != subgroup->GetGroupMembersEnd(); ++itr)
						{
							if(!(*itr)->m_loggedInPlayer || Plr == (*itr)->m_loggedInPlayer) 
								continue;
							QueuePlayerForTournament((*itr)->m_loggedInPlayer, IntId - 3 - TOURNAMENT_TYPE_COUNT, Code );
							maxcount--;
							if( maxcount == 0 )
								break;
						}
					}
				}
				Plr->GetGroup()->Unlock();
			}
			Plr->CloseGossip();
			break;
		// remove from all queues
		case 100:		
			UpdateMutex.Acquire();
			if( RemovePlayerFromTournamentQueue( Plr ) )
				Plr->BroadcastMessage("You have been removed from the Tournament Queue");
			else
				Plr->BroadcastMessage("Could not find you in any of Tournament Queues");
			UpdateMutex.Release();
			Plr->CloseGossip();
			break;
		//gm commands
		case 660:
			debug_commands = SUPERVIZOR_COMMAND_FORCESTART;
			Plr->CloseGossip();
			break;
		default:	send_MainPage(pObject,Plr);													break;
	}
}

void TSNPCGOSSIP::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupTournamentSupervisorGossipIngame(ScriptMgr * mgr)
{
	//make him have a gossip
	GossipScript * gs = (GossipScript*) new TSNPCGOSSIP();
    mgr->register_gossip_script(CREATURE_ENTRY_FOR_IGTS_SUPERVIZOR, gs);
}
