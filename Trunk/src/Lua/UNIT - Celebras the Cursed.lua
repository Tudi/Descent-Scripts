function CelebrastheCursed_OnDied(pUnit, event, player)
	pUnit:SpawnCreature(13716,pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(),pUnit:GetO(),36,360000);
end

RegisterUnitEvent(12225, 4, "CelebrastheCursed_OnDied")