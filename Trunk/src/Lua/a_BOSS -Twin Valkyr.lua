function EydisDarkbane_OnEnterCombat(pUnit, event)
	pUnit:CastSpell(65768)
	pUnit:RegisterEvent("EydisDarkbane_DarkVortex", 10000, 1)
	pUnit:RegisterEvent("EydisDarkbane_TwinSpike", 20000, 1)
end

function EydisDarkbane_ShieldofDarkness(pUnit, event)
	pUnit:CastSpell(65874)
	pUnit:FullCastSpell(65875)
	local soulLink = pUnit:GetSoulLinkedWith()
	soulLink:CastSpell(65916)
	soulLink:RegisterEvent("FjolaLightbane_ShieldofLights", 45000, 1)
end

function EydisDarkbane_DarkVortex(pUnit, event)
	pUnit:FullCastSpell(66058)
	pUnit:RegisterEvent("EydisDarkbane_DarkVortex", 30000, 1)
end

function EydisDarkbane_TwinSpike(pUnit, event)
	pUnit:CastSpell(66069)
	pUnit:RegisterEvent("EydisDarkbane_TwinSpike", 30000, 1)
end

function EydisDarkbane_OnSpawn(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if (player ~= nil) then
		pUnit:FullCastSpellOnTarget(68124, player)
	end
end

function EydisDarkbane_OnSpawn(pUnit, event)
	pUnit:SetSoulLinkedWith(pUnit:GetCreatureNearestCoords(pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), 34497))
end

RegisterUnitEvent(34496, 1, "EydisDarkbane_OnEnterCombat")
RegisterUnitEvent(34496, 18, "EydisDarkbane_OnSpawn")

function FjolaLightbane_OnEnterCombat(pUnit, event)
	pUnit:CastSpell(65766)
	pUnit:RegisterEvent("FjolaLightbane_ShieldofLights", 45000, 1)
	pUnit:RegisterEvent("FjolaLightbane_LightVortex", 30000, 0)
	pUnit:RegisterEvent("FjolaLightbane_TwinSpike", 30000, 0)
end

function FjolaLightbane_ShieldofLights(pUnit, event)
	pUnit:CastSpell(65858)
	pUnit:FullCastSpell(65876)
	local soulLink = pUnit:GetSoulLinkedWith()
	soulLink:CastSpell(65916)
	soulLink:RegisterEvent("EydisDarkbane_ShieldofDarkness", 45000, 1)
end

function FjolaLightbane_LightVortex(pUnit, event)
	pUnit:FullCastSpell(66046)
end

function FjolaLightbane_TwinSpike(pUnit, event)
	pUnit:CastSpell(66069)
end

function FjolaLightbane_OnSpawn(pUnit, event)
	pUnit:SetSoulLinkedWith(pUnit:GetCreatureNearestCoords(pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), 34496))
end

RegisterUnitEvent(34497, 1, "FjolaLightbane_OnEnterCombat")
RegisterUnitEvent(34497, 18, "FjolaLightbane_OnSpawn")