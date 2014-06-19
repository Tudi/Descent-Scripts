function velen_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(10154)
end

RegisterUnitEvent(17468, 1, "velen_OnCombat")