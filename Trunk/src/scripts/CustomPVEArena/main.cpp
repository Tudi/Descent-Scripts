#include "StdAfx.h"
#include "Setup.h"

//std::list< QuestKillObjective* >	TeamQuestObjectiveList[ CUSTOM_BG_MAX_TEAMS ];
std::list< uint32 >					RewardItemIdsAll;
std::list< QuestKillObjective* >	QuestObjectiveList;

void RegisterCustomPVEArenaHandler(ScriptMgr* mgr)
{
	SetupBuildings( mgr );
	SetupSpells( mgr );
	SetupNPCs( mgr );
	SetupQuests( mgr );
	SetupPlayers( mgr );
	SetupChatCommands( mgr );
}
