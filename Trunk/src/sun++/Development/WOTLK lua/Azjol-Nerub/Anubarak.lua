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
Made by: AzoleX
-- ]]
--[[ To-Do:
*Add add's AI
*Test it with Wotlk
]]
-- MISSING SOME ADDS
-- Defines
local UNIT_FLAG_NOT_SELECTABLE = 0x02000000
local UNIT_FLAG_NOT_ATTACKABLE_9           = 0x00000100
local anubaraka = 29120
local assasian = 29214
local darter = 29213

-- Registers
RegisterUnitEvent(anubaraka, 1, "Anubarak_OnEnterCombat")
RegisterUnitEvent(anubaraka, 2, "Anubarak_OnLeaveCombat")
RegisterUnitEvent(anubaraka, 4, "Anubarak_OnDied")
RegisterUnitEvent(anubaraka, 3, "Anubarak_OnKilledTarget")

-- Script
function Anubarak_OnEnterCombat(pUnit,Event)
	pUnit:SendChatMessage(14, 0,"Eternal agony awaits you!")
	pUnit:RegisterEvent("Anubarak_Swarm",8000, 0)
	pUnit:RegisterEvent("Anubarak_CarrionBeetlesSummon",6000, 0).
	pUnit:RegisterEvent("Anubarak_SuperSpell",6000, 0)
	if pUnit:GetHealthPct() < 66 then
	pUnit:RegisterEvent("Anubarak_Phazo1",1,1)
	if pUnit:GetHealthPct() < 33 then
	pUnit:RegisterEvent("Anubarak_Phazo2",1,1)
	if pUnit:GetHealthPct() < 15 then
	pUnit:RegisterEvent("Anubarak_Phazo3",1,1)
	end
end
end
end


function Anubarak_Swarm(pUnit, Event)
	pUnit:FullCastSpellOnTarget(53467,pUnit:GetRandomPlayer(0))
	local anubarakchoice=math.random(1, 2)
	if killchoice==1 then
		pUnit:SendChatMessage(14, 0,"Your armor is useless against my locusts!")
	elseif killchoice==2 then	
		pUnit:SendChatMessage(14, 0,"The pestilence upon you!")	
end
end

function Anubarak_CarrionBeetlesSummon(pUnit,Event)
	pUnit:FullCastSpell(53520)
end

function Anubarak_Swarm(pUnit, Event)
	Unit:FullCastSpell(53454)
end
	
function Anubarak_OnKilledTarget (pUnit, Event)
	local anubarakchoice=math.random(1, 3)
	if Krikthichoice==1 then
		pUnit:SendChatMessage(14, 0,"You shall experience my torment, first-hand!")
	elseif Krikthichoice==2 then	
		pUnit:SendChatMessage(14, 0,"You have made your choice.")	
	elseif Krikthichoice==3 then	
		pUnit:SendChatMessage(14, 0,"Soon, the Master's voice will call to you.")
end		
end

function Anubarak_Phazo1(pUnit,Event)
	pUnit:Root()
	pUnit:SetScale(0)
	local plr = pUnit:GetRandomPlayer(0)
	if type(plr) == "userdata" then
	pUnit:SpawnCreature(assasian , plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 14, 0)
	pUnit:SpawnCreature(assasian , plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 14, 0) 
	pUnit:SpawnCreature(darter , plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 14, 0) 
	pUnit:SpawnCreature(darter , plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 14, 0) 	
	pUnit:SendChatMessage(14, 0,"Come forth, my brethren. Feast on their flesh")
	pUnit:RegisterEvent("Anubarak_RemovePhazo1",10000, 1)
	pUnit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9)
	pUnit:FullCastSpell(53472) -- Core script, will add patch
end

function Anubarak_RemovePhazo1(pUnit,Event)
	pUnit:RemoveEvents()
	pUnit:RegisterEvent("Anubarak_Swarm",8000, 0)
	pUnit:RegisterEvent("Anubarak_CarrionBeetlesSummon",6000, 0)
end

function Anubarak_Phazo2(pUnit,Event)
	pUnit:Root()
	pUnit:SetScale(0)
	local plr = pUnit:GetRandomPlayer(0)
	if type(plr) == "userdata" then
	pUnit:SpawnCreature(assasian , plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 14, 0)
	pUnit:SpawnCreature(assasian , plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 14, 0) 
	pUnit:SpawnCreature(darter , plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 14, 0) 
	pUnit:SpawnCreature(darter , plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 14, 0) 
	pUnit:SendChatMessage(14, 0,"Come forth, my brethren. Feast on their flesh")
	pUnit:RegisterEvent("Anubarak_RemovePhazo2",10000, 1)
	pUnit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9)
	pUnit:FullCastSpell(53472) -- Core script, will add patch
end

function Anubarak_RemovePhazo2(pUnit,Event)
	pUnit:RemoveEvents()
	pUnit:RegisterEvent("Anubarak_Swarm",8000, 0)
	pUnit:RegisterEvent("Anubarak_CarrionBeetlesSummon",6000, 0)
end

function Anubarak_Phazo3(pUnit,Event)
	pUnit:Root()
	pUnit:SetScale(0)
	local plr = pUnit:GetRandomPlayer(0)
	if type(plr) == "userdata" then
	pUnit:SpawnCreature(assasian , plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 14, 0)
	pUnit:SpawnCreature(assasian , plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 14, 0) 
	pUnit:SpawnCreature(darter , plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 14, 0) 
	pUnit:SpawnCreature(darter , plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 14, 0) 	
	pUnit:SendChatMessage(14, 0,"Come forth, my brethren. Feast on their flesh")
	pUnit:RegisterEvent("Anubarak_RemovePhazo3",10000, 1)
	pUnit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9)
	pUnit:FullCastSpell(53472) -- Core script, will add patch
end

function Anubarak_RemovePhazo3(pUnit,Event)
	pUnit:RemoveEvents()
	pUnit:RegisterEvent("Anubarak_Swarm",8000, 0)
	pUnit:RegisterEvent("Anubarak_CarrionBeetlesSummon",6000, 0)
end

function Anubarak_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Anubarak_OnDied(pUnit, event, player)
	pUnit:SendChatMessage(14, 0,"I never thought... I would be rid of him.")
	pUnit:RemoveEvents()
end
