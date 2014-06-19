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


function ingvar_OnEnterCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"I'll paint my face with your blood!")
	pUnit:RegisterEvent("Cleave",3500, 0) --42724--
	pUnit:RegisterEvent("Enrage",19500, 0) --42705--
	pUnit:RegisterEvent("smash",7100, 0) --42669--
	pUnit:RegisterEvent("Staggering_Roar",12500, 0) --42708--
	pUnit:RegisterEvent("phase_two", 20, 0)
end

function Cleave(pUnit,Event)
	pUnit:FullCastSpellOnTarget(42724,pUnit:GetMainTank())
end

function Enrage(pUnit,Event)
	pUnit:FullCastSpell(42705)
end

function smash(pUnit,Event)
	pUnit:FullCastSpellOnTarget(42669,pUnit:GetMainTank())
end

function Staggering_Roar(pUnit,Event)
	pUnit:FullCastSpell(42708)
end



function phase_two(pUnit,Event)
	if pUnit:GetHealthPct() < 2 then	
	pUnit:RegisterEvent("death", 1, 1)
	pUnit:RegisterEvent("DarkSmash",3500, 0) --42723--
	pUnit:RegisterEvent("DreadfulRoar",5500, 0) --42729--
	pUnit:RegisterEvent("WoeStrike",10500, 0) --42730--
	pUnit:RegisterEvent("darkaxe",15500, 0) --42730--
	end
end 

function death(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"My life for the... death god")
	pUnit::RemoveAllAuras() --  debuff remove --
	pUnit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9) -- Dont attack --
	sleep(3000)  -- I hope this working
	pUnit:FullCastSpell(42795) --Feign death :) 
	sleep(2000)  -- I hope this working  i dont know
	--Annhylde the Caller  rescuret   need  coord :) ---
	--pUnit:SpawnCreature(24068,x, y, z, o, 35, 15000)
	pUnit:FullCastSpell(42796) --Transform--
	sleep(2000)
	
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"I return! A second chance to carve out your skull")
	pUnit::GetMaxHealth()  -- max hp :)
end

function darkaxe(pUnit,Event)
	pUnit:FullCastSpellOnTarget(42748pUnit:GetRandomPlayer(0))
end
	
function DarkSmash(pUnit,Event)
	pUnit:FullCastSpellOnTarget(42723,pUnit:GetMainTank())
end

function WoeStrike(pUnit,Event)
	pUnit:FullCastSpellOnTarget(42730,pUnit:GetMainTank())
end


function DreadfulRoar(pUnit,Event)  -- Its AOE? --
	pUnit:FullCastSpell(42729)
end



function ingvar_OnKill(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"Mjul orm agn gjor!")
end

function ingvar_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function ingvar_OnDied(pUnit, event, player)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"No! I can do... better! I can..")
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23980, 1, "ingvar_OnEnterCombat")
RegisterUnitEvent(23980, 2, "ingvar_OnLeaveCombat")
RegisterUnitEvent(23980, 3, "ingvar_OnDied")
RegisterUnitEvent(23980, 4, "ingvar_OnKill")


--AnnhyldetheCaller  monolog--

function AnnhyldetheCaller (pUnit,Event)
	pUnit:RegisterEvent("AnnhyldetheCallersay", 1000, 1)
end

function AnnhyldetheCallersay (pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"Ingvar! Your pathetic failure will serve as a warning to all... you are damned! Arise and carry out the master's will!")
end

RegisterUnitEvent(24068, 1, "AnnhyldetheCaller")
