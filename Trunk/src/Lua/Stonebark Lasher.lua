function Lasher_OnCombat(Unit, Event)
	Unit:RegisterEvent("Lasher_Sunbeam",6000, 0)
end

function Lasher_Sunbeam(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nill) then
	Unit:FullCastSpellOnTarget(62623, rand)
	end
end

function Lasher_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
	Unit:Despawn(6000, 0)
end

function Lasher_OnKilledTarget(Unit, Event)
end

function Lasher_OnDeath(Unit, Event)
	Unit:RemoveEvents()
	Unit:Despawn(6000, 0)
end

RegisterUnitEvent(111008, 1, "Lasher_OnCombat")
RegisterUnitEvent(111008, 2, "Lasher_OnLeaveCombat")
RegisterUnitEvent(111008, 3, "Lasher_OnKilledTarget")
RegisterUnitEvent(111008, 4, "Lasher_OnDeath")