function DevourerofSouls_OnEnterCombat(pUnit, event)
	pUnit:SendChatMessage(14, 0, "You dare look upon the host of souls?! I SHALL DEVOUR YOU WHOLE!")
	pUnit:PlaySoundToSet(16884)
	pUnit:RegisterEvent("DevourerofSouls_PhantomBlast", 8000, 0)
--	pUnit:RegisterEvent("DevourerofSouls_MirroredSoul", 20000, 0)
	pUnit:RegisterEvent("DevourerofSouls_WellofSouls", 17500, 0)
	pUnit:RegisterEvent("DevourerofSouls_UnleashedSouls", 15000, 0)
	pUnit:RegisterEvent("DevourerofSouls_WailingSouls", 20000, 0)
end

function DevourerofSouls_PhantomBlast(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if(player ~= nil) then
		pUnit:FullCastSpellOnTarget(68982,player)
	end
end

function DevourerofSouls_MirroredSoul(pUnit, event)
	Unit:SendChatMessage(42, 0, "Devourer of Souls begins to cast Mirrored Soul!")
	local player = pUnit:GetRandomPlayer(7)
	if(player ~= nil) then
		pUnit:FullCastSpellOnTarget(69051,player)
	end
end

function DevourerofSouls_WellofSouls(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if(player ~= nil) then
		pUnit:CastSpellOnTarget(68820,player)
	end
end

function DevourerofSouls_UnleashedSouls(pUnit, event)
	Unit:SendChatMessage(42, 0, "Devourer of Souls begins to Unleash Souls!")
	pUnit:PlaySoundToSet(16888)
	pUnit:SendChatMessage(14, 0, "SUFFERING! ANGUISH! CHAOS! RISE AND FEED!")
	pUnit:FullCastSpell(68939)
end

function DevourerofSouls_WailingSouls(pUnit, event)
	Unit:SendChatMessage(42, 0, "Devourer of Souls begins to cast Wailing Souls!")
	pUnit:PlaySoundToSet(16888)
	pUnit:SendChatMessage(14, 0, "Stare into the abyss, and see your end!")
	pUnit:RegisterEvent("DevourerofSouls_WailingSoulsCast", 400, 10)
end

function DevourerofSouls_WailingSoulsCast(pUnit, event)
	pUnit:CastSpell(68873)
end

function DevourerofSouls_Slay(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
			pUnit:SendChatMessage(14, 0, "Damnation! Doomed for eternity!")
		pUnit:PlaySoundToSet(16885)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Fodder for the engine!")
		pUnit:PlaySoundToSet(16886)
	end
	pUnit:PlaySoundToSet(13329)
end

function DevourerofSouls_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "The swell of souls will not be abated! You only delay the inevitable!")
	pUnit:PlaySoundToSet(16887)
	pUnit:RemoveEvents()
end

function DevourerofSouls_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(36502, 1, "DevourerofSouls_OnEnterCombat")
RegisterUnitEvent(36502, 2, "DevourerofSouls_OnCombatExit")
RegisterUnitEvent(36502, 3, "DevourerofSouls_Slay")
RegisterUnitEvent(36502, 4, "DevourerofSouls_Death")