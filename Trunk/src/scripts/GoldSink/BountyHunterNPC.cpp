#include "StdAfx.h"

#define NPC_ID_BOUNTY_HUNTER_NPC		143329
#define MAX_BOUNTIES_LISTED				10
#define ORGANIZER_GOLD_CUT_PCT			10
#define MAX_REWARD_AURA_STACKS			10
#define SAVE_LOAD_REVISION				1

/*
insert into `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) values
('143329','Bounty Hunter','Greedy GoldSink Bankers','','0','0','7','0','0','0','0','4617','4618','0','0','2','1','0','0','0','0','0','0','0','0','0');
insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) values
('143329','85','85','35','100000','100000','0','1.0','1','1000','0','4784','6219','0','0','0','900000','0','0','0','0','0','0','0','1.8','0.5','','0','0','0','0','3.095','9.5','14','0','1','0','20','0','0','0','0','0');

update creature_spawns set emote_state=433 where entry in( 143329 );
*/

struct BountyHuntStore
{
	BountyHuntStore()
	{
		TargetPlayerGUID = GoldPromissed = PrevGoldPromissed = HuntSartedAtStamp = Cooldown = 0;
	}
	uint64				TargetPlayerGUID;
	uint64				GoldPromissed;
	uint64				PrevGoldPromissed;
	uint64				Cooldown;		//not yet used
	uint32				HuntSartedAtStamp;
	std::set<uint64>	PlayersHunting;
};

class SharedBountyNPCStore
{
public:
	SharedBountyNPCStore(){}
	~SharedBountyNPCStore(){}
	std::list< BountyHuntStore* >	BountyList;
};

SharedBountyNPCStore SharedBountyStore;

void LoadFromFile()
{
	FILE *f = fopen( "./script_vars/BountyHunterNPC.txt","rt");
	if( f )
	{
		int FileRevision;
		fscanf( f, "%d\n", &FileRevision );
		if( FileRevision == SAVE_LOAD_REVISION )
		{
			int RowCount;
			fscanf( f, "%d\n", &RowCount );
			for( uint32 j=0;j<RowCount;j++)
			{
				BountyHuntStore *bh = new BountyHuntStore;
				uint64 HunterListSize = 0;
				fscanf( f, "%lld %lld %lld %lld %lld", &bh->TargetPlayerGUID, &bh->GoldPromissed, &bh->PrevGoldPromissed, &bh->Cooldown, &HunterListSize );
				for( uint32 i=0;i<HunterListSize;i++ )
				{
					uint64 HunterGuid;
					fscanf( f, "%lld ", &HunterGuid );
					bh->PlayersHunting.insert( HunterGuid );
				}
				fscanf( f, "\n");

				bool Inserted = false;
				if( bh->GoldPromissed > 0 )
				{
					PlayerInfo *inf = objmgr.GetPlayerInfo( bh->TargetPlayerGUID );
					if( inf != NULL )
					{
						SharedBountyStore.BountyList.push_front( bh );
						Inserted = true;
					}
				}
				if( Inserted == false )
				{
					delete bh;
					bh = NULL;
				}
			}
		}
		fclose( f );
	}
}

void SaveToFile()
{
	FILE *f = fopen( "./script_vars/BountyHunterNPC.txt","wt");
	if( f )
	{
		fprintf( f, "%d \n", SAVE_LOAD_REVISION );
		fprintf( f, "%d \n", (uint32)SharedBountyStore.BountyList.size() );
		for( std::list< BountyHuntStore* >::iterator itr = SharedBountyStore.BountyList.begin(); itr != SharedBountyStore.BountyList.end(); itr++ )
		{
			BountyHuntStore *bh = (*itr);
			if( bh->GoldPromissed == 0 )
				continue;
			uint32 Count = bh->PlayersHunting.size();
			fprintf( f, "%lld %lld %lld %lld %d ", bh->TargetPlayerGUID, bh->GoldPromissed, bh->PrevGoldPromissed, bh->Cooldown, (uint32)( Count ) );
			for( std::set<uint64>::iterator	itr2=bh->PlayersHunting.begin(); itr2!=bh->PlayersHunting.end(); itr2++ )
				fprintf( f, "%lld ", (uint64)(*itr2) );
			fprintf( f, "\n");
		}
		fclose( f );
	}
}

