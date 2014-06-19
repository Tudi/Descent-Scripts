<br />
			<?php
				//direct acces of a skin is not allowed
				if( !$dbi )
					die("Can't acces page directly");

				//check if we have a team
				$result=mysql_query("select id from tournament_team_settings where team_leader_guid='$s_user_guid' and tournament_id='$s_user_tournament'",$dbi) or die("Q 200501180949".mysql_error($dbi));
				list($team_id)=mysql_fetch_row($result);
				if( $team_id=="" )
					die("You do not have a team yet");

				if( isset( $disband_team ) )
				{
					//kick all chars who joined this team
					$result=mysql_query("delete from tournament_players where team_id='$team_id'",$dbi) or die("Q 200501180949".mysql_error($dbi));
					//delete the team itself
					$result=mysql_query("delete from tournament_team_settings where id='$team_id'",$dbi) or die("Q 200501180949".mysql_error($dbi));
					Alert("Team disbanded");
					jumpto("index.php");
				}
			?>

			<center>
			<form name="getinfoform" action="index.php?skin=disband_team.skin.php" method="post">
 		  	 I want to kick all players who joined team and delete all team info:
			  <input type="checkbox" name="disband_team"><br />
			<input type="submit" value=" Disband Team " style="width:110px">
			</form>
			</center>
