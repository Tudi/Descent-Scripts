<?php
	//direct acces of a skin is not allowed
	if( !$dbi )
		die("Can't acces page directly");
	//check if we are team owner
	//check if we have a team
	$result=mysql_query("select id from tournament_team_settings where team_leader_guid='$s_user_guid' and tournament_id='$s_user_tournament'",$dbi) or die("Q 200501180949".mysql_error($dbi));
	list($team_id)=mysql_fetch_row($result);
	if( $team_id!="" )
		include_once("my_team_owner.skin.php");
	else
		include_once("my_team_normal.skin.php");
?>