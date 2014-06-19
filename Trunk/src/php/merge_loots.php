<?php
include("include/header.inc.php");
include("include/functions.php");
set_time_limit (3000);
/*
$table_name="skinningloot";
$query="select entryid,itemid,percentchance,heroicpercentchance,mincount,maxcount from $table_name";
$get_res=mysql_query($query,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query);
while(list($entryid,$itemid,$percentchance,$heroicpercentchance,$mincount,$maxcount)=mysql_fetch_row($get_res))
{
	$query2="select count(*) from ".$table_name."_old where itemid=$itemid and entryid=$entryid";
	$get_res2=mysql_query($query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query2);
	list($is_inserted)=mysql_fetch_row($get_res2);
	if($is_inserted==0)
	{
		$query2="INSERT into ".$table_name."_old (entryid,itemid,percentchance,heroicpercentchance,mincount,maxcount) values ($entryid,$itemid,$percentchance,$heroicpercentchance,$mincount,$maxcount)";
		$get_res2=mysql_query($query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query2);
	}
}

$table_name="creatureloot";
$query="select entryid,itemid,percentchance,heroicpercentchance,mincount,maxcount from $table_name";
$get_res=mysql_query($query,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query);
while(list($entryid,$itemid,$percentchance,$heroicpercentchance,$mincount,$maxcount)=mysql_fetch_row($get_res))
{
	$query2="select count(*) from ".$table_name."_old where itemid=$itemid and entryid=$entryid";
	$get_res2=mysql_query($query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query2);
	list($is_inserted)=mysql_fetch_row($get_res2);
	if($is_inserted==0)
	{
		$query2="INSERT into ".$table_name."_old (entryid,itemid,percentchance,heroicpercentchance,mincount,maxcount) values ($entryid,$itemid,$percentchance,$heroicpercentchance,$mincount,$maxcount)";
		$get_res2=mysql_query($query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query2);
	}
}*/

$table_name="pickpocketingloot";
$query="select entryid,itemid,percentchance,heroicpercentchance,mincount,maxcount from $table_name";
$get_res=mysql_query($query,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query);
while(list($entryid,$itemid,$percentchance,$heroicpercentchance,$mincount,$maxcount)=mysql_fetch_row($get_res))
{
	$query2="select count(*) from ".$table_name."_old where itemid=$itemid and entryid=$entryid";
	$get_res2=mysql_query($query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query2);
	list($is_inserted)=mysql_fetch_row($get_res2);
	if($is_inserted==0)
	{
		$query2="INSERT into ".$table_name."_old (entryid,itemid,percentchance,heroicpercentchance,mincount,maxcount) values ($entryid,$itemid,$percentchance,$heroicpercentchance,$mincount,$maxcount)";
		$get_res2=mysql_query($query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi).$query2);
	}
}

?>