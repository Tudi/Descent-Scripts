<?php
	//direct acces of a skin is not allowed
	if( !$dbi )
		die("Can't acces page directly");

	//we will create a matrix NxN -> let's get N first
	$query = "select count(*) from tournament_team_settings where tournament_id='$s_user_tournament'";
	$result=mysql_query($query,$dbi) or die("Q 2005011809491".mysql_error($dbi));
	list($teams_available )=mysql_fetch_row($result);

	//see if we have the team strengths set up
	$query = "select strength_ordering from tournament_team_settings where tournament_id='$s_user_tournament' limit 0,1";
	$result=mysql_query($query,$dbi) or die("Q 2005011809492".mysql_error($dbi));
	list( $has_strength_defined )=mysql_fetch_row($result);
	if( $has_strength_defined == 0 )
	{
		//fetch all teams and define there strengths
		$query = "select id from tournament_team_settings where tournament_id='$s_user_tournament' and strength_ordering=0";
		$result=mysql_query($query,$dbi) or die("Q 2005011809493".mysql_error($dbi));
		while(list($cur_team_id )=mysql_fetch_row($result) )
		{
			//get the strength of this team
			$query = "select sum(strength_points) from tournament_players where tournament_id='$s_user_tournament' and team_id='$cur_team_id' and is_pending_to_join_team=0 and is_pending_to_accept_team_invite=0";
			$result1=mysql_query($query,$dbi) or die("Q 2005011809494".mysql_error($dbi));
			list( $cur_team_strength )=mysql_fetch_row($result1);
			$query = "update tournament_team_settings set strength_ordering='$cur_team_strength' where tournament_id='$s_user_tournament' and id='$cur_team_id'";
			$result2=mysql_query($query,$dbi) or die("Q 2005011809495".mysql_error($dbi));
		}
	}

	//create the table
?>
<table width="400"  border="1" align="center" cellpadding="0" cellspacing="0">
		<tr>
		<br />
		<center>
<TD width=400 height=20 bgcolor="#352516" colspan="<?=$teams_available;?>">
<div align=center><font color="#FFFFFF"><B>TEAMS</B></font></div>
</TD>

	<?php 
	//pregenrate and NxN matrix for the match ladder
	$ladder="";
	$is_looser="";
	for($match_round=0;$match_round<$teams_available;$match_round++)
	{
		$teamnr=0;
		$query = "select id,team_name,matches_won from tournament_team_settings where tournament_id='$s_user_tournament' and matches_won>='$match_round' order by id asc";
		$result=mysql_query($query,$dbi) or die("Q 2005011809494".mysql_error($dbi));
		while(list($cur_team_id,$team_name,$matches_won )=mysql_fetch_row($result) )
		{
			$ladder[$match_round][$teamnr]=$team_name;
			$is_looser[$match_round][$teamnr]=($matches_won==$match_round);
			$teamnr++;
		}
	}
//print_r($ladder);
//echo "$teams_available <br>";
	for($match_round=0;$match_round<$teams_available;$match_round++) //we map this to columns
	{
		?>
		<tr>
			<TD><?php echo $ladder[0][$match_round];?></td>
		</tr>
	<?php
	}
	?>
</table>
</center>

<table width="400"  border="1" align="center" cellpadding="0" cellspacing="0">
		<tr>
		<br />
		<center>
<TD width=400 height=20 bgcolor="#352516" colspan="<?=$teams_available;?>">
<div align=center><font color="#FFFFFF"><B>Ladder</B></font></div>
</TD>

	<?php 
	$span_sizes="";
	for($match_round=0;$match_round<$teams_available;$match_round++) //we map this to columns
	{
		$number_of_matches_in_round=count($ladder[$match_round]);
		$prev_round_ind = 0;	//we need to monitor how many teams lost in previous match. Using this iter for it
		?>
		<tr align="center">
		<?php 
		for($cur_match=0;$cur_match<$number_of_matches_in_round;$cur_match++) //map this to rows
		{
			if( $match_round == 0 )
				$span_size = 1;
			else 
			{
				$span_size = 0;
				$has_winner = 0;
				$matches_merged=0;
				//we need to have at least 1 winner
				for(;$prev_round_ind<=$number_of_matches_in_round;)
				{
//echo ($match_round-1)." - $prev_round_ind <BR>";
					$span_size +=$span_sizes[$match_round-1][$prev_round_ind]; //second row this is eighter 2 or 1
					if( $is_looser[$match_round-1][$prev_round_ind] != 1 )
						$has_1_winner = 1;
					$prev_round_ind++;
					$matches_merged++;
					if($matches_merged>=2 && $has_1_winner )
						break;
				}
//echo "====<br>";
			}
			$span_sizes[$match_round][$cur_match] = $span_size;
//echo "$match_round - $cur_match -> $ladder[$cur_match][$match_round] <br>";
			$color = "";
			if( $is_looser[$match_round][$cur_match] )
				$color = "bgcolor=\"#FF0000\"";
//echo ((int)($cur_match*$stacking_precission))."!=".((int)(($cur_match+1)*$stacking_precission))." / $stacking_precission - $cur_match - $rowspan_add<br>";
			$name = $ladder[$match_round][$cur_match]; //this can be null too
			{ ?>
				<TD colspan="<?=$span_size?>" <?=$color;?> ><?=$name;?></td>
			<?php
			}
		}
			?>
		</tr>
	<?php
	}
	?>
</table>
</center>
