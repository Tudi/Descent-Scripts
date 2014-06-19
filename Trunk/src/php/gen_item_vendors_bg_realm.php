<?php

$VENDOR_FIRST_ENTRY = $VENDOR_FIRST_ENTRY2 = 101000;
$ITEMS_PER_VENDOR = 119;
$VENDOR_NAME = "Reagent Vendor";
$vendor_look=array(20761,20705,29054,17374,18482,4532,1355,16203,17372,1859,3808,15516,18222,1882,4209,16610,3346,3798,1878,18324,11900,1728);
$vendor_look_count=22;
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="BG_world";
$creature_spawns_middle_x = -1863.74;
$creature_spawns_middle_y = 5429.98;
$creature_spawns_z = -11.8;
$creature_spawns_radius =  130; //50 is great

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

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
};

enum ITEM_QUALITY
{
	ITEM_QUALITY_POOR_GREY             = 0,
	ITEM_QUALITY_NORMAL_WHITE          = 1,
	ITEM_QUALITY_UNCOMMON_GREEN        = 2,
	ITEM_QUALITY_RARE_BLUE             = 3,
	ITEM_QUALITY_EPIC_PURPLE           = 4,
	ITEM_QUALITY_LEGENDARY_ORANGE      = 5,
	ITEM_QUALITY_ARTIFACT_LIGHT_YELLOW = 6,
	ITEM_QUALITY_HEIRLOOM_LIGHT_YELLOW = 7,
	ITEM_MAX_KNOWN_ITEM_QUALITY = ITEM_QUALITY_HEIRLOOM_LIGHT_YELLOW,
};
*/

//first col = item class
//second col = item subclass
//3rd col = inv type
//weapons
$item_vendor_names[2][0][-1]="Axe vendor";
$item_vendor_names[2][1][-1]="2H Axe vendor";
$item_vendor_names[2][2][-1]="Bow vendor";
$item_vendor_names[2][3][-1]="GUN vendor";
$item_vendor_names[2][4][-1]="Mace vendor";
$item_vendor_names[2][5][-1]="2H Mace vendor";
$item_vendor_names[2][6][-1]="Polearm vendor";
$item_vendor_names[2][7][-1]="Sword vendor";
$item_vendor_names[2][8][-1]="2H Sword vendor";
$item_vendor_names[2][10][-1]="Staff vendor";
$item_vendor_names[2][13][-1]="Fist vendor";
$item_vendor_names[2][15][-1]="Dagger vendor";
$item_vendor_names[2][16][-1]="Thrown vendor";
$item_vendor_names[2][18][-1]="Crossbow vendor";
$item_vendor_names[2][19][-1]="Wand vendor";

//armors
$item_vendor_names[4][-1][2]="Amulet vendor";
$item_vendor_names[4][-1][11]="Rings vendor";
$item_vendor_names[4][-1][12]="Trinket vendor";
$item_vendor_names[4][6][14]="Shield vendor";
$item_vendor_names[4][1][16]="Cloack vendor";
$item_vendor_names[4][-1][19]="Tabard vendor";
$item_vendor_names[4][-1][23]="Offhand vendor";
$item_vendor_names[4][-1][28]="Relics vendor";

$item_vendor_names[4][1][1]="Cloth Head vendor";
$item_vendor_names[4][1][3]="Cloth Shoulders vendor";
//$item_vendor_names[4][1][4]="Cloth Body vendor";
$item_vendor_names[4][1][5]="Cloth Chest vendor";
$item_vendor_names[4][1][6]="Cloth Waist vendor";
$item_vendor_names[4][1][7]="Cloth Legs vendor";
$item_vendor_names[4][1][8]="Cloth Feet vendor";
$item_vendor_names[4][1][9]="Cloth Wrists vendor";
$item_vendor_names[4][1][10]="Cloth Hands vendor";
$item_vendor_names[4][1][20]="Cloth Robe vendor";

$item_vendor_names[4][2][1]="Leather Head vendor";
$item_vendor_names[4][2][3]="Leather Shoulders vendor";
//$item_vendor_names[4][2][4]="Leather Body vendor";
$item_vendor_names[4][2][5]="Leather Chest vendor";
$item_vendor_names[4][2][6]="Leather Waist vendor";
$item_vendor_names[4][2][7]="Leather Legs vendor";
$item_vendor_names[4][2][8]="Leather Feet vendor";
$item_vendor_names[4][2][9]="Leather Wrists vendor";
$item_vendor_names[4][2][10]="Leather Hands vendor";
$item_vendor_names[4][2][20]="Leather Robe vendor";

