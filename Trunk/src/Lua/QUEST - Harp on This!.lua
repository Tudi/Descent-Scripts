function ColdwindWasteHuntress_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12052, 1)
end

RegisterUnitEvent(26575, 4, "ColdwindWasteHuntress_OnDied")