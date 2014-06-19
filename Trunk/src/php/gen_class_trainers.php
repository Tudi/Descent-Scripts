<?php
set_time_limit (3000);

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$trainer_entry = 101000;

$SKILL_FROST                  = 6;
$SKILL_FIRE                   = 8;
$SKILL_ARMS                   = 26;
$SKILL_COMBAT                 = 38;
$SKILL_SUBTLETY               = 39;
$SKILL_SWORDS                 = 43;
$SKILL_AXES                   = 44;
$SKILL_BOWS                   = 45;
$SKILL_GUNS                   = 46;
$SKILL_BEAST_MASTERY          = 50;
$SKILL_SURVIVAL               = 51;
$SKILL_MACES                  = 54;
$SKILL_2H_SWORDS              = 55;
$SKILL_HOLY                   = 56;
$SKILL_SHADOW_MAGIC           = 78;
$SKILL_DEFENSE                = 95;
$SKILL_LANG_COMMON            = 98;
$SKILL_RACIAL_DWARVEN         = 101;
$SKILL_LANG_ORCISH            = 109;
$SKILL_LANG_DWARVEN           = 111;
$SKILL_LANG_DARNASSIAN        = 113;
$SKILL_LANG_TAURAHE           = 115;
$SKILL_DUAL_WIELD             = 118;
$SKILL_RACIAL_TAUREN          = 124;
$SKILL_RACIAL_ORC             = 125;
$SKILL_RACIAL_NIGHT_ELF       = 126;
$SKILL_FIRST_AID              = 129;
$SKILL_FERAL_COMBAT           = 134;
$SKILL_STAVES                 = 136;
$SKILL_LANG_THALASSIAN        = 137;
$SKILL_LANG_DRACONIC          = 138;
$SKILL_LANG_DEMON_TONGUE      = 139;
$SKILL_LANG_TITAN             = 140;
$SKILL_LANG_OLD_TONGUE        = 141;
$SKILL_SURVIVAL2               = 142;
$SKILL_HORSE_RIDING           = 148;
$SKILL_WOLF_RIDING            = 149;
$SKILL_TIGER_RIDING           = 150;
$SKILL_RAM_RIDING             = 152;
$SKILL_SWIMMING               = 155;
$SKILL_2H_MACES               = 160;
$SKILL_UNARMED                = 162;
$SKILL_MARKSMANSHIP           = 163;
$SKILL_BLACKSMITHING          = 164;
$SKILL_LEATHERWORKING         = 165;
$SKILL_ALCHEMY                = 171;
$SKILL_2H_AXES                = 172;
$SKILL_DAGGERS                = 173;
$SKILL_THROWN                 = 176;
$SKILL_HERBALISM              = 182;
$SKILL_GENERIC_DND            = 183;
$SKILL_RETRIBUTION            = 184;
$SKILL_COOKING                = 185;
$SKILL_MINING                 = 186;
$SKILL_PET_IMP                = 188;
$SKILL_PET_FELHUNTER          = 189;
$SKILL_TAILORING              = 197;
$SKILL_ENGINEERING            = 202;
$SKILL_PET_SPIDER             = 203;
$SKILL_PET_VOIDWALKER         = 204;
$SKILL_PET_SUCCUBUS           = 205;
$SKILL_PET_INFERNAL           = 206;
$SKILL_PET_DOOMGUARD          = 207;
$SKILL_PET_WOLF               = 208;
$SKILL_PET_CAT                = 209;
$SKILL_PET_BEAR               = 210;
$SKILL_PET_BOAR               = 211;
$SKILL_PET_CROCOLISK          = 212;
$SKILL_PET_CARRION_BIRD       = 213;
$SKILL_PET_CRAB               = 214;
$SKILL_PET_GORILLA            = 215;
$SKILL_PET_RAPTOR             = 217;
$SKILL_PET_TALLSTRIDER        = 218;
$SKILL_RACIAL_UNDEAD          = 220;
$SKILL_CROSSBOWS              = 226;
$SKILL_WANDS                  = 228;
$SKILL_POLEARMS               = 229;
$SKILL_PET_SCORPID            = 236;
$SKILL_ARCANE                 = 237;
$SKILL_PET_TURTLE             = 251;
$SKILL_ASSASSINATION          = 253;
$SKILL_FURY                   = 256;
$SKILL_PROTECTION             = 257;
$SKILL_PROTECTION2            = 267;
$SKILL_PET_GENERIC_HUNTER     = 270;
$SKILL_PLATE_MAIL             = 293;
$SKILL_LANG_GNOMISH           = 313;
$SKILL_LANG_TROLL             = 315;
$SKILL_ENCHANTING             = 333;
$SKILL_DEMONOLOGY             = 354;
$SKILL_AFFLICTION             = 355;
$SKILL_FISHING                = 356;
$SKILL_ENHANCEMENT            = 373;
$SKILL_RESTORATION            = 374;
$SKILL_ELEMENTAL_COMBAT       = 375;
$SKILL_SKINNING               = 393;
$SKILL_MAIL                   = 413;
$SKILL_LEATHER                = 414;
$SKILL_CLOTH                  = 415;
$SKILL_SHIELD                 = 433;
$SKILL_FIST_WEAPONS           = 473;
$SKILL_RAPTOR_RIDING          = 533;
$SKILL_MECHANOSTRIDER_PILOTING= 553;
$SKILL_UNDEAD_HORSEMANSHIP    = 554;
$SKILL_RESTORATION2           = 573;
$SKILL_BALANCE                = 574;
$SKILL_DESTRUCTION            = 593;
$SKILL_HOLY2                  = 594;
$SKILL_DISCIPLINE             = 613;
$SKILL_LOCKPICKING            = 633;
$SKILL_PET_BAT                = 653;
$SKILL_PET_HYENA              = 654;
$SKILL_PET_BIRD_OF_PREY       = 655;
$SKILL_PET_WIND_SERPENT       = 656;
$SKILL_LANG_GUTTERSPEAK       = 673;
$SKILL_KODO_RIDING            = 713;
$SKILL_RACIAL_TROLL           = 733;
$SKILL_RACIAL_GNOME           = 753;
$SKILL_RACIAL_HUMAN           = 754;
$SKILL_JEWELCRAFTING          = 755;
$SKILL_RACIAL_BLOOD_ELF       = 756;
$SKILL_PET_EVENT_REMOTECONTROL= 758;
$SKILL_LANG_DRAENEI           = 759;
$SKILL_RACIAL_DRAENEI         = 760;
$SKILL_PET_FELGUARD           = 761;
$SKILL_RIDING                 = 762;
$SKILL_PET_DRAGONHAWK         = 763;
$SKILL_PET_NETHER_RAY         = 764;
$SKILL_PET_SPOREBAT           = 765;
$SKILL_PET_WARP_STALKER       = 766;
$SKILL_PET_RAVAGER            = 767;
$SKILL_PET_SERPENT            = 768;
$SKILL_INTERNAL               = 769;
$SKILL_BLOOD                  = 770;
$SKILL_FROST2                 = 771;
$SKILL_UNHOLY                 = 772;
$SKILL_INSCRIPTION            = 773;
$SKILL_PET_MOTH               = 775;
$SKILL_RUNEFORGING            = 776;
$SKILL_MOUNTS                 = 777;
$SKILL_COMPANIONS             = 778;
$SKILL_PET_EXOTIC_CHIMAERA    = 780;
$SKILL_PET_EXOTIC_DEVLISAUR   = 781;
$SKILL_PET_GHOUL              = 782;
$SKILL_PET_EXOTIC_SILITHID    = 783;
$SKILL_PET_EXOTIC_WORM        = 784;
$SKILL_PET_WASP               = 785;
$SKILL_PET_EXOTIC_RHINO       = 786;
$SKILL_PET_EXOTIC_CORE_HOUND  = 787;
$SKILL_PET_EXOTIC_SPIRIT_BEAST =788;

