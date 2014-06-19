function Darrowshire_Spirit_Spawn (pUnit, Event)
	if (math.random (2) == 2) then
		pUnit:SpawnCreature(11064, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), pUnit:GetO(), 35, 300000)
	end
end

RegisterUnitEvent (8530, 4, "Darrowshire_Spirit_Spawn")
RegisterUnitEvent (8531, 4, "Darrowshite_Spirit_Spawn")
RegisterUnitEvent (8532, 4, "Darrowshire_Spirit_Spawn")
