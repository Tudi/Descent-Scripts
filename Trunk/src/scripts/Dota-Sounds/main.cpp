#include "StdAfx.h"

/*
1 st Hero to die - First Blood
3 hero kills - Killing Spree
4 hero kills - Dominating
5 hero kills - Unstoppable
6 hero kills - Monster Kill
7 hero kills - Mega Kill
8 hero kills - Wicked Sick
9 hero kills - GodLike!
2 consecutive kills - Double Kill!
3 consecutive kills - Triple Kill
4 consecutive kills - Ultra Kill!
5 consecutive kills - Rampage! 

.admin playall 6762
.admin playall 6761
.admin playall 6760
.admin playall 6759
.admin playall 6538
.admin playall 6537
.admin playall 6534
.admin playall 6533
.admin playall 6532
.admin playall 6488
.admin playall 6487
.admin playall 6485
.admin playall 4714
.admin playall 4679
.admin playall 3380

6762 - Combowhore.wav - WroughtIronDoorOpen.wav
6761 - Dominating.wav - WroughtIronDoorClose.wav
6760 - WhickedSick.wav - WroughtIronDoor02Open.wav
6759 - Unstoppable.wav - WroughtIronDoor02Close.wav
6538 - UltraKill.wav - BlackRockIronDoor01Open.wav
6537 - triple_kill.wav - BlackRockDoors01Open.wav
6534 - Rampage.wav - BlackRockVaultDepositDoor01Open.wav
6533 - Ownage.wav - BlackRockDoorSingleDestroy.wav
6532 - monster_kill.wav - BlackRockDoorSingleOpen.wav
6488 - MegaKill.wav - BlackRockCellDoor01Destroy.wav
6487 - Killing_Spree.wav - BlackRockCellDoor01Open.wav
6485 - HolyShit.wav - BlackRockHugeDoorsClose.wav
4714 - GodLike.wav - MausoleumActiveDoorsClose.wav
4679 - firstblood.wav - MausoleumActiveDoorsOpen.wav
3380 - Double_Kill.wav - DeadmineDoor02Open.wav
3360 - nemesis.wav - DeadmineDoor01Open.wav
3363 - massacre.wav - DeadmineDoor01Destroy.wav
*/

//#define PLAYER_EXTENSION_DOTA_DISABLE			"DotaDisable"
//#define PLAYER_EXTENSION_DOTA_STREAK_EXPIRE		"DotaStreakExpire"
//#define PLAYER_EXTENSION_DOTA_STREAK_COUNTER	"DotaStreakCounter"
//#define PLAYER_EXTENSION_DOTA_KILL_COUNTER		"DotaKillCounter"
#define PLAYER_EXTENSION_DOTA_STREAK_DURATION	30*1000
#define PLAYER_EXTENSION_DOTA_ANTISPAM			1000
#define PLAYER_EXTENSION_DOTA_COUNTER_SCALE		1		//instead of each kill, count each Nth kill

static int DotaExtensionStreakSounds[] =
{
	3380, // Double_Kill 
	6537, // triple_kill
	6538, // UltraKill
	6534, // Rampage
	6533, // Ownage
	6762  // Combowhore
};

static int DotaExtensionKillSounds[] =
{
	4679, // First Blood
	6487, // Killing Spree
	6761, // Dominating
	6759, // Unstoppable
	6532, // Monster Kill
	3360, // nemesis
	6488, // Mega Kill
	3363, // massacre
	6760, // Wicked Sick
	4714 // GodLike
};

void DotaSendSoundToSet(Player *p, uint32 sound_ID)
{
	if(!p->IsInWorld())
	{ 
		return;
	}

	InrangeLoopExitAutoCallback AutoLock;
	InRangePlayerSetRecProt::iterator itr = p->GetInRangePlayerSetBegin( AutoLock );
	InRangePlayerSetRecProt::iterator it_end = p->GetInRangePlayerSetEnd();
	for(; itr != it_end; ++itr)
	{
		int64 *ext = (int64 *)(*itr)->GetExtension( EXTENSION_ID_DOTA_DISABLE );
		if( ext == NULL || *ext != 2 )
		{
			if( (*itr)->GetSession() )
				(*itr)->GetSession()->OutPacket(SMSG_PLAY_SOUND, 4, &sound_ID);
		}
	}
}

