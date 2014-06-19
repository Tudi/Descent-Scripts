function LadyAnacondra_OnAggro(pUnit, Event)
	pUnit:SendChatMessage(12, 0,"None can stand against the Serpent Lords!")
	pUnit:PlaySoundToSet(5786)
end

RegisterUnitEvent(3671, 1, "LadyAnacondra_OnAggro")