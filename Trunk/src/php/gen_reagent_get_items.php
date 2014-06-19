<?php
include("include/header.inc.php");

//$get_query = "select Reagent_1,Reagent_2,Reagent_3,Reagent_4,Reagent_5,Reagent_6,Reagent_7,Reagent_8,EffectItemType_1,EffectItemType_2,EffectItemType_3 from dbc_spell ";
$get_query = "select EffectItemType_1,EffectItemType_2,EffectItemType_3 from dbc_spell ";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
$unique_list="";
while($row=mysql_fetch_row($get_res))
{
	for($i=0;$i<6;$i++)
		$unique_list[$row[$i]]=1;
}

$vendor_entry1=25;
$vendor_entry2=26;
$counter=0;
foreach($unique_list as $key => $value)
	if($key>0 && $key<60000)
			echo "$key<br>";
?>
