function Morlim_OnSpawn(Unit, Event)
	Unit:RegisterEvent("Morlim_CheckInRangePlayers", 2000, 0)
end

function Morlim_CheckInRangePlayers(Unit, Event)
local plr = Unit:GetClosestPlayer()
  if plr ~= nil then
    if Unit:GetDistance(plr) < 460 then
       Unit:RemoveEvents()
       Unit:SendChatMessage(14, 0, "Oh, my! I wasn't expecting company! The workshop is such a mess! How embarrassing!")
       Unit:PlaySoundToSet(15611)
       Unit:MoveTo(-4512.5810, -2477.4628, 42.4846, 1.6399)
       Unit:RegisterEvent("Morlim_CheckPlayersStill", 2600, 0)
       Unit:RegisterEvent("Morlim_Move1", 18800, 1)
       Unit:RegisterEvent("Morlim_Phase1", 18000, 1)
       Unit:RegisterEvent("Morlim_Phase2", 240000, 1)
       Unit:RegisterEvent("Morlim_Enrage", 360000, 0)
    end
  end
end

function Morlim_CheckPlayersStill(Unit, Event)
local plr = Unit:GetClosestPlayer()
  if plr ~= nil then
    if Unit:GetDistance(plr) > 4000 then
       Unit:RemoveEvents()
       Unit:Despawn(500, 6000)
    end
  end
end

function Morlim_Move1(Unit, Event)
	Unit:MoveTo(-4512.4067, -2479.1867, 43.0908, 4.7767)
end

function Morlim_Phase1(Unit, Event)
	Unit:SpawnCreature(111011, -4509.4892, -2518.5737, 33.1020, 4.70, 14, 0)
end

function Morlim_Phase2(Unit, Event)
	Unit:RegisterEvent("Morlim_Attackable", 16000, 1)
	Unit:SetFaction(16)
	Unit:MoveTo(-4509.3886, -2512.7590, 31.9676, 4.7006)
	Unit:SendChatMessage(14, 0, "It looks like if you want something done, you gotta do it yourself!")
end

function Morlim_Attackable(Unit, Event)
	Unit:SetFaction(16)
end

function Morlim_OnCombat(Unit, Event)
	Unit:RegisterEvent("Morlim_ElectricalDischarge", 2000, 0)
	Unit:RegisterEvent("Morlim_ThorimsHammer", 18000, 0)
	Unit:RegisterEvent("Morlim_Enrage", 240000, 0)
end

function Morlim_ElectricalDischarge(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(79879, rand)
	end
end

function Morlim_ThorimsHammer(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(62912, rand)
	end
end

function Morlim_Enrage(Unit, Event)
	Unit:FullCastSpell(26662)
end

function Morlim_OnLeaveCombat(Unit, Event)
	Unit:PlaySoundToSet(15622)
	Unit:SendChatMessage(14, 0, "You can do better than that!")
	Unit:RemoveEvents()
	Unit:SetFaction(35)
	Unit:Despawn(500, 6000)
end

function Morlim_OnKilledTarget(Unit, Event)
	Unit:PlaySoundToSet(15617)
	Unit:SendChatMessage(14, 0, "Fascinating! I think they call that a clean kill!")
end

function Morlim_OnDeath(Unit, Event)
	Unit:PlaySoundToSet(15627)
	Unit:SendChatMessage(14, 0, "It would appear that I made a slight miscalculation. I allowed my mind to be corrupted by that fiend in the final chamber. Over-riding my primary directive. All systems seem to be functional now. Clear.")
	Unit:RemoveEvents()
end

RegisterUnitEvent(111007, 1, "Morlim_OnCombat")
RegisterUnitEvent(111007, 2, "Morlim_OnLeaveCombat")
RegisterUnitEvent(111007, 3, "Morlim_OnKilledTarget")
RegisterUnitEvent(111007, 4, "Morlim_OnDeath")
RegisterUnitEvent(111007, 18, "Morlim_OnSpawn")