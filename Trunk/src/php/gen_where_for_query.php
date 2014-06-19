<?php
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="nc_world";
$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239");
set_time_limit (3000);
$query="select distinct entry from creature_spawns where map not in (0,1,530,571)";
$get_res=mysql_query($query,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query);
$res = "(";
$vals = 0;
while(list($val)=mysql_fetch_row($get_res))
{
//	$res .="'".mysql_escape_string($val)."',";
	$res .="$val,";
	$vals++;
}
$res = substr($res,0,-1).")";
echo $res."<br>";
echo "found $vals values for where query<br>";
?>
