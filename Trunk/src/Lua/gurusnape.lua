function On_Gossip(unit, event, player)
	unit:GossipCreateMenu(100, player, 0)
	unit:GossipMenuAddItem(0, "How do you contact a game master?", 1, 0)
	unit:GossipMenuAddItem(0, "I am dead and can\'t find my corpse!", 2, 0)
	unit:GossipMenuAddItem(0, "I am dead and can\'t revive even after using the unstuck button!", 3, 0)
	unit:GossipMenuAddItem(0, "What are the rules regarding cursing in this realm?", 4, 0)
	unit:GossipMenuAddItem(0, "Someone hacked my account and I didn\'t tell them my password.", 5, 0)
	unit:GossipMenuAddItem(0, "I have lost an item/gold due to server disconentions and/or rollbacks.", 6, 0)
	unit:GossipMenuAddItem(0, "How do I donate to Thunderstorm realm? What do I get?", 7, 0)
	unit:GossipMenuAddItem(0, "I am unable to talk what I say does not appear on the screen!", 8, 0)
	unit:GossipMenuAddItem(0, "How do I have a player form another realm transfered into this one?", 9, 0)
	unit:GossipMenuAddItem(0, "How do I apply to become a Game Master?", 10, 0)
	unit:GossipMenuAddItem(0, "A player is harrasing me how do I report him?", 11, 0)
	unit:GossipMenuAddItem(0, "What is a hack?", 12, 0)
	unit:GossipMenuAddItem(0, "What is an exploit?", 13, 0)
	unit:GossipMenuAddItem(0, "My game suddenly froze and I was disconnected form the server!", 14, 0)
	unit:GossipMenuAddItem(0, "What is a rollback?", 15, 0)
	unit:GossipMenuAddItem(0, "How can I help reduce rollbacks and server disconnections?", 16, 0)
	unit:GossipSendMenu(player)
end

function Gossip_Submenus(unit, event, player, id, intid, code)
	if(intid == 0) then
		unit:GossipCreateMenu(100, player, 0)
		unit:GossipMenuAddItem(0, "How do you contact a game master?", 1, 0)
		unit:GossipMenuAddItem(0, "I am dead and can\'t find my corpse!", 2, 0)
		unit:GossipMenuAddItem(0, "I am dead and can\'t revive even after using the unstuck button!", 3, 0)
		unit:GossipMenuAddItem(0, "What are the rules regarding cursing in this realm?", 4, 0)
		unit:GossipMenuAddItem(0, "Someone hacked my account and I didn\'t tell them my password.", 5, 0)
		unit:GossipMenuAddItem(0, "I have lost an item/gold due to server disconentions and/or rollbacks.", 6, 0)
		unit:GossipMenuAddItem(0, "How do I donate to Thunderstorm realm? What do I get?", 7, 0)
		unit:GossipMenuAddItem(0, "I am unable to talk what I say does not appear on the screen!", 8, 0)
		unit:GossipMenuAddItem(0, "How do I have a player form another realm transfered into this one?", 9, 0)
		unit:GossipMenuAddItem(0, "How do I apply to become a Game Master?", 10, 0)
		unit:GossipMenuAddItem(0, "A player is harrasing me how do I report him?", 11, 0)
		unit:GossipMenuAddItem(0, "What is a hack?", 12, 0)
		unit:GossipMenuAddItem(0, "What is an exploit?", 13, 0)
		unit:GossipMenuAddItem(0, "My game suddenly froze and I was disconnected form the server!", 14, 0)
		unit:GossipMenuAddItem(0, "What is a rollback?", 15, 0)
		unit:GossipMenuAddItem(0, "How can I help reduce rollbacks and server disconnections?", 16, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 1) then
		unit:GossipCreateMenu(101, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 2) then
		unit:GossipCreateMenu(102, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 3) then
		unit:GossipCreateMenu(103, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 4) then
		unit:GossipCreateMenu(105, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 5) then
		unit:GossipCreateMenu(107, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 6) then
		unit:GossipCreateMenu(108, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 7) then
		unit:GossipCreateMenu(109, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 8) then
		unit:GossipCreateMenu(110, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 9) then
		unit:GossipCreateMenu(111, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 10) then
		unit:GossipCreateMenu(112, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 11) then
		unit:GossipCreateMenu(113, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 12) then
		unit:GossipCreateMenu(115, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 13) then
		unit:GossipCreateMenu(116, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 14) then
		unit:GossipCreateMenu(117, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 15) then
		unit:GossipCreateMenu(118, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
	if(intid == 16) then
		unit:GossipCreateMenu(119, player, 1)
		unit:GossipMenuAddItem(0, "Return to main menu", 0, 0)
		unit:GossipSendMenu(player)
	end
end


RegisterUnitGossipEvent(18, 1, "On_Gossip")
RegisterUnitGossipEvent(18, 2, "Gossip_Submenus")