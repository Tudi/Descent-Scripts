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


	
function Prince_Keleseth_OnEnterCombat(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"Your blood is mine!")
	pUnit:RegisterEvent("PkShadowBolt",3500, 0) 
	pUnit:RegisterEvent("PkSummon",17000, 0)
	pUnit:RegisterEvent("PkFrostTomb",22000, 0)
end

function PkShadowBolt(pUnit,Event)
	pUnit:FullCastSpellOnTarget(43667,pUnit:GetMainTank())
end

function PkFrostTomb(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"Not so fast.")
	pUnit:FullCastSpellOnTarget(48400,GetRandomPlayer(0))
end


function PkSummon(pUnit,Event)
	--pUnit:PlaySoundToSet()
	pUnit:SendChatMessage(12, 0,"Aranal, lidel! Their fate shall be yours!")
	-- need coord
	--[[
	pUnit:SpawnCreature(23970 , x, y, z, o, 14, 70000)
	pUnit:SpawnCreature(23970 , x, y, z, o, 14, 70000)
	pUnit:SpawnCreature(23970 , x, y, z, o, 14, 70000)
	pUnit:SpawnCreature(23970 , x, y, z, o, 14, 70000)
	pUnit:SpawnCreature(23970 , x, y, z, o, 14, 70000)
	]]--
end 


function Prince_Keleseth_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Prince_Keleseth_OnDied(pUnit, event, player)
	pUnit:SendChatMessage(12, 0,"I join... the night.")
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23953, 1, "Prince_Keleseth_OnEnterCombat")
RegisterUnitEvent(23953, 2, "Prince_Keleseth_OnLeaveCombat")
RegisterUnitEvent(23953, 3, "Prince_Keleseth_OnDied")

-- Minon --

function Princeminon_OnEnterCombat(pUnit,Event)
		pUnit:RegisterEvent("Decrepify",10000, 0)
end

function Decrepify(pUnit,Event)
	pUnit:FullCastSpellOnTarget(42702,pUnit:GetMainTank())
end

function Princeminon_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Princeminon_OnDied(pUnit, event, player)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(23970, 1, "Princeminon_OnEnterCombat")
RegisterUnitEvent(23970, 2, "Princeminon_OnLeaveCombat")
RegisterUnitEvent(23970, 3, "Princeminon_OnDied")