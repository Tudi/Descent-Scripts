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



function MalGanis_EnterCombat(pUnit, Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "This will be a fine test, Prince Arthas.") 
	pUnit:RegisterEvent("swarm", 6000, 0)
	pUnit:RegisterEvent("vampir", 8000, 0)
	pUnit:RegisterEvent("blast", 13000, 0)
	pUnit:RegisterEvent("sleep", 21000, 0)
	pUnit:RegisterEvent("outro", 20, 0)
end

function vampir(pUnit,Event)
	pUnit:FullCastSpellOnTarget(52723,pUnit:GetMainTank())  -- Serverside script!! Dont forget! :D
end



function swarm(pUnit, Event)
	pUnit:FullCastSpell(52720) 
end

function blast(pUnit,Event)
	pUnit:FullCastSpellOnTarget(52722,pUnit:GetMainTank()) 
end

function sleep(pUnit,Event)
pUnit:FullCastSpellOnTarget(52721,pUnit:GetRandomPlayer(0))
	local Choice=math.random(1, 2)
		if Choice==1 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(12, 0, "Time out.")
		elseif Choice==2 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(12, 0, "You seem tired.")
end
end

function blast(pUnit,Event)
	pUnit:FullCastSpellOnTarget(52723,pUnit:GetMainTank()) 
end

function outro pUnit,Event)
	if pUnit:GetHealthPct() < 2 then	
	pUnit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9)
	pUnit:Emote(1)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "Your journey has just begun, young prince...Gather your forces, and meet me in the arctic land of Northrend...It is there we shall settle the score between us...It is there that your true destiny will unfold...")
	SLEEP(9000); -- Its Work very well? :D --
	pUnit:RemoveFromWorld()
	--pUnit:SpawnGameObject(entry_id,x,y,z,duration)   well  chest id unknow :D
	end
end

function MalGanis_OnKill(pUnit,Event)
	pUnit:RemoveEvents()
	local Choice=math.random(1, 4)
		if Choice==1 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(12, 0, "Anak'Keri...")
		elseif Choice==2 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(12, 0, "My onslaught will wash over the Lich King's forces...")
		elseif Choice==3 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(12, 0, "My onslaught will wash over the Lich King's forces...")
		elseif Choice==4 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(12, 0, "Your death is in vain, tiny mortal...")			
end
end

function MalGanis_OnDied(pUnit,Event)
	-- MalGains never Die --
	pUnit:RemoveEvents()
end

function MalGanis_OnLeaveCombat(pUnit,Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(26533, 1, "MalGanis_EnterCombat")
RegisterUnitEvent(26533, 2, "MalGanis_OnLeaveCombat")
RegisterUnitEvent(26533, 3, "MalGanis_OnKill")
RegisterUnitEvent(26533, 4, "MalGanis_OnDied")