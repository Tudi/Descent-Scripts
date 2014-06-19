#include "StdAfx.h"

#define GOLD_SINK_ITEM_NPC_ID					143330	//Spawn this. note that NPC has individual list of items and not global
#define GOLD_SINK_ITEM_NPC_DESC_TEXT_ID			100001	//Gossip text so the NPC will explain how he functions
#define GOLD_SINK_AURA_NPC_ID					143331	//Spawn this. note that NPC has individual list of items and not global
#define GOLD_SINK_AURA_NPC_DESC_TEXT_ID			100002	//Gossip text so the NPC will explain how he functions
//can put the same entry multiple times and NPC will list it
#define RENTED_ITEM_COUNT						15
static int RentedItemIDs[RENTED_ITEM_COUNT]		= { 32838, 32837, 22691, 13262, 296675, 296681, 296691, 296596, 296628, 296635, 296656, 296571, 296730, 295326 }; //!! non tradable, non stackable items only
#define ITEM_RESERVE_GOLD_INCREMENT_PCT			110		//at least 100%
#define ITEM_PRICE_DECREASE_PCT_PER_REFRESH		75
#define RENTED_ITEM_MIN_GOLD					5000
#ifdef _DEBUG
	#define MINIMUM_RESERVE_MINUTES					0
	#define MAXIMIM_RESERVE_MINUTES					(5-MINIMUM_RESERVE_MINUTES)		
#else
	#define MINIMUM_RESERVE_MINUTES					10
	#define MAXIMIM_RESERVE_MINUTES					(180-MINIMUM_RESERVE_MINUTES)		
#endif

#define EXTENSION_ITEM_INFO_INDEX_TIME			0
#define EXTENSION_ITEM_INFO_INDEX_COST			1
#define EXTENSION_ITEM_INFO_INDEX_IGUID			2
#define EXTENSION_ITEM_INFO_INDEX_PGUID			3
#define EXTENSION_ITEM_INFO_SIZE				5

#define RENTED_AURA_COUNT						2
static int RentedAuraIDs[RENTED_AURA_COUNT]					= { 35849,		98635	};
static int RentedAuraValues[RENTED_AURA_COUNT][3]			= { {60,60,0},	{-1,0,0}	};
static int RentedAuraDurations[RENTED_AURA_COUNT]			= { 60*60,		60*60	};
static int RentedAuraBaseCost[RENTED_AURA_COUNT]			= { 20000,		20000	};
static int RentedAuraBaseCostIncreaseSQ[RENTED_AURA_COUNT]	= { 15,			15		};
static char RentedAuraNames[RENTED_AURA_COUNT][10]			= { "DMG",		"DEF"	};

int8 RemoveItemFromPlayer( uint64 PlayerGUID, uint64 ItemGUID, Creature *Admin, char *ItemLink )
{
	Player *p = objmgr.GetPlayer( PlayerGUID );
	if( p == NULL )
		return 0;
	bool res = p->GetItemInterface()->SafeFullRemoveItemByGuid( ItemGUID );
	if( res && Admin && ItemLink )
	{
		char WriteBuff[500];
		sprintf( WriteBuff, "Someone payed more for the item %s. Taking it back", ItemLink );
		Admin->SendChatMessageToPlayer( CHAT_MSG_MONSTER_WHISPER, LANG_COMMON, WriteBuff, p );
	}
	return (res==true);
}

