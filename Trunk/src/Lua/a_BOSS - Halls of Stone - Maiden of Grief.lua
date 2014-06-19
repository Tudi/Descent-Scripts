function MaidenofGrief_OnEnterCombat(pUnit, event)
	pUnit:SendChatMessage(14, 0, "You shouldn't have come...now you will die!")
	pUnit:PlaySoundToSet(13487)
	pUnit:RegisterEvent("MaidenofGrief_PartingSorrow", 7000, 0)
	pUnit:RegisterEvent("MaidenofGrief_PillarofWoe", 12000, 0)
	pUnit:RegisterEvent("MaidenofGrief_ShockofSorrow", 30000, 0)
	pUnit:RegisterEvent("MaidenofGrief_StormofGrief", 20000, 0)
end

function MaidenofGrief_PartingSorrow(pUnit, event)
	local player = pUnit:GetRandomPlayer(4)
	if player ~= nil then
		pUnit:FullCastSpellOnTarget(59723,player)
	end
end

function MaidenofGrief_PillarofWoe(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if player ~= nil then
		pUnit:FullCastSpellOnTarget(50761,player)
	end
end

function MaidenofGrief_ShockofSorrow(pUnit, event)
	pUnit:SendChatMessage(14, 0, "So much lost time... that you'll never get back!")
	pUnit:PlaySoundToSet(13492)
	pUnit:FullCastSpell(50760)
end

function MaidenofGrief_StormofGrief(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if player ~= nil then
		pUnit:CastSpellAoF(player:GetX(), player:GetY(), player:GetZ(), 50752)
	end
end

function MaidenofGrief_Slay(pUnit, event)
	local rand = math.random(1,4)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Why must it be this way?")
		pUnit:PlaySoundToSet(13488)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "You had it coming!")
		pUnit:PlaySoundToSet(13489)
	elseif rand == 3 then
		pUnit:SendChatMessage(14, 0, "My burden grows heavier.")
		pUnit:PlaySoundToSet(13490)
	elseif rand == 4 then
		pUnit:SendChatMessage(14, 0, "This is your own fault!")
		pUnit:PlaySoundToSet(13491)
	end
end

function MaidenofGrief_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "I hope you all rot! I never...wanted...this.")
	pUnit:PlaySoundToSet(13493)
	pUnit:RemoveEvents()
end

function MaidenofGrief_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(27975, 1, "MaidenofGrief_OnEnterCombat")
RegisterUnitEvent(27975, 2, "MaidenofGrief_OnCombatExit")
RegisterUnitEvent(27975, 3, "MaidenofGrief_Slay")
RegisterUnitEvent(27975, 4, "MaidenofGrief_Death")