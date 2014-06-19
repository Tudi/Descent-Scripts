function ladyswindr_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5886)
end

RegisterUnitEvent(10181, 1, "ladyswindr_OnCombat")