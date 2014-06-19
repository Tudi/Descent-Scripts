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
RegisterUnitEvent(8666, 18, "timmy")
RegisterUnitEvent(8666, 4, "timmy_Died")

function timmy_Died(Unit, event, player)
Unit:RemoveEvents()
end

function timmy(Unit, event, player)
Unit:RegisterEvent("timmy_Say",68000, 0)
end

function timmy_Say(Unit, event, player)
local chance = math.random(1,6)
if(chance == 1) then
Unit:SendChatMessage(12, 0, "Kitten for sale, looking for a good home." )
end
if(chance == 2) then
Unit:SendChatMessage(12, 0, "I can't believe dad won't let me keep your sister.")
end
if(chance == 3) then
Unit:SendChatMessage(12, 0, "Can anyone give my adorable, extra little kitty a home?")
end
if(chance == 4) then
Unit:SendChatMessage(12, 0, "What does allergic mean anyway? And what does it have to do with either of my kitties?")
end
if(chance == 5) then
Unit:SendChatMessage(12, 0, "Will someone please give my little kitten a good home?")
end
if(chance == 6) then
Unit:SendChatMessage(12, 0, "Don't worry, I'll find a good home for ya.")
end
end