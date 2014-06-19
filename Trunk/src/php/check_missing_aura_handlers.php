<?php
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$limit_for_sampling=10;

$missing_handlers="46,48,50,63,70,111,114,119,145,146,157,160,164,170,173,178,181,183,194,195,196,201,205,214,217,218,221,222,223,224,225,229,,233,235,241,242,244,245,,246,247,248,251,253,254,257,258,259,260,261,264,265,266,269,270,272,273,274,276,277,278,279,283,284,286";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$aura_counts="";
$missing_handlers = explode(",",$missing_handlers);
foreach($missing_handlers as $key => $val )
	if($val)
	{
		$get_res=mysql_query("select count(*) from dbc_spell where (EffectApplyAuraName_1=$val or EffectApplyAuraName_2=$val or EffectApplyAuraName_3=$val)",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
		$row=mysql_fetch_row($get_res);
//		$aura_counts[$row[0]]=$val;
//		$aura_counts[$val]=$row[0];
		$aura_counts[$key]=$row[0] * 10000 + $val;
	}
//print_r($aura_counts ); echo "<br>";
rsort( $aura_counts );
//print_r($aura_counts );

echo "<table border='1'>";
		echo "<tr>";
			echo "<td>id</td>";
			echo "<td>name</td>";
			echo "<td>description</td>";
			echo "<td>effect_1</td>";
			echo "<td>EffectApplyAuraName_1</td>";
			echo "<td>EffectBasePoints_1</td>";
			echo "<td>EffectImplicitTargetA_1</td>";
			echo "<td>EffectImplicitTargetB_1</td>";
			echo "<td>EffectMiscValue_1</td>";
			echo "<td>EffectMiscValueB_1</td>";
			echo "<td>EffectTriggerSpell_1</td>";
			echo "<td>SpellGroupType_1</td>";
			echo "<td>EffecGroup_1</td>";
			echo "<td>EffecGroup_2</td>";
			echo "<td>EffecGroup_3</td>";
		echo "</tr>";
foreach($aura_counts as $key => $val )
{
	$aura = $val % 10000;
	if($aura==0)
		continue;	//this aura does not need a handler
	$count = (int)($val / 10000);
	echo "<tr>";
	for($i=0;$i<10;$i++)
		echo "<td>$count</td>";
	echo "</tr>";
		echo "<tr>";
			echo "<td>id</td>";
			echo "<td>name</td>";
			echo "<td>description</td>";
			echo "<td>effect_1</td>";
			echo "<td>EffectApplyAuraName_1</td>";
			echo "<td>EffectBasePoints_1</td>";
			echo "<td>EffectImplicitTargetA_1</td>";
			echo "<td>EffectImplicitTargetB_1</td>";
			echo "<td>EffectMiscValue_1</td>";
			echo "<td>EffectMiscValueB_1</td>";
			echo "<td>EffectTriggerSpell_1</td>";
			echo "<td>SpellGroupType_1</td>";
			echo "<td>EffecGroup_1</td>";
			echo "<td>EffecGroup_2</td>";
			echo "<td>EffecGroup_3</td>";
		echo "</tr>";
	$get_res=mysql_query("select id,name,description,effect_1,EffectApplyAuraName_1,EffectBasePoints_1,EffectImplicitTargetA_1,EffectImplicitTargetB_1,EffectMiscValue_1,EffectMiscValueB_1,EffectTriggerSpell_1,SpellGroupType_1,EffecGroup_1,EffecGroup_2,EffecGroup_3 from dbc_spell where EffectApplyAuraName_1=$aura limit 0,$limit_for_sampling",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	while($row=mysql_fetch_row($get_res))
	{
		echo "<tr>";
		foreach($row as $key1 => $val1 )
			echo "<td>$val1</td>";
		echo "</tr>";
	}
		echo "<tr>";
			echo "<td>id</td>";
			echo "<td>name</td>";
			echo "<td>description</td>";
			echo "<td>effect_1</td>";
			echo "<td>EffectApplyAuraName_1</td>";
			echo "<td>EffectBasePoints_1</td>";
			echo "<td>EffectImplicitTargetA_1</td>";
			echo "<td>EffectImplicitTargetB_1</td>";
			echo "<td>EffectMiscValue_1</td>";
			echo "<td>EffectMiscValueB_1</td>";
			echo "<td>EffectTriggerSpell_1</td>";
			echo "<td>SpellGroupType_1</td>";
			echo "<td>EffecGroup_1</td>";
			echo "<td>EffecGroup_2</td>";
			echo "<td>EffecGroup_3</td>";
		echo "</tr>";
	$get_res=mysql_query("select id,name,description,effect_2,EffectApplyAuraName_2,EffectBasePoints_2,EffectImplicitTargetA_2,EffectImplicitTargetB_2,EffectMiscValue_2,EffectMiscValueB_2,EffectTriggerSpell_2,SpellGroupType_2,EffecGroup_4,EffecGroup_5,EffecGroup_6 from dbc_spell where EffectApplyAuraName_2=$aura limit 0,$limit_for_sampling",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	while($row=mysql_fetch_row($get_res))
	{
		echo "<tr>";
		foreach($row as $key1 => $val1 )
			echo "<td>$val1</td>";
		echo "</tr>";
	}
		echo "<tr>";
			echo "<td>id</td>";
			echo "<td>name</td>";
			echo "<td>description</td>";
			echo "<td>effect_1</td>";
			echo "<td>EffectApplyAuraName_1</td>";
			echo "<td>EffectBasePoints_1</td>";
			echo "<td>EffectImplicitTargetA_1</td>";
			echo "<td>EffectImplicitTargetB_1</td>";
			echo "<td>EffectMiscValue_1</td>";
			echo "<td>EffectMiscValueB_1</td>";
			echo "<td>EffectTriggerSpell_1</td>";
			echo "<td>SpellGroupType_1</td>";
			echo "<td>EffecGroup_1</td>";
			echo "<td>EffecGroup_2</td>";
			echo "<td>EffecGroup_3</td>";
		echo "</tr>";
	$get_res=mysql_query("select id,name,description,effect_3,EffectApplyAuraName_3,EffectBasePoints_3,EffectImplicitTargetA_3,EffectImplicitTargetB_3,EffectMiscValue_3,EffectMiscValueB_3,EffectTriggerSpell_3,SpellGroupType_3,EffecGroup_7,EffecGroup_8,EffecGroup_9 from dbc_spell where EffectApplyAuraName_3=$aura limit 0,$limit_for_sampling",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	while($row=mysql_fetch_row($get_res))
	{
		echo "<tr>";
		foreach($row as $key1 => $val1 )
			echo "<td>$val1</td>";
		echo "</tr>";
	}
}
echo "</table>";
/**/
?>