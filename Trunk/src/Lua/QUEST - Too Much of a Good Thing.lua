function ProphetofSseratus_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12516, 0)
end
RegisterUnitEvent(28068, 4, "ProphetofSseratus_OnDied")