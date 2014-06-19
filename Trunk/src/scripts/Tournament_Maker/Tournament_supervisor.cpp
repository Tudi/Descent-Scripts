#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"

#ifdef WIN32
  #pragma warning(disable:4305)        // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

enum Tournament_Status
{
	TS_NOT_STARTED				, //don't really care what is happening around us
	TS_GATHER_TEAM_INFO			, //gather info about next teams 
	TS_TELEPORT_PLAYERS			, //gather players from around the world
	TS_TELEPORT_WAIT_PLAYERS	, //might take up to 10 seconds to get players to load map
	TS_PREPARE_PLAYERS			, //set factions and flags
	TS_PREBUFF_INTERVAL			, //let players buff each other
	TS_PREPARE_PLAYERS2			, //set factions and flags
	TS_WAIT_FOR_FIGHT			, //players fight each other until one of teams have only dead/no players
};

struct Team_Info
{
	uint32				id;
	char				name[MAX_STRING_LENGTH];
	uint64				leader_guid;
	uint32				matches_won_total;
};

class Tournament_Supervisor : public AIInterface
{
public:
	void				Init(Unit *un, AIType at, MovementType mt);
	void				Update(uint32 p_time);
private:
	uint8				IsPlayerInTeam1(uint32 pguid);//this is supervisor checking player in ring
	uint8				IsPlayerInTeam2(uint32 pguid);//this is supervisor checking player in ring
	uint8				IsPlayerForMatch(uint32 pguid);//this is supervisor checking player in ring
	void				ScanAndKickNoTournamentPlayers();
	void				MakeCurTeamSpectator();		//forces last team that was fighting to become spectators
	void				Event_team1_loose();//mark this event in db
	void				Event_team2_loose();//mark this event in db
	void				Event_team1_win();//mark this event in db
	void				Event_team2_win();//mark this event in db
	void				Event_team1_kick();//mark this event in db
	void				Event_team2_kick();//mark this event in db
	uint32				next_update;
	uint32				tournament_status;
	uint64				team1guids[MAX_PLAYERS_PER_TEAM];
	uint8				team1_tel_status[MAX_PLAYERS_PER_TEAM];
	uint8				team1_inrange_warnings[MAX_PLAYERS_PER_TEAM];
	uint32				team1_players;
	uint64				team2guids[MAX_PLAYERS_PER_TEAM];
	uint8				team2_tel_status[MAX_PLAYERS_PER_TEAM];
	uint8				team2_inrange_warnings[MAX_PLAYERS_PER_TEAM];
	uint32				team2_players;
	uint32				total_team_count;				//total teams available
	uint32				cur_match_round;				//round 1 means to make matches for teams with no matches
	uint32				player_teleport_retrys;
	Team_Info			Team1Info;
	Team_Info			Team2Info;
	uint32				max_chars_per_team;
	uint32				min_player_level_to_join;
	uint32				max_player_level_to_join;
	char				msg[MAX_STRING_LENGTH];			//announce buffer
	uint32				sudden_death_trigger_stamp;		//next sudden death will trigger at this stamp
	uint32				sudden_death_trigger_count;		//next sudden death will trigger at this stamp
};

void Tournament_Supervisor::Init(Unit *un, AIType at, MovementType mt)
{
	AIInterface::Init(un,at,mt);//run the original init and we add our custom stuff too
	next_update = 0;
	tournament_status = TS_NOT_STARTED;
	memset(team1guids,NULL,sizeof(uint64)*MAX_PLAYERS_PER_TEAM);
	memset(team2guids,NULL,sizeof(uint64)*MAX_PLAYERS_PER_TEAM);
	team1_players = team2_players = 0;
	cur_match_round = 0;
	total_team_count = 0;
	cur_tournament_id = 1;
}

uint8 Tournament_Supervisor::IsPlayerInTeam1(uint32 pguid)
{
	for( uint32 i=0;i<team1_players;i++)
		if( team1guids[i] == pguid )
			return i+1;
	return 0;
}

uint8 Tournament_Supervisor::IsPlayerInTeam2(uint32 pguid)
{
	for( uint32 i=0;i<team2_players;i++)
		if( team2guids[i] == pguid )
			return i+1;
	return 0;
}

//check if we have this guid in the team list
uint8 Tournament_Supervisor::IsPlayerForMatch(uint32 pguid)
{
	if( IsPlayerInTeam1( pguid ) )
		return 1;
	if( IsPlayerInTeam2( pguid ) )
		return 2;
	return 0;
}

//kick old team by making them spectators
void Tournament_Supervisor::MakeCurTeamSpectator()
{
	MapMgr *mmgr = m_Unit->GetMapMgr();
	if( !mmgr )
		return; //omg wtf ?

	for(uint32 i=0;i<team1_players;i++)
	{
		Player *plr = mmgr->GetPlayer( team1guids[ i ] );
		if( plr )
		{
			//reset faction
			if( plr->GetCreateInfo() )
				plr->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, plr->GetCreateInfo()->factiontemplate );
			MakeUnitSpectator(plr,1);
		}
	}

	for(uint32 i=0;i<team2_players;i++)
	{
		Player *plr = mmgr->GetPlayer( team2guids[ i ] );
		if( plr )
		{
			//reset faction
			if( plr->GetCreateInfo() )
				plr->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, plr->GetCreateInfo()->factiontemplate );
			MakeUnitSpectator(plr,1);
		}
	}
}

