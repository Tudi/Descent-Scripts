--[[
		CRYSTAL SPIKES MISSING
						]]
local Ormok = 26794

--REGISTERS
RegisterUnitEvent(Ormok, 1, "Ormok_OnEnterCombat")
RegisterUnitEvent(Ormok, 2, "Ormok_OnLeaveCombat")
RegisterUnitEvent(Ormok, 4, "Ormok_OnDied")

function Ormorok_OnEnterCombat(pUnit, Event)
	pUnit:SendChatMessage(14, 0,"Noo!")
	pUnit:RegisterEvent("Ormorok_Trample", 10000, 0)
	pUnit:RegisterEvent("Ormorok_SpellReflection", 12500, 0)
	if pUnit:GetHealthPct() < 25 then
	pUnit:RegisterEvent("Ormorok_Frenzy",1000,1)
end
end

function Ormorok_OnDied(pUnit, event, player)
	pUnit:SendChatMessage(14, 0,"Aaggh!")
	pUnit:RemoveEvents()
end	
function Ormorok_OnLeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()	
end

function Ormorok_Frenzy(pUnit, Event)
	pUnit:CastSpell(48017)
	pUnit:SendChatMessage(14, 0,"Back!")
end
function Ormorok_Trample(pUnit, Event)
	pUnit:CastSpell(48016)
end
function Ormorok_SpellReflection(pUnit, Event)
	pUnit:CastSpell(47981)
end	

	