function DeathbringerSaurfang_OnEnterCombat(pUnit, event)
	pUnit:SendChatMessage(14, 0, "BY THE MIGHT OF THE LICH KING!")
	pUnit:PlaySoundToSet(16694)
	pUnit:RegisterEvent("DeathbringerSaurfang_CallBloodBeast", 80000, 0)
	pUnit:RegisterEvent("DeathbringerSaurfang_MarkoftheFallenChampion", 20000, 0)
	pUnit:RegisterEvent("DeathbringerSaurfang_BoilingBlood", 15000, 0)
	pUnit:RegisterEvent("DeathbringerSaurfang_BloodNova", 22500, 0)
--	pUnit:RegisterEvent("DeathbringerSaurfang_RuneofBlood", 20000, 0)
	pUnit:RegisterEvent("DeathbringerSaurfang_FrenzyCheck", 1000, 1)
end


function DeathbringerSaurfang_BoilingBlood(pUnit, event)
	local playerCount = pUnit:GetInRangePlayersCount()
	local player1 = pUnit:GetRandomPlayer(7)
	local player2 = pUnit:GetRandomPlayer(7)
	while ((player1 == player2) and player1 ~= nil and playerCount > 2) do
		player2 = pUnit:GetRandomPlayer(7)
	end
	local player3 = pUnit:GetRandomPlayer(7)
	while ((player3 == player1 or player3 == player2) and player2 ~= nil and playerCount > 3) do
		player3 = pUnit:GetRandomPlayer(7)
	end
	if player1 ~= nil then
		pUnit:CastSpellOnTarget(72442, player1)
	end
	if player2 ~= nil then
		pUnit:CastSpellOnTarget(72442, player2)
	end
	if player3 ~= nil then
		pUnit:CastSpellOnTarget(72442, player3)
	end
end

function DeathbringerSaurfang_MarkoftheFallenChampion(pUnit, event)
	pUnit:SendChatMessage(14, 0, "The ground runs red with your blood!")
	pUnit:PlaySoundToSet(16699)
	pUnit:FullCastSpellOnTarget(72293, pUnit:GetRandomPlayer(0))
end

function DeathbringerSaurfang_BloodNova(pUnit, event)
	pUnit:CastSpellOnTarget(72438, pUnit:GetRandomPlayer(0))
end

function DeathbringerSaurfang_RuneofBlood(pUnit, event)
	pUnit:CastSpell(72408)
	pUnit:CastSpellOnTarget(72410, pUnit:GetRandomPlayer(0))
end

