function LordSerpentis_OnAggro(pUnit, Event)
	pUnit:SendChatMessage(12, 0,"I am the serpent king, i can do anything!")
	pUnit:PlaySoundToSet(5788)
end

RegisterUnitEvent(3673, 1, "LordSerpentis_OnAggro")