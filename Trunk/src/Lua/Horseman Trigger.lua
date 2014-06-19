function Trigger_OnSpawn(Unit, Event)
	Unit:SetFaction(35)
	Unit:RegisterEvent("Trigger_ThrowPumpkin", 4800, 1)
	Unit:RegisterEvent("Trigger_SummonShrine", 5800, 1)
	Unit:RegisterEvent("Trigger_Laugh1", 10000, 1)
	Unit:RegisterEvent("Trigger_EndEvent", 14000, 1)
end

function Trigger_ThrowPumpkin(Unit, Event)
	Unit:CastSpellAoF(-4738.006348, 3076.176025, 94.725266, 43715)
end

function Trigger_SummonShrine(Unit, Event)
	Unit:SpawnCreature(111052, -4738.165039, 3076.000000, 95.067508, 0.000000, 35, 80000)
end

function Trigger_Laugh1(Unit, Event)
	Unit:PlaySoundToSet(11965)
end

function Trigger_EndEvent(Unit, Event)
	Unit:Despawn(2000, 0)
end

function Trigger_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Trigger_OnDeath(Unit, Event)
	Unit:RemoveEvents()
end

RegisterUnitEvent(111051, 2, "Trigger_OnLeaveCombat")
RegisterUnitEvent(111051, 4, "Trigger_OnDeath")
RegisterUnitEvent(111051, 18, "Trigger_OnSpawn")