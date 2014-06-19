function GameAnimals_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12592, 0)
end

RegisterUnitEvent(28001, 4, "GameAnimals_OnDied")
RegisterUnitEvent(28002, 4, "GameAnimals_OnDied")
RegisterUnitEvent(28009, 4, "GameAnimals_OnDied")
RegisterUnitEvent(28011, 4, "GameAnimals_OnDied")
RegisterUnitEvent(28129, 4, "GameAnimals_OnDied")
RegisterUnitEvent(28297, 4, "GameAnimals_OnDied")
RegisterUnitEvent(28378, 4, "GameAnimals_OnDied")
RegisterUnitEvent(28379, 4, "GameAnimals_OnDied")
RegisterUnitEvent(28380, 4, "GameAnimals_OnDied")