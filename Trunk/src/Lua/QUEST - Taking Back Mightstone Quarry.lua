function Nerubar_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(11598, 0)
end

RegisterUnitEvent(25451, 4, "Nerubar_OnDied")
RegisterUnitEvent(25445, 4, "Nerubar_OnDied")
RegisterUnitEvent(25294, 4, "Nerubar_OnDied")
RegisterUnitEvent(24566, 4, "Nerubar_OnDied")
RegisterUnitEvent(24562, 4, "Nerubar_OnDied")