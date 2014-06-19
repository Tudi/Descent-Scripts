function Keristrasza_OnEnterCombat(pUnit, event)
	pUnit:CastSpell(48094)
	pUnit:SendChatMessage(14, 0, "Preserve? Why? There's no truth in it. No no no... only in the taking! I see that now!")
	pUnit:PlaySoundToSet(13450)
	pUnit:RegisterEvent("Keristrasza_TailSweep", 30000, 0)
	pUnit:RegisterEvent("Keristrasza_CrystalChains", 20000, 0)
	pUnit:RegisterEvent("Keristrasza_CrystalfireBreath", 17500, 0)
	pUnit:RegisterEvent("Keristrasza_FrenzyCheck", 1000, 1)
end

function Keristrasza_TailSweep(pUnit, event)
	pUnit:CastSpell(50155)
end

function Keristrasza_CrystalChains(pUnit, event)
	pUnit:PlaySoundToSet(13451)
	pUnit:SendChatMessage(14, 0, "Stay. Enjoy your final moments.")
	local player = pUnit:GetRandomPlayer(7)
	if (player ~= nil) then
		pUnit:CastSpellOnTarget(50997,player)
	end
end

function Keristrasza_CrystalfireBreath(pUnit, event)
	pUnit:FullCastSpell(48096)
end

function Keristrasza_FrenzyCheck(pUnit, event)
	if pUnit:GetHealthPct() <= 25 then
		pUnit:RegisterEvent("Keristrasza_Frenzy", 1000, 1)
	else
		pUnit:RegisterEvent("Keristrasza_FrenzyCheck", 1000, 1)
	end
end

function Keristrasza_Frenzy(pUnit, event)
	pUnit:PlaySoundToSet(13452)
	pUnit:SendChatMessage(14, 0, "Finish it! Finish it! Kill me, or I swear by the Dragonqueen you'll never see daylight again!")
	pUnit:CastSpell(8599)
end

function Keristrasza_Slay(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Now we've come to the truth!")
	pUnit:PlaySoundToSet(13453)
end

function Keristrasza_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Dragonqueen... Life-Binder... preserve... me.")
	pUnit:PlaySoundToSet(13454)
	pUnit:RemoveEvents()
end

function Keristrasza_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(26723, 1, "Keristrasza_OnEnterCombat")
RegisterUnitEvent(26723, 2, "Keristrasza_OnCombatExit")
RegisterUnitEvent(26723, 3, "Keristrasza_Slay")
RegisterUnitEvent(26723, 4, "Keristrasza_Death")