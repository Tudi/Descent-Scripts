<?php

//conn settings
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="wowbeez_db";

//database connection 
 $dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
 mysql_select_db($dbname, $dbi) or die("Q 200603201239".mysql_error($dbi));

//timeout for the page
set_time_limit ( 5*60 ); 

$i=0;
$NPC_IDs[$i] = 47120; $limit_to[$i] = 3; $i++;
$NPC_IDs[$i] = 52363; $limit_to[$i] = 3; $i++;
$NPC_IDs[$i] = 55869; $limit_to[$i] = 3; $i++;
//get the loot list
for($j=0;$j<$i;$j++)
{
	$entry = $NPC_IDs[$j];
	$loot_list = GetLootList( $entry );
	//out of curiosity. How many groups are there now ?
	$distinct_group_count_now = 0;
	$distinct_groups = "";
	$distinct_groups[] = "";
	$loot_count = count( $loot_list );
	echo "# NPC has $loot_count drop types <br>";
	foreach( $loot_list as $key => $value )
	{
		if( $distinct_groups[$value[3]] <= 0 )
		{
			$distinct_groups[$value[3]] = 1;
			$distinct_group_count_now++;
		}
		//before 
		$sql = "# Replace into loot_creature values (";
		foreach( $value as $key2 => $value2 )
			$sql .= "'".mysql_escape_string( $value2 )."',";
		$sql = substr( $sql, 0, -1 ).");<br>";
		echo $sql;
		//after
		$value[3] = (int)($key / ( $loot_count / $limit_to[$j] ));
		$sql = "Replace into loot_creature values (";
		foreach( $value as $key2 => $value2 )
			$sql .= "'".mysql_escape_string( $value2 )."',";
		$sql = substr( $sql, 0, -1 ).");<br>";
		echo $sql;
	}
//	echo "# NPC $entry had $distinct_group_count_now max loot count<br>";
}
	
function GetLootList( $entry )
{
	global $dbi;
	$ret[]="";
	$row_count = 0;
	$get_query3 = "SELECT * from loot_creature where entry = $entry "; 
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	while( $row=mysql_fetch_row($get_res) )
	{
		$ret[$row_count] = $row;
		$row_count++;
	}
	return $ret;
}
?>