<?php

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="nc_chars";

global $dbi;
$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

foreach($_REQUEST as $key => $value)
	$$key=mysql_escape_string($value);
?>

<table>
	<form name="get_data" action="<?=$_SERVER['PHP_SELF'];?>" method="post">
	<tr>
	  <td width="150" height="30" align="right">#Player Name</td>
	  <td width="240"><input type="text" name="player_name" style="width:180" value="<?=$player_name?>"></td>
	</tr>
	<tr><td><input type="submit" value="Generate SQL"></td></tr>
	</form>
</table>

<?php

if( $player_name == "" )
	exit("Need input data"); //nothing more to do

//get guid 
$get_query = "select guid from characters where name='$player_name'";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
$row=mysql_fetch_row($get_res);
$player_guid = $row[0];
if( $player_guid < 1 )
	exit("could not find player");

//export char columns and values
$assign_new_ident=6666666;
if( $assign_new_ident )
	echo "#Going to swap old row identifier with new one $assign_new_ident <br>";
export_table_row("characters","guid",$player_guid,$assign_new_ident);
export_table_row("mailbox","player_guid",$player_guid,$assign_new_ident);
export_table_row("playercooldowns","player_guid",$player_guid,$assign_new_ident);
export_table_row("playeritems","ownerguid",$player_guid,$assign_new_ident);
export_table_row("playerpets","ownerguid",$player_guid,$assign_new_ident);
export_table_row("playerpetspells","ownerguid",$player_guid,$assign_new_ident);
export_table_row("questlog","player_guid",$player_guid,$assign_new_ident);
export_table_row("social_friends","character_guid",$player_guid,$assign_new_ident);
export_table_row("social_ignores","character_guid",$player_guid,$assign_new_ident);

function export_table_row($table,$row_id_name,$row_id_value,$assign_new_ident)
{
	global $dbi;
	//get col count 
	$get_query = "SHOW FIELDS FROM $table";
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	$query_to_ret = "Insert into $table (";
	$ident_column_number=0;
	$col_counter=0;
	while($row=mysql_fetch_row($get_res) )
	{
		$query_to_ret .= "`".$row[0]."`,";
		if($row[0] == $row_id_name)
			$ident_column_number=$col_counter;
		$col_counter++;
	}
	$query_to_ret = substr($query_to_ret,0,-1).") values (";

	//get col values
	$get_query = "select * from $table where $row_id_name='".mysql_escape_string($row_id_value)."'";
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	while($row=mysql_fetch_row($get_res) )
	{
		$query_to_ret_base = $query_to_ret;
		foreach($row as $key => $value )
		{
			if( $key == $ident_column_number && $assign_new_ident )
				$query_to_ret_base .= "'".mysql_escape_string($assign_new_ident)."',";
			else
				$query_to_ret_base .= "'".mysql_escape_string($value)."',";
		}
		$query_to_ret_base = substr($query_to_ret_base,0,-1).");";
		echo $query_to_ret_base."<br>";
	}
}
?>
