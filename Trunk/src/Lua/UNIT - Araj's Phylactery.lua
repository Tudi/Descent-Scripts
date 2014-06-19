function Arajs_Phylactery (pUnit, Event)
	pUnit:SpawnGameObject(177241, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), pUnit:GetO(), 360000)
end

RegisterUnitEvent(1852, 4, "Arajs_Phylactery")