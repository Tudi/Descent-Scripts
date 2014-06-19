function tyrande_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5885)
end

RegisterUnitEvent(7999, 1, "tyrande_OnCombat")