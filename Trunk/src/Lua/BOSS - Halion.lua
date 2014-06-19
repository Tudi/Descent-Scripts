--[[

INSTRUCTIONS:

Create object: ID 259272; type 0; display 9624; name: Shadow Circle
^
pUnit:SpawnGameObject(259272, 3158.877, 536.4, 72.89, 6.27, 0, 1)

Create object: ID 259273; type 0; display 9485; name Fire wall.
^
pUnit:SpawnGameObject(262973, 3057.2, 527, 89.16, 6.136089, 0, 1) -- Id, x, y, z, o, duration, scale[, phase, save]

Create object: ID 259271; type 0; display 9482; name Fire Circle.
^
pUnit:SpawnGameObject(259271, 3158.877, 536.4, 72.889694, 6.27, 0, 1.5)
pUnit:GetGameObjectNearestCoords(3158.877, 536.4, 72.889694, 259271):Despawn(100,0)

Create creature: ID 40029; displayid 11686; name Meteor Strike; faction 35; no respawn time; scale 1.
^
Meteor Strike

TO DO:

- Fires from Meteor Strike
- Making the combustion dispellable or whatever when you reach wall (is this even possible with current game mechanics?)
- Shadow orbs channel 74768 as they revolve around the room, but I can't get this to deal damage properly so not added (it either wipes everyone instantly or does nothing).

]]


