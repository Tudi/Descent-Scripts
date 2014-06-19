function CelebrastheCursedDeath(pUnit, event)
	pUnit:SpawnCreature(13716, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), pUnit:GetO(), 35, 0)
end


RegisterUnitEvent(12225, 4, "CelebrastheCursedDeath")