$item_vendor_names[4][3][1]="Mail Head vendor";
$item_vendor_names[4][3][3]="Mail Shoulders vendor";
//$item_vendor_names[4][3][4]="Mail Body vendor";
$item_vendor_names[4][3][5]="Mail Chest vendor";
$item_vendor_names[4][3][6]="Mail Waist vendor";
$item_vendor_names[4][3][7]="Mail Legs vendor";
$item_vendor_names[4][3][8]="Mail Feet vendor";
$item_vendor_names[4][3][9]="Mail Wrists vendor";
$item_vendor_names[4][3][10]="Mail Hands vendor";
$item_vendor_names[4][3][20]="Mail Robe vendor";

$item_vendor_names[4][4][1]="Plate Head vendor";
$item_vendor_names[4][4][3]="Plate Shoulders vendor";
//$item_vendor_names[4][4][4]="Plate Body vendor";
$item_vendor_names[4][4][5]="Plate Chest vendor";
$item_vendor_names[4][4][6]="Plate Waist vendor";
$item_vendor_names[4][4][7]="Plate Legs vendor";
$item_vendor_names[4][4][8]="Plate Feet vendor";
$item_vendor_names[4][4][9]="Plate Wrists vendor";
$item_vendor_names[4][4][10]="Plate Hands vendor";
$item_vendor_names[4][4][20]="Plate Robe vendor";

//arrows
$item_vendor_names[6][2][24]="Arrows vendor";
//bullet
$item_vendor_names[6][3][24]="Bullet vendor";
//bullet bag
//$item_vendor_names[11][3][18]="Ammo pouch vendor";
//arrow bag
//$item_vendor_names[11][2][18]="Quiver vendor";
$item_vendor_names[11][-1][18]="Ammo bag vendor";

//potions 
$item_vendor_names[0][1][-1]="Potions vendor";
$item_needs_spells[0][1][-1]=1;
//elixir
$item_vendor_names[0][2][-1]="Elixir vendor";
$item_needs_spells[0][2][-1]=1;
//flask
$item_vendor_names[0][3][-1]="Flask vendor";
$item_needs_spells[0][3][-1]=1;
//food
$item_vendor_names[0][5][-1]="Food Drink vendor";
$item_needs_spells[0][5][-1]=1;

//bandages
$item_vendor_names[0][7][-1]="Bandage vendor";
$item_needs_spells[0][7][-1]=1;

//gems
$item_vendor_names[3][-1][-1]="Gems vendor";

//scrolls
$item_vendor_names[0][4][-1]="Scrolls vendor";
$item_needs_spells[0][4][-1]=1;

//enchants
$item_vendor_names[0][6][-1]="Enchants vendor";
$item_needs_spells[0][6][-1]=1;
$item_vendor_names[0][8][-1]="Enchants vendor";
$item_needs_spells[0][8][-1]=1;
$item_vendor_names[12][0][-1]="Enchants vendor";
$item_needs_spells[12][0][-1]=1;
$item_needs_enchants[12][0][-1]=1;
$item_needs_flags [12][0][-1]=64 | 1;
$item_avoid_flags [12][0][-1]=2048 | 0x8000 | 0x10000 | 0x20000;
//$item_flags_explicit[12][0][-1]=65;

//Mounts
$item_vendor_names[15][5][-1]="Mounts vendor";
$item_needs_spells[15][5][-1]=1;

//Tabards
$item_vendor_names[4][0][19]="Tabard vendor";

//Glyphs
$item_vendor_names[16][-1][-1]="Glyph vendor";
$item_needs_spells[16][-1][-1]=1;

//companions - non combat pets
$item_vendor_names[15][2][-1]="Companion vendor";
//not working : 44820,22781,21308,19055,19054,18964,44822,54436,49343,45606,45057,37298,34519,34518,34425,32498,54847,53641,50446,49663,49662,38050,35350,35349,33818,33816,38658,38628,35504,35227,34493,23712
$item_vendor_list[15][2][-1]="34493,8494,23713,49287,34478,39973,13582,13583,13584,20371,22114,25535,32233,32616,32617,32622,33154,33993,34955,39286,39656,41133,43698,44723,44794,44810,45180,46707,46767,46802,49646,49665,49693,49912,23083,30360,32588,34492,37297,40653,44738,44841,44965,44970,44971,44973,44974,44980,44982,44983,44984,44998,45002,45022,46544,46545,46820,46821,31760,27445,29363,29364,29902,29958,21277,12264,12529,20769,15996,11474,11825,11826,10398,11023,11026,11027,19450,19462,8498,8499,8500,8501,10360,10361,10392,10393,10394,10822,34535,46398,48112,48114,48116,48118,48120,48122,48124,48126,49362,8485,8486,8487,8488,8489,8490,8491,8492,8495,8496,8497,21168,29901,29903,29904,29953,29956,29957,29960,45942,4401,11110,21301,21305,21309,22200,22235,23002,23007,23015,37460,44819,46396,46397,46831,39896,39898,39899,44721";

