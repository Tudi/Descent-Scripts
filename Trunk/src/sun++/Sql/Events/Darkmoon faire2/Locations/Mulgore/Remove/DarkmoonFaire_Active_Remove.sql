/*
Title: Darkmoon Faire
Author: Nexis
Series: Active
Location: Mulgore
*/

-- Remove Gameobjects
delete from `gameobject_spawns` where `id` between '100000' and '100129';

-- Remove NPCs
delete from `creature_spawns` where `id` between '500000' and '500027';

-- NPC Text
delete from `npc_text` where `entry` between '60000' and '60050';

-- Waypoints
delete from `creature_waypoints` where (`spawnid`='500001');
delete from `creature_waypoints` where (`spawnid`='500003');
delete from `creature_waypoints` where (`spawnid`='500005');
delete from `creature_waypoints` where (`spawnid`='500009');
delete from `creature_waypoints` where (`spawnid`='500010');
delete from `creature_waypoints` where (`spawnid`='500014');
delete from `creature_waypoints` where (`spawnid`='500015');
delete from `creature_waypoints` where (`spawnid`='500016');
delete from `creature_waypoints` where (`spawnid`='500013');
delete from `creature_waypoints` where (`spawnid`='500004');
delete from `creature_waypoints` where (`spawnid`='500007');

-- Formations
delete from `creature_formations` where (`spawn_id`='500002');
delete from `creature_formations` where (`spawn_id`='500006');