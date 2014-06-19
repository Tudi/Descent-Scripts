function Lasher_OnCombat(Unit, Event)
	Unit:FullCastSpell(51730)
	Unit:RegisterEvent("Lasher_ChainHeal", 2000, 0)
	Unit:RegisterEvent("Lasher_LifeGift", 8800, 0)
end

function Lasher_ChainHeal(Unit, Event)
	Unit:FullCastSpell(75370)
end

function Lasher_LifeGift(Unit, Event)
	Unit:FullCastSpell(64185)
end

function Lasher_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Lasher_OnKilledTarget(Unit, Event)
end

function Lasher_OnDeath(Unit, Event)
	Unit:RemoveEvents()
end

RegisterUnitEvent(111023, 1, "Lasher_OnCombat")
RegisterUnitEvent(111023, 2, "Lasher_OnLeaveCombat")
RegisterUnitEvent(111023, 3, "Lasher_OnKilledTarget")
RegisterUnitEvent(111023, 4, "Lasher_OnDeath")