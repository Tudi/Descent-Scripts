#ifndef ARENA_SPECTATOR_SCRIPTS_SETUP_H
#define ARENA_SPECTATOR_SCRIPTS_SETUP_H

void RegisterCustomPVEArenaHandler(ScriptMgr * mgr);

void SetupBuildings( ScriptMgr* mgr );
void SetupSpells( ScriptMgr* mgr );
void SetupNPCs( ScriptMgr* mgr );
void SetupQuests( ScriptMgr* mgr );
void SetupPlayers( ScriptMgr* mgr );
void SetupChatCommands( ScriptMgr* mgr );

void ConvertToStaticSpawn( Creature *c );
int GetCreatureGroupFromEntry( uint32 Entry );
void ResetPlayerForTeam( Player * pPlayer, uint8 Team );

#define CUSTOM_BG_MAP_ID				37	// Azshara Crater
#define CUSTOM_BG_MAX_TEAMS				2
#define	BROADCAST_MAX_MSG_LEN			120
#define CUSTOM_BG_FACTION_ALLIANCE		1
#define CUSTOM_BG_FACTION_HORDE			2
#define RESPAWN_MAX_WAIT				60000
#define PLAYER_WIN_CURRENCY_TYPE		390
#define PLAYER_WIN_CURRENCY_AMT			500
#define EXPECTED_NPC_KILL_TIME			20000
#define EXPECTED_NPC_KILL_PLAYER_LIFE	4

struct SpawnCord
{
	float x,y,z,o;
};

struct QuestKillObjective
{
	uint32		Group;
	uint32		Entry;
	uint32		KillCountRequired;
	uint32		KillCountNow;
	uint32		KillCountTotal;
	char		Name[BROADCAST_MAX_MSG_LEN];
	char		Description[BROADCAST_MAX_MSG_LEN];
	SpawnCord	Spawn;

};

enum QuestObjectiveGroups
{
	QO_BUILD_BANK		= 1,
	QO_BUILD_VENDOR		= 2,
	QO_BUILD_DEFENSE	= 3,
	QO_BUILD_HEALER		= 4,

	QO_BUILD_TEAM_SHIFTER1	=	8,	//multiply this by player team id : ex : 1, 11, 21...
	QO_BUILD_TEAM_SHIFTER2	=	16,	//multiply this by player team id : ex : 1, 11, 21...
};

//extern std::list< QuestKillObjective* >	TeamQuestObjectiveList[ CUSTOM_BG_MAX_TEAMS ];
extern std::list< QuestKillObjective* >		QuestObjectiveList;
extern std::list< uint32 >					RewardItemIdsAll;	//need to remove these when player joins the game

uint32 GetTeamAnyMask( Object *o );

#endif