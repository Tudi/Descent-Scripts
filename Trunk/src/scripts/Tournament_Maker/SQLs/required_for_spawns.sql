INSERT IGNORE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`) VALUES
(4, 'Tournament Supervisor', 'Need slaps ?', '', 0, 7, 0, 0, 0, 0, 12679, 0, 0, 0, 2, 1, NULL, 0),
(5, 'Tournament Spectator Teleporter', 'Only if there is an active one', '', 0, 7, 0, 1, 0, 0, 22602, 0, 0, 0, 8, 1, NULL, 0);

INSERT IGNORE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `spell1`, `spell2`, `spell3`, `spell4`, `spell_flags`, `modImmunities`) VALUES
(4, 96, 96, 35, 100000, 100000, 0, 0.25, 0, 1000, 0, 100, 200, 0, 0, 0, 360000000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 1.8, 0.5, '0', 0, 0, 0, 0, 2.5, 8, 14, 0, 0, 0, 0, 0, 0, 0),
(5, 96, 96, 35, 100000, 100000, 0, 1.00, 0, 1000, 0, 100, 200, 0, 0, 0, 360000000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 1.8, 0.5, '0', 0, 0, 0, 0, 2.5, 8, 14, 0, 0, 0, 0, 0, 0, 0);

