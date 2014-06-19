<?php
/*
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); 
*/
$spell_durations[1]=10000;
$spell_durations[2]=300000010;
$spell_durations[3]=60000;
$spell_durations[4]=120000;
$spell_durations[5]=300000;
$spell_durations[6]=600000;
$spell_durations[7]=5000000;
$spell_durations[8]=15000;
$spell_durations[9]=30000;
$spell_durations[10]=60000000;
$spell_durations[11]=100000000;
$spell_durations[12]=30000000;
$spell_durations[13]=6000000;
$spell_durations[14]=12000000;
$spell_durations[15]=30000000;
$spell_durations[16]=230000;
$spell_durations[17]=5000000;
$spell_durations[18]=20000;
$spell_durations[19]=3000000;
$spell_durations[20]=60000000;
$spell_durations[21]=2147483647;
$spell_durations[22]=45000;
$spell_durations[23]=90000;
$spell_durations[24]=160000;
$spell_durations[25]=180000;
$spell_durations[26]=240000;
$spell_durations[27]=3000;
$spell_durations[28]=5000;
$spell_durations[29]=12000;
$spell_durations[30]=1800000;
$spell_durations[31]=8000;
$spell_durations[32]=6000;
$spell_durations[35]=4000;
$spell_durations[36]=1000;
$spell_durations[37]=1;
$spell_durations[38]=11000;
$spell_durations[39]=2000;
$spell_durations[40]=1200000;
$spell_durations[41]=360000;
$spell_durations[42]=3600000;
$spell_durations[62]=75000;
$spell_durations[63]=25000;
$spell_durations[64]=40000;
$spell_durations[65]=1500;
$spell_durations[66]=2500;
$spell_durations[85]=18000;
$spell_durations[86]=21000;
$spell_durations[105]=9000;
$spell_durations[106]=24000;
$spell_durations[125]=35000;
$spell_durations[145]=2700000;
$spell_durations[165]=7000;
$spell_durations[185]=21000;
$spell_durations[186]=22000;
$spell_durations[187]=5000;
$spell_durations[205]=27000;
$spell_durations[225]=604800000;
$spell_durations[245]=50000;
$spell_durations[265]=55000;
$spell_durations[285]=6000;
$spell_durations[305]=14000;
$spell_durations[325]=36000;
$spell_durations[326]=44000;
$spell_durations[327]=500;
$spell_durations[328]=250;
$spell_durations[347]=900000;
$spell_durations[367]=7200000;
$spell_durations[387]=16000;
$spell_durations[407]=100;
$spell_durations[427]=600000;
$spell_durations[447]=6000;
$spell_durations[467]=22000;
$spell_durations[468]=26000;
$spell_durations[487]=1700;
$spell_durations[507]=1100;
$spell_durations[508]=1100;
$spell_durations[527]=14400000;
$spell_durations[547]=5400000;
$spell_durations[548]=10800000;
$spell_durations[549]=3800;
$spell_durations[550]=2147483647;
$spell_durations[551]=3500;
$spell_durations[552]=210000;
$spell_durations[553]=16000;
$spell_durations[554]=155000;
$spell_durations[555]=4500;
$spell_durations[556]=28000;
$spell_durations[557]=165000;
$spell_durations[558]=114000;
$spell_durations[559]=53000;
$spell_durations[560]=299000;
$spell_durations[561]=3300000;
$spell_durations[562]=150000;
$spell_durations[563]=20500;
$spell_durations[564]=13000;
$spell_durations[565]=70000;
$spell_durations[566]=0;
$spell_durations[567]=135000;
$spell_durations[568]=1250;
$spell_durations[569]=280000;
$spell_durations[570]=32000;
$spell_durations[571]=5500;
$spell_durations[572]=100000;
$spell_durations[573]=119999;
$spell_durations[574]=200;
$spell_durations[575]=17000;
$spell_durations[576]=43200000;
$spell_durations[577]=160000;
$spell_durations[578]=14250;
$spell_durations[579]=170000;
$spell_durations[580]=64800000;
$spell_durations[581]=34000;

foreach($_REQUEST as $key => $value)
	$$key=mysql_escape_string($value);

