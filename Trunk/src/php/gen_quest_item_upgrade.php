<?php
set_time_limit( 15*60 );

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="wowbeez_db";

global $dbi;
$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

//get col count 
$get_query = "SHOW FIELDS FROM items";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
$columns = "";
$ident_column_number=0;
$col_counter=0;
while($row=mysql_fetch_row($get_res) )
{
	$columns .= "`".$row[0]."`,";
	if($row[0] == $row_id_name)
		$ident_column_number=$col_counter;
	$columns_vect[$col_counter] = $row[0];
	$col_counter++;
}
$columns = substr($columns,0,-1)."";

#common
$classmask = -1;
$NPC = 111027;
$item_price[77992]=array(30,199999,0,-1,$classmask,$NPC);
$item_price[77990]=array(30,199999,0,-1,$classmask,$NPC);
$item_price[77993]=array(30,199999,0,-1,$classmask,$NPC);
$item_price[77989]=array(30,199999,0,-1,$classmask,$NPC);
$item_price[77991]=array(30,199999,0,-1,$classmask,$NPC);
$item_price[74783]=array(25,199999,0,-1,$classmask,$NPC);
$item_price[74784]=array(25,199999,0,-1,$classmask,$NPC);
$item_price[74785]=array(25,199999,0,-1,$classmask,$NPC);
$item_price[74786]=array(25,199999,0,-1,$classmask,$NPC);
$item_price[73412]=array(30,199999,0,-1,$classmask,$NPC);
$item_price[73438]=array(30,199999,0,-1,$classmask,$NPC);
#shaman
$classmask = (1<<(7-1));
$NPC = 111038;
$item_price[78733]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78704]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78686]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78667]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78724]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78725]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78672]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78691]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78718]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78739]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78741]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78711]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78685]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78666]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78723]=array(60,199999,0,-1,$classmask,$NPC);
#hunter
$classmask = (1<<(3-1));
$NPC = 111035;
$item_price[78737]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78709]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78698]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78674]=array(40,199999,0,-1,$classmask,$NPC);
#rogue
$classmask = (1<<(4-1));
$NPC = 111034;
$item_price[78738]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78708]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78699]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78679]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78664]=array(60,199999,0,-1,$classmask,$NPC);
#druid
$classmask = (1<<(11-1));
$NPC = 111033;
$item_price[78740]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78660]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78710]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78690]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78680]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78743]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78713]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78694]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78684]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78665]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78744]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78662]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78714]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78696]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78676]=array(40,199999,0,-1,$classmask,$NPC);
#DK
$classmask = (1<<(6-1));
$NPC = 111037;
$item_price[78736]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78707]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78687]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78670]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78659]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78751]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78716]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78697]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78678]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78663]=array(60,199999,0,-1,$classmask,$NPC);
#pala
$classmask = (1<<(2-1));
$NPC = 111031;
$item_price[78745]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78715]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78695]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78677]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78732]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78742]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78712]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78693]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78675]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78727]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78746]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78717]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78692]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78673]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78726]=array(60,199999,0,-1,$classmask,$NPC);
#wari
$classmask = (1<<(1-1));
$NPC = 111032;
$item_price[78734]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78705]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78689]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78669]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78658]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78735]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78706]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78668]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78657]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78688]=array(60,199999,0,-1,$classmask,$NPC);
#Priest
$classmask = (1<<(5-1));
$NPC = 111030;
$item_price[78747]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78728]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78719]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78700]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78683]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78682]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78703]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78722]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78731]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78750]=array(40,199999,0,-1,$classmask,$NPC);
#warl
$classmask = (1<<(9-1));
$NPC = 111036;
$item_price[78681]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78702]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78721]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78730]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78749]=array(40,199999,0,-1,$classmask,$NPC);
#mage
$classmask = (1<<(8-1));
$NPC = 111039;
$item_price[78748]=array(40,199999,0,-1,$classmask,$NPC);
$item_price[78729]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78720]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78701]=array(60,199999,0,-1,$classmask,$NPC);
$item_price[78671]=array(40,199999,0,-1,$classmask,$NPC);

$currency_list[0] = 199999;
$currency_list[1] = 199995;

