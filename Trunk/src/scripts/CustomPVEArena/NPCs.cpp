#include "StdAfx.h"
#include "Setup.h"

////////////////////////////////////////////////////////////////////////////////////////
/*
{
	"The specialty of this realm relies in the custom items. Try to have fun using them in pve/pvp"
	"You can't level. Have talents or do anything else than the custom BG",
	"Queue up for the custom BG at the NPC",
	"You can trade your win currency to visual items",
	"You win the game by killing the enemy king",
	"Small mistakes add up. A war can be only won by those who survive it",
	"Game is slow paced, might take some time to finish a match",
	"Quest objectives are global. You can check #objectives after match starts",
	"You should build some buildings. Your choice in which order. Regen? Gold? Items? Defense?",
	"You can't tag mosters for kill score.You can kill steal",
	"Killing players is a good thing, but only killing players does not make you win the game",
	"Higher level vendors are scattered over the map. You can kill enemy to cripple their team",
	"A good build is when you barelly survive a fight and kill as many as possible. Then recover",
	"Damage multipliers are haste and bonus effects that scale"
	"Some item effects are unique. Make sure to sell older version before upgrade",
	"Bonus effects apply in the order you bought the items! Might need to sell / buy some items to get proper effects"
	"Chance based events are as much possible avoided. No crit chance. Maybe pseudo events",
	"Loss of control mechanics are avoided if possible",
	"If you do not like PVP, you can PVE farm to compensate",
	"You can build your character as you wish, but teamplay is advised",
	"You can duel with items you used last match"
};
*/
////////////////////////////////////////////////////////////////////////////////////////

#define NPC_ID_ARENA_NOTEAM_10V10		131
//#define NPC_ID_ARENA_NOTEAM_5V5			25
//#define NPC_ID_ARENA_NOTEAM_3V3			26
//#define NPC_ID_ARENA_NOTEAM_2V2			27

#define MAX_PLAYERS_QUEUED				400	
#define CACHE_REFRESH_INTERVAL			10000

enum NoTeamQueuType
{
	QUEUE_TYPE_10V10	= 0,
	QUEUE_TYPE_5V5		= 1,
	QUEUE_TYPE_3V3		= 2,
	QUEUE_TYPE_2V2		= 3,
//	QUEUE_TPE_COUNT		= 4,
	QUEUE_TPE_COUNT		= 1,
};

