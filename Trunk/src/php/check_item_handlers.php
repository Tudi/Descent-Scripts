<?php

set_time_limit ( 60 * 5 ); 

global $file_content;
$file_content = "";

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); 

$unique_spellids="";

//get spells
$get_query = "select spellid_1,spellid_2,spellid_3,spellid_4,spellid_5 from items ";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while($row=mysql_fetch_row($get_res) )
	for($i=0;$i<10;$i++)
		$unique_spellids[$row[$i]]=1;

//check proc spells
$sql_list="";
foreach($unique_spellids as $key => $val )
	if( $key > 0 )
		$sql_list.=$key.",";
$sql_list = substr($sql_list,0,-1);

$unique_proc_spellids="";
//$get_query = "select id from dbc_spell where id in ($sql_list) and (EffectApplyAuraName_1=42 or EffectApplyAuraName_2=42 or EffectApplyAuraName_3=42)";
$get_query = "select id from dbc_spell where id in ($sql_list) and (EffectApplyAuraName_1=4 or EffectApplyAuraName_2=4 or EffectApplyAuraName_3=4 or EffectApplyAuraName_1=42 or EffectApplyAuraName_2=42 or EffectApplyAuraName_3=42)";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)."<br> $get_query");
while($row=mysql_fetch_row($get_res) )
	$unique_proc_spellids[$row[0]]=1;

$as_sql_proclist="";
foreach($unique_proc_spellids as $key => $val )
{
	if( have_hardcoded_fixes_for( $key ) == 0 )
	{
		echo $key."<br>";
		$as_sql_proclist .=$key.",";
	}
}
$as_sql_proclist = substr($as_sql_proclist,0,-1);
echo "select id,name,description from dbc_spell where id in ($as_sql_proclist);<br>";

//omg strpos fails on large strings
function strfind( $haystack, $needle )
{
	$needle = $needle."";
	$search_letter = 0;
	$total_letters = strlen($needle);
	$stacklen = strlen( $haystack );
//echo "needle len = $total_letters, stack len = $stacklen, stack :  $haystack, needle : $needle <br>";
	for($i=0;$i<$stacklen;$i++)
	{
//echo "testing ".$haystack[$i]." with ".$needle[$search_letter]." <br> ";
		if( $haystack[$i] == $needle[$search_letter] )
		{
			$search_letter++;
			if( $search_letter == $total_letters )
				return 1;
		}
		else 
			$search_letter = 0;
	}
	if( $search_letter == $total_letters )
		return 1;
	return 0;
}

function have_hardcoded_fixes_for( $entry )
{
	global $file_content;
	if( $file_content == "" )
	{
		$file = fopen( "SpellFixes.cpp", "r" );
		while(!feof($file))
			$file_content .= trim( fgets($file) );
	}
//	return strpos( $file_content, $entry );	// -> fails at large files !!!
	return strfind( $file_content, $entry );
}


//echo $file_content;
//echo "<br> ! ".strpos(  $file_content, 67771 );
//echo "<br> ! ".strfind(  $file_content, 67771 );
//echo "<br> ! ".strfind(  " test 67771 ", "67771" );