void Tournament_Supervisor::ScanAndKickNoTournamentPlayers()
{
	InRangeSet::iterator itr;
	//i really hope inrange is big enough to see everything
	for( itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); ++itr)
	{
		//check if player is for tournament
		if( (*itr)->IsPlayer() )
		{
			Player *pptr = (Player*)(*itr);

			//gms are always allowed
			if( pptr->GetSession() && pptr->GetSession()->HasGMPermissions() )
				continue;

			//check if he is part for the tournament. If not we kick him
			if( !IsPlayerForMatch( pptr->GetGUID() ) )
			{ 
				if( !IsPlayerSpectator( pptr->GetGUID() ) )
				{
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : Player %s is not spectator and he is not fighter ",pptr->GetName() );
#endif
//					pptr->EventTeleport( TOURNAMENT_SPECTATOR_RET_MAP, TOURNAMENT_SPECTATOR_RET_X, TOURNAMENT_SPECTATOR_RET_Y, TOURNAMENT_SPECTATOR_RET_Z );
					if(!sEventMgr.HasEvent(pptr, EVENT_PLAYER_TELEPORT))
						sEventMgr.AddEvent(pptr, &Player::EventTeleport, TOURNAMENT_SPECTATOR_RET_MAP, TOURNAMENT_SPECTATOR_RET_X, TOURNAMENT_SPECTATOR_RET_Y, TOURNAMENT_SPECTATOR_RET_Z, EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				}
				//check if he is still in he's seat
				else if( pptr->GetPositionX() != m_Unit->GetPositionX() || pptr->GetPositionY() != m_Unit->GetPositionY() )
				{
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : Player %s is spectator but he is not in place ",pptr->GetName() );
#endif
					RemoveUnitSpectator( pptr, 1 ); 
				}
			}
			//if player somehow exited ring then we warn him
			else if( !IsInrangeSQ( m_Unit, pptr, SUPERVIZOR_ARENA_SIZE_SQUARE) )
			{
				uint8 team1slot = IsPlayerInTeam1( pptr->GetGUID() );
				if( team1slot )
				{
					team1_inrange_warnings[ team1slot - 1 ]++;
					sprintf(msg,SUPERVISOR_WARNING_TXT_OUT_OF_ARENA,SUPERVIZOR_WARNINGS_FOR_EXITED_ARENA - team1_inrange_warnings[ team1slot - 1 ] );
					pptr->BroadcastMessage( msg );
					if( team1_inrange_warnings[ team1slot - 1 ] > SUPERVIZOR_WARNINGS_FOR_EXITED_ARENA )
						pptr->Root();
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : Player %s is fighter and leaving the ring ",pptr->GetName() );
#endif
				}
				else
				{
					uint8 team2slot = IsPlayerInTeam1( pptr->GetGUID() );
					if( team2slot ) //now this would be a deep suprize
					{
						team2_inrange_warnings[ team2slot - 1 ]++;
						sprintf(msg,SUPERVISOR_WARNING_TXT_OUT_OF_ARENA,SUPERVIZOR_WARNINGS_FOR_EXITED_ARENA - team2_inrange_warnings[ team2slot - 1 ] );
						pptr->BroadcastMessage( msg );
						if( team2_inrange_warnings[ team2slot - 1 ] > SUPERVIZOR_WARNINGS_FOR_EXITED_ARENA )
							pptr->Root();
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : Player %s is fighter and leaving the ring ",pptr->GetName() );
#endif
					}
				}
			}
			//make sure to clear out of range warnings
			else 
			{
			}

		}
		else if( (*itr)->IsUnit() )
		{
			//Does this thing belong to us ?
			if( ( (*itr)->GetUInt32Value( OBJECT_FIELD_ENTRY ) == CREATURE_ENTRY_FOR_SPECTATOR_SUPERVIZOR )
				|| ( (*itr)->GetUInt32Value( OBJECT_FIELD_ENTRY ) == CREATURE_ENTRY_FOR_SPECTATOR_SUPERVIZOR ) )
				continue; // we are ok to idle there

			//is he a summon of one of the tournament players ?
			if( (*itr)->GetUInt32Value( UNIT_FIELD_SUMMONEDBY ) && IsPlayerForMatch( (*itr)->GetUInt32Value( UNIT_FIELD_SUMMONEDBY ) ) )
				continue;

			//so is he created at least ?
			if( (*itr)->GetUInt32Value( UNIT_FIELD_CREATEDBY ) && IsPlayerForMatch( (*itr)->GetUInt32Value( UNIT_FIELD_CREATEDBY ) ) )
				continue;

#ifdef WRITE_DEBUG_INFO_TO_LOG
//don't report dead mobs
if( (Unit*)(*itr)->GetUInt32Value(UNIT_FIELD_HEALTH) > 0 )
sLog.outDebug("Tournament DEBUG : Killing mob with entry %u as he is not part of fight ",(*itr)->GetUInt32Value( OBJECT_FIELD_ENTRY ) );
#endif
			// die then -> really hope he will not resist it :D
			m_Unit->DealDamage( (Unit*)(*itr), (*itr)->GetUInt32Value(UNIT_FIELD_HEALTH) * 10, 0, 0, 0);
		}
	}
}

