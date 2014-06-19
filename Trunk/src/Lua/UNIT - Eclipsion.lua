function EclipsionSpawn(pUnit, Event)
	pUnit:SpawnCreature(21627, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), pUnit:GetO(), pUnit:GetFaction(), 0)
end

function EclipsionDespawn(pUnit, Event)
	pUnit:Despawn(1000, 0);
end

RegisterUnitEvent(19806, 1, "EclipsionSpawn");
RegisterUnitEvent(22018, 1, "EclipsionSpawn");
RegisterUnitEvent(21627, 2, "EclipsionDespawn");