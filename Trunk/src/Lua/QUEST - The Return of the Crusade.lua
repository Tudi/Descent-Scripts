function ScarletOnslaught_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12476, 0)
end

RegisterUnitEvent(32417, 4, "ScarletOnslaught_OnDied")
RegisterUnitEvent(27334, 4, "ScarletOnslaught_OnDied")
RegisterUnitEvent(27333, 4, "ScarletOnslaught_OnDied")
RegisterUnitEvent(27332, 4, "ScarletOnslaught_OnDied")
RegisterUnitEvent(27330, 4, "ScarletOnslaught_OnDied")