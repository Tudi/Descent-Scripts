#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"

uint32				cur_tournament_id = 1;		//guid
uint32				cur_team_id	= 1;			//guid
Tournament_Info		Tournaments[ TOURNAMENT_TYPE_COUNT ];
uint32				global_update_next_stamp = 0;		//do not spam updates in case there are multiple supervizor spawns
Mutex				UpdateMutex;
uint32				teamsize_limitations[ TOURNAMENT_TYPE_COUNT ] = { 2, 3, 4, 5 };
uint32				debug_commands = SUPERVIZOR_COMMAND_NONE;

bool IsInrangeSQ( Object *t1, Object *t2,float sq_r)
{
   if( !t1 || !t2 )
	  return false;

   float distx=t1->GetPositionX()-t2->GetPositionX();
   float disty=t1->GetPositionY()-t2->GetPositionY();
   float dist = distx*distx + disty*disty; 
   return dist <= sq_r;
}

bool IsInrangeSQ( float x1, float y1,float x2, float y2, float sq_r)
{
   float distx=x1-x2;
   float disty=y1-y2;
   float dist = distx*distx + disty*disty; 
   return dist <= sq_r;
}

bool	IsUnitInsideBattleRing(uint32 TournamentType, uint32 map, float x, float y)
{
	if( 
		TournamentType < TOURNAMENT_TYPE_COUNT
		&& Tournaments[ TournamentType ].map_id == map 
		&& IsInrangeSQ(x,y,Tournaments[ TournamentType ].center_x,Tournaments[ TournamentType ].center_y,Tournaments[ TournamentType ].arena_range_sq)
		)
			return true;
	return false;
}

bool	IsUnitInsideAnyBattleRing(uint32 map, float x, float y)
{
	for(uint32 i=0;i<TOURNAMENT_TYPE_COUNT;i++)
		if( IsUnitInsideBattleRing( i, map, x, y) == true )
			return true;
	return false;
}

bool	IsPlayerSpectator( Player *plr )
{
	if( IsUnitInsideAnyBattleRing( plr->GetMapId(), plr->GetPositionX(), plr->GetPositionY() ) )
	{
		//check if he is participant in any of the active tournaments
		uint64 PGUID = plr->GetGUID();
		for(uint32 i=0;i<TOURNAMENT_TYPE_COUNT;i++)
			//check if insight fighting team
			if( Tournaments[ i ].status >= TS_TELEPORT_PLAYERS && Tournaments[ i ].status <= TS_WAIT_FOR_FIGHT )
			{
				uint32 T1 = Tournaments[ i ].fighting_teams[ 0 ];
				uint32 T2 = Tournaments[ i ].fighting_teams[ 1 ];
				for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
					if( Tournaments[ i ].teams_registered[ T1 ].player_guids[ k ] == PGUID 
						||
						Tournaments[ i ].teams_registered[ T2 ].player_guids[ k ] == PGUID 
						)
						return false;
			}
			//maybe tournament did not start yet
			else if( (Tournaments[ i ].status < TS_TELEPORT_PLAYERS || Tournaments[ i ].status > TS_WAIT_FOR_FIGHT) && IsUnitInsideBattleRing( i, plr->GetMapId(), plr->GetPositionX(), plr->GetPositionY() ) == true )
				return false;
		return true;
	}
	return false;
}

uint32	GetTeamMemberCount(uint32 TournamentType,uint32 team)
{
	return 0;
}

