function StormforgedAttacker_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12931, 0)
end

RegisterUnitEvent(29382, 4, "StormforgedAttacker_OnDied")