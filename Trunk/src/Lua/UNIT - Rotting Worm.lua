function Rotting_Worm_OnLeaveCombat (pUnit, Event)
	pUnit:Despawn (1000, 0)
end

RegisterUnitEvent (10925, 2, "Rotting_Worm_OnLeaveCombat")