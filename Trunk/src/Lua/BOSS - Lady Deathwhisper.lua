function LadyDeathwhisper_OnEnterCombat(pUnit, event)
	pUnit:Root()
	pUnit:SendChatMessage(14, 0, "What is this disturbance?! You dare trespass upon this hallowed ground? This shall be your final resting place.")
	pUnit:PlaySoundToSet(16868)
	pUnit:RegisterEvent("LadyDeathwhisper_PhaseOne", 1000, 1)
end

function LadyDeathwhisper_PhaseOne(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:CastSpell(70842)
	pUnit:RegisterEvent("LadyDeathwhisper_ShadowBolt", 3000, 0)
	pUnit:RegisterEvent("LadyDeathwhisper_MindControl", 40000, 0)
	pUnit:RegisterEvent("LadyDeathwhisper_SpawnAddsPhase1", 45000, 0)
	pUnit:RegisterEvent("LadyDeathwhisper_DeathAndDecay", 15000, 0)
	pUnit:RegisterEvent("LadyDeathwhisper_DarkEmpowerment", 45000, 0)
	pUnit:RegisterEvent("LadyDeathwhisper_PhaseTwoCheck", 1000, 0)
end

function LadyDeathwhisper_PhaseTwoCheck(pUnit, event)
	if pUnit:GetManaPct() <= 5 then
		pUnit:RegisterEvent("LadyDeathwhisper_PhaseTwo", 1000, 1)
	end
end

function LadyDeathwhisper_PhaseTwo(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:SetCombatMeleeCapable(1)
	pUnit:SendChatMessage(14, 0, "Enough! I see I must take matters into my own hands!")
	pUnit:PlaySoundToSet(16877)
	pUnit:RemoveAura(70842)
	pUnit:Unroot()
	pUnit:RegisterEvent("LadyDeathwhisper_FrostBolt", 10000, 0)
	pUnit:RegisterEvent("LadyDeathwhisper_TouchofInsignificance", 30000, 0)
	pUnit:RegisterEvent("LadyDeathwhisper_MindControl", 40000, 0)
	pUnit:RegisterEvent("LadyDeathwhisper_DeathAndDecay", 15000, 0)
	pUnit:RegisterEvent("LadyDeathwhisper_SpawnAddsPhase2", 40000, 0)
end

function LadyDeathwhisper_ShadowBolt(pUnit, event)
	pUnit:CastSpellOnTarget(72008, pUnit:GetRandomPlayer(0))
end

function LadyDeathwhisper_MindControl(pUnit, event)
	pUnit:SendChatMessage(14, 0, "You are weak, powerless to resist my will!")
	pUnit:PlaySoundToSet(16876)
	pUnit:CastSpellOnTarget(71289, pUnit:GetRandomPlayer(7))
end

function LadyDeathwhisper_DeathAndDecay(pUnit, event)
	local target = pUnit:GetRandomPlayer(0)
	pUnit:CastSpellAoF(target:GetX(), target:GetY(), target:GetZ(), 72109)
end

function LadyDeathwhisper_DarkEmpowerment(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Take this blessing and show these intruders a taste of our master's power.")
	pUnit:PlaySoundToSet(16873)
	local friend = pUnit:GetRandomFriend()
	Unit:FullCastSpellOnTarget(70901, friend)
end

function LadyDeathwhisper_SpawnAddsPhase1(pUnit, event)
	pUnit:SpawnCreature(37949, -578.707, 2154.17, 51.032, 0, pUnit:GetFaction(), 0) 
	pUnit:SpawnCreature(37890, -619.286, 2154.46, 51.032, 0, pUnit:GetFaction(), 0) 
	pUnit:SpawnCreature(37949, -598.903, 2155.01, 51.032, 0, pUnit:GetFaction(), 0) 

	pUnit:SpawnCreature(37890, -619.432, 2268.52, 51.032, 0, pUnit:GetFaction(), 0) 
	pUnit:SpawnCreature(37949, -598.969, 2269.26, 51.032, 0, pUnit:GetFaction(), 0) 
	pUnit:SpawnCreature(37890, -578.7, 2269.86, 51.032, 0, pUnit:GetFaction(), 0) 
end

function LadyDeathwhisper_SpawnAddsPhase2(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Arise and exalt in your pure form!")
	pUnit:PlaySoundToSet(16875)
	local rand = math.random(1,6)
	if rand == 1 then
		pUnit:SpawnCreature(38222, -578.707, 2154.17, 51.032, 0, pUnit:GetFaction(), 60000) 
	elseif rand == 2 then
		pUnit:SpawnCreature(38222, -619.286, 2154.46, 51.032, 0, pUnit:GetFaction(), 60000) 
	elseif rand == 3 then
		pUnit:SpawnCreature(38222, -598.903, 2155.01, 51.032, 0, pUnit:GetFaction(), 60000) 
	elseif rand == 4 then
		pUnit:SpawnCreature(38222, -619.432, 2268.52, 51.032, 0, pUnit:GetFaction(), 60000) 
	elseif rand == 5 then
		pUnit:SpawnCreature(38222, -598.969, 2269.26, 51.032, 0, pUnit:GetFaction(), 60000)
	elseif rand == 6 then
		pUnit:SpawnCreature(38222, -578.700, 2269.86, 51.032, 0, pUnit:GetFaction(), 60000) 
	end
end

function LadyDeathwhisper_FrostBolt(pUnit, event)
	local target = pUnit:GetMainTank()
	pUnit:FullCastSpellOnTarget(72501, target)
end

function LadyDeathwhisper_TouchofInsignificance(pUnit, event)
	local target = pUnit:GetMainTank()
	pUnit:CastSpellOnTarget(71204, target)
end

function LadyDeathwhisper_Slay(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Do you yet grasp of the futility of your actions?!")
		pUnit:PlaySoundToSet(16869)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Embrace the darkness... Darkness eternal!")
		pUnit:PlaySoundToSet(16870)
	end
end

function LadyDeathwhisper_Death(pUnit, event, player)
	pUnit:SendChatMessage(14, 0, "All part of the masters plan! Your end is... inevitable!")
	pUnit:PlaySoundToSet(16871)
	player:AdvanceQuestObjective(24547, 0)
	pUnit:RemoveEvents()
end

function LadyDeathwhisper_OnCombatExit(pUnit, event)
	pUnit:Unroot()
	pUnit:RemoveEvents()
end

function LadyDeathwhisper_Spawn(pUnit, event)
	pUnit:Root()
	pUnit:SetCombatMeleeCapable(0)
	pUnit:RegisterEvent("LadyDeathwhisper_Spawn1A", 500, 1)
	pUnit:RegisterEvent("LadyDeathwhisper_Spawn1B", 10500, 1)
	pUnit:RegisterEvent("LadyDeathwhisper_Spawn1C", 17000, 1)

	pUnit:RegisterEvent("LadyDeathwhisper_Spawn2A", 30000, 1)
	pUnit:RegisterEvent("LadyDeathwhisper_Spawn2B", 40000, 1)

	pUnit:RegisterEvent("LadyDeathwhisper_Spawn3A", 60000, 1)
	pUnit:RegisterEvent("LadyDeathwhisper_Spawn3B", 70000, 1)
end

function LadyDeathwhisper_Spawn1A(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Fix your eyes upon your crude hands! The sinew, the soft meat, the dark blood coursing within!")
	pUnit:PlaySoundToSet(16878)
end

function LadyDeathwhisper_Spawn1B(pUnit, event)
	pUnit:SendChatMessage(14, 0, "It is a weakness! A crippling flaw! A joke played by the creators upon their own creations!")
	pUnit:PlaySoundToSet(17268)
end

function LadyDeathwhisper_Spawn1C(pUnit, event)
	pUnit:SendChatMessage(14, 0, "The sooner you come to accept your condition as a defect, the sooner you will find yourselves in a position to transcend it!")
	pUnit:PlaySoundToSet(17269)
end


function LadyDeathwhisper_Spawn2A(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Through our master all things are possible! His power is without limit, and his will unbending!")
	pUnit:PlaySoundToSet(17270)
end

function LadyDeathwhisper_Spawn2B(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Those who oppose him will be destroyed utterly! And those who serve, who serve wholly, unquestioningly, with utter devotion of mind and soul? Elevated! To heights beyond your ken!")
	pUnit:PlaySoundToSet(17271)
end

function LadyDeathwhisper_Spawn3A(pUnit, event)
	pUnit:SendChatMessage(14, 0, "You have found your way here, because you are among the few gifted with true vision in a world cursed with blindness!")
	pUnit:PlaySoundToSet(17272)
end

function LadyDeathwhisper_Spawn3B(pUnit, event)
	pUnit:SendChatMessage(14, 0, "You can see through the fog that hangs over this world like a shroud and grasp where true power lies!")
	pUnit:PlaySoundToSet(17273)
end

RegisterUnitEvent(36855, 1, "LadyDeathwhisper_OnEnterCombat")
RegisterUnitEvent(36855, 2, "LadyDeathwhisper_OnCombatExit")
RegisterUnitEvent(36855, 3, "LadyDeathwhisper_Slay")
RegisterUnitEvent(36855, 4, "LadyDeathwhisper_Death")
RegisterUnitEvent(36855, 18, "LadyDeathwhisper_Spawn")

function LadyDeathwhisperAdd_Spawn(pUnit, event)
	pUnit:SetMovementFlags(1)
	local player = pUnit:GetRandomPlayer(0)
	pUnit:MoveTo(player:GetX(),player:GetY(),player:GetZ(),0)
end

RegisterUnitEvent(37949, 18, "LadyDeathwhisperAdd_Spawn")
RegisterUnitEvent(37890, 18, "LadyDeathwhisperAdd_Spawn")
RegisterUnitEvent(38222, 18, "LadyDeathwhisperAdd_Spawn")