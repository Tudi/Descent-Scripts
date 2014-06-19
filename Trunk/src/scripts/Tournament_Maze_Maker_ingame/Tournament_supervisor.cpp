#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"
#include "Tournament_supervisor.h"


void Event_Supervisor_AI::Init(Unit *un, AIType at, MovementType mt)
{
	AIInterface::Init(un,at,mt);//run the original init and we add our custom stuff too
	forced_reset_next_stamp = GetTickCount();
	Event_map = NULL;
	debug_commands = 0;
	memset( &Event, 0, sizeof(Event) );
	memset( &maps, 0, sizeof(maps) );
	global_update_next_stamp = 0;
}

void Event_Supervisor_AI::Update(uint32 p_time)
{
/*	//method to pass commands to tournament via any supervizor
	debug_commands = m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH );

	//make him normal 
	m_Unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, SUPERVIZOR_MAX_HEALTH );*/

	//see if we need to turn anyone into a spectator

	//update spam will be filtered inside function
	UpdateEventProgress( );

	//check for winners(or weeeners ? )
/*	for( set<Object*>::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
	{
		if( !(*itr) || !(*itr)->IsInWorld() )
			continue;
		if( (*itr)->IsPlayer() )
		{
		}
	}*/
}


//function is called from a threadsafe enviroment
//Note this function should be only called from PickNext2TeamsToFight() or UpdateTournaments() since it relies on local data
void Event_Supervisor_AI::BroadcastToTournament( const char *what )
{
	for(uint32 k=0;k<MAX_TEAMS_PER_EVENT; k++ )
		if( Event.teams_registered[ k ].is_used != 0 )
			for(uint32 i=0;i<MAX_PLAYERS_PER_TEAM; i++ )
				if( Event.teams_registered[ k ].players_for_local_acces[ i ] )
					Event.teams_registered[ k ].players_for_local_acces[i]->BroadcastMessage( what );
}

void Event_Supervisor_AI::BroadcastToTournamentSafe( const char *what )
{
	for(uint32 k=0;k<MAX_TEAMS_PER_EVENT; k++ )
		if( Event.teams_registered[ k ].is_used != 0 )
			for(uint32 i=0;i<MAX_PLAYERS_PER_TEAM; i++ )
				if( Event.teams_registered[ k ].player_guids[ i ] )
				{
					//check if player is still ingame
					Player *p = objmgr.GetPlayer( Event.teams_registered[ k ].player_guids[ i ] );
					if( p == NULL )
						continue;
					p->BroadcastMessage( what );
				}
}

//called only from a safe environment
void Event_Supervisor_AI::AssignTeamStartPossitions()
{
	//start positions
	uint32 next_free = 0;
	for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
		if( Event.teams_registered[ j ].is_used != 0 )
		{
			Event.teams_registered[ j ].start_x = Event_map->team_start_x[next_free];
			Event.teams_registered[ j ].start_y = Event_map->team_start_y[next_free];
			next_free++;
		}
}

void Event_Supervisor_AI::SendGoalPositionToPlayers()
{
	for(uint32 k=0;k<MAX_TEAMS_PER_EVENT; k++ )
		if( Event.teams_registered[ k ].is_used != 0 )
			for(uint32 i=0;i<MAX_PLAYERS_PER_TEAM; i++ )
				if( Event.teams_registered[ k ].players_for_local_acces[ i ] )
					Event.teams_registered[ k ].players_for_local_acces[i]->Gossip_SendPOI( Event_map->goal_x,Event_map->goal_y, 6, 6, 0, "Maze event goal" );
}

