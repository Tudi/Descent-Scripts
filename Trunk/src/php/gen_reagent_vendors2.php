<?php

$VENDOR_FIRST_ENTRY = 102000;
$ITEMS_PER_VENDOR = 100;
$VENDOR_NAME = "Reagent Vendor";
$vendor_look=array(20761,20705,29054,17374,18482,4532,1355,16203,17372,1859,3808,15516,18222,1882,4209,16610,3346,3798,1878,18324,11900,1728);
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$get_query = "select Reagent_1,Reagent_2,Reagent_3,Reagent_4,Reagent_5,Reagent_6,Reagent_7,Reagent_8 from dbc_spell group by Reagent_1,Reagent_2,Reagent_3,Reagent_4,Reagent_5,Reagent_6,Reagent_7,Reagent_8";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
$unique_list="";
while($row=mysql_fetch_row($get_res))
	for($i=0;$i<6;$i++)
		if( $row[$i] )
			$unique_list.=$row[$i].",";
$unique_list=substr($unique_list,0,-1);

if( $dbi)
	mysql_close($dbi);

$dbname="test311_world";
$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

//echo $unique_list."<br>";
$get_query = "select entry,name1,flags from items where entry in ($unique_list) "
." and armor=0 "
." and stat_type1=0 "
//." and spellid_1=0 "		//maybe let potions and pet food to be sold ?
." and dmg_min1=0 "
//." and quality=1 "
." and !(flags & (2|0x800)) "
." and !(class=0 and subclass=0) "
." and name1 not like 'zzz%' "
." and name1 not like 'elixir%' "
." and name1 not like '%potion' "
." and name1 not like 'scroll%' "
." and name1 not like 'flask%' "
." and !((class=7 and subclass=11 and (flags & (~(0x10000)))!=0)) "
//." and class in ( 0,1,2,3,4,5,6,7,8,11) order by name1 asc ";
." and class in ( 0,3,5,7,8) order by name1 asc ";
//echo $get_query."<br>";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));

$counter=0;
$vendors_made=0;
$common_flags=0xFFFFFFFF;
//echo "LOCK TABLES `vendors` WRITE;<br>/*!40000 ALTER TABLE `vendors` DISABLE KEYS */;<br>";
while($row=mysql_fetch_row($get_res) )
{
	if( $counter == 0 )
	{
		echo "delete from vendors where entry=$VENDOR_FIRST_ENTRY;<br>";
		$first_name = $row[1];
	}
	echo "insert into vendors (entry,item,amount,max_amount) values ($VENDOR_FIRST_ENTRY,".$row[0].",1,1); <br>";
//echo $row[0]."-".$row[1]."<br>";
	$counter++;
	$last_name=$row[1];
//if( $row[2] != 0 ) $common_flags = $common_flags & $row[2];
//echo "common flags now $common_flags <br>";
//echo $row[0].",";
	if($counter >= $ITEMS_PER_VENDOR)
	{
		echo "replace into creature_names (entry,name,subname,info_str,type,male_displayid) values ( $VENDOR_FIRST_ENTRY,'$VENDOR_NAME','".$first_name[0]."-".$last_name[0]."','Buy',7,".$vendor_look[$vendors_made].");<br>";
		echo "replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $VENDOR_FIRST_ENTRY,90,90,35,100,100,1,129,1);<br>";
		$VENDOR_FIRST_ENTRY++;
		$counter = 0;
		$vendors_made++;
	}
}
//maybe last vendor does not exactly have N items
if($counter < $ITEMS_PER_VENDOR)
{
	echo "replace into creature_names (entry,name,subname,info_str,type,male_displayid) values ( $VENDOR_FIRST_ENTRY,'$VENDOR_NAME','".$first_name[0]."-".$last_name[0]."','Buy',7,".$vendor_look[$vendors_made].");<br>";
	echo "replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $VENDOR_FIRST_ENTRY,90,90,35,100,100,1,129,1);<br>";
	$VENDOR_FIRST_ENTRY++;
	$counter = 0;
	$vendors_made++;
}

if( $dbi)
	mysql_close($dbi);

?>