class SCRIPT_DECL BountyHunterGossip : public GossipScript
{
	char MenuText[MAX_BOUNTIES_LISTED][50];
public:
	BountyHunterGossip()
	{
	}
    void GossipHello( Object * pObject, Player* Plr, bool AutoSend )
	{
		if( AutoSend )
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

			Menu->AddItem( 6, "Add target bounty", 1 );
			Menu->AddItem( 6, "List bounties available", 2 );
			Menu->AddItem( 6, "Abandon all my targets", 3 );

			Menu->AddItem( 6, "Close", 4 );

			Menu->SendTo(Plr);
		}
	}
    void GossipSelectOption(Object * pObject, Player *Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		//add a new bounty
		if( IntId == 1 )
		{
			Plr->CloseGossip();
			int64 *PlayerGuid = Plr->GetCreateIn64Extension( EXTENSION_ID_BOUNTY_HUNTER_GUID );
			if( *PlayerGuid == 0 )
			{
				Plr->BroadcastMessage("First type in chat : #BountyHunterAdd <PlayerName> <Gold>" );
				return;
			}
			PlayerInfo *inf = objmgr.GetPlayerInfo( *PlayerGuid );
			if( inf == NULL )
			{
				Plr->BroadcastMessage("Player no longer exists" );
				return;
			}
			int64 *GoldStore = Plr->GetCreateIn64Extension( EXTENSION_ID_BOUNTY_HUNTER_PRICE );
			if( *GoldStore * 100 * 100 > Plr->GetGold() )
			{
				Plr->BroadcastMessage("You do not have enough gold" );
				return;
			}
			//check if there is a higher bid already
			BountyHuntStore *bh = NULL;
			bool NeedAdd =  true;
			for( std::list< BountyHuntStore* >::iterator itr = SharedBountyStore.BountyList.begin(); itr != SharedBountyStore.BountyList.end(); itr++ )
			{
				if( (*itr)->TargetPlayerGUID == *PlayerGuid )
				{
					bh = (*itr);
					NeedAdd = false;
					break;
				}
			}
//			if( bh != NULL && bh->GoldPromissed >= *GoldStore )
//			{
//				Plr->BroadcastMessage("There is a %d gold bounty already on this target", ExistingBounty );
//				return;
//			}
			if( bh == NULL )
			{
				bh = new BountyHuntStore;
				bh->GoldPromissed = 0;
			}
			bh->GoldPromissed += (*GoldStore) * ( 100 - ORGANIZER_GOLD_CUT_PCT ) / 100;
			bh->HuntSartedAtStamp = GetTickCount();
			bh->TargetPlayerGUID = *PlayerGuid;
			if( NeedAdd == true )
				SharedBountyStore.BountyList.push_front( bh );
			Plr->ModGold( -(*GoldStore) * 100 * 100 );
			Plr->BroadcastMessage( "Bounty on player %s is %d gold", inf->name, (uint32)( bh->GoldPromissed ) );
			//persistency
			SaveToFile();
		}
		//list bounties we are not yet subscribed to
		else if( IntId == 2 )
		{
			uint32 PrintedCount = 0;
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

			for( std::list< BountyHuntStore* >::iterator itr = SharedBountyStore.BountyList.begin(); itr != SharedBountyStore.BountyList.end(); itr++ )
			{
				if( (*itr)->GoldPromissed == 0 )
					continue;

				if( (*itr)->PlayersHunting.find( Plr->GetGUID() ) != (*itr)->PlayersHunting.end() )
					continue;

				PlayerInfo *inf = objmgr.GetPlayerInfo( (*itr)->TargetPlayerGUID );
				if( inf == NULL )
					continue;

				sprintf( MenuText[PrintedCount], "%s - %d", inf->name, (*itr)->GoldPromissed );
				Menu->AddItem( 6, MenuText[PrintedCount], 10 + (*itr)->TargetPlayerGUID );
				PrintedCount++;
				if( PrintedCount >= MAX_BOUNTIES_LISTED )
					break;
			}

			if( PrintedCount == 0 )
				Menu->AddItem( 6, "No bounties available", 3 );

			Menu->AddItem( 6, "Close", 3 );

			Menu->SendTo(Plr);
		}
		//abandon all bounties
		else if( IntId == 3 )
		{
			for( std::list< BountyHuntStore* >::iterator itr = SharedBountyStore.BountyList.begin(); itr != SharedBountyStore.BountyList.end(); itr++ )
				(*itr)->PlayersHunting.erase( Plr->GetGUID() );
			Plr->BroadcastMessage("Congrats, you will not receive a reward even if you kill the target" );
			Plr->CloseGossip();
		}
		else if( IntId > 10 )
		{
			uint64 SubscribeToKillPlayerGUID = IntId - 10;
			bool FoundTarget = false;
			for( std::list< BountyHuntStore* >::iterator itr = SharedBountyStore.BountyList.begin(); itr != SharedBountyStore.BountyList.end(); itr++ )
			{
				if( (*itr)->GoldPromissed == 0 )
					continue;

				if( (*itr)->TargetPlayerGUID != SubscribeToKillPlayerGUID )
					continue;

				if( (*itr)->PlayersHunting.find( Plr->GetGUID() ) != (*itr)->PlayersHunting.end() )
					continue;

				PlayerInfo *inf = objmgr.GetPlayerInfo( (*itr)->TargetPlayerGUID );
				if( inf == NULL )
					continue;

				(*itr)->PlayersHunting.insert( Plr->GetGUID() );
				Plr->BroadcastMessage("You subscribed to kill player %s", inf->name );
				FoundTarget = true;
				break;
			}
			if( FoundTarget == false )
				Plr->BroadcastMessage("Something broke. Could not find target" );
			Plr->CloseGossip();
		}
		else
		{
			Plr->CloseGossip();
		}
	}
    void GossipEnd(Object * pObject, Player* Plr)
	{
		GossipScript::GossipEnd(pObject, Plr);
	}
	void Destroy()	{		delete this;	}
};

