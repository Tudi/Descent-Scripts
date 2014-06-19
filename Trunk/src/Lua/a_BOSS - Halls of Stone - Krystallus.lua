function Krystallus_OnEnterCombat(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Crush....")
	pUnit:PlaySoundToSet(14176)
	pUnit:RegisterEvent("Krystallus_BoulderToss", 7000, 0)
	pUnit:RegisterEvent("Krystallus_Stomp", 10000, 0)
	pUnit:RegisterEvent("Krystallus_GroundSlam", 25000, 0)
end

function Krystallus_BoulderToss(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if player ~= nil then
		pUnit:FullCastSpellOnTarget(50843,player)
	end
end

function Krystallus_Stomp(pUnit, event)
	pUnit:CastSpell(50868)
end

function Krystallus_GroundSlam(pUnit, event)
	pUnit:FullCastSpell(50827)
	pUnit:RegisterEvent("Krystallus_Shatter", 1000, 1)
end

function Krystallus_Shatter(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Break.... you....")
	pUnit:PlaySoundToSet(14178)
	pUnit:FullCastSpell(50810)
end

function Krystallus_Slay(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Ha...ha...ha...ha...")
	pUnit:PlaySoundToSet(14177)
end

function Krystallus_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Uuuuhhhhhhhhhh......")
	pUnit:PlaySoundToSet(14179)
	pUnit:RemoveEvents()
end

function Krystallus_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(27977, 1, "Krystallus_OnEnterCombat")
RegisterUnitEvent(27977, 2, "Krystallus_OnCombatExit")
RegisterUnitEvent(27977, 3, "Krystallus_Slay")
RegisterUnitEvent(27977, 4, "Krystallus_Death")