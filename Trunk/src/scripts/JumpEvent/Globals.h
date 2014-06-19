//load config values for the tournament from file. Or not ? :D
//extern FILE *config_file; 

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	The Tournament supervisor NPC -> he can start, pause the tournament
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CREATURE_ENTRY_FOR_SUPERVIZOR					4		// creature entry spawned at center of arena
#define SUPERVISOR_FACION								35		// friendly to everyone, not like he could fight
																// make him non attackable and preferably non selectable
#define SUPERVISOR_UNIT_FLAGS							( UNIT_FLAG_UNKNOWN_8 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 )
#define MAX_STRING_LENGTH								1000
#define MAX_NUMBER_OF_CONCURENT_JUMP_EVENT_HOLDERS		100		//should be plenty
#define MAX_EVENT_DURATION								15*60*1000
#define MAX_JUMPPADS_IN_EVENT							60
#define MIN_JUMPPADS_IN_EVENT							10
#define MAX_JUMPPAD_DISTANCE_FROM_LAST					4
#define MAX_JUMPPAD_DISTANCE_SINCE_SOURCE				10
#define JUMP_PAD_GO_ENTRY								12345
#define JUMP_PAD_ACCEPTED_PLAYER_POSITION_ERROR			1.5f	//player jumps on pad and will not stand exactly on middle
																//depends on GO size
#define SUPERVIZOR_MAX_HEALTH							9999999999

enum JumpEventStates
{
	JES_NOTHING_INTELIGENT,
	JES_WAITING_TO_START,		//since player pushed the NPC gossip menu and actually started spawning the pads
	JES_WAITING_ON_PLAYER_MOVE,	//waiting for player to finish the event
};
/*
enum JumpPadState
{
	JPS_NOT_SPAWNED,
	JPS_WAITING_TO_TRIGGER,
	JPS_CLEARED,
};*/

//holds data that can identify a player that is present to some specific Jump Event holder
struct JumpEventHolder
{
	uint64			PlayerGUID;			//the player
	uint64			SupervizorNPC;		//the supervizor
	uint32			EventExpireStamp;	//just so we can quick check for possible bugs of event getting stuck
	JumpEventStates	EventState;			//started or finished
};

//we save and load our progress into a special file type that we may load later
struct FileLogRecord
{
	uint64	PlayerGUID;			//the guy who managed to do this
	char	Name[15];			//store name for fast lookup
	uint32	UnixRecordStamp;	//when he managed to achieve this. Unix means time is in seconds and not MS
};

struct JumpPadStore
{
	float			x,y,z;
	GameObject		*pad;
//	JumpPadState	state;
};
	
//kinda somthing to link NPC with player
extern JumpEventHolder	JumpEventHolders[ MAX_NUMBER_OF_CONCURENT_JUMP_EVENT_HOLDERS ];
//global variables are bad. We promis to ourself that we will have only 1 tournament per server !
extern	Mutex			CreateLock;

//load saved records
void	LoadLogs();
//save current record list. Needs to have integrated spam protection
void	SaveLogs();
//when player pushes gossip menu to start event
void	StartJumpEvent(Player *Plr, uint64 NPCGUID);
//early end a jump event due to player request
void	EarlyEndJumpEvent(Player *Plr, uint64 NPCGUID);
//called when an event is won
void	Event_Reward_Winner_Player(Player *Plr,uint32 intime);
