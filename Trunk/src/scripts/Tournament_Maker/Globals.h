//load config values for the tournament from file. Or not ? :D
//extern FILE *config_file; 

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	The NPC that will teleport players to a special spot so they can watch the match and do nothing meanwhile
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//menu is added to a mob with entry ...
#define TOURNAMENT_SPECTATOR_TELPORT_NPC_ENTRY			5

//tournament location is fixed maybe later read from file
//this location is only a buffer from where the tournament supervisor will pick them up and place them where can supervise them
/*
#define TOURNAMENT_SPECTATOR_TEL_MAP					530			
#define TOURNAMENT_SPECTATOR_TEL_X						-1758
#define TOURNAMENT_SPECTATOR_TEL_Y						5363
#define TOURNAMENT_SPECTATOR_TEL_Z						-12
*/

//port them all to shatrath ?
#define TOURNAMENT_SPECTATOR_RET_MAP					((uint32)(530))
#define TOURNAMENT_SPECTATOR_RET_X						((float)(-1872))
#define TOURNAMENT_SPECTATOR_RET_Y						((float)(5407))
#define TOURNAMENT_SPECTATOR_RET_Z						((float)(-12))

//you can see this at NPC as menu option to port player
#define TOURNAMENT_SPECTATOR_TELEPORT_NPC_GOSSIP		"I wish to be a tournament spectator"
#define TOURNAMENT_SPECTATOR_TELEPORT_NPC_REFUSE_SEAT	"Sorry, no more free seats are available atm"
#define TOURNAMENT_SPECTATOR_TELEPORT_NPC_REFUSE_TOURNAMENT	"Sorry, there is no ongoing tournament atm"
#define TOURNAMENT_SPECTATOR_TELEPORT_NPC_HELP_VIEW		 "Do not use public chat or you get muted. Type in private chat : #view <playername> to switch focus to a player. Focus on yourself to leave spectator mode"
#define TOURNAMENT_SPECTATOR_TELEPORT_NPC_VIEW_ER_NOCHAR "Character is not good to bind view"
#define TOURNAMENT_SPECTATOR_TELEPORT_NPC_SET_TOURNAMENT "Tournament id set to %u"

#define NUMBER_OF_SPECTATOR_SEETS						100

#define SET_SPECTATOR_UNIT_FLAGS						( UNIT_FLAG_UNKNOWN_8 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_SILENCED | UNIT_FLAG_PACIFIED | UNIT_FLAG_STUNNED | UNIT_FLAG_COMBAT | UNIT_FLAG_MOUNTED_TAXI | UNIT_FLAG_DISARMED | UNIT_FLAG_CONFUSED | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_LOCK_PLAYER )

#define SPECTATOR_SEET_REFRESH_INTERVAL					30000
#define MUTE_SPECTATORS_DISTURBING_FIGHT				5*60	//value is give in seconds and not milliseconds

//#define SPECTATOR_AURA_ID								55944
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	end 
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	The Tournament supervisor NPC -> he can start, pause the tournament
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CREATURE_ENTRY_FOR_SPECTATOR_SUPERVIZOR			4		// creature entry spawned at center of arena
#define SUPERVISOR_UPDATE_INTERVAL						2000	// don't spam updates
#define PLAYER_TELEPORT_RETRY_CYCLES_COUNT_MAX			10		// at each update interval we will try port players to us
#define PLAYER_TELEPORT_WAIT_CYCLES_COUNT_MAX			6		// at each update interval we will try port players to us
#define PLAYER_PREBUFF_UPDATE_CYCLES					7		// time to wait until players realize where they are and buff self / other

#define SUPERVISOR_FACION								35		// friendly to everyone, not like he could fight
#define MAX_PLAYERS_PER_TEAM							40		// I think it would lag server for more :P

#define PLAYER_FACTION_TEAM1_BUFF						35		//	some noobs attack each other while buffing. Prevent that
#define PLAYER_FACTION_TEAM2_BUFF						35		//	some noobs attack each other while buffing. Prevent that
#define PLAYER_FACTION_TEAM1							1		//	make sure they are friendly even if they are not same race
#define PLAYER_FACTION_TEAM2							5		//	make sure they are friendly even if they are not same race

															// let them attack each other
#define TEAM_COMMON_UNIT_FLAGS							( UNIT_FLAG_PVP )
															// make him non attackable and preferably non selectable
#define SUPERVISOR_UNIT_FLAGS							( UNIT_FLAG_UNKNOWN_8 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 )

#define SUPERVISOR_ANNOUNCE_TYPE						12		// talk baloon over head
#define SUPERVISOR_ANNOUNCE_LANG						0		// general lang

