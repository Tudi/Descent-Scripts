function Stonewatcher_OnCombat(Unit, Event)
	Unit:PlaySoundToSet(15657)
	Unit:SendChatMessage(14, 0, "Nothing short of total decimation will suffice.")
	Unit:RegisterEvent("Stonewatcher_Tantrum", 36000, 0)
	Unit:RegisterEvent("Stonewatcher_StoneMantle", 48000, 0)
	Unit:RegisterEvent("Stonewatcher_Thunder", 18000, 0)
	Unit:RegisterEvent("Stonewatcher_ThunderShock", 28000, 0)
	Unit:RegisterEvent("Stonewatcher_Enrage", 280000, 0)
	Unit:RegisterEvent("Stonewatcher_RuneOfDeath", 62000, 0)
	Unit:RegisterEvent("Stonewatcher_HealthCheck", 2200, 0)
end

function Stonewatcher_Thunder(Unit, Event)
	Unit:FullCastSpell(53630)
end

function Stonewatcher_ThunderShock(Unit, Event)
	Unit:PlaySoundToSet(15661)
	Unit:SendChatMessage(14, 0, "Face the lightning surge!")
	Unit:FullCastSpell(83067)
end

function Stonewatcher_Tantrum(Unit, Event)
	Unit:FullCastSpell(62776)
	Unit:ClearThreatList()
end

function Stonewatcher_StoneMantle(Unit, Event)
	Unit:PlaySoundToSet(15660)
	Unit:SendChatMessage(14, 0, "Decipher this!")
	Unit:FullCastSpell(93561)
	Unit:ClearThreatList()
end

function Stonewatcher_RuneOfDeath(Unit, Event)
	Unit:PlaySoundToSet(15660)
	Unit:SendChatMessage(14, 0, "Decipher this!")
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nill) then
	Unit:FullCastSpellOnTarget(63490, rand)
	end
end

function Stonewatcher_OnLeaveCombat(Unit, Event)
	Unit:PlaySoundToSet(15662)
	Unit:SendChatMessage(14, 0, "The legacy of storms shall not be undone.")
	Unit:RemoveAllEvents()
	Unit:ReturnToSpawnPoint()
end

function Stonewatcher_OnKilledTarget(Unit, Event)
end

function Stonewacther_Enrage(Unit, Event)
	Unit:FullCastSpell(27680)
	Unit:PlaySoundToSet(15662)
	Unit:SendChatMessage(14, 0, "The legacy of storms shall not be undone.")
end

function Stonewatcher_HealthCheck(Unit, Event)
    if Unit:GetHealthPct() <= 1 then
        Unit:GetGameObjectNearestCoords(-4397.889648, -2702.456055, -43.460609, 45540):Activate()
	Unit:RegisterEvent("Stonewatcher_UnlockDoor", 1000, 1)
    end
end

function StoneWatcher_UnlockDoor(Unit, Event)
	Unit:GetGameObjectNearestCoords(-4509.145020, -2561.877197, 31.966936, 45541):Activate()
end

function Stonewatcher_OnDeath(Unit, Event)
	Unit:PlaySoundToSet(15663)
	Unit:SendChatMessage(14, 0, "What have you gained from my defeat? You are no less doomed, mortals!")
end

RegisterUnitEvent(111013, 1, "Stonewatcher_OnCombat")
RegisterUnitEvent(111013, 2, "Stonewatcher_OnLeaveCombat")
RegisterUnitEvent(111013, 3, "Stonewatcher_OnKilledTarget")
RegisterUnitEvent(111013, 4, "Stonewatcher_OnDeath")