inline char LowerCase( char in )
{
	if( in <= 'Z' && in >= 'A' )
		return in + ('a'-'A');
	return in;
}

bool CaseInsensitiveParamCheck( const char *src, char *param, int maxlen = 255 )
{
	int i;
	for( i=0;i<maxlen && src[i] != 0 && param[i] != 0;i++)
		if( LowerCase(src[i]) != LowerCase(param[i]) )
			return false;
	//we checked the whole param without finding a mismatch
	if( param[i] == 0 )
		return true;
	return false;
}

void OnPlayerChatBountyHunterNPC(Player * pPlayer, uint32 Type, uint32 Lang, const char * Message, const char * Misc)
{
	if( Message == NULL || pPlayer == NULL )
		return;
	if( Message[0] != '#' || Message[1] != 'B' )
		return;
	//safety check to make sure string has a terminator and 2 " "
	uint32	SpaceCount = 0;
	bool	FoundTerminator = false;
	for( uint32 i=0;i<70;i++)
	{
		if( Message[i] == 0 )
		{
			FoundTerminator = true;
			break;
		}
		if( Message[i] == ' ' )
			SpaceCount++;
	}
	if( FoundTerminator == false )
		return;
	if( SpaceCount < 2 )
		return;

	if( CaseInsensitiveParamCheck( Message, "#BountyHunterAdd" ) == true )
	{
		char	Command[70];
		char	PlayerName[70];
		uint32	Gold;
		sscanf( Message, "%s %s %d",Command, PlayerName, &Gold );

		if( pPlayer->GetGold() < Gold * 100*100 )
		{
			pPlayer->BroadcastMessage("You do not have enough gold" );
			return;
		}
		if( Gold * 100*100 == 0 )
		{
			pPlayer->BroadcastMessage("It's pointless to put 0 cent on someone head" );
			return;
		}
		PlayerInfo *inf = objmgr.GetPlayerInfoByName( PlayerName );
		if( inf == NULL )
		{
			pPlayer->BroadcastMessage("Could not find player with name %s", PlayerName );
			return;
		}
		if( inf->guid == pPlayer->GetGUID() )
		{
			pPlayer->BroadcastMessage("Suicide is not rewarded" );
			return;
		}

		int64 *PlayerGuid = pPlayer->GetCreateIn64Extension( EXTENSION_ID_BOUNTY_HUNTER_GUID );
		*PlayerGuid = inf->guid;
		int64 *GoldStore = pPlayer->GetCreateIn64Extension( EXTENSION_ID_BOUNTY_HUNTER_PRICE );
		*GoldStore = Gold;
		pPlayer->BroadcastMessage( "Talk to the NPC to add bounty on %s", PlayerName );
	}
}