void OnPlayerKillDotaSounds(Player *pPlayer, Player *pVictim)
{
	if( pPlayer->m_bg == NULL || pVictim->m_bg == NULL )
		return;
	//do we need to init ?
	int64 *SoundDisable = pPlayer->GetCreateIn64Extension( EXTENSION_ID_DOTA_DISABLE );
	if( *SoundDisable == 0 )
	{
		pPlayer->BroadcastMessage("To disable kill sounds type in chat : #DotaSoundDisable");
		*SoundDisable = 1;
	}
	else if( *SoundDisable == 2 )
		return;

	int64 *StreakExpireStamp = pPlayer->GetCreateIn64Extension( EXTENSION_ID_DOTA_STREAK_EXPIRE );
	int64 *StreakCounter = pPlayer->GetCreateIn64Extension( EXTENSION_ID_DOTA_STREAK_COUNTER );
	int64 *KillCounter = pPlayer->GetCreateIn64Extension( EXTENSION_ID_DOTA_KILL_COUNTER );
	if( GetTickCount() <  *StreakExpireStamp )
	{
		 int64 module_counter = (*StreakCounter) % PLAYER_EXTENSION_DOTA_COUNTER_SCALE;
		 if( module_counter == 0 )
		 {
			 int64 scaled_counter = (*StreakCounter) / PLAYER_EXTENSION_DOTA_COUNTER_SCALE;
			 if( scaled_counter >= sizeof( DotaExtensionStreakSounds ) / sizeof( int ) )
				scaled_counter = sizeof( DotaExtensionStreakSounds ) / sizeof( int ) - 1;
			DotaSendSoundToSet( pPlayer, DotaExtensionStreakSounds[ scaled_counter ] );
		 }
		 *StreakCounter+=1;
		 *KillCounter+=1;
	}
	else
	{
		*StreakCounter = 0;
		 int64 module_counter = (*KillCounter) % PLAYER_EXTENSION_DOTA_COUNTER_SCALE;
		 if( module_counter == 0 )
		 {
			 int64 scaled_counter = (*KillCounter) / PLAYER_EXTENSION_DOTA_COUNTER_SCALE;
			 if( scaled_counter >= sizeof( DotaExtensionKillSounds ) / sizeof( int ) )
				scaled_counter = sizeof( DotaExtensionKillSounds ) / sizeof( int ) - 1;
			DotaSendSoundToSet( pPlayer, DotaExtensionKillSounds[ scaled_counter ] );
		 }
		*KillCounter+=1;
	}
	*StreakExpireStamp = GetTickCount() + PLAYER_EXTENSION_DOTA_STREAK_DURATION;
}

void OnPlayerDeathDotaSounds(Player *pPlayer)
{
	if( pPlayer->m_bg == NULL )
		return;
	int64 *SoundDisable = pPlayer->GetCreateIn64Extension( EXTENSION_ID_DOTA_DISABLE );
	if( *SoundDisable == 0 )
	{
		pPlayer->BroadcastMessage("To disable kill sounds type in chat : #DotaSoundDisable");
		*SoundDisable = 1;
	}
	else if( *SoundDisable == 2 )
		return;
	int64 *StreakCounter = pPlayer->GetCreateIn64Extension( EXTENSION_ID_DOTA_STREAK_COUNTER );
	int64 *KillCounter = pPlayer->GetCreateIn64Extension( EXTENSION_ID_DOTA_KILL_COUNTER );
	*StreakCounter = 0;
	*KillCounter = 0;
}

void OnPlayerChatDotaSounds(Player * pPlayer, uint32 Type, uint32 Lang, const char * Message, const char * Misc)
{
	if( Message == NULL )
		return;
	if( strstr( Message, "#DotaSoundDisable" ) == Message )
	{
		int64 *SoundDisable = pPlayer->GetCreateIn64Extension( EXTENSION_ID_DOTA_DISABLE );
		*SoundDisable = 2;
	}
	else if( strstr( Message, "#DotaSoundEnable" ) == Message )
	{
		int64 *SoundDisable = pPlayer->GetCreateIn64Extension( EXTENSION_ID_DOTA_DISABLE );
		*SoundDisable = 1;
	}
}

void RegisterDotaSounds(ScriptMgr* mgr)
{
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_KILL_PLAYER, &OnPlayerKillDotaSounds );
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_DEATH, &OnPlayerDeathDotaSounds );
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_CHAT, &OnPlayerChatDotaSounds );
}
