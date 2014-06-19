function Spore_PotentPheromones(Unit, Event)
	Unit:FullCastSpell(62541)
	Unit:ClearThreatList()
end

function Spore_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
	Unit:Despawn(4000, 0)
end

function Spore_OnDeath(Unit, Event)
	Unit:RemoveEvents()
end

function Spore_OnSpawn(Unit, Event)
	Unit:Root()
	Unit:RegisterEvent("Spore_PotentPheromones", 1000, 0)
	Unit:Despawn(14000, 0)
end

RegisterUnitEvent(111016, 2, "Spore_OnLeaveCombat")
RegisterUnitEvent(111016, 4, "Spore_OnDeath")
RegisterUnitEvent(111016, 18, "Spore_OnSpawn")