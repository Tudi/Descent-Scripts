# MySQL-Front Dump 2.5
#
# Host: localhost   Database: acemu_chars
# --------------------------------------------------------
# Server version 5.0.51b-community-nt


#
# Table structure for table 'tournament_player_info'
#

DROP TABLE IF EXISTS `tournament_player_info`;
CREATE TABLE IF NOT EXISTS `tournament_player_info` (
  `player_guid` int(10) unsigned NOT NULL DEFAULT '0' ,
  `matches_won` int(10) unsigned NOT NULL DEFAULT '0' ,
  `matches_played` int(10) unsigned NOT NULL DEFAULT '0' ,
  PRIMARY KEY (`player_guid`),
  UNIQUE KEY player_guid (`player_guid`),
   KEY player_guid_2 (`player_guid`)
);



#
# Table structure for table 'tournament_player_reports'
#

DROP TABLE IF EXISTS `tournament_player_reports`;
CREATE TABLE IF NOT EXISTS `tournament_player_reports` (
  `reporter_guid` int(10) unsigned NOT NULL DEFAULT '0' ,
  `reported_guid` int(10) unsigned NOT NULL DEFAULT '0' ,
  `abuse_count` int(10) unsigned NOT NULL DEFAULT '0' ,
  PRIMARY KEY (`reporter_guid`),
  UNIQUE KEY reporter_guid (`reporter_guid`),
   KEY reporter_guid_2 (`reporter_guid`)
);



#
# Table structure for table 'tournament_players'
#

DROP TABLE IF EXISTS `tournament_players`;
CREATE TABLE IF NOT EXISTS `tournament_players` (
  `team_id` int(10) unsigned NOT NULL DEFAULT '0' ,
  `player_guid` int(10) unsigned NOT NULL DEFAULT '0' ,
  `tournament_id` int(10) unsigned NOT NULL DEFAULT '0' ,
  `is_pending_to_join_team` tinyint(1) unsigned NOT NULL DEFAULT '1' ,
  `is_pending_to_accept_team_invite` tinyint(1) unsigned NOT NULL DEFAULT '1' ,
  `strength_points` int(10) unsigned NOT NULL DEFAULT '0' ,
   KEY id (`team_id`,`player_guid`)
);



#
# Table structure for table 'tournament_rewards'
#

DROP TABLE IF EXISTS `tournament_rewards`;
CREATE TABLE IF NOT EXISTS `tournament_rewards` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `tournament_id` int(10) unsigned NOT NULL DEFAULT '0' ,
  `team_ranking` int(10) unsigned NOT NULL DEFAULT '0' ,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0' ,
  `item_count` int(10) NOT NULL DEFAULT '0' ,
  PRIMARY KEY (`id`),
  UNIQUE KEY id (`id`)
);


#
# Table structure for table 'tournament_settings'
#

DROP TABLE IF EXISTS `tournament_settings`;
CREATE TABLE IF NOT EXISTS `tournament_settings` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `tournament_name` varchar(500) ,
  `min_teams_to_start` int(10) unsigned NOT NULL DEFAULT '2' ,
  `max_chars_per_team` int(10) unsigned NOT NULL DEFAULT '1' ,
  `min_player_level_to_join` int(10) unsigned NOT NULL DEFAULT '1' ,
  `max_player_level_to_join` int(10) unsigned NOT NULL DEFAULT '255' ,
  `inscription_starts` int(10) NOT NULL DEFAULT '0' ,
  `inscription_ends` int(10) unsigned NOT NULL DEFAULT '1262307661' ,
  `tournament_starts` int(10) NOT NULL DEFAULT '1262307661' ,
  `teams_per_account` int(10) unsigned NOT NULL DEFAULT '1' ,
  `cur_match_round` int(10) unsigned NOT NULL DEFAULT '0' ,
  `max_teams` int(10) unsigned NOT NULL DEFAULT '1000000' ,
  PRIMARY KEY (`id`),
  UNIQUE KEY id (`id`),
   KEY id_2 (`id`)
);



#
# Table structure for table 'tournament_team_settings'
#

DROP TABLE IF EXISTS `tournament_team_settings`;
CREATE TABLE IF NOT EXISTS `tournament_team_settings` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `tournament_id` int(10) unsigned NOT NULL DEFAULT '0' ,
  `team_leader_guid` int(10) unsigned NOT NULL DEFAULT '0' ,
  `team_name` varchar(500) ,
  `leader_acc` int(10) unsigned NOT NULL DEFAULT '0' ,
  `is_open_team` tinyint(1) unsigned NOT NULL DEFAULT '1' ,
  `team_desc_public` varchar(500) ,
  `team_desc_private` varchar(500) ,
  `strength_ordering` int(10) unsigned NOT NULL DEFAULT '0' ,
  `matches_won` int(10) unsigned NOT NULL DEFAULT '0' ,
  `matches_won_total` int(10) unsigned NOT NULL DEFAULT '0' ,
  `matches_finished` int(10) unsigned NOT NULL DEFAULT '0' ,
  `team_passw` varchar(500) ,
  PRIMARY KEY (`tournament_id`,`team_leader_guid`,`id`),
  UNIQUE KEY player_guid (`id`)
);

