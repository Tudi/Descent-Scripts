function Matron_OnSpawn(Unit, Event)
	Unit:RegisterEvent("Matron_Chat1", 36000, 0)
end

function Matron_Chat1(Unit, Event)
local Fire1 = Unit:GetCreatureNearestCoords(-4763.8984, 3051.1660, 92.0082, 23537)
 if Fire1 ~= nil and Fire1:IsAlive() == true then
	Unit:SendChatMessage(12, 0, "Grab a bucket from the water barrel and toss it on the fires or toss it to a friend who's closer to the blaze.")
end
end

function Matron_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Matron_OnDeath(Unit, Event)
	Unit:RemoveEvents()
end

RegisterUnitEvent(111049, 2, "Matron_OnLeaveCombat")
RegisterUnitEvent(111049, 4, "Matron_OnDeath")
RegisterUnitEvent(111049, 18, "Matron_OnSpawn")