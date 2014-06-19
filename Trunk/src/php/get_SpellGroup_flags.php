<?php
foreach($_REQUEST as $request_name=>$request_value)
	$$request_name=mysql_escape_string($request_value);

if($f0 == "" )
	$f0 = 0;
if($f1 == "" )
	$f1 = 0;
if($f2 == "" )
	$f2 = 0;
?>

Please use only 1 version of input
<table>
	<form name="get_data" action="<?=$PHP_SELF;?>" method="post">
	<tr>
	  <td width="150" height="30" align="right">Spell Name 1:&nbsp;</td>
	  <td width="240"><input type="text" name="s1" style="width:180" value="<?=$s1?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">Spell Name 2:&nbsp;</td>
	  <td width="240"><input type="text" name="s2" style="width:180" value="<?=$s2?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">Spell Name 3:&nbsp;</td>
	  <td width="240"><input type="text" name="s3" style="width:180" value="<?=$s3?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">Spell Name 4:&nbsp;</td>
	  <td width="240"><input type="text" name="s4" style="width:180" value="<?=$s4?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">Spell id 1:&nbsp;</td>
	  <td width="240"><input type="text" name="sid1" style="width:180" value="<?=$sid1?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">Spell id 2:&nbsp;</td>
	  <td width="240"><input type="text" name="sid2" style="width:180" value="<?=$sid2?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">Spell id 3:&nbsp;</td>
	  <td width="240"><input type="text" name="sid3" style="width:180" value="<?=$sid3?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">Spell id 4:&nbsp;</td>
	  <td width="240"><input type="text" name="sid4" style="width:180" value="<?=$sid4?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">Possible solution f0:&nbsp;</td>
	  <td width="240"><input type="text" name="f0" style="width:180" value="<?=$f0?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">Possible solution f1:&nbsp;</td>
	  <td width="240"><input type="text" name="f1" style="width:180" value="<?=$f1?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">Possible solution f2:&nbsp;</td>
	  <td width="240"><input type="text" name="f2" style="width:180" value="<?=$f2?>"></td>
	</tr>
	<tr><td><input type="submit" value="get Flags"></td></tr>
	</form>
</table>

<?php
if( $s1 || $s2 || $s3 || $s14 )
{
	echo "!!! Not all spells have unique spell_group_flags !! <BR>";
	$dbhost="localhost";
	$dbuname="root";
	$dbupass="";
	$dbname="temp";
	$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
	mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/
	//get the skill line of one of the spells

	// get the ID of this spell name
	$get_query = "select id,name,description from dbc_spell where name in ('".mysql_escape_string( $s1 )."','".mysql_escape_string( $s2 )."','".mysql_escape_string( $s3 )."','".mysql_escape_string( $s4 )."')";
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)."<br> $get_query ");
	$spell_ids = "";
	while( $row=mysql_fetch_row($get_res) )
	{
		$spell_ids .= $row[0].",";
	}
	if( $sid1 )	$spell_ids .= $sid1.",";
	if( $sid2 )	$spell_ids .= $sid2.",";
	if( $sid3 )	$spell_ids .= $sid3.",";
	if( $sid4 )	$spell_ids .= $sid4.",";
	$spell_ids = substr( $spell_ids, 0 , -1 );
//echo $get_query."<br>";
	echo "Reference spell 1 ID : $spell_ids <br>";
	
	//get the skill of this spell 
	$get_query = "select distinct col_1 from skilllineabilitydbccsv where col_2 in ( $spell_ids )";
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)."<br> $get_query ");
	$spell_skill_ids = "";
	while( $row=mysql_fetch_row($get_res) )
		$spell_skill_ids .= $row[0].",";
	$spell_skill_ids = substr( $spell_skill_ids, 0 , -1 );
//echo $get_query."<br>";
	echo "Reference spell skill line ID : $spell_skill_ids <br>";

	//get possible spells for this skill
	$get_query = "select col_2 from skilllineabilitydbccsv where col_1 in ( $spell_skill_ids )";
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)."<br> $get_query ");
	$possible_id_list_for_skill = "";
	while($row=mysql_fetch_row($get_res))
		$possible_id_list_for_skill .= $row[0].",";
	$possible_id_list_for_skill = substr( $possible_id_list_for_skill, 0, -1 );
