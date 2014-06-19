<?php
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$my_spell_id = 2343;
$needed_count = 200;
for( $i=$my_spell_id;$i<100000;$i++)
{
	$get_query = "select id from dbc_spell where id=$i ";
//	$get_query = "select entry from wowbeez_db.items where entry=$i ";
//	$get_query = "select entry from itemcsv where entry=$i ";
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	$row=mysql_fetch_row($get_res);
	if( $row[0] != $i )
	{
//		echo "next free slot is ID = $i<br>";
		echo "$i,";
		$needed_count--;
		if( $needed_count == 0 )
			break;
	}
}
?>