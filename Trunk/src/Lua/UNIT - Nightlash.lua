local AllowSpawn = 1

function spawn_Nightlash_spawn(unit)
	if (math.random (6) == 1) then
		unit:SpawnCreature(1983, 1081.495, 1545.082, 28.64, 0, 70, 600000)
		AllowSpawn = 0
	end
end

function Nightlash_onDead (pUnit, Event)
	AllowSpawn = 1
end

RegisterUnitEvent (1983, 4,"Nightlash_onDead")
RegisterUnitEvent (1773, 4, "spawn_Nightlash_spawn") 
RegisterUnitEvent (1772, 4, "spawn_Nightlash_spawn") 