//echo $get_query."<br>";
	echo "Spells grouped in same skill lines : $possible_id_list_for_skill <br>";

	//get the flags of other ids then the ones we have
	$get_query = "select SpellGroupType_1,SpellGroupType_2,SpellGroupType_3 from dbc_spell where id not in ( $spell_ids ) and id in ( $possible_id_list_for_skill )";
	$shared_not_good_flags[0] = 0;
	$shared_not_good_flags[1] = 0;
	$shared_not_good_flags[2] = 0;
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)."<br> $get_query ");
	while($row=mysql_fetch_row($get_res))
	{
		$shared_not_good_flags[0] |= $row[0];
		$shared_not_good_flags[1] |= $row[1];
		$shared_not_good_flags[2] |= $row[2];
	}

	//get the flags that we might use
	$get_query = "select SpellGroupType_1,SpellGroupType_2,SpellGroupType_3,id,name,description from dbc_spell where id in ( $spell_ids )";
	$shared_good_flags[0] = 0;
	$shared_good_flags[1] = 0;
	$shared_good_flags[2] = 0;
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	while($row=mysql_fetch_row($get_res))
	{
		$shared_good_flags[0] |= $row[0];
		$shared_good_flags[1] |= $row[1];
		$shared_good_flags[2] |= $row[2];
		echo "(${row[3]})(${row[4]})(${row[5]})<br>";
	}

	//we need flags that are possibly not common
	echo "all flags for spells : ".$shared_good_flags[0]." ".$shared_good_flags[1]." ".$shared_good_flags[2]." <br>";
	$uf[0] = $shared_good_flags[0] & ~$shared_not_good_flags[0];
	$uf[1] = $shared_good_flags[1] & ~$shared_not_good_flags[1];
	$uf[2] = $shared_good_flags[2] & ~$shared_not_good_flags[2];
	echo "<b> unique flags for spells : ".($uf[0])." ".($uf[1])." ".($uf[2])." </b><br> ";
	echo "spell flags for this skilline and not for these spells : ".$shared_not_good_flags[0]." ".$shared_not_good_flags[1]." ".$shared_not_good_flags[2]." <br>";
	//let's see to make sure we will have only 1 active flag / spell
	$get_query = "select SpellGroupType_1,SpellGroupType_2,SpellGroupType_3,id,name,description,EffecGroup_1,EffecGroup_2,EffecGroup_3,Effect_1,Effect_2,Effect_3 from dbc_spell where id in ( $spell_ids )";
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	while($row=mysql_fetch_row($get_res))
	{
		echo "(${row[3]})(${row[4]})(${row[5]})(${row[8]},${row[9]},${row[10]})<br>";
		echo "&nbsp&nbsp&nbsp&nbsp&nbsp flags VS non good flags (".$row[0].",".$row[1].",".$row[2].") ".($row[0] & ~$shared_not_good_flags[0])." ".($row[1] & ~$shared_not_good_flags[1])." ".($row[2] & ~$shared_not_good_flags[2])." with bitcount ".CountBits($row[0] & ~$shared_not_good_flags[0])." ".CountBits($row[1] & ~$shared_not_good_flags[1])." ".CountBits($row[2] & ~$shared_not_good_flags[2])."<br>";
		echo "&nbsp&nbsp&nbsp&nbsp&nbsp current values match ours :  ".($uf[0] & $row[6])." ".($uf[1] & $row[7])." ".($uf[2] & $row[8])." <br> ";
		echo "&nbsp&nbsp&nbsp&nbsp&nbsp testing with our value :  ".($uf[0] & $f0)." ".($uf[1] & $f1)." ".($uf[2] & $f2)." <br> ";
		echo "&nbsp&nbsp&nbsp&nbsp&nbsp testing with our value :  ".($row[0] & $f0)." ".($row[1] & $f1)." ".($row[2] & $f2)." <br> ";
	}
	//see if currect flags are correct
	if( $f0 || $f1 || $f2 )
	{
		echo "<B>Experimental selected flags influence spells <BR></B>";
		$get_query = "select id,name,description from dbc_spell where ($f0=0 or (SpellGroupType_1 & $f0)) and ($f1=0 or (SpellGroupType_2 & $f1)) and ($f2=0 or (SpellGroupType_3 & $f2)) and id in ( $possible_id_list_for_skill ) group by name";
		$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)."<br>".$get_query);
		while($row=mysql_fetch_row($get_res))
			echo "(${row[0]})(${row[1]})(${row[2]})<br>";
	}
}

function CountBits( $num )
{
	$count = 0;
	$bitmask = 1;
	for($i=0;$i<32;$i++)
	{
		if( $num & $bitmask )
			$count++;
		$bitmask = $bitmask << 1;
	}
	return $count;
}
?>