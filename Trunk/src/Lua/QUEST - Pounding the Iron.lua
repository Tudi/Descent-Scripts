function IronDwarfDefenders_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12073, 0)
end
RegisterUnitEvent(26268, 4, "IronDwarfDefenders_OnDied")