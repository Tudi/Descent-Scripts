<?php
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$table_1="dbc_spell";		//new
$table_2="dbc_spell_330";	//old

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

//get col count 
$get_query = "SHOW FIELDS FROM $table_1";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
$field_count = 0;
$field_name_to_index="";
while($row=mysql_fetch_row($get_res) )
{
//	print_r($row); echo "<br> ${row[0]} <br>";
	$field_name_to_index[$row[0]]=$field_count;
	$field_names[$field_count++]=$row[0];
}
//die();

$get_query = "SHOW FIELDS FROM $table_2";
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

//changes only
echo "#########################################################################<br>";
$change_counter=0;
$review_list_names="";
$get_query = "select * from $table_1,$table_2 where $table_1.id=$table_2.id";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while($row=mysql_fetch_row($get_res) )
{
	//fetch target row
	$get_query = "select * from $table_2 where id={$row[0]}";
	$get_res2=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	$row2=mysql_fetch_row($get_res2);
	if($row2[0]!=$row[0])
		continue; // new table does not have this item. Maybe it is a custom item.
	$needs_report=0;
	$query2 = "#revert_to_old update $table_1 set ";
	$query = "update $table_1 set ";
	for($i=0;$i<$field_count;$i++)
		if($row[$i]!=$row2[$i] 
			&& $field_names[$i] != "Targets" 
			&& $field_names[$i] != "ManaCostPercentage" 
			&& $field_names[$i] != "ManaCostPercentage" 
			&& $field_names[$i] != "ManaCostPercentage" 
			&& $field_names[$i] != "RequiredShapeShift" )
		{
			$query2 .= $field_names[$i]."='".mysql_escape_string($row[$i])."',";
			$query .= $field_names[$i]."='".mysql_escape_string($row2[$i])."',";
			$needs_report=1;
		}
	if( $needs_report == 1 )
	{
		$query2 = substr($query2,0,-1)." where entry={$row[0]};";
		$query = substr($query,0,-1)." where entry={$row[0]};";
		if( have_hardcoded_fixes_for( $row[0] ) )
		{
			$change_counter++;
			echo "$change_counter)Spell {$row[0]} needs to be reviewed : ".$row[$field_name_to_index['Name']]." ###  ".$row[$field_name_to_index['Description']]."<br>";
			echo "<b>".$query."</b><br>";
			echo $query2."<br>";
			$review_list_names[$row[$field_name_to_index['Name']]]=1;
		}
	}
}
echo "#########################################################################<br>";
foreach($review_list_names as $key => $value )
	echo "# ".$key."<br>";
echo "#########################################################################<br>";
/*
//to insert newly got items
$get_query = "select * from $table_2";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while($row=mysql_fetch_row($get_res) )
{
	$get_query = "select * from $table_1 where entry={$row[0]}";
	$get_res2=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	$row2=mysql_fetch_row($get_res2);
	if($row2[0]==$row[0])
		continue; //not new
	$query = "insert into $table_1 values ( ";
	for($i=0;$i<$field_count;$i++)
		$query .= "'".mysql_escape_string($row[$i])."',";
	$query = substr($query,0,-1).");";
	echo $query."<br>";
}*/

$file_content = "";
function have_hardcoded_fixes_for( $entry )
{
	global $file_content;
	if( $file_content == "" )
	{
		$file = fopen( "SpellFixes.cpp", "r" );
		while(!feof($file))
			$file_content .= trim( fgets($file) );
	}
	return strpos( $file_content, $entry );
}
?>
