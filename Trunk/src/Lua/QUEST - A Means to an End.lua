function LevineFamilyTermiteBunny_OnSpawn(Unit, Event, Player, misc)
	Unit:SpawnCreature(27238, Unit:GetX(), Unit:GetY(), Unit:GetZ(), Unit:GetO(), 14, 180000)
	Unit:Despawn(100,0)
end
RegisterUnitEvent(27353, 18, "LevineFamilyTermiteBunny_OnSpawn")