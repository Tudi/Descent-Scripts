<?php
set_time_limit( 15*60 );
	
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$already_have="			case 11: // mount : Reins of the Drake of the East Wind
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_FLAWLESS_VICTOR;
				break;
			case 182: // Title Reward: Champion of the Frozen Wastes
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_CHAMPION_OF_THE_FROZEN_WASTES;
				break;
			case 553: // Title Reward: Guardian of Cenarius
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_GUARDIAN_OF_CENARIUS;
				break;
			case 580: // Title Reward: Salty
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_SALTY;
				break;
			case 1672: // Title: Of the Ashen Verdict
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_ASHEN_VERDICT;
				break;
			case 1522: // Reward: Little Fawn's Salt Lick
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 44841;
				break;
/*			case 1522: // Reward: Reeking Pet Carrier
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 40653;
				break;*/
			case 1644: // Reward: Title & Loremaster's Colors
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM | ACHIEVEMENT_REWARDTYPE_TITLE;
				r.itemId = 43300;
				r.rankId = PVPTITLE_LOREMASTER;
				break;
			case 3936: // Reward: Pilgrim Title & Plump Turkey Pte
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL | ACHIEVEMENT_REWARDTYPE_TITLE;
				r.spellId = 61773;
				r.rankId = PVPTITLE_THE_PILGRIM;
				break;
			case 1700: // Title Reward: The Magic Seeker
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_MAGIC_SEEKER;
				break;
			case 1703: // Reward: Black War Bear [Horde]
				// spellId 60018 or 60019 ?
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 44224;
				break;
			case 1704: // Reward: Black War Bear [Alliance]
				// spellId 60018 or 60019 ?
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 44223;
				break;
			case 1865: // Reward: The Schools of Arcane Magic - Mastery
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 59983;
				break;
			case 4007: // Reward: Perky Pug
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 70613;
				break;
			case 4156: // Reward: Icebound Frostbrood Vanquisher
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 72807;
				break;
			case 1898: // Title Reward: Conqueror of Naxxramas
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_CONQUEROR_OF_NAXXRAMAS;
				break;
			case 4148: // Title: Bane of the Fallen King
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_BANE_OF_FALLEN_KING;
				break;
			case 4179: // Title: The Kingslayer
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_KINGSLAYER;
				break;
			case 2044: // Reward: Black Proto-Drake
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 59976;
				break;
			case 2136: // Title Reward: Elder
			case 3698: // Title Reward: Elder
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_ELDER;
				break;
			case 2139: // Title Reward: The Argent Champion
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_ARGENT_CHAMPION;
				break;
			case 2143: // Title Reward: Conqueror of Ulduar
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_CONQUEROR_OF_ULDUAR;
				break;
			case 2292: // Title Reward: Champion of Ulduar
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_CHAMPION_OF_ULDUAR;
				break;
