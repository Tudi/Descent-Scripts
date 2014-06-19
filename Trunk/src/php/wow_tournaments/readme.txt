from sql folder run the sqls in your "char" db (mysql database server). It should create the following tables :
	tournament_player_info -> not used atm
	tournament_rewards -> not used atm
	tournament_players -> filled by the tournament web interface. what player to what team is subscribed
	tournament_player_reports -> not used atm
	tournament_settings -> tournaments are created in this table
		tournament_name -> visible in login page dropdown
		min_teams_to_start -> not used atm
		max_chars_per_team -> max players allowed to join a team
		min_player_level_to_join -> min level of player in team, not sure it works :P
		max_player_level_to_join -> max level of player in team, not sure it works :P
		inscription_starts -> not used atm. start date when players will be able to create teams and join others
		inscription_ends -> end date when players will be able to create teams and join others
		tournament_starts -> NPC ingame can only start tournament if server time is passed this date
		teams_per_account -> should be limited to 1 
		cur_match_round -> in case of server crash used to recover state of tournament
	tournament_team_settings -> filled by the tournament web interface. teams that group players together
adjust sql configs in : ".\include\header.inc.php"
put content of the directory to your webserver that can interpret PHP file.
You need to have shortags enabled for PHP to make it work