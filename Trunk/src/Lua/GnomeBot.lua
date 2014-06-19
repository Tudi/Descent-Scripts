function gnome_OnCombat(Unit, Event)
	Unit:FullCastSpell(45813)
	Unit:RegisterEvent("gnome_ChainLightning", 8000, 0)
	Unit:RegisterEvent("gnome_PowerSurge", 18000, 0)
	Unit:RegisterEvent("gnome_SwarmBot", 22000, 0)
	Unit:RegisterEvent("gnome_Enrage", 140000, 0)
	Unit:RegisterEvent("gnome_Check1", 100, 0)
end

function gnome_ChainLightning(Unit, Event)
local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(97496, rand)
	end
end

function gnome_SwarmBot(Unit, Event)
	x = Unit:GetX()
	y = Unit:GetY()
	z = Unit:GetZ()
	o = Unit:GetO()
	Unit:SpawnCreature(34057, x+8, y+8, z, o, 16, 600000)
	Unit:SpawnCreature(34057, x+9, y+9, z, o, 16, 600000)
	Unit:SpawnCreature(34057, x+10, y+10, z, o, 16, 600000)
	Unit:SpawnCreature(34057, x+12, y-12, z, o, 16, 600000)
end

function gnome_PowerSurge(Unit, Event)
	Unit:FullCastSpell(60936)
end

function gnome_Enrage(Unit, Event)
	Unit:FullCastSpell(26662)
end

function gnome_OnLeaveCombat(Unit, Event)
	Unit:GetGameObjectNearestCoords(-4509.145020, -2561.877197, 31.966936, 45541):Activate()
	Unit:Despawn(6000, 0)
end

function gnome_OnDeath(Unit, Event)
	Unit:Despawn(4000, 0)
end

function gnome_OnKilledtarget(Unit, Event)
end

RegisterUnitEvent(111011, 1, "gnome_OnCombat")
RegisterUnitEvent(111011, 2, "gnome_OnLeaveCombat")
RegisterUnitEvent(111011, 3, "gnome_OnKilledTarget")
RegisterUnitEvent(111011, 4, "gnome_Death")