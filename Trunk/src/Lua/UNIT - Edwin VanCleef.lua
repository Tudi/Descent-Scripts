local phase = 1
function EdwinVanCleef_OnAggro(pUnit, Event)
	pUnit:SendChatMessage(12, 0,"None may challenge the brotherhood.")
	pUnit:PlaySoundToSet(5780)
	pUnit:RegisterEvent("EdwinVanCleef_PeriodicCheck", 1000 ,0)
end

function EdwinVanCleef_PeriodicCheck(pUnit, Event)
	if (Unit:GetHealthPct() <= 75) and (phase == 1) then
		pUnit:SendChatMessage(12, 0,"Lap dogs, all of you.")
		pUnit:PlaySoundToSet(5782)
		phase = 2
	elseif (Unit:GetHealthPct() <= 50) and (phase == 2) then
		pUnit:SendChatMessage(12, 0,"Fools! Our cause is righteous.")
		pUnit:PlaySoundToSet(5783)
		SpawnCreature(636, -77.8071, -815.097, 40.0188, 3.26377, 17, 0)
		SpawnCreature(636, -78.3562, -825.781, 40.0007, 1.49366, 17, 0)
		phase = 3
	elseif (Unit:GetHealthPct() <= 25) and (phase == 3) then
		pUnit:SendChatMessage(12, 0,"The brotherhood shall remain.")
		pUnit:PlaySoundToSet(5784)
		phase = 4
	end
end

function EdwinVanCleef_OnTargetDied(pUnit, Event)
	pUnit:SendChatMessage(12, 0,"And stay down, $N.")
	pUnit:PlaySoundToSet(5781)
end

function EdwinVanCleef_OnDeath(pUnit, Event)
	pUnit:RemoveEvents()
end

function EdwinVanCleef_LeaveCombat(pUnit, Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(639, 1, "EdwinVanCleef_OnAggro")
RegisterUnitEvent(639, 2, "EdwinVanCleef_LeaveCombat")
RegisterUnitEvent(639, 3, "EdwinVanCleef_OnTargetDied")
RegisterUnitEvent(639, 4, "EdwinVanCleef_OnDeath")