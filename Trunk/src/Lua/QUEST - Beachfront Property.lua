function ForgottenGhost_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12304, 0)
end

RegisterUnitEvent(27224, 4, "ForgottenGhost_OnDied")
RegisterUnitEvent(27225, 4, "ForgottenGhost_OnDied")
RegisterUnitEvent(27226, 4, "ForgottenGhost_OnDied")
RegisterUnitEvent(27229, 4, "ForgottenGhost_OnDied")