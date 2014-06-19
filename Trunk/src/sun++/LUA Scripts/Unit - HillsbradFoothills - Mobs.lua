--[[ UNIT - Hillsbradfoothills - Mobs.lua

********************************
*                                                            *
* The Moon++ Scripting Project        *
*                                                            *
********************************

This software is provided as free and open source by the
staff of The Moon++ Scripting Project, in accordance with 
the GPL license. This means we provide the software we have 
created freely and it has been thoroughly tested to work for 
the developers, but NO GUARANTEE is made it will work for you 
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Staff of The Moon++ Scripting Project, November 2007
~~End of License Agreement

All credits for the files in this folder that are GameMonkey scripts
go to the GMScripts project, for all their hard work for the Ascent 
community.

-- Moon++ Project, December 2007 ]]
-- [[ Unit - HillsbradFoothills - Mobs ]] --

function Rousch_FriendsTribute(pUnit, event)
    pUnit:Emote(113)
end

function Rousch_OnSpawn(pUnit, event)
	pUnit:SetStandState(8)
	pUnit:RegisterEvent("Rousch_FriendsTribute",5000, 0)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(16090, 6, "Rousch_OnSpawn")