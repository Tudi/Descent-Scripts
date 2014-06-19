<?php

echo "CREATE TABLE `custom_item_lookup` (
  `item_id` int(10) unsigned NOT NULL DEFAULT '0' ,
  `itemClassID` int(10) unsigned NOT NULL DEFAULT '0' ,
  `itemSubClassID` int(10) unsigned NOT NULL DEFAULT '0' ,
  `field4` int(10) unsigned NOT NULL DEFAULT '0' ,
  `lock_material` int(10) unsigned NOT NULL DEFAULT '0' ,
  `display_id` int(10) unsigned DEFAULT '0' ,
  `inventorySlotID` int(10) unsigned NOT NULL DEFAULT '0' ,
  `sheathid` varchar(500) ,
  PRIMARY KEY (`item_id`),
  UNIQUE KEY item_id (`item_id`),
   KEY item_id_2 (`item_id`)
);
";
$file=fopen("Item.dbc.csv","r");
if( !$file )
	exit("Could not open input file. Exiting<br>");
$line = fgets($file);	//junk
while(!feof($file)) 
{
   	$line = fgets($file);

	//check if we already have this item used
	$pieces=explode(",",$line);
	if( Check_valid_id($pieces[0]) )
		continue;
	$line = str_replace("\r","",$line);
	$line = str_replace("\n","",$line);
	$line = substr($line,0,-1);
	echo "insert into custom_item_lookup values ($line);<br>";
}

/*
function Check_valid_id($entry)
{
	$dbhost="localhost";
	$dbuname="root";
	$dbupass="";
	$dbname="test311_world";

	$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
	mysql_select_db($dbname, $dbi) or die("Q 200603201239"); 

	$get_query1 = "select entry from items where entry='".$entry."'";
//echo "$get_query1<br>";
	$get_res1=mysql_query($get_query1,$dbi) or die(" Q20061123150142 ".mysql_error($dbi));

	list($already_used)=mysql_fetch_row($get_res1);  
	if( $dbi)
		mysql_close($dbi);
	if( $already_used>0 )
		return 1;
	return 0;
}*/

global $invalids;
$invalids="";
function Check_valid_id($entry)
{
	global $invalids;
	if( $invalids == "" )
	{
		$file=fopen("invalid_ids.txt","r");
		if( !$file )
			exit("Could not open input file. Exiting<br>");
		while(!feof($file)) 
		{
   			$line = fgets($file);
			$invalids[ (int)($line) ] = 1;
		}
		fclose($file);
	}

	if( $invalids[ $entry ] )
		return 0;
	return 1;
}


fclose($file);
?>