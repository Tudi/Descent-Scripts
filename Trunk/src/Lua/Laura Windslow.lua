function Laura_OnSpawn(Unit, Event)
	Unit:RegisterEvent("Laura_CheckInRangePlayers", 2000, 0)
end

function Laura_CheckInRangePlayers(Unit, Event)
local plr = Unit:GetClosestPlayer()
  if plr ~= nil then
    if Unit:GetDistance(plr) < 160 then
       Unit:PlaySoundToSet(16631)
       Unit:SendChatMessage(14, 0, "The chill of this placeÉBrrÉ I can feel my blood freezing.")
       Unit:RemoveEvents()
    end
  end
end

function Laura_OnCombat(Unit, Event)
end

function Laura_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Laura_OnKilledTarget(Unit, Event)
end

function Laura_OnDeath(Unit, Event)
	Unit:RemoveEvents()
end

RegisterUnitEvent(111029, 1, "Laura_OnCombat")
RegisterUnitEvent(111029, 2, "Laura_OnLeaveCombat")
RegisterUnitEvent(111029, 3, "Laura_OnKilledTarget")
RegisterUnitEvent(111029, 4, "Laura_OnDeath")
RegisterUnitEvent(111029, 18, "Laura_OnSpawn")