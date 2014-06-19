<?php
session_start();

//conn settings
$dbhost="localhost";
$dbuname="root";
$dbupass="lofaszka";
$dbname="characters";

//database connection 
 $dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
 mysql_select_db($dbname, $dbi) or die("Q 200603201239".mysql_error($dbi));

//timeout for the page
set_time_limit ( 60*10 ); 

/*
	WARRIOR = 1,
	PALADIN = 2,
	HUNTER = 3,
	ROGUE = 4,
	PRIEST = 5,
	DK = 6,
	SHAMAN = 7,
	MAGE = 8,
	WARLOCK = 9,
	DRUID = 11,
*/

$remove_spells_from_char = array();
$remove_spells_from_char_class[1] = array();
$remove_spells_from_char_class[2] = array();
$remove_spells_from_char_class[3] = array();
$remove_spells_from_char_class[4] = array();
$remove_spells_from_char_class[5] = array(27816,0);
$remove_spells_from_char_class[6] = array();
$remove_spells_from_char_class[7] = array();
$remove_spells_from_char_class[8] = array();
$remove_spells_from_char_class[9] = array();
$remove_spells_from_char_class[11] = array();

$get_query3 = "SELECT guid,class,spells,name FROM characters where class=5 order by guid asc"; 
$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
$counter=0;
while( list($guid,$class,$spells,$name)=mysql_fetch_row($get_res) )
{
//$spell_before=$spells;
	$len_before = strlen($spells);
	foreach($remove_spells_from_char_class[$class] as $key => $rem_spell)
	{
//if( strpos($spells,"$rem_spell") != 0 )
//	echo "found $rem_spell <br>";
		$spells = str_replace($rem_spell.",","",$spells);
		$made_replace = 1;
	}
	foreach($remove_spells_from_char as $key => $rem_spell)
	{
//if( strpos($spells,"$rem_spell") != 0 )
//	echo "found $rem_spell <br>";
		$spells = str_replace($rem_spell.",","",$spells);
		$made_replace = 1;
	}
	if( $len_before != strlen($spells) )
	{
$counter++;
echo "$counter)Removed some spells from char with name and guid $name = $guid<br>";
		$spells = str_replace(",,",",",$spells);
		$query = "Update characters set spells='$spells' where guid=$guid";
//echo "before : $spell_before<br>";
//echo "after : $spells<br>";
		$get_res2=mysql_query($query,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $query ");
	}
}
?>