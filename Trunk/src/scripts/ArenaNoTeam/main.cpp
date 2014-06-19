#include "StdAfx.h"

#define NPC_ID_ARENA_NOTEAM_10V10		24
#define NPC_ID_ARENA_NOTEAM_5V5			25
#define NPC_ID_ARENA_NOTEAM_3V3			26
#define NPC_ID_ARENA_NOTEAM_2V2			27

#define MAX_PLAYERS_QUEUED				400	
#define CACHE_REFRESH_INTERVAL			10000

enum NoTeamQueuType
{
	QUEUE_TYPE_10V10	= 0,
	QUEUE_TYPE_5V5		= 1,
	QUEUE_TYPE_3V3		= 2,
	QUEUE_TYPE_2V2		= 3,
	QUEUE_TPE_COUNT		= 4,
};

/*
insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('24','10vs10 Arena queue','play me !','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('24','85','85','35','100000','100000','0','2.0','1','1000','0','4784','6219','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('25','5vs5 Arena queue','play me !','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('25','85','85','35','100000','100000','0','2.0','1','1000','0','4784','6219','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('26','3vs3 Arena queue','play me !','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('26','85','85','35','100000','100000','0','2.0','1','1000','0','4784','6219','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('27','2vs2 Arena queue','play me !','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('27','85','85','35','100000','100000','0','2.0','1','1000','0','4784','6219','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');
*/

class SharedQueueStore
{
public:
	SharedQueueStore()
	{
	}
	~SharedQueueStore()
	{
		QueueType = -1;
	}
	void AddRef()
	{
		RefCounter++;
	}
	void DecRef()
	{
		RefCounter--;
		if( RefCounter == 0 )
			delete this;
	}
	uint64		PlayerGuids[MAX_PLAYERS_QUEUED];
//	uint32		PlayerGearScore[MAX_PLAYERS_QUEUED];	//do not consider very low geared players ?
	uint32		GearScoreSum;
	uint32		QueueType;
	uint32		CacheNextUpdateStamp;
	uint32		TeamPlayers[2];
	uint32		RefCounter;
	uint8		HasPeriodicUpdate;
};

SharedQueueStore *SharedQueues[QUEUE_TPE_COUNT];

bool IsPlayerQueued( SharedQueueStore *Queue, uint64 Guid )
{
	for( uint32 i=0;i<MAX_PLAYERS_QUEUED;i++)
		if( Queue->PlayerGuids[i] == Guid )
			return true;
	return false;
}
uint8 QueueAddPlayer( SharedQueueStore *Queue, uint32 Guid )
{
	if( IsPlayerQueued( Queue, Guid ) == true )
		return 2;
	for( uint32 i=0;i<MAX_PLAYERS_QUEUED;i++)
		if( Queue->PlayerGuids[i] == 0 )
		{
			Queue->PlayerGuids[i] = Guid;
			return 1;
		}
	return 0;
}
uint8 QueueRemPlayer( SharedQueueStore *Queue, uint32 Guid )
{
	if( IsPlayerQueued( Queue, Guid ) == false )
		return 2;
	for( uint32 i=0;i<MAX_PLAYERS_QUEUED;i++)
		if( Queue->PlayerGuids[i] == Guid )
		{
			Queue->PlayerGuids[i] = 0;
			return 1;
		}
	return 0;
}

void RefreshQueueStatus( SharedQueueStore *Queue, bool Forced )
{
	if( Queue->CacheNextUpdateStamp > GetTickCount() && Forced == false )
		return;
	Queue->CacheNextUpdateStamp = CACHE_REFRESH_INTERVAL + GetTickCount();

	uint64 PlayerGearScore[MAX_PLAYERS_QUEUED];
	memset(	PlayerGearScore, 0, sizeof( PlayerGearScore ) );
	memset( Queue->TeamPlayers, 0, sizeof( Queue->TeamPlayers ) );
	Queue->GearScoreSum = 0;
	for( uint32 i=0;i<MAX_PLAYERS_QUEUED;i++)
		if( Queue->PlayerGuids[i] != 0 )
		{
			Player *p = objmgr.GetPlayer( Queue->PlayerGuids[i] );
			//player logged out
			if( p == NULL )
			{
				Queue->PlayerGuids[i] = 0;
				continue;
			}
			//player is busy now
			if( p->m_bg )
				continue;
			uint32 PlayerTeam = p->GetTeamInitial();
			Queue->TeamPlayers[ PlayerTeam ]++;
			PlayerGearScore[ i ] = p->LFG_GetGearScore();
			Queue->GearScoreSum += PlayerGearScore[ i ];
		}
	//sort by gearscore. Weaker players should face possible weakest. If 20 horde queue, then 10 A will pick easiest 10 horde
	for( uint32 i=0;i<MAX_PLAYERS_QUEUED;i++)
		for( uint32 j=i+1;j<MAX_PLAYERS_QUEUED;j++)
			if( PlayerGearScore[ i ] < PlayerGearScore[ j ] )
			{
				uint64 T = Queue->PlayerGuids[ i ];
				Queue->PlayerGuids[ i ] = Queue->PlayerGuids[ j ];
				Queue->PlayerGuids[ j ] = T;

				T = PlayerGearScore[ i ];
				PlayerGearScore[ i ] = PlayerGearScore[ j ];
				PlayerGearScore[ j ] = T;
			}
}

