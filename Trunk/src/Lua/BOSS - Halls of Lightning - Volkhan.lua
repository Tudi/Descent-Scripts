function Volkhan_OnEnterCombat(pUnit, event)
	pUnit:SendChatMessage(14, 0, "It is you who have destroyed my children? You... shall... pay!")
	pUnit:PlaySoundToSet(13960)
	pUnit:RegisterEvent("Volkhan_Temper", 1000, 1)
	pUnit:RegisterEvent("Volkhan_ShatteringStomp", 30000, 0)
end

function Volkhan_Temper(pUnit, event)
	local health = pUnit:GetHealthPct()
	if (health == 80 or health == 60 or health == 40 or health == 20) then
		local rand = math.random(1,2)
		if rand == 1 then
			pUnit:SendChatMessage(14, 0, "Life from the lifelessness... death for you.")
			pUnit:PlaySoundToSet(13961)
		elseif rand == 2 then
			pUnit:SendChatMessage(14, 0, "Nothing is wasted in the process. You will see...")
			pUnit:PlaySoundToSet(13962)
		end
		pUnit:MoveTo(1326.148, -91.17, 56.72, 5.114)
		pUnit:RegisterEvent("Volkhan_TemperCast", 2000, 1)
		pUnit:RegisterEvent("Volkhan_Temper", 10000, 1)
	else
		pUnit:RegisterEvent("Volkhan_Temper", 1000, 1)
	end
end

function Volkhan_TemperCast(pUnit, event)
	pUnit:FullCastSpell(52238)
end

function Volkhan_ShatteringStomp(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "I will crush you beneath my boots!")
		pUnit:PlaySoundToSet(13963)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "All my work... undone!")
		pUnit:PlaySoundToSet(13964)
	end
	pUnit:FullCastSpell(52237)
end

function Volkhan_Slay(pUnit, event)
	local rand = math.random(1,3)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "The armies of iron will conquer all!")
		pUnit:PlaySoundToSet(13965)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Ha, pathetic!")
		pUnit:PlaySoundToSet(13966)
	elseif rand == 3 then
		pUnit:SendChatMessage(14, 0, "You have cost me too much work!")
		pUnit:PlaySoundToSet(13967)
	end
end

function Volkhan_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "The master was right... to be concerned.")
	pUnit:PlaySoundToSet(13968)
	pUnit:RemoveEvents()
end

function Volkhan_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(28587, 1, "Volkhan_OnEnterCombat")
RegisterUnitEvent(28587, 2, "Volkhan_OnCombatExit")
RegisterUnitEvent(28587, 3, "Volkhan_Slay")
RegisterUnitEvent(28587, 4, "Volkhan_Death")

