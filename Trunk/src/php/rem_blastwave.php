<?php
include("include/header.inc.php");

$get_query = "select guid,spells from characters where spells like '%30600,%'";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while(list($guid,$spells)=mysql_fetch_row($get_res))
{
	$new_spells=str_replace("30600,","",$spells);
	echo "update characters set spells='$new_spells' where guid=$guid;<br>";
}
?>
