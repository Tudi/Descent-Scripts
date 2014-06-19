function NidavelirStormforged_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12978, 0)
end

RegisterUnitEvent(29370, 4, "NidavelirStormforged_OnDied")
RegisterUnitEvent(29374, 4, "NidavelirStormforged_OnDied")
RegisterUnitEvent(29380, 4, "NidavelirStormforged_OnDied")