function DeathbringerSaurfang_CallBloodBeast(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Feast, my minions!")
	pUnit:PlaySoundToSet(16700)
	pUnit:SpawnCreature(38508, pUnit:GetX()+5, pUnit:GetY()+5, pUnit:GetZ(), 0, pUnit:GetFaction(), 600000) 
	pUnit:SpawnCreature(38508, pUnit:GetX()+5, pUnit:GetY()-5, pUnit:GetZ(), 0, pUnit:GetFaction(), 600000) 
	pUnit:SpawnCreature(38508, pUnit:GetX()+5, pUnit:GetY(), pUnit:GetZ(), 0, pUnit:GetFaction(), 600000) 
	pUnit:SpawnCreature(38508, pUnit:GetX()-5, pUnit:GetY()+5, pUnit:GetZ(), 0, pUnit:GetFaction(), 600000) 
	pUnit:SpawnCreature(38508, pUnit:GetX()-5, pUnit:GetY()-5, pUnit:GetZ(), 0, pUnit:GetFaction(), 600000) 
end


function DeathbringerSaurfang_FrenzyCheck(pUnit, event)
	if pUnit:GetHealthPct() <= 30 then
		pUnit:RegisterEvent("DeathbringerSaurfang_Frenzy", 1000, 1)
	else
		pUnit:RegisterEvent("DeathbringerSaurfang_FrenzyCheck", 1000, 1)
	end
end

function DeathbringerSaurfang_Frenzy(pUnit, event)
	pUnit:CastSpell(72737)
end

function DeathbringerSaurfang_Slay(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "You are nothing!")
		pUnit:PlaySoundToSet(16695)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Your soul will find no redemption here!")
		pUnit:PlaySoundToSet(16696)
	end
end

function DeathbringerSaurfang_Death(pUnit, event, player)
	pUnit:SendChatMessage(14, 0, "I... Am... Released.")
	pUnit:PlaySoundToSet(16697)
	player:AdvanceQuestObjective(24547, 0)
	pUnit:RemoveEvents()
end

function DeathbringerSaurfang_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

function DeathbringerSaurfang_Spawn(pUnit, event)
	target = pUnit:GetClosestPlayer()
	if target ~=nil then
		local race = target:GetPlayerRace()
		if (race == 1 or race == 3 or race == 4 or race == 7 or race == 11) then
			pUnit:SpawnCreature(37200, -543, 2206, 539.3, 0, 35, 0)
			pUnit:RegisterEvent("DeathbringerSaurfang_SpawnAlliance1", 500, 1)
			pUnit:RegisterEvent("DeathbringerSaurfang_SpawnAlliance2", 18000, 1)
			pUnit:RegisterEvent("DeathbringerSaurfang_SpawnAlliance3", 34750, 1)
		elseif (race == 2 or race == 5 or race == 6 or race == 8 or race == 10) then
			pUnit:SpawnCreature(37187, -543, 2206, 539.3, 0, 35, 0) 
			pUnit:RegisterEvent("DeathbringerSaurfang_SpawnHorde1", 500, 1)
			pUnit:RegisterEvent("DeathbringerSaurfang_SpawnHorde2", 10500, 1)
			pUnit:RegisterEvent("DeathbringerSaurfang_SpawnHorde3", 17000, 1)
		end
	end
end

function DeathbringerSaurfang_SpawnAlliance1(pUnit, event)
	pUnit:SendChatMessage(14, 0, "For every Horde soldier that you killed, for every Alliance dog that fell, the Lich King's armies grew. Even now the Val'kyr work to raise your fallen... As Scourge.")
	pUnit:PlaySoundToSet(16701)
end

function DeathbringerSaurfang_SpawnAlliance2(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Things are about to get much worse. Come, taste the power that the Lich King has bestowed upon me!")
	pUnit:PlaySoundToSet(16702)
end

function DeathbringerSaurfang_SpawnAlliance3(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Hahahaha! Dwarves.")
	pUnit:PlaySoundToSet(17269)
end


function DeathbringerSaurfang_SpawnHorde1(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Join me, father. Join me and we will crush this world in the name of the Scourge. For the glory of the Lich King!")
	pUnit:PlaySoundToSet(16704)
end

function DeathbringerSaurfang_SpawnHorde2(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Stubborn and old. What chance do you have? I am stronger and more powerful than you ever were!")
	pUnit:PlaySoundToSet(16705)
end

function DeathbringerSaurfang_SpawnHorde3(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Pathetic old orc. Come then heroes, come and face the might of the Scourge!")
	pUnit:PlaySoundToSet(16706)
end

RegisterUnitEvent(37813, 1, "DeathbringerSaurfang_OnEnterCombat")
RegisterUnitEvent(37813, 2, "DeathbringerSaurfang_OnCombatExit")
RegisterUnitEvent(37813, 3, "DeathbringerSaurfang_Slay")
RegisterUnitEvent(37813, 4, "DeathbringerSaurfang_Death")
RegisterUnitEvent(37813, 18, "DeathbringerSaurfang_Spawn")

function MuradinBronzebeard_Spawn(pUnit, event)
	pUnit:RegisterEvent("MuradinBronzebeard_Spawn1", 27750, 1)
	pUnit:RegisterEvent("MuradinBronzebeard_Spawn2", 32750, 1)
end

function MuradinBronzebeard_Spawn1(pUnit, event)
	pUnit:SendChatMessage(14, 0, "A lone orc, against the might of the Alliance?")
	pUnit:PlaySoundToSet(16970)
end

function MuradinBronzebeard_Spawn2(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Charge!")
	pUnit:PlaySoundToSet(16971)
end

function HighOverlordSaurfang_Spawn(pUnit, event)
	pUnit:RegisterEvent("HighOverlordSaurfang_Spawn1", 500, 1)
	pUnit:RegisterEvent("HighOverlordSaurfang_Spawn2", 500, 1)
	pUnit:RegisterEvent("HighOverlordSaurfang_Spawn3", 500, 1)
	pUnit:RegisterEvent("HighOverlordSaurfang_Spawn4", 500, 1)
end

function HighOverlordSaurfang_Spawn1(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Kor'kron, move out! Champions, watch your backs! The Scourge have been...")
	pUnit:PlaySoundToSet(16970)
end

function HighOverlordSaurfang_Spawn2(pUnit, event)
	pUnit:SendChatMessage(14, 0, "My boy died at the Wrathgate. I am here only to collect his body!")
	pUnit:PlaySoundToSet(16971)
end

function HighOverlordSaurfang_Spawn3(pUnit, event)
	pUnit:SendChatMessage(14, 0, "We named him Dranosh. It means \"Heart of Draenor\" in Orcish. I would not let the warlocks take him. My boy would be safe, hidden away by the elders of Garadar.")
	pUnit:PlaySoundToSet(16971)
end

function HighOverlordSaurfang_Spawn4(pUnit, event)
	pUnit:SendChatMessage(14, 0, "I made a promise to his mother before she died: that I would cross the Dark Portal alone - whether I lived or died, my son would be safe. Untainted...")
	pUnit:PlaySoundToSet(16971)
end

function HighOverlordSaurfang_Spawn5(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Today, I fulfill that promise.")
	pUnit:PlaySoundToSet(16971)
end

RegisterUnitEvent(37200, 18, "MuradinBronzebeard_Spawn")
RegisterUnitEvent(37187, 18, "HighOverlordSaurfang_Spawn")