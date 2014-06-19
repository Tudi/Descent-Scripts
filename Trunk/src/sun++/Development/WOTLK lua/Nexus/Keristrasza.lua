-- NOT SURE ABOUT ENRAGE BUT ANYWAY IT DOES IT'S JOB --  
local Keristrasza = 26723

--REGISTERS
RegisterUnitEvent(Keristrasza, 1, "Keristrasza_OnCombat")
RegisterUnitEvent(Keristrasza, 2, "Keristrasza_OnLeaveCombat")
RegisterUnitEvent(Keristrasza, 3, "Keristrasza_OnKillTarget")
RegisterUnitEvent(Keristrasza, 4, "Keristrasza_OnDeath")

function Keristrasza_OnEnterCombat(pUnit, Event)
	pUnit:SendChatMessage(14, 0,"Preserve? Why? There's no truth in it. No no no... only in the taking! I see that now!")
	pUnit:RegisterEvent("Keristrasza_CrystalChains", math.random(12000, 28000), 0)
	pUnit:RegisterEvent("Keristrasza_TailSweep", math.random(10000, 16000), 0)
	pUnit:RegisterEvent("Keristrasza_CrystalfireBreath", math.random(10000, 16000), 0)
	pUnit:RegisterEvent("Keristrasza_IntenseCold", 3000, 0)
	if pUnit:GetHealthPct() < 10 then
	pUnit:RegisterEvent("Keristrasza_Enrage",1000,1)
end
end

function Keristrasza_TailSweep(pUnit, Event) 
	pUnit:FullCastSpell(50155)
end
function Keristrasza_CrystalChains(pUnit, Event) 
	local plr = pUnit:GetMainTank()
	if type(plr) == "userdata" then
	pUnit:FullCastSpellOnTarget(50155, plr)
	pUnit:SendChatMessage(14, 0,"Stay. Enjoy your final moments.")
end
end
function Keristrasza_Enrage(pUnit, Event) 
	pUnit:FullCastSpell(8599)
	pUnit:SendChatMessage(14, 0,"Finish it! Finish it! Kill me, or I swear by the Dragonqueen you'll never see daylight again!")
end
function Keristrasza_CrystalfireBreath(pUnit, Event) 
	pUnit:FullCastSpell(48096)
end
function Keristrasza_IntenseCold(pUnit, Event) 
	pUnit:FullCastSpell(48094)
end

function Keristrasza_OnLeaveCombat(pUnit, Event)
pUnit:RemoveEvents()
end
function Keristrasza_OnDied(pUnit, event, player)
	pUnit:SendChatMessage(14, 0,"Dragonqueen... Life-Binder... preserve... me.")
	pUnit:RemoveEvents()
end
function Keristrasza_OnKilledTarget (pUnit, Event)
	pUnit:SendChatMessage(14, 0,"Now we've come to the truth!")

end		