void OnPlayerKillBountyHunterNPC(Player *pPlayer, Player *pVictim)
{
	if( pPlayer->m_bg != NULL )
		return;
	if( pVictim == NULL )
		return;
	for( std::list< BountyHuntStore* >::iterator itr = SharedBountyStore.BountyList.begin(); itr != SharedBountyStore.BountyList.end(); itr++ )
		if( (*itr)->TargetPlayerGUID == pVictim->GetGUID() 
			&& (*itr)->GoldPromissed > 0
			&& (*itr)->PlayersHunting.find( pPlayer->GetGUID() ) != (*itr)->PlayersHunting.end()
			)
		{
			pPlayer->BroadcastMessage( "Congrats for killing bounty target. Your reward is %d gold", (*itr)->GoldPromissed );
			pPlayer->ModGold( (*itr)->GoldPromissed );
			(*itr)->PrevGoldPromissed = (*itr)->GoldPromissed;
			(*itr)->GoldPromissed = 0;
			(*itr)->PlayersHunting.clear();

			//damage on hunter
			if( pPlayer->CountAura( 35849, AURA_SEARCH_ALL ) < MAX_REWARD_AURA_STACKS )
			{
				SpellEntry *spellInfo = dbcSpell.LookupEntry( 35849 );
				if( spellInfo )
				{
					Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
					spell->Init( pPlayer, spellInfo ,true, NULL);
					SpellCastTargets targets( pPlayer->GetGUID() );
					spell->forced_duration = 30 * 60 * 1000;
					spell->forced_basepoints[ 0 ] = 60;
					spell->forced_basepoints[ 1 ] = 60;
					spell->prepare(&targets);
				}
			}

			//defense on victim
			if( pPlayer->CountAura( 98635, AURA_SEARCH_ALL ) < MAX_REWARD_AURA_STACKS )
			{
				SpellEntry *spellInfo = dbcSpell.LookupEntry( 98635 );
				if( spellInfo )
				{
					Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
					spell->Init( pVictim, spellInfo ,true, NULL);
					SpellCastTargets targets( pVictim->GetGUID() );
					spell->forced_duration = 30 * 60 * 1000;
					spell->forced_basepoints[ 0 ] = -1;
					spell->forced_basepoints[ 1 ] = -1;
					spell->prepare(&targets);
				}
			}
		}
}

void RegisterBountyHunterNPCHandler(ScriptMgr* mgr)
{
	GossipScript *gs;

	gs = (GossipScript*) new BountyHunterGossip();
    mgr->register_gossip_script( NPC_ID_BOUNTY_HUNTER_NPC, gs ); 

	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_CHAT, &OnPlayerChatBountyHunterNPC );
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_KILL_PLAYER, &OnPlayerKillBountyHunterNPC );

	LoadFromFile();
}
