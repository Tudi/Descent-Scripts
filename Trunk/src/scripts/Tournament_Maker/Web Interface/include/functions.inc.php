<?php
function jumpto($jumpto)
{
include("include/jumpto.inc.php");
}
function message($message)
{
include("include/message.inc.php");
}
function Alert($text)
{
	?>
	<script language="JavaScript" type="text/javascript">
		alert("<?=$text?>");
	</script>
	<?php
}
function Get_player_strength($user_guid)
{
	global $dbi;
	//get player level
	$result=mysql_query("select matches_won,matches_played from tournament_player_info where player_guid='$user_guid'",$dbi) or die("Q 200501180949".mysql_error($dbi));
	list($matches_won,$matches_played )=mysql_fetch_row($result);
	$player_strength = $matches_won * 5 + $matches_played + ($matches_won + 1)*100 / ($matches_played+1);
	
	return $player_strength;
}
?>