function Malinda_OnSpawn(Unit, Event)
	Unit:RegisterEvent("Malinda_CheckInRangePlayers", 2000, 0)
end

function Malinda_CheckInRangePlayers(Unit, Event)
local plr = Unit:GetClosestPlayer()
  if plr ~= nil then
    if Unit:GetDistance(plr) < 460 then
       Unit:RemoveEvents()
       Unit:SetNPCFlags(4)
       Unit:Root()
       Unit:SendChatMessage(14, 0, "The time to strike at the head of the beast will soon be upon us. Focus your anger and hatred on his minion!")
       Unit:PlaySoundToSet(15775)
       Unit:RegisterEvent("Malinda_CheckPlayersStill", 2600, 0)
       Unit:RegisterEvent("Malinda_Phase1", 10000, 1)
       Unit:RegisterEvent("Malinda_Phase2", 120000, 1)
    end
  end
end

function Malinda_CheckPlayersStill(Unit, Event)
local plr = Unit:GetClosestPlayer()
  if plr ~= nil then
    if Unit:GetDistance(plr) > 4000 then
       Unit:RemoveEvents()
       Unit:PlaySoundToSet(15780)
       Unit:SendChatMessage(14, 0, "Week minded fool!")
       Unit:Despawn(500, 6000)
    end
  end
end

function Malinda_Phase1(Unit, Event)
	Unit:RegisterEvent("Malinda_Guardians", 6000, 20)
	Unit:RegisterEvent("Malinda_IceVolley", 1800, 60) 
	Unit:RegisterEvent("Malinda_Chat1", 16000, 6)
	Unit:RegisterEvent("Malinda_Chat2", 24000, 4)
end

function Malinda_Guardians(Unit, Event)
	x = Unit:GetX()
	y = Unit:GetY()
	z = Unit:GetZ()
	o = Unit:GetO()
	Unit:SpawnCreature(111020, x+12, y+14, z+1, o, 16, 600000)
end

function Malinda_IceVolley(Unit, Event)
	Unit:FullCastSpell(70464)
end

function Malinda_Chat1(Unit, Event)
	Unit:PlaySoundToSet(15774)
	Unit:SendChatMessage(14, 0, "Let hatred and rage guide your blows!")
end

function Malinda_Chat2(Unit, Event)
	Unit:PlaySoundToSet(15773)
	Unit:SendChatMessage(14, 0, "Yes, Yes! Show them no mercy! Give no pause to your attack!")
end

function Malinda_Phase2(Unit, Event)
	Unit:RegisterEvent("Malinda_Despawn", 8000, 1)
	Unit:RegisterEvent("Malinda_SpawnBoss", 12000, 1)
	Unit:PlaySoundToSet(15777)
	Unit:SendChatMessage(14, 0, "Tremble mortals before the coming of the end!")
end

function Malinda_Despawn(Unit, Event)
	Unit:Despawn(6000, 0)
end

function Malinda_SpawnBoss(Unit, Event)
	x = Unit:GetX()
	y = Unit:GetY()
	z = Unit:GetZ()
	o = Unit:GetO()
	Unit:SpawnCreature(111018, x, y, z+1, o, 35, 600000)
end

function Malinda_OnKilledTarget(Unit, Event)
	Unit:SendChatMessage(14, 0, "Suffocate upon your own hate!")
	Unit:PlaySoundToSet(15776)
end

function Malinda_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
	Unit:SetFaction(35)
	Unit:Despawn(500, 6000)
end

function Malinda_OnDeath(Unit, Event)
	Unit:RemoveEvents()
end

RegisterUnitEvent(111017, 2, "Malinda_OnLeaveCombat")
RegisterUnitEvent(111017, 3, "Malinda_OnKilledTarget")
RegisterUnitEvent(111017, 4, "Malinda_OnDeath")
RegisterUnitEvent(111017, 18, "Malinda_OnSpawn")