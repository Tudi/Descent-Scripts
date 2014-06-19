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
	$result=mysql_query("select level from characters where guid='$user_guid'",$dbi) or die("Q 200501180949".mysql_error($dbi));
	list($level)=mysql_fetch_row($result);
	$player_strength = $level * 5;
	
	return $player_strength;
}
?>