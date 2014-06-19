//load config values for the tournament from file. Or not ? :D
//extern FILE *config_file; 

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	The Maze Event supervisor NPC -> he can start, pause the event
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CREATURE_ENTRY_FOR_IGTS_SUPERVIZOR				123462		// creature entry spawned at center of arena
#define SUPERVIZOR_MAX_HEALTH							20000000	// just so we may give him commands based on health
#define SUPERVISOR_UPDATE_INTERVAL						2000		// don't spam updates
//#define WAIT_FOR_LAST_TEAM_TO_JOIN						1*60*1000	// wait this amount of time so last team can group up properly
#define WAIT_FOR_LAST_TEAM_TO_JOIN						4000		// wait this amount of time so last team can group up properly
#define PLAYER_TELEPORT_WAIT							20000		// at each update interval we will try port players to us
#define PLAYER_PREBUFF_WAIT								10000		// time to wait until players realize where they are and buff self / other
#define EVENT_WAIT_TEAM_MATCH							60*60*1000	// the actual fight time
#define EVENT_WAIT_FORCED_RESET							2*60*60*1000// just in case somethign goes wrong we simply reset our NPC

enum SupervizorGossipCommands
{
	SUPERVIZOR_COMMAND_NONE			= 0,
	SUPERVIZOR_COMMAND_FORCESTART	= 1,		// command to force event start as they are
};
#define SUPERVISOR_FACTION								35		// friendly to everyone, not like he could fight
#define MAX_PLAYERS_PER_TEAM							20		// I think it would lag server for more :P
#define MAX_TEAMS_PER_EVENT								10		// I think it would lag server for more :P
																// let them attack each other
#define TEAM_COMMON_UNIT_FLAGS							( UNIT_FLAG_PVP )
																// make him non attackable and preferably non selectable
#define SUPERVISOR_UNIT_FLAGS							( UNIT_FLAG_UNKNOWN_8 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 )

#define SUPERVISOR_ANNOUNCE_TYPE						12		// talk baloon over head
#define SUPERVISOR_ANNOUNCE_LANG						0		// general lang

#define MAX_STRING_LENGTH								1000
#define MAX_EVENT_TEAM_NAME_LEN							10

#define MAX_MOB_SPAWN_TYPES								10
#define MAX_TEAMS_PER_EVENT								10
#define MAX_STARTUP_BUFFS								3
#define MAX_REWARD_ITEM_IDS								3
#define LIFT_PLAYER_WHEN_TELEPORT						5
#define MAX_MAPS_CACHED									10
#define SUPERVISOR_EXTENSION_NAME						"MAZE_EVENT_SUPERVISOR"
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	end 
////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Team_Info
{
//	uint32					id;							//team guid
	uint64					leader_guid;
	uint64					player_guids[ MAX_PLAYERS_PER_TEAM ];
	uint32					player_dmg_contri[ MAX_PLAYERS_PER_TEAM ];		//monitor how much honor they built up while fighting so we may know how fair the fight was
	uint32					player_heal_contri[ MAX_PLAYERS_PER_TEAM ];		//monitor how much honor they built up while fighting so we may know how fair the fight was
	Player					*players_for_local_acces[ MAX_PLAYERS_PER_TEAM ];	//!!!! these are only available on short term. Inside loop
	char					name[ MAX_EVENT_TEAM_NAME_LEN ];
	uint32					is_used;						//list is static, we need to signal if this structure is "NULL"
	float					start_x,	start_y;			//teams might not queue up in the same order as we registered the 
};

struct Maze_Map_Info
{
	char					map_info_text[ MAX_STRING_LENGTH ];
	char					map_name_menu[ MAX_STRING_LENGTH ];
	uint32					H_GO_entry;
	uint32					V_GO_entry;
	float					GO_size;									//this means the ingame size of the scaled GO
	uint32					H_Gate_GO_entry;
	uint32					V_Gate_GO_entry;
	uint32					H_Start_Gate_GO_entry;
	uint32					V_Start_Gate_GO_entry;
	int32					MOB_spawn_entries[ MAX_MOB_SPAWN_TYPES ];
	uint32					GO_spawn_entries[ MAX_MOB_SPAWN_TYPES ];	//representing whatever reason spawns
	float					team_start_x[ MAX_TEAMS_PER_EVENT ];		// yeah ok so i could have used float and pos type :P
	float					team_start_y[ MAX_TEAMS_PER_EVENT ];		// yeah ok so i could have used float and pos type :P
	uint32					start_buffs[ MAX_STARTUP_BUFFS ];			// casted with forced duration buffs. Maybe we want to allow them to run with greater speed ?
	uint32					reward_item_id[ MAX_REWARD_ITEM_IDS ];
	uint32					reward_item_id_count[ MAX_REWARD_ITEM_IDS ];
	uint32					reward_gold[ MAX_REWARD_ITEM_IDS ];
	uint32					reward_buffs[ MAX_REWARD_ITEM_IDS ];
	uint32					max_players_in_team;						//we have same structure for teams so we need to signal their size
	uint32					max_teams_per_event;
	uint32					map_size_x,map_size_y;						//player is an active event participant if is inside this circle
	float					m_center_x,m_center_y;						//probably where the supervisor is spawned
	float					m_goal_x,m_goal_y;							//probably where the supervisor is spawned
	float					center_x,center_y;							//probably where the supervisor is spawned
	float					start_x,start_y;
	float					goal_x,goal_y;								//probably where the supervisor is spawned
	float					z_adjust;
	//should convert these into lists. I realize there are less spawns then map size...next
	int32					*entry_map;									//entry map that needs to be spawned on event pick
	uint64					*spawn_map;									//guid map containing the spawns we made
	uint32					FFA_enabled;								//force players to have FFA flag on them
};

struct Selected_Event_info
{
	Team_Info				teams_registered[ MAX_TEAMS_PER_EVENT ];	//probably 10
	uint32					status;					//like : wait, preparation, fight
	uint32					next_state_change_stamp;//event states change based on state stamps mostly
	bool					is_spawned;				//do not start event if it is not yet started
};

enum Event_Status
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

#define							WRITE_DEBUG_INFO_TO_LOG			1
bool	IsInrangeSQ( Object *t1, Object *t2,float sq_r); //check distance of 2 objects
bool	IsInrangeSQ( float x1, float y1,float x2, float y2, float sq_r); //check distance of 2 objects

