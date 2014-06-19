#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"

void OnChat(Player *pPlayer, uint32 type, uint32 lang, const char* message, const char * target)
{

	if ( pPlayer == NULL || message == NULL )
		return;   // wtf error

	//if there is no tournament then what is the point to handle this message ?
	if( tournament_supervisor_guid == 0 )
		return; 

	//only spectators and GMs need to be handled
//	if( !IsPlayerSpectator( pPlayer->GetGUID() ) && !( pPlayer->GetSession() && pPlayer->GetSession()->HasPermissions() ) )
//		return;

	//check if player is a spectator and spamming chats
	//!!!!!! need to finish this later
	if( IsPlayerSpectator( pPlayer->GetGUID() ) && tournament_started && 
		( type == CHAT_MSG_EMOTE || type == CHAT_MSG_SAY || type == CHAT_MSG_CHANNEL )
		)
	{
		pPlayer->GetSession()->m_muted = UNIXTIME + MUTE_SPECTATORS_DISTURBING_FIGHT; 
		return;
	}

	if( message[0] != '#' )
		return; //not for us

	char cmd[ MAX_STRING_LENGTH ],param1[ MAX_STRING_LENGTH ];
	strcpy( cmd, "" );
	strcpy( param1, "" );

	sscanf(message, "%s %s", &cmd, &param1);

#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament Spectator Command DEBUG : command = %s and param %s ",cmd,param1);
#endif

	string viewc;				viewc = "#view";
	string settournamentc;		settournamentc = "#settournamentc"; //set the ID of the tournament. Needs admin knowledge :P
	string kickplayerfromT;		kickplayerfromT = "#kickplayer";
	string leavesp;				leavesp = "#leave";

	//get booted from spectator mode
	if( cmd == leavesp )
		RemoveUnitSpectator( pPlayer , 1);

	//spectator can set viewpoint or if focused on self -> get booted
	if( cmd == viewc )
	{
		param1[ MAX_STRING_LENGTH - 1 ] = 0; //make sure it does not lead to illegal acces
		uint32 param1len = strlen( param1 );
		if( param1len == 0 || param1len > 15 )
		{
			pPlayer->BroadcastMessage( TOURNAMENT_SPECTATOR_TELEPORT_NPC_HELP_VIEW );
			return;
		}
		Player *chr = objmgr.GetPlayer( param1, false );
		if( !chr )
		{
			pPlayer->BroadcastMessage( TOURNAMENT_SPECTATOR_TELEPORT_NPC_VIEW_ER_NOCHAR );
			return;
		}
		//if we want to see us as spectator then we better just go home
		if( pPlayer == chr 
			|| IsPlayerSpectator( chr->GetGUID() ) 
			|| !chr->isInRange( pPlayer, SUPERVIZOR_ARENA_SIZE )
			)
		{
			//remove us from the spectators
			RemoveUnitSpectator( pPlayer , 1);
		}
		else 
		{
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament Spectator Command DEBUG : binding sight to player %s ",chr->GetName() );
#endif
			pPlayer->SetUInt64Value(PLAYER_FARSIGHT, chr->GetGUID() );
		}
	}
	//used by gms to be able to switch the id of ongoing tournament
	if( cmd == settournamentc )
	{
		if( !pPlayer->GetSession() || !pPlayer->GetSession()->HasGMPermissions() )
			return;
		cur_tournament_id = atoi( param1 );
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament Spectator Command DEBUG : set ongoing tournament id to %u ",cur_tournament_id );
#endif
		sprintf(cmd,TOURNAMENT_SPECTATOR_TELEPORT_NPC_SET_TOURNAMENT,cur_tournament_id);
		pPlayer->BroadcastMessage( cmd );
	}
}