//crossing fingers that this function will always finish before 1 update cycle or lag might build up
void Event_Supervisor_AI::UpdateEventProgress( )
{
	//mutex to not update multi thrreaded way
//	UpdateMutex.Acquire();
	if( GetTickCount() < global_update_next_stamp || Event_map == NULL ) 
	{
//		UpdateMutex.Release();
		return;
	}
	global_update_next_stamp = GetTickCount() + SUPERVISOR_UPDATE_INTERVAL;

	uint32 local_gm_command = debug_commands;
	debug_commands = SUPERVIZOR_COMMAND_NONE;
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//make a list that we will use internally of players while we update and check tournament statuses
	uint32	tick_count_now = GetTickCount();
	uint32 good_team_count;
	//first let's check what players are not usable anymore
	good_team_count = 0;
	for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
	{
		if( Event.teams_registered[ j ].is_used != 0 )
		{
			uint32 player_counter = 0;
			for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
				if( Event.teams_registered[ j ].player_guids[ k ] )
				{
					//check if player is still ingame
					Player *p = objmgr.GetPlayer( Event.teams_registered[ j ].player_guids[ k ] );
					if( p == NULL )
					{
						Event.teams_registered[ j ].player_guids[ k ] = 0;
						Event.teams_registered[ j ].players_for_local_acces[ k ] = NULL;
						continue;
					}
					if( p->m_bg != NULL || ( Event.status == TS_WAIT_FOR_FIGHT && p->GetMapMgr() && ( p->GetMapId() != m_Unit->GetMapId() || IsInrangeSQ( p->GetPositionX(), p->GetPositionY(), Event_map->center_x, Event_map->center_y, Event_map->map_size_x*Event_map->map_size_y*(Event_map->GO_size+1)*(Event_map->GO_size+1) ) == false ) )
						)
					{
						Event.teams_registered[ j ].player_guids[ k ] = 0;
						Event.teams_registered[ j ].players_for_local_acces[ k ] = NULL;
						if( p )
							p->BroadcastMessage("You have been removed from the Event Queue");
						continue;
					}
					//we monitor contribution so we may reward accordingly
					Event.teams_registered[ j ].player_dmg_contri[ k ] = p->m_dmg_made_since_login; 
					Event.teams_registered[ j ].player_heal_contri[ k ] = p->m_heal_made_since_login; 
					player_counter++;
					Event.teams_registered[ j ].players_for_local_acces[ k ] = p;
				}
				else
					Event.teams_registered[ j ].players_for_local_acces[ k ] = NULL;

			//seems like a team we could use to start tournament
			if( player_counter != 0 )
				good_team_count++;
			//this team left the game so we can simply remove them from our list
			else
			{
				Event.teams_registered[ j ].is_used = 0 ;
				for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
				{
					Event.teams_registered[ j ].player_guids[ k ] = 0;
					Event.teams_registered[ j ].players_for_local_acces[ k ] = NULL;
				}
			}
		}
	}
	if( Event.status == TS_NOT_STARTED )
		forced_reset_next_stamp = tick_count_now + EVENT_WAIT_FORCED_RESET;
	else if( forced_reset_next_stamp < tick_count_now )
		Event.status = TS_NOT_STARTED;
	else if( good_team_count == 0 && Event.status == TS_WAIT_FOR_FIGHT )
	{
		DespawnEventMap();
		Event.status = TS_NOT_STARTED;
	}
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//now check if we could start any of the tournaments
	if( ( good_team_count >= Event_map->max_teams_per_event || local_gm_command == SUPERVIZOR_COMMAND_FORCESTART ) && Event.status == TS_NOT_STARTED )
	{
		Event.next_state_change_stamp = tick_count_now + WAIT_FOR_LAST_TEAM_TO_JOIN;
		Event.status = TS_TELEPORT_PLAYERS;
		BroadcastToTournament( "map info :");
		BroadcastToTournament( Event_map->map_info_text );
		BroadcastToTournament( "Event is preparring to start. You should get teleported.");
		GetUnit()->SendChatMessage(CHAT_MSG_MONSTER_SAY,0,"Event is preparring to start. Players are geting teleported.");
		AssignTeamStartPossitions();
	}
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//keep trying to teleport players to arena. This does not depend on time but expires after a while
	if( Event.status == TS_TELEPORT_PLAYERS )
	{
		//try to port all players to our tournament place that are no yet in range
		uint32 managed_to_teleport_all = 1;
		for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
		{
			Team_Info *team = &Event.teams_registered[ j ];
			for( uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
				if( team->players_for_local_acces[ k ] )
				{
					Player * chr = team->players_for_local_acces[ k ];
					if( chr->GetMapMgr() &&	//when he is getting teleported he will not have a mapmanager
						( chr->GetMapId() != m_Unit->GetMapId()
							|| IsInrangeSQ( chr->GetPositionX(), chr->GetPositionY(), team->start_x, team->start_y, MAX(Event_map->GO_size*Event_map->GO_size,9) ) == false ) )
					{
						if( !sEventMgr.HasEvent(chr, EVENT_PLAYER_TELEPORT) )
							sEventMgr.AddEvent(chr, &Player::EventTeleport, m_Unit->GetMapId(), team->start_x, team->start_y, m_Unit->GetPositionZ()+LIFT_PLAYER_WHEN_TELEPORT, EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
						managed_to_teleport_all = 0;
					}
//					if( chr->GetMapId() == m_Unit->GetMapId()
//						&& IsInrangeSQ( chr->GetPositionX(), chr->GetPositionY(), team->start_x, team->start_y, MAX(Event_map->GO_size*Event_map->GO_size,9) ) == true 
//						&& (chr->m_special_state & UNIT_STATE_ROOT) == 0 )	//avoid players having an advantage just because some PC loads faster then other
//						chr->Root();

				}
		}
		//in case all players are in place or teleport retry count is exhausted then we move on to next step
		if( managed_to_teleport_all == 1 || tick_count_now >= Event.next_state_change_stamp )
		{
			Event.next_state_change_stamp = tick_count_now + WAIT_FOR_LAST_TEAM_TO_JOIN;	//this is required so the loading bar will finish for all players
			Event.status = TS_TELEPORT_WAIT_PLAYERS;
		}
	}
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//if we are in the process of changing maps then we wait a bit
	if( Event.status == TS_TELEPORT_WAIT_PLAYERS && tick_count_now >= Event.next_state_change_stamp )
	{
		//set next state
		Event.status = TS_PREPARE_PLAYERS;
		Event.next_state_change_stamp = tick_count_now + PLAYER_PREBUFF_WAIT;
		//cleanup players so they are tidy :P
		for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
		{
			Team_Info *team = &Event.teams_registered[ j ];
			for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
				if( team->players_for_local_acces[ k ] )
				{
					Player * chr = team->players_for_local_acces[ k ];

					//resurrect the dead
					if( chr->IsDead() )
						chr->ResurrectPlayer();

					//remove all buffs except static ones
//					for(uint32 buffitr=0;buffitr<MAX_AURAS;buffitr++)
					for(uint32 buffitr=MAX_POSITIVE_AURAS;buffitr<MAX_AURAS;buffitr++)
						if( chr->m_auras[ buffitr ] )
							chr->m_auras[ buffitr ]->Remove();

					//set max health
					chr->SetUInt32Value(UNIT_FIELD_HEALTH, chr->GetUInt32Value(UNIT_FIELD_MAXHEALTH) );

					//reset energy
					for(uint32 i=0;i<POWER_TYPE_COUNT;i++)
						chr->SetPower(i, chr->GetMaxPower( i) );

					//set FFA and PVP - need it for cross faction buffs
					if( Event_map->FFA_enabled )
					{
						chr->SetPvPFlag();
						chr->SetFFAPvPFlag();
						chr->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
						chr->SetByteFlag(UNIT_FIELD_BYTES_2,1,UNIT_BYTE2_FLAG_PVP);
					}
					else
					{
						chr->RemovePvPFlag();
						chr->RemoveFFAPvPFlag();
						chr->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
						chr->RemoveByteFlag(UNIT_FIELD_BYTES_2,1,UNIT_BYTE2_FLAG_PVP);
					}

					//clear cooldowns
					chr->ResetAllCooldowns();

					//tell him what is happening
//					chr->BroadcastMessage("Prepare for event. You have %u seconds",PLAYER_PREBUFF_WAIT/1000,PLAYER_PREBUFF_WAIT/1000);

					//cast our buffs on the players
					for(uint32 bi=0;bi<MAX_STARTUP_BUFFS;bi++)
						if( Event_map->start_buffs[ bi ] )
						{
							SpellEntry *spellInfo = dbcSpell.LookupEntry( Event_map->start_buffs[ bi ] );
							Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
							spell->Init(chr, spellInfo ,true, NULL);
							SpellCastTargets targets( chr->GetGUID() );
							spell->forced_duration = 30 * 60 * 1000;
							spell->prepare(&targets);
						}

					//root players so they do not start fighting each others
//					if( (chr->m_special_state & UNIT_STATE_ROOT)==0)
//						chr->Root();
				}
		}
	}
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//players are prepared so we can start fighting
	if( Event.status == TS_PREPARE_PLAYERS && tick_count_now >= Event.next_state_change_stamp )
	{
		SendGoalPositionToPlayers();
		//set next state
		Event.status = TS_PREPARE_COUNTDOWN_6;
//		Event.next_state_change_stamp = tick_count_now + 2000;
		//reset player status again
//		BroadcastToTournament( "Prepare to fight : 6" );
	}
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//players are prepared so we can start fighting
	if( Event.status == TS_PREPARE_COUNTDOWN_6 && tick_count_now >= Event.next_state_change_stamp )
	{
		//set next state
		Event.status = TS_PREPARE_COUNTDOWN_4;
//		Event.next_state_change_stamp = tick_count_now + 2000;
		//notify players
//		BroadcastToTournament( "Prepare to fight : 4" );
	}
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//players are prepared so we can start fighting
	if( Event.status == TS_PREPARE_COUNTDOWN_4 && tick_count_now >= Event.next_state_change_stamp )
	{
		//set next state
		Event.status = TS_PREPARE_COUNTDOWN_2;
//		Event.next_state_change_stamp = tick_count_now + SUPERVISOR_UPDATE_INTERVAL;
		//notify players
//		BroadcastToTournament( "Prepare to fight : 2" );
	}
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//players are prepared so we can start fighting
	if( Event.status == TS_PREPARE_COUNTDOWN_2 && tick_count_now >= Event.next_state_change_stamp )
	{
		//set next state
		Event.status = TS_WAIT_FOR_FIGHT;
		Event.next_state_change_stamp = tick_count_now + EVENT_WAIT_TEAM_MATCH;
		//notify players
		for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
		{
			Team_Info *team = &Event.teams_registered[ j ];
			for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
				if( team->players_for_local_acces[ k ] )
				{
					Player * chr = team->players_for_local_acces[ k ];	
					//tell him what is happening
					chr->BroadcastMessage("Event started(for real) !");
					//remove godmode
					chr->bInvincible = false;
				}
		}
		GetUnit()->SendChatMessage(CHAT_MSG_MONSTER_SAY,0,"Event started. Temporary gates are removed.");
		//unroot players so they do not start fighting each others
		//chr->Unroot();
		DespawnStartGates();
	}
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//in case fight is timouted. This should happen only as last resort
	if( Event.status == TS_WAIT_FOR_FIGHT && tick_count_now >= Event.next_state_change_stamp )
	{
		//What a lamer team. Should not happen. No idea what to do. Right now they both get disqualified
		BroadcastToTournament( "Tournament fight takes too long. Forced ending");
		GetUnit()->SendChatMessage(CHAT_MSG_MONSTER_SAY,0,"Tournament fight takes too long. Forced ending.");
	}
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//fight monitor. Should not depend on any timer
	if( Event.status == TS_WAIT_FOR_FIGHT )
	{
		//update gates = despawn if gate guardians died
		UpdateGates();
		//check if one of team players are all dead
		uint32 has_nondead_active_players = 0;
		uint32 has_winner_team = 0;
		for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
		{
			Team_Info *team = &Event.teams_registered[ j ];
			float mapsizesq=Event_map->map_size_x*Event_map->map_size_y*Event_map->GO_size*Event_map->GO_size; //this is not half size range cause F NPC can be anywhere !!
			for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
				if( team->players_for_local_acces[ k ] )
				{
					Player *chr = team->players_for_local_acces[ k ];
					if( chr->GetMapId() == m_Unit->GetMapId() 
						&& IsInrangeSQ( chr->GetPositionX(), chr->GetPositionY(), Event_map->center_x, Event_map->center_y, mapsizesq ) == true 
						&&  chr->isAlive() )
					{
						has_nondead_active_players = 1;
						//make sure he did not disable FFA
						if( Event_map->FFA_enabled )
						{
							chr->SetPvPFlag();
							chr->SetFFAPvPFlag();
							chr->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
							chr->SetByteFlag(UNIT_FIELD_BYTES_2,1,UNIT_BYTE2_FLAG_PVP);
						}
						else
						{
							chr->RemovePvPFlag();
							chr->RemoveFFAPvPFlag();
							chr->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
							chr->RemoveByteFlag(UNIT_FIELD_BYTES_2,1,UNIT_BYTE2_FLAG_PVP);
						}
						//check for winners
						if( IsInrangeSQ( chr->GetPositionX(), chr->GetPositionY(), Event_map->goal_x, Event_map->goal_y, 2*2 ) == true )
						{
							chr->BroadcastMessage("You are selected to be one of the winners");
							has_winner_team = j+1;
							break;
						}
					}
				}
		}
		if( has_nondead_active_players == 0 )
		{
			BroadcastToTournament("No more active players left for tournament. Forced ending it");
			GetUnit()->SendChatMessage(CHAT_MSG_MONSTER_SAY,0,"No more active players left for tournament. Forced ending it.");
			Event.status = TS_NOT_STARTED;
		}
		if( has_winner_team != 0 )
		{
			char tstr[3000];
			sprintf( tstr,"Weeeeener team is : %s",Event.teams_registered[ has_winner_team - 1 ].name );
			GetUnit()->SendChatMessage(CHAT_MSG_MONSTER_SAY,0, tstr);
			BroadcastToTournament( tstr );
			//award rewards
			HandleEventWin( has_winner_team - 1 );
			//maybe despawn map ?
			DespawnEventMap();
			Event.status = TS_NOT_STARTED;
			Event_map = NULL;
		}
	}
EXIT_FUNCTION_WITH_CLEANUP:
	//morron protection. Clear out local variables
	for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
		for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
			Event.teams_registered[ j ].players_for_local_acces[ k ] = NULL;
	
//	UpdateMutex.Release();
}

void Event_Supervisor_AI::HandleEventWin( uint32 team_id )
{
	//morron protection. Clear out local variables
		for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
			if( Event.teams_registered[ team_id ].players_for_local_acces[ k ] )
			{
				Player *p = Event.teams_registered[ team_id ].players_for_local_acces[ k ];
				//cast our buffs on the players
				for(uint32 bi=0;bi<MAX_REWARD_ITEM_IDS;bi++)
				{
					if( Event_map->reward_buffs[ bi ] )
					{
						SpellEntry *spellInfo = dbcSpell.LookupEntry( Event_map->reward_buffs[ bi ] );
						Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
						spell->Init(p, spellInfo ,true, NULL);
						SpellCastTargets targets( p->GetGUID() );
						spell->forced_duration = 2 * 60 * 60 * 1000;
						spell->prepare(&targets);
					}
					if( Event_map->reward_gold[ bi ] )
					{
						p->ModGold( Event_map->reward_gold[ bi ] );
						sStackWorldPacket( pkt, SMSG_LOOT_MONEY_NOTIFY, 5 );
						pkt << Event_map->reward_gold[ bi ];
						p->GetSession()->SendPacket(&pkt);
					}
					if( Event_map->reward_item_id[ bi ] )
						p->GetItemInterface()->AddItemById( Event_map->reward_item_id[ bi ], MAX(1,Event_map->reward_item_id_count[ bi ]), 0 );
				}
			}
	//flush the queue
	memset( Event.teams_registered, 0, sizeof( Event.teams_registered ) );
}

void Event_Supervisor_AI::RemoveEventStatesFromPlayers( )
{
	if( Event_map == NULL )
		return;
	for(uint32 j=0;j<MAX_TEAMS_PER_EVENT;j++)
	{
		if( Event.teams_registered[ j ].is_used != 0 )
		{
			uint32 player_counter = 0;
			for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
				if( Event.teams_registered[ j ].player_guids[ k ] )
				{
					//check if player is still ingame
					Player *p = objmgr.GetPlayer( Event.teams_registered[ j ].player_guids[ k ] );
					if( p == NULL )
					{
						Event.teams_registered[ j ].player_guids[ k ] = 0;
						Event.teams_registered[ j ].players_for_local_acces[ k ] = NULL;
						continue;
					}
					for(uint32 bi=0;bi<MAX_STARTUP_BUFFS;bi++)
						if( Event_map->start_buffs[ bi ] )
							p->RemoveAura( Event_map->start_buffs[ bi ], p->GetGUID() );
				}
		}
	}
}

void Event_Supervisor_AI::ResetEventForNew( )
{
	//reset special states - before unspawn !
	RemoveEventStatesFromPlayers( );
	//remove spawned stuff
	DespawnEventMap();
	//mark all team places as free to be ocupied by new match participants
	for(uint32 k=0;k<MAX_TEAMS_PER_EVENT; k++ )
	{
		Event.teams_registered[ k ].is_used = 0; //clear used up slot
		strcpy( Event.teams_registered[ k ].name,"");	//clear name
		//clear out team player slots
		Team_Info *team = &Event.teams_registered[ k ];
		for(uint32 i=0;i<MAX_PLAYERS_PER_TEAM; i++ )
		{
			team->players_for_local_acces[ i ] = 0;	//clear used up slot
			team->player_guids[ i ] = 0;
		}
	}
	Event.next_state_change_stamp = GetTickCount();
	Event.status = TS_NOT_STARTED;
}

//despawns gates that do not have some MOB near them
void Event_Supervisor_AI::UpdateGates()
{
	if( m_Unit->GetMapMgr() == NULL )
		return;
	for( uint32 y=0;y<Event_map->map_size_y;y++)
		for( uint32 x=0;x<Event_map->map_size_x;x++)
			if( Event_map->entry_map[x+y*Event_map->map_size_x] > 0 && Event_map->spawn_map[x+y*Event_map->map_size_x] )
			{
				uint32 ge = Event_map->entry_map[x+y*Event_map->map_size_x];
				int32 xv[2],yv[2];
				//over and below the gate spawns
				if( ge == Event_map->H_Gate_GO_entry )
				{
					xv[0] = xv[1] = 0;
					yv[0] = -1;
					yv[1] = 1;
				}
				//left and right of the gate
				else if( ge == Event_map->V_Gate_GO_entry )
				{
					yv[0] = yv[1] = 0;
					xv[0] = -1;
					xv[1] = 1;
				}
				else
					continue;
				//check the neighbour spawns and if 1 is dead then we can get despawned
				for(uint32 tv=0;tv<2;tv++)
				{
					int32 x1 = x+xv[tv];
					int32 y1 = y+yv[tv];
					if( y1<0 || y1>=Event_map->map_size_y || x1<0 || x1>=Event_map->map_size_x || (x1==y1 && x1==0) )
						continue;
					if( Event_map->entry_map[x1+y1*Event_map->map_size_x] >= 0 )
						continue;
					if( Event_map->spawn_map[x1+y1*Event_map->map_size_x] == 0 )//this mob is missing ? wtf ?
						continue;
					//try to get this mob from map, if he is missing or dead then we may despawn this gate
					Creature *c = m_Unit->GetMapMgr()->GetCreature( Event_map->spawn_map[x1+y1*Event_map->map_size_x] );
					if( c == NULL || c->isAlive() == false || c->GetUInt32Value( UNIT_FIELD_HEALTH ) == 0 )
					{
						GameObject *g = m_Unit->GetMapMgr()->GetGameObject( Event_map->spawn_map[x+y*Event_map->map_size_x] );
						if( g )
							g->Despawn( 0 );
						Event_map->spawn_map[x+y*Event_map->map_size_x] = 0;
						break;
					}
				}
			}
}

//despawns gates that do not have some MOB near them
void Event_Supervisor_AI::DespawnStartGates()
{
	if( m_Unit->GetMapMgr() == NULL )
		return;
	for( uint32 y=0;y<Event_map->map_size_y;y++)
		for( uint32 x=0;x<Event_map->map_size_x;x++)
			if( ( Event_map->entry_map[x+y*Event_map->map_size_x] == Event_map->H_Start_Gate_GO_entry || Event_map->entry_map[x+y*Event_map->map_size_x] == Event_map->V_Start_Gate_GO_entry )
				&& Event_map->spawn_map[x+y*Event_map->map_size_x] )
			{
				GameObject *g = m_Unit->GetMapMgr()->GetGameObject( Event_map->spawn_map[x+y*Event_map->map_size_x] );
				if( g )
					g->Despawn( 0 );
				Event_map->spawn_map[x+y*Event_map->map_size_x] = 0;
			}
}

//this NPC is spawned in the midle of the arena to do everything :P
//constructor
//this is the default script AI replace definition :P
class Event_supervisor : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Event_supervisor);
	Event_supervisor(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		//we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
		Event_Supervisor_AI *new_interface = new Event_Supervisor_AI;
		new_interface->Init(_unit,AITYPE_SOCIAL,MOVEMENTTYPE_NONE);
		_unit->ReplaceAIInterface( (AIInterface *) new_interface );

		//make us friendly
		_unit->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, SUPERVISOR_FACTION );
		_unit->_setFaction();

		//make us imune to every spell we know of :P
		for(uint32 i=0;i<7;i++)
			_unit->SchoolImmunityAntiEnemy[i] = 999;
		for(uint32 i=0;i<31;i++)
			_unit->MechanicsDispels[i] = 999;

		//make him an NPC who can talk
		_unit->SetUInt32Value( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP );

		//put us in an IDLE state ( funcky )
	//		_unit->SetUInt32Value(SUPERVISOR_VARIABLE_STATE_STORE,SUPERVISOR_HP_TO_IDLE);

		//make him impossible to select or attack by normal players
//		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, _unit->GetUInt32Value( UNIT_FIELD_FLAGS ) | SUPERVISOR_UNIT_FLAGS );

		//make him normal 
		_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, SUPERVIZOR_MAX_HEALTH );

		//since we cannot guess pointer casts we better make sure
		_unit->SetExtension( SUPERVISOR_EXTENSION_NAME, new_interface );

		//static objects get updated even if mapcell underneath them got unloaded. Crashy ?
		_unit->static_object = true;

		//try to open a few map files and add them to our list if we manage to load them 
		new_interface->CacheMaps();
	}
};

