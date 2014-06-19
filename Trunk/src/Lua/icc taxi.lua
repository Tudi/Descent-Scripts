function On_Gossip(unit, event, player)
	unit:GossipCreateMenu(100, player, 0)
	unit:GossipMenuAddItem(0, "Teleport to Start", 1, 0)
	unit:GossipMenuAddItem(0, "Rampart of Skulls", 2, 0)
	unit:GossipMenuAddItem(0, "Deathbringer's Rise", 3, 0)
	unit:GossipMenuAddItem(0, "The Upper Reaches", 4, 0)
	unit:GossipSendMenu(player)
end

function Gossip_Submenus(unit, event, player, id, intid, code)
	if(intid == 1) then
		player:Teleport(631, -16.119860, 2211.639160, 30.1157)
	end
	if(intid == 2) then
		player:Teleport(631, -609.239136, 2210.9101, 200.072092)
	end
	if(intid == 3) then
		player:Teleport(631, -540.666138, 2211.729980, 539.391077)
	end
	if(intid == 4) then
		player:Teleport(631, 4346.56, 2769.516, 356.035)
	end
end


RegisterUnitGossipEvent(123459, 1, "On_Gossip")
RegisterUnitGossipEvent(123459, 2, "Gossip_Submenus")