//guardians 
$item_vendor_names[15][0][-1]="Combat pets vendor";
$item_vendor_list[15][0][-1]="3456,15778,21325,23379,37464,34113,18232,18297,25883,4391,5332,37815,37859,37860,40110,44844,25882,25881,25880,25498";

//get some weapons and shields for the vendors
$mob_weapons="";
$mob_weapons_coutner=0;
//$get_query = "select entry,name1 from items where class=2 and subclass in (0,4,7) group by displayid order by itemlevel desc; ";
$get_query = "select entry,name1 from items where class=2 and subclass in (0,4,7,15) group by displayid order by itemlevel desc; ";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while($row=mysql_fetch_row($get_res) )
	$mob_weapons[$mob_weapons_coutner++]=$row[0];
$mob_shields="";
$mob_shields_coutner=0;
$get_query = "select entry,name1 from items where class=4 and subclass in (6) and inventorytype=14  group by displayid order by itemlevel desc; ";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while($row=mysql_fetch_row($get_res) )
	$mob_shields[$mob_shields_coutner++]=$row[0];
$mob_2H="";
$mob_2H_coutner=0;
$get_query = "select entry,name1 from items where class=2 and subclass in (1,2,3,5,6,8,10,18) group by displayid order by itemlevel desc; ";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while($row=mysql_fetch_row($get_res) )
	$mob_2H[$mob_2H_coutner++]=$row[0];

