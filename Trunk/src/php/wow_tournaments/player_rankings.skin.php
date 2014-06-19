<?php
	//direct acces of a skin is not allowed
	if( !$dbi )
		die("Can't acces page directly");
	$cur_tournament_id=$id;//we got this as input :P

	//what was the last match for this tournament ?
	$query = "select matches_finished from tournament_team_settings where tournament_id='$cur_tournament_id' and matches_finished!=999999 order by matches_finished desc limit 0,1";
	$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
	list( $matches_in_tournament )=mysql_fetch_row($result);

	//make sure tournament is finished
/*	$query = "select matches_finished from tournament_team_settings where tournament_id='$cur_tournament_id' and matches_finished!='$matches_in_tournament' ";
	$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
	list( $unfinished_match )=mysql_fetch_row($result);
	if( $unfinished_match != "" )
		die("This match is not finished yet");*/

	$real_rank=0;
	for( $rank=0;$rank<=$matches_in_tournament; $rank++)
	{

		$winc=$matches_in_tournament - $rank; //first rank won all matches ;)
		//select rewards where whin count >= $rank
		$query = "select id from tournament_team_settings where tournament_id='$cur_tournament_id' and matches_won=$winc";
//echo $query."<BR>";
		$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
		list( $cur_team_id )=mysql_fetch_row($result);
		if( $cur_team_id == "" )
			continue; //seems like there are no teams with this winc :)

		$real_rank++;
		$players="";
		$players_c=0;
		$query = "select player_guid from tournament_players where tournament_id='$cur_tournament_id' and team_id=$cur_team_id and is_pending_to_join_team=0 and is_pending_to_accept_team_invite=0";
		$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
		while(list( $pguid )=mysql_fetch_row($result))
		{
			//insert blank row un case there is none
			$query = "INSERT IGNORE INTO tournament_player_info (player_guid,matches_won,matches_played) values ( $pguid,0,0 )";
			$result2=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
			//if row was not inserted yet then just update it
			$query = "update tournament_player_info set matches_won = matches_won + '$winc', matches_played = matches_played + '$matches_in_tournament' where player_guid=$pguid";
			$result2=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
		}
	}
?>