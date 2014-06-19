local arcaneClone = 0
local frostClone = 0
local fireClone = 0
local phase = 1

function GrandMagusTelestra_OnEnterCombat(pUnit, event)
	if (phase == 1) then
		pUnit:SendChatMessage(14, 0, "You know what they say about curiosity.")
		pUnit:PlaySoundToSet(13319)
		pUnit:RegisterEvent("GrandMagusTelestra_Phase1", 1000, 1)
	else
		pUnit:SendChatMessage(14, 0, "Now to finish the job!")
		pUnit:PlaySoundToSet(13323)
		pUnit:RegisterEvent("GrandMagusTelestra_Phase3", 1000, 1)
	end
end

function GrandMagusTelestra_Phase1(pUnit, event)
	pUnit:RegisterEvent("GrandMagusTelestra_BasicSpells", 1000, 1)
	pUnit:RegisterEvent("GrandMagusTelestra_PhaseTwoCheck", 1000, 0)
end

function GrandMagusTelestra_Phase3(pUnit, event)
	pUnit:SetHealthPct(50)
	pUnit:RegisterEvent("GrandMagusTelestra_BasicSpells", 1000, 1)
	pUnit:RegisterEvent("GrandMagusTelestra_FreezeCheck", 1000, 1)
end

function GrandMagusTelestra_BasicSpells(pUnit, event)
	pUnit:RegisterEvent("GrandMagusTelestra_FireBomb", 8000, 0)
	pUnit:RegisterEvent("GrandMagusTelestra_IceNova", 17000, 1)
	pUnit:RegisterEvent("GrandMagusTelestra_GravityWell", 25000, 0)
end

function GrandMagusTelestra_PhaseTwoCheck(pUnit, event)
	if pUnit:GetHealthPct() <= 50 then
		pUnit:RemoveEvents()
		local rand = math.random(1,2)
		if rand == 1 then
			pUnit:SendChatMessage(14, 0, "There's plenty of me to go around.")
			pUnit:PlaySoundToSet(13322)
		elseif rand == 2 then
			pUnit:SendChatMessage(14, 0, "I'll give you more than you can handle.")
			pUnit:PlaySoundToSet(13321)
		end
		pUnit:SpawnCreature(26929,501.602, 86.3451, -16.1246, 5.94311, pUnit:GetFaction(), 0) 
		pUnit:SpawnCreature(26928,502.030, 91.7475, -16.1246, 5.94311, pUnit:GetFaction(), 0) 
		pUnit:SpawnCreature(26930,505.450, 88.7605, -16.1246, 5.94311, pUnit:GetFaction(), 0) 
		pUnit:Despawn(500,0)
		phase = 2
	end
end

function GrandMagusTelestra_FireBomb(pUnit, event)
	local player = pUnit:GetRandomPlayer(0)
	if (player ~= nil) then
		pUnit:FullCastSpellOnTarget(47773,player)
	end
end

function GrandMagusTelestra_IceNova(pUnit, event)
	pUnit:FullCastSpell(47772)
end

function GrandMagusTelestra_GravityWell(pUnit, event)
	pUnit:CastSpell(47756)
end

function GrandMagusTelestra_FreezeCheck(pUnit, event)
	if pUnit:GetHealthPct() <= 25 then
		pUnit:RegisterEvent("GrandMagusTelestra_Freeze", 1000, 1)
	else
		pUnit:RegisterEvent("GrandMagusTelestra_FreezeCheck", 1000, 1)
	end
end

function GrandMagusTelestra_Freeze(pUnit, event)
	
end

function GrandMagusTelestra_Slay(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Death becomes you!")
	pUnit:PlaySoundToSet(13324)
end

function GrandMagusTelestra_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Damn the... luck.")
	pUnit:PlaySoundToSet(13320)
	pUnit:RemoveEvents()
end


RegisterUnitEvent(26731, 1, "GrandMagusTelestra_OnEnterCombat")
RegisterUnitEvent(26731, 3, "GrandMagusTelestra_Slay")
RegisterUnitEvent(26731, 4, "GrandMagusTelestra_Death")

function GrandMagusTelestraFire_Death(pUnit, event)
	pUnit:Despawn(500,0)
	fireClone = 1
	if (arcaneClone == 1 and fireClone == 1 and frostClone == 1) then
		pUnit:SpawnCreature(26731,505.450, 88.7605, -16.1246, 5.94311, pUnit:GetFaction(), 0) 
	end
end

function GrandMagusTelestraArcane_Death(pUnit, event)
	pUnit:Despawn(500,0)
	arcaneClone = 1
	if (arcaneClone == 1 and fireClone == 1 and frostClone == 1) then
		pUnit:SpawnCreature(26731,505.450, 88.7605, -16.1246, 5.94311, pUnit:GetFaction(), 0) 
	end
end

function GrandMagusTelestraFrost_Death(pUnit, event)
	pUnit:Despawn(500,0)
	frostClone = 1
	if (arcaneClone == 1 and fireClone == 1 and frostClone == 1) then
		pUnit:SpawnCreature(26731,505.450, 88.7605, -16.1246, 5.94311, pUnit:GetFaction(), 0) 
	end
end

RegisterUnitEvent(26928, 4, "GrandMagusTelestraFire_Death")
RegisterUnitEvent(26929, 4, "GrandMagusTelestraArcane_Death")
RegisterUnitEvent(26930, 4, "GrandMagusTelestraFrost_Death")