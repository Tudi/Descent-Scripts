<?php

//conn settings
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

//database connection 
 $dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
 mysql_select_db($dbname, $dbi) or die("Q 200603201239".mysql_error($dbi));

//timeout for the page
set_time_limit ( 5*60 ); 

//UPDATE items SET allowableclass=-1,allowablerace=-1;
//DELETE FROM playeritems WHERE ownerguid=2891009 AND entry != 14156;
//SELECT * FROM ItemSetdbccsv WHERE col_0>1104 AND col_0 NOT IN ( 1118,1145,1144,1136,1135,1110,1111,1134,1108,1129,1139,1113,1112,1138,1137,1104,1123,1124,1115,1116,1114,1142,1140,1141,1109,1130,1117,1143,1105,1106,1107,1127,1125,1126)
//update itemsparsecsv set itemlevel=1, buyprice=1,sellprice=1,stat_type1=0,stat_type2=0,stat_type3=0,stat_type4=0,stat_type5=0,stat_type6=0,stat_type7=0,stat_type8=0,stat_type9=0,stat_type10=0,spellid_1=0,spellid_2=0,spellid_3=0,spellid_4=0,spellid_5=0,itemset=0;
//delete from itemsparsecsv where entry in ( select entry from wowbeez_db.items );
//sucks : [909]=1118,[1074]=1145,[1073]=1144
//cool not used : 1128
//decent unused : 1146
$i=0;
$itemsetpairs[909]=1118;$i++; //1118 sucks
//$itemsetpairs[909]=1146;$i++; //1118 sucks
$itemsetpairs[1074]=1145;$i++; //1145 sucks
//$itemsetpairs[1074]=1132;$i++; //1145 sucks
$itemsetpairs[1073]=1144;$i++;
$itemsetpairs[1065]=1136;$i++;
$itemsetpairs[1064]=1135;$i++;
$itemsetpairs[918]=1110;$i++;
$itemsetpairs[917]=1111;$i++;
$itemsetpairs[1063]=1134;$i++;
$itemsetpairs[920]=1108;$i++;
$itemsetpairs[1061]=1129;$i++;
$itemsetpairs[1068]=1139;$i++;
$itemsetpairs[914]=1113;$i++;
$itemsetpairs[916]=1112;$i++;
$itemsetpairs[1067]=1138;$i++;
$itemsetpairs[1066]=1137;$i++;
$itemsetpairs[924]=1104;$i++;
$itemsetpairs[1056]=1123;$i++;
$itemsetpairs[1057]=1124;$i++;
$itemsetpairs[912]=1115;$i++;
$itemsetpairs[911]=1116;$i++;
$itemsetpairs[913]=1114;$i++;
$itemsetpairs[1071]=1142;$i++;
$itemsetpairs[1070]=1140;$i++;
$itemsetpairs[1069]=1141;$i++;
$itemsetpairs[919]=1109;$i++;
$itemsetpairs[1062]=1130;$i++;
$itemsetpairs[910]=1117;$i++;
$itemsetpairs[1072]=1143;$i++;
$itemsetpairs[923]=1105;$i++;
$itemsetpairs[922]=1106;$i++;
$itemsetpairs[921]=1107;$i++;
$itemsetpairs[1058]=1127;$i++;
$itemsetpairs[1059]=1125;$i++;
$itemsetpairs[1060]=1126;$i++;

$itemsetpairs[915]=1146;$i++;
/**/
$from="73742,73722,73728,73734,73718,73710,73708,73701,73691,73686,73682,73665,73670,73675,73657,73655";
$to="84372,84396,84388,84380,84408,84416,84418,84429,84443,84448,84462,84483,84477,84472,84502,84504";
/**/
$items_taken = "1";
$item_pairs[] = "";

$picked_pairs[]="";
$picked_items="";

