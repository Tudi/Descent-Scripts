---------------------------------------------
--- SCRIPT MADE BY QABOOMBA FOR WOWBEEZ -----
----- ALL CREDITS GO TO QABOOMBA ------------
---------------------------------------------

function Hodir_OnCombat(Unit, Event)
	Unit:SendChatMessage(14, 0, "You will suffer for this trespass!")
	Unit:PlaySoundToSet(15552)
	Unit:PlaySoundToSet(15563)
	Unit:RegisterEvent("Hodir_Freeze", 16131, 0)
	Unit:RegisterEvent("Hodir_Icicle", 67231, 0)
	Unit:RegisterEvent("Hodir_Frostbolt", 17053, 0)
	Unit:RegisterEvent("Hodir_FrostFever", 11050, 0)
	Unit:RegisterEvent("Hodir_Aah", 17031, 0)
	Unit:RegisterEvent("Hodir_Ah", 24134, 0)
	Unit:RegisterEvent("Hodir_FrozenBlows", 64012, 0)
	Unit:RegisterEvent("Hodir_FrostBreath", 14005, 0)
	Unit:RegisterEvent("Hodir_Flash", 90321, 0)
	Unit:RegisterEvent("Hodir_Phase2", 1000, 0)
end

function Hodir_Freeze(Unit, Event)
	Unit:CastSpellOnTarget(62469, Unit:GetRandomPlayer(0))
end

function Hodir_Icicle(Unit, Event)
	local target = 	Unit:GetRandomPlayer(0)
	local x, y, z = target:GetX(), target:GetY(), target:GetZ()
	if (target ~= nil) then
		Unit:CastSpellAoF(x, y, z, 62460)
	end
end

function Hodir_Frostbolt(Unit, Event)
	Unit:FullCastSpellOnTarget(38837, Unit:GetRandomPlayer(0))
end

function Hodir_FrostFever(Unit, Event)
	Unit:CastSpellOnTarget(67932, Unit:GetRandomPlayer(0))
end

function Hodir_Aah(Unit, Event)
	Unit:PlaySoundToSet(15561)
end

function Hodir_Ah(Unit, Event)
	Unit:PlaySoundToSet(15561)
end

function Hodir_FrozenBlows(Unit, Event)
	Unit:CastSpell(62478)
end

function Hodir_FrostBreath(Unit, Event)
	Unit:CastSpellOnTarget(50505, Unit:GetRandomPlayer(0))
end

function Hodir_Flash(Unit, Event)
	Unit:FullCastSpell(61968)
	Unit:PlaySoundToSet(15562)
end

function Hodir_Phase2(Unit, Event)
	if Unit:GetHealthPct() <= 80 then
		Unit:RemoveEvents()
		Unit:RegisterEvent("Hodir_Winds", 5000, 1)
		Unit:RegisterEvent("Hodir_FrostNova", 10000, 15)
		Unit:RegisterEvent("Hodir_Voice", 20000, 4)
		Unit:RegisterEvent("Hodir_Frostbolt", 26503, 0)
		Unit:RegisterEvent("Hodir_Phase3", 1000, 0)
	end
end

function Hodir_Winds(Unit, Event)
	Unit:SendChatMessage(14, 0, "Winds of the North consume you")
	Unit:PlaySoundToSet(15555)
end

function Hodir_FrostNova(Unit, Event)
	Unit:FullCastSpell(62597)
end

function Hodir_Voice(Unit, Event)
	Unit:PlaySoundToSet(15555)
	Unit:SendChatMessage(14, 0, "Winds of the North consume you!")
end

function Hodir_Frostbolt(Unit, Event)
	Unit:FullCastSpellOnTarget(38837, Unit:GetRandomPlayer(7))
end

function Hodir_Phase3(Unit, Event)
	if Unit:GetHealthPct() <= 75 then
		Unit:RemoveEvents()
		Unit:RegisterEvent("Hodir_Freeze", 16131, 0)
		Unit:RegisterEvent("Hodir_Icicle", 67231, 0)
		Unit:RegisterEvent("Hodir_Frostbolt", 17053, 0)
		Unit:RegisterEvent("Hodir_FrostFever", 11050, 0)
		Unit:RegisterEvent("Hodir_Aah", 17031, 0)
		Unit:RegisterEvent("Hodir_Ah", 24134, 0)
		Unit:RegisterEvent("Hodir_FrozenBlows", 64012, 0)
		Unit:RegisterEvent("Hodir_FrostBreath", 14005, 0)
		Unit:RegisterEvent("Hodir_Flash", 90321, 0)
		Unit:RegisterEvent("Hodir_Phase4", 1000, 0)
	end
end

function Hodir_Freeze(Unit, Event)
	Unit:CastSpellOnTarget(62469, Unit:GetRandomPlayer(0))
end

function Hodir_Icicle(Unit, Event)
	local target = 	Unit:GetRandomPlayer(0)
	local x, y, z = target:GetX(), target:GetY(), target:GetZ()
	if (target ~= nil) then
		Unit:CastSpellAoF(x, y, z, 62460)
	end
end

function Hodir_Frostbolt(Unit, Event)
	Unit:FullCastSpellOnTarget(38837, Unit:GetRandomPlayer(0))
end

function Hodir_FrostFever(Unit, Event)
	Unit:CastSpellOnTarget(67932, Unit:GetRandomPlayer(0))
end

