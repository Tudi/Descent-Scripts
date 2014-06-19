function Cloud_OnSpawn(Unit, Event)
	Unit:FullCastSpell(63084)
	Unit:RegisterEvent("Cloud_CheckInRangePlayers", 2000, 0)
end

function Cloud_CheckInRangePlayers(Unit, Event)
local plr = Unit:GetClosestPlayer()
  if plr ~= nil then
    if Unit:GetDistance(plr) < 80 then
       Unit:SpawnCreature(111046, x+2, y+2, z, o, 16, 600000)
    end
  end
end

RegisterUnitEvent(111045, 18, "Cloud_OnSpawn")