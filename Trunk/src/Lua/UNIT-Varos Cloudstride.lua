function VarosCloudstride_OnDied(pUnit, event, player)
	pUnit:SpawnCreature(28012,pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(),pUnit:GetO(),36,360000);
end

RegisterUnitEvent(27447, 4, "VarosCloudstride_OnDied")