function AllianceSoldier_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12317, 0)
end

function HordeSoldier_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12316, 0)
end
RegisterUnitEvent(27501, 4, "AllianceSoldier_OnDied")
RegisterUnitEvent(27549, 4, "AllianceSoldier_OnDied")
RegisterUnitEvent(27500, 4, "HordeSoldier_OnDied")
RegisterUnitEvent(27550, 4, "HordeSoldier_OnDied")