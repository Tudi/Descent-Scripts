//load config values for the tournament from file. Or not ? :D
//extern FILE *config_file; 

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	The Tournament supervisor NPC -> Spectators
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define		SPECTATOR_AURA_ID								66350
#define		SET_SPECTATOR_UNIT_FLAGS						( UNIT_FLAG_UNKNOWN_8 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_SILENCED | UNIT_FLAG_PACIFIED | UNIT_FLAG_STUNNED | UNIT_FLAG_COMBAT | UNIT_FLAG_MOUNTED_TAXI | UNIT_FLAG_DISARMED | UNIT_FLAG_CONFUSED | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_LOCK_PLAYER )
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	The Tournament supervisor NPC -> Spectators end
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	The Tournament supervisor NPC -> he can start, pause the tournament
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CREATURE_ENTRY_FOR_IGTS_SUPERVIZOR				123459		// creature entry spawned at center of arena
#define SUPERVIZOR_MAX_HEALTH							20000000	// just so we may give him commands based on health
#define SUPERVISOR_UPDATE_INTERVAL						2000		// don't spam updates
//#define WAIT_FOR_LAST_TEAM_TO_JOIN						1*60*1000	// wait this amount of time so last team can group up properly
#define WAIT_FOR_LAST_TEAM_TO_JOIN						1000		// wait this amount of time so last team can group up properly
#define PLAYER_TELEPORT_WAIT							20000		// at each update interval we will try port players to us
#define PLAYER_PREBUFF_WAIT								10000		// time to wait until players realize where they are and buff self / other
#define TOURNAMENT_WAIT_TEAM_MATCH						10*60000	// the actual fight time

//pick one of the 2
//#define TOURNAMENT_TYPE_WEAKEST_VS_WEAKEST							//winner teams go to next round. 1 team is eliminated / round
#define TOURNAMENT_TYPE_LADDER										//if team looses then he is eliminated. Remaining team wins

enum SupervizorGossipCommands
{
	SUPERVIZOR_COMMAND_NONE			= 0,
	SUPERVIZOR_COMMAND_FORCESTART	= 1,		// command to force tournaments start as they are
};
#define SUPERVISOR_FACION								35		// friendly to everyone, not like he could fight
#define MAX_PLAYERS_PER_TEAM							5		// I think it would lag server for more :P
#define MAX_TEAMS_PER_TOURNAMENT						10		// I think it would lag server for more :P
#define TEAMS_TO_FIGHT_AT_SAME_TIME						2		// not sure it is wise to put more then 2

																// let them attack each other
#define TEAM_COMMON_UNIT_FLAGS							( UNIT_FLAG_PVP )
																// make him non attackable and preferably non selectable
#define SUPERVISOR_UNIT_FLAGS							( UNIT_FLAG_UNKNOWN_8 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 )

#define SUPERVISOR_ANNOUNCE_TYPE						12		// talk baloon over head
#define SUPERVISOR_ANNOUNCE_LANG						0		// general lang

#define MAX_STRING_LENGTH								1000
#define MAX_TOURNAMENT_TEAM_NAME_LEN					10

// the idea of sudden death is to not let even matches take froever
// after X minutes we start dealing damage equally to all players
#define SUDDEN_DEATH_TIMER								(TOURNAMENT_WAIT_TEAM_MATCH * 75 / 100) // if there is no progress for the match in this amount of time we will start killing people with a spell
#define SUDDEN_DEATH_REPEAT_TIMER						(TOURNAMENT_WAIT_TEAM_MATCH / 95)		// maybe they are smart and can heal the dmg spell so we start to stack it
#define SUDDEN_DEATH_DAMAGE								100										// this damage sums up
#define SUDDEN_DEATH_SPELL_ID							52437									// whatever spell ID or none, this is just for combat log visuals
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	end 
////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Team_Info
{
	uint32					id;							//team guid
	uint64					leader_guid;
	uint64					player_guids[ MAX_PLAYERS_PER_TEAM ];
	uint32					player_dmg_contri[ MAX_PLAYERS_PER_TEAM ];		//monitor how much honor they built up while fighting so we may know how fair the fight was
	uint32					player_heal_contri[ MAX_PLAYERS_PER_TEAM ];		//monitor how much honor they built up while fighting so we may know how fair the fight was
	Player					*players_for_local_acces[ MAX_PLAYERS_PER_TEAM ];	//!!!! these are only available on short term. Inside loop
	uint32					matches_won_total,matches_played_total;
	char					name[ MAX_TOURNAMENT_TEAM_NAME_LEN ];
	uint32					is_used;				//list is static, we need to signal if this structure is "NULL"
};

