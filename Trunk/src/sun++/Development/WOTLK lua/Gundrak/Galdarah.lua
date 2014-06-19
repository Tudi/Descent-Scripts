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
	
function Galdarah_OnEnterCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
    pUnit:SendChatMessage(12, 0,"I'm gonna spill your guts, mon!")
	pUnit:RegisterEvent("summon",15000, 0)
	pUnit:RegisterEvent("Puncture",15000, 0) --55276--
	pUnit:RegisterEvent("Whirling_Slash",15000, 0) --55250--
    pUnit:RegisterEvent("Transform",100, 0)
end

function Puncture (pUnit,Event)
	pUnit:FullCastSpellOnTarget(55276,pUnit:GetMainTank())
end

function Whirling_Slash (pUnit,Event)
	pUnit:FullCastSpellOnTarget(55250,pUnit:GetMainTank())
end


function summon(pUnit,Event)
	pUnit:FullCastSpell(55218)
	local esely=math.random(1, 3)
		if esely==1 then
		--pUnit:PlaySoundToSet()
        pUnit:SendChatMessage(12, 0,"Gut them! Impale them!")
		elseif esely==2 then    
		--pUnit:PlaySoundToSet()
        pUnit:SendChatMessage(12, 0,"Kill them all!")    
		elseif esely==3 then    
		--pUnit:PlaySoundToSet()
        pUnit:SendChatMessage(12, 0, "Say hello to my big friend!")
		end
    
end

function Transform(pUnit, Event)
	if pUnit:GetHealthPct() < 60 then
	    pUnit:SetModel(26265)     --  displayid change.. 
    local choiceee=math.random(1, 2)
		if choiceee==1 then
		--pUnit:PlaySoundToSet()
        pUnit:SendChatMessage(12, 0,"Ain't gonna be nottin' left after this!")
		elseif choiceee==2 then    
		--pUnit:PlaySoundToSet()
        pUnit:SendChatMessage(12, 0,"You wanna see power? I'm gonna show you power!")    
		end
	pUnit:RegisterEvent("Stomp",7000, 0)
	pUnit:RegisterEvent("Enrage",18000, 0)
	pUnit:RegisterEvent("charge",12000, 0)--54956--
	
	end

end

function charge(pUnit,Event)
	pUnit:FullCastSpellOnTarget(54956,pUnit:GetMainTank())
end

function Enrage(pUnit,Event)
    pUnit:FullCastSpell(55285)
end

function Stomp(pUnit,Event)
    pUnit:FullCastSpell(55292)
end


function Galdarah_OnKilledTarget (pUnit, Event)
    local Krikthichoice=math.random(1, 3)
    if Krikthichoice==1 then
		--pUnit:PlaySoundToSet()
        pUnit:SendChatMessage(12, 0,"What a rush!")
    elseif Krikthichoice==2 then  
		--pUnit:PlaySoundToSet()
        pUnit:SendChatMessage(12, 0,"Who needs gods, when WE ARE GODS!")    
    elseif Krikthichoice==3 then 
		--pUnit:PlaySoundToSet()
        pUnit:SendChatMessage(12, 0,"I told ya so! ")
end        
end

function Galdarah_OnLeaveCombat(pUnit, Event)
    pUnit:RemoveEvents()    
end

function Galdarah_OnDied(pUnit, event, player)
	--pUnit:PlaySoundToSet()
    pUnit:SendChatMessage(12, 0,"Even the mighty... can fall...")
    pUnit:RemoveEvents()
end

RegisterUnitEvent(29306, 1, "Galdarah_OnEnterCombat")
RegisterUnitEvent(29306, 2, "Galdarah_OnLeaveCombat")
RegisterUnitEvent(29306, 3, "Galdarah_OnDied")
RegisterUnitEvent(29306, 4, "Galdarah_OnKilledTarget")