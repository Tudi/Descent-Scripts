<?php
include("include/header.inc.php");
include("include/functions.php");
set_time_limit (3000);

echo "<br>";
for($i=4000;$i<5000;$i++)
{
	$query="select id from dbc_spell where id=$i";
	$get_res=mysql_query($query,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query);
	list($already_taken)=mysql_fetch_row($get_res);
	if( $already_taken == 0 )
		echo "$i,";
}
echo "<br>";
echo "<br>na";
?>