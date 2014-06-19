<?php
set_time_limit (3000);

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";
$dbnameworld = "wowbeez_db";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$raceclassitem[0][0][0]=1;

$get_query = "select * from charstartoutfitdbccsv order by race,class";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query ");
while($row=mysql_fetch_row($get_res))
{
	$race = $row[ 1 ];
	$class = $row[ 2 ];
	//try to fetch our raceclass unique number
	$get_query = "select `index` from $dbnameworld.playercreateinfo where race = $race and class=$class";
	$get_res2=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query ");
	list($raceclassindex)=mysql_fetch_row($get_res2);
	if( $raceclassindex == "" )
	{
		echo "#warning : race $race + class $class should exist but we do not have it defined in playercreateinfo<br>";
		continue;
	}
	//iterate through items that are positioned in slots
	for((int)$i=4;$i<4+24;$i++)
		if( $row[ $i ] > 0 && $raceclassitem[$race][$class][$i] != 1 )
		{
			$itemslot = $row[ 48 + $i ];
			$itemid = $row[ $i ];
			$raceclassitem[$race][$class][$i]=1;
			echo "replace into playercreateinfo_items (indexid,protoid,slotid) values ($raceclassindex,$itemid,$itemslot);<br>";
		}
}
?>