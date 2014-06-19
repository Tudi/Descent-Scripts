function VileSlime_OnDied (pUnit, Event)
	pUnit:SpawnCreature (8257, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), pUnit:GetO(), pUnit:GetFaction(), 360000)
	pUnit:SpawnCreature (8257, pUnit:GetX()-1, pUnit:GetY(), pUnit:GetZ()+1, pUnit:GetO(), pUnit:GetFaction(), 360000)
	pUnit:SpawnCreature (8257, pUnit:GetX()+1, pUnit:GetY(), pUnit:GetZ()+1, pUnit:GetO(), pUnit:GetFaction(), 360000)
end

RegisterUnitEvent (1806, 4, "VileSlime_OnDied")