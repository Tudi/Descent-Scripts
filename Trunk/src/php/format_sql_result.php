<?php

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="wotlk_world";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$get_query1 = "select distinct entryid from loot_creatures where itemid in ( select entry from items where itemlevel>=200 )";
$get_res1=mysql_query($get_query1,$dbi) or die(" Q20061123150142 ".mysql_error($dbi));
while( list($entry)=mysql_fetch_row($get_res1) )
	echo $entry.",";
?>