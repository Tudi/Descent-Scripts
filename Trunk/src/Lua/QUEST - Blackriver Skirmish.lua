function HordeUnit_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12170, 0)
end

RegisterUnitEvent(31435, 4, "HordeUnit_OnDied")
RegisterUnitEvent(27118, 4, "HordeUnit_OnDied")