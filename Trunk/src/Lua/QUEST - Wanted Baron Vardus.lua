local AllowSpawn = 1

function Baron_Vardus_Quest_onAccept (pUnit, Event)
	if (AllowSpawn == 1) then
		spin = math.random (1, 4)
		
		if (spin == 1) then
			pUnit:SpawnCreature (2306, 692.64, -904.74, 157.79, 1.17511, 14, 1800000)
		elseif (spin == 2) then
			pUnit:SpawnCreature (2306, 939.0, -852.46, 114.644, 1.17511, 14, 1800000)
		elseif (spin == 3) then
			pUnit:SpawnCreature (2306, 1184.07, -553.43, 71.3346, 1.17511, 14, 1800000)
		elseif (spin == 4) then
			pUnit:SpawnCreature (2306, 1001.20, -793.93, 108.65, 1.17511, 14, 1800000)
		end
	end
	AllowSpawn = 0
end

function Baron_Vardus_onDead (pUnit, Event)
	AllowSpawn = 1
end

RegisterQuestEvent(566, 2,"Baron_Vardus_Quest_onAccept")
RegisterUnitEvent (2306, 4, "Baron_Vardus_onDead") 