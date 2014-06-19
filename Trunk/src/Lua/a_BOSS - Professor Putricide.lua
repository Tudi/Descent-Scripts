function ProfessorPutricide_OnEnterCombat(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Good news, everyone! I think I perfected a plague that will destroy all life on Azeroth!")
	pUnit:PlaySoundToSet(17114)
	pUnit:RegisterEvent("ProfessorPutricide_PhaseOne", 1000, 1)
end

function ProfessorPutricide_PhaseOne(pUnit, event)
	pUnit:RegisterEvent("ProfessorPutricide_SlimePuddle", 35000, 0)
	pUnit:RegisterEvent("ProfessorPutricide_UnstableExperiment", 38000, 0)
	pUnit:RegisterEvent("ProfessorPutricide_PhaseTwoCheck", 1000, 0)
end

function ProfessorPutricide_PhaseTwoCheck(pUnit, event)
	if pUnit:GetHealthPct() <= 80 then
		pUnit:RemoveEvents()
		pUnit:SetCombatCapable(0)
		pUnit:SetCombatTargetingCapable(0)
		pUnit:SetCombatMeleeCapable(0)
		pUnit:FullCastSpell(71617)
		pUnit:RegisterEvent("ProfessorPutricide_PhaseTwoIntro", 2000, 1)
	end
end

function ProfessorPutricide_PhaseTwoIntro(pUnit,event)
	pUnit:MoveTo(4356.19, 3262.9, 389.59,0)
	pUnit:Root()
	pUnit:RegisterEvent("ProfessorPutricide_GuzzlePotionsPhaseTwo", 1000, 1)
	pUnit:RegisterEvent("ProfessorPutricide_ResultSay", 34000, 1)
	pUnit:RegisterEvent("ProfessorPutricide_PhaseTwo", 35000, 1) 
end

function ProfessorPutricide_GuzzlePotionsPhaseTwo(pUnit, event)
	pUnit:FullCastSpell(72852)
end


function ProfessorPutricide_PhaseTwo(pUnit, event)
	pUnit:SetCombatCapable(1)
	pUnit:SetCombatTargetingCapable(1)
	pUnit:SetCombatMeleeCapable(1)
	pUnit:RegisterEvent("ProfessorPutricide_SlimePuddle", 35000, 0)
	pUnit:RegisterEvent("ProfessorPutricide_UnstableExperiment", 38000, 0)
	pUnit:RegisterEvent("ProfessorPutricide_ChokingGas", 35500, 0)
	pUnit:RegisterEvent("ProfessorPutricide_MalleableGoo", 25000, 0)
	pUnit:RegisterEvent("ProfessorPutricide_PhaseThreeCheck", 1000, 0)
end

function ProfessorPutricide_PhaseThreeCheck(pUnit, event)
	if pUnit:GetHealthPct() <= 35 then
		pUnit:RemoveEvents()
		pUnit:SetCombatCapable(0)
		pUnit:SetCombatTargetingCapable(0)
		pUnit:SetCombatMeleeCapable(0)
		pUnit:FullCastSpell(71617)
		pUnit:RegisterEvent("ProfessorPutricide_PhaseThreeIntro", 2000, 1)
	end
end

function ProfessorPutricide_PhaseThreeIntro(pUnit,event)
	pUnit:MoveTo(4356.19, 3262.9, 389.59,0)
	pUnit:Root()
	pUnit:RegisterEvent("ProfessorPutricide_GuzzlePotionsPhaseThree", 1000, 1)
	pUnit:RegisterEvent("ProfessorPutricide_ResultSay", 34000, 1)
	pUnit:RegisterEvent("ProfessorPutricide_PhaseThree", 35000, 1) 
end

function ProfessorPutricide_GuzzlePotionsPhaseThree(pUnit, event)
	pUnit:FullCastSpell(73121)
end

function ProfessorPutricide_PhaseThree(pUnit, event)
	pUnit:SetCombatCapable(1)
	pUnit:SetCombatTargetingCapable(1)
	pUnit:SetCombatMeleeCapable(1)
	pUnit:RegisterEvent("ProfessorPutricide_MutatedPlague", 10000, 0)
	pUnit:RegisterEvent("ProfessorPutricide_SlimePuddle", 35000, 0)
	pUnit:RegisterEvent("ProfessorPutricide_ChokingGas", 40000, 0)
	pUnit:RegisterEvent("ProfessorPutricide_MalleableGoo", 25000, 0)
end

function ProfessorPutricide_SlimePuddle(pUnit, event)
	local target = pUnit:GetRandomPlayer(7)
	if target ~= nil then
		pUnit:CastSpellOnTarget(70341, target)
		pUnit:SpawnCreature(37690,target:GetX(), target:GetY(), target:GetZ(), 0, pUnit:GetFaction(), 0) 
	end
end

function ProfessorPutricide_ChokingGas(pUnit, event)
	local target = pUnit:GetRandomPlayer(0)
	pUnit:SpawnCreature(38159,target:GetX(), target:GetY(), target:GetZ(), 0, pUnit:GetFaction(), 0) 
end

function ProfessorPutricide_UnstableExperiment(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SpawnCreature(37562, 4356.19, 3163.31, 389.4, 0, pUnit:GetFaction(), 0) 
	elseif rand == 2 then
		pUnit:SpawnCreature(37697, 4356.19, 3262.9, 389.59, 0, pUnit:GetFaction(), 0) 
	end
end


function ProfessorPutricide_ResultSay(pUnit, event)
	pUnit:Unroot()
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Hmm. I don't feel a thing. Whaa...? Where'd those come from?")
		pUnit:PlaySoundToSet(17120)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Tastes like... Cherry! Oh! Excuse me!")
		pUnit:PlaySoundToSet(17121)
	end
end

function ProfessorPutricide_MalleableGoo(pUnit, event)
	local target = pUnit:GetRandomPlayer(0)
	pUnit:CastSpellAoF(target:GetX(), target:GetY(), target:GetZ(), 72297)
end
function ProfessorPutricide_MutatedPlague(pUnit, event)
	pUnit:CastSpell(72672,pUnit:GetMainTank())
end

function ProfessorPutricide_Slay(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Hmm... Interesting...")
		pUnit:PlaySoundToSet(17115)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "That was unexpected!")
		pUnit:PlaySoundToSet(17116)
	end
end

function ProfessorPutricide_Death(pUnit, event, player)
	pUnit:SendChatMessage(14, 0, "Bad news, everyone! I don't think I'm going to make it.")
	pUnit:PlaySoundToSet(17117)
	player:AdvanceQuestObjective(24547, 0)
	pUnit:RemoveEvents()
end

function ProfessorPutricide_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(36678, 1, "ProfessorPutricide_OnEnterCombat")
RegisterUnitEvent(36678, 2, "ProfessorPutricide_OnCombatExit")
RegisterUnitEvent(36678, 3, "ProfessorPutricide_Slay")
RegisterUnitEvent(36678, 4, "ProfessorPutricide_Death")

function OozePuddle_Spawn(pUnit, event)
	pUnit:Root()
	pUnit:RegisterEvent("OozePuddle_SlimePuddle", 1000, 0)
	pUnit:Despawn(10000,0)
end

function OozePuddle_SlimePuddle(pUnit, event)
	pUnit:CastSpell(70346)
end

function OozePuddle_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:Despawn(1000,0)
end

function OozePuddle_Died(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(37690, 18, "OozePuddle_Spawn")
RegisterUnitEvent(37690, 2, "OozePuddle_OnCombatExit")
RegisterUnitEvent(37690, 4, "OozePuddle_Died")

function OozesAdd_Spawn(pUnit, event)
	local target = pUnit:GetRandomPlayer(0)
	pUnit:MoveTo(target:GetX(), target:GetY(), target:GetZ(),0)
end

RegisterUnitEvent(37697, 18, "OozesAdd_Spawn")
RegisterUnitEvent(37562, 18, "OozesAdd_Spawn")
RegisterUnitEvent(37697, 2, "OozePuddle_OnCombatExit")
RegisterUnitEvent(37562, 2, "OozePuddle_OnCombatExit")

function OozesAdd_Spawn(pUnit, event)
	local target = pUnit:GetRandomPlayer(0)
	pUnit:MoveTo(target:GetX(), target:GetY(), target:GetZ(),0)
end

RegisterUnitEvent(37697, 18, "OozesAdd_Spawn")
RegisterUnitEvent(37697, 2, "OozePuddle_OnCombatExit")

function ChokingGasBomb_Spawn(pUnit, event)
	pUnit:Root()
	pUnit:RegisterEvent("ChokingGasBomb_Boom", 10000, 1)
	pUnit:RegisterEvent("ChokingGasBomb_Choke", 1000, 0)
end

function ChokingGasBomb_Choke(pUnit, event)
	pUnit:CastSpell(72619)
end

function ChokingGasBomb_Boom(pUnit, event)
	pUnit:CastSpell(72459)
	pUnit:RemoveEvents()
	pUnit:Despawn(100,0)
end

RegisterUnitEvent(38159, 2, "OozePuddle_OnCombatExit")
RegisterUnitEvent(38159, 18, "ChokingGasBomb_Spawn")