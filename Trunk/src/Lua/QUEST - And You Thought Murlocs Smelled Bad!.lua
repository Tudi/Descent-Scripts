function ChillmereCoastScourge_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(11397, 0)
	player:AdvanceQuestObjective(11394, 0)
end

RegisterUnitEvent(23643, 4, "ChillmereCoastScourge_OnDied")
RegisterUnitEvent(23644, 4, "ChillmereCoastScourge_OnDied")
RegisterUnitEvent(24485, 4, "ChillmereCoastScourge_OnDied")
RegisterUnitEvent(24540, 4, "ChillmereCoastScourge_OnDied")
RegisterUnitEvent(24546, 4, "ChillmereCoastScourge_OnDied")