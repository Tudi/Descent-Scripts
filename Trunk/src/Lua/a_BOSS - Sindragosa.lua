local player1,player2,player3,player4,player5
local count
function Sindragosa_OnEnterCombat(pUnit, event)
	pUnit:SendChatMessage(14, 0, "You are fools who have come to this place! The icy winds of Northrend will consume your souls!")
	pUnit:Land()
	pUnit:PlaySoundToSet(17007)
	pUnit:CastSpell(70109)
	pUnit:CastSpell(71050)
	pUnit:RegisterEvent("Sindragosa_Phase1", 500, 1)
end

function Sindragosa_Phase1(pUnit, event)
	pUnit:RegisterEvent("Sindragosa_LandSpells", 500, 1)
	pUnit:RegisterEvent("Sindragosa_PhaseTwoCheck", 1000, 0)
end

function Sindragosa_PhaseTwoCheck(pUnit, event)
	if pUnit:GetHealthPct() <= 85 then
		pUnit:RemoveEvents()
		pUnit:RegisterEvent("Sindragosa_LandToFly", 1000, 1)
	end
end

function Sindragosa_Phase2(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:MoveTo(4429.88, 2484.27, 203.38,0)
	pUnit:SetCombatCapable(1)
	pUnit:SetCombatTargetingCapable(1)
	pUnit:SetCombatMeleeCapable(1)
	pUnit:Land()
	pUnit:RegisterEvent("Sindragosa_LandSpells", 500, 1)
	pUnit:RegisterEvent("Sindragosa_LandToFly", 60500, 1)
	pUnit:RegisterEvent("Sindragosa_PhaseThreeCheck", 1000, 0)	
end

function Sindragosa_PhaseThreeCheck(pUnit, event)
	if pUnit:GetHealthPct() <= 30 then
		pUnit:RemoveEvents()
		pUnit:RegisterEvent("Sindragosa_Phase3", 1000, 1)
	end
end

function Sindragosa_Phase3(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Now feel my master's limitless power and despair!")
	pUnit:PlaySoundToSet(17016)
	pUnit:RemoveAura(71050)
	pUnit:CastSpell(70128)
	pUnit:RegisterEvent("Sindragosa_LandSpells", 1000, 1)
end

function Sindragosa_LandToFly(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Your incursion ends here! None shall survive!")
	pUnit:PlaySoundToSet(17012)
	pUnit:SetFlying(1)
	pUnit:SetCombatCapable(0)
	pUnit:SetCombatTargetingCapable(0)
	pUnit:SetCombatMeleeCapable(0)
	pUnit:MoveTo(4465.88, 2484.35, 227.09,0)
	pUnit:RegisterEvent("Sindragosa_FlyMode", 500, 1)
end

function Sindragosa_LandSpells(pUnit, event)
	pUnit:RegisterEvent("Sindragosa_Cleave", 8000, 0)
	pUnit:RegisterEvent("Sindragosa_TailSmash", 35000, 0)
	pUnit:RegisterEvent("Sindragosa_FrostBreath", 27500, 0)
	pUnit:RegisterEvent("Sindragosa_UnchainedMagic", 20000, 0)
	pUnit:RegisterEvent("Sindragosa_IcyGrip", 30000, 0)
end

function Sindragosa_FlyMode(pUnit, event)
	pUnit:RegisterEvent("Sindragosa_FrostBeacon", 5000, 1)
	pUnit:RegisterEvent("Sindragosa_FrostBomb", 40000, 1)
	pUnit:RegisterEvent("Sindragosa_FrostBomb", 45000, 1)
	pUnit:RegisterEvent("Sindragosa_FrostBomb", 50000, 1)
	pUnit:RegisterEvent("Sindragosa_FrostBomb", 55000, 1)
	pUnit:RegisterEvent("Sindragosa_Phase2", 60000, 1)
end

function Sindragosa_FrostBeacon(pUnit, event)
	local player1 = Unit:GetRandomPlayer(7)
	local player2 = Unit:GetRandomPlayer(7)
	while ((player1 == player2) and player1 ~= nil and playerCount > 3) do
		player2 = Unit:GetRandomPlayer(7)
	end
	local player3 = pUnit:GetRandomPlayer(7)
	while ((player3 == player1 or player3 == player2) and player2 ~= nil and playerCount > 4) do
		player3 = pUnit:GetRandomPlayer(7)
	end
	local player4 = pUnit:GetRandomPlayer(7)
	while ((player4 == player1 or player4 == player2 or player4 == player3) and player3 ~= nil and playerCount > 5) do
		player4 = pUnit:GetRandomPlayer(7)
	end
	local player5 = pUnit:GetRandomPlayer(7)
	while ((player5 == player1 or player5 == player2 or player5 == player3 or player5 == player4) and player4 ~= nil and playerCount > 6) do
		player5 = pUnit:GetRandomPlayer(7)
	end
	if player1 ~= nil then
		pUnit:CastSpellOnTarget(70126, player1)
	end
	if player2 ~= nil then
		pUnit:CastSpellOnTarget(70126, player2)
	end
	if player3 ~= nil then
		pUnit:CastSpellOnTarget(70126, player3)
	end
	if player4 ~= nil then
		pUnit:CastSpellOnTarget(70126, player4)
	end
	if player5 ~= nil then
		pUnit:CastSpellOnTarget(70126, player5)
	end
	pUnit:RegisterEvent("Sindragosa_IceTomb ", 7000, 1)
end

function Sindragosa_IceTomb(pUnit, event)
	if player1 ~= nil then 
		pUnit:CastSpellOnTarget(70157, player1)
	end
	if player2 ~= nil then
		pUnit:CastSpellOnTarget(70157, player2)
	end
	if player3 ~= nil then
		pUnit:CastSpellOnTarget(70157, player3)
	end
	if player4 ~= nil then
		pUnit:CastSpellOnTarget(70157, player4)
	end
	if player5 ~= nil then
		pUnit:CastSpellOnTarget(70157, player5)
	end
end

function Sindragosa_FrostBomb(pUnit, event)
	if count == 1 then
		pUnit:SpawnCreature(37186, 4374.73, 2445.03, 203.385, 0, pUnit:GetFaction(), 0)
		count = 2
	elseif count == 2 then
		pUnit:SpawnCreature(37186, 4364.52, 2465.54, 203.385, 0, pUnit:GetFaction(), 0)
		count = 3
	elseif count == 3 then
		pUnit:SpawnCreature(37186, 4377.3, 2494.96, 203.382, 0, pUnit:GetFaction(), 0)
		count = 4
	elseif count == 4 then
		pUnit:SpawnCreature(37186, 4361.73, 2522.04, 203.385, 0, pUnit:GetFaction(), 0)
		count = 1
	end
end

function Sindragosa_Cleave(pUnit, event)
	pUnit:CastSpell(19983,pUnit:GetMainTank())
end



function Sindragosa_TailSmash(pUnit, event)
	local mainTank = pUnit:GetMainTank()
	pUnit:CastSpellAoF(mainTank:GetX(), mainTank:GetY(), mainTank:GetZ(), 71077)
end

function Sindragosa_FrostBreath(pUnit, event)
	pUnit:FullCastSpell(71056)
end

function Sindragosa_UnchainedMagic(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Suffer, mortals, as your pathetic magic betrays you!")
	pUnit:PlaySoundToSet(17014)
	pUnit:CastSpell(69762,pUnit:GetRandomPlayer(4))
end

function Sindragosa_IcyGrip(pUnit, event)
	pUnit:CastSpell(70117)
	pUnit:RegisterEvent("Sindragosa_BlisteringCold", 5000, 1)
end

function Sindragosa_BlisteringCold(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Can you feel the cold hand of death upon your heart?")
	pUnit:PlaySoundToSet(17013)
	pUnit:FullCastSpell(71047)
end

function Sindragosa_Slay(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Perish!")
		pUnit:PlaySoundToSet(17008)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "A flaw of mortality...")
		pUnit:PlaySoundToSet(17009)
	end
end

function Sindragosa_Death(pUnit, event, player)
	pUnit:SendChatMessage(14, 0, "Free...at last...")
	pUnit:PlaySoundToSet(17010)
	player:AdvanceQuestObjective(24547, 0)
	pUnit:RemoveEvents()
end

function Sindragosa_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

function Sindragosa_OnSpawn(pUnit, event)
	pUnit:MoveTo(4429.88, 2484.27, 203.38,0)
	pUnit:Land()
end

RegisterUnitEvent(36853, 1, "Sindragosa_OnEnterCombat")
RegisterUnitEvent(36853, 2, "Sindragosa_OnCombatExit")
RegisterUnitEvent(36853, 3, "Sindragosa_Slay")
RegisterUnitEvent(36853, 4, "Sindragosa_Death")
RegisterUnitEvent(36853, 18, "Sindragosa_OnSpawn")


function FrostBomb_OnSpawn(pUnit, event)
	pUnit:Root()
	pUnit:RegisterEvent("FrostBomb_Bomb", 2000, 1)
end

function FrostBomb_Bomb(pUnit, event)
	pUnit:CastSpell(71053)
	pUnit:Despawn(200,0)
end

RegisterUnitEvent(37186, 18, "FrostBomb_OnSpawn")