struct Tournament_Info
{
	uint32					id;						//tournament guid
	uint32					type;					//refers mostly to team max size
	Team_Info				teams_registered[ MAX_TEAMS_PER_TOURNAMENT ];	//probably 10
	uint32					map_id;				
	float					center_x,center_y,center_z;
	float					arena_range_sq;			//kinda limited by visual range !
	uint32					status;					//like : wait, preparation, fight
	uint32					fighting_teams[ TEAMS_TO_FIGHT_AT_SAME_TIME ];
	uint32					sudden_death_stamp;		//avoid infinit fights
	uint32					sudden_death_counter;	//dmg sums up after each hit
	uint32					max_players_in_team;	//we have same structure for teams so we need to signal their size
	uint32					next_state_change_stamp;//tournament states change based on state stamps mostly
	uint32					ladder_step;			//elimination type of tournaments
	uint32					MinLevel,MaxLevel;		//just so we may show for new teams
};

//do not change values as they are used as indexes
enum TournamentTypes
{
	TOURNAMENT_TYPE_2_VS_2 = 0,
	TOURNAMENT_TYPE_3_VS_3 = 1,
	TOURNAMENT_TYPE_4_VS_4 = 2,
	TOURNAMENT_TYPE_5_VS_5 = 3,
	TOURNAMENT_TYPE_COUNT
};

static char TournamentTypeNames[ TOURNAMENT_TYPE_COUNT ][ 30 ] = { "2 VS 2", "3 VS 3","4 VS 4","5 VS 5" };

enum Tournament_Status
{
	TS_NOT_STARTED						, //don't really care what is happening around us
	TS_WAITING_LAST_TEAM_JOIN			, //all teams have members but maybe not all have full member count
	TS_WAITING_LAST_TEAM_JOIN_FORCED	, //all teams have members but maybe not all have full member count
	TS_TELEPORT_PLAYERS					, //gather players from around the world
	TS_TELEPORT_WAIT_PLAYERS			, //might take up to 10 seconds to get players to load map
	TS_PREPARE_PLAYERS					, //set factions and flags
	TS_PREPARE_COUNTDOWN_6				, // 4 seconds remaining
	TS_PREPARE_COUNTDOWN_4				, // 2 seconds remaining
	TS_PREPARE_COUNTDOWN_2				, // 0 seconds remaining
	TS_WAIT_FOR_FIGHT					, //players fight each other until one of teams have only dead/no players
};

#define				WRITE_DEBUG_INFO_TO_LOG			1
//global variables are bad. We promis to ourself that we will have only 1 tournament per server !
extern	uint32		cur_tournament_id;				//guid
extern	uint32		cur_team_id;					//guid
extern	uint32		global_update_next_stamp;		//do not spam updates in case there are multiple supervizor spawns

extern	Tournament_Info			Tournaments[ TOURNAMENT_TYPE_COUNT ];
extern	Mutex					UpdateMutex;
extern	uint32					teamsize_limitations[ TOURNAMENT_TYPE_COUNT ];
extern	uint32					debug_commands;

	
bool	IsInrangeSQ( Object *t1, Object *t2,float sq_r); //check distance of 2 objects
void	CreateTournametSpectatorAura(ScriptMgr * mgr);
bool	IsUnitInsideAnyBattleRing(uint32 map, float x, float y);
bool	IsUnitInsideBattleRing(uint32 TournamentType, uint32 map, float x, float y);
void	UpdateTournaments( );
uint32	GetTeamMemberCount(uint32 TournamentType,uint32 team);
void	PickNext2TeamsToFight( uint32 TournamentID );
void	HandleTournamentWin( uint32 TournamentID, uint32 teamID);
void	ResetTournamentForNew( uint32 TournamentID );
void	BroadcastToTournament( uint32 TournamentID, const char *what );
void	BroadcastToTournamentTeam( uint32 TournamentID, uint32 TeamID, const char *what );
bool	IsPlayerSpectator( Player *plr );
void	RemoveTournamentStatesFromPlayers( uint32 TournamentID );