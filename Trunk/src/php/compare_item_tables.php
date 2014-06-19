<?php
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="test311_world";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

//get col count 
$get_query = "SHOW FIELDS FROM items";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
$field_count = 0;
while($row=mysql_fetch_row($get_res) )
{
//	print_r($row); echo "<br> ${row[0]} <br>";
	$field_names[$field_count++]=$row[0];
}
//die();

$get_query = "SHOW FIELDS FROM items_new";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
$field_count2 = 0;
while($row=mysql_fetch_row($get_res) )
	$field_names2[$field_count2++]=$row[0];

if($field_count != $field_count2 )
{
	echo("Field count does not match $field_count != $field_count2, this tool will always fail if DB structure is not the same <br>");
	for($i=0;$i<$field_count;$i++)
		if( strcasecmp($field_names[$i] ,$field_names2[$i] ) )
			die(" these 2 fields do not match {$field_names[$i]} != {$field_names2[$i]}");
}

//to insert newly got items
$get_query = "select * from items_new";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while($row=mysql_fetch_row($get_res) )
{
	$get_query = "select * from items where entry={$row[0]}";
	$get_res2=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	$row2=mysql_fetch_row($get_res2);
	if($row2[0]==$row[0])
		continue; //not new
	$query = "insert into items values ( ";
	for($i=0;$i<$field_count;$i++)
		$query .= "'".mysql_escape_string($row[$i])."',";
	$query = substr($query,0,-1).");";
	echo $query."<br>";
}
echo "#########################################################################<br>";
$get_query = "select * from items";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while($row=mysql_fetch_row($get_res) )
{
	//fetch target row
	$get_query = "select * from items_new where entry={$row[0]}";
	$get_res2=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	$row2=mysql_fetch_row($get_res2);
	if($row2[0]!=$row[0])
		continue; // new table does not have this item. Maybe it is a custom item.
	$needs_report=0;
	$query2 = "#revert_to_old update items set ";
	$query = "update items set ";
	for($i=0;$i<$field_count;$i++)
		if($row[$i]!=$row2[$i])
		{
			$query2 .= $field_names[$i]."='".mysql_escape_string($row[$i])."',";
			$query .= $field_names[$i]."='".mysql_escape_string($row2[$i])."',";
			$needs_report=1;
		}
	if( $needs_report == 1 )
	{
		$query2 = substr($query2,0,-1)." where entry={$row[0]};";
		$query = substr($query,0,-1)." where entry={$row[0]};";
		echo $query."<br>";
		echo $query2."<br>";
	}
}
?>