class GoldSinkItemNPCAI : public CreatureAIScript
{
public:
	GoldSinkItemNPCAI(Creature* pCreature) : CreatureAIScript(pCreature) 
	{
	}
	void OnLoad()
	{
		//we are always friendly
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
		}
		//in case we are not yet initialized, we should later on despawn ourself and get an updated version
		RegisterAIUpdateEvent( 60000 );
	}
	void AIUpdate() 
	{
		//check and remove player items
		for( uint32 i=0;i<RENTED_ITEM_COUNT;i++)
		{
			ItemPrototype *iProto = ItemPrototypeStorage.LookupEntry( RentedItemIDs[ i ] );
			if( iProto == NULL )
				continue;
			char ItemLink[ 500 ];
			GetItemLinkByProto(iProto, ItemLink );
			int64 *Reserved = _unit->GetCreateIn64Extension( GOLD_SINK_ITEM_NPC_ID + i * EXTENSION_ITEM_INFO_SIZE + EXTENSION_ITEM_INFO_INDEX_TIME );
			//check if item is still reserved
			if( *Reserved + MAXIMIM_RESERVE_MINUTES > UNIXTIME )
				continue;
			//remove item
			int64 *ItemGUID = _unit->GetCreateIn64Extension( GOLD_SINK_ITEM_NPC_ID + i * EXTENSION_ITEM_INFO_SIZE + EXTENSION_ITEM_INFO_INDEX_IGUID );
			if( *ItemGUID )
			{
				//tell the player we have an item available if he agrees to pay more
				int64 *PlayerGUID = _unit->GetCreateIn64Extension( GOLD_SINK_ITEM_NPC_ID + i * EXTENSION_ITEM_INFO_SIZE + EXTENSION_ITEM_INFO_INDEX_PGUID );
				RemoveItemFromPlayer( *PlayerGUID, *ItemGUID, _unit, ItemLink );
				*ItemGUID = 0;
			}
			else
			{
				int64 *Cost = _unit->GetCreateIn64Extension( GOLD_SINK_ITEM_NPC_ID + i * EXTENSION_ITEM_INFO_SIZE + EXTENSION_ITEM_INFO_INDEX_COST );
				*Cost = *Cost * ITEM_PRICE_DECREASE_PCT_PER_REFRESH / 100;
			}
		}
	}
	void OnRemoveFromWorld() 
	{
		for( uint32 i=0;i<RENTED_ITEM_COUNT;i++)
		{
			int64 *Holder = _unit->GetCreateIn64Extension( GOLD_SINK_ITEM_NPC_ID + i * EXTENSION_ITEM_INFO_SIZE + EXTENSION_ITEM_INFO_INDEX_TIME );
			*Holder = 0;
		}
		AIUpdate() ;
		//remove all player items
		RemoveAIUpdateEvent();
	}
	//default
	static CreatureAIScript *Create(Creature * c) { return new GoldSinkItemNPCAI(c); }
	void Destroy() { delete this; }
};

class SCRIPT_DECL GoldSinkItemNPCGossip : public GossipScript
{
public:
	void Destroy()	{		delete this;	}
	void send_MainPage(Object * pObject, Player* Plr)
	{
		if( pObject == NULL || Plr == NULL )
			return; 

		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOLD_SINK_ITEM_NPC_DESC_TEXT_ID, Plr);
		char WriteBuff[500];
		char WriteBuff2[500];
		char ItemLink[ 500 ];