//crossing fingers that this function will always finish before 1 update cycle or lag might build up
void	UpdateTournaments( )
{
	//mutex to not update multi thrreaded way
	UpdateMutex.Acquire();
	if( GetTickCount() < global_update_next_stamp ) 
	{
		UpdateMutex.Release();
		return;
	}
	global_update_next_stamp = GetTickCount() + SUPERVISOR_UPDATE_INTERVAL;

	uint32 local_gm_command = debug_commands;
	debug_commands = SUPERVIZOR_COMMAND_NONE;
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//make a list that we will use internally of players while we update and check tournament statuses
	uint32 good_team_count[TOURNAMENT_TYPE_COUNT];
	uint32	tick_count_now = GetTickCount();
	//first let's check what players are not usable anymore
	for(uint32 i=0;i<TOURNAMENT_TYPE_COUNT;i++)
	{
		good_team_count[i] = 0;
		uint32 MinLevel = 255;
		uint32 MaxLevel = 0;
		for(uint32 j=0;j<MAX_TEAMS_PER_TOURNAMENT;j++)
		{
			if( Tournaments[ i ].teams_registered[ j ].is_used != 0 )
			{
				uint32 player_counter = 0;
				for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
					if( Tournaments[ i ].teams_registered[ j ].player_guids[ k ] )
					{
						//check if player is still ingame
						Player *p = objmgr.GetPlayer( Tournaments[ i ].teams_registered[ j ].player_guids[ k ] );
						if( p != NULL && p->m_bg != NULL )
						{
							Tournaments[ i ].teams_registered[ j ].player_guids[ k ] = 0;
							Tournaments[ i ].teams_registered[ j ].players_for_local_acces[ k ] = NULL;
							if( p )
								p->BroadcastMessage("You have been removed from the Tournamen Queue");
							continue;
						}
						if( p )
						{
							if( MinLevel > p->getLevel() )
								MinLevel = p->getLevel();
							if( MaxLevel < p->getLevel() )
								MaxLevel = p->getLevel();
							//we monitor contribution so we may reward accordingly
							Tournaments[ i ].teams_registered[ j ].player_dmg_contri[ k ] = p->m_dmg_made_since_login; 
							Tournaments[ i ].teams_registered[ j ].player_heal_contri[ k ] = p->m_heal_made_since_login; 
							player_counter++;
						}
						Tournaments[ i ].teams_registered[ j ].players_for_local_acces[ k ] = p;
					}
					else
						Tournaments[ i ].teams_registered[ j ].players_for_local_acces[ k ] = NULL;

				//seems like a team we could use to start tournament
				if( player_counter != 0 )
					good_team_count[i]++;
				//this team left the game so we can simply remove them from our list
				else
				{
					Tournaments[ i ].teams_registered[ j ].is_used = 0 ;
					for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
					{
						Tournaments[ i ].teams_registered[ j ].player_guids[ k ] = 0;
						Tournaments[ i ].teams_registered[ j ].players_for_local_acces[ k ] = NULL;
					}
				}
			}
		}
		if( MinLevel == 255 )
			MinLevel = 0;
		Tournaments[ i ].MinLevel = MinLevel;
		Tournaments[ i ].MaxLevel = MaxLevel;
	}

	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//now check if we could start any of the tournaments
	for(uint32 i=0;i<TOURNAMENT_TYPE_COUNT;i++)
		if( good_team_count[i] >= MAX_TEAMS_PER_TOURNAMENT || local_gm_command == SUPERVIZOR_COMMAND_FORCESTART )
		{
			if( ( Tournaments[ i ].status == TS_NOT_STARTED || Tournaments[ i ].status == TS_WAITING_LAST_TEAM_JOIN ) && local_gm_command == SUPERVIZOR_COMMAND_FORCESTART )
			{
				Tournaments[ i ].next_state_change_stamp = tick_count_now;
				Tournaments[ i ].status = TS_WAITING_LAST_TEAM_JOIN_FORCED;
				Tournaments[ i ].ladder_step = 0;	//we are searching for teams with 0 wins
#ifdef TOURNAMENT_TYPE_WEAKEST_VS_WEAKEST
				BroadcastToTournament( i, "Enough players joined the tournament queue. You will get teleported when your fight starts. All teams fight all other teams");
#endif
#ifdef TOURNAMENT_TYPE_LADDER
				BroadcastToTournament( i, "Enough players joined the tournament queue. You will get teleported when your fight starts. If any team looses then it is eliminated");
#endif
			}
			//we wait some time to check if they are all ready
			else if( Tournaments[ i ].status == TS_NOT_STARTED )
			{
				Tournaments[ i ].next_state_change_stamp = tick_count_now + WAIT_FOR_LAST_TEAM_TO_JOIN;
				Tournaments[ i ].status = TS_WAITING_LAST_TEAM_JOIN;
				Tournaments[ i ].ladder_step = 0;	//we are searching for teams with 0 wins
#ifdef TOURNAMENT_TYPE_WEAKEST_VS_WEAKEST
				BroadcastToTournament( i, "Enough players joined the tournament queue. You will get teleported when your fight starts. All teams fight all other teams");
#endif
#ifdef TOURNAMENT_TYPE_LADDER
				BroadcastToTournament( i, "Enough players joined the tournament queue. You will get teleported when your fight starts. If any team looses then it is eliminated");
#endif

				//in case we want the whole tournament to watch. Note that this means 50 players if 5 vs 5
				//this step is optional
				Tournament_Info *tur = &Tournaments[ i ];
				for(uint32 j=0;j<MAX_TEAMS_PER_TOURNAMENT;j++)
				{
					Team_Info *team = &tur->teams_registered[ j ];
					for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
						if( team->players_for_local_acces[ k ] )
						{
							Player * chr = team->players_for_local_acces[ k ];
							if( chr->IsBeingTeleported() == false && sEventMgr.HasEvent(chr, EVENT_PLAYER_TELEPORT) == false )
								sEventMgr.AddEvent(chr, &Player::EventTeleport, tur->map_id, tur->center_x, tur->center_y, tur->center_z, EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
						}
				}
			}
		}
		//reset waiting if one of the teams left before we could even start the tournament
		else if( Tournaments[ i ].status == TS_WAITING_LAST_TEAM_JOIN )
		{
			Tournaments[ i ].status = TS_NOT_STARTED;
		}

	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//now check if we can start any of the tournaments
	for(uint32 i=0;i<TOURNAMENT_TYPE_COUNT;i++)
	{
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//we are over waiting for teams to build up. Move to next step
		if( ( Tournaments[ i ].status == TS_WAITING_LAST_TEAM_JOIN || Tournaments[ i ].status == TS_WAITING_LAST_TEAM_JOIN_FORCED )
			&& tick_count_now >= Tournaments[ i ].next_state_change_stamp )
		{
			//just pick 2 teams that will fight each other
			PickNext2TeamsToFight( i );
		}
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//keep trying to teleport players to arena. This does not depend on time but expires after a while
		if( Tournaments[ i ].status == TS_TELEPORT_PLAYERS )
		{
			//try to port all players to our tournament place that are no yet in range
			uint32 managed_to_teleport_all = 1;
			Tournament_Info *tur = &Tournaments[ i ];
			for(uint32 j=0;j<TEAMS_TO_FIGHT_AT_SAME_TIME;j++)
			{
				Team_Info *team = &tur->teams_registered[ tur->fighting_teams[ j ] ];
				static const int32 shift_x_placement[2]={-15,15};
				for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
					if( team->players_for_local_acces[ k ] )
					{
						Player * chr = team->players_for_local_acces[ k ];
						float intended_x = tur->center_x + shift_x_placement[ j ];
						float intended_y = tur->center_y - 2 + k;
						chr->bInvincible = true;
						if( chr->IsBeingTeleported() == true ) 
							managed_to_teleport_all = 0;
						else if( chr->GetMapId() != tur->map_id
							|| IsInrangeSQ( chr->GetPositionX(), chr->GetPositionY(), intended_x, intended_y, 5*5 ) == false )
						{
							if( !sEventMgr.HasEvent(chr, EVENT_PLAYER_TELEPORT) )
								sEventMgr.AddEvent(chr, &Player::EventTeleport, tur->map_id, intended_x, intended_y, tur->center_z, EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
							managed_to_teleport_all = 0;
						}
					}
			}
			//in case all players are in place or teleport retry count is exhausted then we move on to next step
			if( managed_to_teleport_all == 1 || tick_count_now >= Tournaments[ i ].next_state_change_stamp )
			{
				Tournaments[ i ].next_state_change_stamp = tick_count_now;
				Tournaments[ i ].status = TS_TELEPORT_WAIT_PLAYERS;
			}
		}
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//if we are in the process of changing maps then we wait a bit
		if( Tournaments[ i ].status == TS_TELEPORT_WAIT_PLAYERS && tick_count_now >= Tournaments[ i ].next_state_change_stamp )
		{
			//set next state
			Tournaments[ i ].status = TS_PREPARE_PLAYERS;
			Tournaments[ i ].next_state_change_stamp = tick_count_now + PLAYER_PREBUFF_WAIT;

			//broadcast to players that we are waiting for them to buff each others and prepare
//				m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, SUPERVISOR_ANNOUNCE_TXT_PLAYERS_BUFF);

			//now cleanup players so they are tidy :P
			Tournament_Info *tur = &Tournaments[ i ];
			for(uint32 j=0;j<TEAMS_TO_FIGHT_AT_SAME_TIME;j++)
			{
				Team_Info *team = &tur->teams_registered[ tur->fighting_teams[ j ] ];
				for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
					if( team->players_for_local_acces[ k ] )
					{
						Player * chr = team->players_for_local_acces[ k ];

						//resurrect the dead
						if( chr->IsDead() )
							chr->ResurrectPlayer();

						//remove all buffs except static ones
						for(uint32 buffitr=0;buffitr<MAX_AURAS;buffitr++)
							if( chr->m_auras[ buffitr ] )
								chr->m_auras[ buffitr ]->Remove();

						//set max health
						chr->SetUInt32Value(UNIT_FIELD_HEALTH, chr->GetUInt32Value(UNIT_FIELD_MAXHEALTH) );

						//reset energy
						chr->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_MANA, chr->GetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_MANA) );
						chr->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_RAGE, 0 );
						chr->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_FOCUS, 0 );
						chr->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_ENERGY, chr->GetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY) );
						chr->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_RUNIC, 0 );

						//set FFA and PVP - need it for cross faction buffs
						chr->SetPvPFlag();
						chr->SetFFAPvPFlag();

						//clear cooldowns
						chr->ResetAllCooldowns();

						//tell him what is happening
						chr->BroadcastMessage("Prepare to fight. You have %u seconds to buff team mates. ReGroup in case you need. At the end of the %u seconds your HP will be reseted",PLAYER_PREBUFF_WAIT/1000,PLAYER_PREBUFF_WAIT/1000);

						//root players so they do not start fighting each others
						chr->Root();
					}
			}
		}
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//players are prepared so we can start fighting
		if( Tournaments[ i ].status == TS_PREPARE_PLAYERS && tick_count_now >= Tournaments[ i ].next_state_change_stamp )
		{
			//set next state
			Tournaments[ i ].status = TS_PREPARE_COUNTDOWN_6;
			Tournaments[ i ].next_state_change_stamp = tick_count_now + 2000;
			//reset player status again
			Tournament_Info *tur = &Tournaments[ i ];
			for(uint32 j=0;j<TEAMS_TO_FIGHT_AT_SAME_TIME;j++)
				BroadcastToTournamentTeam( i, tur->fighting_teams[ j ], "Prepare to fight : 6" );
		}
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//players are prepared so we can start fighting
		if( Tournaments[ i ].status == TS_PREPARE_COUNTDOWN_6 && tick_count_now >= Tournaments[ i ].next_state_change_stamp )
		{
			//set next state
			Tournaments[ i ].status = TS_PREPARE_COUNTDOWN_4;
			Tournaments[ i ].next_state_change_stamp = tick_count_now + 2000;
			//notify players
			Tournament_Info *tur = &Tournaments[ i ];
			for(uint32 j=0;j<TEAMS_TO_FIGHT_AT_SAME_TIME;j++)
				BroadcastToTournamentTeam( i, tur->fighting_teams[ j ], "Prepare to fight : 4" );
		}
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//players are prepared so we can start fighting
		if( Tournaments[ i ].status == TS_PREPARE_COUNTDOWN_4 && tick_count_now >= Tournaments[ i ].next_state_change_stamp )
		{
			//set next state
			Tournaments[ i ].status = TS_PREPARE_COUNTDOWN_2;
			Tournaments[ i ].next_state_change_stamp = tick_count_now + SUPERVISOR_UPDATE_INTERVAL;
			//notify players
			Tournament_Info *tur = &Tournaments[ i ];
			for(uint32 j=0;j<TEAMS_TO_FIGHT_AT_SAME_TIME;j++)
				BroadcastToTournamentTeam( i, tur->fighting_teams[ j ], "Prepare to fight : 2" );
		}
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//players are prepared so we can start fighting
		if( Tournaments[ i ].status == TS_PREPARE_COUNTDOWN_2 && tick_count_now >= Tournaments[ i ].next_state_change_stamp )
		{
			//set next state
			Tournaments[ i ].status = TS_WAIT_FOR_FIGHT;
			Tournaments[ i ].next_state_change_stamp = tick_count_now + TOURNAMENT_WAIT_TEAM_MATCH;
			//notify players
			Tournament_Info *tur = &Tournaments[ i ];
			for(uint32 j=0;j<TEAMS_TO_FIGHT_AT_SAME_TIME;j++)
			{
				Team_Info *team = &tur->teams_registered[ tur->fighting_teams[ j ] ];
				for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
					if( team->players_for_local_acces[ k ] )
					{
						Player * chr = team->players_for_local_acces[ k ];

						//resurrect the dead. Fucking people just can't wait the buff timer
						if( chr->IsDead() )
							chr->ResurrectPlayer();

						//set max health
						chr->SetUInt32Value(UNIT_FIELD_HEALTH, chr->GetUInt32Value(UNIT_FIELD_MAXHEALTH) );

						//reset energy
						chr->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_MANA, chr->GetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_MANA) );
						chr->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_RAGE, 0 );
						chr->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_FOCUS, 0 );
						chr->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_ENERGY, chr->GetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY) );
						chr->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_RUNIC, 0 );

						//set FFA and PVP
						chr->SetPvPFlag();
						chr->SetFFAPvPFlag();
						chr->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
						chr->SetByteFlag(UNIT_FIELD_BYTES_2,1,UNIT_BYTE2_FLAG_PVP);

						//clear cooldowns
						chr->ResetAllCooldowns();
			
						//tell him what is happening
						chr->BroadcastMessage("Prepare to fight : fight !");

						//unroot players so they do not start fighting each others
						chr->Unroot();

						//remove godmode
						chr->bInvincible = false;
					}
			}
			//set sudden death counter
			Tournaments[ i ].sudden_death_stamp = tick_count_now + SUDDEN_DEATH_TIMER;
			Tournaments[ i ].sudden_death_counter = 1;
		}
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//in case fight is timouted. This should happen only as last resort
		if( Tournaments[ i ].status == TS_WAIT_FOR_FIGHT && tick_count_now >= Tournaments[ i ].next_state_change_stamp )
		{
			//What a lamer team. Should not happen. No idea what to do. Right now they both get disqualified
			BroadcastToTournament( i, "Tournament fight takes too long. Abuser protection is eliminating both teams");
			PickNext2TeamsToFight( i );
		}
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////
		//fight monitor. Should not depend on any timer
		if( Tournaments[ i ].status == TS_WAIT_FOR_FIGHT )
		{
			//check if one of team players are all dead
			uint32 all_team_players_dead[TEAMS_TO_FIGHT_AT_SAME_TIME];
			Tournament_Info *tur = &Tournaments[ i ];
			uint32 T1 = tur->fighting_teams[0];
			uint32 T2 = tur->fighting_teams[1];
			for(uint32 j=0;j<TEAMS_TO_FIGHT_AT_SAME_TIME;j++)
			{
				all_team_players_dead[j] = 1;
				Team_Info *team = &tur->teams_registered[ tur->fighting_teams[ j ] ];
				if( team->is_used != 0 )
					for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
						if( team->players_for_local_acces[ k ] )
						{
							Player * chr = team->players_for_local_acces[ k ];
							if( IsUnitInsideBattleRing( i, chr->GetMapId(), chr->GetPositionX(), chr->GetPositionY() ) == false )
							{
								chr->BroadcastMessage("You are outside the Tournament ring. Get back or get disqualified");
								chr->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
								chr->RemoveByteFlag(UNIT_FIELD_BYTES_2,1,UNIT_BYTE2_FLAG_PVP);
								continue;
							}
							//what about corpse ?
							if( chr->isAlive() == true 
								&& chr->IsDead() == false 
								)
							{
								//make sure due to revive they did not loose FFA flag
								chr->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
								chr->SetByteFlag(UNIT_FIELD_BYTES_2,1,UNIT_BYTE2_FLAG_PVP);
								//this match is not over yet
								all_team_players_dead[j] = 0;
								//actually let's make sure all players have FFA flag and notified if they left the zone
//								break;
							}
						}
			}
			if( all_team_players_dead[0] == all_team_players_dead[1] && all_team_players_dead[0] == 1 )
			{
				//match was a draw
				for(uint32 j=0;j<TEAMS_TO_FIGHT_AT_SAME_TIME;j++)
				{
					Team_Info *team = &tur->teams_registered[ tur->fighting_teams[ j ] ];
					for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
						if( team->players_for_local_acces[ k ] )
						{
							Player * chr = team->players_for_local_acces[ k ];
							chr->BroadcastMessage(" Match ended in a draw. (happens to offline teams also)");						
						}
					//let them both fight in the next match if team is indeed dead instead of quit
					if( team->is_used )
					{
						team->matches_won_total++;
						team->matches_played_total++;
					}
				}
				PickNext2TeamsToFight( i );
			}
			else if( all_team_players_dead[0] == 0 && all_team_players_dead[1] != 0 )
			{
				//team 1 is winner
				Tournaments[ i ].teams_registered[ T1 ].matches_won_total++;
				Tournaments[ i ].teams_registered[ T1 ].matches_played_total++;
				Tournaments[ i ].teams_registered[ T2 ].matches_played_total++;
				PickNext2TeamsToFight( i );
			}
			else if( all_team_players_dead[1] == 0 && all_team_players_dead[0] != 0 )
			{
				//team 2 is winner
				Tournaments[ i ].teams_registered[ T2 ].matches_won_total++;
				Tournaments[ i ].teams_registered[ T1 ].matches_played_total++;
				Tournaments[ i ].teams_registered[ T2 ].matches_played_total++;
				PickNext2TeamsToFight( i );
			}

			//sudden death is a parralel state to combat waiting and will try to kill players in time
			if( tick_count_now >= tur->sudden_death_stamp )
			{
				Tournament_Info *tur = &Tournaments[ i ];
				uint32 damage_to_deal = tur->sudden_death_counter * SUDDEN_DEATH_DAMAGE;
				tur->sudden_death_counter++;
				tur->sudden_death_stamp = tick_count_now + SUDDEN_DEATH_REPEAT_TIMER;
				for(uint32 j=0;j<TEAMS_TO_FIGHT_AT_SAME_TIME;j++)
				{
					Team_Info *team = &tur->teams_registered[ tur->fighting_teams[ j ] ];
					for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
						if( team->players_for_local_acces[ k ] )
						{
							Player * chr = team->players_for_local_acces[ k ];
							if( chr->IsInWorld() && chr->isAlive() )
//								chr->DealDamage( chr, damage_to_deal, SUDDEN_DEATH_SPELL_ID, true );
								chr->DealDamage( chr, damage_to_deal, 0, true );
						}
				}
			}
		}
	}
	//morron protection. Clear out local variables
	for(uint32 i=0;i<TOURNAMENT_TYPE_COUNT;i++)
		for(uint32 j=0;j<MAX_TEAMS_PER_TOURNAMENT;j++)
			for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
				Tournaments[ i ].teams_registered[ j ].players_for_local_acces[ k ] = NULL;
	
	UpdateMutex.Release();
}

