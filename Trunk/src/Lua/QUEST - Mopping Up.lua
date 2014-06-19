function FollowersofSseratus_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12508, 0)
end

RegisterUnitEvent(28034, 4, "FollowersofSseratus_OnDied")
RegisterUnitEvent(28035, 4, "FollowersofSseratus_OnDied")