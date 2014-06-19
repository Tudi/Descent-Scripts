function Healer_OnCombat(Unit, Event)
	Unit:RegisterEvent("Healer_PrayerOfHealing", 24600, 0)
	Unit:RegisterEvent("Healer_PrayerOfMending", 14000, 0)
	Unit:RegisterEvent("Healer_HexOfMending", 8200, 0)
end

function Healer_PrayerOfHealing(Unit, Event)
	Unit:FullCastSpell(59698)
end

function Healer_PrayerOfMending(Unit, Event)
	Unit:FullCastSpell(93093)
end

function Healer_HexOfMending(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(67534, rand)
	end
end

function Healer_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Healer_OnDeath(Unit, Event)
	Unit:RemoveEvents()
end

RegisterUnitEvent(111043, 1, "Healer_OnCombat")
RegisterUnitEvent(111043, 2, "Healer_OnLeaveCombat")
RegisterUnitEvent(111043, 4, "Healer_OnDeath")