//function is called from a threadsafe enviroment
//Note this function should be only called from PickNext2TeamsToFight() since it relies on local data
void	RemoveTournamentStatesFromPlayers( uint32 TournamentID )
{
	for(uint32 j=0;j<MAX_TEAMS_PER_TOURNAMENT;j++)
		for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
			if( Tournaments[ TournamentID ].teams_registered[ j ].players_for_local_acces[ k ] != NULL )
			{
				Player * chr = Tournaments[ TournamentID ].teams_registered[ j ].players_for_local_acces[ k ];
				//normal remove
				chr->RemovePvPFlag();
				chr->RemoveFFAPvPFlag();
				//forced remove
				chr->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
				chr->RemoveByteFlag(UNIT_FIELD_BYTES_2,	1, UNIT_BYTE2_FLAG_PVP);
				chr->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
				chr->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_FREE_FOR_ALL_PVP);
				if( chr->IsDead() )
					chr->ResurrectPlayer();
				chr->bInvincible = false;
			}
}

//function is called from a threadsafe enviroment
//Note this function should be only called from UpdateTournaments() since it relies on local data
void	PickNext2TeamsToFight( uint32 TournamentID )
{
	//just make sure we remove PVP from old teams. Putting this here to remove redundancy :(
	RemoveTournamentStatesFromPlayers( TournamentID );

	//there are 2 methods that usually tournaments follow :
	// 1) ladder. 2 teams fight and eliminated team is eliminated forever
	// 2) all on all. All teams fight each other and team with most kills is the winner. With 10 teams this would mean 100 matches. Each match 5 minutes = almost 10 hours
	Tournament_Info *tur = &Tournaments[ TournamentID ];

#ifdef TOURNAMENT_TYPE_LADDER
	//////////////
	// ladder type
	//////////////
	//seems to finish too quickly
	//get max matches made until now
	uint32 reamining_ladder_teams = 0;
	uint32 next_round_matches = 0;
	uint32 possible_winner_team;
	for(uint32 j=0;j<MAX_TEAMS_PER_TOURNAMENT;j++)
		if( tur->teams_registered[ j ].is_used != 0 )
		{
			Team_Info *team = &tur->teams_registered[ j ];
			//if we won all matches that we played until now and we belong to this "level" of ladder then we are a good to go team
			if( tur->ladder_step == team->matches_won_total 
				&& team->matches_won_total == team->matches_played_total
				)
			{
				reamining_ladder_teams++;
				possible_winner_team = j;
			}
			if( tur->ladder_step < team->matches_won_total )
				next_round_matches++;
		}
	//seems like the winner team quitted ? I mean the tournament winner team. What should we do now ?
	if( reamining_ladder_teams == 0 && next_round_matches == 0 )
	{
		BroadcastToTournament( TournamentID, "Lol,all teams that should fight for the win have quitted the tournament. Canceling Tournament" );
		ResetTournamentForNew( TournamentID );
		return;
	}	
	//seems like this team has no pair at this level. We declare them winners and move on to the next level of the tournament
	if( reamining_ladder_teams == 1 && next_round_matches != 0 )
	{
		tur->teams_registered[ possible_winner_team ].matches_won_total++;
		tur->teams_registered[ possible_winner_team ].matches_played_total++;
		tur->ladder_step++;
		PickNext2TeamsToFight( TournamentID );
		return;
	}
	//1 team and no next level = win
	if( reamining_ladder_teams == 1 && next_round_matches == 0 )
	{
		HandleTournamentWin( TournamentID, possible_winner_team );
		ResetTournamentForNew( TournamentID );
		return;
	}
	//pick the next 2 (or 1 ) teams to fight
	if( reamining_ladder_teams == 0 && next_round_matches > 0 )
	{
		tur->ladder_step++;
		PickNext2TeamsToFight( TournamentID );
		return;
	}
	//pick the next 2 (or 1 ) teams to fight
	if( reamining_ladder_teams >= 2 )
	{
		int32 T1 = -1, T2 = -1;
		for(uint32 j=0;j<MAX_TEAMS_PER_TOURNAMENT;j++)
			if( tur->teams_registered[ j ].is_used != 0 )
			{
				Team_Info *team = &tur->teams_registered[ j ];
				//if we won all matches that we played until now and we belong to this "level" of ladder then we are a good to go team
				if( tur->ladder_step == team->matches_won_total 
					&& team->matches_won_total == team->matches_played_total
					)
				{
					if( T1 == -1 )
						T1 = j;
					else if( T2 == -1 )
					{
						T2 = j;
						break;
					}
				}
			}
		//sanity check. Should never happen
		if( T1 == -1 && T2 == -1 )
		{
			BroadcastToTournament( TournamentID, "Something bad happened. Tell the devs that error -2 ocured" );
			ResetTournamentForNew( TournamentID );
			return;
		}
		//check if there is only 1 team remaining = win
		if( T2 == -1 && T1 != -1 )
		{
			HandleTournamentWin( TournamentID, T1 );
			ResetTournamentForNew( TournamentID );
			return;
		}
		tur->fighting_teams[ 0 ] = T1;
		tur->fighting_teams[ 1 ] = T2;
		tur->next_state_change_stamp = GetTickCount() + PLAYER_TELEPORT_WAIT;
		tur->status = TS_TELEPORT_PLAYERS;

		//tell the others who is going to fight
		char tell_what[500];
		sprintf( tell_what,"Tournament : %s, Teams Fighting : '%s' vs '%s'",TournamentTypeNames[ TournamentID ],Tournaments[ TournamentID ].teams_registered[ T1 ].name, Tournaments[ TournamentID ].teams_registered[ T2 ].name );
		BroadcastToTournament( TournamentID, tell_what );
		BroadcastToTournamentTeam( TournamentID, Tournaments[ TournamentID ].fighting_teams[0], "Prepare to port to fight");
		BroadcastToTournamentTeam( TournamentID, Tournaments[ TournamentID ].fighting_teams[1], "Prepare to port to fight");
	}
#endif

#ifdef TOURNAMENT_TYPE_WEAKEST_VS_WEAKEST
	/////////////
	// all vs all
	/////////////
	//might take more then an hour with 10 teams
	//get the 2 lowest score teams
	//1 team is eliminated on each MAX_TEAMS_PER_TOURNAMENT match...
	int32 team1;
	int32 team2;
	int32 last_valid_team=-1;	//this might be winner team
	uint32 total_match_count=(MAX_TEAMS_PER_TOURNAMENT-1)*MAX_TEAMS_PER_TOURNAMENT/2;
	for(uint32 all=0;all<total_match_count;all++)
	{
		team1 = -1;
		team2 = -1;
		for(uint32 j=0;j<MAX_TEAMS_PER_TOURNAMENT;j++)
			if( tur->teams_registered[ j ].is_used != 0 
				&& tur->teams_registered[ j ].matches_won_total == all
				)
			{
				last_valid_team = j;
				if( team1 == -1 )
					team1 = j;
				else if( team2 == -1 )
					team2 = j;
				else
					break;
			}
		//did we find a match ?
		if( team1 != -1 && team2 != -1 )
			break;
	}
	//seems like there are no more teams to be able to match up. This is the winner team
	if( team2 == -1 && ( team1 != -1 || last_valid_team != -1 ) )
	{
		HandleTournamentWin( TournamentID, last_valid_team );
		ResetTournamentForNew( TournamentID );
	}
	//sanity check. Should never happen
	else if( team2 == -1 && team1 == -1 )
	{
		BroadcastToTournament( TournamentID, "Something bad happened. Tell the devs that error -3 ocured" );
		ResetTournamentForNew( TournamentID );
		return;
	}
	else  if( team2 != -1 && team1 != -1 )
	{
		//set fighting teams
		tur->fighting_teams[ 0 ] = team1;
		tur->fighting_teams[ 1 ] = team2;
		//set next tournament state
		tur->next_state_change_stamp = GetTickCount();
		tur->status = TS_TELEPORT_PLAYERS;
		//tell the others who is going to fight
		char tell_what[500];
		sprintf( tell_what,"Tournament : %s, Teams Fighting : '%s' vs '%s'",TournamentTypeNames[ TournamentID ],Tournaments[ TournamentID ].teams_registered[ team1 ].name, Tournaments[ TournamentID ].teams_registered[ team2 ].name );
		BroadcastToTournament( TournamentID, tell_what );
		BroadcastToTournamentTeam( TournamentID, Tournaments[ TournamentID ].fighting_teams[0], "Prepare to port to fight");
		BroadcastToTournamentTeam( TournamentID, Tournaments[ TournamentID ].fighting_teams[1], "Prepare to port to fight");
	}/**/
#endif
}

