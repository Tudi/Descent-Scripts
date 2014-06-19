<?php
	$dbhost="localhost";
	$dbuname="root";
	$dbupass="";
	$dbname="wowbeez_DB";
	$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
	mysql_select_db($dbname, $dbi) or die("Q 200603201239");
//	include("include/header.inc.php");

	$sql_file=fopen("patch.sql","w");
	$cleanup_sql_file=fopen("cleanup_before_patch.sql","w");
	$cleanup_dist = 4;
	$select_spawns_for_patch = "where 1=1";	//leave this uncommented
//	$select_spawns_for_patch=" where entry in (28474,29087,18,28992,28995,29523,28997,21905,35189,26977,34252,29196,28701,33633) ";
#	$select_spawns_for_patch=" where entry in (172911,171715,190557) ";
	$select_spawns_for_patch=" where map=724";
	$get_query = "SELECT * from creature_spawns ".$select_spawns_for_patch; //maybe we get trainers that we did not know before ?
//	$get_query .= " limit 0,1";
//echo $get_query."<br>";
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	while($row=mysql_fetch_row($get_res))
	{
		//delete old creatures
		fputs($cleanup_sql_file,"delete from creature_spawns $select_spawns_for_patch and entry=${row[1]} and position_x>(${row[3]}-$cleanup_dist) and position_x<(${row[3]}+$cleanup_dist) and position_y>(${row[4]}-$cleanup_dist) and position_y<(${row[3]}+$cleanup_dist);\n");
		
		write_row_to_file("creature_spawns",$row,$sql_file,0);

		$entry = $row[1];
		$id = $row[0];

		//get names
		$get_query1 = "SELECT * from creature_names where entry=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		$row1 = mysql_fetch_row($get_res1);
		write_row_to_file("creature_names",$row1,$sql_file,-1);

		//proto
		$get_query1 = "SELECT * from creature_proto where entry=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		$row1 = mysql_fetch_row($get_res1);
		write_row_to_file("creature_proto",$row1,$sql_file,-1);

		//loot
/*		$get_query1 = "SELECT * from loot_creatures where entryid=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		while( $row1 = mysql_fetch_row($get_res1) )
		{
			write_row_to_file("loot_creatures",$row1,$sql_file,0);

			$entry2 = $row1[2];
//echo "entry2=$entry2<br>";
			//the item for the loot
			$get_query2 = "SELECT * from items where entry=$entry2"; //maybe we get trainers that we did not know before ?
			$get_res2 = mysql_query( $get_query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
			$row2 = mysql_fetch_row($get_res2);
			write_row_to_file("items",$row2,$sql_file,-1);
		}*/

		//ai_agents
		$get_query1 = "SELECT * from ai_agents where entry=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		while( $row1 = mysql_fetch_row($get_res1) )
			write_row_to_file("ai_agents",$row1,$sql_file,-1);

		//creature_quest_finisher
		$get_query1 = "SELECT * from creature_quest_finisher where id=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		while( $row1 = mysql_fetch_row($get_res1) )
		{
			write_row_to_file("creature_quest_finisher",$row1,$sql_file,-1);

			$entry2 = $row1[1];

			//the item for the loot
			$get_query2 = "SELECT * from quests where entry=$entry2"; //maybe we get trainers that we did not know before ?
			$get_res2 = mysql_query( $get_query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
			$row2 = mysql_fetch_row($get_res2);
			write_row_to_file("quests",$row2,$sql_file,-1);
//if($row2)die();
		}

		//creature_quest_finisher
		$get_query1 = "SELECT * from creature_quest_starter where id=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		while( $row1 = mysql_fetch_row($get_res1) )
		{
			write_row_to_file("creature_quest_starter",$row1,$sql_file,-1);

			$entry2 = $row1[1];

			//the item for the loot
			$get_query2 = "SELECT * from quests where entry=$entry2"; //maybe we get trainers that we did not know before ?
			$get_res2 = mysql_query( $get_query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
			$row2 = mysql_fetch_row($get_res2);
			write_row_to_file("quests",$row2,$sql_file,-1);
//if($row2)die();
		}

		//creature_waypoints
		$get_query1 = "SELECT * from creature_waypoints where spawnid=$id"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		while( $row1 = mysql_fetch_row($get_res1) )
			write_row_to_file("creature_waypoints",$row1,$sql_file,0);

		//npc_gossip_textid
		$get_query1 = "SELECT * from npc_gossip_textid where creatureid=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		while( $row1 = mysql_fetch_row($get_res1) )
		{
			write_row_to_file("npc_gossip_textid",$row1,$sql_file,-1);

			$entry2 = $row1[1];

			//the item for the loot
			$get_query2 = "SELECT * from npc_text where entry=$entry2"; //maybe we get trainers that we did not know before ?
			$get_res2 = mysql_query( $get_query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
			$row2 = mysql_fetch_row($get_res2);
			write_row_to_file("npc_text",$row2,$sql_file,-1);
		}

		//npc_monstersay
		$get_query1 = "SELECT * from npc_monstersay where entry=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		while( $row1 = mysql_fetch_row($get_res1) )
			write_row_to_file("npc_monstersay",$row1,$sql_file,-1);

		//reputation_creature_onkill
		$get_query1 = "SELECT * from reputation_creature_onkill where creature_id=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		while( $row1 = mysql_fetch_row($get_res1) )
			write_row_to_file("reputation_creature_onkill",$row1,$sql_file,-1);

		//skinningloot
/*		$get_query1 = "SELECT * from loot_skinning where entryid=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		while( $row1 = mysql_fetch_row($get_res1) )
		{
			write_row_to_file("loot_skinning",$row1,$sql_file,0);

			$entry2 = $row1[1];

			//the item for the loot
			$get_query2 = "SELECT * from items where entry=$entry2"; //maybe we get trainers that we did not know before ?
			$get_res2 = mysql_query( $get_query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
			$row2 = mysql_fetch_row($get_res2);
			write_row_to_file("items",$row2,$sql_file,-1);
//if($row2)die();
		}*/

		//trainer_defs
		$get_query1 = "SELECT * from trainer_defs where entry=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		while( $row1 = mysql_fetch_row($get_res1) )
			write_row_to_file("trainer_defs",$row1,$sql_file,-1);

		//trainer_spells
		$get_query1 = "SELECT * from trainer_spells where entry=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		while( $row1 = mysql_fetch_row($get_res1) )
			write_row_to_file("trainer_spells",$row1,$sql_file,-1);

		//vendors
		$get_query1 = "SELECT * from vendors where entry=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		while( $row1 = mysql_fetch_row($get_res1) )
		{
			write_row_to_file("vendors",$row1,$sql_file,-1);

			$entry2 = $row1[1];

			//the item for the loot
//			$get_query2 = "SELECT * from items where entry=$entry2"; //maybe we get trainers that we did not know before ?
//			$get_res2 = mysql_query( $get_query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
//			$row2 = mysql_fetch_row($get_res2);
//			write_row_to_file("items",$row2,$sql_file,-1);
//if($row2)die();
		}
		//item_quest_association
		//item_randomprop_groups
		//item_randomsuffix_groups
		//itemloot
	}/**/
	$get_query = "SELECT * from Gameobject_spawns ".$select_spawns_for_patch; //maybe we get trainers that we did not know before ?
//	$get_query .= " limit 0,1";
//echo $get_query."<br>";
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	while($row=mysql_fetch_row($get_res))
	{
		fputs($cleanup_sql_file,"delete from Gameobject_spawns $select_spawns_for_patch and entry=${row[1]} and position_x>(${row[3]}-$cleanup_dist) and position_x<(${row[3]}+$cleanup_dist) and position_y>(${row[4]}-$cleanup_dist) and position_y<(${row[3]}+$cleanup_dist);\n");
	
		write_row_to_file("Gameobject_spawns",$row,$sql_file,0);

		$entry = $row[1];
		$id = $row[0];

		//get names
		$get_query1 = "SELECT * from gameobject_names where entry=$entry"; //maybe we get trainers that we did not know before ?
		$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
		$row1 = mysql_fetch_row($get_res1);
		write_row_to_file("gameobject_names",$row1,$sql_file,-1);
	}
	
	//more cleanups
	fputs($cleanup_sql_file,"delete from creature_waypoints where spawnid not in (select id from creature_spawns);\n");
	fputs($cleanup_sql_file,"delete from creature_formations where spawnid not in (select id from creature_spawns);\n");
	fputs($cleanup_sql_file,"delete from creature_formations where target_spawn_id not in (select id from creature_spawns);\n");
	
	fclose($sql_file);
	fclose($cleanup_sql_file);

function write_row_to_file($table,$row,$file,$skipind)
{
	global $dbi;
//print_r($row);
	if(!is_array($row) )
		return;
	$intofile = "replace into $table (";

	$get_query1 = "SHOW FIELDS FROM $table";
	$get_res1 = mysql_query( $get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$get_query1);
	$ind=0;
	while( $row1 = mysql_fetch_row($get_res1) )
	{
		if($ind!=$skipind)
			$intofile .= "`".mysql_escape_string($row1[0])."`,";
		$ind++;
	}
	
	$intofile = substr($intofile,0,strlen($intofile)-1).") values (";

	foreach($row as $key => $value)
		if( $key != $skipind )
			$intofile .= "'".mysql_escape_string($value)."',";

//echo $intofile."<br>";
	$intofile = substr($intofile,0,strlen($intofile)-1).");\n";
	fputs($file,$intofile);
}
?>