$SKILL_TYPE_ATTRIBUTES          = 5;
$SKILL_TYPE_WEAPON              = 6;
$SKILL_TYPE_CLASS               = 7;
$SKILL_TYPE_ARMOR               = 8;
$SKILL_TYPE_SECONDARY           = 9;
$SKILL_TYPE_LANGUAGE            = 10;
$SKILL_TYPE_PROFESSION          = 11;
$SKILL_TYPE_NA                  = 12;

$class_skillines=array
(
	array(0,0,0,0),
	array($SKILL_ARMS,$SKILL_FURY,$SKILL_PROTECTION,0),								//WARRIOR
	array($SKILL_HOLY2,$SKILL_PROTECTION2,$SKILL_RETRIBUTION,0),					//PALADIN
	array($SKILL_BEAST_MASTERY,$SKILL_MARKSMANSHIP,$SKILL_SURVIVAL,0),				//HUNTER	
	array($SKILL_ASSASSINATION,$SKILL_COMBAT,$SKILL_SUBTLETY,$SKILL_LOCKPICKING),	//ROGUE
	array($SKILL_DISCIPLINE,$SKILL_SHADOW_MAGIC,$SKILL_HOLY,0),							//PRIEST
	array($SKILL_BLOOD,$SKILL_FROST2,$SKILL_UNHOLY,$SKILL_RUNEFORGING),				//DEATHKNIGHT
	array($SKILL_ELEMENTAL_COMBAT,$SKILL_ENHANCEMENT,$SKILL_RESTORATION,0),			//SHAMAN
	array($SKILL_ARCANE,$SKILL_FIRE,$SKILL_FROST,0),								//MAGE
	array($SKILL_AFFLICTION,$SKILL_DEMONOLOGY,$SKILL_DESTRUCTION,0),				//WARLOCK
	array(0,0,0,0),
	array($SKILL_BALANCE,$SKILL_FERAL_COMBAT,$SKILL_RESTORATION2,0),				//DRUID
);

