function Elder_OnCombat(Unit, Event)
	Unit:RegisterEvent("Elder_CorpseExplode", 2000, 0)
	Unit:RegisterEvent("Elder_ConservatorsGrip", 3000, 0)
end

function Elder_CorpseExplode(Unit, Event)
	Unit:FullCastSpell(59809)
	Unit:ClearThreatList()
end

function Elder_ConservatorsGrip(Unit, Event)
	Unit:FullCastSpell(62532)
end

function Elder_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
	Unit:Despawn(4000, 0)
end

function Elder_OnKilledTarget(Unit, Event)
end

function Elder_OnDeath(Unit, Event)
	Unit:RemoveEvents()
	Unit:Despawn(4000, 0)
end

RegisterUnitEvent(111010, 1, "Elder_OnCombat")
RegisterUnitEvent(111010, 2, "Elder_OnLeaveCombat")
RegisterUnitEvent(111010, 3, "Elder_OnKilledTarget")
RegisterUnitEvent(111010, 4, "Elder_OnDeath")