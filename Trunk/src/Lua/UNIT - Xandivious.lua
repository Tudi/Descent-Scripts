function Xandivious_OnDied(pUnit, Event)
	pUnit:SpawnGameObject(180671, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), pUnit:GetO(), 180000, 1)
end

RegisterUnitEvent(15623, 4, "Xandivious_OnDied")