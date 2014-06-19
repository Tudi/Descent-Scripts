	<?php
	if( $dbi )
	{
		$query = "select inscription_ends,tournament_starts from tournament_settings where id='$s_user_tournament'";
		$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi));
		list($inscription_ends,$tournament_starts_stamp )=mysql_fetch_row($result);
		$tournament_starts = date("Y-m-d H:i",$tournament_starts_stamp);
		$server_time_now = date("Y-m-d H:i",time());
		$remaining_time_stamp = $tournament_starts_stamp - time();
		if($remaining_time_stamp < 0 )
			$remaining_time_stamp = 0;
		$remaining_days = (int)($remaining_time_stamp / (60*60*24));
		$remaining_hours = (int)($remaining_time_stamp % (60*60*24) / (60*60));
		$remaining_minutes = (int)($remaining_time_stamp % (60*6) / (60));
	}
	?>
	<table>
		<tr>
			<td> <b>Server time now : </b></td> <td> <?=$server_time_now;?> </td> 
		</tr>
		<tr>
			<td> <b>Current tournament starts at : </b></td> <td><?=$tournament_starts;?> </td> 
		</tr>
		<tr>
			<td> <b>Time remaining for team subscriptions: </b></td> <td><?=$remaining_days;?> <?=$remaining_hours;?>:<?=$remaining_minutes;?></td> 
		</tr>
	</table>
   &nbsp;<br />

   <b>	How to create a team :	</b><br />
   	- push the "Create/Edit Team" link <br />
   	- Team name will be used ingame and in any calification list <br />
   	- Team info is visible for players who are searching a group to join ( brag about something )<br />
   	- Team private info is visible for players who joined the group ( chat or contact info ? )<br />
   	- Open team means it will be listed as a "looking for more" team<br />
<!--   	- you may only create a team while tournament subscription is open<br /> -->
   &nbsp;<br />

   <b>	How to join a team :	</b><br />
   	- push the "Join Team" link <br />
   	- from the list of teams that are looking for more players select the ones you wish to join by pressing "Pending Join" <br />
   	- wait some time until at least one team will accept your pending subscription <br />
   	- on same list (Join Team) push the "Join Team" to fully join that team<br />
   &nbsp;<br />

   <b>	How to accept new team members:	</b><br />
   	- push the "Accept Pending Joins" link <br />
   	- from the list of players that are pending to join your team, use "Accept member" to allow them to fully join the team  <br />
   	- if you change your mind then you may revoke the invitation with "Kick member" <br />
   	- player will need to accept team invitation to fully join the team <br />
   &nbsp;<br />

   <b>	Rulez : </b><br />
   1) You can only have/join one team per tournament per char <br />
   &nbsp;<br />

   <b>	Infos :	</b><br />
    - if team max member count is 1 then You will not be able to see team managment icons <br />
    - it takes 3 steps to join a team : player subscription -> team accept -> player full join <br />
    - you can join a team while enter a pending state for other teams. Maybe you get lucky to enter a better team<br />
    - you can subscribe to as many teams as you wish, but participate in only one<br />
    - Even if you got accepted to a team, there is a chance it got full and you will not be able to join until someone leaves<br />
    - you can't have a team and join another one. You can be in only 1 team<br />
    - you can edit your own teams by kicking players<br />
    - teams are paired based on strength points in fisrt match. Low level players get paired with other low leveled ones<br />
    - you may report players only once if they behave unaproriate<br />
    - GMs may disqualify you anytime<br />
    - You get ported to event place when it starts. Make sure to be ingame though<br />
    - You can spectate tournament even if you are not part of it. Talk to a tournament spectator NPC. There is a limited number of seats<br />
   &nbsp;<br />


