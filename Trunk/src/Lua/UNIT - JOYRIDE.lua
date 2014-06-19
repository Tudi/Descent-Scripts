--[[
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(250000, 'Pinky\'s Playground', 'Teleporter', '', 0, 0, 7, 0, 0, 0, 0, 574, 0, 0, 0, 1.05, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(250000, 81, 81, 35, 1337, 1337, 1000, 1.5, 1, 1500, 0, 1860, 2418, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.5, 1, '', 0, 0, 0, 0, 2.5, 10.2, 14, 0, 1, 0, 220008, 0, 0, 0, 0, 0);
--]]


function FunTeleOn2_Gossip(unit, event, player)
	unit:GossipCreateMenu(2700000, player, 0)
	unit:GossipMenuAddItem(0, "Take me to Pinky's Playground!", 1, 0)
	unit:GossipMenuAddItem(0, "Take me to WoWBeeZ Village!", 2, 0)
	unit:GossipSendMenu(player)
end

function FunTeleGossip2_Submenus(unit, event, player, id, intid, code)
	if(intid == 1) then
		player:Teleport(169, 2226.928955, -4055.830322, 133.270096)
	elseif(intid == 2) then
--		player:Teleport(169, -4756.417969, 3099.105225, 92.008087)
		player:Teleport(169, -3887.584229, -4162.635742, 92.052072)
	end
end


RegisterUnitGossipEvent(250000, 1, "FunTeleOn2_Gossip")
RegisterUnitGossipEvent(250000, 2, "FunTeleGossip2_Submenus")


--[[
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(250001, 'Playground Teleporter', 'It\'s magic', '', 0, 0, 7, 0, 0, 0, 0, 28213, 0, 0, 0, 1.05, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(250001, 85, 85, 35, 1337, 1337, 1000, 1.5, 1, 1500, 0, 1860, 2418, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.5, 1, '', 0, 0, 0, 0, 2.5, 10.2, 14, 0, 1, 0, 220008, 0, 0, 0, 0, 0);
--]]


function FunTeleOn1_Gossip(unit, event, player)
	unit:GossipCreateMenu(2700000, player, 0)
	unit:GossipMenuAddItem(0, "Teleport me to start. (Staircase 1)", 1, 0)
	unit:GossipMenuAddItem(0, "Staircase 2", 2, 0)
	unit:GossipMenuAddItem(0, "PvP Arena.", 3, 0)
	unit:GossipMenuAddItem(0, "Chillspot.", 4, 0)
	unit:GossipMenuAddItem(0, "Maze.", 5 , 0)
	unit:GossipMenuAddItem(0, "Maze 2.", 6, 0)
	--unit:GossipMenuAddItem(0, "Teleport me to The Event Palace", 7, 0)
	unit:GossipSendMenu(player)
end

function FunTeleGossip1_Submenus(unit, event, player, id, intid, code)
	if(intid == 1) then
		player:Teleport(169, 2226.928955, -4055.830322, 133.270096)
	elseif(intid == 2) then
		player:Teleport(169, 2379.279785, -2726.233887, 20.747299)
	elseif(intid == 3) then
		player:Teleport(169, -3360.785156, -1835.280273, 92.532227)
	elseif(intid == 4) then
		player:Teleport(169, -4490.790039, -619.693176, 125.208267)
	elseif(intid == 5) then
		player:Teleport(169, 1045, 4156, 98)
	elseif(intid == 6) then
		player:Teleport(169, -3928.6958, 2137.2111, 93.2405)
	elseif(intid == 7) then
		player:Teleport(169, -4756.417969, 3099.105225, 92.008087)

	end
end


RegisterUnitGossipEvent(250001, 1, "FunTeleOn1_Gossip")
RegisterUnitGossipEvent(250001, 2, "FunTeleGossip1_Submenus")