/*			case 2143: // Title Reward: The Immortal
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_IMMORTAL;
				break; 
			case 2292: // Title Reward: The Undying
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_UNDYING;
				break; */
			case 2421: // Title: Bloodsail Admiral
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_BLOODSAIL_ADMIRAL;
				break;
			case 2515: // Title Reward: Brewmaster
			case 3697: // Title Reward: Brewmaster
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_BREWMASTER;
				break;
			case 2523: // Title Reward: Matron/Patron
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = (getGender()==1) ? /* Matron */ PVPTITLE_MATRON : /* Patron */ PVPTITLE_PATRON;
				break;
			case 2563: // Title Reward: Conqueror
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_CONQUEROR;
				break;
			case 2737: // Title Reward: The Diplomat
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_DIPLOMAT;
				break;
			case 2759: // Title Reward: The Explorer
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_EXPLORER;
				break;
			case 2924: // Title Reward: Justicar
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_JUSTICAR;
				break;
			case 2974: // Title Reward: Flame Warden
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_FLAME_WARDEN;
				break;
			case 2975: // Title Reward: Flame Keeper
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_FLAME_KEEPER;
				break;
			case 2821: // Title: the Light of Dawn
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_LIGHT_OF_DAWN;
				break;
			case 2938: // Title Reward: The Insane
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_INSANE;
				break;
			case 2993: // Reward: Titanium Seal of Dalaran
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 60650;
				break;
			case 2328: // Reward: Blue Dragonhawk Mount
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 61996;
				break;
			case 3203: // Reward: Bloodbathed Frostbrood Vanquisher
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 72808;
				break;
			case 3040: // Reward: Tabard of the Achiever
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 40643;
				break;
			case 3387: // Reward: Tabard of Conquest
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = GetTeam() ? 49054 : 49052;
				break;
			case 3092: // Title Reward: Merrymaker
			case 3695: // Title Reward: Merrymaker
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_MERRYMAKER;
				break;
			case 3192: // Title Reward: The Love Fool
			case 3699: // Title Reward: The Love Fool
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_LOVE_FOOL;
				break;
			case 3795: // Title Reward: of Silvermoon
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_OF_SILVERMOON;
				break;
			case 3796: // Title Reward: of the Exodar
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_OF_THE_EXODAR;
				break;
			case 3797: // Title Reward: of Ironforge
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_OF_IRONFORGE;
				break;
			case 3798: // Title Reward: of the Undercity
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_OF_THE_UNDERCITY;
				break;
			case 3799: // Title Reward: of Gnomeregan
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_OF_GNOMEREGAN;
				break;
			case 3800: // Title Reward: of Stormwind
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_OF_STORMWIND;
				break;
			case 3801: // Title Reward: of Darnassus
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_OF_DARNASSUS;
				break;
			case 3802: // Title Reward: of Orgrimmar
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_OF_ORGRIMMAR;
				break;
			case 3803: // Title Reward: of Orgrimmar
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_OF_THUNDERBLUFF;
				break;
			case 3804: // Title Reward: of Sen'jin
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_OF_SENJIN;
				break;
			case 3805: // Title Reward: The Grand Crusader
			case 3806: // Title Reward: Crusader. Unlocks Crusader dailies at the Argent Tournament.
			case 3807: // Title Reward: Crusader. Unlocks Crusader dailies at the Argent Tournament.
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_GRAND_CRUSADER;
				break;
			case 3848: // Title Reward: Death's Demise
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_DEATHS_DEMISE;
				break;
			case 3869: // Title Reward: The Celestial Defender
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_CELESTIAL_DEFENDER;
				break;
			case 3049: // Title: the Patient
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_PATIENT;
				break;
			case 3202: // Title Reward: the Noble
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_NOBLE;
				break;
			case 3252: // Title Reward: Of the Nightfall [Normal] Title Reward: Twilight Vanquisher [Heroic]
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = (entry->ID == 2051) ? /* Normal */ PVPTITLE_OF_THE_NIGHTFALL : /* Heroic ID==2054 */ PVPTITLE_TWILIGHT_VANQUISHER;
				break;
			case 3308: // Title Reward: Obsidian Slayer
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_OBSIDIAN_SLAYER;
				break;
			case 3372: // Title Reward: Battlemaster
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_BATTLEMASTER;
				break;
			case 3373: // Title Reward: Battlemaster
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_BATTLEMASTER;
				break;
			case 3374: // Title Reward: Ambassador
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_AMBASSADOR;
				break;
			case 3375: // Title Reward: Ambassador
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_AMBASSADOR;
				break;
			case 3414: // Title Reward: The Seeker
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_SEEKER;
				break;
			case 3444: // Reward: Albino Drake
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 60025;
				break;
			case 3453: // Title Reward: Of the Horde or Of the Alliance
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = (GetTeam()==1) ? /* Horde */ PVPTITLE_OF_THE_HORDE : /* Alliance */ PVPTITLE_OF_THE_ALLIANCE;
				break;
			case 3493: // Reward: Tabard of the Explorer
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 43348;
				break;
			case 3494: // Reward: Red Proto-Drake
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 59961;
				break;
			case 3876: // Reward: Rusted Proto-Drake 
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 63963;
				break;
			case 3877: // Reward: Ironbound Proto-Drake 
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 63956;
				break;
			case 3495: // Title: Jenkins
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_JENKINS;
				break;
			case 3499: // Reward: Plagued Proto-Drake
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 60021;
				break;
			case 3500: // Reward: Violet Proto-Drake
				r.type = ACHIEVEMENT_REWARDTYPE_SPELL;
				r.spellId = 60024;
				break;
			case 3514: // Title Reward: The Hallowed
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_HALLOWED;
				break;
			case 3595: // Reward: Tabard of Brute Force
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 43349;
			case 3600: // Title Reward: Arena Master
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_ARENA_MASTER;
				break;
			case 3602: // Title Reward: The Exalted
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_EXALTED;
				break;
			case 3678: // Title Reward: Chef
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_CHEF;
				break;
			case 4459: // Reward: Reins of the Drake of the East Wind
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 62901;
				break;
			case 4173: // Reward: Reins of the Dark Phoenix
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 63125;
				break;
			case 5278: // Reward: Guild Vault Voucher (8th Tab)
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 62287;
				break;
			case 5277: // Reward: Reins of the Golden King
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 62298;
				break;
			case 5158: // Title Reward: The Bloodthirsty
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_THE_BLOODTHIRSTY;
				break;
			case 5078: // Title Reward: Dragonslayer
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_DRAGONSLAYER;
				break;
			case 5077: // Title Reward: Blackwing's Bane
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_BLACKWING_S_BANE;
				break;
			case 5043: // Title Reward: Defender of a Shattered World
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_DEFENDER_OF_A_SHATTERED_WORLD;
				break;
			case 4730: // Title Reward: Professor
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_PROFESSOR;
				break;
			case 4729: // Title Reward: Associate Professor
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_ASOCIATE_PROFESSOR;
				break;
			case 4728: // Title Reward: Assistant Professor
				r.type = ACHIEVEMENT_REWARDTYPE_TITLE;
				r.rankId = PVPTITLE_ASSISTANT_PROFESSOR;
				break;
			case 4430: // Reward: Reins of the Volcanic Stone Drake
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 62900;
				break;
			case 5538: // Reward: Dragonwrath Whelpling
				r.type = ACHIEVEMENT_REWARDTYPE_ITEM;
				r.itemId = 71033;
				break;";
