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

function Drakkari_colossus_OnEnterCombat(pUnit,Event)
	pUnit:RegisterEvent("MightyBlow",5000, 0)
	pUnit:RegisterEvent("Emerge_summon",30000, 0)
end

function MightyBlow (pUnit,Event)
	pUnit:FullCastSpellOnTarget(54719,pUnit:GetMainTank())
end

function Emerge_summon(pUnit,Event)
	pUnit:RemoveEvents()
	pUnit:Root()
	pUnit::RemoveAllAuras()
	pUnit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9)
	pUnit:FullCastSpell(54851)
	pUnit:RegisterEvent("removesum",30000, 0)
end

function removesum (pUnit,Event)
	 pUnit:RemoveEvents()
	 pUnit:RegisterEvent("MightyBlow",5000, 0)
end


function Drakkari_colossus_OnKilledTarget (pUnit, Event)
end

function Drakkari_colossus_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Drakkari_colossus_OnDied(pUnit, event, player)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(29307, 1, "Drakkari_colossus_OnEnterCombat")
RegisterUnitEvent(29307, 2, "Drakkari_colossus_OnLeaveCombat")
RegisterUnitEvent(29307, 3, "Drakkari_colossus_OnDied")
RegisterUnitEvent(29307, 4, "Drakkari_colossus_OnKilledTarget")