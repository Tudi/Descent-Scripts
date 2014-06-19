--Magni Bronzebeard--
function magni_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5896)
end

RegisterUnitEvent(2784, 1, "magni_OnCombat")