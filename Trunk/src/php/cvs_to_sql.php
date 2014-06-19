<?php
set_time_limit (5*60); //5 minutes timeout ?

$file_name="SpellActivationOverlay.dbc.csv";

//$itemsparse=array("entry","quality","flags","flags2","buyprice","sellprice","inventorytype","allowableclass","allowablerace","itemlevel","requiredlevel","requiredskill","requiredskillrank","requiredspell","RequiredPlayerRank1","RequiredPlayerRank2","RequiredFaction","RequiredFactionStanding","maxcount_overall","maxcount","containerslots","stat_type1","stat_type2","stat_type3","stat_type4","stat_type5","stat_type6","stat_type7","stat_type8","stat_type9","stat_type10","stat_value1","stat_value2","stat_value3","stat_value4","stat_value5","stat_value6","stat_value7","stat_value8","stat_value9","stat_value10","stat_unk1_1","stat_unk1_2","stat_unk1_3","stat_unk1_4","stat_unk1_5","stat_unk1_6","stat_unk1_7","stat_unk1_8","stat_unk1_9","stat_unk1_10","stat_unk2_1","stat_unk2_2","stat_unk2_3","stat_unk2_4","stat_unk2_5","stat_unk2_6","stat_unk2_7","stat_unk2_8","stat_unk2_9","stat_unk2_10","ScaledStatsDistributionId","dmg_type1","delay","range","spellid_1","spellid_2","spellid_3","spellid_4","spellid_5","spelltrigger_1","spelltrigger_2","spelltrigger_3","spelltrigger_4","spelltrigger_5","spellcharges_1","spellcharges_2","spellcharges_3","spellcharges_4","spellcharges_5","spellcooldown_1","spellcooldown_2","spellcooldown_3","spellcooldown_4","spellcooldown_5","spellcategory_1","spellcategory_2","spellcategory_3","spellcategory_4","spellcategory_5","spellcategorycooldown_1","spellcategorycooldown_2","spellcategorycooldown_3","spellcategorycooldown_4","spellcategorycooldown_5","bonding","name1","name2","name3","name4","description","page_id","page_language","page_material","quest_id","lock_id","lock_material","sheathID","randomprop","randomsuffix","itemset","maxdurability","ZoneNameID","mapid","bagfamily","totemcategory","socket_color_1","socket_color_2","socket_color_3","socket_content_1","socket_content_2","socket_content_3","socket_bonus","gemproperties","armordamagemodifier","existingduration","ItemLimitCategoryId","holidayid","statscalingfactor","field_130","field_131");
//$itemsparse=array("entry","quality","flags","flags2","unkf1","unkf2","buyprice","unk3","sellprice","inventorytype","allowableclass","allowablerace","itemlevel","requiredlevel","requiredskill","requiredskillrank","requiredspell","RequiredPlayerRank1","RequiredPlayerRank2","RequiredFaction","RequiredFactionStanding","maxcount_overall","maxcount","containerslots","stat_type1","stat_type2","stat_type3","stat_type4","stat_type5","stat_type6","stat_type7","stat_type8","stat_type9","stat_type10","stat_value1","stat_value2","stat_value3","stat_value4","stat_value5","stat_value6","stat_value7","stat_value8","stat_value9","stat_value10","stat_unk1_1","stat_unk1_2","stat_unk1_3","stat_unk1_4","stat_unk1_5","stat_unk1_6","stat_unk1_7","stat_unk1_8","stat_unk1_9","stat_unk1_10","stat_unk2_1","stat_unk2_2","stat_unk2_3","stat_unk2_4","stat_unk2_5","stat_unk2_6","stat_unk2_7","stat_unk2_8","stat_unk2_9","stat_unk2_10","ScaledStatsDistributionId","dmg_type1","delay","range","spellid_1","spellid_2","spellid_3","spellid_4","spellid_5","spelltrigger_1","spelltrigger_2","spelltrigger_3","spelltrigger_4","spelltrigger_5","spellcharges_1","spellcharges_2","spellcharges_3","spellcharges_4","spellcharges_5","spellcooldown_1","spellcooldown_2","spellcooldown_3","spellcooldown_4","spellcooldown_5","spellcategory_1","spellcategory_2","spellcategory_3","spellcategory_4","spellcategory_5","spellcategorycooldown_1","spellcategorycooldown_2","spellcategorycooldown_3","spellcategorycooldown_4","spellcategorycooldown_5","bonding","name1","name2","name3","name4","description","page_id","page_language","page_material","quest_id","lock_id","lock_material","sheathID","randomprop","randomsuffix","itemset","maxdurability","ZoneNameID","mapid","bagfamily","totemcategory","socket_color_1","socket_color_2","socket_color_3","socket_content_1","socket_content_2","socket_content_3","socket_bonus","gemproperties","armordamagemodifier","existingduration","ItemLimitCategoryId","holidayid","statscalingfactor","field_130","field_131");
$itemsparse=array("entry","quality","flags","flags2","unkf1","unkf2","unk3","buyprice","sellprice","inventorytype","allowableclass","allowablerace","itemlevel","requiredlevel","requiredskill","requiredskillrank","requiredspell","RequiredPlayerRank1","RequiredPlayerRank2","RequiredFaction","RequiredFactionStanding","maxcount_overall","maxcount","containerslots","stat_type1","stat_type2","stat_type3","stat_type4","stat_type5","stat_type6","stat_type7","stat_type8","stat_type9","stat_type10","stat_value1","stat_value2","stat_value3","stat_value4","stat_value5","stat_value6","stat_value7","stat_value8","stat_value9","stat_value10","stat_unk1_1","stat_unk1_2","stat_unk1_3","stat_unk1_4","stat_unk1_5","stat_unk1_6","stat_unk1_7","stat_unk1_8","stat_unk1_9","stat_unk1_10","stat_unk2_1","stat_unk2_2","stat_unk2_3","stat_unk2_4","stat_unk2_5","stat_unk2_6","stat_unk2_7","stat_unk2_8","stat_unk2_9","stat_unk2_10","ScaledStatsDistributionId","dmg_type1","delay","range","spellid_1","spellid_2","spellid_3","spellid_4","spellid_5","spelltrigger_1","spelltrigger_2","spelltrigger_3","spelltrigger_4","spelltrigger_5","spellcharges_1","spellcharges_2","spellcharges_3","spellcharges_4","spellcharges_5","spellcooldown_1","spellcooldown_2","spellcooldown_3","spellcooldown_4","spellcooldown_5","spellcategory_1","spellcategory_2","spellcategory_3","spellcategory_4","spellcategory_5","spellcategorycooldown_1","spellcategorycooldown_2","spellcategorycooldown_3","spellcategorycooldown_4","spellcategorycooldown_5","bonding","name1","name2","name3","name4","description","page_id","page_language","page_material","quest_id","lock_id","lock_material","sheathID","randomprop","randomsuffix","itemset","maxdurability","ZoneNameID","mapid","bagfamily","totemcategory","socket_color_1","socket_color_2","socket_color_3","socket_content_1","socket_content_2","socket_content_3","socket_bonus","gemproperties","armordamagemodifier","existingduration","ItemLimitCategoryId","holidayid","statscalingfactor","field_130","field_131");
$CharStartOutfit=array("rowid","race","class","gender","itemid_1","itemid_2","itemid_3","itemid_4","itemid_5","itemid_6","itemid_7","itemid_8","itemid_9","itemid_10","itemid_11","itemid_12","itemid_13","itemid_14","itemid_15","itemid_16","itemid_17","itemid_18","itemid_19","itemid_20","itemid_21","itemid_22","itemid_23","itemid_24","itemdisp_1","itemdisp_2","itemdisp_3","itemdisp_4","itemdisp_5","itemdisp_6","itemdisp_7","itemdisp_8","itemdisp_9","itemdisp_10","itemdisp_11","itemdisp_12","itemdisp_13","itemdisp_14","itemdisp_15","itemdisp_16","itemdisp_17","itemdisp_18","itemdisp_19","itemdisp_20","itemdisp_21","itemdisp_22","itemdisp_23","itemdisp_24","itemslot_1","itemslot_2","itemslot_3","itemslot_4","itemslot_5","itemslot_6","itemslot_7","itemslot_8","itemslot_9","itemslot_10","itemslot_11","itemslot_12","itemslot_13","itemslot_14","itemslot_15","itemslot_16","itemslot_17","itemslot_18","itemslot_19","itemslot_20","itemslot_21","itemslot_22","itemslot_23","itemslot_24");
$items=array("entry","class","subclass","field4","quality","displayid","inventorytype","sheathID");
$creatures=array("entry","item_mainhand","item_offhand","item_ranged","item_ammo","","","male_displayid","female_displayid","male_displayid2","female_displayid2","","","","","Name");
$vehicles=array("ID","flags","turnSpeed","pitchSpeed","pitchMin","pitchMax","seatID_0","seatID_1","seatID_2","seatID_3","seatID_4","seatID_5","seatID_6","seatID_7","mouseLookOffsetPitch","cameraFadeDistScalarMin","cameraFadeDistScalarMax","cameraPitchOffset","facingLimitRight","facingLimitLeft","msslTrgtTurnLingering","msslTrgtPitchLingering","msslTrgtMouseLingering","msslTrgtEndOpacity","msslTrgtArcSpeed","msslTrgtArcRepeat","msslTrgtArcWidth","msslTrgtImpactRadius_0","msslTrgtImpactRadius_1","msslTrgtArcTexture","msslTrgtImpactTexture","msslTrgtImpactModel_0","msslTrgtImpactModel_1","cameraYawOffset","uiLocomotionType","msslTrgtImpactTexRadius","uiSeatIndicatorType","powerType");
$vehicleseats=array("ID","flags","attachmentID","attachmentOffsetX"," attachmentOffsetY"," attachmentOffsetZ"," enterPreDelay"," enterSpeed","enterGravity","enterMinDuration","enterMaxDuration","enterMinArcHeight"," enterMaxArcHeight"," enterAnimStart","enterAnimLoop"," rideAnimStart"," rideAnimLoop","rideUpperAnimStart","rideUpperAnimLoop"," exitPreDelay","exitSpeed"," exitGravity"," exitMinDuration"," exitMaxDuration"," exitMinArcHeight","exitMaxArcHeight","exitAnimStart"," exitAnimLoop","exitAnimEnd"," passengerYaw","passengerPitch","passengerRoll"," passengerAttachmentID"," vehicleEnterAnim","vehicleExitAnim"," vehicleRideAnimLoop"," vehicleEnterAnimBone","vehicleExitAnimBone"," vehicleRideAnimLoopBone"," vehicleEnterAnimDelay","vehicleExitAnimDelay","vehicleAbilityDisplay"," enterUISoundID","exitUISoundID"," uiSkin","flagsB","unk_0","unk_1","unk_2","unk_3","unk_4","unk_5","unk2","unk3");
$CharSections=array("ID","race","gender","GeneralType","Texture1","Texture2","Texture3","Flags","Type","Variation");
if( $file_name=="Item-sparse.csv" )
	$col_names = $itemsparse;
