function MistwhisperGorlocs_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12538, 0)
end

RegisterUnitEvent(28110, 4, "MistwhisperGorlocs_OnDied")