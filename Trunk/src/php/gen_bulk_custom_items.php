<?php
	
/*
enum INVENTORY_TYPES
{
	INVTYPE_NON_EQUIP       = 0x0,
	INVTYPE_HEAD            = 0x1,
	INVTYPE_NECK            = 0x2,
	INVTYPE_SHOULDERS       = 0x3,
	INVTYPE_BODY            = 0x4,
	INVTYPE_CHEST           = 0x5,
	INVTYPE_WAIST           = 0x6,
	INVTYPE_LEGS            = 0x7,
	INVTYPE_FEET            = 0x8,
	INVTYPE_WRISTS          = 0x9,
	INVTYPE_HANDS           = 0xa,
	INVTYPE_FINGER          = 0xb,
	INVTYPE_TRINKET         = 0xc,
	INVTYPE_WEAPON          = 0xd,
	INVTYPE_SHIELD          = 0xe,
	INVTYPE_RANGED          = 0xf,
	INVTYPE_CLOAK           = 0x10,
	INVTYPE_2HWEAPON        = 0x11,
	INVTYPE_BAG             = 0x12,
	INVTYPE_TABARD          = 0x13,
	INVTYPE_ROBE            = 0x14,
	INVTYPE_WEAPONMAINHAND  = 0x15,
	INVTYPE_WEAPONOFFHAND   = 0x16,
	INVTYPE_HOLDABLE        = 0x17,
	INVTYPE_AMMO            = 0x18,
	INVTYPE_THROWN          = 0x19,
	INVTYPE_RANGEDRIGHT     = 0x1a,
	INVTYPE_QUIVER          = 0x1b,
	INVTYPE_RELIC           = 0x1c,
	NUM_INVENTORY_TYPES     = 0x1d,
};

enum ITEM_CLASS
{
	ITEM_CLASS_CONSUMABLE	= 0,
	ITEM_CLASS_CONTAINER	= 1,
	ITEM_CLASS_WEAPON		= 2,
	ITEM_CLASS_JEWELRY		= 3,
	ITEM_CLASS_ARMOR		= 4,
	ITEM_CLASS_REAGENT		= 5,
	ITEM_CLASS_PROJECTILE	= 6,
	ITEM_CLASS_TRADEGOODS	= 7,
	ITEM_CLASS_GENERIC		= 8,
	ITEM_CLASS_RECIPE		= 9,
	ITEM_CLASS_MONEY		= 10,
	ITEM_CLASS_QUIVER		= 11,
	ITEM_CLASS_QUEST		= 12,
	ITEM_CLASS_KEY			= 13,
	ITEM_CLASS_PERMANENT	= 14,
	ITEM_CLASS_MISCELLANEOUS= 15,
	ITEM_CLASS_GLYPH        = 16,
};

enum Item_Subclass
{
   // Weapon
   ITEM_SUBCLASS_WEAPON_AXE					= 0,
   ITEM_SUBCLASS_WEAPON_TWOHAND_AXE			= 1,
   ITEM_SUBCLASS_WEAPON_BOW					= 2,
   ITEM_SUBCLASS_WEAPON_GUN					= 3,
   ITEM_SUBCLASS_WEAPON_MACE				= 4,
   ITEM_SUBCLASS_WEAPON_TWOHAND_MACE		= 5,
   ITEM_SUBCLASS_WEAPON_POLEARM				= 6,
   ITEM_SUBCLASS_WEAPON_SWORD				= 7,
   ITEM_SUBCLASS_WEAPON_TWOHAND_SWORD		= 8,
   ITEM_SUBCLASS_WEAPON_STAFF				= 10,
   ITEM_SUBCLASS_WEAPON_FIST_WEAPON			= 13,
   ITEM_SUBCLASS_WEAPON_MISC_WEAPON			= 14,
   ITEM_SUBCLASS_WEAPON_DAGGER				= 15,
   ITEM_SUBCLASS_WEAPON_THROWN				= 16,
   ITEM_SUBCLASS_WEAPON_CROSSBOW			= 18,
   ITEM_SUBCLASS_WEAPON_WAND				= 19,
   ITEM_SUBCLASS_WEAPON_FISHING_POLE		= 20,
   
   // Armor
   ITEM_SUBCLASS_ARMOR_MISC					= 0,
   ITEM_SUBCLASS_ARMOR_CLOTH				= 1,
   ITEM_SUBCLASS_ARMOR_LEATHER				= 2,
   ITEM_SUBCLASS_ARMOR_MAIL					= 3,
   ITEM_SUBCLASS_ARMOR_PLATE_MAIL			= 4,
   ITEM_SUBCLASS_ARMOR_SHIELD				= 6,
   ITEM_SUBCLASS_ARMOR_SIGIL                = 10,

   // Projectile
   ITEM_SUBCLASS_PROJECTILE_ARROW			= 2,
   ITEM_SUBCLASS_PROJECTILE_BULLET			= 3,
   
   // Trade goods
   ITEM_SUBCLASS_PROJECTILE_TRADE_GOODS		= 0,
   ITEM_SUBCLASS_PROJECTILE_PARTS			= 1,
   ITEM_SUBCLASS_PROJECTILE_EXPLOSIVES      = 2,
   ITEM_SUBCLASS_PROJECTILE_DEVICES			= 3,
   
   // Recipe
   ITEM_SUBCLASS_RECIPE_BOOK				= 0,
   ITEM_SUBCLASS_RECIPE_LEATHERWORKING		= 1,
   ITEM_SUBCLASS_RECIPE_TAILORING			= 2,
   ITEM_SUBCLASS_RECIPE_ENGINEERING			= 3,
   ITEM_SUBCLASS_RECIPE_BLACKSMITHING		= 4,
   ITEM_SUBCLASS_RECIPE_COOKING				= 5,
   ITEM_SUBCLASS_RECIPE_ALCHEMY				= 6,
   ITEM_SUBCLASS_RECIPE_FIRST_AID			= 7,
   ITEM_SUBCLASS_RECIPE_ENCHANTING			= 8,
   ITEM_SUBCLASS_RECIPE_FISNING				= 9,
	ITEM_SUBCLASS_MATERIAL                      = 13,
	ITEM_SUBCLASS_ARMOR_ENCHANTMENT             = 14,
	ITEM_SUBCLASS_WEAPON_ENCHANTMENT            = 15,

   // Quiver
   ITEM_SUBCLASS_QUIVER_AMMO_POUCH			= 3,
   ITEM_SUBCLASS_QUIVER_QUIVER				= 2,

   // Misc
   ITEM_SUBCLASS_MISC_JUNK					= 0,
}; */

	$itemClass = 4;
	$itemSubClass = 0;
	$itemInvetorySlot = 0x2;

	$dbhost="localhost";
	$dbuname="root";
	$dbupass="";
	$dbname="temp";

	$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
	mysql_select_db($dbname, $dbi) or die("Q 200603201239"); 

	$delayed_echo="";
	$get_query1 = "select item_id,display_id,lock_material,sheathid from custom_item_lookup where itemClassID='$itemClass' and itemSubClassID='$itemSubClass' and inventorySlotID='$itemInvetorySlot'";
//echo "$get_query1<br>";
	$get_res1=mysql_query($get_query1,$dbi) or die(" Q20061123150142 ".mysql_error($dbi));

	while(list($item_id,$display_id,$lock_material,$sheathid  )=mysql_fetch_row($get_res1))
	{
		echo "replace INTO items (entry,class,subclass,name1,displayid,inventorytype,lock_material,sheathID,quality,MaxDurability,buyprice,sellprice,dmg_min1,dmg_max1,armor,block,itemlevel,allowableclass,allowablerace) 
		VALUES 
			( $item_id, $itemClass,$itemSubClass,'bulky $item_id',$display_id,$itemInvetorySlot,$lock_material,$sheathid,1,2,3,4,5,6,7,8,9,-1,-1 );<br>";
		$delayed_echo .= ".char additem $item_id<br>";
	}

//	echo $delayed_echo;

	$get_res1=mysql_query($get_query1." group by display_id ",$dbi) or die(" Q20061123150142 ".mysql_error($dbi));
	while(list($item_id,$display_id,$lock_material,$sheathid  )=mysql_fetch_row($get_res1))
	{
		echo ".char additem $item_id<br>";
	}

	if( $dbi)
		mysql_close($dbi);
?>