<br />
<table width="770"  border="0" align="center" cellpadding="0" cellspacing="0">
   <tr>
		<td>
			<?php
				//direct acces of a skin is not allowed
				if( !$dbi )
					die("Can't acces page directly");

				//check if we are part of any teams so we can warn morron
				$result=mysql_query("select count(*) from tournament_players where player_guid='$s_user_guid' and team_id!='$d_id' group by team_id",$dbi) or die("Q 2005011809499".mysql_error($dbi));
				list($part_of_teams)=mysql_fetch_row($result);
				if( $part_of_teams > 0 )
					Alert("You are part of $part_of_teams teams. Creating a team will remove you from any team you joined or pending to join");

				//register new team
				if( isset( $submited_data ) )
				{
					//check if username / passw is valid
					$query = "select id,team_name from tournament_team_settings where team_name='$team_name' and team_passw='$team_passw' and tournament_id='$s_user_tournament'";
//echo $query."<br>";
					$result=mysql_query($query,$dbi) or die("Q 2005011809492".mysql_error($dbi));
					list($team_id,$team_name)=mysql_fetch_row($result);

					if( $team_id )
					{
						//check if team is full
						$query = "select count(*) from tournament_players where tournament_id='$s_user_tournament' and is_pending_to_join_team=0 and is_pending_to_accept_team_invite=0 and team_id='$team_id'";
//echo $query."<br>";
						$result=mysql_query($query,$dbi) or die("Q 2005011809492".mysql_error($dbi));
						list($team_member_count)=mysql_fetch_row($result);
						if( $team_member_count >= $s_user_max_team_members )
							die("That team is already full");

						//clear other joins
						$result=mysql_query("delete from tournament_players where player_guid='$s_user_guid' and tournament_id='$s_user_tournament' and is_pending_to_join_team=0 and is_pending_to_accept_team_invite=0",$dbi) or die("Q 2005011809497".mysql_error($dbi));
						//join this team
						$result=mysql_query("Insert into tournament_players (team_id,player_guid,tournament_id,is_pending_to_join_team,is_pending_to_accept_team_invite,strength_points) values('$team_id','$s_user_guid','$s_user_tournament','0','0','".Get_player_strength($s_user_guid)."')",$dbi) or die("Q 2005011809498".mysql_error($dbi));
						Alert("Joined team : $team_name");
						jumpto("index.php");
					}
				}
			?>
			<form name="getinfoform" action="index.php?skin=quick_team_join.skin.php" method="post">
				<tr>
				  <td width="150" height="30" align="right" class="clsLoginText01">Team Name:&nbsp;</td>
				  <td width="240"><input type="text" name="team_name" style="width:180" class="clsTextField01" value="<?=$d_team_name;?>"></td>
				</tr>
				<tr>
				  <td width="150" height="30" align="right" class="clsLoginText01">Passw for quick join:&nbsp;</td>
				  <td width="240"><input type="text" name="team_passw" style="width:180" class="clsTextField01" value="<?=$d_team_passw;?>"></td>
				</tr>
				<tr>
				  <td height="30"><img src="images/blank.gif" width="1" height="1"></td>
				  <td><input type="submit" value="Quick Join" style="width:90px"></td>
				</tr>
				<input type="hidden" name="submited_data" value="1" >
			</form>
		</td>
	</tr>
</table>