void TryBuildTeam( SharedQueueStore *Queue )
{
	uint32 RequiredMinPlayers;
	if( Queue->QueueType == QUEUE_TYPE_10V10 )
		RequiredMinPlayers = 10;
	else if( Queue->QueueType == QUEUE_TYPE_5V5 )
		RequiredMinPlayers = 5;
	else if( Queue->QueueType == QUEUE_TYPE_3V3 )
		RequiredMinPlayers = 3;
	else if( Queue->QueueType == QUEUE_TYPE_2V2 )
		RequiredMinPlayers = 2;
	else
		return;
#ifdef _DEBUG
	RequiredMinPlayers = 1;
#endif
	//check if we have available players to create the teams
	RefreshQueueStatus( Queue, false );
	if( Queue->TeamPlayers[ 0 ] < RequiredMinPlayers || Queue->TeamPlayers[ 1 ] < RequiredMinPlayers )
		return;
	//create instance
	CBattleground *a = BattlegroundManager.CreateInstance( BATTLEGROUND_ARENA_5V5, MAX_LEVEL_GROUP );
	if ( a == NULL )
		return;
	a->m_SkipRewards = true;
	a->m_DenyNewJoins = true;
	a->GetGroup(0)->ExpandToRaid( true );
	a->GetGroup(1)->ExpandToRaid( true );
	a->m_ScriptCPReward = 100;
	if( Queue->QueueType == QUEUE_TYPE_10V10 )
		a->m_ScriptCPReward = 100;
	else if( Queue->QueueType == QUEUE_TYPE_5V5 )
		a->m_ScriptCPReward = 80;
	else if( Queue->QueueType == QUEUE_TYPE_3V3 )
		a->m_ScriptCPReward = 60;
	else if( Queue->QueueType == QUEUE_TYPE_2V2 )
		a->m_ScriptCPReward = 40;
	//queue players to the instance
	uint32 TeamPlayersAdded[2];
	memset( TeamPlayersAdded, 0, sizeof( TeamPlayersAdded ) );
	for( uint32 i=0;i<MAX_PLAYERS_QUEUED;i++)
		if( Queue->PlayerGuids[i] != 0 )
		{
			Player *p = objmgr.GetPlayer( Queue->PlayerGuids[i] );
			//player logged out
			if( p == NULL )
			{
				Queue->PlayerGuids[i] = 0;
				continue;
			}
			//player is busy now
			if( p->m_bg != NULL )
				continue;
			//logging out ?
			if( p->GetSession() == NULL )
				continue;
			//can we add more players ?
			uint32 PlayerTeam = p->GetTeamInitial();
			if( TeamPlayersAdded[ PlayerTeam ] >= RequiredMinPlayers )
				continue;
			//remove from static group to add him to BG group
			if( p->GetGroup() )
				p->GetGroup()->RemovePlayer( p->getPlayerInfo(), true );
			//try to balance groups
			TeamPlayersAdded[ PlayerTeam ]++;
			a->AddPlayer( p, PlayerTeam, true );
			//remove from queue
			Queue->PlayerGuids[ i ] = 0;
		}
	//allow queue status to update next time
	Queue->CacheNextUpdateStamp = 0;
	memset( Queue->TeamPlayers, 0, sizeof( Queue->TeamPlayers ) );
}

