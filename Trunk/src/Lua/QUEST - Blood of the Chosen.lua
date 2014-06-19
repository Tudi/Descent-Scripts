function YmirheimVrykul_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(13330, 0)
end

RegisterUnitEvent(31262, 4, "YmirheimVrykul_OnDied")
RegisterUnitEvent(31267, 4, "YmirheimVrykul_OnDied")
RegisterUnitEvent(31399, 4, "YmirheimVrykul_OnDied")
RegisterUnitEvent(31746, 4, "YmirheimVrykul_OnDied")