$global_debug_itemID = 78658;
$global_debug_itemID = 0;
$get_query3 = "SELECT name,item1,item2,item3,item4,item5 FROM rewards order by ID"; 
$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
$global_return = "";
$additems = "";
$RewardUpdates="";
while( $row=mysql_fetch_row($get_res) )
{
	$need_echo_main = 1;
if( $global_debug_itemID != 0 && $row[1] != $global_debug_itemID ) continue;	
	$row2 = "";
	$additems .= "Set name : ".($row[0])."<br>";
	for( $i=1;$i<6;$i++)
	{
		if( $row[$i] > 0 )
		{
//echo $row[$i]."<br>";
			$ret = FindExactUpgrade( $row[$i], ( $row[2] == 0 ) );
			if( $ret == "" )
			{
				if( $need_echo_main > 0 )
				{
//					echo "<br>Name :".$row[0]."<br>";
					$need_echo_main = 0;
				}
				$ret = FindCloseUpgrade( $row[$i] );
				$upgrades = $row[$i]." Close Match - $ret ";
//				echo "$upgrades<br>";
			}
			else
			{
				if( $need_echo_main > 0 )
				{
//					echo "<br>Name :".$row[0]."<br>";
					$need_echo_main = 0;
				}
				$upgrades = $row[$i]." Exact Match - $ret ";
//				echo "$upgrades<br>";
			}
			if( $ret > 0 )
			{
				$row2[$i] = $ret;
				$additems .= ".char additem ".($row[$i])."<br>";
				$additems .= ".char additem ".(100000+$ret)."<br>";
				$name1 = GetItemName( $row[$i] );
				$name2 = GetItemName( $ret );
#				$additems .= "#$name1 - $name2<br>";
				$item_pairs[ $row[$i] ] = $ret;
				$RewardUpdates .="Update rawards set item$i=100000+$ret,name='Sup ".mysql_escape_string($row[0])."' where item$i=".$row[$i]." and name like '%".mysql_escape_string($row[0])."';<br>";
//				echo $row[$i]."-".$row2[$i]."<br>";
			}
			else
			{
				$additems .= "Missing !.char additem ".($row[0])." - ".($row[$i])."<br>";
				echo $row[$i]."-".$row[0]."<br>";
			}
			
//die();				
		}
	}
//	echo "<br>";
}

echo $additems;
echo $RewardUpdates;

