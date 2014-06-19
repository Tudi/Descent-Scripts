<?php
	//direct acces of a skin is not allowed
	if( !$dbi )
		die("Can't acces page directly");
	$cur_tournament_id=$id;//we got this as input :P

	//mark player rankings
	include_once( "player_rankings.skin.php" );

	//send rewards
	include_once( "sendrewards.skin.php" );

	//list team members
	include_once( "list_team_members.skin.php" );

	//reset the teams
	$query = "update tournament_team_settings set matches_won_total = matches_won_total + matches_won,matches_won=0,matches_finished=0 where tournament_id='$cur_tournament_id' ";
	$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);

	//reset the tournament
	$query = "update tournament_settings set cur_match_round = 0 where id='$cur_tournament_id' ";
	$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
?>