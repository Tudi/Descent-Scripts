<?php
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$res="";
for($i=0;$i<20;$i++)
	$res[$i] |= 0xFFFFFFFF;

$get_query = "select Id,name,description,Attributes,AttributesEx,AttributesExB,AttributesExC,AttributesExD,AttributesExE,AttributesExF from dbc_spell where EffectApplyAuraName_1=20 and description like \"%over%\"";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while($row=mysql_fetch_row($get_res))
{
	for($i=3;$i<20;$i++)
	{
		$res[$i] &= $row[$i];
echo "state now $i = ${res[$i]}<br>";
	}
	echo $row[0]." - ".$row[1]." - ".$row[2]."<br>";
}
for($i=3;$i<20;$i++)
	echo "common fields ".($i-3)." = ${res[$i]} <br>";
?>