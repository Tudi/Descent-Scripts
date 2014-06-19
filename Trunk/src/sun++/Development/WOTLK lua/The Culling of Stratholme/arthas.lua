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
This software is provided as free and open source by the
staff of the Sun++ Scripts Project, in accordance with 
the GPL license. This means we provide the software we have 
created freely and it has been thoroughly tested to work for 
the developers, but NO GUARANTEE is made it will work for you 
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Sun++ Team, July 2008
~End of License Agreement
Made by: Recon
-- ]]
--[[

http://www.youtube.com/watch?v=wmN3040a-aI

]]--


local npcid = 26499

function arthasevent_OnGossipTalk(pUnit, event, player, pMisc)
pUnit:GossipCreateMenu(100, player, 0)
pUnit:GossipMenuAddItem(7, "I'm ready to battle the dreadlord Sir", 1, 0) 
pUnit:GossipSendMenu(player)
end

function arthasevent_OnGossipSelect(pUnit, event, player, id, intid, code, pMisc)
if (intid == 1) then
--[[	pUnit:CreateWaypoint(x,y,z,waittime,moveflags,displayid)
	pUnit:CreateWaypoint(x,y,z,waittime,moveflags,displayid)
	pUnit:CreateWaypoint(x,y,z,waittime,moveflags,displayid)
	pUnit:CreateWaypoint(x,y,z,waittime,moveflags,displayid)
	pUnit:CreateWaypoint(x,y,z,waittime,moveflags,displayid)
	pUnit:CreateWaypoint(x,y,z,waittime,moveflags,displayid)
	pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"Justice will be done")	
]]--
end
end

RegisterUnitGossipEvent(npcid, 1, "arthasevent_OnGossipTalk")
RegisterUnitGossipEvent(npcid, 2, "arthasevent_OnGossipSelect")