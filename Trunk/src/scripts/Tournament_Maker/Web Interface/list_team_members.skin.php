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
		$query = "select id,team_name from tournament_team_settings where tournament_id='$cur_tournament_id' and matches_won=$winc";
//echo $query."<BR>";
		$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
		while( list( $cur_team_id,$team_name )=mysql_fetch_row($result) )
		{
			if( $cur_team_id == "" )
				continue; //seems like there are no teams with this winc :)

			echo "=============================<BR>";
			echo ($rank+1)."$team_name : <br>";
			$players="";
			$players_c=0;
			$query = "select player_guid from tournament_players where tournament_id='$cur_tournament_id' and team_id=$cur_team_id and is_pending_to_join_team=0 and is_pending_to_accept_team_invite=0";
			$result1=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
			while(list( $pguid )=mysql_fetch_row($result1))
			{
				//get name
				$query = "select name from characters where guid=$pguid";
				$result2=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
				list( $p_name )=mysql_fetch_row($result2);
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$p_name - $pguid <br>";
			}
		}
	}
/*	echo "/////////////////////// normal listing ////////////////////////////////////<BR>";
	//what was the last match for this tournament ?
	$query = "select id,team_name from tournament_team_settings where tournament_id='$cur_tournament_id'";
	$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
	$rank=0;
	while(list( $cur_team_id,$team_name )=mysql_fetch_row($result) )
	{
		$rank++;
		echo "=============================<BR>";
		echo $rank.")$team_name : <br>";
		$players="";
		$players_c=0;
		$query = "select player_guid from tournament_players where tournament_id='$cur_tournament_id' and team_id=$cur_team_id and is_pending_to_join_team=0 and is_pending_to_accept_team_invite=0";
//echo $query."<BR>";
		$result1=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
		while(list( $pguid )=mysql_fetch_row($result1))
		{
			//get name
			$query = "select name from characters where guid=$pguid";
			$result2=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
			list( $p_name )=mysql_fetch_row($result2);
			echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$p_name - $pguid <br>";
		}
	}*/
?>