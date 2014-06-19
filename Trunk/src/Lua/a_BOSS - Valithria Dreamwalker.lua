local Valithria;

function ValithriaDreamwalker_Gossip(unit, event, player)
	unit:GossipCreateMenu(150, player, 0)
	unit:GossipMenuAddItem(0, "Start the boss fight", 1, 0)
	unit:GossipSendMenu(player)
end

function ValithriaDreamwalker_Submenus(pUnit, event, player, id, intid, code)
	if(intid == 1) then
		pUnit:RemoveEvents()
		pUnit:SetCombatCapable(0)
		pUnit:SetCombatTargetingCapable(0)
		pUnit:SetCombatMeleeCapable(0)
		pUnit:SendChatMessage(14, 0, "Heroes, lend me your aid! I... I cannot hold them off much longer! You must heal my wounds!")
		pUnit:RegisterEvent("ValithriaDreamwalker_ResetHealth", 750, 10)
		pUnit:RegisterEvent("ValithriaDreamwalker_CheckHighhealth", 1000, 0)
		pUnit:RegisterEvent("ValithriaDreamwalker_CheckLowhealth", 1000, 0)
		pUnit:RegisterEvent("ValithriaDreamwalker_EmeraldVigor", 2500, 0)
		pUnit:RegisterEvent("ValithriaDreamwalker_SpawnEnemiesCicle1", 1000, 1)
		pUnit:PlaySoundToSet(17064)
	end
end

function ValithriaDreamwalker_ResetHealth(pUnit, event)
	pUnit:SetHealthPct(50)
end

function ValithriaDreamwalker_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Forgive me for what I do! I... cannot... stop... ONLY NIGHTMARES REMAIN!")
	pUnit:PlaySoundToSet(17072)
	pUnit:RemoveEvents()
	pUnit:Despawn(5000,0)
end

function ValithriaDreamwalker_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

function ValithriaDreamwalker_Spawn(pUnit, event)
	Valithria = pUnit
	pUnit:Root()
	pUnit:RegisterEvent("ValithriaDreamwalker_ResetHealth", 750, 0)
end

function ValithriaDreamwalker_FightStartCheck(pUnit, event)
	local plr = pUnit:GetClosestPlayer()
	if plr ~= nil then
		if pUnit:GetDistance(plr) < 7 then
			
		end
	end
end

function ValithriaDreamwalker_CheckLowhealth(pUnit, event)
	if pUnit:GetHealthPct() <= 25 then
		pUnit:SendChatMessage(14, 0, "I will not last much longer!")
		pUnit:PlaySoundToSet(17069)
		pUnit:RemoveEvents()
		pUnit:RegisterEvent("ValithriaDreamwalker_CheckHighhealth", 1000, 0)
		pUnit:RegisterEvent("ValithriaDreamwalker_EmeraldVigor", 10000, 0)
		pUnit:RegisterEvent("ValithriaDreamwalker_SpawnEnemiesCicle1", 1000, 1)
	end
end

function ValithriaDreamwalker_CheckHighhealth(pUnit, event)
	if pUnit:GetHealthPct() >= 75 then
		pUnit:SendChatMessage(14, 0, "My strength is returning! Press on, heroes!")
		pUnit:PlaySoundToSet(17070)
		pUnit:RemoveEvents()
		pUnit:RegisterEvent("ValithriaDreamwalker_CheckLowhealth", 1000, 0)
		pUnit:RegisterEvent("ValithriaDreamwalker_Checkwin", 1000, 0)
		pUnit:RegisterEvent("ValithriaDreamwalker_EmeraldVigor", 2500, 0)
		pUnit:RegisterEvent("ValithriaDreamwalker_SpawnEnemiesCicle1", 1000, 1)
	end
end

function ValithriaDreamwalker_Checkwin(pUnit, event)
	if pUnit:GetHealthPct() >= 99 then
		pUnit:SendChatMessage(14, 0, "I am renewed! Ysera grants me the favor to lay these foul creatures to rest!")
		pUnit:PlaySoundToSet(17071)
		pUnit:FullCastSpell(71189)
		pUnit:RemoveEvents()
		pUnit:SpawnGameObject(201959, pUnit:GetX(), pUnit:Gety(), pUnit:GetZ(), pUnit:GetO(), 360000, 1)
		pUnit:Despawn(10000,0)
	end
end

function ValithriaDreamwalker_EmeraldVigor(pUnit, event)
	pUnit:CastSpell(70873,pUnit:GerRandomPlayer(0))
	pUnit:CastSpell(70873,pUnit:GerRandomPlayer(0))
	pUnit:CastSpell(70873,pUnit:GerRandomPlayer(0))
end

function ValithriaDreamwalker_SpawnEnemiesCicle1(pUnit, event)
	pUnit:SpawnCreature(37886, 4241.34, 2411.52, 365.06, 0, 16, 0)
	pUnit:SpawnCreature(37886, 4241.19, 2557.49, 365.06, 0, 16, 0)
	pUnit:RegisterEvent("ValithriaDreamwalker_SpawnEnemiesCicle2", 16000, 1)
end

