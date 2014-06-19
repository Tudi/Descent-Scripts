/*
Title: Darkmoon Faire
Author: Nexis
Series: Active
Location: Elwynn Forest
*/

-- Remove Gameobjects
delete from `gameobject_spawns` where `id` between '100000' and '100114';

-- Remove NPCs
delete from `creature_spawns` where `id` between '100000' and '100029';

-- NPC Text
delete from `npc_text` where `entry` between '60000' and '60050';

-- Waypoints
delete from `creature_waypoints` where `spawnid` between '100001' and '100028';

-- Formations
delete from `creature_formations` where (`spawn_id`='100022');
delete from `creature_formations` where (`spawn_id`='100026');