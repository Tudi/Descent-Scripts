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

function Meathook(pUnit,Event)
	pUnit:RegisterEvent("Meathook_say", 25000, 0)
end

function Meathook_say(pUnit,Event)
pUnit:SendChatMessage(14, 0, "Play time!")
end

function Meathook_OnEnterCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(14, 0, "New toys!")
	pUnit:RegisterEvent("Constrictingchains", 12000, 0)
	pUnit:RegisterEvent("Diseaseexp", 7000, 0)
end

function Meathook_OnKill(pUnit,Event)
	local Choice=math.random(1, 3)
		if Choice==1 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Boring...")
		elseif Choice==2 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Why you stop moving?")
		elseif Choice==3 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Get up! Me not done!")
end
end

function Meathook_DiseaseExplusion(pUnit,Event)
	pUnit:FullCastSpellOnTarget(52666,pUnit:GetMainTank())
end

function Constrictingchains(pUnit,Event)
	pUnit:FullCastSpellOnTarget(52696,pUnit:GetMainTank())
end

function Meathook_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Meathook_OnDied(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(14, 0, "This not fun...")
	pUnit:RemoveEvents()
end

RegisterUnitEvent(26529, 1, "Meathook")
RegisterUnitEvent(26529, 2, "Meathook_OnEnterCombat")
RegisterUnitEvent(26529, 3, "Meathook_OnKill")
RegisterUnitEvent(26529, 4, "Meathook_OnLeaveCombat")
RegisterUnitEvent(26529, 5, "Meathook_OnDied")
