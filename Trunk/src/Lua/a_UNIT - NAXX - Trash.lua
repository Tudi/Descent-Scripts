function DeathKnightCaptain_OnSpawn(pUnit, event)
	pUnit:CastSpell(55222)
end

function DeathKnight_OnSpawn(pUnit, event)
	pUnit:CastSpell(55212)
end

RegisterUnitEvent(16145, 18, "DeathKnightCaptain_OnSpawn")
RegisterUnitEvent(16146, 18, "DeathKnight_OnSpawn")