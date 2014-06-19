function vojin_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5881)
end

RegisterUnitEvent(10540, 1, "vojin_OnCombat")