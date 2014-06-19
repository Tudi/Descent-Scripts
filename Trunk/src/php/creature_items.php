<?php
include("include/header.inc.php");

$get_query = "select entry,equip_slotid_1,equip_slotid_2,equip_slotid_3 from creature_proto where equip_slotid_1 != 0 or equip_slotid_2 != 0 or equip_slotid_3 !=0";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while(list($id,$it1,$it2,$it3)=mysql_fetch_row($get_res))
{
	echo "update creature_proto set equipmodel1=$it1,equipmodel2=$it2,equipmodel3=$it3 where entry=$id;<br>";
}
proceedtonextstep();
?>