$get_query = "SELECT * FROM items WHERE entry>100000 AND entry < 199995 AND entry NOT IN ( 133458 );";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
$quest_counter = 0;
$ThisNPCQuestCounter=0;
while( $row = mysql_fetch_row($get_res) )
{

	foreach( $columns_vect as $key => $value )
		$$value = $row[$key];
	
	$expected_name = str_replace("Superior ", "", $name1 );
	//try to get the best possible match for this item
	$get_query = "SELECT * FROM items WHERE quality=$quality and inventorytype=$inventorytype and class=$class and subclass=$subclass and stat_type1=$stat_type1 and stat_type2=$stat_type2 and stat_type3=$stat_type3 and 
	stat_type4=$stat_type4 and stat_type5 = stat_type5 and entry<100000 and name1='".mysql_escape_string($expected_name)."' order by itemlevel desc limit 0,1;";
//echo $get_query."<br>";die();
	
	$get_res2=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	$row=mysql_fetch_row($get_res2);

	foreach( $columns_vect as $key => $value )
	{
		$name = $value."2";
		$$name = $row[$key];
	}

	//generate quests
	$item_price[78671]=array(40,199999,0,-1,$classmask,$NPC);
	$currency_count1 = $item_price[$entry2][0];
	$currency_type1 = $item_price[$entry2][1];
	$currency_count2 = $item_price[$entry2][2];
	$currency_type2 = $item_price[$entry2][3];
	if( $currency_type2 == -1 )
	{
		$currency_type2 = 199995;
		$currency_count2 = $currency_count1 / 2;
	}
	$required_class = $allowableclass | $item_price[$entry2][4];
	$required_npc = $item_price[$entry2][5];
	$quest_entry = 112000+$entry;

	if( $required_npc <= 0 )
		continue;
		
	echo "#".$entry."-".$name1."<br>";
	echo "#".$entry2."-".$name12."<br>";
	echo "<br>";
	
	for( $i=1;$i<10;$i++)
	{
		$name1 = "stat_value".$i;
		$name2 = "stat_value".$i."2";
		$expected_stat = (int)( $$name2 * 101 / 100 );
		$real_value = $$name1;
		if( $real_value < $expected_stat )
			echo "Update items set stat_value$i=$expected_stat where entry=$entry; #it was $real_value <br>";
	}
	$name1 = "itemlevel";
	$name2 = "itemlevel"."2";
	$expected_stat = (int)( $$name2 * 101 / 100 );
	$real_value = $$name1;
	if( $real_value < $expected_stat )
		echo "Update items set itemlevel=$expected_stat where entry=$entry; #it was $real_value <br>";
//	die();

	//make sure it is lootable or buyable
	$get_query = "SELECT count(*) FROM loot_creature where item=$entry2;";
//echo $get_query."<br>";die();
	$get_res2=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	list( $is_lootable )=mysql_fetch_row($get_res2);
	if( $is_lootable == 0 )
	{
		$get_query = "SELECT count(*) FROM vendors where item=$entry2;";
	//echo $get_query."<br>";die();
		$get_res2=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
		list($is_buyable)=mysql_fetch_row($get_res2);
		if( $is_buyable == 0 )
		{
			echo "#!!! Warning item $entry2 - $name12 can't be looted or bought !<br>";
			continue;
		}
	}

	echo "replace into quests (entry,RewItemId1,RewItemCount1,Title,Details,Objectives,EndText,ReqItemId1,ReqItemCount1,ReqItemId2,ReqItemCount2,ReqItemId3,ReqItemCount3,IncompleteText,RequiredRaces,RequiredClass,ReqKillMobOrGOId1,ReqKillMobOrGOCount1,ReqKillMobOrGOId2,ReqKillMobOrGOCount2,ReqKillMobOrGOId3,ReqKillMobOrGOCount3,ReqKillMobOrGOId4,ReqKillMobOrGOCount4 ) values (
	".($quest_entry).",
	".($entry).",1,
	'Upgrade |cffa335ee|Hitem:$entry2:0:0:0:0:0:0:0|h[".mysql_escape_string($name12)."]|h|r',
	'If you are geared enough already, then it\'s time for you to step to the next level.',
	'If you manage to do these objectives, you are ready for higher tier challanges.',
	'Need to slain or get these tokens.',
	$entry2,1,
	$currency_type1,$currency_count1,
	$currency_type2,$currency_count2,
	'Maybe do something usefull instead reading useless text messages',
	$allowablerace,
	$required_class,
	0,1,
	0,1,
	0,1,
	0,1
	);<br>";
	echo "insert ignore into creature_quest_starter ( id, quest ) values ( $required_npc,".($quest_entry).");<br>";
	echo "insert ignore into creature_quest_finisher ( id, quest ) values ( $required_npc,".($quest_entry).");<br>";
	$quest_counter++;
/**/	
}
	
?>
<br>
<br>
#DELETE FROM creature_quest_starter WHERE id=111206;
#DELETE FROM creature_quest_finisher WHERE id=111206;
#INSERT INTO creature_quest_starter ( id, quest ) VALUES ( 111026,111002);
#INSERT INTO creature_quest_finisher ( id, quest ) VALUES ( 111026,111002);

REPLACE INTO quests ( entry, Title, CompletionText, ReqKillMobOrGOId1, ReqKillMobOrGOId2, ReqKillMobOrGOId3, ReqKillMobOrGOId4,ReqKillMobOrGOCount1,ReqKillMobOrGOCount2,ReqKillMobOrGOCount3,ReqKillMobOrGOCount4,RewItemId1,RewItemCount1, IsRepeatable ) VALUES
( 111002, 'Get upgrade components', 'Here, take these here tokens,  in which I hope you find it both useful and rewarding.', 111005, 111006, 111013, 0, 1, 1, 1, 1, 199995, 8, 1 );
REPLACE INTO quests ( entry, Title, CompletionText, ReqKillMobOrGOId1, ReqKillMobOrGOId2, ReqKillMobOrGOId3, ReqKillMobOrGOId4,ReqKillMobOrGOCount1,ReqKillMobOrGOCount2,ReqKillMobOrGOCount3,ReqKillMobOrGOCount4,RewItemId1,RewItemCount1, IsRepeatable ) VALUES
( 111008, 'Get upgrade components', 'Here, take these here tokens,  in which I hope you find it both useful and rewarding.', 111014, 111007, 111019, 111204, 1, 1, 1, 1, 199999, 8, 1 );

DELETE FROM gameobject_quest_starter WHERE id=45508;
DELETE FROM gameobject_quest_finisher WHERE id=45508;

#INSERT INTO gameobject_quest_starter ( id, quest ) VALUES ( 45508,111002);
INSERT INTO gameobject_quest_starter ( id, quest ) VALUES ( 45509,111002);
INSERT INTO gameobject_quest_finisher ( id, quest ) VALUES ( 45508,111002);
INSERT INTO gameobject_quest_starter ( id, quest ) VALUES ( 45508,111008);
INSERT INTO gameobject_quest_finisher ( id, quest ) VALUES ( 45508,111008);

INSERT INTO gameobject_quest_starter ( id, quest ) VALUES ( 45509,111009);
INSERT INTO gameobject_quest_finisher ( id, quest ) VALUES ( 45508,111009);

