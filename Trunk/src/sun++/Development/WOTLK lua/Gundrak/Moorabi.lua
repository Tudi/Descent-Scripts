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
function Moorabi_OnCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"We fought back the Scourge. What chance you be thinking you got?")
	pUnit:RegisterEvent("Determined_Gore",4000, 0) --55102--
	pUnit:RegisterEvent("Determined_Stab",7000, 0) --55104--
	pUnit:RegisterEvent("Mojo_Frenzy",10000, 0) --55163--
	pUnit:RegisterEvent("Numbing_Roar",20000, 0)--55100-
	pUnit:RegisterEvent("quake",15000, 0)-- ???--
	pUnit:RegisterEvent("transform",1000, 0) --55098--
end

-- I dont know what is spell ID :D   http://wotlk.wowhead.com/?search=quake#uncategorized-spells --
function quake (pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"The ground will swallow you up!"")
	--pUnit:CastSpell()
end

function Determined_Gore(pUnit,Event)
	pUnit:FullCastSpellOnTarget(55102,pUnit:GetMainTank())
end

function Determined_Stab(pUnit,Event)
	pUnit:FullCastSpellOnTarget(55104,pUnit:GetMainTank())
end

function Mojo_Frenzy(pUnit,Event)
	  pUnit:FullCastSpell(55163)
end

function Numbing_Roar(pUnit,Event)
	  pUnit:FullCastSpell(55100)
end

function transform (pUnit,Event)
 if pUnit:GetHealthPct() < 85 then
  pUnit:RemoveEvents()
  --pUnit:PlaySoundToSet()
  pUnit:SendChatMessage(12, 0,"Get ready for something much bigger!")
  pUnit:FullCastSpell(55098)
 end
end

function Moorabi_OnKilledTarget (pUnit, Event)
	local killchoice=math.random(1, 3)
	if killchoice==1 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"I crush you, cockroaches!")
	elseif killchoice==2 then	
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Who gonna stop me; you?")
	elseif killchoice==3 then
		--pUnit:PlaySoundToSet()
		pUnit:SendChatMessage(12, 0,"Not so tough now.")	
end		
end

function Moorabi_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Moorabi_OnDied(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0, "If our gods can die, then so can we..")	
	pUnit:RemoveEvents()
end

RegisterUnitEvent(29305, 1, "Moorabi_OnCombat")
RegisterUnitEvent(29305, 2, "Moorabi_OnLeaveCombat")
RegisterUnitEvent(29305, 3, "Moorabi_OnDied")
RegisterUnitEvent(29305, 4, "Moorabi_OnKilledTarget")