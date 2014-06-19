#include "StdAfx.h"

#define NPC_ID_ARENA_SPECTATOR_2V2	21
#define NPC_ID_ARENA_SPECTATOR_3V3	22
#define NPC_ID_ARENA_SPECTATOR_5V5	23
#define MAX_TEAMS_IN_GOSSIP			20
#define CACHE_REFRESH_INTERVAL		10000
#define MAX_PLAYERS_ARENA_MATCH		20	//5vs5

/*
insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('21','Spectate 2V2 Teams','watch me !','','0','0','7','0','0','0','0','2421','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('21','85','85','35','100000','100000','0','2.0','1','1000','0','4784','6219','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('22','Spectate 3V3 Teams','watch me !','','0','0','7','0','0','0','0','2421','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('22','85','85','35','100000','100000','0','2.0','1','1000','0','4784','6219','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('23','Spectate 5V5 Teams','watch me !','','0','0','7','0','0','0','0','2421','0','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('23','85','85','35','100000','100000','0','2.0','1','1000','0','4784','6219','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

*/

class SCRIPT_DECL ArenaSpectator : public GossipScript
{
public:
	ArenaTeam	*TeamCached[MAX_TEAMS_IN_GOSSIP];
	uint32		TeamsCachedCount;
	uint32		NextCacheRefresh;
	uint32		BGType;
	ArenaSpectator()
	{
		NextCacheRefresh = 0;
		TeamsCachedCount = 0;
		BGType = BATTLEGROUND_ARENA_2V2;
	}
	Player *GetInBGPlayer( ArenaTeam	*Team )
	{
		int membercount = MIN( 10, Team->m_memberCount );
		for( uint32 i=0;i<membercount; i++ )
			if( Team->m_members[i].Info != NULL
				&& Team->m_members[i].Info->m_loggedInPlayer 
				&& Team->m_members[i].Info->m_loggedInPlayer->m_bg )
			{
				return Team->m_members[i].Info->m_loggedInPlayer;
			}
		return NULL;
	}
	CBattleground *GetTeamBG( ArenaTeam	*Team )
	{
		Player *bgPlayer = GetInBGPlayer( Team );
		if( bgPlayer )
			return bgPlayer->m_bg;
		return NULL;
	}
	ArenaTeam	*FindMatchedTeam( ArenaTeam	*Team1 )
	{
		CBattleground *Team1BG = GetTeamBG( Team1 );

		for( uint32 i=0;i<TeamsCachedCount;i++)
			if( TeamCached[i] != NULL 
				&& TeamCached[i] != Team1 )
			{
				CBattleground *Team2BG = GetTeamBG( TeamCached[i] );
				if( Team2BG == Team1BG )
					return TeamCached[i];
			}
		return NULL;
	}
    void GossipHello( Object * pObject, Player* Plr, bool AutoSend )
	{
		if( Plr->CombatStatus.IsInCombat() )
		{
			Plr->BroadcastMessage("You are not allowed to use this NPC while in combat");
			return;
		}
		Plr->SpectatorRemove();
		if( AutoSend )
		{
			RefreshCache();

			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

			for( uint32 i=0;i<TeamsCachedCount;i++)
				if( TeamCached[i] != NULL )
				{
					ArenaTeam *TeamMatch = FindMatchedTeam( TeamCached[i] );
					//arena teams are not set up yet
					if( TeamMatch == NULL )
						continue;

					char MenuText[500];
					sprintf( MenuText, "%s(%d) - %s(%d)",TeamCached[i]->m_name.c_str(), TeamCached[i]->m_stat_rating, TeamMatch->m_name.c_str(), TeamMatch->m_stat_rating );
					Menu->AddItem( 6, MenuText, i );
				}

			if( TeamsCachedCount == 0 )
				Menu->AddItem( 6, "mmorpg PVP is a lie", 1 );

			Menu->AddItem( 6, "Close", 1 );

			Menu->SendTo(Plr);
		}
	}
    void GossipSelectOption(Object * pObject, Player *Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		if( IntId < TeamsCachedCount && TeamCached[ IntId ] == NULL )
		{
			return;
		}

		if( IntId < TeamsCachedCount )	//there is a chance this player has a guid less then 20 ? :( hope not
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

			ArenaTeam *Team1 = TeamCached[ IntId ];
			int membercount1 = MIN( 10, Team1->m_memberCount );
			for( uint32 i=0;i<membercount1; i++ )
			{
				PlayerInfo *inf = Team1->m_members[i].Info;
				if( inf != NULL
					&& inf->m_loggedInPlayer 
					&& inf->m_loggedInPlayer->m_bg )
				{
					char MenuText[500];
					sprintf( MenuText, "1)%s(%d)",inf->m_loggedInPlayer->GetName(), Team1->m_members[i].PersonalRating );
					Menu->AddItem( 6, MenuText, inf->m_loggedInPlayer->GetLowGUID() );
				}
			}

			ArenaTeam *Team2 = FindMatchedTeam( Team1 );
			if( Team2 )
			{
				int membercount2 = MIN( 10, Team2->m_memberCount );
				for( uint32 i=0;i<membercount2; i++ )
				{
					PlayerInfo *inf = Team2->m_members[i].Info;
					if( inf != NULL
						&& inf->m_loggedInPlayer 
						&& inf->m_loggedInPlayer->m_bg )
					{
						char MenuText[500];
						sprintf( MenuText, "2)%s(%d)",inf->m_loggedInPlayer->GetName(), Team2->m_members[i].PersonalRating );
						Menu->AddItem( 6, MenuText, inf->m_loggedInPlayer->GetLowGUID() );
					}
				}
			}

			Menu->AddItem( 6, "Close", 1 );

			Menu->SendTo( Plr );
		}
		else
		{
			Plr->CloseGossip();
			Player *dstplr = objmgr.GetPlayer( IntId );

			if( dstplr == NULL || dstplr->m_bg == NULL )
			{
				Plr->BroadcastMessage("Congrats, you broke it");
				return;
			}

			Plr->m_bgEntryPointX = Plr->GetPositionX();
			Plr->m_bgEntryPointY = Plr->GetPositionY();
			Plr->m_bgEntryPointZ = Plr->GetPositionZ();
			Plr->m_bgEntryPointMap = Plr->GetMapId();
//			Plr->BroadcastMessage( "Type '#spectate' to exit spectator mode" );
			Plr->BroadcastMessage( "Talk to this NPC again to exit spectator mode" );
			Plr->SpectatorApply();
			Plr->SafeTeleport( dstplr->GetMapMgr(), dstplr->GetPosition() );
			Plr->SetUInt64Value( PLAYER_FARSIGHT, 0 );
		}

	}
    void GossipEnd(Object * pObject, Player* Plr)
	{
		GossipScript::GossipEnd(pObject, Plr);
	}
	void Destroy()	{		delete this;	}
	void RefreshCache()
	{
		if( NextCacheRefresh > GetTickCount() )
			return;
		NextCacheRefresh = GetTickCount() + CACHE_REFRESH_INTERVAL;
		memset( TeamCached, 0, sizeof( TeamCached ) );
		objmgr.ListArenaMatches( NULL, BGType, TeamCached, MAX_TEAMS_IN_GOSSIP );

		TeamsCachedCount = 0;
		for( uint32 i=0;i<MAX_TEAMS_IN_GOSSIP;i++)
			if( TeamCached[i] != NULL )
				TeamsCachedCount++;
	}
};

void RegisterArenaSpectatorHandler(ScriptMgr* mgr)
{
	GossipScript *gs;

	gs = (GossipScript*) new ArenaSpectator();
	((ArenaSpectator*)gs)->BGType = BATTLEGROUND_ARENA_2V2;
    mgr->register_gossip_script( NPC_ID_ARENA_SPECTATOR_2V2, gs ); 

	gs = (GossipScript*) new ArenaSpectator();
	((ArenaSpectator*)gs)->BGType = BATTLEGROUND_ARENA_3V3;
    mgr->register_gossip_script( NPC_ID_ARENA_SPECTATOR_3V3, gs ); 

	gs = (GossipScript*) new ArenaSpectator();
	((ArenaSpectator*)gs)->BGType = BATTLEGROUND_ARENA_5V5;
    mgr->register_gossip_script( NPC_ID_ARENA_SPECTATOR_5V5, gs ); 
}