$AGENT=4;	//we are only creating spells 
?>
<table>
	<form name="get_data" action="<?=$_SERVER['PHP_SELF'];?>" method="post">
	<tr>
	  <td width="150" height="30" align="right">mob id</td>
	  <td width="240"><input type="text" name="mob_entry" style="width:180" value="<?=$mob_entry?>"></td>
	  <td> Bind this new event to a mob </td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">spell id&nbsp;</td>
	  <td width="240"><input type="text" name="spell_id" style="width:180" value="<?=$spell_id?>"></td>
	  <td> It is used when agent spell is selected. Practicaly mob will cast this spell if conditions are met</td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">Target type:&nbsp;</td>
	  <?php if($ttype=="") $ttype=1; ?>
	  <td width="240">
				<select name="ttype">
					<option value="1" <?php if($ttype==1) echo "selected"; ?>>Our attacktarget</option>
					<option value="2" <?php if($ttype==2) echo "selected"; ?>>location of our attacktarget</option>
					<option value="3" <?php if($ttype==3) echo "selected"; ?>>location where we stand now</option>
					<option value="4" <?php if($ttype==4) echo "selected"; ?>>Ourself</option>
					<option value="5" <?php if($ttype==5) echo "selected"; ?>>Owner(pet ?)</option>
					<option value="6" <?php if($ttype==6) echo "selected"; ?>>second most hated</option>
					<option value="7" <?php if($ttype==7) echo "selected"; ?>>random healer(priest,pala,druid,shaman)</option>
					<option value="8" <?php if($ttype==8) echo "selected"; ?>>random tank(war,pala,druid)</option>
					<option value="9" <?php if($ttype==9) echo "selected"; ?>>random dps(mage,hunt,rog,dk,warl)</option>
					<option value="10" <?php if($ttype==10) echo "selected"; ?>>anyone attackable at range</option>
					<option value="11" <?php if($ttype==11) echo "selected"; ?>>friendly to buff</option>
					<option value="12" <?php if($ttype==12) echo "selected"; ?>>friendly to heal</option>
				</select>
		</td>
	  <td>name should explain it. Try to cast spell on given target types. Note that spell has internal targetting too. You can't cast self buffs on others </td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">Initial cooldown:&nbsp;</td>
	  <?php if($initial_cooldown=="" || $initial_cooldown==0 ) $initial_cooldown=20000; ?>
	  <td width="240"><input type="text" name="initial_cooldown" style="width:180" value="<?=$initial_cooldown?>"></td>
	  <td>Time needs to be spent since we entered combat to cast spell</td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">Cooldown:&nbsp;</td>
	  <?php 
		if($Cooldown=="") 
			$Cooldown=10000; 
		else if( $Cooldown % 10000 != 0 )
		{
			//randomize cooldown for us
			srand();
			$Cooldown = $Cooldown + mt_rand (-20,20) * 1000;
		}
		if($Cooldown <= 0 ) 
			$Cooldown=10001; 
		?>
	  <td width="240"><input type="text" name="Cooldown" style="width:180" value="<?=$Cooldown?>"></td>
	  <td>minimum time between 2 casts of this spell</td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">Global cooldown:&nbsp;</td>
	  <?php if($global_Cooldown=="") $global_Cooldown=2000; ?>
	  <td width="240"><input type="text" name="global_Cooldown" style="width:180" value="<?=$global_Cooldown?>"></td>
	  <td>minimum time between this and next any cast</td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">min range:&nbsp;</td>
	  <?php if($min_range=="") $min_range=0; ?>
	  <td width="240"><input type="text" name="min_range" style="width:180" value="<?=$min_range?>"></td>
	  <td>Cast spell only if selected target is further then given range</td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">max range:&nbsp;</td>
	  <?php if($max_range=="") $max_range=30; ?>
	  <td width="240"><input type="text" name="max_range" style="width:180" value="<?=$max_range?>"></td>
	  <td>Cast spell only if selected target is closer then given range</td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">min hp pct:&nbsp;</td>
	  <?php if($min_hp=="") $min_hp=0; ?>
	  <td width="240"><input type="text" name="min_hp" style="width:180" value="<?=$min_hp?>"></td>
	  <td>Cast spell only if selected target has health higher </td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">max hp pct:&nbsp;</td>
	  <?php if($max_hp=="") $max_hp=90; ?>
	  <td width="240"><input type="text" name="max_hp" style="width:180" value="<?=$max_hp?>"></td>
	  <td>Cast spell only if selected target has health lower </td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">Agent:&nbsp;</td>
	  <?php if($agent=="") $agent=4; ?>
	  <td width="240">
				<select name="agent">
					<option value="1" <?php if($agent==1) echo "selected"; ?>>Melee</option>
					<option value="2" <?php if($agent==2) echo "selected"; ?>>Ranged</option>
					<option value="3" <?php if($agent==3) echo "selected"; ?>>Flee</option>
					<option value="4" <?php if($agent==4) echo "selected"; ?>>Spell</option>
					<option value="5" <?php if($agent==5) echo "selected"; ?>>Call for help</option>
				</select>
		</td>
	  <td>Switch ai behavior. Default is melee. Spell type ads a new spell to NPC spellbook </td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">Chance:&nbsp;</td>
	  <?php if($chance=="") $chance=50; ?>
	  <td width="240"><input type="text" name="chance" style="width:180" value="<?=$chance?>"></td>
	  <td>Note that if you have too small cooldowns spells from the beggining of the list might get casted more then those at the end. Set chance low to give all spells a chance to get casted</td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">maxcount&nbsp;</td>
	  <?php if($maxcount=="") $maxcount=0; ?>
	  <td width="240"><input type="text" name="maxcount" style="width:180" value="<?=$maxcount?>"></td>
	  <td>if a non 0 value is specified then mob will only cast this spell X times since he enterd combat</td>
	</tr>

	<tr>
	  <td width="150" height="30" align="right">shout before cast&nbsp;</td>
	  <td width="240"><input type="text" name="shout" style="width:180" value="<?=$shout?>"></td>
	  <td>mob will shout this before casting the spell</td>
	</tr>
	
	<tr>
	  <td width="150" height="30" align="right">difficulty mask&nbsp;</td>
	  <?php if($diff_mask=="") $diff_mask=65535; ?>
	  <td width="240"><input type="text" name="diff_mask" style="width:180" value="<?=$diff_mask?>"></td>
	  <td>map modes are represented by a mask(see worldmapinfo), same flags can limit spell casts</td>
	</tr>
	
	<tr><td><input type="submit" value="Generate SQL"></td></tr>
	</form>
