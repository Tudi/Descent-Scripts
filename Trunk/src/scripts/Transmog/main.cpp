#include "StdAfx.h"

enum LocalTransmogStatusStates
{
	TRANGMOG_STATE_UNINITIALIZED	= 0,
	TRANGMOG_STATE_LOADING_FROM_DB	= 1,
	TRANGMOG_STATE_ADDED_TO_CHAR	= 2,
};

void OnPlayerChatTransmog( Player *pPlayer, uint32 Type, uint32 Lang, const char * Message, const char * Misc);

inline char LowerCase( char in )
{
	if( in <= 'Z' && in >= 'A' )
		return in + ('a'-'A');
	return in;
}

bool CaseInsensitiveParamCheck( char *src, char *param, int maxlen = 255 )
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

void OnPlayerEnterWorldTransmog( Player *pPlayer )
{
	if( pPlayer == NULL || pPlayer->GetExtension( EXTENSION_ID_TRANSMOG_SAVE_CACHE ) != NULL )
		return;

	int64 *Status = pPlayer->GetCreateIn64Extension( EXTENSION_ID_TRANSMOG_SAVE_CACHE );
	QueryResult *result = CharacterDatabase.Query( "select slot, entry from Transmogs where OwnerGUID=%d", pPlayer->GetLowGUID() );
	if( result )
	{
		do
		{
			Field *fields = result->Fetch();
			uint32 Slot = fields[0].GetUInt32();
			uint32 Entry = fields[1].GetUInt32();

			if( pPlayer->GetItemInterface() )
			{
				Item *it = pPlayer->GetItemInterface()->GetInventoryItem( Slot );
				if( it == NULL || it->GetEntry() == Entry )
				{
					CharacterDatabase.Execute( "delete from Transmogs where OwnerGUID=%d and slot=%d", pPlayer->GetLowGUID(), Slot );
					continue;
				}
			}
			char LocalBuffer[200];

			if( Slot == EQUIPMENT_SLOT_HEAD )
				sprintf_s( LocalBuffer, 200, "@Transmog head" );
			else if( Slot == EQUIPMENT_SLOT_SHOULDERS )
				sprintf_s( LocalBuffer, 200, "@Transmog shoulder" );
			else if( Slot == EQUIPMENT_SLOT_CHEST )
				sprintf_s( LocalBuffer, 200, "@Transmog chest" );
			else if( Slot == EQUIPMENT_SLOT_BODY )
				sprintf_s( LocalBuffer, 200, "@Transmog body" );
			else if( Slot == EQUIPMENT_SLOT_WAIST )
				sprintf_s( LocalBuffer, 200, "@Transmog waist" );
			else if( Slot == EQUIPMENT_SLOT_LEGS )
				sprintf_s( LocalBuffer, 200, "@Transmog legs" );
			else if( Slot == EQUIPMENT_SLOT_FEET )
				sprintf_s( LocalBuffer, 200, "@Transmog feet" );
			else if( Slot == EQUIPMENT_SLOT_HANDS )
				sprintf_s( LocalBuffer, 200, "@Transmog hands" );
			else if( Slot == EQUIPMENT_SLOT_BACK )
				sprintf_s( LocalBuffer, 200, "@Transmog back" );
			else if( Slot == EQUIPMENT_SLOT_MAINHAND )
				sprintf_s( LocalBuffer, 200, "@Transmog main" );
			else if( Slot == EQUIPMENT_SLOT_OFFHAND )
				sprintf_s( LocalBuffer, 200, "@Transmog off" );
			else if( Slot == EQUIPMENT_SLOT_RANGED )
				sprintf_s( LocalBuffer, 200, "@Transmog ranged" );
			else if( Slot == EQUIPMENT_SLOT_TABARD )
				sprintf_s( LocalBuffer, 200, "@Transmog tabard" );
			else if( Slot == EQUIPMENT_SLOT_WRISTS )
				sprintf_s( LocalBuffer, 200, "@Transmog wrists" );

			if( Entry == 0 )
				sprintf_s( LocalBuffer, 200, "%s erase", LocalBuffer );
			else
				sprintf_s( LocalBuffer, 200, "%s %d", LocalBuffer, Entry );

			*Status = TRANGMOG_STATE_LOADING_FROM_DB;
			OnPlayerChatTransmog( pPlayer, 0, 0, LocalBuffer, "" );
			if( *Status != TRANGMOG_STATE_ADDED_TO_CHAR )
				CharacterDatabase.Execute( "delete from Transmogs where OwnerGUID=%d and slot=%d", pPlayer->GetLowGUID(), Slot );
		}while( result->NextRow() );
		delete result;
	}
}

