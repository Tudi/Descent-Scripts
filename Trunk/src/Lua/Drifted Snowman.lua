function Snow_OnCombat(Unit, Event)
	Unit:FullCastSpell(62650)
	Unit:RegisterEvent("Snow_FrostAura", 500, 1)
	Unit:RegisterEvent("Snow_Frostshock", 8000, 0)
end

function Snow_FrostAura(Unit, Event)
	Unit:FullCastSpell(34202)
end

function Snow_Frostshock(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(102266, rand)
	end
end

function Snow_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
	Unit:RemoveAllAuras()
end

function Snow_OnKilledTarget(Unit, Event)
end

function Snow_OnDeath(Unit, Event)
	Unit:RemoveEvents()
	Unit:Despawn(6000, 0)
	Unit:RemoveAllAuras()
end

function Snow_OnSpawn(Unit, Event)
	Unit:Root()
end

RegisterUnitEvent(111058, 1, "Snow_OnCombat")
RegisterUnitEvent(111058, 2, "Snow_OnLeaveCombat")
RegisterUnitEvent(111058, 3, "Snow_OnKilledTarget")
RegisterUnitEvent(111058, 4, "Snow_OnDeath")
RegisterUnitEvent(111058, 18, "Snow_OnSpawn")