function SneedsShredder_OnDeath(pUnit, Event)
	pUnit:SpawnCreature(643, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), pUnit:GetO(), pUnit:GetFaction(), 0)
end

RegisterUnitEvent(642, 4, "SneedsShredder_OnDeath")