void OnPlayerSetTransmog( Player *pPlayer, uint32 Slot, uint32 Entry )
{
	int64 *Status = pPlayer->GetCreateIn64Extension( EXTENSION_ID_TRANSMOG_SAVE_CACHE );
	if( *Status == TRANGMOG_STATE_UNINITIALIZED || *Status == TRANGMOG_STATE_LOADING_FROM_DB )
		return;
	*Status = TRANGMOG_STATE_ADDED_TO_CHAR;
//	CharacterDatabase.Execute( "replace into Transmogs ( %d,%d,%d )", pPlayer->GetLowGUID(), Slot, Entry );
	if( Entry == -1 )
		CharacterDatabase.Execute( "delete from Transmogs where OwnerGUID=%d and slot=%d", pPlayer->GetLowGUID(), Slot );
}

void OnPlayerLogoutTransmog( Player *pPlayer )
{
	if( pPlayer->GetItemInterface() == NULL )
		return;

	for(uint32 i=EQUIPMENT_SLOT_START;i<EQUIPMENT_SLOT_END;i++)
	{
		Item *it = pPlayer->GetItemInterface()->GetInventoryItem( i );
		uint32 VisibleItemEntry = pPlayer->GetUInt32Value( PLAYER_VISIBLE_ITEM_1_ENTRYID + i * ( PLAYER_VISIBLE_ITEM_2_ENTRYID - PLAYER_VISIBLE_ITEM_1_ENTRYID ) );
		if( it == NULL )
			continue;
		uint32 ItemEntry = it->GetEntry();
		if( ItemEntry == VisibleItemEntry )
			continue;
		CharacterDatabase.Execute( "replace into Transmogs values (%d,%d,%d)", pPlayer->GetLowGUID(), i, VisibleItemEntry );
	}
}

