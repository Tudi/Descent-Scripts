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


function MaidenofGrief_OnCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"You shouldn't have come...now you will die!")
	pUnit:RegisterEvent("PillarofWoe",4000, 0) --50761--
	pUnit:RegisterEvent("ShockofSorrow",7000, 0) --50760-- stun
	pUnit:RegisterEvent("StormofGrief",15000, 0) --50752  Aoe --
end

function StormofGrief (pUnit,Event)
	pUnit:FullCastSpell(50752)
end

function ShockofSorrow (pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"So much lost time... that you'll never get back!")
	pUnit:FullCastSpellOnTarget(50760,pUnit:GetMainTank())
end

function PillarofWoe (pUnit,Event)
	pUnit:FullCastSpellOnTarget(50761,pUnit:GetMainTank())
end

function MaidenofGrief_OnKilledTarget (pUnit, Event)
	local killchoice=math.random(1, 4)
	if killchoice==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Why must it be this way?")
	elseif killchoice==2 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"You had it coming!")
	elseif killchoice==3 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"This is your own fault!")	
	elseif killchoice==4 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"My burden grows heavier.")	
end		
end

function MaidenofGrief_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function MaidenofGrief_OnDied(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "I hope you all rot! I never...wanted...this.")	
	pUnit:RemoveEvents()
end

RegisterUnitEvent(27975, 1, "MaidenofGrief_OnCombat")
RegisterUnitEvent(27975, 2, "MaidenofGrief_OnLeaveCombat")
RegisterUnitEvent(27975, 3, "MaidenofGrief_OnDied")
RegisterUnitEvent(27975, 4, "MaidenofGrief_OnKilledTarget")