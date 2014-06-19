function Oozeling_OnLeaveCombat (pUnit, Event)
	pUnit:Despawn (1000, 0)
end

RegisterUnitEvent (8257, 2, "Oozeling_OnLeaveCombat") 