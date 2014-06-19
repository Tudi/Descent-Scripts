function VentureCompanymembers_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12524, 0)
end

RegisterUnitEvent(28124, 4, "VentureCompanymembers_OnDied")