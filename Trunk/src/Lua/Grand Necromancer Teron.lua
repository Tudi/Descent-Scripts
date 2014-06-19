function Teron_OnSpawn(Unit, Event)
	Unit:RegisterEvent("Teron_ChangeFaction", 6000, 1)
	Unit:FullCastSpell(59084)
end

function Teron_ChangeFaction(Unit, Event)
	Unit:SetFaction(16)
end

function Teron_OnCombat(Unit, Event)
	Unit:SendChatMessage(14, 0, "Finally, a captive audience!")
	Unit:PlaySoundToSet(16595)
	Unit:RegisterEvent("Teron_EarthernVortex", 8000, 0)
	Unit:RegisterEvent("Teron_SummonPortal", 16000, 0)
	Unit:RegisterEvent("Teron_DisruptingRoar", 10800, 0)
	Unit:RegisterEvent("Teron_FrostBomb", 36800, 0)
	Unit:RegisterEvent("Teron_Enrage", 240000, 0)
	Unit:RegisterEvent("Teron_Check1", 100, 0)
end

function Teron_EarthernVortex(Unit, Event)
	Unit:FullCastSpell(103821)
end

function Teron_SummonPortal(Unit, Event)
	Unit:PlaySoundToSet(16600)
	Unit:SendChatMessage(14, 0, "I will suffer your souls from your bodys.")
	x = Unit:GetX()
	y = Unit:GetY()
	z = Unit:GetZ()
	o = Unit:GetO()
	Unit:SpawnCreature(111021, x+8, y+8, z+1, o, 16, 600000)
	Unit:SpawnCreature(111021, x-14, y+16, z+1, o, 16, 600000)
end

function Teron_DisruptingRoar(Unit, Event)
	Unit:FullCastSpell(108044)
end

function Teron_FrostBolt(Unit, Event)
	Unit:FullCastSpell(101810)
end

function Teron_FrostBomb(Unit, Event)
	Unit:FullCastSpell(70521)
end

function Teron_Enrage(Unit, Event)
	Unit:FullCastSpell(26662)
end

function Teron_OnKilledTarget(Unit, Event)
end

function Teron_OnLeaveCombat(Unit, Event)
	Unit:SpawnCreature(111017, -4650.0649, -2613.5583, 22, 6.2745)
	Unit:Despawn(4000, 0)
end

function Teron_Check1(Unit, Event)
    if Unit:GetHealthPct() <= 1 then
        Unit:SpawnCreature(111019, -4691.1982, --2613.4086, 27.48, 0.0196)
	Unit:PlaySoundToSet(17105)
    end
end

function Teron_OnDeath(Unit, Event)
	Unit:Despawn(8000, 0)
end

RegisterUnitEvent(111018, 1, "Teron_OnCombat")
RegisterUnitEvent(111018, 2, "Teron_OnLeaveCombat")
RegisterUnitEvent(111018, 3, "Teron_OnKilledTarget")
RegisterUnitEvent(111018, 4, "Teron_Death")
RegisterUnitEvent(111018, 18, "Teron_OnSpawn")