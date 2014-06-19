local closestPlayerToTank
local currTank

function BloodQueenLanathel_OnEnterCombat(pUnit, event)
	pUnit:CastSpell(72981)
	pUnit:SendChatMessage(14, 0, "You have made an... unwise... decision.")
	pUnit:PlaySoundToSet(16782)
	pUnit:RegisterEvent("BloodQueenLanathel_LandSpells", 1000, 1)
	pUnit:RegisterEvent("BloodQueenLanathel_VampiricBite", 15000, 1)
	pUnit:RegisterEvent("BloodQueenLanathel_PhaseTwo", 141000, 1)
end

function BloodQueenLanathel_LandSpells(pUnit, event)
	pUnit:Unroot()
	pUnit:RegisterEvent("BloodQueenLanathel_Bloodlink", 2000, 0)
	pUnit:RegisterEvent("BloodQueenLanathel_DeliriousSlash", 22000, 0)
	pUnit:RegisterEvent("BloodQueenLanathel_PactoftheDarkfallen", 30500, 0)
	pUnit:RegisterEvent("BloodQueenLanathel_SwarmingShadows", 30500, 0)
	pUnit:RegisterEvent("BloodQueenLanathel_TwilightBloodbolt", 20000, 0)
end

function BloodQueenLanathel_PhaseTwo(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:Root()
	pUnit:SendChatMessage(14, 0, "Here it comes.")
	pUnit:PlaySoundToSet(16788)
	pUnit:RegisterEvent("BloodQueenLanathel_BloodboltWhirl", 2000, 3)
	pUnit:RegisterEvent("BloodQueenLanathel_PhaseOne", 10000, 1)
end


function BloodQueenLanathel_PhaseOne(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:RegisterEvent("BloodQueenLanathel_LandSpells", 1000, 1)
	pUnit:RegisterEvent("BloodQueenLanathel_PhaseTwo", 61000, 1)
end

function BloodQueenLanathel_VampiricBite(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Just a taste...")
		pUnit:PlaySoundToSet(16783)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Know my hunger!")
		pUnit:PlaySoundToSet(16784)
	end
	local player = pUnit:GetRandomPlayer(7)
	if player ~= nil then
		pUnit:CastSpellOnTarget(71726,player)
	end
end

function BloodQueenLanathel_Bloodlink(pUnit, event)
	local CloseTankGuid = pUnit:GetUInt32Variable(2)
	if (CloseTankGuid ~= 0) then
		local closestPlayerToTank = pUnit:GetUnitByGUID(CloseTankGuid)
		closestPlayerToTank:RemoveAura(71510)
	end

	local currTankGuid = pUnit:GetUInt32Variable(1)
	if (currTankGuid ~= 0) then
		local currTank = pUnit:GetUnitByGUID(currTankGuid)
		currTank:RemoveAura(70821)
	end
	
	local currTank = pUnit:GetMainTank()
	pUnit:SetUInt32Variable(1,pUnit:GetGUID(currTank))

	local closestPlayerToTank = currTank:GetClosestPlayer()
	pUnit:SetUInt32Variable(1,pUnit:GetGUID(closestPlayerToTank))

	if(closestPlayerToTank ~= nil) then
		pUnit:CastSpellOnTarget(71510,closestPlayerToTank)
		pUnit:CastSpellOnTarget(70821,currTank)
	end
end

function BloodQueenLanathel_DeliriousSlash(pUnit, event)
	local CloseTankGuid = pUnit:GetUInt32Variable(2)
	if (CloseTankGuid ~= 0) then
		local closestPlayerToTank = pUnit:GetUnitByGUID(CloseTankGuid)
		pUnit:CastSpellOnTarget(71623,closestPlayerToTank)
	end
end

function BloodQueenLanathel_PactoftheDarkfallen(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Can you handle this?")
	pUnit:PlaySoundToSet(16787)
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
		pUnit:CastSpellOnTarget(71340, player1)
	end
	if player2 ~= nil then
		pUnit:CastSpellOnTarget(71340, player2)
	end
	if player3 ~= nil then
		pUnit:CastSpellOnTarget(71340, player3)
	end
end

function BloodQueenLanathel_SwarmingShadows(pUnit, event)
	pUnit:SendChatMessage(14, 0, "SUFFER!")
	pUnit:PlaySoundToSet(16786)
	local player = pUnit:GetRandomPlayer(7)
	if player ~= nil then
		pUnit:CastSpellOnTarget(71265, player)
	end
end

function BloodQueenLanathel_TwilightBloodbolt(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if player ~= nil then
		pUnit:CastSpellOnTarget(71446, player)
	end
end

function BloodQueenLanathel_BloodboltWhirl(pUnit, event)
	
end

function BloodQueenLanathel_Slay(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Really... Is that all you've got?")
		pUnit:PlaySoundToSet(16791)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Such a pity.")
		pUnit:PlaySoundToSet(16792)
	end
end

function BloodQueenLanathel_Death(pUnit, event, player, Misc)
	pUnit:SendChatMessage(14, 0, "But... we were getting along... so well...")
	pUnit:PlaySoundToSet(16794)
	player:AdvanceQuestObjective(24547, 0)
	pUnit:RemoveEvents()
end

function BloodQueenLanathel_OnCombatExit(pUnit, event)
	pUnit:PlaySoundToSet(16789)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(37955, 1, "BloodQueenLanathel_OnEnterCombat")
RegisterUnitEvent(37955, 2, "BloodQueenLanathel_OnCombatExit")
RegisterUnitEvent(37955, 3, "BloodQueenLanathel_Slay")
RegisterUnitEvent(37955, 4, "BloodQueenLanathel_Death")