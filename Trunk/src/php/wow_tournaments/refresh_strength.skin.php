<table width="970"  border="0" align="center" cellpadding="0" cellspacing="0">
   <tr>
		<td>
			<?php
				//direct acces of a skin is not allowed
				if( !$dbi )
					die("Can't acces page directly");

				$result=mysql_query("update tournament_players set strength_points='".Get_player_strength($s_user_guid)."' where player_guid='$s_user_guid' and tournament_id='$s_user_tournament'",$dbi) or die("Q 200501180949".mysql_error($dbi));

				die("Player Strength factor recalculated and updated");
			?>
		</td>
	</tr>
</table>

