function Mildred_OnSpawn(Unit, Event)
	Unit:FullCastSpell(83369)
	Unit:RegisterEvent("Mildred_ChangeFaction", 22000, 1)
	Unit:RegisterEvent("Mildred_Walk1", 1000, 1)
	Unit:PlaySoundToSet(8799)
	Unit:SendChatMessage(14, 0, "Your old lives, your mortal desires, mean nothing. You are acolytes of the master now, and you will serve the cause without question! The greatest glory is to die in the master's service.")
end

function Mildred_Walk1(Unit, Event)
	Unit:MoveTo(-4659.0097, -2613.5466, 21.9006, 0.0196)
end

function Mildred_ChangeFaction(Unit, Event)
	Unit:SetFaction(16)
end

function Mildred_OnCombat(Unit, Event)
	Unit:PlaySoundToSet(8794)
	Unit:SendChatMessage(14, 0, "Slay them in the Master's name!")		Unit:RegisterEvent("Mildred_WebSpray", 28000, 0)
	Unit:RegisterEvent("Mildred_NecroticPoison", 16000, 0)
	Unit:RegisterEvent("Mildred_NecroticPoison2", 4000, 0)
	Unit:RegisterEvent("Mildred_ShadowNova", 14000, 0)
	Unit:RegisterEvent("Mildred_Defile", 200000, 0)
	Unit:RegisterEvent("Mildred_Enrage", 280000, 0)
	Unit:RegisterEvent("Mildred_Check1", 100, 0)
end

function Mildred_WebSpray(Unit, Event)
	Unit:FullCastSpell(29484)
	Unit:PlaySoundToSet(8794)
	Unit:SendChatMessage(14, 0, "Slay them in the Master's name!")
end

function Mildred_NecroticPoison(Unit, Event)
	Unit:CastSpellOnTarget(28776, Unit:GetMainTank())
end

function Mildred_NecroticPoison2(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(28776, rand)
	Unit:PlaySoundToSet(8795)
	Unit:SendChatMessage(14, 0, "You cannot hide from me!")
	end
end

function Mildred_ShadowNova(Unit, Event)
	Unit:FullCastSpell(65209)
end

function Mildred_Defile(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(72743, rand)
	Unit:PlaySoundToSet(8797)
	Unit:SendChatMessage(14, 0, "Run while you still can!")
	end
end

function Mildred_Enrage(Unit, Event)
	Unit:FullCastSpell(47008)
	Unit:PlaySoundToSet(8800)
	Unit:SendChatMessage(14, 0, "You have failed!")
end

function Mildred_OnKilledTarget(Unit, Event)
	Unit:PlaySoundToSet(8796)
	Unit:SendChatMessage(14, 0, "Pathetic Wrench!")
end

function Mildred_OnLeaveCombat(Unit, Event)
	Unit:PlaySoundToSet(8800)
	Unit:SendChatMessage(14, 0, "You have failed!")
	Unit:SpawnCreature(111017, -4650.0649, -2613.5583, 22, 6.2745)
	Unit:RemoveEvents()	
	Unit:Despawn(2000, 0)
end

function Mildred_Check1(Unit, Event)
   if Unit:GetHealthPct() <= 1 then
        Unit:SendWorldMessage("Lady Mildred has been defeated once more within the Halls of Mortality. Tranquility slowly sets in over Azeroth.", 2)
    end
end

function Mildred_OnDeath(Unit, Event)
	Unit:PlaySoundToSet(8801)
	Unit:SendChatMessage(14, 0, "The MasterÉ will avenge me.")

end

RegisterUnitEvent(111019, 1, "Mildred_OnCombat")
RegisterUnitEvent(111019, 2, "Mildred_OnLeaveCombat")
RegisterUnitEvent(111019, 3, "Mildred_OnKilledTarget")
RegisterUnitEvent(111019, 4, "Mildred_Death")
RegisterUnitEvent(111019, 18, "Mildred_OnSpawn")