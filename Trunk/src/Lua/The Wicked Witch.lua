function Witch_OnCombat(Unit, Event)
	Unit:FullCastSpell(45813)
	Unit:PlaySoundToSet(9179)
	Unit:SendChatMessage(14, 0, "Woe to each and every one of you my pretties!")
	Unit:RegisterEvent("Witch_LavaMobs", 34000, 0)
	Unit:RegisterEvent("Witch_LightningBlast", 2800, 0)
	Unit:RegisterEvent("Witch_FireNova", 6000, 0)
	Unit:RegisterEvent("Witch_Incinerate", 14000, 0)
	Unit:RegisterEvent("Witch_Enrage", 240000, 0)
end

function Witch_LavaMobs(Unit, Event)
	Unit:FullCastSpell(99575)
end

function Witch_LightningBlast(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(88357, rand)
	end
end

function Witch_FireNova(Unit, Event)
	Unit:FullCastSpell(97721)
end

function Witch_Incinerate(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(102573, rand)
	end
end

function Witch_Enrage(Unit, Event)
	Unit:FullCastSpell(47008)
end

function Witch_OnKilledTarget(Unit, Event)
	Unit:PlaySoundToSet(9180)
	Unit:SendChatMessage(14, 0, "Fixed you, didn't I?")
end

function Witch_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
	Unit:RemoveAuras()
end

function Witch_OnDeath(Unit, Event)
	Unit:PlaySoundToSet(9178)
	Unit:SendChatMessage(14, 0, "How could you? What a cruel, cruel world!")
	Unit:RemoveAuras()
end

RegisterUnitEvent(111040, 1, "Witch_OnCombat")
RegisterUnitEvent(111040, 2, "Witch_OnLeaveCombat")
RegisterUnitEvent(111040, 3, "Witch_OnKilledTarget")
RegisterUnitEvent(111040, 4, "Witch_OnDeath")