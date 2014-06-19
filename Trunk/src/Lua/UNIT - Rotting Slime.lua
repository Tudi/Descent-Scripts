function RottingSlime_Died(pUnit, event, miscUnit, Misc)
	pUnit:SpawnGameObject(19021, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), pUnit:GetO(), 60000)
end

RegisterUnitEvent(3928, 4, "RottingSlime_Died")