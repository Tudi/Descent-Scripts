function SnowVisual_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function SnowVisual_OnKilledTarget(Unit, Event)
end

function SnowVisual_OnDeath(Unit, Event)
	Unit:RemoveEvents()
end

function SnowVisual_OnSpawn(Unit, Event)
	
	Unit:FullCastSpell(54717)
end

RegisterUnitEvent(111061, 2, "SnowVisual_OnLeaveCombat")
RegisterUnitEvent(111061, 3, "SnowVisual_OnKilledTarget")
RegisterUnitEvent(111061, 4, "SnowVisual_OnDeath")
RegisterUnitEvent(111061, 18, "SnowVisual_OnSpawn")