<?php
$src_table="quests_new";
$dst_table="quests";
$entry_col_name="entry";	
$entry_col_pos = 0;

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="wotlk_world";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

//fetch all columns from new
$get_query1 = "SHOW FIELDS FROM $src_table";
$get_res1=mysql_query($get_query1,$dbi) or die(" Q20061123150142 ".mysql_error($dbi));
$col_names_vect = mysql_fetch_row($get_res1);

//get all rows from src table
$get_query1 = "select * FROM $src_table";
$get_res1=mysql_query($get_query1,$dbi) or die(" Q20061123150142 ".mysql_error($dbi));
while( $row=mysql_fetch_row($get_res1) )
{
	//is this a new row ? if yes then we can insert it
	$get_query1 = "select count(*) FROM $dst_table where $entry_col_name='".$row[$entry_col_pos]."'";
	$get_res2=mysql_query($get_query1,$dbi) or die(" Q20061123150142 ".mysql_error($dbi));
	list($is_inserted) = mysql_fetch_row($get_res2);
	if( $is_inserted == 0 )
		echo "insert into $dst_table (select * from $src_table);<br>";
	else
	{
		echo "update $dst_table set ";
		foreach( $col_names_vect as $key => $value )
			echo "$value='".mysql_escape_string($row[$key])."',";
		echo "1=1 where $entry_col_name='".$row[$entry_col_pos].";";
	}
}
?>