$display_randomizer=0;
for($class=-1;$class<50;$class++)
	for($subclass=-1;$subclass<50;$subclass++)
		for($inventorytype=-1;$inventorytype<50;$inventorytype++)
			if($item_vendor_names[$class][$subclass][$inventorytype] )
			{
				$display = $vendor_look[$display_randomizer % $vendor_look_count];
				$display_randomizer++;
				//echo $unique_list."<br>";
				$get_query = "select entry,name1,quality*quality*itemlevel*2/10,spellid_1,spellid_2,spellid_3,spellid_4,spellid_5 from items where 1=1 ";
				if( $item_vendor_list[$class][$subclass][$inventorytype] )
					$get_query .= " and entry in (".$item_vendor_list[$class][$subclass][$inventorytype].") ";
				else
				{
					if( $class != -1 )
						$get_query .= " and class=$class ";
					if( $subclass != -1 )
						$get_query .= " and subclass=$subclass ";
					if( $inventorytype != -1 )
						$get_query .= " and inventorytype=$inventorytype ";
					if( $item_needs_spells[$class][$subclass][$inventorytype] )
						$get_query .= " and (spellid_1+spellid_2+spellid_3+spellid_4+spellid_5)>0 ";
					if( $item_needs_flags[$class][$subclass][$inventorytype] )
						$get_query .= " and (flags & ".$item_needs_flags[$class][$subclass][$inventorytype].")!=0 ";
					if( $item_avoid_flags[$class][$subclass][$inventorytype] )
						$get_query .= " and (flags & ".$item_avoid_flags[$class][$subclass][$inventorytype].")=0 ";
					if( $item_flags_explicit[$class][$subclass][$inventorytype] )
						$get_query .= " and flags=".$item_flags_explicit[$class][$subclass][$inventorytype].")=0 ";
					//." and quality=1 "
					//." and !(flags & 2) "
					$get_query .= " and (quality > 0 )";
					$get_query .= " and itemlevel > 10 ";
					$get_query .= " and (flags & (4 | 2 | 0x200 | 0x400 | 0x4000 | 0x40000 | 0x20000000))=0 ";
					$get_query .= " and !(class=4 and subclass != 0 and (stat_value1+stat_value2+stat_value3+stat_value4+stat_value5+stat_value6+stat_value7+stat_value8+stat_value9+stat_value10+dmg_max1+dmg_max2+armor+holy_res+fire_res+nature_res+frost_res+shadow_res+arcane_res+spellid_1+spellid_2+spellid_3+spellid_4+spellid_5)<10) ";
					$get_query .= " and !(class=2 and (stat_value1+stat_value2+stat_value3+stat_value4+stat_value5+stat_value6+stat_value7+stat_value8+stat_value9+stat_value10+dmg_max1+dmg_max2+armor+holy_res+fire_res+nature_res+frost_res+shadow_res+arcane_res+spellid_1+spellid_2+spellid_3+spellid_4+spellid_5)<10) ";
					$get_query .= " and ( ContainerSlots = 0 or inventorytype=18)";
					//these might be custom items. Better not use them
				}
				$get_query .= " and entry not in ( 89,93,100,101,105,123,133,134,138,836,876,913,951,996,997,1024,1026,1027,1029,1041,1057,1133,1134,1146,1162,1255,1258,1298,1324,1350,1356,1403,1450,1544,1622,1623,1700,1914,1957,1961,1984,2012,2016,2128,2248,2404,2405,2410,2413,2415,2442,2443,2444,2481,2482,2483,2484,2485,2486,2487,2496,2497,2498,2500,2501,2502,2503,2556,2588,2599,2600,2602,2638,2693,2803,2804,2808,2891,2920,2921,2922,2929,2932,3002,3003,3004,3005,3034,3068,3144,3222,3507,3512,3536,3648,3675,3677,3762,4033,4143,4191,4193,4196,4273,4295,4930,4959,4981,4988,4989,4990,4997,5000,5004,5008,5010,5013,5024,5090,5108,5150,5222,5226,5229,5235,5330,5333,5378,5562,5563,5603,5657,5660,5663,5768,5769,5823,5828,5858,5859,5874,5875,5878,6075,6116,6131,6142,6183,6216,6273,6276,6277,6278,6279,6280,6374,6376,6478,6490,6491,6492,6495,6496,6497,6498,6500,6501,6516,6544,6589,6619,6623,6648,6649,6673,6698,6754,6891,6987,6988,7188,7426,7427,7466,7467,7497,7547,7548,7550,7681,7716,7948,7949,7950,7951,7952,7977,7986,7987,7988,7994,7996,8147,8388,8583,8589,8590,8627,8630,8633,8688,8840,8964,9325,9380,9417,9443,9572,9685,9700,9701,9888,10049,10595,10683,11170,12104,12106,12107,12750,12751,12817,12866,12970,13080,13090,13092,13242,13247,13325,13338,13612,13642,13643,13812,13906,13936,15769,16141,16159,16165,16181,16315,16334,16336,16337,16340,17024,17027,17142,17363,18002,18153,18155,18438,18970,19642,19879,20591,20596,20880,21044,21044,21441,21441,21794,21798,21794,21798,22765,22805,22822,22765,22805,22896,22822,22896,23051,23227,23457,23462,23578,23579,23741,23743,24228,24229,24288,24442,24444,25581,25588,25641,25747,25840,26128,26129,26130,26131,26132,26133,26134,26135,26513,26527,26541,26569,26765,26779,27002,27007,27419,27421,28048,28107,28598,28648,29188,29206,29311,29819,31265,31802,31824,32148,32448,32730,32972,33080,33147,33338,34103,34104,40604,41118,44233,5632,12947,19868,5634 )";
				//$get_query .= " order by name1 asc ";
				$get_query .= " order by quality*quality*itemlevel asc ";
				//echo $get_query."<br>";
				$sell_count = 1;
				$max_count = 1;
				//projectiles
				if( $class==6 && $subclass==2 && $inventorytype==24 )
				{
					$sell_count = 1000;
					$max_count = 500;
				}
				if( $class==6 && $subclass==3 && $inventorytype==24 )
				{
					$sell_count = 1000;
					$max_count = 500;
				}
				//scrolls
				else if( $class==0 && $subclass==4 )
				{
					$sell_count = 100;
					$max_count = 100;
				}
				//potions
				else if( $class==0 && $subclass==1 )
				{
					$sell_count = 10;
					$max_count = 10;
				}
				//elixir
				else if( $class==0 && $subclass==2 )
				{
					$sell_count = 10;
					$max_count = 10;
				}
				//flask
				else if( $class==0 && $subclass==3 )
				{
					$sell_count = 10;
					$max_count = 10;
				}
				//food
				else if( $class==0 && $subclass==5 )
				{
					$sell_count = 10;
					$max_count = 10;
				}
				//bandage
				else if( $class==0 && $subclass==7 )
				{
					$sell_count = 40;
					$max_count = 10;
				}
				$counter=0;
				$vendors_made=0;
				$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
				while($row=mysql_fetch_row($get_res) )
				{
					//check if item has valid spells 
					if($item_needs_spells[$class][$subclass][$inventorytype] )
					{
						$has_spells=0;
						$has_enchanting_spell=0;
						for($sk=3;$sk<=7;$sk++)
							if( $row[$sk]!=0 && $row[$sk]!=-1)
							{
								$get_query = "select effect_1,effect_2,effect_3 from temp.dbc_spell where id={$row[$sk]} ";
								$get_res2=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
								$row2=mysql_fetch_row($get_res2);
								if( $row2[0] != 0 )
								{
									$has_spells=1;
									if( $row2[0] == 53 || $row2[1] == 53 || $row2[2] == 53 
										|| $row2[0] == 54 || $row2[1] == 54 || $row2[2] == 54
										|| $row2[0] == 92 || $row2[1] == 92 || $row2[2] == 92
										)
										$has_enchanting_spell=1;
									break;
								}
							}
						if($has_spells==0 || ( $item_needs_enchants[$class][$subclass][$inventorytype]>0 && $has_enchanting_spell==0) )
							continue;
					}
					if( $counter == 0 )
					{
						echo "delete from vendors where entry=$VENDOR_FIRST_ENTRY;<br>";
						$first_name = $row[1];
						$first_level = (int)$row[2];
					}
//					echo "insert into vendors (entry,item,amount,max_amount,inctime) values ($VENDOR_FIRST_ENTRY,".$row[0].",$sell_count,$max_count,".(1000*60*5)."); <br>";
					echo "insert into vendors (entry,item,amount) values ($VENDOR_FIRST_ENTRY,".$row[0].",$sell_count); <br>";
#echo $row[0]."-".$row[1]."<br>";
					$counter++;
					$last_name=$row[1];
					$last_level = (int)$row[2];
					if($counter >= $ITEMS_PER_VENDOR)
					{
						echo "replace into creature_names (entry,name,subname,info_str,type,male_displayid) values ( $VENDOR_FIRST_ENTRY,'".$item_vendor_names[$class][$subclass][$inventorytype]."','".$first_level."-".$last_level."','Buy',7,$display);<br>";
						echo "replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $VENDOR_FIRST_ENTRY,90,90,35,100000000,100000000,1,129+4096,1);<br>";
						$VENDOR_FIRST_ENTRY++;
						$counter = 0;
						$vendors_made++;
					}
				}
				//maybe last vendor does not exactly have N items
				if($counter != 0 && $counter < $ITEMS_PER_VENDOR)
				{
					echo "replace into creature_names (entry,name,subname,info_str,type,male_displayid) values ( $VENDOR_FIRST_ENTRY,'".$item_vendor_names[$class][$subclass][$inventorytype]."','".$first_level."-".$last_level."','Buy',7,$display);<br>";
					echo "replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $VENDOR_FIRST_ENTRY,90,90,35,100,100,1,129+4096,1);<br>";
					$VENDOR_FIRST_ENTRY++;
					$counter = 0;
					$vendors_made++;
				}
			}

