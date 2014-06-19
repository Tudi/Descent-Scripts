function NWM_OnEnterCombat(Unit, Event)
	Unit:SendChatMessage(14, 0, "The conservatory must be protected!") 
	Unit:PlaySoundToSet(15526)
	Unit:RegisterEvent("NWM_Swarm1", 24000, 1)
	Unit:RegisterEvent("NWM_Swarm2", 108000, 1)
	Unit:RegisterEvent("NWM_Swarm3", 180000, 1)
	Unit:RegisterEvent("NWM_Reduct", 20800, 0)
	Unit:RegisterEvent("NWM_Enrage", 400000, 0)
	Unit:RegisterEvent("NWM_Growth", 42000, 5)
	Unit:RegisterEvent("NWM_Elders1", 66000, 1)
	Unit:RegisterEvent("NWM_Elders2", 140000, 1)
	Unit:RegisterEvent("NWM_Elders3", 198000, 1)
	Unit:RegisterEvent("NWM_Gift", 28000, 0)
	Unit:RegisterEvent("NWM_Mystic1", 70600, 1)
	Unit:RegisterEvent("NWM_Mystic2", 144600, 1)
	Unit:RegisterEvent("NWM_Mystic3", 202600, 1)
	Unit:RegisterEvent("NWM_Phase2", 202800, 1)
	Unit:RegisterEvent("PeriodicGainAHealstack", 500, 10)
	Unit:RegisterEvent("NWM_Check1", 100, 0)
end

function PeriodicGainAHealstack(Unit, Event)
	Unit:CastSpell(62519)
	Unit:CastSpell(62519)
	Unit:CastSpell(62519)
	Unit:CastSpell(62519)
	Unit:CastSpell(62519)
	Unit:CastSpell(62519)
	Unit:CastSpell(62519)
	Unit:CastSpell(62519)
	Unit:CastSpell(62519)
	Unit:CastSpell(62519)
end

function NWM_Swarm1(Unit, Event)
	Unit:PlaySoundToSet(15534)
	Unit:SendChatMessage(14, 0, "The Swarm of the elements shall overtake you!")
	x = Unit:GetX()
	y = Unit:GetY()
	z = Unit:GetZ()
	o = Unit:GetO()
	Unit:SpawnCreature(111008, x+8, y+8, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x+9, y+9, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x+10, y+10, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x-8, y-8, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x-9, y-9, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x-10, y-10, z, o, 16, 600000)
end

function NWM_Swarm2(Unit, Event)
	Unit:PlaySoundToSet(15534)
	Unit:SendChatMessage(14, 0, "The Swarm of the elements shall overtake you!")
	x = Unit:GetX()
	y = Unit:GetY()
	z = Unit:GetZ()
	o = Unit:GetO()
	Unit:SpawnCreature(111008, x+8, y+8, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x+9, y+9, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x+10, y+10, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x-8, y-8, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x-9, y-9, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x-10, y-10, z, o, 16, 600000)
end

function NWM_Swarm3(Unit, Event)
	Unit:PlaySoundToSet(15534)
	Unit:SendChatMessage(14, 0, "The Swarm of the elements shall overtake you!")
	x = Unit:GetX()
	y = Unit:GetY()
	z = Unit:GetZ()
	o = Unit:GetO()
	Unit:SpawnCreature(111008, x+8, y+8, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x+9, y+9, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x+10, y+10, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x-8, y-8, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x-9, y-9, z, o, 16, 600000)
	Unit:SpawnCreature(111008, x-10, y-10, z, o, 16, 600000)
end

function NWM_Gift(Unit, Event)
local plr = Unit:GetRandomPlayer(0)
	if plr ~= nil then
	local x,y,z,o = plr:GetX(),plr:GetY(),plr:GetZ(),plr:GetO()
	Unit:SpawnCreature(111009, x,y,z,o , 21, 360000)
	Unit:PlaySoundToSet(15527)
	Unit:SendChatMessage(14, 0, "Elders… Grant me your strength!")
	end
end

function NWM_Elders1(Unit, Event)
	Unit:PlaySoundToSet(15533)
	Unit:SendChatMessage(14, 0, "Children assist me!")
	Unit:SpawnCreature(111010, x, y, z,  4, 14, 600000)
end

function NWM_Elders2(Unit, Event)
	Unit:PlaySoundToSet(15533)
	Unit:SendChatMessage(14, 0, "Children assist me!")
	Unit:SpawnCreature(111010, x, y, z,  4, 14, 600000)
end

function NWM_Elders3(Unit, Event)
	Unit:PlaySoundToSet(15533)
	Unit:SendChatMessage(14, 0, "Children assist me!")
	Unit:SpawnCreature(111010, x, y, z,  4, 14, 600000)
end

function NWM_Mystic1(Unit, Event)
	Unit:SpawnCreature(111016, x, y, z,  4, 14, 600000)
	Unit:SpawnCreature(111016, x, y, z,  4, 14, 600000)
end

function NWM_Mystic2(Unit, Event)
	Unit:SpawnCreature(111016, x, y, z,  4, 14, 600000)
	Unit:SpawnCreature(111016, x, y, z,  4, 14, 600000)
end

function NWM_Mystic3(Unit, Event)
	Unit:SpawnCreature(111016, x, y, z,  4, 14, 600000)
	Unit:SpawnCreature(111016, x, y, z,  4, 14, 600000)
end

function NWM_Reduct(Unit, Event)
	Unit:PlaySoundToSet(15528)
	Unit:SendChatMessage(14, 0, "Eonar, your servant requires aid!")
	Unit:FullCastSpell(62525)
end

function NWM_Phase2(Unit, Event)
	Unit:SendChatMessage(14, 0, "Enough of this!")
	Unit:RegisterEvent("NWM_Sunbeam", 4000, 0)
end

function NWM_Sunbeam(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(62623, rand)
	end
end

function NWM_OnKilledTarget(Unit, Event)
	Unit:PlaySoundToSet(15530)
	Unit:SendChatMessage(14, 0, "From your deaths springs life anew!")
end

function NWM_Growth(Unit, Event)
	Unit:FullCastSpell(62584)
end

function NWM_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
	Unit:RemoveAllAuras()
	Unit:ReturnToSpawnPoint()
end

function NWM_Check1(Unit, Event)
    if Unit:GetHealthPct() <= 1 then
        Unit:PlaySoundToSet(15531)
	Unit:SendChatMessage(14, 0, "His hold on me disapates... I, can see clearly once more. Thank you heroes.")
    end
end

function NWM_OnDeath(Unit, Event)
end

function NWM_Enrage(Unit, Event)
	Unit:FullCastSpell(27680)
	Unit:PlaySoundToSet(82395)
	Unit:SendChatMessage(14, 0, "You have strayed to far, wasted to much time!")
end

RegisterUnitEvent(111006, 1, "NWM_OnEnterCombat")
RegisterUnitEvent(111006, 2, "NWM_OnLeaveCombat")
RegisterUnitEvent(111006, 3, "NWM_OnKilledTarget")
RegisterUnitEvent(111006, 4, "NWM_Death")