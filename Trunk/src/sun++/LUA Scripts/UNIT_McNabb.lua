--[[
***************************
*      .-.                *
*      `-.. ..-. + +      *
*      `-''-'' '          *
*  OpenSource Scripting   *
*          Team           *
* http://sunplusplus.info *
*                         *
***************************
Created:Recon
-- ]]
RegisterUnitEvent(1402, 18, "mcnabb")
RegisterUnitEvent(1402, 4, "mcnabb_Died")

function mcnabb_Died(Unit, event, player)
Unit:RemoveEvents()
end

function mcnabb(Unit, event, player)
Unit:RegisterEvent("mcnabb_Say",40000, 0)
end

function mcnabb_Say(Unit, event, player)
local chance = math.random(1,3)
if(chance == 1) then
Unit:SendChatMessage(12, 0, "It's all their fault, stupid Alliance army. Just had to build their towers right behind my farm." )
end
if(chance == 2) then
Unit:SendChatMessage(12, 0, "I will gladly pay you Tuesday for a hamburger today.")
end
if(chance == 3) then
Unit:SendChatMessage(12, 0, "Spare some change for a poor blind man? ...What do you mean I'm not blind? ...I'M NOT BLIND! I CAN SEE! It's a miracle!")
end
end