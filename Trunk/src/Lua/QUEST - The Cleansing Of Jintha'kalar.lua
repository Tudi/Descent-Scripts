function JinthakalarScourge_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12545, 0)
end

RegisterUnitEvent(26942, 4, "JinthakalarScourge_OnDied")
RegisterUnitEvent(26943, 4, "JinthakalarScourge_OnDied")
RegisterUnitEvent(26946, 4, "JinthakalarScourge_OnDied")
RegisterUnitEvent(26948, 4, "JinthakalarScourge_OnDied")
RegisterUnitEvent(26965, 4, "JinthakalarScourge_OnDied")