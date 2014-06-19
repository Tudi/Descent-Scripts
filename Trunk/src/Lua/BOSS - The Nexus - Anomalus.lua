local Anomalus
local rift
function Anomalus_OnEnterCombat(pUnit, event)
	Anomalus = pUnit
	pUnit:SendChatMessage(14, 0, "Chaos beckons.")
	pUnit:PlaySoundToSet(13186)
	pUnit:RegisterEvent("Anomalus_Spells", 1000, 1)
	pUnit:RegisterEvent("Anomalus_CreateRiftCheck", 1000, 1)
end

function Anomalus_Spells(pUnit, event)
	pUnit:RegisterEvent("Anomalus_ArcaneAttraction", 15000, 0)
	pUnit:RegisterEvent("Anomalus_Spark", 7000, 0)
end

function Anomalus_ArcaneAttraction(pUnit, event)
	local player = pUnit:GetRandomPlayer(0)
	if (player ~= nil) then
		pUnit:FullCastSpellOnTarget(57063,player)
	end
end

function Anomalus_Spark(pUnit, event)
	local player = pUnit:GetRandomPlayer(0)
	if (player ~= nil) then
		pUnit:FullCastSpellOnTarget(47751,player)
	end
end

function Anomalus_CreateRiftCheck(pUnit, event)
	if pUnit:GetHealthPct() <= 50 then
		pUnit:RegisterEvent("Anomalus_CreateRiftEvent", 1000, 1)
	else
		pUnit:RegisterEvent("Anomalus_CreateRiftCheck", 1000, 1)
	end
end

function Anomalus_CreateRiftEvent(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:RegisterEvent("Anomalus_CreateRift", 500, 1)
	pUnit:RegisterEvent("Anomalus_RiftShield", 1500, 1)
end

function Anomalus_CreateRift(pUnit, event)
	pUnit:SpawnCreature(26918,655.016,-275.999,-8.85383,3.6238, pUnit:GetFaction(), 0) 
end
function Anomalus_RiftShield(pUnit, event)
	pUnit:CastSpell(47748)
end

function Anomalus_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Of course.")
	pUnit:PlaySoundToSet(13187)
	pUnit:RemoveEvents()
end

function Anomalus_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(26763, 1, "Anomalus_OnEnterCombat")
RegisterUnitEvent(26763, 2, "Anomalus_OnCombatExit")
RegisterUnitEvent(26763, 4, "Anomalus_Death")

function AnomalusRift_Death(pUnit, event)
	Anomalus:RemoveAura(47748)
	Anomalus:RegisterEvent("Anomalus_Spells", 1000, 1)
end

function AnomalusRift_Spawn(pUnit, event)
	rift = pUnit
	pUnit:Root()
	pUnit:CastSpell(47687)
	pUnit:CastSpell(47747)
	pUnit:RegisterEvent("Anomalus_SummonWraith", 9000, 0)
end

function Anomalus_SummonWraith(pUnit, event)
	pUnit:SpawnCreature(26746,655.016,-275.999,-8.85383,3.6238, pUnit:GetFaction(), 0) 
end

RegisterUnitEvent(26918, 4, "AnomalusRift_Death")
RegisterUnitEvent(26918, 18, "AnomalusRift_Spawn")