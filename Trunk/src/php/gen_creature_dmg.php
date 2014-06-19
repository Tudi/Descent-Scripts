<?php
	
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="wowbeez_db";

//database connection 
$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239".mysql_error($dbi));

//get list of ranks from creature_names
$get_query2 = "SELECT distinct rank FROM creature_names order by rank"; 
$get_res2 = mysql_query($get_query2 ,$dbi) or die(" Q200806091705 ".mysql_error($dbi)." $get_query2 " );
while( list($rank)=mysql_fetch_row($get_res2) )
{
	//get a list of entries for this rank
	$get_query3 = "SELECT entry FROM creature_names where rank=$rank"; 
	$get_res3 = mysql_query($get_query3 ,$dbi) or die(" Q200806091705 ".mysql_error($dbi)." $get_query3 " );
	$entry_list = "";
	while( list($entry)=mysql_fetch_row($get_res3) )
		$entry_list .="$entry,";
	$entry_list = substr( $entry_list, 0, -1 );
	//get list of possible levels
	$get_query3 = "SELECT minlevel,maxlevel,mindamage,maxdamage FROM creature_proto where entry in ($entry_list)"; 
	$get_res3 = mysql_query($get_query3 ,$dbi) or die(" Q200806091705 ".mysql_error($dbi)." $get_query3 " );
//if( $rank==0 ) echo	"($entry_list)<br>";	
	while( list($minlevel,$maxlevel,$mindamage,$maxdamage)=mysql_fetch_row($get_res3) )
		if( $mindamage / $minlevel / ($rank + 1) < 100 && $maxdamage / $minlevel / ($rank + 1) < 100 )
		{
			//make a sum of the DMGes
			$res[$rank][$minlevel]["min_dmg_sum"] += $mindamage;
			$res[$rank][$minlevel]["max_dmg_sum"] += $maxdamage;
			$res[$rank][$minlevel]["val_count"]++;
		}
}
//generate AVG dmg
foreach( $res as $rank => $value )
{
	//get first + last valid value
	$first = 0;
	$first_val_min = 0;
	$first_val_max = 0;
	$last = 0;
	$last_val_min = 0;
	$last_val_max = 0;
	for($i = 1; $i < 255; $i++)
	{
		if( $res[$rank][$i]["val_count"] <= 0 )
			$divider = 1;
		else
			$divider = $res[$rank][$i]["val_count"];
			
		$avg_dmg_min = $res[$rank][$i]["min_dmg_sum"] / $divider;
		$avg_dmg_max = $res[$rank][$i]["max_dmg_sum"] / $divider;
		
		$res[$rank][$i]["dmg_min"] = $avg_dmg_min;
		$res[$rank][$i]["dmg_max"] = $avg_dmg_max;
		
		if( $res[$rank][$i]["val_count"] > 0 )
		{
			$avg_sum_min += $avg_dmg_min;
			$avg_sum_max += $avg_dmg_max;
			$avg_sum_count += $i;
		}
		
		if( $first == 0 && $first_val_min < $avg_dmg_min )
		{
			$first = $i;
			$first_val_min = $avg_dmg_min;
			$first_val_max = $avg_dmg_max;
		}
		if( $last_val_min < $avg_dmg_min )
		{
			$last = $i;
			$last_val_min = $avg_dmg_min;
			$last_val_max = $avg_dmg_max;
		}
	}
	if( $avg_sum_count > 0 )
	{
		$res[$rank]["avg_min"] = $avg_sum_min / $avg_sum_count;
//		$res[$rank]["avg_max"] = $avg_sum_max / $avg_sum_count;
		
		$max_diff_pct = 30 + ( 50 - 50 / ( $rank + 1 ) );
		$res[$rank]["avg_max"] = $res[$rank]["avg_min"] * ( 100 + $max_diff_pct ) / 100;
	}

//	for($i = 1; $i < $first; $i++)
	for($i = 1; $i < 255; $i++)
	{
		$res[$rank][$i]["min_dmg_sum"] = $res[$rank]["avg_min"] * ($i*$i)/4;
		$res[$rank][$i]["max_dmg_sum"] = $res[$rank]["avg_max"] * ($i*$i)/4;
		$res[$rank][$i]["val_count"] = 1;			
		$res[$rank][$i]["dmg_min"] = $res[$rank][$i]["min_dmg_sum"];
		$res[$rank][$i]["dmg_max"] = $res[$rank][$i]["max_dmg_sum"];
	}
/*	
	//fill out missing values using a linear formula
	$dmg_min_level = $last_val_min - $first_val_min;
	$dmg_max_level = $last_val_max - $first_val_max;
	$level_diff = $last - $first;
	if( $level_diff == 0 )
		$level_diff = 1;
	$dmg_min_level = $dmg_min_level / $level_diff;
	$dmg_max_level = $dmg_max_level / $level_diff;
	for($i = $first; $i < 255; $i++)
		if( $res[$rank][$i]["val_count"] <= 0 )
		{
			$res[$rank][$i]["min_dmg_sum"] = $first_val_min + $dmg_min_level * $i;
			$res[$rank][$i]["max_dmg_sum"] = $first_val_max + $dmg_max_level * $i;
			$res[$rank][$i]["val_count"] = 1;			
			$res[$rank][$i]["dmg_min"] = $res[$rank][$i]["min_dmg_sum"];
			$res[$rank][$i]["dmg_max"] = $res[$rank][$i]["max_dmg_sum"];
		}
		*/
}
//now output the results to be used as 
foreach( $res as $rank => $value )
{
	for( $i=1;$i<255;$i++)
	{
		echo "\$dmg_stats[$rank][$i][\"dmg_min\"] = ".$res[$rank][$i]["dmg_min"].";<br>";
		echo "\$dmg_stats[$rank][$i][\"dmg_max\"] = ".$res[$rank][$i]["dmg_max"].";<br>";
	}
}
?>