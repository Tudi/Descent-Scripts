function RottingCadaver_OnDied (pUnit, Event)
	pUnit:SpawnCreature (10925, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), pUnit:GetO(), pUnit:GetFaction(), 360000)
	pUnit:SpawnCreature (10925, pUnit:GetX()+0.3, pUnit:GetY(), pUnit:GetZ()+1, pUnit:GetO(), pUnit:GetFaction(), 360000)
end

RegisterUnitEvent (4474, 4, "RottingCadaver_OnDied")