function HELION_OnCombat(pUnit, Event)
	pUnit:SendChatMessage(14, 0, "Your world teeters on the brink of annihilation. You will ALL bear witness to the coming of a new age of DESTRUCTION!")
	pUnit:PlaySoundToSet(17500)
	pUnit:CastSpell(78243) -- He has a buff but it doesn't show for some reason -- passive
	pUnit:RegisterEvent("HELION_Spawn_Fire_Circle", 3000, 1)
	pUnit:RegisterEvent("HELION_Cleave", 7000, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Tail_Swipe", 11000, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Flame_Breath", 15000, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Fiery_Combustion", 20000, 0) -- Not sure on correct timer, we want this high since players can't get it removed :/
	pUnit:RegisterEvent("HELION_Meteor_Strike", 17500, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Phase_Two", 2500, 0)
end

function HELION_Spawn_Fire_Circle(pUnit, Event)
	pUnit:SpawnGameObject(259271, 3158.877, 536.4, 72.89, 6.27, 0, 1) -- fire circle -- spawning at scale 1073741824 for some retarded reason
	local tgo = pUnit:GetGameObjectNearestCoords(3158.877, 536.4, 72.889694, 259271) -- Fixes the scale issue ;D
	if tgo ~= nil then
		tgo:SetScale(1.5) -- Fixes the scale issue ;D
	end
end

function HELION_Cleave(pUnit, Event)
	for _, v in pairs(pUnit:GetInRangePlayers()) do
		if (v:IsInFront(pUnit) == true) then
			if (v:IsDead() == false) then
				pUnit:FullCastSpellOnTarget(74524, v)
			end
		end
	end
end

function HELION_Tail_Swipe(pUnit, Event)
	for _, v in pairs(pUnit:GetInRangePlayers()) do
		if (v:IsInFront(pUnit) == false) then
			if (v:IsDead() == false) then
				pUnit:FullCastSpellOnTarget(74531, v)
			end
		end
	end
end

function HELION_Flame_Breath(pUnit, Event)
	pUnit:FullCastSpell(74525)
end

function HELION_Fiery_Combustion(pUnit, Event)
	pUnit:FullCastSpellOnTarget(74562, pUnit:GetRandomPlayer(0))
end

function HELION_Meteor_Strike(pUnit, Event)
	pUnit:SendChatMessage(14,0,"The heavens burn!")
	pUnit:PlaySoundToSet(17505)
	local plr = pUnit:GetRandomPlayer(0)
	pUnit:SpawnCreature(40029, plr:GetX(), plr:GetY(), plr:GetZ(), plr:GetO(), 35, 0)
	--pUnit:CastSpellAoF(plr:GetX(), plr:GetY(), plr:GetZ(), 75877)
end

function HELION_Phase_Two(pUnit, Event)
	if pUnit:GetHealthPct() < 76 then
	pUnit:RemoveEvents()
	pUnit:SendChatMessage(14,0,"You will find only suffering within the realm of twilight! Enter if you dare!")
	pUnit:PlaySoundToSet(17507)
	pUnit:SpawnGameObject(259272, 3158.877, 536.4, 72.89, 6.27, 0, 1) -- shadow circle -- spawning at scale 1073741824 for some retarded reason
	local tgo = pUnit:GetGameObjectNearestCoords(3158.877, 536.4, 72.889694, 259272)
	if tgo ~= nil then
		tgo:SetScale(1.5) -- Fixes the scale issue ;D
		tgo:SetPhase(32) -- Phases it correctly
	end
	pUnit:CastSpell(74807) -- Phase
	pUnit:CastSpell(75484) -- Aura
	SetDBCSpellVar(74807, "c_is_flags", 0x01000) -- Set it to a debuff
	local PlayersAllAround = pUnit:GetInRangePlayers()
	for a, plrs in pairs(PlayersAllAround) do
		plrs:CastSpell(74807) -- Phase
	end
	pUnit:RegisterEvent("HELION_Cleave", 7000, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Tail_Swipe", 11000, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Shadow_Breath", 15000, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Shadowy_Combustion", 20000, 0) -- Not sure on correct timer, we want this high since players can't get it removed :/
	pUnit:RegisterEvent("HELION_Phase_Three", 2500, 0)
	end
end

function HELION_Shadow_Breath(pUnit, Event)
	pUnit:FullCastSpell(74806)
end

function HELION_Shadowy_Combustion(pUnit, Event)
	pUnit:FullCastSpellOnTarget(74792, pUnit:GetRandomPlayer(0))
end

function HELION_Phase_Three(pUnit, Event)
	if pUnit:GetHealthPct() < 51 then
	pUnit:RemoveEvents()
	pUnit:SendChatMessage(14,0,"I am the light and the darkness! Cower, mortals, before the herald of Deathwing!")
	pUnit:PlaySoundToSet(17508)
	pUnit:FullCastSpell(75063) -- Visual and exist in both realms
	pUnit:SpawnCreature(29218, 3187.42, 535.4, 73, 0, 35, 360000) -- Shadow realm portal
	pUnit:RegisterEvent("HELION_Phase_Delay_Three", 2500, 1)
	end
end

function HELION_Phase_Delay_Three(pUnit, Event)
	pUnit:RemoveAura(74807) -- Phase
	pUnit:RemoveAura(75484) -- Aura
	pUnit:SpawnCreature(40142, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), pUnit:GetO(), 21, 0)
	pUnit:GetRandomPlayer(0):RemoveAura(74807)
	pUnit:GetRandomPlayer(0):RemoveAura(74807)
	pUnit:GetRandomPlayer(0):RemoveAura(74807) -- For lack of a better way to handle the combat system
	pUnit:GetRandomPlayer(0):RemoveAura(74807)
	pUnit:GetRandomPlayer(0):RemoveAura(74807)
	pUnit:RegisterEvent("HELION_Cleave", 7000, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Tail_Swipe", 11000, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Flame_Breath", 15000, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Fiery_Combustion", 20000, 0) -- Not sure on correct timer, we want this high since players can't get it removed :/
	pUnit:RegisterEvent("HELION_Meteor_Strike", 17500, 0) -- Not sure on correct timer
end

function HELION_OnKill(pUnit, Event)
	if math.random(1,2) == 1 then
	pUnit:PlaySoundToSet(17501)
	pUnit:SendChatMessage(14,0,"Another hero falls!")
	else
	pUnit:PlaySoundToSet(17502)
	end
end

function HELION_OnLeave(pUnit, Event)
	pUnit:RemoveEvents()
	pUnit:RemoveAura(78243) -- Aura
	pUnit:RemoveAura(74807) -- Phase
	pUnit:RemoveAura(75484) -- Aura
	local tgo = pUnit:GetGameObjectNearestCoords(3158.877, 536.4, 72.889694, 259272) -- Remove shadow circle
	if tgo ~= nil then
		tgo:Despawn(100,0)
	end
	local tgo = pUnit:GetGameObjectNearestCoords(3158.877, 536.4, 72.889694, 259271) -- Remove fire circle
	if tgo ~= nil then
		tgo:Despawn(100,0)
	end
end

function HELION_OnDead(pUnit, Event)
	pUnit:RemoveEvents()
	pUnit:RemoveAura(78243) -- Aura
	pUnit:RemoveAura(74807) -- Phase
	pUnit:RemoveAura(75484) -- Aura
	pUnit:GetGameObjectNearestCoords(3158.877, 536.4, 72.889694, 259272):Despawn(100,0) -- Remove shadow circle
	pUnit:GetGameObjectNearestCoords(3158.877, 536.4, 72.889694, 259271):Despawn(100,0) -- Remove fire circle
	pUnit:SendChatMessage(14,0,"Relish this victory, mortals, for it will be your last! This world will burn with the master's return!")
	pUnit:PlaySoundToSet(17503)
end

RegisterUnitEvent(39863, 1, "HELION_OnCombat")
RegisterUnitEvent(39863, 2, "HELION_OnLeave")
RegisterUnitEvent(39863, 3, "HELION_OnKill")
RegisterUnitEvent(39863, 4, "HELION_OnDead")

-- Meteor Strike

function METEOR_STRIKE_ADD(pUnit, Event)
	pUnit:RegisterEvent("METEOR_STRIKE_ADD_DELAY", 750, 1)
end

function METEOR_STRIKE_ADD_DELAY(pUnit, Event)
	pUnit:Root()
	pUnit:CastSpell(32475) -- Not the correct spell but exactly the same visual and effects
	pUnit:RegisterEvent("METEOR_STRIKE_HELLFIRE_VISUAL", 1000, 4)
	pUnit:RegisterEvent("METEOR_STRIKE_IMPACT", 5000, 1)
end

function METEOR_STRIKE_HELLFIRE_VISUAL(pUnit, Event)
	pUnit:CastSpell(32475) -- Not the correct spell but exactly the same visual and effects
end

function METEOR_STRIKE_IMPACT(pUnit, Event)
	pUnit:SetFaction(21) -- Hostile so damage is dealt
	pUnit:FullCastSpell(75877) -- Impact
	pUnit:Despawn(2000, 0)
end

RegisterUnitEvent(40029, 18, "METEOR_STRIKE_ADD")

--

-- Helion 2, the shadow phased one

function HELION_TWO_OnCombat(pUnit, Event)
	pUnit:SetHealth(pUnit:GetMaxHealth()/2) -- Appropiate
	pUnit:RegisterEvent("HELION_Cleave", 7000, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Tail_Swipe", 11000, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Shadow_Breath", 15000, 0) -- Not sure on correct timer
	pUnit:RegisterEvent("HELION_Shadowy_Combustion", 20000, 0) -- Not sure on correct timer, we want this high since players can't get it removed :/
end

function HELION_TWO_OnSpawn(pUnit, Event)
	pUnit:CastSpell(74807) -- Phase
	pUnit:CastSpell(75484) -- Aura
end

function HELION_TWO_OnLeave(pUnit, Event)
	pUnit:RemoveEvents()
	pUnit:Despawn(100,0)
end

RegisterUnitEvent(40142, 1, "HELION_TWO_OnCombat")
RegisterUnitEvent(40142, 2, "HELION_TWO_OnLeave")
RegisterUnitEvent(40142, 4, "HELION_TWO_OnLeave")
RegisterUnitEvent(40142, 18, "HELION_TWO_OnSpawn")

--

function Shadow_Realm_Portal_HELION(pUnit, Event)
	pUnit:RegisterEvent("Check_For_Players_And_Transfer_HELION", 1000, 1)
end

function Check_For_Players_And_Transfer_HELION(pUnit, Event)
	pUnit:SetPhase(4294967295) -- SHould be them all, and no this isn't a randomly generated number. ^^
	local player = pUnit:GetClosestPlayer()
	if player ~= nil then
		if pUnit:GetDistance(player) < 8 then
			if player:HasAura(74807) == true then
			player:RemoveAura(74807)
			pUnit:RegisterEvent("Check_For_Players_And_Transfer_HELION", 4000, 1)
			else
			player:CastSpell(74807)
			pUnit:RegisterEvent("Check_For_Players_And_Transfer_HELION", 4000, 1)
			end
		else
		pUnit:RegisterEvent("Check_For_Players_And_Transfer_HELION", 1000, 1)
		end
	else
	pUnit:RegisterEvent("Check_For_Players_And_Transfer_HELION", 1000, 1)
	end
end

RegisterUnitEvent(29218, 18, "Shadow_Realm_Portal_HELION")

--