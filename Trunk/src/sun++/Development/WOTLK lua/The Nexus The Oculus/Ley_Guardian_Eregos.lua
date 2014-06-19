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
TODO:

-air fight
-Missing  spell?
Arcane shield:
    * We command the arcane! It shall not be used against us. 
Fire shield:
    * It is trivial to extinguish your fire! 
Nature shield:
    * No magic of nature will help you now! 
Frenzy:
    * Such insolence... such arrogance... must be PUNISHED! 
]]--

function LeyGuardianEregos_OnEnterCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(14, 0, "You brash interlopers are out of your element! I will ground you! ")
	pUnit:RegisterEvent("PlanarShift", 15000, 0)  -- Well I hope This Cd works  i think  our problem this spell
	pUnit:RegisterEvent("ArcaneVolley", 10000, 0)
	pUnit:RegisterEvent("EnragedAssault", 25000, 0)
	pUnit:RegisterEvent("ArcaneBarrage", 7000, 0)
end

function LeyGuardianEregos(pUnit,Event)
	pUnit:RegisterEvent("say", 70000, 0)
end

function Say (pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(14, 0, "Simpletons! You cannot comprehend the forces you have set in motion. The ley line conduit will not be disrupted! Your defeat shall be absolute! ")
end


function PlanarShift(pUnit,Event)
		pUnit:FullCastSpell(51162)
end

function EnragedAssault(pUnit,Event)
		pUnit:FullCastSpell(51170)
end

function ArcaneVolley(pUnit,Event)
	pUnit:FullCastSpellOnTarget(51153,pUnit:GetMainTank())

end

function ArcaneBarrage(pUnit,Event)
	pUnit:FullCastSpellOnTarget(50804,pUnit:GetMainTank())

end




function LeyGuardianEregos_OnKill(pUnit,Event)
	local killchoice=math.random(1, 3)
		if killchoice==1 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Enjoy the fall! ")
		elseif killchoice==2 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Back to the earth with you!")
		elseif killchoice==3 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "It's a long way down!")
end
end

function LeyGuardianEregos_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function LeyGuardianEregos_OnDied(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(14, 0, "Savor this small victory, foolish little creatures. You and your dragon allies have won this battle. But we will win... the Nexus War")
	pUnit:RemoveEvents()
end

RegisterUnitEvent(27656, 1, "LeyGuardianEregos_OnEnterCombat")
RegisterUnitEvent(27656, 2, "LeyGuardianEregos_OnLeaveCombat")
RegisterUnitEvent(27656, 3, "LeyGuardianEregos_OnKill")
RegisterUnitEvent(27656, 4, "LeyGuardianEregos_OnDied")
RegisterUnitEvent(27656, 5, "LeyGuardianEregos")