$old_displayIDs="";
$new_displayIDs="";
$old_entries="";
$new_entries="";
foreach( $item_pairs as $key => $val )
	if( $key > 0 )
	{
		$fields_sql = "entry,`class`,`subclass`,`displayID`,quality,flags,flags2,buyprice,sellprice,inventorytype,allowableclass,allowablerace,itemlevel,requiredlevel,requiredskill,requiredskillrank,requiredspell,RequiredPlayerRank1,RequiredPlayerRank2,RequiredFaction,RequiredFactionStanding,`maxcount_overall`,maxcount,containerslots,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,stat_type8,stat_type9,stat_type10,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,stat_value8,stat_value9,stat_value10,stat_unk1_1,stat_unk1_2,stat_unk1_3,stat_unk1_4,stat_unk1_5,stat_unk1_6,stat_unk1_7,stat_unk1_8,stat_unk1_9,stat_unk1_10,stat_unk2_1,stat_unk2_2,stat_unk2_3,stat_unk2_4,stat_unk2_5,stat_unk2_6,stat_unk2_7,stat_unk2_8,stat_unk2_9,stat_unk2_10,ScaledStatsDistributionId,dmg_type1,delay,`range`,spellid_1,spellid_2,spellid_3,spellid_4,spellid_5,spelltrigger_1,spelltrigger_2,spelltrigger_3,spelltrigger_4,spelltrigger_5,spellcharges_1,spellcharges_2,spellcharges_3,spellcharges_4,spellcharges_5,spellcooldown_1,spellcooldown_2,spellcooldown_3,spellcooldown_4,spellcooldown_5,spellcategory_1,spellcategory_2,spellcategory_3,spellcategory_4,spellcategory_5,spellcategorycooldown_1,spellcategorycooldown_2,spellcategorycooldown_3,spellcategorycooldown_4,spellcategorycooldown_5,bonding,name1,name2,name3,name4,description,page_id,page_language,page_material,quest_id,lock_id,lock_material,sheathID,randomprop,randomsuffix,itemset,maxdurability,ZoneNameID,mapid,bagfamily,totemcategory,socket_color_1,socket_color_2,socket_color_3,socket_content_1,socket_content_2,socket_content_3,socket_bonus,gemproperties,armordamagemodifier,existingduration,ItemLimitCategoryId,holidayid,statscalingfactor,field_130,field_131";
		$get_query3 = "SELECT $fields_sql FROM wowbeez_db.items where entry in ($key) order by entry asc"; 
		$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
		$row1 = mysql_fetch_row($get_res);
		list($entry,$class,$subclass,$displayID,$quality,$flags,$flags2,$buyprice,$sellprice,$inventorytype,$allowableclass,$allowablerace,$itemlevel,$requiredlevel,$requiredskill,$requiredskillrank,$requiredspell,$RequiredPlayerRank1,$RequiredPlayerRank2,$RequiredFaction,$RequiredFactionStanding,$unique,$maxcount,$containerslots,$stat_type1,$stat_type2,$stat_type3,$stat_type4,$stat_type5,$stat_type6,$stat_type7,$stat_type8,$stat_type9,$stat_type10,$stat_value1,$stat_value2,$stat_value3,$stat_value4,$stat_value5,$stat_value6,$stat_value7,$stat_value8,$stat_value9,$stat_value10,$stat_unk1_1,$stat_unk1_2,$stat_unk1_3,$stat_unk1_4,$stat_unk1_5,$stat_unk1_6,$stat_unk1_7,$stat_unk1_8,$stat_unk1_9,$stat_unk1_10,$stat_unk2_1,$stat_unk2_2,$stat_unk2_3,$stat_unk2_4,$stat_unk2_5,$stat_unk2_6,$stat_unk2_7,$stat_unk2_8,$stat_unk2_9,$stat_unk2_10,$ScaledStatsDistributionId,$dmg_type1,$delay,$range,$spellid_1,$spellid_2,$spellid_3,$spellid_4,$spellid_5,$spelltrigger_1,$spelltrigger_2,$spelltrigger_3,$spelltrigger_4,$spelltrigger_5,$spellcharges_1,$spellcharges_2,$spellcharges_3,$spellcharges_4,$spellcharges_5,$spellcooldown_1,$spellcooldown_2,$spellcooldown_3,$spellcooldown_4,$spellcooldown_5,$spellcategory_1,$spellcategory_2,$spellcategory_3,$spellcategory_4,$spellcategory_5,$spellcategorycooldown_1,$spellcategorycooldown_2,$spellcategorycooldown_3,$spellcategorycooldown_4,$spellcategorycooldown_5,$bonding,$name1,$name2,$name3,$name4,$description,$page_id,$page_language,$page_material,$quest_id,$lock_id,$lock_material,$sheathID,$randomprop,$randomsuffix,$itemset,$maxdurability,$ZoneNameID,$mapid,$bagfamily,$totemcategory,$socket_color_1,$socket_color_2,$socket_color_3,$socket_content_1,$socket_content_2,$socket_content_3,$socket_bonus,$gemproperties,$armordamagemodifier,$existingduration,$ItemLimitCategoryId,$holidayid,$statscalingfactor,$field_130,$field_131)=$row1;
		
		$get_query3 = "SELECT $fields_sql FROM itemsparsecsv where entry in ($val) order by entry asc"; 
		$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
		$row2 = mysql_fetch_row($get_res);
		list($entry_,$class_,$subclass_,$displayID_,$quality_,$flags_,$flags2_,$buyprice_,$sellprice_,$inventorytype_,$allowableclass_,$allowablerace_,$itemlevel_,$requiredlevel_,$requiredskill_,$requiredskillrank_,$requiredspell_,$RequiredPlayerRank1_,$RequiredPlayerRank2_,$RequiredFaction_,$RequiredFactionStanding_,$unique_,$maxcount_,$containerslots_,$stat_type1_,$stat_type2_,$stat_type3_,$stat_type4_,$stat_type5_,$stat_type6_,$stat_type7_,$stat_type8_,$stat_type9_,$stat_type10_,$stat_value1_,$stat_value2_,$stat_value3_,$stat_value4_,$stat_value5_,$stat_value6_,$stat_value7_,$stat_value8_,$stat_value9_,$stat_value10_,$stat_unk1_1_,$stat_unk1_2_,$stat_unk1_3_,$stat_unk1_4_,$stat_unk1_5_,$stat_unk1_6_,$stat_unk1_7_,$stat_unk1_8_,$stat_unk1_9_,$stat_unk1_10_,$stat_unk2_1_,$stat_unk2_2_,$stat_unk2_3_,$stat_unk2_4_,$stat_unk2_5_,$stat_unk2_6_,$stat_unk2_7_,$stat_unk2_8_,$stat_unk2_9_,$stat_unk2_10_,$ScaledStatsDistributionId_,$dmg_type1_,$delay_,$range_,$spellid_1_,$spellid_2_,$spellid_3_,$spellid_4_,$spellid_5_,$spelltrigger_1_,$spelltrigger_2_,$spelltrigger_3_,$spelltrigger_4_,$spelltrigger_5_,$spellcharges_1_,$spellcharges_2_,$spellcharges_3_,$spellcharges_4_,$spellcharges_5_,$spellcooldown_1_,$spellcooldown_2_,$spellcooldown_3_,$spellcooldown_4_,$spellcooldown_5_,$spellcategory_1_,$spellcategory_2_,$spellcategory_3_,$spellcategory_4_,$spellcategory_5_,$spellcategorycooldown_1_,$spellcategorycooldown_2_,$spellcategorycooldown_3_,$spellcategorycooldown_4_,$spellcategorycooldown_5_,$bonding_,$name1_,$name2_,$name3_,$name4_,$description_,$page_id_,$page_language_,$page_material_,$quest_id_,$lock_id_,$lock_material_,$sheathID_,$randomprop_,$randomsuffix_,$itemset_,$maxdurability_,$ZoneNameID_,$mapid_,$bagfamily_,$totemcategory_,$socket_color_1_,$socket_color_2_,$socket_color_3_,$socket_content_1_,$socket_content_2_,$socket_content_3_,$socket_bonus_,$gemproperties_,$armordamagemodifier_,$existingduration_,$ItemLimitCategoryId_,$holidayid_,$statscalingfactor_,$field_130_,$field_131_)=$row2;
//		echo "# $name1 ( $entry ) - $name1_ ( $entry_ )<br>";
//		echo "# .char additem $entry_<br>";
		$additems .= ".char additem $entry_<br>";
		//update all fields except entry and displayID
		$fields = explode( ",", str_replace( "`", "", $fields_sql ) );
		$full_sql = "update items set ";
		$full_sql_replace = "replace into items (";
		$full_sql_replace_val = ") values (";
		foreach( $fields as $key2 => $val2 )
		{
			if( $val2 != 'entry' && $val2 != 'displayID' )
			{
				if( $val2 == 'unique' )
					$full_sql_replace .= "`maxcount_overall`,";
				else
					$full_sql_replace .= "`$val2`,";					
				if( $val2 == 'name1' )
				{
					$full_sql .= " $val2='Superior ".mysql_escape_string($row1[$key2])."',";
					$full_sql_replace_val .= "'Superior ".mysql_escape_string($row1[$key2])."',";
				}
				else if( strpos( "#".$val2, "stat_value" ) )
				{
					$full_sql .= " $val2='".mysql_escape_string((int)($row1[$key2] * 101 / 100))."',";
					$full_sql_replace_val .= "'".mysql_escape_string((int)($row1[$key2] * 101 / 100))."',";
				}
				else if( strpos( "#".$val2, "itemlevel" ) )
				{
					$full_sql .= " $val2='".mysql_escape_string((int)($row1[$key2] + 3 ))."',";
					$full_sql_replace_val .= "'".mysql_escape_string((int)($row1[$key2] + 3 ))."',";
				}
				else
				{
					$full_sql .= " $val2='".mysql_escape_string($row1[$key2])."',";
					$full_sql_replace_val .= "'".mysql_escape_string($row1[$key2])."',";
				}
			}
			else
			{
				$full_sql_replace .= "`$val2`,";
				if( $val2 == 'entry' )
				{
					$full_sql_replace_val .= "'".mysql_escape_string(100000+$row2[$key2])."',";
					$new_entries .= $row2[$key2].",";
					$old_entries .= $row1[$key2].",";
				}
				else
				{
					$full_sql_replace_val .= "'".mysql_escape_string($row2[$key2])."',";
					$new_displayIDs .= $row2[$key2].",";
					$old_displayIDs .= $row1[$key2].",";
				}
			}
		}
		$full_sql = substr( $full_sql, 0, -1 );
		$full_sql_replace = substr( $full_sql_replace, 0, -1 );
		$full_sql_replace_val = substr( $full_sql_replace_val, 0, -1 ).")";
		$full_sql .= " where entry=$entry_;";	
//		echo "$full_sql<br>";
		echo "$full_sql_replace$full_sql_replace_val;<br>";
	}

