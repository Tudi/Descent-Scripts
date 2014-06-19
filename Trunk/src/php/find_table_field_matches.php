<?php
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";
$table1="itemcsv";
$table2="itemsparsecsv";
$rows_to_compare = 5;	
$entries_to_compare = 10;	
$EntryColName1 = "entry";
$EntryColName2 = "entry";
$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

//get col count 
$get_query = "SHOW FIELDS FROM $table1";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
$field_count1 = 0;
while($row=mysql_fetch_row($get_res) )
{
//	print_r($row); echo "<br> ${row[0]} <br>";
	$field_names1[$field_count1++]=$row[0];
}
//die();

$get_query = "SHOW FIELDS FROM $table2";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
$field_count2 = 0;
while($row=mysql_fetch_row($get_res) )
	$field_names2[$field_count2++]=$row[0];

for( $i=1;$i<$field_count1;$i++)
{
	$selected_col_1 = $field_names1[$i];
	$EntriesTried = "''";
	for( $l=0;$l<$entries_to_compare;$l++)
	{
		$values_tried = "''";
	//echo "selected_col_1 = $selected_col_1<br>";
		for( $k=0;$k<$rows_to_compare;$k++)
		{
			$get_query = "select $EntryColName1,`$selected_col_1` from $table1 where `$selected_col_1` not in ($values_tried) and $EntryColName1 not in ($EntriesTried)";
//echo $get_query."<br>";			
			$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." : ".$get_query);
			$row1=mysql_fetch_row($get_res);
			$EntriesTried .= ",'".mysql_escape_string( $row1[0] )."'";
			$values_tried .= ",'".mysql_escape_string( $row1[1] )."'";
	//echo "values_tried = $values_tried<br>";
			$entry1 = $row1[0];
			if( $entry1 <= 0 )
				continue;
			$val1 = $row1[1];
	//echo "entry = $entry1<br>";
	//echo "val1 = $val1<br>";
			for($j=1;$j<$field_count2;$j++)
			{
				$selected_col_2 = $field_names2[$j];
				$get_query = "select `$selected_col_2` from $table2 where $EntryColName2=$entry1";
				$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." : ".$get_query);
				$row2=mysql_fetch_row($get_res);
				$val2 = $row2[0];
				if( $val2 == "" )
					continue;
	//if( $selected_col_1 == $selected_col_2 ) echo "$val1 = $val2<br>";
				if( $val2 == $val1 )
					$good_matches[$i][$j]++;
				else
					$bad_matches[$i][$j]++;
			}
		}
	}
//	break;
}
for( $i=1;$i<$field_count1;$i++)
{
	for($j=1;$j<$field_count2;$j++)
	{
//		echo $good_matches[$i][$j]."<br>";
//		echo $bad_matches[$i][$j]."<br>";
		if( (int)$good_matches[$i][$j] > 0 && (int)$bad_matches[$i][$j] == 0 )
			echo "field ".$field_names1[$i]." matches ",$field_names2[$j]."<br>";
		else if( $field_names1[$i] == 'field4' && $good_matches[$i][$j] != "" )
			echo "field ".$field_names1[$i]." does not match ",$field_names2[$j]." => '".$good_matches[$i][$j]."' - '".$bad_matches[$i][$j]."' <br>";
//		else if( (int)$good_matches[$i][$j] == 0 && (int)$bad_matches[$i][$j] > 0 )
//			echo "field ".$field_names1[$i]." does not match ",$field_names2[$j]." => ".$good_matches[$i][$j]." ".$bad_matches[$i][$j]." <br>";
	}
}
?>
