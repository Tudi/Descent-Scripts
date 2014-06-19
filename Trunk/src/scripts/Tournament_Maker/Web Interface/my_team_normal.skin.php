<table border="0" align="center" cellpadding="0" cellspacing="0">
   <tr>
		<td>
			<?php
				//direct acces of a skin is not allowed
				if( !$dbi )
					die("Can't acces page directly");

				if( $leave_team )
				{
					$result=mysql_query("delete from tournament_players where player_guid='$s_user_guid' and tournament_id='$s_user_tournament' and is_pending_to_join_team=0 and is_pending_to_accept_team_invite=0",$dbi) or die("Q 2005011809497".mysql_error($dbi));
					Alert("You left your team");
					jumpto("index.php");
				}
				//check if we have a team
				$result=mysql_query("select team_id from tournament_players where is_pending_to_join_team='0' and is_pending_to_accept_team_invite='0' and player_guid='$s_user_guid' and tournament_id='$s_user_tournament'",$dbi) or die("Q 200501180949".mysql_error($dbi));
				list($my_team_id)=mysql_fetch_row($result);
				if( $team_id=="" )
					$team_id = $my_team_id;
				if( $team_id=="" )
					die("No team selected to view");
			?>
				<tr>
				  <td height="30" align="right" class="clsLoginText01">Character name</td>
				  <td height="30" align="right" class="clsLoginText01">Character strength</td>
				</tr>
			<?php
				$query = "select player_guid,strength_points from tournament_players where is_pending_to_join_team='0' and is_pending_to_accept_team_invite='0' and team_id='$team_id' and tournament_id='$s_user_tournament'";
				$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi));
				while(list($player_guid,$strength_points )=mysql_fetch_row($result) )
				{
					//get player name for guid
					$result1=mysql_query("select name from characters where guid='$player_guid'",$dbi) or die("Q 200501180949".mysql_error($dbi));
					list($name)=mysql_fetch_row($result1);
				?>
					<tr>
						<td height="30" align="right" class="clsLoginText01"><?=$name;?></td>
						<td height="30" align="right" class="clsLoginText01"><?=$strength_points;?></td>
					</tr>
				<?php
				}
			?>
		</td>
	</tr>
<?php 
if( $my_team_id )
{
	?>
<form name="getinfoform" action="index.php?skin=my_team.skin.php" method="post">
	<tr>
	  <td height="30"><img src="images/blank.gif" width="1" height="1"></td>
	  <td><input type="submit" value="Leave this team" style="width:140px"></td>
	</tr>
	<input type="hidden" name="leave_team" value="1" >
</form>
<?php } ?>
</table>