/*
struct skilllinespell //SkillLineAbility.dbc
{
    uint32 Id;							//	0
    uint32 skilline;					//	1
    uint32 spell;						//	2
    uint32 raceMask;					//	3
	uint32 classMask;					//	4
    uint32 excludeRace;					//	5
    uint32 excludeClass;				//	6
    uint32 minSkillLineRank;			//	7
    uint32 next;						//	8
    uint32 talent_tab_tree;					//	9
    uint32 grey;						//	10
    uint32 green;						//	11
	uint32 unk10;						//	12
    uint32 unk11;						//	13
};

//trainer spell struct :
teach spell
required spell
required gold
*/

echo "replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( 101000,'Warrior Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( 101001,'Paladin Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( 101002,'Hunter Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( 101003,'Rogue Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( 101004,'Priest Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( 101005,'DeathKnight Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( 101006,'Shaman Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( 101007,'Mage Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( 101008,'Warlock Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( 101009,'Druid Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( 101000,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( 101001,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( 101002,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( 101003,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( 101004,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( 101005,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( 101006,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( 101007,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( 101008,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( 101009,90,90,35,100,100,1,1+2+16+32,1);<br>
";

$max_class = 11;				
//there are a few race specific spells but we ignore those atm
//$class=2;
//$should_get_empty=",465,54968,635,21084,19740,20271,498,639,853,1152,10290,1022,633,19834,53408,7328,19742,647,31789,62124,7294,25780,1044,20217,26573,643,879,19750,5502,34769,13819,19835,19746,1026,20164,19850,5588,5599,10322,10326,19939,1038,10298,5614,53407,19876,20116,10291,19752,1042,2800,20165,19836,19888,19852,642,19940,5615,19891,10324,10299,10278,3472,20166,23214,20922,1032,5589,19895,34767,19837,4987,19941,19853,10312,19897,24275,6940,10328,10300,19899,20929,20772,20923,10292,19942,20927,2812,10310,19838,10313,25782,24274,19896,19854,25894,10308,10329,19898,20930,10301,19943,32699,25291,25290,20924,10293,10314,19900,25898,25916,25899,25918,24239,25292,20928,10318,53651,20773,32223,27135,27151,27174,31801,27142,27143,27137,27150,53736,27138,27152,27180,27139,27154,32700,31884,27140,27173,27149,27153,27141,27136,27179,33072,48935,54428,48937,48816,48949,48931,48800,48933,48941,48784,48805,48826,48818,48781,48951,48824,53600,54043,48943,48936,48945,48938,48947,48817,48788,48932,48942,48801,48785,48934,48950,48827,48819,48806,48782,64891,48952,48825,53601,61411,";
for( $class = 1; $class < $max_class ; $class++)
if( $class_skillines[$class][0] )
{
	echo "insert into `trainer_defs` (`entry`, `req_class`, `trainer_type`, `trainer_ui_window_message`, `can_train_gossip_textid`, `cannot_train_gossip_textid`) values($trainer_entry,'$class','0','Hello, \$c! Ready for some training?','1040','5721');<br>";
	$class_mask = 1 << ($class - 1);			//col_4
	$exclude_class = ~$class_mask;				//col_6 -> is this even required ?
	//we store results in here then create trainer if we found anything
	$spell_list="";
	//iterate through all the skillines we have and even more ?
	for($skillinec=0;$skillinec<5;$skillinec++)
		if( $class_skillines[$class][$skillinec] )
		{
			//create the trainer def
			//get all possible spells for this skilline that a player should have
			$get_query = "select * from skilllineabilitydbccsv where (col_4 & $class_mask) and ((col_6 & $exclude_class)=0) and col_1={$class_skillines[$class][$skillinec]}";
			$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query ");
			while($row=mysql_fetch_row($get_res))
			{
				$spell_id = $row[2];
				$next_rank = $row[8];
//				$get_query2 = "select id,name,description from dbc_spell where id={$row[2]}";
//				$get_res2=mysql_query($get_query2,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query2 ");
//				$row2=mysql_fetch_row($get_res2);
//				echo "{$row2[0]} = {$row2[1]} = {$row2[2]} <br>";

				//check if this is a talent
				$get_query2 = "select count(*) from Talentdbccsv where col_4=$spell_id or col_5=$spell_id or col_6=$spell_id or col_7=$spell_id or col_8=$spell_id";
				$get_res2=mysql_query($get_query2,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query2 ");
				$row2=mysql_fetch_row($get_res2);
				if( $row2[0] > 0 )
					continue;	//do not let trainer teach talents

				//check if this is a proc spell
				$get_query2 = "select id from dbc_spell where EffectTriggerSpell_1=$spell_id or EffectTriggerSpell_2=$spell_id or EffectTriggerSpell_3=$spell_id or EffectMiscValue_1=$spell_id or EffectMiscValue_2=$spell_id or EffectMiscValue_3=$spell_id";
				$get_res2=mysql_query($get_query2,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query2 ");
				$row2=mysql_fetch_row($get_res2);
				if( $row2[0] > 0 )
					continue;	//do not let trainer teach talents

				//check if it learned by a talent spell -> merged with above query
/*				$get_query2 = "select id from dbc_spell where EffectMiscValue_1=$spell_id or EffectMiscValue_2=$spell_id or EffectMiscValue_3=$spell_id";
				$get_res2=mysql_query($get_query2,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query2 ");
				$extra_q = "";
				while( $row2=mysql_fetch_row($get_res2))
					$extra_q .= $row2[0].",";
				if( $extra_q != "" )
				{
					continue;
					$extra_q = substr($extra_q,0,-1);
					$get_query2 = "select count(*) from Talentdbccsv where col_4 in ($extra_q) or col_5 in ($extra_q) or col_6 in ($extra_q) or		col_7 in ($extra_q) or col_8 in ($extra_q)";
					$get_res2=mysql_query($get_query2,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query2 ");
					$row2=mysql_fetch_row($get_res2);
					if( $row2[0] > 0 )
						continue;	//do not let trainer teach spells that teach talents
				}*/

				//get spell base_level = req_level
				$get_query2 = "select baselevel,id,name,description,spellIconID from dbc_spell where id=$spell_id";
				$get_res2=mysql_query($get_query2,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query2 ");
				$row2=mysql_fetch_row($get_res2);
				$req_level = $row2[0];
				if( $req_level == 0 || $row2[4] == 0 )
					continue;
//echo "{$row2[1]} = {$row2[2]} = {$row2[3]} = {$row2[4]} = {$row2[5]} <br>";

				//get required_spell
				$get_query2 = "select col_2 from skilllineabilitydbccsv where col_8=$spell_id";
				$get_res2=mysql_query($get_query2,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query2 ");
				$row2=mysql_fetch_row($get_res2);
				$req_spell = $row2[0];
				if( $req_spell == "" )
					$req_spell = 0;

				//spell cost
				$spell_cost = $req_level * 3000;

				if( $class == 1 || $class == 4 )
					$delete_spell = $req_spell;
				else 
					$delete_spell = 0;

				$sql =  "insert into trainer_spells (entry,learn_spell,reqlevel,reqspell,deletespell,spellcost ) values ( $trainer_entry, $spell_id, $req_level, $req_spell, $delete_spell, $spell_cost);";
				if( $req_level == 1 && $next_rank < 1 )
					$sql .= " #check manually if valid ";
				echo "$sql<BR>";
//				if( strpos($should_get_empty,"".$spell_id ) == 0 )
//					echo "!!!!could not find $spell_id<br>";
//				$should_get_empty=str_replace( $spell_id,"",$should_get_empty);
			}
		}
		$trainer_entry++;
}
//echo "this should be empty now ! : $should_get_empty<br>";
?>