/*
insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('131','Queue for PVE arena','play me !','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('131','85','85','35','100000','100000','0','2.0','1','1000','0','4784','6219','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');
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

	memset( Queue->TeamPlayers, 0, sizeof( Queue->TeamPlayers ) );
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
			if( p->m_bg || p->GetMapMgr()->pInstance )
				continue;

			uint32 PlayerTeam = p->GetTeamInitial();
			Queue->TeamPlayers[ PlayerTeam ]++;
		}
}

void TryBuildTeam( SharedQueueStore *Queue )
{
	uint32 RequiredMinPlayers, RequiredMaxPlayers;
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
	RequiredMaxPlayers = RequiredMinPlayers * 2;
	//check if we have available players to create the teams
	RefreshQueueStatus( Queue, false );
//	if( Queue->TeamPlayers[ 0 ] < RequiredMinPlayers || Queue->TeamPlayers[ 1 ] < RequiredMinPlayers )
	if( Queue->TeamPlayers[ 0 ] + Queue->TeamPlayers[ 1 ] < RequiredMinPlayers )
		return;
	//create instance
	MapMgr *mgr = sInstanceMgr.CreateBattlegroundInstance( CUSTOM_BG_MAP_ID );
	if( mgr == NULL )
		return;
	//queue players to the instance
	uint32 TeamPlayersAdded[2];
	memset( TeamPlayersAdded, 0, sizeof( TeamPlayersAdded ) );

	//check if we have enough players to start queueing them
	uint32 CanAddPlayersCount = 0;
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
			if( p->m_bg != NULL || p->GetMapMgr()->pInstance )
				continue;
			//logging out ?
			if( p->GetSession() == NULL )
				continue;
			//suppose we can port him
			CanAddPlayersCount++;
		}
	if( CanAddPlayersCount < RequiredMinPlayers )
		return;

	uint32 PlayersAdded = 0;
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
			if( p->m_bg != NULL || p->GetMapMgr()->pInstance )
				continue;
			//logging out ?
			if( p->GetSession() == NULL )
				continue;
			//can we add more players ?
//			uint32 PlayerTeam = p->GetTeamInitial();
//			if( TeamPlayersAdded[ PlayerTeam ] >= RequiredMinPlayers )
//				continue;
			//try to balance groups
//			TeamPlayersAdded[ PlayerTeam ]++;
			//remove from static group to add him to BG group
			if( p->GetGroup() )
				p->GetGroup()->RemovePlayer( p->getPlayerInfo(), true );

			p->m_bgEntryPointX=p->GetPositionX();
			p->m_bgEntryPointY=p->GetPositionY();
			p->m_bgEntryPointZ=p->GetPositionZ();
			p->m_bgEntryPointMap=p->GetMapId();
			if( PlayersAdded % CUSTOM_BG_MAX_TEAMS != 0 )
			{
				//convert it to alliance
				ResetPlayerForTeam( p, 0 );
				p->SafeTeleport( CUSTOM_BG_MAP_ID, mgr->GetInstanceID(), 1094, 297, 339, 3.46 );
			}
			else
			{
				//convert it to horde
				ResetPlayerForTeam( p, 1 );
				p->SafeTeleport( CUSTOM_BG_MAP_ID, mgr->GetInstanceID(), 44, 1172, 368, 5.48 );
			}
			PlayersAdded++;
			//remove from queue
			Queue->PlayerGuids[ i ] = 0;
			if( PlayersAdded > 2 * RequiredMaxPlayers )
				break;
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
		if( AutoSend )
		{
			{
				RefreshQueueStatus( Queue, false );
				Plr->BroadcastMessage( "Queue status : %d - %d", Queue->TeamPlayers[0], Queue->TeamPlayers[1] );

				GossipMenu *Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

				if( IsPlayerQueued( Queue, Plr->GetGUID() ) == false )
				{
					if( Queue->QueueType == QUEUE_TYPE_10V10 )
						Menu->AddItem( 6, "Queue for PVEPVP arena", 1 );
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

////////////////////////////////////////////////////////////////////////////////////////

/*
insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('132','Banker Credit','','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('132','1','1','6','100','100','0','1.0','0','2000','0','2','2','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('133','Vendor Credit','','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('133','1','1','6','100','100','0','1.0','0','2000','0','2','2','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('134','Defense Credit','','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('134','1','1','6','200','200','0','1.0','0','3000','0','3','3','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('135','Healer Credit','','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('135','1','1','6','50','50','0','1.0','0','1000','0','1','1','1000','1','1','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');


insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('136','Banker Credit 2','','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('136','1','1','6','200','200','0','1.0','0','2000','0','4','4','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('137','Vendor Credit 2','','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('137','1','1','6','200','200','0','1.0','0','2000','0','4','4','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('138','Defense Credit 2','','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('138','1','1','6','200','200','0','1.0','0','3000','0','6','6','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('139','Healer Credit 2','','','0','0','7','0','0','0','0','28227','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('139','1','1','6','200','200','0','1.0','0','1000','1','1','1','1000','2','2','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('140','Catapult','','','0','0','7','0','0','0','0','23884','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('140','1','1','6','500','500','0','1.0','0','1000','0','1','1','5000','15','15','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','0.1','0.1','0.1','0','1','0','20','0','0','0','0','0');
insert into ai_agent ( entry, type ) values ( 140, 2 ); #ranged

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('141','Ballistae','','','0','0','7','0','0','0','0','25915','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('141','1','1','6','500','500','0','1.0','0','1000','0','1','1','5000','15','15','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','0.1','0.1','0.1','0','1','0','20','0','0','0','0','0');
insert into ai_agent ( entry, type ) values ( 140, 2 ); #ranged

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('140','Archer','','','0','0','7','0','0','0','0','9790','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('140','1','1','6','200','200','0','1.0','0','1000','0','2','2','1000','2','2','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');
insert into ai_agent ( entry, type ) values ( 140, 2 ); #ranged
insert into ai_agent ( entry, type ) values ( 140, 3 ); #flee

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('141','Archer','','','0','0','7','0','0','0','0','2688','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('141','1','1','6','200','200','0','1.0','0','1000','0','2','2','1000','2','2','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');
insert into ai_agent ( entry, type ) values ( 140, 2 ); #ranged
insert into ai_agent ( entry, type ) values ( 140, 3 ); #flee

value of an NPC : Damage made over it's lifetime based on player DPS
*/

