function Shadow_OnCombat(Unit, Event)
	Unit:RegisterEvent("Shadow_ShadowNova2", 6000, 0)
end

function Shadow_ShadowNova2(Unit, Event)
	Unit:FullCastSpell(65209)
	Unit:ClearThreatList()
end

function Shadow_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
	
	Unit:Despawn(6000, 0)
end

function Shadow_OnKilledTarget(Unit, Event)
end

function Shadow_OnDeath(Unit, Event)
	Unit:RemoveEvents()
	Unit:Despawn(4000, 0)
end

RegisterUnitEvent(111015, 1, "Shadow_OnCombat")
RegisterUnitEvent(111015, 2, "Shadow_OnLeaveCombat")
RegisterUnitEvent(111015, 3, "Shadow_OnKilledTarget")
RegisterUnitEvent(111015, 4, "Shadow_OnDeath")