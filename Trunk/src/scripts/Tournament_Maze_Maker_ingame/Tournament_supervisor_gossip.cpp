#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"
#include "Tournament_supervisor.h"

#define MENU_ID_STATUS_PAGE					2
#define MENU_ID_CREATE_JOIN_TEAM_AS_GROUP	3
#define MENU_ID_CREATE_JOIN_TEAM_AS_PLAYER	4

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
		Menu->SetTextID( 20050 );
		Menu->AddItem( 0, "Queue for Maze Event",2);
		Menu->SendTo(Plr);
	}
	void send_StatusPage(Maze_Map_Info *selected_map,Selected_Event_info *eventi,Object * pObject, Player* Plr)
	{

		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		if( selected_map == NULL )
		{
			Plr->BroadcastMessage( "! GM needs to select a MAP before player menu is available for event!");
		}
		else
		{
			bool isleader;
			if( Plr->IsGroupLeader() )
				isleader = true;
			else
				isleader = false;

			char tstr[500];
			uint32 teamsize = selected_map->max_players_in_team;

			if( eventi->status != TS_NOT_STARTED )
			{
				sprintf( tstr,"%s Maze Event started. Come back later.", selected_map->map_name_menu );
				Menu->AddItem( 0, tstr, MENU_ID_STATUS_PAGE);
			}
			else
			{
				//count teams registered so far
				uint32 teams_registered_so_far = 0;
				for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
					if( eventi->teams_registered[ j ].is_used != 0 )
						teams_registered_so_far++;

				if( selected_map->max_teams_per_event <= teams_registered_so_far )
				{
					sprintf( tstr,"Event Queue is full %u", teams_registered_so_far);
					Menu->AddItem( 0, tstr, 2 );
				}
				else if( isleader )
				{
					sprintf( tstr,"Queue for %s Event as group(%u/%u)", selected_map->map_name_menu,teams_registered_so_far,selected_map->max_teams_per_event);
					Menu->AddMenuItem( 0, tstr,0,0,"Input your unique team name to join/create(use Accept button not Enter)",0,1,MENU_ID_CREATE_JOIN_TEAM_AS_GROUP);
				}
				else 
				{
					sprintf( tstr,"Queue for %s Event to a group(%u/%u)", selected_map->map_name_menu,teams_registered_so_far,selected_map->max_teams_per_event);
					Menu->AddMenuItem( 0, tstr,0,0,"Input your unique team name to join/create(use Accept button not Enter)",0,1,MENU_ID_CREATE_JOIN_TEAM_AS_PLAYER);
				}
			}
		}
		// leaving so soon ?
		Menu->AddItem( 0, "Leave event queue",100);
		//special menu for GMs
		if( Plr->GetSession() && Plr->GetSession()->HasPermissions() )
		{
			Menu->AddItem( 0, "(GM)Pick a MAP for next event",671);
			if( eventi->is_spawned == true )
			{
				if( eventi->status == TS_NOT_STARTED )
					Menu->AddItem( 0, "(GM)Start Maze Event",660);
				Menu->AddItem( 0, "(GM)Stop Event",672);
			}
		}
		//dev commands
		if( Plr->GetSession() && Plr->GetSession()->HasGMPermissions() )
		{
			Menu->AddItem( 0, "(GM-DEV)Reload Map Files",673);
		}
		Menu->SendTo(Plr);
	}
};

void TSNPCGOSSIP::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
	if( AutoSend )
		send_MainPage(pObject,Plr);
}

bool  RemovePlayerFromTournamentQueue( Event_Supervisor_AI *organizer, Selected_Event_info *eventi, Player *plr )
{
	if( eventi == NULL )
		return false;
	uint64 guid_to_remove = plr->GetGUID();
	for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
		if( eventi->teams_registered[ j ].is_used != 0 )
			for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
				if( eventi->teams_registered[ j ].player_guids[ k ] == guid_to_remove )
				{
					eventi->teams_registered[ j ].player_guids[ k ] = 0;
					char textbuff[500];
					sprintf(textbuff,"Player %s left team %s and the event",plr->GetName(),eventi->teams_registered[ j ].name);
					organizer->GetUnit()->SendChatMessage(CHAT_MSG_MONSTER_SAY,0,textbuff);
					organizer->BroadcastToTournamentSafe( textbuff );
					return true;
				}
	return false;
}

