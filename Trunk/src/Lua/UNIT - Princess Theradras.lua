function PrincessTheradras_OnDied(pUnit, event, player)
	pUnit:SpawnCreature(12238,pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(),pUnit:GetO(),36,360000);
end

RegisterUnitEvent(12201, 4, "PrincessTheradras_OnDied")