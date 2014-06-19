<?php
set_time_limit (3000);

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="acemu_chars";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$get_res1=mysql_query("select guid from characters where timestamp>1257033600",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while($row=mysql_fetch_row($get_res1))
{
	//ncwow
/*	{
		$get_res=mysql_query("insert into mailbox_insert_queue values (".$row[0].",".$row[0].",'Happy Holidays','Wowbeez team wishes you all best ingame and mostly in real life.',61,0,44178,1)",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
		$get_res=mysql_query("insert into mailbox_insert_queue values (".$row[0].",".$row[0].",'Happy Holidays','Wowbeez team wishes you all best ingame and mostly in real life.',61,0,43599,1)",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
		$get_res=mysql_query("insert into mailbox_insert_queue values (".$row[0].",".$row[0].",'Happy Holidays','Wowbeez team wishes you all best ingame and mostly in real life.',61,0,44819,1)",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
		$get_res=mysql_query("insert into mailbox_insert_queue values (".$row[0].",".$row[0].",'Happy Holidays','Wowbeez team wishes you all best ingame and mostly in real life.',61,0,5,500)",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	}/**/
	//northrend
	{
		$get_res=mysql_query("insert into mailbox_insert_queue values (".$row[0].",".$row[0].",'Happy Holidays','Wowbeez team wishes you all best ingame and mostly in real life.',61,0,44178,1)",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
		$get_res=mysql_query("insert into mailbox_insert_queue values (".$row[0].",".$row[0].",'Happy Holidays','Wowbeez team wishes you all best ingame and mostly in real life.',61,0,43599,1)",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
		$get_res=mysql_query("insert into mailbox_insert_queue values (".$row[0].",".$row[0].",'Happy Holidays','Wowbeez team wishes you all best ingame and mostly in real life.',61,0,44819,1)",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	}/**/
	//BG
	{
		$get_res=mysql_query("insert into mailbox_insert_queue values (".$row[0].",".$row[0].",'Happy Holidays','Wowbeez team wishes you all best ingame and mostly in real life.',61,5000000,38082,1)",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	}/**/
}
?>