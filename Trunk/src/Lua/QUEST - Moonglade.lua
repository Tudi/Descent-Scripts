function Moonglade_Quest_onAccept (pUnit, Event,pPlayer)
	pUnit:CastSpellOnTarget (19027, pPlayer)
end

RegisterQuestEvent (5921, 2, "Moonglade_Quest_onAccept")