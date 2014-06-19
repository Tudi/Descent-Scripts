function LordPythas_OnAggro(pUnit, Event)
	pUnit:SendChatMessage(12, 0,"The coils of death... Will crush you!")
	pUnit:PlaySoundToSet(5787)
end

RegisterUnitEvent(3670, 1, "LordPythas_OnAggro")