void Tournament_Supervisor::Event_team1_loose()
{
	QueryResult *result = CharacterDatabase.Query("UPDATE tournament_team_settings SET matches_finished = matches_finished + 1 WHERE tournament_id='%u' and id='%u'", cur_tournament_id,Team1Info.id);
	if( result )
		delete result;
}

void Tournament_Supervisor::Event_team2_loose()
{
	QueryResult *result = CharacterDatabase.Query("UPDATE tournament_team_settings SET matches_finished = matches_finished + 1 WHERE tournament_id='%u' and id='%u'", cur_tournament_id,Team2Info.id);
	if( result )
		delete result;
}
void Tournament_Supervisor::Event_team1_win()
{
	QueryResult *result = CharacterDatabase.Query("UPDATE tournament_team_settings SET matches_finished = matches_finished + 1,matches_won = matches_won + 1, matches_won_total = matches_won_total + 1 WHERE tournament_id='%u' and id='%u'", cur_tournament_id,Team1Info.id);
	if( result )
		delete result;
}
void Tournament_Supervisor::Event_team2_win()
{
	QueryResult *result = CharacterDatabase.Query("UPDATE tournament_team_settings SET matches_finished = matches_finished + 1,matches_won = matches_won + 1, matches_won_total = matches_won_total + 1 WHERE tournament_id='%u' and id='%u'", cur_tournament_id,Team2Info.id);
	if( result )
		delete result;
}
void Tournament_Supervisor::Event_team1_kick()
{
	QueryResult *result = CharacterDatabase.Query("UPDATE tournament_team_settings SET matches_finished = 999999 WHERE tournament_id='%u' and id='%u'", cur_tournament_id,Team1Info.id);
	if( result )
		delete result;
}
void Tournament_Supervisor::Event_team2_kick()
{
	QueryResult *result = CharacterDatabase.Query("UPDATE tournament_team_settings SET matches_finished = 999999 WHERE tournament_id='%u' and id='%u'", cur_tournament_id,Team2Info.id);
	if( result )
		delete result;
}

