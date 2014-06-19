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
--Recon from Hungary :)

--[[Recon don't forget update this script :) ]]
-- Herald Volazj is incapable of uttering actual words. Instead, he makes sounds that can be understood by very few creatures. However, players also receive whispers, translating what Volazj "says".  --

function HeraldVolazj_OnCombat(pUnit, Event)
	print "Herald Volazj"
	pUnit:RegisterEvent("Insanity",10000,0)
	pUnit:RegisterEvent("Mindflay",20000,0)

end

-- I dont know what this :S --
function Insanity (pUnit, Event)
	--pUnit:FullCastSpellOnTarget(57496,pUnit:GetRandomPlayer(0))
end

function Mindflay (pUnit, Event)
	pUnit:FullCastSpellOnTarget(52586,pUnit:GetMainTank())
end

function HeraldVolazj_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function HeraldVolazj_OnDied(pUnit, Event)
	--pUnit:PlaySoundToSet()	
	pUnit:RemoveEvents()
end

RegisterpUnitEvent(29311, 1, "HeraldVolazj_OnCombat")
RegisterpUnitEvent(29311, 2, "HeraldVolazj_OnLeaveCombat")
RegisterpUnitEvent(29311, ,, "HeraldVolazj_OnDied")