function OrmoroktheTreeShaper_OnEnterCombat(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Noo!")
	pUnit:PlaySoundToSet(13328)
	pUnit:RegisterEvent("OrmoroktheTreeShaper_Reflection", 30000, 0)
	pUnit:RegisterEvent("OrmoroktheTreeShaper_Trample", 20000, 0)
	pUnit:RegisterEvent("OrmoroktheTreeShaper_CrystalSpike", 17500, 0)
	pUnit:RegisterEvent("OrmoroktheTreeShaper_FrenzyCheck", 1000, 1)
end

function OrmoroktheTreeShaper_Reflection(pUnit, event)
	pUnit:PlaySoundToSet(13331)
	pUnit:SendChatMessage(14, 0, "Such a pity.")
	pUnit:FullCastSpell(47981)
end

function OrmoroktheTreeShaper_Trample(pUnit, event)
	pUnit:CastSpell(48016)
end

function OrmoroktheTreeShaper_CrystalSpike(pUnit, event)
	pUnit:PlaySoundToSet(13332)
	pUnit:SendChatMessage(14, 0, "Bleed!")
	pUnit:CastSpell(47958)
end

function OrmoroktheTreeShaper_FrenzyCheck(pUnit, event)
	if pUnit:GetHealthPct() <= 25 then
		pUnit:RegisterEvent("OrmoroktheTreeShaper_Frenzy", 1000, 1)
	else
		pUnit:RegisterEvent("OrmoroktheTreeShaper_FrenzyCheck", 1000, 1)
	end
end

function OrmoroktheTreeShaper_Frenzy(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Back!")
	pUnit:CastSpell(57086)
end

function OrmoroktheTreeShaper_Slay(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Such a pity.")
	pUnit:PlaySoundToSet(13329)
end

function OrmoroktheTreeShaper_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Aaggh!")
	pUnit:PlaySoundToSet(13330)
	pUnit:RemoveEvents()
end

function OrmoroktheTreeShaper_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(26794, 1, "OrmoroktheTreeShaper_OnEnterCombat")
RegisterUnitEvent(26794, 2, "OrmoroktheTreeShaper_OnCombatExit")
RegisterUnitEvent(26794, 3, "OrmoroktheTreeShaper_Slay")
RegisterUnitEvent(26794, 4, "OrmoroktheTreeShaper_Death")