function Hodir_Aah(Unit, Event)
	Unit:PlaySoundToSet(15561)
end

function Hodir_Ah(Unit, Event)
	Unit:PlaySoundToSet(15561)
end

function Hodir_FrozenBlows(Unit, Event)
	Unit:CastSpell(62478)
end

function Hodir_FrostBreath(Unit, Event)
	Unit:CastSpellOnTarget(50505, Unit:GetRandomPlayer(0))
end

function Hodir_Flash(Unit, Event)
	Unit:FullCastSpell(61968)
	Unit:PlaySoundToSet(15562)
end

function Hodir_Phase4(Unit, Event)
	if Unit:GetHealthPct() <= 45 then
		Unit:RemoveEvents()
		Unit:RegisterEvent("Hodir_Winds", 5000, 1)
		Unit:RegisterEvent("Hodir_FrostNova", 10000, 15)
		Unit:RegisterEvent("Hodir_Voice", 20000, 4)
		Unit:RegisterEvent("Hodir_Frostbolt", 26503, 0)
		Unit:RegisterEvent("Hodir_Phase5", 1000, 0)
	end
end

function Hodir_Winds(Unit, Event)
	Unit:SendChatMessage(14, 0, "Winds of the North consume you!")
	Unit:PlaySoundToSet(15555)
end

function Hodir_FrostNova(Unit, Event)
	Unit:FullCastSpell(62597)
end

function Hodir_Voice(Unit, Event)
	Unit:PlaySoundToSet(15555)
	Unit:SendChatMessage(14, 0, "Winds of the North consume you!")
end

function Hodir_Frostbolt(Unit, Event)
	Unit:FullCastSpellOnTarget(38837, Unit:GetRandomPlayer(7))
end

function Hodir_Phase5(Unit, Event)
	if Unit:GetHealthPct() <= 40 then
		Unit:RemoveEvents()
		Unit:RegisterEvent("Hodir_Thunder", 5000, 1)
		Unit:RegisterEvent("Hodir_Freeze", 16131, 0)
		Unit:RegisterEvent("Hodir_Icicle", 67231, 0)
		Unit:RegisterEvent("Hodir_Frostbolt", 25032, 0)
		Unit:RegisterEvent("Hodir_FrostFever", 11050, 0)
		Unit:RegisterEvent("Hodir_Aah", 17031, 0)
		Unit:RegisterEvent("Hodir_Ah", 24134, 0)
		Unit:RegisterEvent("Hodir_FrozenBlows", 64012, 0)
		Unit:RegisterEvent("Hodir_FrostBreath", 14005, 0)
		Unit:RegisterEvent("Hodir_Flash", 90321, 0)
		Unit:RegisterEvent("Hodir_Berserk", 362055, 1)
	end
end

function Hodir_Thunder(Unit, Event)
	Unit:PlaySoundToSet(15563)
end

function Hodir_Freeze(Unit, Event)
	Unit:CastSpellOnTarget(62469, Unit:GetRandomPlayer(0))
end

function Hodir_Icicle(Unit, Event)
	local target = 	Unit:GetRandomPlayer(0)
	local x, y, z = target:GetX(), target:GetY(), target:GetZ()
	if (target ~= nil) then
		Unit:CastSpellAoF(x, y, z, 62460)
	end
end

function Hodir_Frostbolt(Unit, Event)
	Unit:PlaySoundToSet(15556)
	Unit:FullCastSpellOnTarget(38837, Unit:GetRandomPlayer(0))
end

function Hodir_FrostFever(Unit, Event)
	Unit:CastSpellOnTarget(67932, Unit:GetRandomPlayer(0))
end

function Hodir_Aah(Unit, Event)
	Unit:PlaySoundToSet(15561)
end

function Hodir_Ah(Unit, Event)
	Unit:PlaySoundToSet(15561)
end

function Hodir_FrozenBlows(Unit, Event)
	Unit:CastSpell(62478)
end

function Hodir_FrostBreath(Unit, Event)
	Unit:CastSpellOnTarget(50505, Unit:GetRandomPlayer(0))
end

function Hodir_Flash(Unit, Event)
	Unit:FullCastSpell(61968)
	Unit:PlaySoundToSet(15562)
end

function Hodir_Berserk(Unit, Event)
	Unit:SendChatMessage(14, 0, "Enough! This ends now!")
	Unit:PlaySoundToSet(15558)
	Unit:FullCastSpell(41107)
end

function Hodir_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Hodir_OnKilledTarget(Unit, Event)
	Unit:PlaySoundToSet(15553)
	Unit:SendChatMessage(14, 0, "Tragic. To come so far, only to fail.")
end

function Hodir_OnDeath(Unit, Event)
	Unit:PlaySoundToSet(15557)
	Unit:SendChatMessage(14, 0, "I... I am released from his grasp.. at last.")
	Unit:RemoveEvents()
end

RegisterUnitEvent(32845, 1, "Hodir_OnCombat")
RegisterUnitEvent(32845, 2, "Hodir_OnLeaveCombat")
RegisterUnitEvent(32845, 3, "Hodir_OnKilledTarget")
RegisterUnitEvent(32845, 4, "Hodir_OnDeath")

---------------------------------------------
--- SCRIPT MADE BY QABOOMBA FOR WOWBEEZ -----
----- ALL CREDITS GO TO QABOOMBA ------------
---------------------------------------------