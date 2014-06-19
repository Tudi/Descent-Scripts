<?php
session_start();

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="ncdb_wi";

 $dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
 mysql_select_db($dbname, $dbi) or die("Q 200603201239");

foreach($_REQUEST as $key => $value)
	$$key=mysql_escape_string($value);

?>
 
<!--

===============================================================

	               DEVELOPED BY:
	    Jozsa Istvan - jozsab1@yahoo.com

===============================================================

-->

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<title>Dump parser</title>
<link href="../css/pbx.css" rel="stylesheet" type="text/css">
</head>
<body>
<a class="clsListText01" href="index.php">HOME</a>

<?php

//steps are given by EE scanner jumps
$step_x = 100;
$step_y = 100;
$map = 1;

//delete the 2 temp columns
$query="ALTER TABLE creature_spawns DROP tposxq";
$get_res=mysql_query($query,$dbi);
$query="ALTER TABLE creature_spawns DROP tposyq";
$get_res=mysql_query($query,$dbi);

//add 2 columns
$query="ALTER TABLE creature_spawns ADD tposxq INT(30)  DEFAULT '0'";
$get_res=mysql_query($query,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query);
$query="ALTER TABLE creature_spawns ADD tposyq INT(30)  DEFAULT '0'";
$get_res=mysql_query($query,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query);


//fill the 2 new columns
$query="update creature_spawns set tposxq = position_x / $step_x,tposyq = position_y / $step_y where map = $map";
$get_res=mysql_query($query,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query);

//get grouped spawns
$of = fopen("scanlist_$map.txt","w");
$query="select position_x,position_y,position_z from creature_spawns where map = $map group by tposxq,tposyq";
$get_res=mysql_query($query,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query);
while(list($position_x,$position_y,$position_z )=mysql_fetch_row($get_res))
{
	$position_x = (int)($position_x);
	$position_y = (int)($position_y);
	$position_z = (int)($position_z)+1;
	fputs($of,"$position_x $position_y $position_z\n");
}
fclose( $of );

//delete the 2 temp columns
$query="ALTER TABLE creature_spawns DROP tposxq";
$get_res=mysql_query($query,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query);
$query="ALTER TABLE creature_spawns DROP tposyq";
$get_res=mysql_query($query,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query);
?>