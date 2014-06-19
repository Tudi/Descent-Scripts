function Tank_OnCombat(Unit, Event)
	Unit:RegisterEvent("Tank_BatteringRam", 18000, 0)
	Unit:RegisterEvent("Tank_FlameVents", 12000, 0)
	Unit:RegisterEvent("Tank_MimironInferno", 26000, 0)
	Unit:RegisterEvent("Tank_Enrage", 220000, 0)
end

function Tank_BatteringRam(Unit, Event)
	Unit:CastSpellOnTarget(62376, Unit:GetMainTank())
end

function Tank_FlameVents(Unit, Event)
	Unit:FullCastSpell(62396)
	Unit:ClearThreatList()
end

function Tank_MimironInferno(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(62910, rand)
	Unit:PlaySoundToSet(15510)
	Unit:SendChatMessage(14, 0, "Orbital countermeasures enabled.")
	end
end

function Tank_Enrage(Unit, Event)
	Unit:FullCastSpell(26662)
end

function Tank_OnLeaveCombat(Unit, Event)
	Unit:GetGameObjectNearestCoords(-4509.145020, -2561.877197, 31.966936, 45541):Activate()
	Unit:Despawn(4000, 0)
end

function Tank_OnDeath(Unit, Event)
	Unit:PlaySoundToSet(15520)
	Unit:SendChatMessage(14, 0, "Total systems failure. Defence protocols breached. XT-Tank Unit shutting down.")
	Unit:Despawn(4000, 0)
end

function Tank_OnKilledtarget(Unit, Event)
end

RegisterUnitEvent(111012, 1, "Tank_OnCombat")
RegisterUnitEvent(111012, 2, "Tank_OnLeaveCombat")
RegisterUnitEvent(111012, 3, "Tank_OnKilledTarget")
RegisterUnitEvent(111012, 4, "Tank_Death")