<?php
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$get_query = "select entry,class,subclass,field4,lock_material,displayid,inventorytype from itemdbccsv where entry not in (89,138,2248,17,93,6142,133,5090,101,105,123,134,876,1957,1961,1984,5222,5226,4033,3002,1350,1544,1403,1622
,18438,12751,28648,12750,996,5828,6673,6698,2588,5378,16141,16159,13642,24228,24229,1078,22805,23741,23743,13338,22765,29188
,23227,34103,34104,1356,2638,16181,27419,951,3512,35518,27421,35517,35519,836,1450,35520,35521,35522,2804,2012,2404,2405,2600
,2602,3677,4295,4997,6273,7977,2599,8388,12817,12818,12831,12832,15780,1122,20946) ";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while($row=mysql_fetch_row($get_res))
{
	$get_query2 = "select entry,class,subclass,field4,lock_material,displayid,inventorytype from items where entry=${row[0]}";
	$get_res2=mysql_query($get_query2,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	$row2=mysql_fetch_row($get_res2);
	if( $row2 == "" )
		continue;
	$update_sql = "";
	if( $row[1] != $row2[1] )
		$update_sql .= "class=${row[1]},";
	if( $row[2] != $row2[2] )
		$update_sql .= "subclass=${row[2]},";
	if( $row[3] != $row2[3] )
		$update_sql .= "field4=${row[3]},";
	if( $row[4] != $row2[4] )
		$update_sql .= "lock_material=${row[4]},";
	if( $row[5] != $row2[5] )
		$update_sql .= "displayid=${row[5]},";
	if( $row[6] != $row2[6] )
		$update_sql .= "inventorytype=${row[6]},";
	$update_sql = substr($update_sql, 0, -1 );
	if( strlen($update_sql) > 5 )
	{
		$update_sql = "update items set ".$update_sql." where entry=${row[0]};";
		echo $update_sql."<br>";
	}
}
?>