function LadyCobrahn_OnAggro(pUnit, Event)
	pUnit:SendChatMessage(12, 0,"You will never wake the dreamer!")
	pUnit:PlaySoundToSet(5785)
end

RegisterUnitEvent(3669, 1, "LadyCobrahn_OnAggro")