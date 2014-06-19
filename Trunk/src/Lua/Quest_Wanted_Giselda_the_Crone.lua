function KilsorrowAgent_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(9935, 1)
	player:AdvanceQuestObjective(9936, 1)
end

RegisterUnitEvent(17146, 4, "KilsorrowAgent_OnDied")
RegisterUnitEvent(17147, 4, "KilsorrowAgent_OnDied")
RegisterUnitEvent(17148, 4, "KilsorrowAgent_OnDied")
RegisterUnitEvent(18397, 4, "KilsorrowAgent_OnDied")
RegisterUnitEvent(18658, 4, "KilsorrowAgent_OnDied")