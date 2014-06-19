function Rotface_OnEnterCombat(pUnit, event)
	pUnit:PlaySoundToSet(16986)
	pUnit:RegisterEvent("Rotface_OozeFlood", 30000, 0)
	pUnit:RegisterEvent("Rotface_SlimeSpray", 21000, 0)
	pUnit:RegisterEvent("Rotface_MutatedInfection", 24000, 0)
end

function Rotface_OozeFlood(pUnit, event)
	pUnit:CastSpell(71215)
end

function Rotface_SlimeSpray(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Icky sticky.")
	pUnit:PlaySoundToSet(16991)
	pUnit:FullCastSpellOnTarget(69508, pUnit:GetRandomPlayer(0))
end

function Rotface_MutatedInfection(pUnit, event)
	pUnit:CastSpellOnTarget(71224, pUnit:GetRandomPlayer(0))
	-- TODO: Spawn!
end

function Rotface_Slay(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Daddy make toys out of you!")
		pUnit:PlaySoundToSet(16988)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "I brokes-ded it...")
		pUnit:PlaySoundToSet(16987)
	end
end

function Rotface_Death(pUnit, event, player)
	pUnit:SendChatMessage(14, 0, "Bad news daddy.")
	pUnit:PlaySoundToSet(16989)
	player:AdvanceQuestObjective(24547, 0)
	pUnit:RemoveEvents()
	pUnit:RegisterEvent("Rotface_Death1", 3000, 0)
end

function Rotface_Death1(pUnit,event)
	local Putricide = pUnit:GetCreatureNearestCoords(pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(),36678)
	Putricide:SendChatMessage(14, 0, "Terrible news, everyone, Rotface is dead! But great news everyone, he left behind plenty of ooze for me to use! Whaa...? I'm a poet, and I didn't know it? Astounding!")
	pUnit:PlaySoundToSet(17146)
end

function Rotface_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(36627, 1, "Rotface_OnEnterCombat")
RegisterUnitEvent(36627, 2, "Rotface_OnCombatExit")
RegisterUnitEvent(36627, 3, "Rotface_Slay")
RegisterUnitEvent(36627, 4, "Rotface_Death")