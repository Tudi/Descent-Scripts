function Tosamina_onSpawn(Unit, event, player)
	Unit:RegisterEvent("Tosamina_Say",40000, 0)
end

function Tosamina_Say(Unit, event, player)
	local rand = math.random(1,4)
	if rand == 1 then
		Unit:SendChatMessage(12, 0, "You are mine now children. You will grow up to be a strong of the horde!")
	elseif rand == 2 then
		Unit:SendChatMessage(12, 0, "Clean up this mess! We will not live in such squalor!")
	elseif rand == 3 then
		Unit:SendChatMessage(12, 0, "It's my way or the Gold Road. Is that understood?")
	elseif rand == 4 then
		Unit:SendChatMessage(12, 0, "Stop whining!")
	end
end

function Tosamina_Died(Unit, event, player)
	Unit:RemoveEvents()
end

function thrall_OnCombat(pUnit, Event)
	pUnit:PlaySoundToSet(5880)
end

RegisterUnitEvent(4949, 1, "thrall_OnCombat")
RegisterUnitEvent(14498, 18, "Tosamina_onSpawn")
RegisterUnitEvent(14498, 4, "Tosamina_Died")