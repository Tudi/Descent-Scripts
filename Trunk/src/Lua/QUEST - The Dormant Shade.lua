function TheDormantShade_OnComplete(pUnit, event)
	pUnit:SpawnCreature (1946, 2489.98, 21.37, 25.9, 3.0126, 16, 60000)
end


RegisterQuestEvent(410, 2, "TheDormantShade_OnComplete")