if( $dbi)
	mysql_close($dbi);

echo "delete from creature_spawns where entry>=$VENDOR_FIRST_ENTRY2 and entry< ".($VENDOR_FIRST_ENTRY2+300).";<br>";
//now to spawns them
$count_to_spawn = $VENDOR_FIRST_ENTRY - $VENDOR_FIRST_ENTRY2;
$angle_step = 360 / $count_to_spawn;
$facing_angle_step = 6.28 / $count_to_spawn;
$r_itr=0;
$n_itr=0;
for($i=0;$i<$count_to_spawn;$i++)
{
	$angle_now = $angle_step * $i;
	$radian_angle = $angle_now / 180 * 3.14;
	$facing = 3.14 + $i * $facing_angle_step;
	$x_now = $creature_spawns_middle_x + $creature_spawns_radius * cos( $radian_angle );
	$y_now = $creature_spawns_middle_y + $creature_spawns_radius * sin( $radian_angle );
	$entry=$i+$VENDOR_FIRST_ENTRY2;
	$display = $vendor_look[$i % $vendor_look_count];
	if( $i % 2 == 0 )
		echo "insert into creature_spawns (entry,map,position_x,position_y,position_z,orientation,faction,slot1item,slot2item,slot3item,bytes2) values ($entry,530,$x_now,$y_now,$creature_spawns_z,$facing,35,".$mob_2H[$r_itr++].",0,0,1);<br>";
	else
		echo "insert into creature_spawns (entry,map,position_x,position_y,position_z,orientation,faction,slot1item,slot2item,slot3item,bytes2) values ($entry,530,$x_now,$y_now,$creature_spawns_z,$facing,35,".$mob_weapons[$n_itr].",".$mob_shields[$n_itr++].",0,1);<br>";
}

?>