echo "Select entry,name from items where displayid in ( $old_displayIDs 1 ) and entry in ( $new_entries 1 )<br>";

//echo $additems;

function FindExactUpgrade( $itemID, $singleitem )
{
	global $dbi,$itemsetpairs,$picked_pairs,$picked_items,$global_return,$items_taken,$item_pairs,$global_debug_itemID;
//echo "1'$global_debug_itemID'-'$itemID'<br>";
	//get info about this item
//	$get_query3 = "SELECT itemset,name1,quality,flags,flags2,inventorytype,allowableclass,allowablerace,itemlevel,requiredlevel,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,stat_type8,stat_type9,stat_type10,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,stat_value8,stat_value9,stat_value10,sheathid FROM itemsparsecsv where entry=$itemID"; 
	$get_query3 = "SELECT itemset,name1,quality,flags,flags2,inventorytype,allowableclass,allowablerace,itemlevel,requiredlevel,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,stat_type8,stat_type9,stat_type10,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,stat_value8,stat_value9,stat_value10,sheathid FROM itemsparsecsv_beta_mop where entry=$itemID"; 
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	list($itemset,$name1,$quality,$flags,$flags2,$inventorytype,$allowableclass,$allowablerace,$itemlevel,$requiredlevel,$stat_type1,$stat_type2,$stat_type3,$stat_type4,$stat_type5,$stat_type6,$stat_type7,$stat_type8,$stat_type9,$stat_type10,$stat_value1,$stat_value2,$stat_value3,$stat_value4,$stat_value5,$stat_value6,$stat_value7,$stat_value8,$stat_value9,$stat_value10,$sheathid)=mysql_fetch_row($get_res);
	if( $name1 == "" )
	{
//		echo $get_query3;
		return;
	}
	
	//try to get a better version of it
//	and flags&$flags and ((flags2&$flags2) or flags2 = 0 ) and inventorytype=$inventorytype and allowableclass=$allowableclass and allowablerace=$allowablerace
	$get_query3 = "SELECT itemset,entry,name1,quality,flags,flags2,inventorytype,allowableclass,allowablerace,itemlevel,requiredlevel,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,stat_type8,stat_type9,stat_type10,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,stat_value8,stat_value9,stat_value10,sheathid FROM itemsparsecsv_beta_mop
	where 
	entry > $itemID
	and flags&$flags and inventorytype=$inventorytype and allowableclass=$allowableclass and allowablerace=$allowablerace
	and quality=$quality and sheathid=$sheathid
	and stat_type1>=$stat_type1 and stat_type2>=$stat_type2 and stat_type3>=$stat_type3 and stat_type4>=$stat_type4 and stat_type5>=$stat_type5 and stat_type6>=$stat_type6 and stat_type7>=$stat_type7 and stat_type8>=$stat_type8 and stat_type9>=$stat_type9 and stat_type10>=$stat_type10
	and requiredlevel >= $requiredlevel
	and itemlevel > $itemlevel
	and stat_value1>=$stat_value1 and stat_value2>=$stat_value2 and stat_value3>=$stat_value3 and stat_value4>=$stat_value4 and stat_value5>=$stat_value5 and stat_value6>=$stat_value6 and stat_value7>=$stat_value7 and stat_value8>=$stat_value8 and stat_value9>=$stat_value9 and stat_value10>=$stat_value10
	and entry not in ( $items_taken )
	"; 
	if( $itemsetpairs[$itemset] != 0 )
		$get_query3 .= " and itemset=".$itemsetpairs[$itemset];
	$extra_sql = "";
	if( $singleitem )
	{
		$name_words = explode( " ", $name1 );
		$extra_sql = " and ( ";
		foreach( $name_words as $key => $value )
			if( strpos( "#".$value, "Gladiator" ) == 0 or strlen( $value ) <= 3 )
				$extra_sql .= " name1 like '%".mysql_escape_string($value)."%' or ";
		$extra_sql = substr( $extra_sql, 0, -3 ).")";
//echo "2'$global_debug_itemID'-'$itemID'<br>";
/*if( $itemID == $global_debug_itemID )
{
	echo $get_query3;
	echo $extra_sql;
	die();
}/**/
	}
	$ret = "";
	$ret_entry = 0;
	$get_res=mysql_query($get_query3.$extra_sql,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	$count = 0;
	$inserted_one = 0;
	while( list($itemset_,$entry,$name1,$quality,$flags,$flags2,$inventorytype,$allowableclass,$allowablerace,$itemlevel,$requiredlevel,$stat_type1,$stat_type2,$stat_type3,$stat_type4,$stat_type5,$stat_type6,$stat_type7,$stat_type8,$stat_type9,$stat_type10,$stat_value1,$stat_value2,$stat_value3,$stat_value4,$stat_value5,$stat_value6,$stat_value7,$stat_value8,$stat_value9,$stat_value10,$sheathid)=mysql_fetch_row($get_res) )
	{
		$ret .= $name1."($entry,$itemset,$itemset_) - ";
		$count++;
		if( $inserted_one == 0 )
		{
			$inserted_one = 1;
			$items_taken .= ",$entry";
			$item_pairs[$itemID]=$entry;
			$ret_entry = $entry;
		}
	}
	
if( $global_debug_itemID != 0 && $itemID == $global_debug_itemID )
{
	echo $get_query3;
	echo $extra_sql;
	die();
}/**/
	
	if( $singleitem && $count == 0 )
	{
		$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
		$count = 0;
		while( list($itemset_,$entry,$name1,$quality,$flags,$flags2,$inventorytype,$allowableclass,$allowablerace,$itemlevel,$requiredlevel,$stat_type1,$stat_type2,$stat_type3,$stat_type4,$stat_type5,$stat_type6,$stat_type7,$stat_type8,$stat_type9,$stat_type10,$stat_value1,$stat_value2,$stat_value3,$stat_value4,$stat_value5,$stat_value6,$stat_value7,$stat_value8,$stat_value9,$stat_value10,$sheathid)=mysql_fetch_row($get_res) )
		{
			$ret .= $name1."($entry,$itemset,$itemset_) - ";
			$count++;
			if( $inserted_one == 0 )
			{
				$inserted_one = 1;
				$items_taken .= ",$entry";
				$item_pairs[$itemID]=$entry;
				$ret_entry = $entry;
			}
		}
	}
//if( $ret == "" ) echo $get_query3."<br>";
//die();		
//echo "eeeek $count $ret";	
//$global_return = $ret;
//	return $ret;
	return $ret_entry;
}

function FindCloseUpgrade( $itemID )
{
//	echo "could not find close upgrade for $itemID<br>";
//	die();
	global $dbi,$itemsetpairs,$picked_pairs,$picked_items,$items_taken,$item_pairs;
	//get info about this item
//	$get_query3 = "SELECT itemset,name1,quality,flags,flags2,inventorytype,allowableclass,allowablerace,itemlevel,requiredlevel,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,stat_type8,stat_type9,stat_type10,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,stat_value8,stat_value9,stat_value10,sheathid FROM itemsparsecsv where entry=$itemID"; 
	$get_query3 = "SELECT itemset,name1,quality,flags,flags2,inventorytype,allowableclass,allowablerace,itemlevel,requiredlevel,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,stat_type8,stat_type9,stat_type10,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,stat_value8,stat_value9,stat_value10,sheathid FROM itemsparsecsv_beta_mop where entry=$itemID"; 
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	list($itemset,$name1,$quality,$flags,$flags2,$inventorytype,$allowableclass,$allowablerace,$itemlevel,$requiredlevel,$stat_type1,$stat_type2,$stat_type3,$stat_type4,$stat_type5,$stat_type6,$stat_type7,$stat_type8,$stat_type9,$stat_type10,$stat_value1,$stat_value2,$stat_value3,$stat_value4,$stat_value5,$stat_value6,$stat_value7,$stat_value8,$stat_value9,$stat_value10,$sheathid)=mysql_fetch_row($get_res);
	if( $name1 == "" )
	{
//		echo $get_query3;
		return;
	}
	
	//try to get a better version of it
//	and ( stat_type1=$stat_type1 or stat_type2=$stat_type2 or stat_type3=$stat_type3 or stat_type4=$stat_type4 or stat_type5=$stat_type5 or stat_type6=$stat_type6 or stat_type7=$stat_type7 or stat_type8=$stat_type8 or stat_type9=$stat_type9 or stat_type10=$stat_type10 )
//	and ( stat_value1>=$stat_value1 or stat_value2>=$stat_value2 or stat_value3>=$stat_value3 or stat_value4>=$stat_value4 or stat_value5>=$stat_value5 or stat_value6>=$stat_value6 or stat_value7>=$stat_value7 or stat_value8>=$stat_value8 or stat_value9>=$stat_value9 or stat_value10>=$stat_value10 )
	$get_query3 = "SELECT itemset,entry,name1,quality,flags,flags2,inventorytype,allowableclass,allowablerace,itemlevel,requiredlevel,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,stat_type8,stat_type9,stat_type10,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,stat_value8,stat_value9,stat_value10,sheathid FROM itemsparsecsv_beta_mop
	where 
	entry > $itemID
	and inventorytype=$inventorytype and allowableclass=$allowableclass and allowablerace=$allowablerace
	and quality=$quality and sheathid=$sheathid
	and requiredlevel >= $requiredlevel
	and itemlevel > $itemlevel
	and entry not in ( $items_taken )
	"; 
	if( $itemsetpairs[$itemset] != 0 )
		$get_query3 .= " and itemset=".$itemsetpairs[$itemset];

	$ret = "";
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	$match_factor[]="";
	$match_name[]="";
	$match_count=0;
	while( list($itemset_,$entry,$name1_,$quality_,$flags_,$flags2_,$inventorytype_,$allowableclass_,$allowablerace_,$itemlevel_,$requiredlevel_,$stat_type1_,$stat_type2_,$stat_type3_,$stat_type4_,$stat_type5_,$stat_type6_,$stat_type7_,$stat_type8_,$stat_type9_,$stat_type10_,$stat_value1_,$stat_value2_,$stat_value3_,$stat_value4_,$stat_value5_,$stat_value6_,$stat_value7_,$stat_value8_,$stat_value9_,$stat_value10_,$sheathid)=mysql_fetch_row($get_res) )
	{
		$match_factor[$entry] = 0;
		$match_name[$entry] = $name1_;
		$match_name[$entry] = $name1."($entry,$itemset,$itemset_)";
		if( $stat_type1==$stat_type1_ )	$match_factor[$entry]++;
		if( $stat_type2==$stat_type2_ )	$match_factor[$entry]++;
		if( $stat_type3==$stat_type3_ )	$match_factor[$entry]++;
		if( $stat_type4==$stat_type4_ )	$match_factor[$entry]++;
		if( $stat_type5==$stat_type5_ )	$match_factor[$entry]++;
		if( $stat_type6==$stat_type6_ )	$match_factor[$entry]++;
		if( $stat_type7==$stat_type7_ )	$match_factor[$entry]++;
		if( $stat_type8==$stat_type8_ )	$match_factor[$entry]++;
		if( $stat_type9==$stat_type9_ )	$match_factor[$entry]++;
		if( $stat_type10==$stat_type10_ )	$match_factor[$entry]++;
	}
	$best_score = -1;
	$best_key = 0;
	foreach($match_factor as $key => $value )
		if( $value > $best_score )
		{
			$best_score = $value;
			$best_key = $key;
		}
	$ret = $match_name[$best_key]."($best_key)";
	$items_taken .= ",$best_key";
//if( $ret == "" ) echo $get_query3."<br>";
//	return $ret;
	return $best_key;
}

function GetItemName( $itemID )
{
	global $dbi;
	//get info about this item
	$get_query3 = "SELECT name1 FROM itemsparsecsv_beta_mop where entry=$itemID"; 
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	list($name1)=mysql_fetch_row($get_res);
	return $name1;
}

?>