#include "StdAfx.h"
#include "Setup.h"

inline char LowerCase( char in )
{
	if( in <= 'Z' && in >= 'A' )
		return in + ('a'-'A');
	return in;
}

bool CaseInsensitiveParamCheck( char *src, char *param, int maxlen = 255 )
{
	int i;
	for( i=0;i<maxlen && src[i] != 0 && param[i] != 0;i++)
		if( LowerCase(src[i]) != LowerCase(param[i]) )
			return false;
	//we checked the whole param without finding a mismatch
	if( param[i] == 0 )
		return true;
	return false;
}

void OnPlayerChat( Player *pPlayer, uint32 Type, uint32 Lang, const char * Message, const char * Misc)
{
	if( Message == NULL )
		return;
	if( CaseInsensitiveParamCheck( (char*)Message, "#objectives" ) == true )
	{
		std::set< uint32 > AlreadyPrinted;
		uint32 MyTeamMask = GetTeamAnyMask( pPlayer );
		std::list< QuestKillObjective* >::iterator itr;
		for( itr = QuestObjectiveList.begin(); itr != QuestObjectiveList.end(); itr++ )
		{
			if( AlreadyPrinted.find( (*itr)->Group ) != AlreadyPrinted.end() )
				continue;
			AlreadyPrinted.insert( (*itr)->Group );

			if( ( (*itr)->Group & MyTeamMask ) != MyTeamMask )
				continue;

			if( pPlayer->GetMapMgr()->GetCreatureByEntry( (*itr)->Entry ) != NULL )
				continue;

			pPlayer->BroadcastMessage( "%s. Status : %d / %d. %s ", (*itr)->Name, (*itr)->KillCountNow, (*itr)->KillCountRequired, (*itr)->Description );
		}
	}
}

void SetupChatCommands( ScriptMgr* mgr )
{
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_CHAT, &OnPlayerChat );
}