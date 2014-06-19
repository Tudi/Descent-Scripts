function cairblood_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5884)
end

RegisterUnitEvent(3057, 1, "cairblood_OnCombat")