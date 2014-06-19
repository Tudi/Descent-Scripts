function AllianceUnit_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12284, 0)
end

RegisterUnitEvent(27475, 4, "AllianceUnit_OnDied")
RegisterUnitEvent(27481, 4, "AllianceUnit_OnDied")
RegisterUnitEvent(27482, 4, "AllianceUnit_OnDied")