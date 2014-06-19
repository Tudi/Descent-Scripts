function Acolyte_OnCombat(Unit, Event)
	Unit:RegisterEvent("Acolyte_VoidBolt", 18800, 0)
	Unit:RegisterEvent("Acolyte_ShadowVolley", 8000, 0)
end

function Acolyte_VoidBolt(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(104849, rand)
	end
end

function Acolyte_ShadowVolley(Unit, Event)
	Unit:FullCastSpell(108172)
end

function Acolyte_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Acolyte_OnDeath(Unit, Event)
	Unit:RemoveEvents()
end

RegisterUnitEvent(111041, 1, "Acolyte_OnCombat")
RegisterUnitEvent(111041, 2, "Acolyte_OnLeaveCombat")
RegisterUnitEvent(111041, 4, "Acolyte_OnDeath")