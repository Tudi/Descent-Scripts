function Loken_OnEnterCombat(pUnit, event)
	pUnit:SendChatMessage(14, 0, "What hope is there for you? None!")
	pUnit:PlaySoundToSet(14162)
	pUnit:RegisterEvent("Loken_ArcLightning", 7000, 0)
	pUnit:RegisterEvent("Loken_LightningNova", 30000, 0)
	pUnit:RegisterEvent("Loken_HealthCheck", 1000, 1)
end

function Loken_ArcLightning(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if player ~= nil then
		pUnit:CastSpellOnTarget(52921,player)
	end
end

function Loken_LightningNova(pUnit, event)
	local rand = math.random(1,3)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "You cannot hide from fate!")
		pUnit:PlaySoundToSet(14163)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Come closer. I will make it quick.")
		pUnit:PlaySoundToSet(14164)
	elseif rand == 3 then
		pUnit:SendChatMessage(14, 0, "Your flesh cannot hold out for long.")
		pUnit:PlaySoundToSet(14165)
	end
	pUnit:FullCastSpell(52960)
end

function Loken_HealthCheck(pUnit, event)
	if pUnit:GetHealthPct() <= 75 then
		pUnit:PlaySoundToSet(14169)
		pUnit:SendChatMessage(14, 0, "You stare blindly into the abyss!")
		pUnit:RegisterEvent("Loken_HealthCheck2", 1000, 1)
	else
		pUnit:RegisterEvent("Loken_HealthCheck", 1000, 1)
	end
end

function Loken_HealthCheck2(pUnit, event)
	if pUnit:GetHealthPct() <= 50 then
		pUnit:PlaySoundToSet(14170)
		pUnit:SendChatMessage(14, 0, "Your ignorance is profound. Can you not see where this path leads?")
		pUnit:RegisterEvent("Loken_HealthCheck3", 1000, 1)
	else
		pUnit:RegisterEvent("Loken_HealthCheck2", 1000, 1)
	end
end

function Loken_HealthCheck3(pUnit, event)
	if pUnit:GetHealthPct() <= 25 then
		pUnit:PlaySoundToSet(14171)
		pUnit:SendChatMessage(14, 0, "You cross the precipice of oblivion!")
	else
		pUnit:RegisterEvent("Loken_HealthCheck3", 1000, 1)
	end
end

function Loken_Slay(pUnit, event)
	local rand = math.random(1,3)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Only mortal...")
		pUnit:PlaySoundToSet(14166)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "I... am... FOREVER!")
		pUnit:PlaySoundToSet(14167)
	elseif rand == 3 then
		pUnit:SendChatMessage(14, 0, "What little time you had, you wasted!")
		pUnit:PlaySoundToSet(14168)
	end
end

function Loken_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "My death... heralds the end of this world.")
	pUnit:PlaySoundToSet(14172)
	pUnit:RemoveEvents()
end

function Loken_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

function Loken_Spawn(pUnit, event)
	pUnit:SendChatMessage(14, 0, "I have witnessed the rise and fall of empires. The birth and extinction of entire species. Over countless millennia the foolishness of mortals has remained the only constant. Your presence here confirms this.")
	pUnit:PlaySoundToSet(14160)
	pUnit:RegisterEvent("Loken_Spawn2", 19500, 1)
end

function Loken_Spawn2(pUnit, event)
	pUnit:SendChatMessage(14, 0, "My master has shown me the future, and you have no place in it. Azeroth will be reborn in darkness. Yogg-Saron shall be released! The Pantheon shall fall!")
	pUnit:PlaySoundToSet(14161)
end

RegisterUnitEvent(28923, 1, "Loken_OnEnterCombat")
RegisterUnitEvent(28923, 2, "Loken_OnCombatExit")
RegisterUnitEvent(28923, 3, "Loken_Slay")
RegisterUnitEvent(28923, 4, "Loken_Death")
RegisterUnitEvent(28923, 18, "Loken_Spawn")