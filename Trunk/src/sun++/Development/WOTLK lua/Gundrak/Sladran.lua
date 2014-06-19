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
	
function Sladran_OnCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"Drakkari gonna kill anybody who trespass on these lands!")
	pUnit:RegisterEvent("poisonnova",10000, 0)
	pUnit:RegisterEvent("saldran_bite",7000, 0)
	pUnit:RegisterEvent("Venombolt",8000, 0)
	pUnit:RegisterEvent("saldran_summon",1000, 0)
end

function poisonnova(pUnit,Event)
	pUnit:FullCastSpell(55081)
end

function saldran_bite (pUnit,Event)
	pUnit:FullCastSpellOnTarget(48287,pUnit:GetMainTank())
end

function Venombolt (pUnit,Event)
	pUnit:FullCastSpellOnTarget(54970,pUnit:GetMainTank())
end


function saldran_summon (pUnit,Event)
 if pUnit:GetHealthPct() < 30 then
  pUnit:RemoveEvents()
  pUnit:SendChatMessage(12, 0,"Minions of the scale, heed my call! ")
 local x = pUnit:GetX()
 local y = pUnit:GetY()
 local z = pUnit:GetZ()
 local o = pUnit:GetO()
  -- more than  one snake summon, but not  under the boss  I will fix it :D --
 pUnit:SpawnCreature (29680 , x, y, z, o, 14, 60000)
 pUnit:SpawnCreature (29680 , x, y, z, o, 14, 60000)
 pUnit:SpawnCreature (29680 , x, y, z, o, 14, 60000)
 pUnit:RegisterEvent("saldran_summonn", 1000, 0)
 
  end
end

function saldran_summonn(pUnit,Event)
 if pUnit:GetHealthPct() < 25 then
  pUnit:RemoveEvents()
 pUnit:SendChatMessage(12, 0,"Our thousand fangs gonna rend your flesh!  ")
 local x = pUnit:GetX()
 local y = pUnit:GetY()
 local z = pUnit:GetZ()
 local o = pUnit:GetO()
 -- more than  one snake summon, but not  under the boss  I will fix it :D --
 pUnit:SpawnCreature (29713 , x, y, z, o, 14, 60000)
 pUnit:SpawnCreature (29713 , x, y, z, o, 14, 60000)
 pUnit:SpawnCreature (29713 , x, y, z, o, 14, 60000)
end
end

function Sladran_OnKilledTarget (pUnit, Event)
	local killchoice=math.random(1, 3)
	if killchoice==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Ye not breathin'! Good.")
	elseif killchoice==2 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"You scared now?")
	elseif killchoice==3 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"I'll eat you next, mon!")	
end		
end

function Sladran_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Sladran_OnDied(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "I see now... Scourge was not our greatest enemy.")	
	pUnit:RemoveEvents()
end

RegisterUnitEvent(29304, 1, "Sladran_OnCombat")
RegisterUnitEvent(29304, 2, "Sladran_OnLeaveCombat")
RegisterUnitEvent(29304, 3, "Sladran_OnDied")
RegisterUnitEvent(29304, 4, "Sladran_OnKilledTarget")