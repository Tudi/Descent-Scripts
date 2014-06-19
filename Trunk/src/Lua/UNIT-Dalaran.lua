function Horde_OnEnterCombat(pUnit, Event)
	pUnit:RegisterEvent("Horde_Knock",1000,0)
end

function Horde_Knock(pUnit, Event)
	pUnit:FullCastSpellOnTarget(54108,pUnit:GetMainTank())
end 

function Horde_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Horde_Died(pUnit, Event)
	pUnit:RemoveEvents()
end

function Horde_Died(pUnit, Event)
	pUnit:Root()
end

RegisterUnitEvent(29255, 1, "Horde_OnEnterCombat")
RegisterUnitEvent(29255, 2, "Horde_OnLeaveCombat")
RegisterUnitEvent(29255, 4, "Horde_OnDied")
RegisterUnitEvent(29255, 18, "Horde_Spawn")

function Ally_OnEnterCombat(pUnit, Event)
	pUnit:RegisterEvent("Ally_Knock",1000,0)
end

function Ally_Knock(pUnit, Event)
	pUnit:FullCastSpellOnTarget(54620,pUnit:GetMainTank())
end 

function Ally_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Ally_Died(pUnit, Event)
	pUnit:RemoveEvents()
end

function Ally_OnSpawn(pUnit, Event)
	pUnit:Root()
end

RegisterUnitEvent(29254, 1, "Ally_OnEnterCombat")
RegisterUnitEvent(29254, 2, "Ally_OnLeaveCombat")
RegisterUnitEvent(29254, 4, "Ally_OnDied")
RegisterUnitEvent(29254, 18, "Ally_OnSpawn")