void OnPlayerChatTransmog( Player *pPlayer, uint32 Type, uint32 Lang, const char * Message, const char * Misc)
{
	if( Message == NULL )
		return;
	if( CaseInsensitiveParamCheck( (char*)Message, "@Transmog" ) == true )
	{
		char *FirstParam = (char*)Message;
		char *LimitSearch = FirstParam + 1024;	//if we get passed this then the message is strangely large
		while( *FirstParam != ' ' && *FirstParam != 0 )
			FirstParam++;
		if( *FirstParam == ' ' )
			FirstParam++;
		//is there a second param at all ?
		if( *FirstParam == 0 )	
		{
			pPlayer->BroadcastMessage("Transmog Error: Missing first parameter.Printing help");
JMP_INCORRECT_USAGE:
			pPlayer->BroadcastMessage("Transmog Usage: @Transmog <slot> erase");
			pPlayer->BroadcastMessage("Transmog Usage: @Transmog ID [itemlink]");
			pPlayer->BroadcastMessage("Transmog Usage: @Transmog <slot> <ItemID>");
			pPlayer->BroadcastMessage("Transmog Usage: slot : head/shoulder/chest/body/waist/legs/feet/hands/back/main/off/ranged/tabard");
			pPlayer->BroadcastMessage("Transmog Usage: ItemID : You need to have item in your inventory.You can get id for the name on wowhead.");
			return;
		}
		int selected_slot = -1;
		bool CanErase = false;
		if( CaseInsensitiveParamCheck( FirstParam, "id" ) == true )
		{
			char *SecondParam = FirstParam;
			while( *SecondParam != ' ' && *SecondParam != 0 && SecondParam < LimitSearch )
				SecondParam++;
			if( *SecondParam == ' ' )
				SecondParam++;
			char *src_item_link = SecondParam;
			char *LocID = SecondParam;
			while( *LocID != ':' && *LocID != 0 && LocID < LimitSearch )
				LocID++;
			if( *LocID == ':' )
				LocID++;
			int32 SrcItemID = atoi( LocID );
			//we could double check to see if the name of the item matches
			pPlayer->BroadcastMessage("The Item ID of %s is %d", src_item_link, SrcItemID );
			return;
		}
		else if( CaseInsensitiveParamCheck( FirstParam, "head" ) == true )
			selected_slot = EQUIPMENT_SLOT_HEAD;
		else if( CaseInsensitiveParamCheck( FirstParam, "shoulder" ) == true )
		{
			CanErase = true;
			selected_slot = EQUIPMENT_SLOT_SHOULDERS;
		}
		else if( CaseInsensitiveParamCheck( FirstParam, "chest" ) == true )
			selected_slot = EQUIPMENT_SLOT_CHEST;
		else if( CaseInsensitiveParamCheck( FirstParam, "body" ) == true )
			selected_slot = EQUIPMENT_SLOT_BODY;
		else if( CaseInsensitiveParamCheck( FirstParam, "waist" ) == true )
		{
			CanErase = true;
			selected_slot = EQUIPMENT_SLOT_WAIST;
		}
		else if( CaseInsensitiveParamCheck( FirstParam, "legs" ) == true )
			selected_slot = EQUIPMENT_SLOT_LEGS;
		else if( CaseInsensitiveParamCheck( FirstParam, "feet" ) == true )
			selected_slot = EQUIPMENT_SLOT_FEET;
		else if( CaseInsensitiveParamCheck( FirstParam, "hands" ) == true )
			selected_slot = EQUIPMENT_SLOT_HANDS;
		else if( CaseInsensitiveParamCheck( FirstParam, "back" ) == true )
			selected_slot = EQUIPMENT_SLOT_BACK;
		else if( CaseInsensitiveParamCheck( FirstParam, "main" ) == true )
			selected_slot = EQUIPMENT_SLOT_MAINHAND;
		else if( CaseInsensitiveParamCheck( FirstParam, "off" ) == true )
			selected_slot = EQUIPMENT_SLOT_OFFHAND;
		else if( CaseInsensitiveParamCheck( FirstParam, "ranged" ) == true )
			selected_slot = EQUIPMENT_SLOT_RANGED;
		else if( CaseInsensitiveParamCheck( FirstParam, "tabard" ) == true )
			selected_slot = EQUIPMENT_SLOT_TABARD;
		else if( CaseInsensitiveParamCheck( FirstParam, "wrists" ) == true )
			selected_slot = EQUIPMENT_SLOT_WRISTS;
		if( selected_slot == -1 )
		{
			pPlayer->BroadcastMessage("Transmog Error: Bad first parameter. Possible values");
			pPlayer->BroadcastMessage("Transmog : head/shoulder/chest/body/waist/legs/feet/hands/back/main/off/ranged/tabard");
			return;
		}
		Item *it_dst = pPlayer->GetItemInterface()->GetInventoryItem( selected_slot );
		if( it_dst == NULL )
		{
			pPlayer->BroadcastMessage("Transmog Error: There is no item in destination slot.");
			pPlayer->BroadcastMessage("Transmog : Pick a slot where you can change item display");
			return;
		}
		int dst_item_id = it_dst->GetProto()->ItemId;
		char *SecondParam = FirstParam;
		while( *SecondParam != ' ' && *SecondParam != 0 )
			SecondParam++;
		if( *SecondParam == ' ' )
			SecondParam++;
		int src_item_id = atoi( SecondParam );
		if( CanErase == true && CaseInsensitiveParamCheck( SecondParam, "erase" ) == true )
		{
//			pPlayer->SetUInt32Value( PLAYER_VISIBLE_ITEM_1_ENTRYID + selected_slot * ( PLAYER_VISIBLE_ITEM_2_ENTRYID - PLAYER_VISIBLE_ITEM_1_ENTRYID ), dst_item_id );
			pPlayer->SetUInt32Value( PLAYER_VISIBLE_ITEM_1_ENTRYID + selected_slot * ( PLAYER_VISIBLE_ITEM_2_ENTRYID - PLAYER_VISIBLE_ITEM_1_ENTRYID ), 0 );
			return;
		}
		if( src_item_id == 0 )
		{
			pPlayer->BroadcastMessage("Transmog Error: Missing second parameter.Printing help");
			pPlayer->SetUInt32Value( PLAYER_VISIBLE_ITEM_1_ENTRYID + selected_slot * ( PLAYER_VISIBLE_ITEM_2_ENTRYID - PLAYER_VISIBLE_ITEM_1_ENTRYID ), dst_item_id );
			OnPlayerSetTransmog( pPlayer, selected_slot, -1 );
			goto JMP_INCORRECT_USAGE;
		} 
		if( src_item_id <= 0 )
		{
			pPlayer->SetUInt32Value( PLAYER_VISIBLE_ITEM_1_ENTRYID + selected_slot * ( PLAYER_VISIBLE_ITEM_2_ENTRYID - PLAYER_VISIBLE_ITEM_1_ENTRYID ), dst_item_id );
			pPlayer->BroadcastMessage("Transmog Error: Second parameter is missing or bad.");
			pPlayer->BroadcastMessage("Transmog : You can get itemID for an item name using website www.wowhead.com");
			return;
		}
		Item *it_src = pPlayer->GetItemInterface()->FindItemLessMax( src_item_id, 0, true );
		if( it_src == NULL )
			it_src = pPlayer->GetItemInterface()->FindItemLessMax( 200000 + src_item_id, 0, true );
		if( it_src == NULL )
		{
			pPlayer->BroadcastMessage("Transmog Error: You do not own an item with that ID.");
			pPlayer->BroadcastMessage("Transmog : you need to use an item ID that you own");
			return;
		}
		if( it_dst->GetProto() == NULL || it_src->GetProto() == NULL )
		{
			pPlayer->BroadcastMessage("Transmog Error: Server error.");
			return;
		}
		if( pPlayer->GetItemInterface()->CanEquipItemInSlot( -1, selected_slot, it_src->GetProto(), true, false ) != INV_ERR_OK )
		{
			pPlayer->BroadcastMessage("Transmog Error: You cannot equip that item in that slot.");
			pPlayer->BroadcastMessage("Transmog : You need to be able to equip item in selected slot");
			return;
		}
		uint32 dst_inv_type = it_dst->GetProto()->InventoryType;
		uint32 src_inv_type = it_src->GetProto()->InventoryType;
		uint32 dst_it_class = it_dst->GetProto()->Class;
		uint32 src_it_class = it_src->GetProto()->Class;
		uint32 src_it_subclass = it_src->GetProto()->SubClass;
		if( dst_inv_type != src_inv_type )
		{
			bool can_mix = false;
			if( src_inv_type == INVTYPE_WEAPON && ( dst_inv_type == INVTYPE_WEAPONMAINHAND || dst_inv_type == INVTYPE_WEAPONOFFHAND ) )
				can_mix = true;
			if( src_inv_type == INVTYPE_2HWEAPON && dst_inv_type == INVTYPE_2HWEAPON && ( dst_it_class == ITEM_SUBCLASS_WEAPON_TWOHAND_AXE || dst_it_class == ITEM_SUBCLASS_WEAPON_TWOHAND_MACE || dst_it_class == ITEM_SUBCLASS_WEAPON_TWOHAND_SWORD ) && ( src_it_class == ITEM_SUBCLASS_WEAPON_TWOHAND_AXE || src_it_class == ITEM_SUBCLASS_WEAPON_TWOHAND_MACE || src_it_class == ITEM_SUBCLASS_WEAPON_TWOHAND_SWORD ) )
				can_mix = true;
			if( src_inv_type == INVTYPE_2HWEAPON && dst_inv_type == INVTYPE_2HWEAPON && ( dst_it_class == ITEM_SUBCLASS_WEAPON_STAFF || dst_it_class == ITEM_SUBCLASS_WEAPON_POLEARM || dst_it_class == ITEM_SUBCLASS_SPEAR ) && ( src_it_class == ITEM_SUBCLASS_WEAPON_STAFF || src_it_class == ITEM_SUBCLASS_WEAPON_POLEARM || src_it_class == ITEM_SUBCLASS_SPEAR ) )
				can_mix = true;
			if( ( src_inv_type == INVTYPE_CHEST && dst_inv_type == INVTYPE_ROBE ) || ( src_inv_type == INVTYPE_ROBE && dst_inv_type == INVTYPE_CHEST ) )
				can_mix = true;
			if( ( dst_it_class == ITEM_SUBCLASS_WEAPON_AXE || dst_it_class == ITEM_SUBCLASS_WEAPON_MACE || dst_it_class == ITEM_SUBCLASS_WEAPON_SWORD || dst_it_class == ITEM_SUBCLASS_WEAPON_AXE ) && ( src_it_class == ITEM_SUBCLASS_WEAPON_AXE || src_it_class == ITEM_SUBCLASS_WEAPON_MACE || src_it_class == ITEM_SUBCLASS_WEAPON_SWORD ) )
				can_mix = true;
			if( ( dst_it_class == ITEM_SUBCLASS_DAGGER || dst_it_class == ITEM_SUBCLASS_FIST_WEAPON ) && ( src_it_class == ITEM_SUBCLASS_DAGGER || src_it_class == ITEM_SUBCLASS_FIST_WEAPON ) )
				can_mix = true;
			if( src_it_subclass == ITEM_SUBCLASS_ARMOR_COSMETIC_CLOTH_LEATHER_MAIL_PLATE && src_it_class == dst_it_class )
				can_mix = true;
			if( can_mix == false )
			{
				pPlayer->BroadcastMessage("Transmog Error: Item type mismatch.");
				pPlayer->BroadcastMessage("Transmog : Both items should be of same type");
				return;
			}
		}
		uint32 src_item_id_to_use = src_item_id;
		//is this a MOG only item created by us ? If so, try to use original that the client already has so we can see it on the character's back
		if( src_item_id > 200000 )
		{
			ItemPrototype *pProto = ItemPrototypeStorage.LookupEntry( src_item_id - 200000 );
			if( pProto != NULL && pProto->DisplayInfoID == it_src->GetProto()->DisplayInfoID )
				src_item_id_to_use = src_item_id - 200000;
		}
		//i hope it is safe to set the new item look ?
		pPlayer->SetUInt32Value( PLAYER_VISIBLE_ITEM_1_ENTRYID + selected_slot * ( PLAYER_VISIBLE_ITEM_2_ENTRYID - PLAYER_VISIBLE_ITEM_1_ENTRYID ), src_item_id_to_use );
		OnPlayerSetTransmog( pPlayer, selected_slot, src_item_id );
	}
}

void RegisterTransmog(ScriptMgr* mgr)
{
/*
CREATE TABLE `transmogs` (
  `OwnerGUID` int(10) DEFAULT NULL,
  `Slot` int(10) DEFAULT NULL,
  `Entry` int(10) DEFAULT NULL,
  KEY `Index1` (`OwnerGUID`,`Slot`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;
*/
	QueryResult *result = CharacterDatabase.Query( "Delete from Transmogs where OwnerGUID not in ( select guid from characters )" );
	if( result )
		delete result;
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_CHAT, &OnPlayerChatTransmog );
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_ENTER_WORLD, &OnPlayerEnterWorldTransmog );
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_LOGOUT, &OnPlayerLogoutTransmog );
}
