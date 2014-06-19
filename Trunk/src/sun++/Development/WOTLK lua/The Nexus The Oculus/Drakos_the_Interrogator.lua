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
--[[TODO:
When Drakos dies:

    * Varos Cloudstrider yells: Intruders, your victory will be short-lived. I am Commander Varos Cloudstrider. My drakes control the skies and protest this conduit. I will see to it personally that the Oculus does not fall into your hands! 
]]--
	
	
function DrakosInterrogator_OnCombat(pUnit, Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "The prisoners shall not go free. The word of Malygos is law!")
	pUnit:RegisterEvent("Magic_Pull",7000, 0) --51336--
	pUnit:RegisterEvent("ThunderingStomp",44000, 0) --50774 AOE --

end	


--Teleports all nearby enemy targets to the caster.  well I dont know its work..--
function Magic_Pull (pUnit, Event)
	pUnit:FullCastSpellOnTarget(51336,:GetRandomPlayer(4))
	local magicChoice=math.random(1, 3)
	if magicChoice==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"It is too late to run!")
	elseif magicChoice==2 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Gather 'round!")
	elseif magicChoice==3 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"None shall escape!")	
	elseif magicChoice==3 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"I condemn you to death! ")	
	end
end

function ThunderingStomp (pUnit, Event)
	pUnit:FullCastSpell(50774)
	local thundChoice=math.random(1, 3)
	if thundChoice==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"A fitting punishment!")
	elseif thundChoice==2 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"I will crush you!")
	elseif thundChoice==3 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Can you fly?")	
	end
end	

function Thundershock (pUnit, Event)
	pUnit:FullCastSpell(56926)

function DrakosInterrogator_OnKilledTarget (pUnit, Event)
	local Choice=math.random(1, 3)
	if Choice==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"A fitting punishment!")
	elseif Choice==2 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Sentence: executed!")
	elseif Choice==3 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Another casualty of war! ")		
end		
end

function DrakosInterrogator_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function DrakosInterrogator_OnDied(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "The war... goes on.")	
	pUnit:RemoveEvents()
end

RegisterUnitEvent(27654, 1, "DrakosInterrogator_OnCombat")
RegisterUnitEvent(27654, 2, "DrakosInterrogator_OnLeaveCombat")
RegisterUnitEvent(27654, 3, "DrakosInterrogator_OnDied")
RegisterUnitEvent(27654, 4, "DrakosInterrogator_OnKilledTarget")