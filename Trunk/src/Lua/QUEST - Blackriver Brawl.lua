function AllianceUnit_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12170, 0)
end

RegisterUnitEvent(27117, 4, "AllianceUnit_OnDied")
RegisterUnitEvent(31414, 4, "AllianceUnit_OnDied")