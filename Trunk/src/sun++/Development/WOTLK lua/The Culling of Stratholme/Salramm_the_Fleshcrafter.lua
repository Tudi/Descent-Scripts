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

function Salramm_OnEnterCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(14, 0, "Ah, the entertainment has arrived!")
	pUnit:RegisterEvent("Salramm_flesh", 12000, 0)
	pUnit:RegisterEvent("Salramm_ShBolt", 3500, 0)
	pUnit:RegisterEvent("Salramm_ExplodeGhouls", 25000, 0)
	pUnit:RegisterEvent("Salramm_ghsummon", 7000, 0)
end

function Salramm(pUnit,Event)
	pUnit:RegisterEvent("say", 70000, 0)
end

function Say (pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(14, 0, "You are too late, champion of Lordaeron. The dead shall have their day.")
end

function Salramm_OnKill(pUnit,Event)
	local killchoice=math.random(1, 3)
		if killchoice==1 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "The fun is just beginning!")
		elseif killchoice==2 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Aah, quality materials!")
		elseif killchoice==3 then
				--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Don't worry, I'll make good use of you.")
end
end

function Salramm_OnDied(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(14, 0, "You only advance... the master's plan... ")
	pUnit:RemoveEvents()
end

function Salramm_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

function Salramm_ShBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(15232,pUnit:GetMainTank())
end

function Salramm_flesh(pUnit,Event)
	pUnit:FullCastSpellOnTarget(52708,pUnit:GetMainTank())
	local Choice=math.random(1, 3)
		if Choice==1 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "I want a sample...")
		elseif Choice==2 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Such strength... it must be mine!")
		elseif Choice==3 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Your flesh betrays you.")
end
end

function Salramm_ExplodeGhouls(pUnit,Event)
	pUnit:CastSpell(52480)
	local Choice=math.random(1, 2)
		if Choice==1 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "BOOM! Hahahahah...")
		elseif Choice==2 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Blood... destruction... EXHILARATING!")
end
end

function Salramm_ghsummon(pUnit,Event)
	pUnit:CastSpell(52451)
	local Choice=math.random(1, 2)
		if Choice==1 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Say hello to some friends of mine.")
		elseif Choice==2 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Come, citizen of Stratholme! Meet your saviors.")
end
end

RegisterUnitEvent(26530, 1, "Salramm_OnEnterCombat")
RegisterUnitEvent(26530, 2, "Salramm_OnLeaveCombat")
RegisterUnitEvent(26530, 3, "Salramm_OnKill")
RegisterUnitEvent(26530, 4, "Salramm_OnDied")
RegisterUnitEvent(26530, 5, "Salramm")


