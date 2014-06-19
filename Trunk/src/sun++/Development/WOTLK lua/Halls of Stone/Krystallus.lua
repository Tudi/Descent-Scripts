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

	
function Krystallus_OnCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"Crush....")
	pUnit:RegisterEvent("BoulderToss",10000, 0) --50843--
	pUnit:RegisterEvent("stone",7000, 0) --50810--    Script effect
	pUnit:RegisterEvent("Stomp",8000, 0) --50868 --
end

function Stomp (pUnit,Event)
	pUnit:FullCastSpell(50868)
end

-- Need  c++ scipt--
function stone (pUnit,Event)  
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"Break.... you....")
	pUnit:FullCastSpellOnTarget(50810,pUnit:GetMainTank())
end

function BoulderToss (pUnit,Event)
	pUnit:FullCastSpellOnTarget(50843,pUnit:GetMainTank())
end


function Krystallus_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Krystallus_OnDied(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "Uuuuhhhhhhhhhh......")	
	pUnit:RemoveEvents()
end

RegisterUnitEvent(27977, 1, "Krystallus_OnCombat")
RegisterUnitEvent(27977, 2, "Krystallus_OnLeaveCombat")
RegisterUnitEvent(27977, 3, "Krystallus_OnDied")
