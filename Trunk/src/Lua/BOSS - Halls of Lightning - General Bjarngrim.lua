function GeneralBjarngrim_OnEnterCombat(pUnit, event)
	pUnit:PlaySoundToSet(14149)
	pUnit:SendChatMessage(14, 0, "I am the greatest of my father's sons! Your end has come!!")
	local rand = math.random(1,3)
	if rand == 1 then
		pUnit:RegisterEvent("GeneralBjarngrim_DefensiveStance", 20000, 1)
	elseif rand == 2 then
		pUnit:RegisterEvent("GeneralBjarngrim_BerserkerStance", 20000, 1)
	elseif rand == 3 then
		pUnit:RegisterEvent("GeneralBjarngrim_BattleStance", 20000, 1)
	end
end

function GeneralBjarngrim_DefensiveStance(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:RemoveAura(53792) 
	pUnit:RemoveAura(41106) 
	pUnit:CastSpell(53790)
	pUnit:CastSpell(41105)
	pUnit:PlaySoundToSet(14150)
	pUnit:SendChatMessage(14, 0, "Give me your worst!")
	pUnit:RegisterEvent("GeneralBjarngrim_SpellReflect", 12000, 0)
	pUnit:RegisterEvent("GeneralBjarngrim_Intercept", 4000, 1)
	pUnit:RegisterEvent("GeneralBjarngrim_Pummel", 8000, 0)
	pUnit:RegisterEvent("GeneralBjarngrim_KnockAway", 15000, 0)
	pUnit:RegisterEvent("GeneralBjarngrim_BerserkerStance", 20000, 1)
end

function GeneralBjarngrim_BerserkerStance(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:RemoveAura(53790) 
	pUnit:RemoveAura(41105) 
	pUnit:CastSpell(53791)
	pUnit:CastSpell(41107)
	pUnit:PlaySoundToSet(14152)
	pUnit:SendChatMessage(14, 0, "GRAAAAAH! Behold the fury of iron and steel!")
	pUnit:RegisterEvent("GeneralBjarngrim_MortalStrike", 8000, 0)
	pUnit:RegisterEvent("GeneralBjarngrim_Whirlwind", 13000, 1)
	pUnit:RegisterEvent("GeneralBjarngrim_BattleStance", 20000, 1)
end

function GeneralBjarngrim_BattleStance(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:RemoveAura(53791) 
	pUnit:RemoveAura(41107) 
	pUnit:CastSpell(53792)
	pUnit:CastSpell(41106)
	pUnit:PlaySoundToSet(14151)
	pUnit:SendChatMessage(14, 0, "Defend yourself, for all the good it will do!")
	pUnit:RegisterEvent("GeneralBjarngrim_Cleave", 7500, 0)
	pUnit:RegisterEvent("GeneralBjarngrim_DefensiveStance", 20000, 1)
end

function GeneralBjarngrim_SpellReflect(pUnit, event)
	pUnit:CastSpell(36096)
end

function GeneralBjarngrim_KnockAway(pUnit, event)
	pUnit:CastSpell(52029)
end

function GeneralBjarngrim_Intercept(pUnit, event)
	local player = pUnit:GetRandomPlayer(2)
	if(player ~= nil) then
		pUnit:CastSpellOnTarget(58769,player)
	end
end

function GeneralBjarngrim_Pummel(pUnit, event)
	local player = pUnit:GetRandomPlayer(1)
	if(player ~= nil) then
		pUnit:CastSpellOnTarget(12555,player)
	end
end

function GeneralBjarngrim_MortalStrike(pUnit, event)
	pUnit:CastSpellOnTarget(16856,pUnit:GetMainTank())
end

function GeneralBjarngrim_Whirlwind(pUnit, event)
	pUnit:CastSpell(52027)
end

function GeneralBjarngrim_Cleave(pUnit, event)
	pUnit:CastSpellOnTarget(15284,pUnit:GetMainTank())
end

function GeneralBjarngrim_Slay(pUnit, event)
	local rand = math.random(1,3)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "So ends your curse!")
		pUnit:PlaySoundToSet(14153)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Flesh... is... weak!")
		pUnit:PlaySoundToSet(14154)
	elseif rand == 3 then
		pUnit:SendChatMessage(14, 0, "BUGA BUGA BUGA BLABLA!")
		pUnit:PlaySoundToSet(14155)
	end
end

function GeneralBjarngrim_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "How can it be...? Flesh is not... stronger!")
	pUnit:PlaySoundToSet(14156)
	pUnit:RemoveEvents()
end

function GeneralBjarngrim_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(28586, 1, "GeneralBjarngrim_OnEnterCombat")
RegisterUnitEvent(28586, 2, "GeneralBjarngrim_OnCombatExit")
RegisterUnitEvent(28586, 3, "GeneralBjarngrim_Slay")
RegisterUnitEvent(28586, 4, "GeneralBjarngrim_Death")