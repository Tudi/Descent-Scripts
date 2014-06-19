<?php
	//direct acces of a skin is not allowed
	if( !$dbi )
		die("Can't acces page directly");
	$mails_sent = 0;
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
		while( list( $cur_team_id )=mysql_fetch_row($result) )
		{
			if( $cur_team_id == "" )
				continue; //seems like there are no teams with this winc :)

			$real_rank++;
			$players="";
			$players_c=0;
			$query = "select player_guid from tournament_players where tournament_id='$cur_tournament_id' and team_id=$cur_team_id and is_pending_to_join_team=0 and is_pending_to_accept_team_invite=0";
			$result1=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
			while(list( $pguid )=mysql_fetch_row($result1))
				$players[$players_c++] = $pguid;
			//now select rewards for this rank and send them all
			$mail_gold = 50000000;
			$query = "select item_id,item_count from tournament_rewards where tournament_id='$cur_tournament_id' and team_ranking=$real_rank";
			$result2=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
			while(list( $item_id,$item_count  )=mysql_fetch_row($result2))
				for($ms=0;$ms<$players_c;$ms++)
				{
					$query = "select matches_won,matches_played from tournament_player_info where player_guid='".$players[$ms]."'";
					$result3=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
					list( $matches_won,$matches_played )=mysql_fetch_row($result3);
					$win_pct = ($matches_played+1) * 100 / ($matches_won+1);

					$query="INSERT INTO mailbox_insert_queue (sender_guid, receiver_guid, subject, body, stationary, money, item_id, item_stack) VALUES ('".$players[$ms]."', '".$players[$ms]."', '".mysql_escape_string("Tournament Rewards")."', '".mysql_escape_string("you ranked at $real_rank place in the tournament. Your overall ranking is $win_pct percent.Here is your reward")."', 61, '$mail_gold', '$item_id', '$item_count')";
	//echo $query."<br>";
					$result3=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi));
					$mails_sent++;
				}
		}
	}
	Alert("$mails_sent mails with reward has been sent");
?>