else if( $file_name=="CharStartOutfit.dbc.csv" )
	$col_names = $CharStartOutfit;
else if( $file_name=="Item.csv" )
	$col_names = $items;
else if( $file_name=="Item.dbc.csv" )
	$col_names = $items;
else if( $file_name=="Creature.csv" )
	$col_names = $creatures;
else if( $file_name=="Vehicle.dbc.csv" )
	$col_names = $vehicles;
else if( $file_name=="VehicleSeat.dbc.csv" )
	$col_names = $vehicleseats;
else if( $file_name=="CharSections.dbc.csv" )
	$col_names = $CharSections;
else 
	$col_names = "";

$file=fopen($file_name,"r");
if( !$file )
	exit("Could not open input file. Exiting<br>");
$output_line=0;
//generate table format
$table_name=str_replace(array(".",","," "),"",$file_name);
$create_table_sql="CREATE TABLE $table_name (";

$line = trim( fgets($file) );
$line = substr($line,0,-1);	//cut last extra ","
$parts=explode(",",$line);
foreach($parts as $key => $value )
{
	$col_name = "col_$key";
	if( $col_names != "" && $col_names[$key] != "" )
		$col_name = $col_names[$key];
	if( 
		stristr("#".$value,"long") 
		|| stristr("#".$value,"int") 
		|| stristr("#".$value,"byte") 
		|| stristr("#".$value,"none") 
		|| stristr("#".$value,"flag") 
		|| stristr("#".$value,"bool") 
		)
		$create_table_sql .="`$col_name` int(30) NOT NULL default '0',";
	else if( stristr("#".$value,"string") || stristr("#".$value,"str") )
		$create_table_sql .="`$col_name` varchar(1500) collate latin1_general_ci NOT NULL default '',";
	else if( stristr("#".$value,"float") )
		$create_table_sql .="`$col_name` float(10) NOT NULL default '0',";
	else if( strlen( $value ) > 1 )
		echo "!!!!!!! unknown col type '$value' <BR>";
}
$create_table_sql = substr( $create_table_sql, 0, -1);
$create_table_sql .=") ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;";
echo $create_table_sql."<br>";

while(!feof($file)) 
{
   	$line = trim( fgets($file) );
//	$line = substr($line,0,-1);	//cut last extra ","
	//we need to manually parse content and not "explode" inner string ","
	$prev_index = 0;
	$inside_string = 0;
	$the_sql="";
	for($i=0;$i<strlen($line);$i++)
	{
		if( $inside_string == 0 && $line[$i]=='"' )
			$inside_string = 1;
		else if( $inside_string == 1 && $line[$i]=='"' )
			$inside_string = 0;
		if( $inside_string == 0 && $line[$i]==',')
		{
			$value = substr( $line, $prev_index, $i-$prev_index );
//echo "$value <br>";
			$prev_index = $i+1;
			if( strpos("#".$value,"\"") != 0 )
			{
				$value = str_replace("\"","",$value);
				$the_sql .= "'".mysql_escape_string($value)."',";
			}
			else if( strlen( $value ) == 0 )
				$the_sql .= "'',";
			else
				$the_sql .= mysql_escape_string($value).",";
		}
	}
	if( $the_sql == "" )
		continue;
	$the_sql = "insert into $table_name values (".substr($the_sql,0,-1).");";
	echo "$the_sql <br>";
}
?>