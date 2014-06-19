function ArchbishopBenedictus_OnAggro(pUnit, Event)
	pUnit:SendChatMessage(12, 0,"Sacrilege! Defend the cathedral!")	
	pUnit:PlaySoundToSet(5883)
	pUnit:CastSpell(2791)
end

RegisterUnitEvent(1284, 1, "ArchbishopBenedictus_OnAggro")