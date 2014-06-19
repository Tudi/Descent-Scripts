<?php
foreach($_REQUEST as $request_name=>$request_value)
	$$request_name=mysql_escape_string($request_value);
if( $basehp == "" || $basehp <= 0 )
{
	$basehp = 100;
	$fdur = 120000;
	$dmg = 1;
	$cooldown = 2000;
	$edur = 0;
}
?>
<table>
	<form name="get_data" action="<?=$_SERVER['PHP_SELF'];?>" method="post">
	<tr>
	  <td width="150" height="30" align="right">Base char HP &nbsp;</td>
	  <td width="240"><input type="text" name="basehp" style="width:1024" value="<?=$basehp?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">fight duration we are testing this item. Not same as expected fight duration</td>
	  <td width="240"><input type="text" name="fdur" style="width:300" value="<?=$fdur?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">Damage per trigger</td>
	  <td width="240"><input type="text" name="dmg" style="width:300" value="<?=$dmg?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">effect cooldown or attack interval</td>
	  <td width="240"><input type="text" name="cooldown" style="width:300" value="<?=$cooldown?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">effect duration. 0 is attack based event</td>
	  <td width="240"><input type="text" name="edur" style="width:300" value="<?=$edur?>"></td>
	</tr>
	<tr><td><input type="submit" value="filter logs"></td></tr>
	</form>
</table>
<?php
$count_activete_over_duration = $fdur / $cooldown;
echo "times it will apply buff or effect over fight duration : ".$count_activete_over_duration."<br>";
if( $edur <= 0 )
	$edur = $cooldown;
//$time_active_over_duration = $count_activete_over_duration * $edur / 1000;
//echo "count it will be active due to cooldown over fight duration : ".$time_active_over_duration."<br>";
$count_activate_while_active = $time_active_over_duration / $edur;
$damage_done_over_duration = $count_activete_over_duration * $dmg;
echo "damage it will do only when it is active : ".$damage_done_over_duration."<br>";
$dps = $damage_done_over_duration * 1000 / $fdur;
echo "item DPS : ".$dps."<br>";
$actual_player_fight_time = $basehp / $dps;
echo "Time required to kill player ".$actual_player_fight_time." seconds <br>";
?>