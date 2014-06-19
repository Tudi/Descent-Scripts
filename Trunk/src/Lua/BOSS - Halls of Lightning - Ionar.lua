function Ionar_OnEnterCombat(pUnit, event)
	pUnit:SendChatMessage(14, 0, "You wish to confront the master? You must weather the storm!")
	pUnit:PlaySoundToSet(14453)
	pUnit:RegisterEvent("Ionar_BallLightning", 6000, 0)
	pUnit:RegisterEvent("Ionar_StaticOverload", 13000, 0)
	pUnit:RegisterEvent("Ionar_Disperse", 30000, 1)
end

function Ionar_Disperse(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "The slightest spark shall be your undoing.")
		pUnit:PlaySoundToSet(14454)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "No one is safe!")
		pUnit:PlaySoundToSet(14455)
	end
	pUnit:FullCastSpell(52770)
	pUnit:RegisterEvent("Ionar_DisperseSpawn", 3000, 1)
end

function Ionar_DisperseSpawn(pUnit, event)
	pUnit:SpawnCreature(28926, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), 0, pUnit:GetFaction(), 20000)
end

function Ionar_BallLightning(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if player ~= nil then
		pUnit:FullCastSpellOnTarget(52780,player)
	end
end

function Ionar_StaticOverload(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if player ~= nil then
		pUnit:CastSpellOnTarget(52658,player)
	end
end

function Ionar_Slay(pUnit, event)
	local rand = math.random(1,3)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Shocking, I know")
		pUnit:PlaySoundToSet(14456)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "You attempt the impossible")
		pUnit:PlaySoundToSet(14457)
	elseif rand == 3 then
		pUnit:SendChatMessage(14, 0, "Your spark of life is..extinguished!")
		pUnit:PlaySoundToSet(14458)
	end
end

function Ionar_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Master... you have guests.")
	pUnit:PlaySoundToSet(14459)
	pUnit:RemoveEvents()
end

function Ionar_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(28546, 1, "Ionar_OnEnterCombat")
RegisterUnitEvent(28546, 2, "Ionar_OnCombatExit")
RegisterUnitEvent(28546, 3, "Ionar_Slay")
RegisterUnitEvent(28546, 4, "Ionar_Death")

function SparkofIonar_OnSpawn(pUnit, event)
	pUnit:MoveTo(1171.34, -262.21, 52.35, 6.22)
	pUnit:RegisterEvent("SparkofIonar_ArcingBurn", 1000, 0)
end

function SparkofIonar_ArcingBurn(pUnit, event)
	pUnit:CastSpell(52671)
	pUnit:CastSpell(52662)
end

RegisterUnitEvent(28926, 18, "SparkofIonar_OnSpawn")