$have_ids_parts = explode( "case ", $already_have );
foreach( $have_ids_parts as $key => $t_entry )
{
	$subpart = explode( ":", $t_entry );
	if( $subpart[0] > 0 )
		$have_ids .= $subpart[0].",";
}
$have_ids .= "0";	

$query = "select col_10,col_11,col_0,col_4 from achievementdbccsv where col_11 != '' and col_10 != 0 and col_10 not in ($have_ids) order by col_10";
#echo "$query <br>";

#die();
//database connection 
 $dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
 mysql_select_db($dbname, $dbi) or die("Q 200603201239".mysql_error($dbi));

$get_res1=mysql_query($query,$dbi) or die(" Q200901211542 ".mysql_error($dbi)." $query<br>");
while(	list($unk,$name,$entry,$aname)=mysql_fetch_row($get_res1) )
{
	echo "- Achievement reward handler for : $name ( $entry ) <br>";
/*	
	//do we have this already but with a bad entry ?
	if( strpos( $already_have, $name ) )
	{
//		$already_have = str_replace( $name );
		echo "!!!!!!!already have $name <br>";
	}
	echo "case $unk: // $name - $entry - $aname<br>";
	if( strpos( "#".$name,"Title" ) != 0 )
	{
		echo "r.type = ACHIEVEMENT_REWARDTYPE_TITLE;<br>";
		echo "r.rankId = ;<br>";
	}
	else if( strpos( "#".$name,"Reward" ) != 0 )
	{
		//maybe there is an item here ?
		$item_name = explode( ": ",$name);
		$item_name = $item_name[1];
		$item_name = explode( "(",$item_name);
		$item_name = $item_name[0];
		$item_name = mysql_escape_string( $item_name );
		$query = "select count(*) from wowbeez_db.items where name1 like '%$item_name%'";
//		echo $query."<br>";
		$get_res2=mysql_query($query,$dbi) or die(" Q200901211542 ".mysql_error($dbi)." $query<br>");
		list($count)=mysql_fetch_row($get_res2);
		if( $count == 1 )
		{
			$query = "select entry from wowbeez_db.items where name1 like '%$item_name%'";
			$get_res2=mysql_query($query,$dbi) or die(" Q200901211542 ".mysql_error($dbi)." $query<br>");
			list($item)=mysql_fetch_row($get_res2);
			echo "r.type = ACHIEVEMENT_REWARDTYPE_ITEM;<br>";
			echo "r.itemId = $item;<br>";
		}
	}
	echo "break;<br>";
*/	
}
?>