</table>

<?php
//check if cooldown is big enough
if( $spell_id && $mob_entry)
{
/*	$get_query1 = "select RecoveryTime,CategoryRecoveryTime,DurationIndex from dbc_spell where id='$spell_id'";
	$get_res1=mysql_query($get_query1,$dbi) or die(" Q20061123150142 ".mysql_error($dbi));
	list($RecoveryTime,$CategoryRecoveryTime,$DurationIndex )=mysql_fetch_row($get_res1);
	if( $RecoveryTime=="")
		echo "!!spell does not exist in db ? <br>";
	$recon_cooldown = 2000;
	if($RecoveryTime > $recon_cooldown)
		$recon_cooldown=$RecoveryTime;
	if($CategoryRecoveryTime > $recon_cooldown)
		$recon_cooldown=$CategoryRecoveryTime;
	if($spell_durations[$DurationIndex]>$recon_cooldown)
		$recon_cooldown=$spell_durations[$DurationIndex];
	if( $recon_cooldown > $Cooldown)
		echo "from spell DBC cooldown should be $recon_cooldown. are you sure you want to use existing one <br>";
*/
	echo "<br>";

	echo "INSERT INTO ai_agents (entry, type, minrange, chance, maxcount, spell, maxrange, targettype, cooldown, min_hp_pct, max_hp_pct, initial_cooldown, global_cooldown, shout_before_cast, difficulty_mask) 
	VALUES ($mob_entry, $agent, $min_range, $chance, $maxcount, $spell_id, $max_range, $ttype, $Cooldown, $min_hp, $max_hp, $initial_cooldown, $global_Cooldown, '".mysql_escape_string($shout)."', $diff_mask);";

	echo "<br>";
}


if( $dbi)
	mysql_close($dbi);
?>