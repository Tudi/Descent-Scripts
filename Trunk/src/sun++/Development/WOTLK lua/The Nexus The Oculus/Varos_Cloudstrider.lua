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
function Varoscloudstrider_OnCombat(pUnit, Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "There will be no mercy!")
	pUnit:RegisterEvent("airstrike",20000, 0)
	pUnit:RegisterEvent("EnergizeCores",7000, 0)

end	

function EnergizeCores (pUnit, Event)
	pUnit:FullCastSpellOnTarget(50785,pUnit:GetMainTank())
end

--[[Azure Ring Captain to assist him - this will cause a dragon to appear to target one of the party members with a beam of damage - this beam moves at 50% speed so whoever is targeted will need to run away from the beam. 
Also note that any blue dragons in the air can be aggroed during this boss fight - so be sure to clear the dragons that are close to the boss before engaging.     
]]--
function airstrike (pUnit, Event)
	--pUnit:SpawnCreature(28236, X, Y, Z, O, 14, 0)
	local Choice=math.random(1, 3)
		if Choice==1 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Blast them! Destroy them!")
		elseif Choice==2 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Take no prisoners! Attack!")
		elseif Choice==3 then
			--pUnit:PlaySoundToSet()
			pUnit:SendChatMessage(14, 0, "Strike now! Obliterate them! ")
		end
end	


function Varoscloudstrider_OnKilledTarget (pUnit, Event)
end

function Varoscloudstrider_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Varoscloudstrider_OnDied(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "The are... too strong! Underestimated their... fortitude.")	
	pUnit:RemoveEvents()
end

RegisterUnitEvent(29310, 1, "Varoscloudstrider_OnCombat")
RegisterUnitEvent(29310, 2, "Varoscloudstrider_OnLeaveCombat")
RegisterUnitEvent(29310, 3, "Varoscloudstrider_OnDied")
RegisterUnitEvent(29310, 4, "Varoscloudstrider_OnKilledTarget")