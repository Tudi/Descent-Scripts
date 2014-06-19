--[[ 
SQL UPDATE
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `civilian`) VALUES (143001, 'Icecrown Teleport', 'Your best friend', '', 0, 7, 0, 0, 0, 0, 26239, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `civilian`) VALUES (143000, 'Icecrown Teleport', 'Your best friend', '', 0, 7, 0, 0, 0, 0, 26239, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `modImmunities`) VALUES (143000, 137, 137, 35, 1, 1, 0, 1, 1, 2000, 0, 1, 1, 0, 0, 0, 6000, 0, 0, 0, 0, 0, 0, 0, 1, 1, '0', 0, 0, 0, 0, 5, 7, 21, 0, 0);
REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `modImmunities`) VALUES (143001, 137, 137, 35, 1, 1, 0, 1, 1, 2000, 0, 1, 1, 0, 0, 0, 6000, 0, 0, 0, 0, 0, 0, 0, 1, 1, '0', 0, 0, 0, 0, 5, 7, 21, 0, 0);
UPDATE `creature_proto` SET npcflags = 1 where entry in (143000,143001);
]]


function ICCOn_Gossip(unit, event, player)
	unit:GossipCreateMenu(102, player, 0)
	unit:GossipMenuAddItem(0, "Teleport to Start", 1, 0)
	unit:GossipMenuAddItem(0, "Deathbringer's Rise", 2, 0)
	unit:GossipSendMenu(player)
end

function ICCGossip_Submenus(unit, event, player, id, intid, code)
	if(intid == 1) then
		player:Teleport(631, -16.119860, 2211.639160, 30.1157)
	elseif(intid == 2) then
		player:Teleport(631, -540.666138, 2211.729980, 539.391077)
	end
end


RegisterUnitGossipEvent(143000, 1, "ICCOn_Gossip")
RegisterUnitGossipEvent(143000, 2, "ICCGossip_Submenus")

function ICCOn2_Gossip(unit, event, player)
	unit:GossipCreateMenu(103, player, 0)
	unit:GossipMenuAddItem(0, "Teleport to Start", 1, 0)
	unit:GossipMenuAddItem(0, "Deathbringer's Rise", 2, 0)
	unit:GossipMenuAddItem(0, "The Upper Reaches", 3, 0)
	unit:GossipMenuAddItem(0, "The Frozen Throne", 4, 0)
	unit:GossipSendMenu(player)
end

function ICCGossip2_Submenus(unit, event, player, id, intid, code)
	if(intid == 1) then
		player:Teleport(631, -16.119860, 2211.639160, 30.1157)
	elseif(intid == 2) then
		player:Teleport(631, -540.666138, 2211.729980, 539.391077)
	elseif(intid == 3) then
		player:Teleport(631, 4346.56, 2769.516, 356.035)
	elseif(intid == 4) then
		player:Teleport(631, 485, -2123, 956)
	end
end


RegisterUnitGossipEvent(143001, 1, "ICCOn2_Gossip")
RegisterUnitGossipEvent(143001, 2, "ICCGossip2_Submenus")