		for( uint32 i=0;i<RENTED_ITEM_COUNT;i++)
		{
			ItemPrototype *iProto = ItemPrototypeStorage.LookupEntry( RentedItemIDs[ i ] );
			if( iProto == NULL )
				continue;
			GetItemLinkByProto(iProto, ItemLink );
			int64 *Holder;
			Holder = pObject->GetCreateIn64Extension( GOLD_SINK_ITEM_NPC_ID + i * EXTENSION_ITEM_INFO_SIZE + EXTENSION_ITEM_INFO_INDEX_TIME );
			//check if item is still reserved
			if( *Holder > UNIXTIME )
			{
				int64 ReservedSecondsRemaining = *Holder - UNIXTIME;
				sprintf( WriteBuff, "Item %s is reserved for %d seconds",ItemLink, ReservedSecondsRemaining );
				Menu->AddItem( 0, WriteBuff, 2);
				continue;
			}
			//tell the player we have an item available if he agrees to pay more
			Holder = pObject->GetCreateIn64Extension( GOLD_SINK_ITEM_NPC_ID + i * EXTENSION_ITEM_INFO_SIZE + EXTENSION_ITEM_INFO_INDEX_COST );
			uint32 NeededGold = ( RENTED_ITEM_MIN_GOLD + *Holder ) * ITEM_RESERVE_GOLD_INCREMENT_PCT / 100;
			sprintf( WriteBuff, "Pay at least %u to reserve %s for min %u max %u minutes", NeededGold, ItemLink, MINIMUM_RESERVE_MINUTES, MINIMUM_RESERVE_MINUTES + MAXIMIM_RESERVE_MINUTES );
			sprintf( WriteBuff2, "Steal %s from other player?", ItemLink );
			Menu->AddMenuItem( 0, WriteBuff, 0, 0, WriteBuff2, NeededGold * 10000, true, 2 + i );
		}
		Menu->SendTo(Plr);
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		if( AutoSend )
			send_MainPage(pObject,Plr);
	}
	void GossipEnd(Object * pObject, Player* Plr)	{		GossipScript::GossipEnd(pObject, Plr);	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature * pCreature = (pObject->IsCreature())?((Creature*)pObject):NULL;
		if( pCreature==NULL || Plr == NULL )
			return;
		uint32 ChosenItemSlot = IntId - 2;
		if( ChosenItemSlot >= 0 && ChosenItemSlot < RENTED_ITEM_COUNT )
		{
			int64 *Reserved = pObject->GetCreateIn64Extension( GOLD_SINK_ITEM_NPC_ID + ChosenItemSlot * EXTENSION_ITEM_INFO_SIZE + EXTENSION_ITEM_INFO_INDEX_TIME );
			//check if item is still reserved
			if( *Reserved > UNIXTIME )
			{
				Plr->BroadcastMessage("Item got already reserved by someone else");
				Plr->CloseGossip();
				return;
			}
			//check if offering enough money for the item
			//tell the player we have an item available if he agrees to pay more
			int64 *Cost = pObject->GetCreateIn64Extension( GOLD_SINK_ITEM_NPC_ID + ChosenItemSlot * EXTENSION_ITEM_INFO_SIZE + EXTENSION_ITEM_INFO_INDEX_COST );
			int32 NeededGold = ( RENTED_ITEM_MIN_GOLD + *Cost ) * ITEM_RESERVE_GOLD_INCREMENT_PCT / 100 * 10000;
			int32 BetGoldAmount;
			if( Code != NULL )
				BetGoldAmount = atoi( Code ) * 10000;
			else
				BetGoldAmount = 0;
			if( Plr->GetGold() < NeededGold || Plr->GetGold() < BetGoldAmount )
			{
				Plr->BroadcastMessage("You do not have enough gold");
				Plr->CloseGossip();
				return;
			}
			if( BetGoldAmount < NeededGold )
			{
				Plr->BroadcastMessage("You need to pay more then the other player to rent this item");
				Plr->CloseGossip();
				return;
			}
			Plr->ModGold( -BetGoldAmount );
			//mark the new gold reserver limit
			*Cost = NeededGold / 10000;
			//avoid threading issues, lock this item asp
			*Reserved = UNIXTIME + MINIMUM_RESERVE_MINUTES * 60;
			//take the item away from the other player if possible
			int64 *ItemGUID = pObject->GetCreateIn64Extension( GOLD_SINK_ITEM_NPC_ID + ChosenItemSlot * EXTENSION_ITEM_INFO_SIZE + EXTENSION_ITEM_INFO_INDEX_IGUID );
			int64 *PlayerGUID = pObject->GetCreateIn64Extension( GOLD_SINK_ITEM_NPC_ID + ChosenItemSlot * EXTENSION_ITEM_INFO_SIZE + EXTENSION_ITEM_INFO_INDEX_PGUID );
			{
				ItemPrototype *iProto = ItemPrototypeStorage.LookupEntry( RentedItemIDs[ ChosenItemSlot ] );
				char ItemLink[ 500 ];
				GetItemLinkByProto(iProto, ItemLink );
				RemoveItemFromPlayer( *PlayerGUID, *ItemGUID, pCreature, ItemLink );
			}
			//create and add a new item to the new player
/*
FILE *f = fopen( "gold.txt","at");
if( f )
{
	fprintf( f, "Adding %d to %d at %d cost %d expire %d\n", RentedItemIDs[ ChosenItemSlot ], (uint32)Plr->GetGUID(), UNIXTIME, *Cost, *Reserved );
	fclose( f );
}*/
			Item *It = Plr->GetItemInterface()->AddItemById( RentedItemIDs[ ChosenItemSlot ], 1, 0 );
			if( It )
			{
				*ItemGUID = It->GetGUID();
				*PlayerGUID = Plr->GetGUID();
				It->SetFlag( ITEM_FIELD_FLAGS, ITEM_FLAG_CONJURED );
			}
		}
		Plr->CloseGossip();
	}
};

class SCRIPT_DECL GoldSinkAuraNPCGossip : public GossipScript
{
public:
	void Destroy()	{		delete this;	}
	void send_MainPage(Object * pObject, Player* Plr)
	{
		if( pObject == NULL || Plr == NULL )
			return; 

		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOLD_SINK_AURA_NPC_DESC_TEXT_ID, Plr);
		char WriteBuff[500];
		char WriteBuff2[500];

