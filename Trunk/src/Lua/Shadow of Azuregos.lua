function SOA_OnCombat(Unit, Event)
	Unit:FullCastSpell(45813)
	Unit:SendChatMessage(14, 0, "Soldiers of the shadows, rise to meet your master's call!")
	Unit:PlaySoundToSet(16714)
	Unit:RegisterEvent("SOA_FlashFreeze", 16000, 0)
	Unit:RegisterEvent("SOA_FrostBreathe", 8000, 0)
	Unit:RegisterEvent("SOA_FrostAura", 1000, 1)
	Unit:RegisterEvent("SOA_IceVolley", 22000, 0)
	Unit:RegisterEvent("SOA_Enrage", 280000, 0)
	Unit:RegisterEvent("SOA_IcyGrip", 28000, 0)
	Unit:RegisterEvent("SOA_HPCheck", 800, 0)
end

function SOA_FrostAura(Unit, Event)
	Unit:FullCastSpell(70084)
end

function SOA_IceVolley(Unit, Event)
	Unit:FullCastSpell(70464)
end

function SOA_IcyGrip(Unit, Event)
	Unit:FullCastSpell(70117)
end

function SOA_FrostBreathe(Unit, Event)
	Unit:FullCastSpell(73061)
end

function SOA_FlashFreeze(Unit, Event)
	Unit:FullCastSpell(64141)
	Unit:ClearThreatList()
end

function SOA_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
	Unit:RemoveAllAuras()
	Unit:ReturnToSpawnPoint()
end

function SOA_OnKilledTarget(Unit, Event)
	Unit:PlaySoundToSet(16715)
	Unit:SendChatMessage(14, 0, "Despair.. so delicious..")
end

function SOA_HPCheck(Unit, Event)
	if Unit:GetHealthPct() <= 1 then
        Unit:GetGameObjectNearestCoords(-4003.071777, -2613.740967, 22.346905, 45540):Activate()
    end
end

function SOA_OnDeath(Unit, Event)
	Unit:PlaySoundToSet(16713)
	Unit:SendChatMessage(14, 0, "Martha, finish them")
	Unit:RemoveEvents()
	Unit:RemoveAllAuras()
end

function SOA_Enrage(Unit, Event)
	Unit:FullCastSpell(82395)
	Unit:SendChatMessage(14, 0, "It seems as if you thought you would defeat me!")
end

RegisterUnitEvent(111005, 1, "SOA_OnCombat")
RegisterUnitEvent(111005, 2, "SOA_OnLeaveCombat")
RegisterUnitEvent(111005, 3, "SOA_OnKilledTarget")
RegisterUnitEvent(111005, 4, "SOA_OnDeath")