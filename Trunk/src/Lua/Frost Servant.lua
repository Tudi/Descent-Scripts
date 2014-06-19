function Servant_OnCombat(Unit, Event)
	Unit:RegisterEvent("Servant_FrostShield", 14000, 0)
	Unit:RegisterEvent("Servant_FrostBolt", 4000, 0)
end

function Servant_FrostShield(Unit, Event)
	Unit:FullCastSpell(51763)
end

function Servant_FrostBolt(Unit, Event)
	Unit:FullCastSpell(101810)
end

function Servant_OnKilledTarget(Unit, Event)
end

function Servant_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Servant_OnDeath(Unit, Event)
end

RegisterUnitEvent(111022, 1, "Servant_OnCombat")
RegisterUnitEvent(111022, 2, "Servant_OnLeaveCombat")
RegisterUnitEvent(111022, 3, "Servant_OnKilledTarget")
RegisterUnitEvent(111022, 4, "Servant_Death")