void SetupTournamentSupervisorIngame(ScriptMgr * mgr)
{
    mgr->register_creature_script(CREATURE_ENTRY_FOR_IGTS_SUPERVIZOR, &Event_supervisor::Create);

/*
replace into npc_gossip_textid values( 123462, 20050);
replace into npc_text (entry,text0_0) values (20050,"Maze Events can vary in types. The goal is always to reach a certain point on the map.Meantime you might be forced to PVP or kill creatures in the way or simply try to find the path to the exit before other teams. $B    Good luck and have fun.");

REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `type`, `male_displayid`) VALUES
(123462, 'Event Organizer', 'are you ready boy ?', 'PVP', 7, 12679);

REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`,  `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`) VALUES
(123462, 96, 96, 35, 100000, 100000, 1.25, 0, 1000, 0, 100, 200, 0, 0, 0, 360000000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 1.8, 0.5, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);

UPDATE worldmap_info SET viewingDistance= 500 WHERE entry=169;
#delete from creature_spawns where entry=123462;
#REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123462,169,3068,941,24.5,35);
#REPLACE INTO creature_staticspawns (entry,map,x,y,z,factionid)VALUES(123462,169,3068,941,24.5,35);
#REPLACE INTO creature_staticspawns (entry,map,position_x,position_y,position_z,faction)VALUES(123462,169,3068,941,24.5,35);

insert into gameobject_names (entry,type,DisplayID,name,scale) values ('555','14','7915','Maze H','1');
insert into gameobject_names (entry,type,DisplayID,name,scale) values ('556','14','7915','Maze V','1');
insert into gameobject_names (entry,type,DisplayID,name,scale) values ('557','14','7915','Maze GH','1');
insert into gameobject_names (entry,type,DisplayID,name,scale) values ('558','14','7915','Maze GV','1');
insert into gameobject_names (entry,type,DisplayID,name,scale) values ('559','14','7915','Maze SGH','1');
insert into gameobject_names (entry,type,DisplayID,name,scale) values ('560','14','7915','Maze SGV','1');

insert into creature_names (entry,name,male_displayid,type) values ( 123464,"Damage absorber",26589,9);
insert into creature_proto (entry,minlevel,maxlevel,faction,minhealth,maxhealth,mana,attacktime,mindamage,maxdamage,scale,money) values ( 123464,80,80,17,5000000,5000000,5000000,2000,1,2,1,1);
insert into creature_names (entry,name,male_displayid,type) values ( 123465,"Damage absorber small",26589,9);
insert into creature_proto (entry,minlevel,maxlevel,faction,minhealth,maxhealth,mana,attacktime,mindamage,maxdamage,scale,money) values ( 123465,80,80,17,50000,50000,50000,2000,1,2,1,1);
insert into creature_names (entry,name,male_displayid,type) values ( 123466,"Damage absorber large",26589,9);
insert into creature_proto (entry,minlevel,maxlevel,faction,minhealth,maxhealth,mana,attacktime,mindamage,maxdamage,scale,money) values ( 123466,80,80,17,20000000,20000000,20000000,2000,1,2,1,1);
*/
}
