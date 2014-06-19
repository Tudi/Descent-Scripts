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
RegisterUnitEvent(14498, 18, "tosi")
RegisterUnitEvent(14498, 4, "tosi_Died")

function tosi_Died(Unit, event, player)
Unit:RemoveEvents()
end

function tosi(Unit, event, player)
Unit:RegisterEvent("tosi_Say",69000, 0)
end

function tosi_Say(Unit, event, player)
Unit:SendChatMessage(12, 0, "You are mine now children. You will grow up to be a strong of the horde!")
end