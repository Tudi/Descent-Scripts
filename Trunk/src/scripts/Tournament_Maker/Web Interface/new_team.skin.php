<br />
<table width="770"  border="0" align="center" cellpadding="0" cellspacing="0">
   <tr>
		<td>
			<?php
				//direct acces of a skin is not allowed
				if( !$dbi )
					die("Can't acces page directly");

				//check if we can create a new team
/*				$query="select count(*) from tournament_team_settings where leader_acc='$s_acct_id'";
//echo $query."<br>";
				$result=mysql_query($query,$dbi) or die("Q 2005011809491".mysql_error($dbi));
				list($team_count)=mysql_fetch_row($result);
				if( $s_user_max_teams<=$team_count )
					die("You reached the max amount of teams per account");*/

				//check if we reached max amount of teams per tournament. Never going to be used
				$query = "select count(*) from tournament_team_settings where tournament_id='$s_user_tournament'";
				$result=mysql_query($query,$dbi) or die("Q 2005011809492".mysql_error($dbi));
				list($teamcount)=mysql_fetch_row($result);
				if( $teamcount > $s_max_teams )
					die("Cannot create new team.Reached maximum number of teams per tournament");

				//check if we can create a new team
				$query = "select id,team_name,is_open_team,team_desc_public,team_desc_private,team_passw from tournament_team_settings where team_leader_guid='$s_user_guid' and tournament_id='$s_user_tournament'";
//echo $query."<br>";
				$result=mysql_query($query,$dbi) or die("Q 2005011809492".mysql_error($dbi));
				list($d_id,$d_team_name,$d_is_open_team,$d_team_desc_public,$d_team_desc_private,$d_team_passw)=mysql_fetch_row($result);

				//check if we are pending joins
				$query = "select player_guid from tournament_players where player_guid='$s_user_guid' and tournament_id='$s_user_tournament' and team_id!='$d_id'";
//echo $query."<br>";
				$result=mysql_query($query,$dbi) or die("Q 2005011809493".mysql_error($dbi));
				list($is_joining)=mysql_fetch_row($result);
				if( $is_joining )
					die("You already joind or pending arena team join. You can't both host and join a team.");

				//register new team
				if( isset( $submited_data ) )
				{
					if($is_open_team!="")
						$is_open_team = 1;
					if($team_name=="")
						$team_name="Noname";

					//we have a new team or we are just editing old one ?
					if( $d_id=="" )
					{
						//you may only create a team for this tournament if it did not start yet
						$time = time();
						$query = "select id from tournament_settings where id='$s_user_tournament' and tournament_starts>'$time'";
//echo $query."<br>";
						$result=mysql_query($query,$dbi) or die("Q 2005011809494".mysql_error($dbi));
						list($is_open_for_subscribe)=mysql_fetch_row($result);
						if( $is_open_for_subscribe!="" )
						{
							$result=mysql_query("Insert into tournament_team_settings (tournament_id,team_leader_guid,team_name,leader_acc,is_open_team,team_desc_public,team_desc_private,team_passw)	values('$s_user_tournament','$s_user_guid','$team_name','$s_acct_id','$is_open_team','$team_desc_pub','$team_desc_priv','$team_passw')",$dbi) or die("Q 2005011809495".mysql_error($dbi));
							$new_team = mysql_insert_id( $dbi );

							//make sure we are not pending for any other teams
							$result=mysql_query("delete from tournament_players where player_guid='$s_user_guid' and tournament_id='$s_user_tournament'",$dbi) or die("Q 2005011809497".mysql_error($dbi));

							//add leader as part of the team members
							$result=mysql_query("Insert into tournament_players (team_id,player_guid,tournament_id,is_pending_to_join_team,is_pending_to_accept_team_invite,strength_points) values('$new_team','$s_user_guid','$s_user_tournament','0','0','".Get_player_strength($s_user_guid)."')",$dbi) or die("Q 2005011809498".mysql_error($dbi));
						}
						else
							die("Tournament Started. Cannot subscribe now.");
					}
					else
					{
						$query = "update tournament_team_settings set team_name = '$team_name',is_open_team='$is_open_team',team_desc_public='$team_desc_pub',team_desc_private='$team_desc_priv',team_passw='$team_passw' where team_leader_guid='$s_user_guid' and tournament_id='$s_user_tournament' ";
						$result=mysql_query($query,$dbi) or die("Q 2005011809496".mysql_error($dbi).$query);
					}
					$d_team_name = $team_name;
					$d_is_open_team = $is_open_team;
					$d_team_desc_public = $team_desc_pub;
					$d_team_desc_private  = $team_desc_priv;
					$d_team_passw = $team_passw;

					Alert("Team registered / updated");
					jumpto("index.php");
				}

				//check if we are part of any teams so we can warn morron
				$result=mysql_query("select count(*) from tournament_players where player_guid='$s_user_guid' and team_id!='$d_id' group by team_id",$dbi) or die("Q 2005011809499".mysql_error($dbi));
				list($part_of_teams)=mysql_fetch_row($result);
				if( $part_of_teams > 0 )
					Alert("You are part of $part_of_teams teams. Creating a team will remove you from any team you joined or pending to join");
				
				if( $d_id )
					$button_text = " Update Team ";
				else
					$button_text = " Create Team ";
			?>
			<form name="getinfoform" action="index.php?skin=new_team.skin.php" method="post">
				<tr>
				  <td width="150" height="30" align="right" class="clsLoginText01">Team Name:&nbsp;</td>
				  <td width="240"><input type="text" name="team_name" style="width:180" class="clsTextField01" value="<?=$d_team_name;?>"></td>
				</tr>
				<tr>
				  <td width="150" height="30" align="right" class="clsLoginText01">Passw for quick join:&nbsp;</td>
				  <td width="240"><input type="text" name="team_passw" style="width:180" class="clsTextField01" value="<?=$d_team_passw;?>"></td>
				</tr>
				<?php if( $s_user_max_team_members > 1 ) { ?>
				<tr>
				  <td width="150" height="30" align="right" class="clsLoginText01">Team info public:&nbsp;</td>
				  <td width="240"><input type="text" name="team_desc_pub" style="width:180" class="clsTextField01" value="<?=$d_team_desc_public;?>"></td>
				</tr>
				<tr>
				  <td width="150" height="30" align="right">Team info private:&nbsp;</td>
				  <td width="240"><input type="text" name="team_desc_priv" style="width:180" class="clsTextField01" value="<?=$d_team_desc_private;?>"></td>
				</tr>
				<tr>
				  <td height="30" align="right">Open team( players can subscribe for join ):&nbsp;</td>
				  <td><input type="checkbox" name="is_open_team" style="width:180" class="clsTextField01" <?php if($d_is_open_team) echo "checked"; ?>></td>
				</tr>
				<?php } ?>
				<tr>
				  <td height="30"><img src="images/blank.gif" width="1" height="1"></td>
				  <td><input type="submit" value="<?=$button_text;?>" style="width:90px"></td>
				</tr>
				<input type="hidden" name="submited_data" value="1" >
			</form>
		</td>
	</tr>
</table>