bool QueuePlayerForTournament( Event_Supervisor_AI *organizer, Maze_Map_Info *selected_map, Selected_Event_info *eventi, Player *plr, const char *team_name)
{
	if( eventi == NULL || selected_map == NULL )
	{
		plr->BroadcastMessage(" There is no event selected. " );
		return false;
	}
	if( eventi->status != TS_NOT_STARTED )
	{
		plr->BroadcastMessage(" Event Started. Wait to finish " );
		return false ;
	}
	if( team_name == NULL || strlen( team_name ) == 0 || strlen( team_name ) >= MAX_EVENT_TEAM_NAME_LEN )
	{
		plr->BroadcastMessage(" Team name is inapropriate to register. Min len 1, max len %u ",MAX_EVENT_TEAM_NAME_LEN);
		return false;
	}

//	UpdateMutex.Acquire();
	int32 team_size_changed = 0;
	//make sure he is not already registered
	team_size_changed -= RemovePlayerFromTournamentQueue( organizer, eventi, plr );

	bool needs_team_create=true;
	//try to see if we find this team
	for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
		if( eventi->teams_registered[ j ].is_used != 0 
			&& strstr( team_name,eventi->teams_registered[ j ].name ) == team_name )
		{
			//check if team has open positions
			uint32 found_players = 0, open_slot;
			for(uint32 i = 0;i< selected_map->max_players_in_team; i++)
				if( eventi->teams_registered[ j ].player_guids[ i ] != 0 )
					found_players++;
				else 
					open_slot = i;
			if( found_players >= selected_map->max_players_in_team )
			{
				plr->BroadcastMessage(" Team with name '%s' is full ",team_name );
				needs_team_create =  false;
				break;
			}
			eventi->teams_registered[ j ].player_guids[ open_slot ] = plr->GetGUID();
			team_size_changed++;
			if( found_players == 0 )
				eventi->teams_registered[ j ].leader_guid = plr->GetGUID();
			plr->BroadcastMessage(" Added you to event queue inside team '%s'",team_name);
			char textbuff[500];
			sprintf(textbuff,"Player %s joined team %s and joined the event",plr->GetName(),team_name);
			organizer->GetUnit()->SendChatMessage(CHAT_MSG_MONSTER_SAY,0,textbuff);
			organizer->BroadcastToTournamentSafe( textbuff );
			needs_team_create = false;
			break;
		}
	if( needs_team_create == true )
	{
		uint32 registered_teams = 0;
		//check if more teams can join this event
		uint32 slot = MAX_TEAMS_PER_EVENT;
		for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
			if( eventi->teams_registered[ j ].is_used != 0 )
				registered_teams++;
		if( registered_teams >= selected_map->max_teams_per_event )
		{
			plr->BroadcastMessage(" This event is full. Wait until it is finished ");
			needs_team_create = false;
		}
	}
	if( needs_team_create == true )
	{
		//add a new team to our list
		uint32 slot = MAX_TEAMS_PER_EVENT;
		for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
			if( eventi->teams_registered[ j ].is_used == 0 )
			{
				slot = j;
				break;
			}
		if( slot < MAX_TEAMS_PER_EVENT )
		{
			Team_Info *freespot = &eventi->teams_registered[ slot ];
//			freespot->id = cur_team_id++;
			freespot->leader_guid = plr->GetGUID();
//			freespot->matches_won_total = 0;
//			freespot->matches_played_total = 0;
			strcpy( freespot->name, team_name );
			freespot->player_guids[ 0 ] = plr->GetGUID();
			freespot->is_used = 1;
			plr->BroadcastMessage(" Created new event team with name '%s'",team_name);
			team_size_changed++;

			char textbuff[500];
			sprintf(textbuff,"Player %s created team %s and joined the event",plr->GetName(),team_name);
			organizer->GetUnit()->SendChatMessage(CHAT_MSG_MONSTER_SAY,0,textbuff);
			organizer->BroadcastToTournamentSafe( textbuff );
		}
		else
		{
			plr->BroadcastMessage(" This event is full. Wait until it is finished ");
		}
	}
//	UpdateMutex.Release();
	return (team_size_changed>0);
}

