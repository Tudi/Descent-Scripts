function Gift_OnCombat(Unit, Event)
	Unit:RegisterEvent("Gift_LifebindersGift", 8000, 0)
end

function Gift_LifebindersGift(Unit, Event)
	Unit:FullCastSpell(64185)
	Unit:ClearThreatList()
end

function Gift_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
	Unit:Despawn(4000, 0)
end

function Gift_OnKilledTarget(Unit, Event)
end

function Gift_OnDeath(Unit, Event)
	Unit:RemoveEvents()
	Unit:Despawn(6000, 0)
end

function Gift_OnSpawn(Unit, Event)
	Unit:Root()
end

RegisterUnitEvent(111009, 1, "Gift_OnCombat")
RegisterUnitEvent(111009, 2, "Gift_OnLeaveCombat")
RegisterUnitEvent(111009, 3, "Gift_OnKilledTarget")
RegisterUnitEvent(111009, 4, "Gift_OnDeath")
RegisterUnitEvent(111009, 18, "Gift_OnSpawn")