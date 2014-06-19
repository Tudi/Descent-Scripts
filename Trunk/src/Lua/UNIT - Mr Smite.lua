local phase = 1
function MrSmite_OnAggro(pUnit, Event)
	pUnit:SendChatMessage(12, 0,"We're under attack! Avast ya swabs! Repel the invaders!")
	pUnit:PlaySoundToSet(5777)
	pUnit:RegisterEvent("MrSmite_PeriodicCheck", 1000 ,0)
end

function MrSmite_PeriodicCheck(pUnit, Event)
	if (Unit:GetHealthPct() <= 66) and (phase == 1) then
		pUnit:SendChatMessage(12, 0,"You landlubbers are tougher than I thought. I'll have to improvise!")
		pUnit:PlaySoundToSet(5778)
		pUnit:CastSpellAoF(pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(),6432)
		phase = 2
	elseif (Unit:GetHealthPct() <= 33) and (phase == 2) then
		pUnit:SendChatMessage(12, 0,"D'ah! Now you're making me angry!")
		pUnit:PlaySoundToSet(5779)
		pUnit:CastSpellAoF(pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(),6432)
		phase = 3
	end
end

function MrSmite_OnDeath(pUnit, Event)
	pUnit:RemoveEvents()
end

function MrSmite_LeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(646, 1, "MrSmite_OnAggro")
RegisterUnitEvent(646, 2, "MrSmite_LeaveCombat")
RegisterUnitEvent(646, 4, "MrSmite_OnDeath")