void TSNPCGOSSIP::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
		return;
	//check if we are an event supervisor for real (ofc we are)
	if( pCreature->GetExtension( SUPERVISOR_EXTENSION_NAME ) == NULL )
		return;	//wow, lies !
	Event_Supervisor_AI *new_AI = (Event_Supervisor_AI *)pCreature->GetAIInterface();
	Maze_Map_Info *selected_map = new_AI->Event_map;
	Selected_Event_info *eventi = &new_AI->Event;

	
	switch(IntId)
    {
		// info menu
	    case 1:						send_MainPage(pObject,Plr);										break;
	    case MENU_ID_STATUS_PAGE:	send_StatusPage(selected_map,eventi,pObject,Plr);				break;
		//one of the tournament types
		case MENU_ID_CREATE_JOIN_TEAM_AS_PLAYER:		
			QueuePlayerForTournament( new_AI, selected_map, eventi, Plr, Code );
			Plr->CloseGossip();
			break;
		//one of the tournament types as group leader
		case MENU_ID_CREATE_JOIN_TEAM_AS_GROUP:		
			if( Plr->IsGroupLeader() == false || Plr->GetGroup() == NULL )
				Plr->BroadcastMessage("You are not a group leader");
			else
			{
				if( selected_map == NULL )
					return;
				//button pusher is leader and has group
				QueuePlayerForTournament( new_AI, selected_map, eventi, Plr, Code );
				Group * group = Plr->GetGroup(); 
				uint32 maxcount = selected_map->max_players_in_team - 1;
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
							if( QueuePlayerForTournament( new_AI, selected_map, eventi, (*itr)->m_loggedInPlayer, Code ) )
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
//			UpdateMutex.Acquire();
			if( RemovePlayerFromTournamentQueue( new_AI, eventi, Plr ) )
				Plr->BroadcastMessage("You have been removed from the Event Queue");
			else
				Plr->BroadcastMessage("Could not find you in any of Event Queues");
//			UpdateMutex.Release();
			Plr->CloseGossip();
			break;
		//gm commands
		case 660:
			new_AI->debug_commands = SUPERVIZOR_COMMAND_FORCESTART;
			Plr->CloseGossip();
			break;
		case 671:	//choose map 1 from map list
			{
				GossipMenu *Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
				char tstr[500];
				uint32 map_found = 0;
				for(uint32 i=0;i<MAX_MAPS_CACHED;i++)
					if( new_AI->maps[i].map_size_x != 0 )
					{
						sprintf( tstr,"%s", new_AI->maps[i].map_name_menu );
						Menu->AddItem( 0, tstr, 700+i);
						map_found++;
					}
				if( map_found == 0 )
					Menu->AddItem( 0, "No map files found", MENU_ID_STATUS_PAGE);
				Menu->AddItem( 0, "Back", MENU_ID_STATUS_PAGE);
				Menu->SendTo(Plr);
			}break;
		case 672:	//stop current event
			{
				//kick out existing players from current event
				if( selected_map )
				{
					for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
					{
						eventi->teams_registered[ j ].is_used = 0;
						for(uint32 i = 0;i< selected_map->max_players_in_team; i++)
							eventi->teams_registered[ j ].player_guids[ i ] = 0;
					}
				}
				//now change map
				new_AI->DespawnEventMap();
				Plr->BroadcastMessage("Kicked all old queued people from old event");
				Plr->CloseGossip();
			}break;
		case 673:	//reload maps from server dir
			{
				//kick out existing players from current event
				if( selected_map )
				{
					for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
					{
						eventi->teams_registered[ j ].is_used = 0;
						for(uint32 i = 0;i< selected_map->max_players_in_team; i++)
							eventi->teams_registered[ j ].player_guids[ i ] = 0;
					}
				}
				//now change map
				new_AI->ReloadMaps();
				Plr->BroadcastMessage("Reloaded maps in case you changed their content. Needs event restart");
				Plr->CloseGossip();
			}break;
		default:	
		{
			if( IntId >= 700 && IntId <= 700 + MAX_MAPS_CACHED )
			{
				//kick out existing players from current event
				new_AI->ResetEventForNew();
				//now change map
				new_AI->Event_map = &new_AI->maps[ IntId - 700 ];
				Plr->BroadcastMessage("Kicked all old queued people from old event");
				Plr->BroadcastMessage("Picked map with name %s for next event",new_AI->Event_map->map_name_menu);
				new_AI->SpawnEventMap();
				Plr->CloseGossip();
			}
			send_MainPage(pObject,Plr);							
		}break;
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