function ValithriaDreamwalker_SpawnEnemiesCicle2(pUnit, event)
	pUnit:SpawnCreature(37863, 4241.34, 2411.52, 365.06, 0, 16, 0)
	pUnit:SpawnCreature(37863, 4241.19, 2557.49, 365.06, 0, 16, 0)
	pUnit:RegisterEvent("ValithriaDreamwalker_SpawnEnemiesCicle3", 12000, 1)
end

function ValithriaDreamwalker_SpawnEnemiesCicle3(pUnit, event)
	pUnit:SpawnCreature(36791, 4241.34, 2411.52, 365.06, 0, 16, 0)
	pUnit:SpawnCreature(36791, 4241.19, 2557.49, 365.06, 0, 16, 0)
	pUnit:RegisterEvent("ValithriaDreamwalker_SpawnEnemiesCicle4", 12000, 1)
end

function ValithriaDreamwalker_SpawnEnemiesCicle4(pUnit, event)
	pUnit:SpawnCreature(37934, 4241.34, 2411.52, 365.06, 0, 16, 0)
	pUnit:SpawnCreature(37934, 4241.19, 2557.49, 365.06, 0, 16, 0)
	pUnit:RegisterEvent("ValithriaDreamwalker_SpawnEnemiesCicle5", 12000, 1)
end

function ValithriaDreamwalker_SpawnEnemiesCicle5(pUnit, event)
	pUnit:SpawnCreature(37868, 4241.34, 2411.52, 365.06, 0, 16, 0)
	pUnit:SpawnCreature(37868, 4241.19, 2557.49, 365.06, 0, 16, 0)
	pUnit:RegisterEvent("ValithriaDreamwalker_SpawnEnemiesCicle1", 12000, 1)
end

RegisterUnitEvent(36789, 2, "ValithriaDreamwalker_OnCombatExit")
RegisterUnitEvent(36789, 4, "ValithriaDreamwalker_Death")
RegisterUnitEvent(36789, 18, "ValithriaDreamwalker_Spawn")
RegisterUnitGossipEvent(36789, 1, "ValithriaDreamwalker_Gossip")
RegisterUnitGossipEvent(36789, 2, "ValithriaDreamwalker_Submenus")

function ValithriaDreamwalkerEnemies_Spawn(pUnit, event)
	pUnit:SetMovementFlags(1)
	pUnit:MoveTo(Valithria:GetX(),Valithria:GetY(),Valithria:GetZ(),0)
end

function ValithriaDreamwalkerEnemies_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:Despawn(500,0)
end

function GluttonousAbomination_onDied(pUnit,event)
	pUnit:RemoveEvents()
	local x = pUnit:GetX()
	local y = pUnit:GetY()
	local z = pUnit:GetZ()
	local faction = pUnit:GetFaction()
	pUnit:Despawn(500,0)
	pUnit:SpawnCreature(37907, x+2, y+2, z, 0, faction, 0)
	pUnit:SpawnCreature(37907, x+2, y-2, z, 0, faction, 0)
	pUnit:SpawnCreature(37907, x-2, y+2, z, 0, faction, 0)
	pUnit:SpawnCreature(37907, x-2, y-2, z, 0, faction, 0)
end

RegisterUnitEvent(37886, 2, "ValithriaDreamwalkerEnemies_OnCombatExit")
RegisterUnitEvent(37863, 2, "ValithriaDreamwalkerEnemies_OnCombatExit")
RegisterUnitEvent(37907, 2, "ValithriaDreamwalkerEnemies_OnCombatExit")
RegisterUnitEvent(36791, 2, "ValithriaDreamwalkerEnemies_OnCombatExit")
RegisterUnitEvent(37934, 2, "ValithriaDreamwalkerEnemies_OnCombatExit")
RegisterUnitEvent(37868, 2, "ValithriaDreamwalkerEnemies_OnCombatExit")
RegisterUnitEvent(37886, 4, "GluttonousAbomination_onDied")
RegisterUnitEvent(37863, 4, "ValithriaDreamwalkerEnemies_OnCombatExit")
RegisterUnitEvent(37907, 4, "ValithriaDreamwalkerEnemies_OnCombatExit")
RegisterUnitEvent(36791, 4, "ValithriaDreamwalkerEnemies_OnCombatExit")
RegisterUnitEvent(37934, 4, "ValithriaDreamwalkerEnemies_OnCombatExit")
RegisterUnitEvent(37868, 4, "ValithriaDreamwalkerEnemies_OnCombatExit")
RegisterUnitEvent(37886, 18, "ValithriaDreamwalkerEnemies_Spawn")
RegisterUnitEvent(37863, 18, "ValithriaDreamwalkerEnemies_Spawn")
RegisterUnitEvent(37907, 18, "ValithriaDreamwalkerEnemies_Spawn")
RegisterUnitEvent(36791, 18, "ValithriaDreamwalkerEnemies_Spawn")
RegisterUnitEvent(37934, 18, "ValithriaDreamwalkerEnemies_Spawn")
RegisterUnitEvent(37868, 18, "ValithriaDreamwalkerEnemies_Spawn")