class TeamAutoScalingNPC : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TeamAutoScalingNPC);
	TeamAutoScalingNPC(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		//get avg base DPS of players
		if( _unit->GetMapMgr() )
		{
			uint32 SumDPS = 0, SumDPSCount = 0; 
			uint32 SumHP = 0;
			for( PlayerStorageMap::iterator itr = _unit->GetMapMgr()->m_PlayerStorage.begin(); itr != _unit->GetMapMgr()->m_PlayerStorage.end(); itr++ )
				if( GetTeamAnyMask( itr->second ) == GetTeamAnyMask( _unit ) )
				{
					SumDPS += itr->second->GetFloatValue( UNIT_FIELD_MINDAMAGE ) * 1000 / itr->second->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
					SumDPS += itr->second->GetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE ) * 1000 / itr->second->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME_1 );
					SumDPSCount += 2;
					SumHP += itr->second->GetMaxHealth() * 2;
				}
			if( SumDPSCount > 0 )
			{
				uint32	AvgDPS = SumDPS/ SumDPSCount;
				uint32	AvgHP = SumHP / SumDPSCount;
				//my hp needs to be low enough so avg player can kill me before he's health drops to x%
				uint32	MyHP = EXPECTED_NPC_KILL_TIME * AvgDPS;
				uint32	MyDPS = AvgHP / EXPECTED_NPC_KILL_PLAYER_LIFE;
				_unit->SetMaxHealth( ( _unit->GetMaxHealth() + MyHP ) / 2 );
				_unit->SetHealth( _unit->GetMaxHealth() );
				_unit->SetMaxDamage( ( _unit->GetMaxDamage() + MyDPS * _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME ) / 1000 ) / 2 );
				_unit->SetMinDamage( _unit->GetMaxDamage( ) );
			}
		}
	}
};

uint32 GetTeamAnyMask( Object *o )
{
	if( o == NULL )
		return (uint32)(-1);
	if( o->IsCreature() && o->GetTeam() == (uint32)(-1) )
	{
		uint32 Entry = o->GetEntry();
		if( Entry == 1 )
			return 0;
		return 1;
	}
	return ( o->GetTeam() + 1 ) * QO_BUILD_TEAM_SHIFTER1;
}

void SetupNPCs( ScriptMgr* mgr )
{
	//safety init all to 0
	memset( SharedQueues, 0, sizeof( SharedQueues ) );

	for( uint32 i = 0; i < QUEUE_TPE_COUNT; i++ )
	{
		SharedQueues[ i ] = new SharedQueueStore;
		memset( SharedQueues[ i ], 0, sizeof( SharedQueueStore ) );
		SharedQueues[ i ]->QueueType = i;

		//there is only 1 gossip script shared between many NPCs
		GossipScript *gs = (GossipScript*) new ArenaNoTeam();
		((ArenaNoTeam*)gs)->Queue = SharedQueues[ i ];
		mgr->register_gossip_script( NPC_ID_ARENA_NOTEAM_10V10 + i , gs ); 
	}

	//periodically update the queue
	mgr->register_creature_script( NPC_ID_ARENA_NOTEAM_10V10, &ArenaNoTeamGossipOwnerNPC::Create);

//	mgr->register_creature_script( 1, &TheKing::Create);	
}