#define SUPERVISOR_ANNOUNCE_TXT_START_ROUND				"Tournament Round %u is starting"
#define SUPERVISOR_ANNOUNCE_TXT_START_MATCH				"Team '%s' VS Team '%s' match will start"
#define SUPERVISOR_ANNOUNCE_TXT_TEAM_STATS				"Team '%s' has %u victories"
#define SUPERVISOR_ANNOUNCE_TXT_WAIT_PORT				"Starting to search for players ingame. Might take %u seconds"
#define SUPERVISOR_ANNOUNCE_TXT_TEAM_PLAYERS_ONLINE		"Team '%s' has %u players available for fight"
#define SUPERVISOR_ANNOUNCE_TXT_PLAYERS_BUFF			"Players, group up and buff each other"
#define SUPERVISOR_ANNOUNCE_TXT_FIGHT_START				"Let the fight begin"
#define SUPERVISOR_WARNING_TXT_OUT_OF_ARENA				"Warning : Return to the arena or you will be kicked. Warnings left %u."
#define SUPERVISOR_ANNOUNCE_TXT_FIGHT_START				"Let the fight begin"
#define SUPERVISOR_ANNOUNCE_TXT_FIGHT_WIN				"Team '%s' has won the match"
#define SUPERVISOR_ANNOUNCE_TXT_FIGHT_DRAW				"Match ended in a draw. Both teams advance to next stage"
#define SUPERVISOR_ANNOUNCE_TXT_FIGHT_NOONLINE			"Match ended in a draw. Both teams loose as they have no players online"
#define SUPERVISOR_ANNOUNCE_TXT_TOURNAMENT_WIN			"Team %s has won the tournament. Yaaay for them. You will get the rewards by mail."
#define SUPERVISOR_ANNOUNCE_TXT_START_TIMER				"Seconds until fight : %u."

#define SUPERVISOR_GOSSIPGM_MENU_START					"Start/Restart tournament match"
#define SUPERVISOR_GOSSIPGM_MENU_START_AN				"GM Resumed the tournament"
#define SUPERVISOR_GOSSIPGM_MENU_ABORT					"Abort tournament match"
//#define SUPERVISOR_GOSSIPGM_MENU_ABORT_AN				"GM Aborted the tournament"
#define SUPERVISOR_GOSSIPGM_MENU_FORCE_LOOSE1			"Force Team '%s' to loose"
#define SUPERVISOR_GOSSIPGM_MENU_FORCE_LOOSE2			"Force Team '%s' to loose"
#define SUPERVISOR_GOSSIPGM_MENU_LOOSE_AN				"GM forced team %s to loose"
#define SUPERVISOR_GOSSIPGM_MENU_FORCE_WIN1				"Force Team '%s' to win"
#define SUPERVISOR_GOSSIPGM_MENU_FORCE_WIN2				"Force Team '%s' to win"
#define SUPERVISOR_GOSSIPGM_MENU_WIN_AN					"GM forced team %s to win"
#define SUPERVISOR_GOSSIPGM_MENU_FORCE_KICK1			"Force Team '%s' to quit tournament"
#define SUPERVISOR_GOSSIPGM_MENU_FORCE_KICK2			"Force Team '%s' to quit tournament"
#define SUPERVISOR_GOSSIPGM_MENU_KICK_AN				"GM forced team '%s' to loose"


#define MAX_STRING_LENGTH								1000

//THERE IS NO REASON FOR YOU TO CHANGE THESE
#define SUPERVISOR_HP_TO_IDLE							1000
#define SUPERVISOR_HP_TO_NO_NEW_COMMAND					1001
#define SUPERVISOR_HP_TO_START							2000
#define SUPERVISOR_HP_TO_ABORT							3000
#define SUPERVISOR_HP_TO_LOOSE1							4000
#define SUPERVISOR_HP_TO_LOOSE2							5000
#define SUPERVISOR_HP_TO_WIN1							6000
#define SUPERVISOR_HP_TO_WIN2							7000
#define SUPERVISOR_HP_TO_KICK1							8000
#define SUPERVISOR_HP_TO_KICK2							9000

#define SUPERVIZOR_ARENA_SIZE							60
#define SUPERVIZOR_ARENA_SIZE_SQUARE					(SUPERVIZOR_ARENA_SIZE*SUPERVIZOR_ARENA_SIZE)
#define SUPERVIZOR_WARNINGS_FOR_EXITED_ARENA			10

// the idea of sudden death is to not let even matches take froever
// after X minutes we start dealing damage equally to all players
#define SUDDEN_DEATH_TIMER								(1000*60*10) // if there is no progress for the match in this amount of time we will start killing people with a spell
#define SUDDEN_DEATH_REPEAT_TIMER						(1000*10*1)  // maybe they are smart and can heal the dmg spell so we start to stack it
#define SUDDEN_DEATH_DAMAGE								100			 // afeter

// disqualify players that logout while fighting 
#define DISQUALIFY_LOGOUT_FIGHTERS						
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	end 
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define		WRITE_DEBUG_INFO_TO_LOG			1
//global variables are bad. We promis to ourself that we will have only 1 tournament per server !
extern	uint32		tournament_supervisor_guid; //we will bind vision to him
extern	Unit		*tournament_supervisor;
extern	uint64		spectator_guids[ NUMBER_OF_SPECTATOR_SEETS ];
extern	uint32		spectator_seet_refresh_stamp;
extern	char		team1name[ MAX_STRING_LENGTH ];
extern	char		team2name[ MAX_STRING_LENGTH ];
extern	uint32		cur_tournament_id;	//can have only 1 tournament ongoing per realm!
extern	uint32		tournament_started;	// only have spectators if there is a tournament ongoing

void	MakeUnitSpectator(Unit *target, uint8 porttofight);
void	RemoveUnitSpectator(Unit *target, uint8 porthome);
void	RefreshSpectators(); //check seets to see if players left seet or not
uint8	IsPlayerSpectator(uint32 pguid); //check if the given guid is listed as spectator
bool	IsInrangeSQ( Object *t1, Object *t2,float sq_r); //check distance of 2 objects