function Portal_OnCombat(Unit, Event)
	Unit:RegisterEvent("Portal_SpawnMobs", 8000, 0)
	Unit:RegisterEvent("Portal_DespawnMob", 36000, 0)
end

function Portal_SpawnMobs(Unit, Event)
	x = Unit:GetX()
	y = Unit:GetY()
	z = Unit:GetZ()
	o = Unit:GetO()
	Unit:SpawnCreature(111020, x+2, y+2, z+1, o, 16, 600000)
end

function Portal_DespawnMob(Unit, Event)
	Unit:Despawn(1000, 0)
end

function Portal_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
	Unit:Despawn(4000, 0)
end

function Portal_OnKilledTarget(Unit, Event)
end

function Portal_OnDeath(Unit, Event)
	Unit:RemoveEvents()
	Unit:Despawn(2000, 0)
end

function Portal_OnSpawn(Unit, Event)
	Unit:Root()
end

RegisterUnitEvent(111021, 1, "Portal_OnCombat")
RegisterUnitEvent(111021, 2, "Portal_OnLeaveCombat")
RegisterUnitEvent(111021, 3, "Portal_OnKilledTarget")
RegisterUnitEvent(111021, 4, "Portal_OnDeath")
RegisterUnitEvent(111021, 18, "Portal_OnSpawn")