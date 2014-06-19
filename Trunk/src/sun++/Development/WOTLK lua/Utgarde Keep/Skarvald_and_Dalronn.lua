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
function Dalronn_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("Dalronn_Reply",2000, 1)	
	pUnit:RegisterEvent("DShadowBolt",3200, 0)
	pUnit:RegisterEvent("DDebilitate",8500, 0)
end

function Reply(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(14, 0,"By all means, don't assess the situation, you halfwit! Just jump into the fray!")
end


function DDebilitate(pUnit, Event)
	pUnit:FullCastSpellOnTarget(43650,pUnit:GetMainTank())
end

function DShadowBolt(pUnit, Event)
	pUnit:FullCastSpellOnTarget(43649,pUnit:GetMainTank())
end

function Dalronn_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Dalronn_OnDied(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(14, 0,"See... you... soon.")
	pUnit:RemoveEvents()
end

RegisterUnitEvent(24201, 1, "Dalronn_OnEnterCombat")
RegisterUnitEvent(24201, 2, "Dalronn_OnLeaveCombat")
RegisterUnitEvent(24201, 3, "Dalronn_OnDied")


function Skarvald_OnEnterCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(14, 0,"Dalronn! See if you can muster the nerve to join my attack!")
	pUnit:RegisterEvent("SCharge",6500, 0)
	pUnit:RegisterEvent("sStoneStrike",10000, 0)
end

function SCharge(pUnit, Event)
	pUnit:FullCastSpellOnTarget(43651,pUnit:GetRandomPlayer(0))
end

function sStoneStrike(pUnit, Event)
	pUnit:FullCastSpellOnTarget(48583,pUnit:GetMainTank())
end

function Skarvald_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Skarvald_OnDied(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(14, 0,"A warrior's death.")
	pUnit:RemoveEvents()
end

RegisterUnitEvent(24200, 1, "Skarvald_OnEnterCombat")
RegisterUnitEvent(24200, 2, "Skarvald_OnLeaveCombat")
RegisterUnitEvent(24200, 3, "Skarvald_OnDied")
