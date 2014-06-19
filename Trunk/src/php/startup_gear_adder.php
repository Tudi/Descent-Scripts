<?php
	include("include/header.inc.php");
/*
	WARRIOR = 1,
	PALADIN = 2,
	HUNTER = 3,
	ROGUE = 4,
	PRIEST = 5,
	DK = 6,
	SHAMAN = 7,
	MAGE = 8,
	WARLOCK = 9,
	DRUID = 11,
*/
	$for_classes=array(1,2,3,4,5,6,7,8,9,10,11);
	//tier 2
//	$add_items=array(16864,16861,16863,16867,16866,16862,16868,16865);
//	$add_items=array(16860,16853,16854,16855,16856,16857,16858,16859);
//	$add_items=array(16845,16846,16847,16848,16849,16850,16851,16852);
//	$add_items=array(16820,16821,16822,16823,16824,16825,16826,16827);
//	$add_items=array(16811,16812,16813,16814,16815,16816,16817,16819);
//	$add_items=array(16837,16838,16839,16840,16841,16842,16843,16844);
//	$add_items=array(16795,16796,16797,16798,16799,16800,16801,16802);
//	$add_items=array(16803,16804,16805,16806,16807,16808,16809,16810);
//	$add_items=array(16828,16829,16830,16831,16833,16834,16835,16836);
	//tier 3
//	$add_items=array(22416,22417,22418,22419,22420,22421,22422,22423);
//	$add_items=array(22431,22424,22425,22426,22427,22428,22429,22430);
//	$add_items=array(22436,22437,22438,22439,22440,22441,22442,22443);
//	$add_items=array(22476,22477,22478,22479,22480,22481,22482,22483);
//	$add_items=array(22518,22519,22514,22517,22513,22512,22516,22515);
//	$add_items=array(22464,22465,22466,22467,22468,22469,22470,22471);
//	$add_items=array(22496,22497,22498,22499,22500,22501,22502,22503);
//	$add_items=array(22504,22505,22506,22507,22508,22509,22510,22511);
//	$add_items=array(22488,22489,22490,22491,22492,22493,22494,22495);
	//tier 7
	$add_items=array(20946); 
	$add_amounts=array(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1);

	$removal_query ="#delete from `playercreateinfo_items` where protoid in (";
	foreach($add_items as $key => $value)
		$removal_query .=$value.",";
	$removal_query = substr( $removal_query,0,-1).");<br>";
	echo $removal_query;
	foreach($for_classes as $key => $selected_class)
	{
		$get_query = "SELECT `index` FROM playercreateinfo WHERE `class`='$selected_class'"; //maybe we get trainers that we did not know before ?
		$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
		while(list($t_index)=mysql_fetch_row($get_res))
			foreach($add_items as $key => $value)
			{
				echo "replace INTO `playercreateinfo_items` (`indexid`, `protoid`, `slotid`, `amount`) 	VALUES('$t_index','$value','28','".$add_amounts[$key]."');<br>";
			}
	}
?>
