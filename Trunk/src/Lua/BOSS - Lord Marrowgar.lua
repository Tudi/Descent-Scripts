function Marrowgar_OnEnterCombat(pUnit, event)
	pUnit:SendChatMessage(14, 0, "The Scourge will wash over this world as a swarm of death and destruction!")
	pUnit:PlaySoundToSet(16941)
	pUnit:RegisterEvent("Marrowgar_SaberLash", 12000, 0)
	pUnit:RegisterEvent("Marrowgar_Coldflame", 15000, 0)
	pUnit:RegisterEvent("Marrowgar_BoneSpike", 18000, 0)
	pUnit:RegisterEvent("Marrowgar_BoneStorm", 110000, 0)
end

function Marrowgar_SaberLash(pUnit, event)
	pUnit:CastSpellOnTarget(70814, pUnit:GetMainTank())
end

function Marrowgar_Coldflame(pUnit, event) 
	pUnit:SpawnCreature(36672, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), pUnit:GetO(), 35, 10000) 
end

function Marrowgar_BoneSpike(pUnit, event)
	local rand = math.random(1,3)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Bound by bone!")
		pUnit:PlaySoundToSet(16947)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Stick Around!")
		pUnit:PlaySoundToSet(16948)
	elseif rand == 3 then
		pUnit:SendChatMessage(14, 0, "The only escape is death!")
		pUnit:PlaySoundToSet(16949)
	end
	local target = pUnit:GetRandomPlayer(7)
	if target ~= nil then
		pUnit:SpawnCreature(36619, target:GetX(), target:GetY(), target:GetZ() + 1, 0, pUnit:GetFaction(), 0) 
	end
end
function Marrowgar_BoneStorm(pUnit, event)
	pUnit:SendChatMessage(14, 0, "BONE STORM!")
	pUnit:PlaySoundToSet(16946)
	pUnit:FullCastSpell(69076)
	pUnit:ClearThreatList() 
end



function Marrowgar_Slay(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "More bones for the offering!")
		pUnit:PlaySoundToSet(16942)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Languish in damnation!")
		pUnit:PlaySoundToSet(16943)
	end
end

function Marrowgar_Death(pUnit, event, player)
	pUnit:SendChatMessage(14, 0, "I see... only darkness...")
	pUnit:PlaySoundToSet(16944)
	player:AdvanceQuestObjective(24547, 0)
	pUnit:RemoveEvents()
end

function Marrowgar_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

function Marrowgar_Spawn(pUnit, event)
	pUnit:SendChatMessage(14, 0, "This is the beginning AND the end, mortals. None may enter the master's sanctum!")
	pUnit:PlaySoundToSet(16950)
end

RegisterUnitEvent(36612, 1, "Marrowgar_OnEnterCombat")
RegisterUnitEvent(36612, 2, "Marrowgar_OnCombatExit")
RegisterUnitEvent(36612, 3, "Marrowgar_Slay")
RegisterUnitEvent(36612, 4, "Marrowgar_Death")
RegisterUnitEvent(36612, 18, "Marrowgar_Spawn")

function Coldflame_OnSpawn(pUnit, event)
	pUnit:RegisterEvent("Coldflame_Spell", 1000, 0)
	local rand = math.random(1,4)
	if rand == 1 then
		pUnit:MoveTo(-379.501, 2219.69, 41.8, 0)
	elseif rand == 2 then
		pUnit:MoveTo(-369.355, 2187.07, 41.77, 0)
	elseif rand == 3 then
		pUnit:MoveTo(-359.90, 2221.43, 42.3, 0)
	elseif rand == 4 then
		pUnit:MoveTo(-379.05, 2254.61, 41.76, 0)
	end
end

function Coldflame_Spell(pUnit, event)
	pUnit:CastSpell(70823)
end


RegisterUnitEvent(36672, 18, "Coldflame_OnSpawn")

function BoneSpike_Spawn(pUnit, event)
	pUnit:CastSpellOnTarget(69065, pUnit:GetClosestPlayer())
	pUnit:Root()
end

function BoneSpike_OnCombatExit(pUnit, event)
	local player = pUnit:GetClosestPlayer()
	player:RemoveAura(69065)
	player:Unroot()
end


RegisterUnitEvent(36619, 18, "BoneSpike_Spawn")
RegisterUnitEvent(36619, 2, "BoneSpike_OnCombatExit")
RegisterUnitEvent(36619, 3, "BoneSpike_OnCombatExit")
RegisterUnitEvent(36619, 4, "BoneSpike_OnCombatExit")