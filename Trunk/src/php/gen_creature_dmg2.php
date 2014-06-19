<?php
	
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="wowbeez_db";

//database connection 
$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239".mysql_error($dbi));

//we define rank / level 85 and hope for the best for the rest 
$dmg_stats[0][85]["dmg_avg"] = 6000;
$dmg_stats[1][85]["dmg_avg"] = 12000;
$dmg_stats[2][85]["dmg_avg"] = 19000;
$dmg_stats[3][85]["dmg_avg"] = 26000;
$dmg_stats[4][85]["dmg_avg"] = 32000;
$dmg_stats[5][85]["dmg_avg"] = 40000;

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
	for( $i=1;$i<255;$i++)
	{
		//get list of possible levels
		$get_query3 = "SELECT entry FROM creature_proto where minlevel=$i and entry in ($entry_list)"; 
		$get_res3 = mysql_query($get_query3 ,$dbi) or die(" Q200806091705 ".mysql_error($dbi)." $get_query3 " );
		$entry_list2 = "";
		while( list($entry)=mysql_fetch_row($get_res3) )
			$entry_list2 .="$entry,";
		if( $entry_list2 != "" )
		{
			$entry_list2 = substr( $entry_list2, 0, -1 );
			
			$avg_dmg = $dmg_stats[$rank][85]["dmg_avg"] / (85*85) * 4;
			$min_dmg = $avg_dmg * ($i*$i) / 4;
			
			if( $min_dmg < 5 )
				$min_dmg = 5;
			$max_diff_pct = 30 + ( 50 - 50 / ( $rank + 1 ) );
			$max_dmg = $min_dmg * ( 100 + $max_diff_pct ) / 100;
			echo "update creature_proto set mindamage=$min_dmg,maxdamage=$max_dmg where minlevel=$i and entry in ($entry_list2);<br>"; 
		}
	}
}
?>