void Tournament_Supervisor::Update( uint32 p_time )
{
	//ai interface is updated usually every 500 ms
	if( next_update > getMSTime() )
		return;
	else next_update = getMSTime() + SUPERVISOR_UPDATE_INTERVAL;

	//clear auras in case they managed to case on us
	for(uint32 i=0;i<MAX_AURAS+MAX_PASSIVE_AURAS;i++)
		if( m_Unit->m_auras[i] )
			m_Unit->m_auras[i]->Remove();

	//while we are on idle we do not take any actions. We are waiting on GM to resume us
	if( m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) == SUPERVISOR_HP_TO_IDLE )
		return;

	//abort curent paused match and restart the whole thing
	if( m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) == SUPERVISOR_HP_TO_START )
	{
		m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, SUPERVISOR_GOSSIPGM_MENU_START_AN);
		tournament_status = TS_GATHER_TEAM_INFO;
		m_Unit->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_NO_NEW_COMMAND);
	}

	//force a team to loose
	if( m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) == SUPERVISOR_HP_TO_LOOSE1 )
	{
		sprintf(msg,SUPERVISOR_GOSSIPGM_MENU_LOOSE_AN,Team1Info.name);
		m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
		Event_team1_loose();
		tournament_status = TS_NOT_STARTED;
		m_Unit->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_NO_NEW_COMMAND);
	}

	//force a team to loose
	if( m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) == SUPERVISOR_HP_TO_LOOSE2 )
	{
		sprintf(msg,SUPERVISOR_GOSSIPGM_MENU_LOOSE_AN,Team2Info.name);
		m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
		Event_team2_loose();
		tournament_status = TS_NOT_STARTED;
		m_Unit->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_NO_NEW_COMMAND);
	}

	//force a team to win
	if( m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) == SUPERVISOR_HP_TO_WIN1 )
	{
		sprintf(msg,SUPERVISOR_GOSSIPGM_MENU_WIN_AN,Team1Info.name);
		m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
		Event_team1_win();
		tournament_status = TS_NOT_STARTED;
		m_Unit->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_NO_NEW_COMMAND);
	}

	//force a team to win
	if( m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) == SUPERVISOR_HP_TO_WIN2 )
	{
		sprintf(msg,SUPERVISOR_GOSSIPGM_MENU_WIN_AN,Team2Info.name);
		m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
		Event_team2_win();
		tournament_status = TS_NOT_STARTED;
		m_Unit->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_NO_NEW_COMMAND);
	}

	//force a team to disband by loosing all matches
	if( m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) == SUPERVISOR_HP_TO_KICK1 )
	{
		sprintf(msg,SUPERVISOR_GOSSIPGM_MENU_KICK_AN,Team1Info.name);
		m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
		Event_team1_kick();
		tournament_status = TS_NOT_STARTED;
		m_Unit->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_NO_NEW_COMMAND);
	}

	//force a team to disband by loosing all matches
	if( m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) == SUPERVISOR_HP_TO_KICK2 )
	{
		sprintf(msg,SUPERVISOR_GOSSIPGM_MENU_KICK_AN,Team2Info.name);
		m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
		Event_team2_kick();
		tournament_status = TS_NOT_STARTED;
		m_Unit->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_NO_NEW_COMMAND);
	}

	//////////////////////////////////////////////
	//kick everything that is not related to the tournament
	//////////////////////////////////////////////
	if( tournament_status == TS_PREPARE_PLAYERS
		|| tournament_status == TS_PREBUFF_INTERVAL 
		|| tournament_status == TS_WAIT_FOR_FIGHT 
		)
		ScanAndKickNoTournamentPlayers();

	//////////////////////////////////////////////
	//make sure to not let spectators come in if there is no fight
	//////////////////////////////////////////////
	if( tournament_status == TS_NOT_STARTED )
		tournament_started = 0;

	//////////////////////////////////////////////
	//check the status of the tournament
	//////////////////////////////////////////////
	if( tournament_status == TS_GATHER_TEAM_INFO )
	{
		//send previous team to spectators mode
		MakeCurTeamSpectator();

		//instant query execution
		QueryResult *result;
		result = CharacterDatabase.Query("SELECT count(*) FROM tournament_team_settings WHERE tournament_id='%u'", cur_tournament_id);	
		if(result)
		{
			total_team_count = result->Fetch()[0].GetUInt32();
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : Team count : %u ",total_team_count);
#endif
			delete result;
		}

		//get the round count we are supposed to be
		if( total_team_count )
		{
			tournament_started = 1; //let spectators flow in

			Field *fields;
			const char *name;
			result = CharacterDatabase.Query("SELECT cur_match_round,max_chars_per_team,min_player_level_to_join,max_player_level_to_join FROM tournament_settings WHERE id='%u'", cur_tournament_id);	
			if( result )
			{
				fields = result->Fetch();
				cur_match_round = fields[0].GetUInt32();
				max_chars_per_team = fields[1].GetUInt32();
				min_player_level_to_join = fields[2].GetUInt32();
				max_player_level_to_join = fields[3].GetUInt32();
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : cur_match_round : %u ",cur_match_round);
sLog.outDebug("Tournament DEBUG : max_chars_per_team : %u ",max_chars_per_team);
sLog.outDebug("Tournament DEBUG : min_player_level_to_join : %u ",min_player_level_to_join);
sLog.outDebug("Tournament DEBUG : max_player_level_to_join : %u ",max_player_level_to_join);
#endif
				delete result;
			}
			sprintf(msg,SUPERVISOR_ANNOUNCE_TXT_START_ROUND,cur_match_round + 1 );
			m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
			//get the 2 next available teams

			memset( &Team1Info, 0 , sizeof(Team_Info) );
			result = CharacterDatabase.Query("SELECT id,team_leader_guid,team_name,matches_won_total FROM tournament_team_settings WHERE tournament_id='%u' and matches_finished='%u' and matches_won=matches_finished order by id asc limit 0,2", cur_tournament_id,cur_match_round);	
			if( result )
			{
				fields = result->Fetch();
				Team1Info.id = fields[0].GetUInt32();
				Team1Info.leader_guid = fields[1].GetUInt32();
				name = fields[2].GetString();
				if( name && strlen(name)<MAX_STRING_LENGTH )
					strcpy( Team1Info.name, name);
				else
					strcpy( Team1Info.name, "NoName" );
				strcpy(team1name,Team1Info.name);
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : team1 id : %u ",Team1Info.id);
sLog.outDebug("Tournament DEBUG : team1 name : %s ",team1name);
#endif
				Team1Info.matches_won_total = fields[3].GetUInt32();
			}
			else
			{
				//this means we have no more teams on this level of matches. We eighter advance round or we need finished this tournament
				result = CharacterDatabase.Query("update tournament_settings set cur_match_round = cur_match_round + 1 WHERE id='%u'", cur_tournament_id);
				if( result )
					delete result;
				return;
			}

			uint8 has_next_row;
			if( result )
				has_next_row = result->NextRow();

			memset( &Team2Info, 0 , sizeof(Team_Info) );
			if( result && has_next_row )
			{
				fields = result->Fetch();
				Team2Info.id = fields[0].GetUInt32();
				Team2Info.leader_guid = fields[1].GetUInt32();
				name = fields[2].GetString();
				if( name && strlen(name)<MAX_STRING_LENGTH )
					strcpy( Team2Info.name, name);
				else
					strcpy( Team2Info.name, "NoName" );
				strcpy(team2name,Team2Info.name);
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : team2 id : %u ",Team2Info.id);
sLog.outDebug("Tournament DEBUG : team2 name : %s ",team2name);
#endif
				Team2Info.matches_won_total = fields[3].GetUInt32();
			}
			else
			{
				//if next level of rounds has only 0 teams then this team is the winner
				result = CharacterDatabase.Query("SELECT count(*) FROM tournament_team_settings WHERE tournament_id='%u' and matches_finished='%u'", cur_tournament_id,cur_match_round+1);	
				if( result )
				{
					fields = result->Fetch();
					uint32 teams_on_next_round = fields[0].GetUInt32();
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : round %u will have %u teams to compete ",cur_match_round+1,teams_on_next_round);
#endif
					delete result;
					if( teams_on_next_round == 0 )
					{
						//we probably have only 1 team left so we can declare him as winner
						tournament_status = TS_NOT_STARTED;
						sprintf(msg,SUPERVISOR_ANNOUNCE_TXT_TOURNAMENT_WIN,Team1Info.name);
						m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
						return;
					}
				}
			}
			sprintf(msg,SUPERVISOR_ANNOUNCE_TXT_START_MATCH,Team1Info.name,Team2Info.name);
			m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);

			sprintf(msg,SUPERVISOR_ANNOUNCE_TXT_TEAM_STATS,Team1Info.name,Team1Info.matches_won_total);
			m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
			sprintf(msg,SUPERVISOR_ANNOUNCE_TXT_TEAM_STATS,Team2Info.name,Team2Info.matches_won_total);
			m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);

			//gather player info for teams
			memset(team1_tel_status,NULL,sizeof(uint8)*MAX_PLAYERS_PER_TEAM);
			memset(team1_inrange_warnings,NULL,sizeof(uint8)*MAX_PLAYERS_PER_TEAM);			
			team1_players=0;
			result = CharacterDatabase.Query("SELECT player_guid FROM tournament_players WHERE tournament_id='%u' and team_id='%u' and is_pending_to_join_team=0 and is_pending_to_accept_team_invite=0 limit 0,%u", cur_tournament_id,Team1Info.id,max_chars_per_team);	
			if( result )
			{
				Field *fields;
				do {
					team1guids[team1_players++] = result->Fetch()[0].GetInt32();
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : team1 add guid : %u ",team1guids[team1_players-1]);
#endif
					if( team1_players == MAX_PLAYERS_PER_TEAM || team1_players == max_chars_per_team )
						break;
				} while(result->NextRow());
				delete result;					
			}
			memset(team2_tel_status,NULL,sizeof(uint8)*MAX_PLAYERS_PER_TEAM);
			memset(team2_inrange_warnings,NULL,sizeof(uint8)*MAX_PLAYERS_PER_TEAM);			
			team2_players=0;
			result = CharacterDatabase.Query("SELECT player_guid FROM tournament_players WHERE tournament_id='%u' and team_id='%u' and is_pending_to_join_team=0 and is_pending_to_accept_team_invite=0 limit 0,%u", cur_tournament_id,Team2Info.id,max_chars_per_team);	
			if( result )
			{
				Field *fields;
				do {
					team2guids[team2_players++] = result->Fetch()[0].GetInt32();
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : team2 add guid : %u ",team2guids[team2_players-1]);
#endif
					if( team2_players == MAX_PLAYERS_PER_TEAM || team2_players == max_chars_per_team )
						break;
				} while(result->NextRow());
				delete result;					
			}
		}
		player_teleport_retrys = 0;
		tournament_status = TS_TELEPORT_PLAYERS; //the next step
		uint32 time_to_wait = SUPERVISOR_UPDATE_INTERVAL * ( PLAYER_TELEPORT_RETRY_CYCLES_COUNT_MAX + PLAYER_TELEPORT_WAIT_CYCLES_COUNT_MAX ) / 1000;
		sprintf( msg, SUPERVISOR_ANNOUNCE_TXT_WAIT_PORT, time_to_wait );
		m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
	}

	//////////////////////////////////////////////
	//search for these players all around the world to teleport hem to us
	//////////////////////////////////////////////
	if( tournament_status == TS_TELEPORT_PLAYERS )
	{
		uint32 managed_to_teleport_all = 1;
		//for each player in the teams
		for(uint32 i=0;i<team1_players;i++)
			if( team1_tel_status[i] == 0 )
			{
				Player *chr = objmgr.GetPlayer( team1guids[i] );
				if( chr )
				{
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : team1 char %u = %s is getting ported ",i,chr->GetName());
#endif
					if( chr->IsBeingTeleported() == true ) 
						managed_to_teleport_all = 0;
					else
					{
						if(!sEventMgr.HasEvent(chr, EVENT_PLAYER_TELEPORT))
						{
							chr->RemovePvPFlag();
							if( PLAYER_FACTION_TEAM1_BUFF )
							{
								chr->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, PLAYER_FACTION_TEAM1_BUFF );
								chr->_setFaction();
							}
							sEventMgr.AddEvent(chr, &Player::EventTeleport, m_Unit->GetMapId(), m_Unit->GetPositionX() - 5, m_Unit->GetPositionY() + i, m_Unit->GetPositionZ() + 1, EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
							team1_tel_status[i] = 1;
						}
					}
				}
				else 
					managed_to_teleport_all = 0; //maybe he is not online yet
			}
		for(uint32 i=0;i<team2_players;i++)
			if( team2_tel_status[i] == 0 )
			{
				Player *chr = objmgr.GetPlayer( team2guids[i] );
				if( chr )
				{
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : team2 char %u = %s is getting ported ",i,chr->GetName());
#endif
					if( chr->IsBeingTeleported() == true ) 
						managed_to_teleport_all = 0;
					else
					{
						if(!sEventMgr.HasEvent(chr, EVENT_PLAYER_TELEPORT))
						{
							chr->RemovePvPFlag();
							if( PLAYER_FACTION_TEAM2_BUFF )
							{
								chr->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, PLAYER_FACTION_TEAM2_BUFF );
								chr->_setFaction();
							}
							sEventMgr.AddEvent(chr, &Player::EventTeleport, m_Unit->GetMapId(), m_Unit->GetPositionX() + 5, m_Unit->GetPositionY() + i, m_Unit->GetPositionZ() + 1, EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
							team2_tel_status[i] = 1;
						}
					}
				}
				else 
					managed_to_teleport_all = 0; //maybe he is not online yet
			}
		if( managed_to_teleport_all == 1 || player_teleport_retrys > PLAYER_TELEPORT_RETRY_CYCLES_COUNT_MAX )
		{
			tournament_status = TS_TELEPORT_WAIT_PLAYERS;
			player_teleport_retrys = 0;
		}
		else
			player_teleport_retrys++;
	}

	//////////////////////////////////////////////
	//wait until client loads the maps
	//////////////////////////////////////////////
	if( tournament_status == TS_TELEPORT_WAIT_PLAYERS )
	{
		if ( player_teleport_retrys > PLAYER_TELEPORT_WAIT_CYCLES_COUNT_MAX )
			tournament_status = TS_PREPARE_PLAYERS;
		player_teleport_retrys++;
	}

	//////////////////////////////////////////////
	//prepare players for battle.
	//////////////////////////////////////////////
	if( tournament_status == TS_PREPARE_PLAYERS )
	{
		uint32 team1active_player_count = 0;
		uint32 team2active_player_count = 0;
		set<Player*>::iterator itr;
		//i really hope inrange is big enough to see everything
		for( itr = m_Unit->GetInRangePlayerSetBegin(); itr != m_Unit->GetInRangePlayerSetEnd(); ++itr)
		{
			if( !(*itr) || !(*itr)->IsInWorld() )
				continue;
			if( !IsPlayerForMatch( (*itr)->GetGUID() ) )
				continue; //no match players will get taken care by supervisor

			//count active players for team
			if( IsPlayerInTeam1( (*itr)->GetGUID() ) )
				team1active_player_count++;
			else if( IsPlayerInTeam2( (*itr)->GetGUID() ) )
				team2active_player_count++;

			//check if he is in one of the teams to be revived and stuff
			if( (*itr)->IsDead() )
				(*itr)->ResurrectPlayer();

			//remove all negative buffs
			for(uint32 buffitr=0;buffitr<MAX_AURAS;buffitr++)
				if( (*itr)->m_auras[ buffitr ] )
					(*itr)->m_auras[ buffitr ]->Remove();

			//set max health
			(*itr)->SetUInt32Value(UNIT_FIELD_HEALTH, (*itr)->GetUInt32Value(UNIT_FIELD_MAXHEALTH) );

			//reset energy
			(*itr)->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_MANA, (*itr)->GetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_MANA) );
			(*itr)->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_RAGE, 0 );
			(*itr)->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_FOCUS, 0 );
			(*itr)->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_ENERGY, (*itr)->GetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY) );
			(*itr)->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_RUNIC_POWER, 0 );

			//in case he was a spectator then remove spectator related stuff
			RemoveUnitSpectator( (*itr), 0 );

			//set faction
			if( PLAYER_FACTION_TEAM1 && IsPlayerInTeam1( (*itr)->GetGUID() ) )
			{
				(*itr)->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, PLAYER_FACTION_TEAM1 );
				(*itr)->_setFaction();
			}
			if( PLAYER_FACTION_TEAM2 && IsPlayerInTeam2( (*itr)->GetGUID() ) )
			{
				(*itr)->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, PLAYER_FACTION_TEAM2 );
				(*itr)->_setFaction();
			}

		}
		//announce team status
		sprintf(msg,SUPERVISOR_ANNOUNCE_TXT_TEAM_PLAYERS_ONLINE,Team1Info.name,team1active_player_count);
		m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
		sprintf(msg,SUPERVISOR_ANNOUNCE_TXT_TEAM_PLAYERS_ONLINE,Team2Info.name,team2active_player_count);
		m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);

		if( team1active_player_count == 0 || team2active_player_count == 0 )
			tournament_status = TS_PREBUFF_INTERVAL;
		else
			tournament_status = TS_WAIT_FOR_FIGHT;
		player_teleport_retrys = 0;
		m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, SUPERVISOR_ANNOUNCE_TXT_PLAYERS_BUFF);
	}

	//////////////////////////////////////////////
	//wait for players to buff each other.
	//////////////////////////////////////////////
	if ( tournament_status == TS_PREBUFF_INTERVAL )
	{
		if ( player_teleport_retrys > PLAYER_PREBUFF_UPDATE_CYCLES )
		{
			tournament_status = TS_PREPARE_PLAYERS2;
			m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, SUPERVISOR_ANNOUNCE_TXT_FIGHT_START);
			sudden_death_trigger_stamp = getMSTime() + SUDDEN_DEATH_TIMER;
			sudden_death_trigger_count = 0;
		}
		sprintf(msg,SUPERVISOR_ANNOUNCE_TXT_START_TIMER,PLAYER_PREBUFF_UPDATE_CYCLES - player_teleport_retrys);
		m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
		player_teleport_retrys++;
	}

	//////////////////////////////////////////////
	//Final preparations on player (faction + hp)
	//////////////////////////////////////////////
	if ( tournament_status == TS_PREPARE_PLAYERS2 )
	{
		set<Player*>::iterator itr;
		//i really hope inrange is big enough to see everything
		for( itr = m_Unit->GetInRangePlayerSetBegin(); itr != m_Unit->GetInRangePlayerSetEnd(); ++itr)
		{
			if( !(*itr) || !(*itr)->IsInWorld() )
				continue;
			if( !IsPlayerForMatch( (*itr)->GetGUID() ) )
				continue; //no match players will get taken care by supervisor

			//set max health
			(*itr)->SetUInt32Value(UNIT_FIELD_HEALTH, (*itr)->GetUInt32Value(UNIT_FIELD_MAXHEALTH) );

			//reset energy
			(*itr)->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_MANA, (*itr)->GetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_MANA) );
			(*itr)->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_RAGE, 0 );
			(*itr)->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_FOCUS, 0 );
			(*itr)->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_ENERGY, (*itr)->GetUInt32Value(UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY) );
			(*itr)->SetUInt32Value(UNIT_FIELD_POWER1 + POWER_TYPE_RUNIC_POWER, 0 );

			//set faction
			if( PLAYER_FACTION_TEAM1 && IsPlayerInTeam1( (*itr)->GetGUID() ) )
			{
				(*itr)->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, PLAYER_FACTION_TEAM1 );
				(*itr)->_setFaction();
			}
			if( PLAYER_FACTION_TEAM2 && IsPlayerInTeam2( (*itr)->GetGUID() ) )
			{
				(*itr)->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, PLAYER_FACTION_TEAM2 );
				(*itr)->_setFaction();
			}
			if( !(*itr)->IsPvPFlagged() )
				(*itr)->SetPvPFlag();
		}
		tournament_status = TS_WAIT_FOR_FIGHT;
	}

	//////////////////////////////////////////////
	//periodically check if any of the team has fully dead players
	//////////////////////////////////////////////
	if ( tournament_status == TS_WAIT_FOR_FIGHT )
	{
		//check if any of the 2 teams has all players dead so we can declare one team winner
		MapMgr *mmgr = m_Unit->GetMapMgr();
		if( !mmgr )
			return; //omg wtf ?

		uint8 isteam1dead = 1;
		uint8 has_team1_players_online = 0;
		for(uint32 i=0;i<team1_players;i++)
			if( team1guids[ i ] )
			{
				Player *plr = mmgr->GetPlayer( team1guids[ i ] );
				if( plr )
				{
					has_team1_players_online = 1;
					if( plr->isAlive() )
					{
						isteam1dead = 0;
						//set PVP to make sure they did not disable it
						if( !plr->IsPvPFlagged() )
							plr->SetPvPFlag();
					}
					//check if he felt underground 
					if( plr->GetPositionZ() + 10 < m_Unit->GetPositionZ() )
						plr->_Relocate(m_Unit->GetMapId(),m_Unit->GetPosition(),false,false,m_Unit->GetInstanceID());
				}
				//he probably DC-ed. Fight is not fair this way
				else
				{
#ifdef DISQUALIFY_LOGOUT_FIGHTERS
					team1guids[ i ] = 0;
#endif
				}
			}

		uint8 isteam2dead = 1;
		uint8 has_team2_players_online = 0;
		for(uint32 i=0;i<team2_players;i++)
			if( team2guids[ i ] )
			{
				Player *plr = mmgr->GetPlayer( team2guids[ i ] );
				if( plr )
				{
					has_team2_players_online = 1;
					if( plr->isAlive() )
					{
						isteam2dead = 0;
						//set PVP to make sure they did not disable it
						if( !plr->IsPvPFlagged() )
							plr->SetPvPFlag();
					}

					//check if he felt underground 
					if( plr->GetPositionZ() + 10 < m_Unit->GetPositionZ() )
						plr->_Relocate(m_Unit->GetMapId(),m_Unit->GetPosition(),false,false,m_Unit->GetInstanceID());
				}
				//he probably DC-ed. Fight is not fair this way
				else
				{
#ifdef DISQUALIFY_LOGOUT_FIGHTERS
					team2guids[ i ] = 0;
#endif
				}
			}
		
		//draw = both teams dead but they had players online
		if( isteam1dead == 1 && has_team1_players_online == 1 && isteam2dead == 1 && has_team2_players_online == 1)
		{
			m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, SUPERVISOR_ANNOUNCE_TXT_FIGHT_DRAW );
			tournament_status = TS_GATHER_TEAM_INFO;
			Event_team1_win();
			Event_team2_win();
		}
		// team 1 win and team 2 loose
		if( (( isteam1dead == 1 && has_team1_players_online == 1 ) || has_team1_players_online == 0 ) 
			&& 
			( isteam2dead == 0 && has_team2_players_online == 1 )
			)
		{		
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : team1 dead=%u team2 dead=%u team1 online=%u team2 online=%u ",isteam1dead,isteam2dead,has_team1_players_online,has_team2_players_online);
#endif
			sprintf(msg,SUPERVISOR_ANNOUNCE_TXT_FIGHT_WIN,Team2Info.name);
			m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
			tournament_status = TS_GATHER_TEAM_INFO;
			Event_team2_win();
			Event_team1_loose();
		}
		// team 2 win and team 1 loose
		if( (( isteam2dead == 1 && has_team2_players_online == 1 ) ||  has_team2_players_online == 0 )
			&&
			( isteam1dead == 0 && has_team1_players_online == 1 )
			)
		{		
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : team1 dead=%u team2 dead=%u team1 online=%u team2 online=%u ",isteam1dead,isteam2dead,has_team1_players_online,has_team2_players_online);
#endif
			sprintf(msg,SUPERVISOR_ANNOUNCE_TXT_FIGHT_WIN,Team1Info.name);
			m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, msg);
			tournament_status = TS_GATHER_TEAM_INFO;
			Event_team1_win();
			Event_team2_loose();
		}
		//both teams loose because no teams are online
		if( has_team1_players_online == 0 && has_team2_players_online == 0 )
		{
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament DEBUG : team1 dead=%u team2 dead=%u team1 online=%u team2 online=%u ",isteam1dead,isteam2dead,has_team1_players_online,has_team2_players_online);
#endif
			m_Unit->SendChatMessage(SUPERVISOR_ANNOUNCE_TYPE, SUPERVISOR_ANNOUNCE_LANG, SUPERVISOR_ANNOUNCE_TXT_FIGHT_NOONLINE );
			tournament_status = TS_GATHER_TEAM_INFO;
			Event_team1_loose();
			Event_team2_loose();
		}
		//////////////////////////////////////////////
		//periodically check for sudden death event
		//////////////////////////////////////////////
		if( getMSTime() > sudden_death_trigger_stamp )
		{
			sudden_death_trigger_count++;
			sudden_death_trigger_stamp = getMSTime() + SUDDEN_DEATH_REPEAT_TIMER;

			//deal damage to players by directly reducing their health
			MapMgr *mmgr = m_Unit->GetMapMgr();
			if( !mmgr )
				return; //omg wtf ?

			for(uint32 i=0;i<team1_players;i++)
			{
				Player *plr = mmgr->GetPlayer( team1guids[ i ] );
				if( plr && plr->isAlive() )
					m_Unit->DealDamage( plr, sudden_death_trigger_count*SUDDEN_DEATH_DAMAGE, 0, 0, 0, true);
			}

			for(uint32 i=0;i<team2_players;i++)
			{
				Player *plr = mmgr->GetPlayer( team2guids[ i ] );
				if( plr && plr->isAlive() )
					m_Unit->DealDamage( plr, sudden_death_trigger_count*SUDDEN_DEATH_DAMAGE, 0, 0, 0, true);
			}
		}
	}
}

