function InfectedWildlife_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12100, 0)
end

RegisterUnitEvent(26616, 4, "InfectedWildlife_OnDied")
RegisterUnitEvent(26643, 4, "InfectedWildlife_OnDied")