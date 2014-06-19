<?php
foreach($_REQUEST as $request_name=>$request_value)
$$request_name=mysql_escape_string($request_value);
?>
Please use only 1 version of input
<table>
	<form name="get_data" action="<?=$PHP_SELF;?>" method="post">
	<tr>
	  <td width="150" height="30" align="right">Keep Maps:&nbsp;</td>
	  <td width="240"><input type="text" name="maps" style="width:180" value="<?=$maps?>"></td>
	</tr>
	<tr><td><input type="submit" value="convert"></td></tr>
	</form>
</table>
<?php
if( $maps )
{
	$dbhost="localhost";
	$dbuname="root";
	$dbupass="";
	$dbname="dump_parse";

	$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
	mysql_select_db($dbname, $dbi) or die("Q 2010021214071"); /**/

	$get_query = "delete from creature_spawns where map not in ($maps)";
	$get_res=mysql_query($get_query,$dbi) or die(" 2010021214072 ".mysql_error($dbi));

	$get_query = "delete from gameobject_spawns where map not in ($maps)";
	$get_res=mysql_query($get_query,$dbi) or die(" 2010021214073 ".mysql_error($dbi));

	$get_query = "delete from creature_names where entry not in ( select distinct entry from creature_spawns )";
	$get_res=mysql_query($get_query,$dbi) or die(" 2010021214074 ".mysql_error($dbi));

	$get_query = "delete from creature_proto where entry not in ( select distinct entry from creature_spawns )";
	$get_res=mysql_query($get_query,$dbi) or die(" 2010021214075 ".mysql_error($dbi));

	$get_query = "delete from gameobject_names where entry not in ( select distinct entry from gameobject_spawns )";
	$get_res=mysql_query($get_query,$dbi) or die(" 2010021214076 ".mysql_error($dbi));

	$get_query = "delete from creature_waypoints where spawnid not in ( select id from creature_spawns )";
	$get_res=mysql_query($get_query,$dbi) or die(" 2010021214077 ".mysql_error($dbi));

	$get_query = "delete from creature_waypoints where waypointid >= 10000";
	$get_res=mysql_query($get_query,$dbi) or die(" 2010021214078 ".mysql_error($dbi));

	$get_query = "delete from creature_formations where spawn_id not in ( select id from creature_spawns )";
	$get_res=mysql_query($get_query,$dbi) or die(" 2010021214079 ".mysql_error($dbi));

	$get_query = "delete from creature_quest_starter where id not in ( select id from creature_names )";
	$get_res=mysql_query($get_query,$dbi) or die(" 20100212140710 ".mysql_error($dbi));

	$get_query = "delete from creature_quest_finisher where id not in ( select id from creature_names )";
	$get_res=mysql_query($get_query,$dbi) or die(" 20100212140711 ".mysql_error($dbi));

	$get_query = "delete from creature_yells where id not in ( select id from creature_names )";
	$get_res=mysql_query($get_query,$dbi) or die(" 20100212140712 ".mysql_error($dbi));

	$get_query = "delete from loot_creatures where entryid not in ( select entry from creature_names )";
	$get_res=mysql_query($get_query,$dbi) or die(" 20100212140713 ".mysql_error($dbi));

	$get_query = "delete from loot_gameobjects where entryid not in ( select entry from gameobject_names )";
	$get_res=mysql_query($get_query,$dbi) or die(" 20100212140714 ".mysql_error($dbi));

	$get_query = "delete from npc_gossip_textid where creatureid not in ( select entry from creature_names )";
	$get_res=mysql_query($get_query,$dbi) or die(" 20100212140715 ".mysql_error($dbi));

	$get_query = "delete from vendors where entry not in ( select entry from creature_names )";
	$get_res=mysql_query($get_query,$dbi) or die(" 20100212140716 ".mysql_error($dbi));

	//!! only if there are no quests
	$get_query = "delete from items where entry not in ( select itemid from loot_creatures ) and entry not in ( select itemid from loot_gameobjects ) and entry not in ( select itemid from loot_items ) and entry not in ( select item from vendors )";
//	$get_res=mysql_query($get_query,$dbi) or die(" 20100212140716 ".mysql_error($dbi));

	if( $dbi)
		mysql_close($dbi);
}
?>