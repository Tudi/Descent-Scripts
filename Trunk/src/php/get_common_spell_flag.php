<?php
include("include/header.inc.php");
include("include/functions.php");
set_time_limit (3000);

echo "<br>";
$max_entry=0;
$storage="";
$query="select EffectMiscValue1,EffectMiscValue2,EffectMiscValue3,name,description from spells112 where Effect1=42 or Effect2=42 or Effect3=42 or  Effect1=28 or Effect2=28 or Effect3=28 or Effect1=41 or Effect2=41 or Effect3=41 or Effect1=56 or Effect2=56 or Effect3=56 or Effect1=73 or Effect2=73 or Effect3=73 or Effect1=97 or Effect2=97 or Effect3=97 or Effect1=109 or Effect2=109 or Effect3=109 or Effect1=112 or Effect2=112 or Effect3=112";
$get_res=mysql_query($query,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query);
while(list($m[0],$m[1],$m[2],$spell_name,$desc)=mysql_fetch_row($get_res))
{
	for($i=0;$i<3;$i++)
	{
		if($m[$i]>0)
		{
			if($m[$i]>$max_entry)
				$max_entry=$m[$i];
			$cn="select creature_name from creature_names where entry='".$m[$i]."'";
			$cnr=mysql_query($cn,$dbi) or die(" Q200611231502 ".mysql_error($dbi).$cn);
			list($crname)=mysql_fetch_row($cnr);
			$cp="select count(entry) from creature_proto where entry='".$m[$i]."'";
			$cpr=mysql_query($cp,$dbi) or die(" Q200611231503 ".mysql_error($dbi).$cp);
			list($has_details)=mysql_fetch_row($cpr);
			if(strlen($crname)<2 || $has_details==0)
			{
				$storage[$m[$i]]['crname']=$crname;
				$storage[$m[$i]]['spname']=$spell_name;
				$storage[$m[$i]]['spdesc']=$desc;
//				echo "Missing details for creature entry ".$m[$i]." name if available : $crname <br>";
			}
		}
	}
}
echo "Missing details for creatures : <br>";
$index=0;
for($i=0;$i<$max_entry;$i++)
	if($storage[$i]['spdesc']!="")
	{
		$index++;
		echo "$index) entry ' ".$i." ',name '".$storage[$i]['crname']."', spell name '".$storage[$i]['spname']."', spell desc '".$storage[$i]['spdesc']."' <br><br>";
	}
?>