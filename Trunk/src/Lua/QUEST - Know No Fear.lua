function Garm_Watcher_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12822, 0)
end

function Snowblind_Devotee_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12822, 1)
end

RegisterUnitEvent(29409, 4, "Garm_Watcher_OnDied")
RegisterUnitEvent(29407, 4, "Snowblind_Devotee_OnDied")