class SCRIPT_DECL ArenaNoTeam : public GossipScript
{
public:
	SharedQueueStore	*Queue;
	ArenaNoTeam()
	{
		Queue = SharedQueues[ QUEUE_TYPE_10V10 ];
		Queue->AddRef();
	}
    void GossipHello( Object * pObject, Player* Plr, bool AutoSend )
	{
		if( Plr->CombatStatus.IsInCombat() )
		{
			Plr->BroadcastMessage("You are not allowed to use this NPC while in combat");
			return;
		}
		if( Plr->getLevel() < 85 )
		{
			Plr->BroadcastMessage("You need to drink more milk and grow healthy before facing real men");
			return;
		}
#ifndef _DEBUG
		if( Plr->LFG_GetGearScore() < 10000 && Plr->LFG_GetGearScore() < Queue->GearScoreSum / ( Queue->TeamPlayers[0] + Queue->TeamPlayers[1] + 1 ) / 2 )
		{
			Plr->BroadcastMessage("What do you intend ? Run naked in arena ? Get some decent gear first" );
			return;
		}
#endif
		if( AutoSend )
		{
			int DayOfTheUnivers = UNIXTIME / ( 24 * 60 * 60 );
			int HourIntervalAvailable = DayOfTheUnivers % 3;
			int HourOfTheUniverseDay = ( UNIXTIME / ( 60 * 60 ) ) % 24;
			int MinuteOfTheUniverseHour = ( UNIXTIME / 60 ) % 60;
			if( 
#ifdef _DEBUG
				1 == 1 || 
#endif
				HourOfTheUniverseDay >= HourIntervalAvailable * 8 && HourOfTheUniverseDay <= ( HourIntervalAvailable + 1 ) * 8 )
			{
				RefreshQueueStatus( Queue, false );
				Plr->BroadcastMessage( "Queue status : %d - %d", Queue->TeamPlayers[0], Queue->TeamPlayers[1] );

				GossipMenu *Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

				if( IsPlayerQueued( Queue, Plr->GetGUID() ) == false )
				{
					if( Queue->QueueType == QUEUE_TYPE_10V10 )
						Menu->AddItem( 6, "Queue for 10v10 arena", 1 );
					else if( Queue->QueueType == QUEUE_TYPE_5V5 )
						Menu->AddItem( 6, "Queue for 5v5 arena", 1 );
					else if( Queue->QueueType == QUEUE_TYPE_3V3 )
						Menu->AddItem( 6, "Queue for 3v3 arena", 1 );
					else if( Queue->QueueType == QUEUE_TYPE_2V2 )
						Menu->AddItem( 6, "Queue for 2v2 arena", 1 );
				}
				else
					Menu->AddItem( 6, "Leave queue", 2 );

				Menu->AddItem( 6, "Close", 3 );

				Menu->SendTo(Plr);
			}
			else
			{
				int RemainingHoursUntilStart;
				if( HourOfTheUniverseDay < HourIntervalAvailable * 8 )
					RemainingHoursUntilStart = HourIntervalAvailable * 8 - HourOfTheUniverseDay;
				else
					RemainingHoursUntilStart = ( HourIntervalAvailable + 1 ) * 8 + 16 - HourOfTheUniverseDay; 
				Plr->BroadcastMessage( "NPC disable remaining : %d Hours", RemainingHoursUntilStart );
			}
		}
	}
    void GossipSelectOption(Object * pObject, Player *Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Plr->CloseGossip();
		if( IntId == 1 )
		{
			uint32 ret = QueueAddPlayer( Queue, Plr->GetGUID() );
			if( ret == 1 )
			{
				Plr->BroadcastMessage("You entered the queue");
				Queue->GearScoreSum += Plr->LFG_GetGearScore();
			}
			else if( ret == 2 )
				Plr->BroadcastMessage("You are already queued ?");
		}
		else if( IntId == 2 )
		{
			uint32 ret = QueueRemPlayer( Queue, Plr->GetGUID() );
			if( ret == 1 )
				Plr->BroadcastMessage("You left the queue");
			else if( ret == 2 )
				Plr->BroadcastMessage("You never queued ?");
		}
	}
    void GossipEnd(Object * pObject, Player* Plr)
	{
		GossipScript::GossipEnd(pObject, Plr);
	}
	void Destroy()	{		delete this;	}
};

void ConvertToStaticSpawn( Creature *c )
{
	if( c && c->static_object == false )
	{
		c->static_object = true; //do not go out of visual range
		c->GetMapMgr()->PushStaticObject( c );
//		uint32 x = c->GetMapMgr()->GetPosX(c->GetPositionX());
//		uint32 y = c->GetMapMgr()->GetPosY(c->GetPositionY());
//		c->GetMapMgr()->UpdateCellActivity( x, y, 1, 2 );
	}
}

class ArenaNoTeamGossipOwnerNPC : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ArenaNoTeamGossipOwnerNPC);
	ArenaNoTeamGossipOwnerNPC(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		SharedQueues[ QUEUE_TYPE_10V10 ]->AddRef();
		if( SharedQueues[ QUEUE_TYPE_10V10 ]->HasPeriodicUpdate == 0 )
		{
			SharedQueues[ QUEUE_TYPE_10V10 ]->HasPeriodicUpdate = 1;
			ConvertToStaticSpawn( pCreature );
			RegisterAIUpdateEvent( CACHE_REFRESH_INTERVAL );
		}
	}
    void AIUpdate()
    {
		for( uint32 i=0;i<QUEUE_TPE_COUNT;i++)
			if( SharedQueues[ QUEUE_TYPE_10V10 + i ] )
				TryBuildTeam( SharedQueues[ QUEUE_TYPE_10V10 + i ] );
	}
};

void RegisterArenaNoTeamHandler(ScriptMgr* mgr)
{
	GossipScript *gs;

	//safety init all to 0
	memset( SharedQueues, 0, sizeof( SharedQueues ) );

	for( uint32 i = 0; i < QUEUE_TPE_COUNT; i++ )
	{
		SharedQueues[ i ] = new SharedQueueStore;
		SharedQueues[ i ]->QueueType = i;

		//there is only 1 gossip script shared between many NPCs
		gs = (GossipScript*) new ArenaNoTeam();
		((ArenaNoTeam*)gs)->Queue = SharedQueues[ i ];
		mgr->register_gossip_script( NPC_ID_ARENA_NOTEAM_10V10 + i , gs ); 
	}

	//periodically update the queue
	mgr->register_creature_script( NPC_ID_ARENA_NOTEAM_10V10, &ArenaNoTeamGossipOwnerNPC::Create);
}