		for( uint32 i=0;i<RENTED_AURA_COUNT;i++)
		{
			char AuraLink[ 500 ];
			GetSpellLinkByProto( RentedAuraIDs[ i ], AuraLink );
			uint32 Count = Plr->CountAura( RentedAuraIDs[ i ], AURA_SEARCH_POSITIVE );
			uint32 Price = RentedAuraBaseCost[ i ] * ( Count + 1 );
			uint32 PriceSQ = 1;
			for( uint32 j=0; j<Count; j++ )
				PriceSQ = PriceSQ * RentedAuraBaseCostIncreaseSQ[ i ];
			Price += PriceSQ;
			Price = Price * 10000;
			sprintf( WriteBuff, "Buy a new stack of '%s' for %d gold.Lasts %d minutes", RentedAuraNames[ i ], Price / 10000, RentedAuraDurations[ i ] / 60 );
			sprintf( WriteBuff2, "Buy the new stack of %s ?", AuraLink );
			Menu->AddMenuItem( 0, WriteBuff, 0, 0, WriteBuff2, Price, false, 2 + i );
		}
		Menu->SendTo(Plr);
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		if( AutoSend )
			send_MainPage(pObject,Plr);
	}
	void GossipEnd(Object * pObject, Player* Plr)	{		GossipScript::GossipEnd(pObject, Plr);	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature * pCreature = (pObject->IsCreature())?((Creature*)pObject):NULL;
		if( pCreature==NULL || Plr == NULL )
			return;
		uint32 ChosenAuraSlot = IntId - 2;
		int64 *Holder;
		if( ChosenAuraSlot >= 0 && ChosenAuraSlot <= RENTED_AURA_COUNT )
		{
			uint32 Count = Plr->CountAura( RentedAuraIDs[ ChosenAuraSlot ], AURA_SEARCH_POSITIVE );
			int32 Price = RentedAuraBaseCost[ ChosenAuraSlot ] * ( Count + 1 );
			int32 PriceSQ = 1;
			for( uint32 j=0; j<Count; j++ )
				PriceSQ = PriceSQ * RentedAuraBaseCostIncreaseSQ[ ChosenAuraSlot ];
			Price += PriceSQ;
			Price = Price * 10000;
			//do we have the money ?
			if( Plr->GetGold() < Price )
			{
				Plr->BroadcastMessage("You can't afford this. Go work all your life in a cubicle");
				Plr->CloseGossip();
				return;
			}
			Plr->ModGold( -Price );
			SpellEntry *spellInfo = dbcSpell.LookupEntry( RentedAuraIDs[ ChosenAuraSlot ] );
			Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
			spell->Init( Plr, spellInfo ,true, NULL);
			SpellCastTargets targets( Plr->GetGUID() );
			spell->forced_duration = RentedAuraDurations[ ChosenAuraSlot ] * 1000;
			spell->forced_basepoints[ 0 ] = RentedAuraValues[ ChosenAuraSlot ][0];
			spell->forced_basepoints[ 1 ] = RentedAuraValues[ ChosenAuraSlot ][1];
			spell->forced_basepoints[ 2 ] = RentedAuraValues[ ChosenAuraSlot ][2];
			spell->prepare(&targets);
		}
		Plr->CloseGossip();
	}
};
/*
replace into npc_gossip_textid values( 143330, 100001);
replace into npc_gossip_textid values( 143331, 100002);
replace into npc_text (entry,text0_0) values (100001,"I can lend you my items for a while with a few conditions$B- If after a while someone pays more, i will take it back and give it to him$B- You can only keep it for a while$B- If you logout you loose it$B$BLook into my eyes and say \"I need these items\"");
replace into npc_text (entry,text0_0) values (100002,"Wanna get stronger ? Sure, but it will cost you ! Maybe you can make a fortune using this strength. Get rich! Die happy ! ( or not )");

replace INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143330, 'Warren Buffett', 'Greedy GoldSink Bankers', '', 0, 0, 7, 0, 4, 0, 0, 26372, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
replace INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143330, 86, 86, 35, 350000, 350000, 1000000, 3, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);

replace INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143331, 'Michael Bloomberg', 'Greedy GoldSink Bankers', '', 0, 0, 7, 0, 4, 0, 0, 33527, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
replace INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143331, 86, 86, 35, 350000, 350000, 1000000, 2, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);

replace INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143332, 'Lapdancer', 'Lures new people to get suckered', '', 0, 0, 7, 0, 4, 0, 0, 16696, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
replace INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143332, 86, 86, 35, 350000, 350000, 1000000, 0.7, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);

update creature_spawns set emote_state=69 where entry in( 143330, 143331 );
update creature_spawns set emote_state=10 where entry in( 143332 );
*/
void RegisterGoldSinkNPC( ScriptMgr* mgr )
{
	GossipScript * gs;
//	mgr->register_creature_script( GOLD_SINK_ITEM_NPC_ID, &GoldSinkItemNPCAI::Create );
//	gs = (GossipScript*) new GoldSinkItemNPCGossip();
//	mgr->register_gossip_script( GOLD_SINK_ITEM_NPC_ID, gs );
	gs = (GossipScript*) new GoldSinkAuraNPCGossip();
	mgr->register_gossip_script( GOLD_SINK_AURA_NPC_ID, gs );
}
