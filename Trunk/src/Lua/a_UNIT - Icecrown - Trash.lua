function DeathspeakerHighPriest_OnSpawn(pUnit, event)
	pUnit:CastSpell(69491)
end

RegisterUnitEvent(36829, 18, "DeathspeakerHighPriest_OnSpawn")

function TheDamned_OnDeath(pUnit, event, player, Misc)
	pUnit:CastSpell(70961)
	player:AdvanceQuestObjective(24547, 0)
end

RegisterUnitEvent(37011, 4, "TheDamned_OnDeath")