function Anubarak_OnEnterCombat(pUnit, event)
	pUnit:RegisterEvent("Anubarak_GroundPhase", 1000, 1)
end

function Anubarak_GroundPhase(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:RegisterEvent("Anubarak_FreezingSlash", 20000, 0)
	pUnit:RegisterEvent("Anubarak_PenetratingCold", 18000, 0)
	pUnit:RegisterEvent("Anubarak_LeechingSwarm", 1000, 1)
--	pUnit:RegisterEvent("Anubarak_DivePhase", 60000, 1)
end

function Anubarak_DivePhase(pUnit, event)
	pUnit:RemoveEvents()
	local target = pUnit:GetRandomPlayer(0)
	pUnit:CastSpellOnTarget(67574,target)
	pUnit:RegisterEvent("Anubarak_GroundPhase", 60000, 1)
end

function Anubarak_FreezingSlash(pUnit, event)
	local target = pUnit:GetRandomPlayer(7)
	if (target ~= nil) then
		pUnit:CastSpellOnTarget(66012,target)
	end
end

function Anubarak_PenetratingCold(pUnit, event)
	local playerCount = pUnit:GetInRangePlayersCount()
	local player1 = pUnit:GetRandomPlayer(7)
	local player2 = pUnit:GetRandomPlayer(7)
	while ((player1 == player2) and player1 ~= nil and playerCount > 3) do
		player2 = pUnit:GetRandomPlayer(7)
	end
	local player3 = pUnit:GetRandomPlayer(7)
	while ((player3 == player1 or player3 == player2) and player2 ~= nil and playerCount > 4) do
		player3 = pUnit:GetRandomPlayer(7)
	end
	local player4 = pUnit:GetRandomPlayer(7)
	while ((player4 == player1 or player4 == player2 or player4 == player3) and player3 ~= nil and playerCount > 5) do
		player4 = pUnit:GetRandomPlayer(7)
	end
	if player1 ~= nil then
		pUnit:CastSpellOnTarget(66013, player1)
	end
	if player2 ~= nil then
		pUnit:CastSpellOnTarget(66013, player2)
	end
	if player3 ~= nil then
		pUnit:CastSpellOnTarget(66013, player3)
	end
	if player4 ~= nil then
		pUnit:CastSpellOnTarget(66013, player4)
	end
end

function Anubarak_LeechingSwarm(pUnit, event)
	local hp = pUnit:GetHealthPct()
	if (hp < 30) then
		pUnit:CastSpell(66118)
	else
		pUnit:RegisterEvent("Anubarak_LeechingSwarm", 1000, 1)
	end
end

RegisterUnitEvent(34564, 1, "Anubarak_OnEnterCombat")