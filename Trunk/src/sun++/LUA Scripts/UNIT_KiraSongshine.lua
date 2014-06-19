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
RegisterUnitEvent(3937, 18, "kira")
RegisterUnitEvent(3937, 4, "kira_Died")

function kira_Died(Unit, event, player)
Unit:RemoveEvents()
end

function kira(Unit, event, player)
Unit:RegisterEvent("kira_Say",49000, 0)
end

function kira_Say(Unit, event, player)
local chance = math.random(1,3)
if(chance == 1) then
Unit:SendChatMessage(12, 0, "Fresh bread, baked this very morning!")
end
if(chance == 2) then
Unit:SendChatMessage(12, 0, "Come get yer fresh bread!")
end
if(chance == 3) then
Unit:SendChatMessage(12, 0, "Fresh bread for sale!")
end
end