function LordJaraxxus_OnEnterCombat(pUnit, event)
	pUnit:RegisterEvent("LordJaraxxus_IncinerateFlesh", 23000, 0)
	pUnit:RegisterEvent("LordJaraxxus_LegionFlame", 30000, 0)
	pUnit:RegisterEvent("LordJaraxxus_InfernalEruption", 120000, 0)
	pUnit:RegisterEvent("LordJaraxxus_NetherPortal", 120000, 0)
	pUnit:RegisterEvent("LordJaraxxus_FelFireball", 10000, 0)
	pUnit:RegisterEvent("LordJaraxxus_FelLightning", 20000, 0)
	pUnit:RegisterEvent("LordJaraxxus_NetherPower", 15000, 0)
end

function LordJaraxxus_IncinerateFlesh(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if (player ~= nil) then
		pUnit:CastSpellOnTarget(66237, player)
	end
end

function LordJaraxxus_LegionFlame(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if (player ~= nil) then
		pUnit:FullCastSpellOnTarget(68124, player)
	end
end

function LordJaraxxus_InfernalEruption(pUnit, event)
	pUnit:CastSpell(66258)
end

function LordJaraxxus_NetherPortal(pUnit, event)
	pUnit:SpawnCreature(34825, pUnit:GetX()+10, pUnit:GetY()+10, pUnit:GetZ()+1, pUnit:GetO(), pUnit:GetFaction(), 8000)
end

function LordJaraxxus_FelFireball(pUnit, event)
	pUnit:FullCastSpellOnTarget(66532,pUnit:GetMainTank())
end

function LordJaraxxus_FelLightning(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if (player ~= nil) then
		pUnit:CastSpellOnTarget(66528, player)
	end
end

function LordJaraxxus_NetherPower(pUnit, event)
	pUnit:CastSpell(67108)
end

RegisterUnitEvent(34780, 1, "LordJaraxxus_OnEnterCombat")

function NetherPortal_OnSpawn(pUnit, event)
	pUnit:CastSpell(66264)
end

RegisterUnitEvent(34825, 18, "NetherPortal_OnSpawn")

function InfernalVolcano_OnSpawn(pUnit, event)
	pUnit:RegisterEvent("InfernalVolcano_InfernalEruption", 8000, 3)
end

function InfernalVolcano_InfernalEruption(pUnit, event)
	pUnit:CastSpell(66255)
end

RegisterUnitEvent(34813, 18, "InfernalVolcano_OnSpawn")