//this NPC is spawned in the midle of the arena to do everything :P
class Tournament_supervisor : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Tournament_supervisor);

	//constructor
    Tournament_supervisor(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
		Tournament_Supervisor *new_interface = new Tournament_Supervisor;
		new_interface->Init(_unit,AITYPE_SOCIAL,MOVEMENTTYPE_NONE);
		_unit->ReplaceAIInterface( (AIInterface *) new_interface );

		//make us friendly
		_unit->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, SUPERVISOR_FACION );
		_unit->_setFaction();

#ifndef _DEBUG
/*		//make him invisible 
		_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, 15435 );
		
		//make us small. Like really small :P
		_unit->SetFloatValue( OBJECT_FIELD_SCALE_X , 0.01f );
		*/
#endif

		//make us imune to every spell we know of :P
		for(uint32 i=0;i<7;i++)
			_unit->SchoolImmunityList[i] = 999;
		for(uint32 i=0;i<31;i++)
			_unit->MechanicsDispels[i] = 999;

		//make him an NPC who can talk
		_unit->SetUInt32Value( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP );

		//make us global 
		tournament_supervisor_guid = _unit->GetGUID();
		tournament_supervisor = _unit;

		//put us in an IDLE state ( funcky )
		_unit->SetUInt32Value(UNIT_FIELD_MAXHEALTH,SUPERVISOR_HP_TO_IDLE);

		//make him impossible to select or attack by normal players
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, _unit->GetUInt32Value( UNIT_FIELD_FLAGS ) | SUPERVISOR_UNIT_FLAGS );

		//global var sets
		strcpy(team1name,"");
		strcpy(team2name,"");
    }
};

void SetupTournamentSupervisor(ScriptMgr * mgr)
{
	//supervize the 
    mgr->register_creature_script(CREATURE_ENTRY_FOR_SPECTATOR_SUPERVIZOR, &Tournament_supervisor::Create);

}