//function is called from a threadsafe enviroment
//Note this function should be only called from PickNext2TeamsToFight() since it relies on local data
void	HandleTournamentWin( uint32 TournamentID, uint32 teamID)
{
	//reset special states
	RemoveTournamentStatesFromPlayers( TournamentID );
	//award
	Team_Info *team = &Tournaments[ TournamentID ].teams_registered[ teamID ];
	char say_what[500];
	sprintf(say_what,"Team '%s' has won the tournament",team->name );
	BroadcastToTournament( TournamentID, say_what );
	for(uint32 k=0;k<MAX_PLAYERS_PER_TEAM; k++ )
		if( team->players_for_local_acces[ k ] )
		{
			Player * chr = team->players_for_local_acces[ k ];
			//get next available free title and award it
			for( uint32 i=PVPTITLE_THE_ARGENT_DEFENDER;i>0;i--)
				if( chr->HasKnownTitle( (RankTitles)i ) == false )
				{
					chr->SetKnownTitle( (RankTitles)i, true );
					break;
				}
		}
}

void	ResetTournamentForNew( uint32 TournamentID )
{
	//reset special states
	RemoveTournamentStatesFromPlayers( TournamentID );
	//mark all team places as free to be ocupied by new match participants
	for(uint32 k=0;k<MAX_TEAMS_PER_TOURNAMENT; k++ )
	{
		Tournaments[ TournamentID ].teams_registered[ k ].is_used = 0; //clear used up slot
		strcpy( Tournaments[ TournamentID ].teams_registered[ k ].name,"");	//clear name
		//clear out team player slots
		Team_Info *team = &Tournaments[ TournamentID ].teams_registered[ k ];
		for(uint32 i=0;i<MAX_PLAYERS_PER_TEAM; i++ )
			team->players_for_local_acces[ i ] = 0;	//clear used up slot
	}
	Tournaments[ TournamentID ].next_state_change_stamp = GetTickCount();
	Tournaments[ TournamentID ].status = TS_NOT_STARTED;
	Tournaments[ TournamentID ].fighting_teams[0] = Tournaments[ TournamentID ].fighting_teams[1] = 0;
}

//function is called from a threadsafe enviroment
//Note this function should be only called from PickNext2TeamsToFight() or UpdateTournaments() since it relies on local data
void	BroadcastToTournament( uint32 TournamentID, const char *what )
{
	for(uint32 k=0;k<MAX_TEAMS_PER_TOURNAMENT; k++ )
		if( Tournaments[ TournamentID ].teams_registered[ k ].is_used != 0 )
			BroadcastToTournamentTeam( TournamentID, k, what );
}

//function is called from a threadsafe enviroment
//Note this function should be only called from BroadcastToTournament() or UpdateTournaments() since it relies on local data
void	BroadcastToTournamentTeam( uint32 TournamentID, uint32 TeamID, const char *what )
{
	Team_Info *team = &Tournaments[ TournamentID ].teams_registered[ TeamID ];
	for(uint32 i=0;i<MAX_PLAYERS_PER_TEAM; i++ )
		if( team->players_for_local_acces[ i ] )
			team->players_for_local_acces[i]->BroadcastMessage( what );
}