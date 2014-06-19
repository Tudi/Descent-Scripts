--[[
	THIS SCRIPT WAS WRITTEN BY PAROXYSM AND IT IS MEANT FOR ASCENT AND ASCENT ONLY.
		]]
--[[
	PREDEFINED VARS
		]]
local OBJECT_END                                            =	0x006
local UNIT_END                                               			= OBJECT_END + 0x0E2
local OBJECT_FIELD_SCALE_X                                  =	0x004
local UNIT_FLAG_NOT_SELECTABLE = 0x02000000
local UNIT_FLAG_NOT_ATTACKABLE_9           = 0x00000100
local UNIT_FIELD_CHANNEL_OBJECT                              = OBJECT_END + 0x00E
local UNIT_FIELD_HEALTH                                      	= OBJECT_END + 0x010
local UNIT_FIELD_MAXHEALTH                                   	= OBJECT_END + 0x016
local UNIT_FIELD_FLAGS                                       	= OBJECT_END + 0x028
local UNIT_FIELD_FACTIONTEMPLATE                             = OBJECT_END + 0x01D
local UNIT_CHANNEL_SPELL                                     		= OBJECT_END + 0x09F
local CHAT_MSG_MONSTER_YELL							= 14
local CHAT_MSG_RAID_WARNING				= 40
local LANG_UNIVERSAL				= 0
--[[
	~PREDEFINED VARS
		]]
local kiljaeden = 25315
local cn_kiljaeden = nil
local kiljaedendummy = nil
local handofkiljaeden = 25588
local handaddtable = {}
local anveena = nil
local blueorbs = {}
local startbl = {}
local spawnpts = {}
local activatedorbs = {}
spawnpts.x = {1747.962891,1695.568115,1652.105713,1705.071899}
spawnpts.y = {620.713135,676.063477,635.099670,582.911438 }
spawnpts.z = {28.050375,28.050201,28.125195,28.141369 }
spawnpts.o = {2.964534,4.755246,6.129690,1.743238}
--Fill table w/ dummy values
for i = 1, 3 do 
	handaddtable[i] = 1
end
startbl[1] = 1
blueorbs[1] = 1
RegisterUnitEvent(handofkiljaeden,18,"HandOfKil_OnSpawn")
RegisterUnitEvent(handofkiljaeden, 1, "HandOfKil_OnCombat")
RegisterUnitEvent(handofkiljaeden, 2, "HandOfKil_OnWipe")
RegisterUnitEvent(handofkiljaeden, 4, "HandOfKil_OnDeath")
RegisterUnitEvent(handofkiljaeden, 21, "HandOfKil_AIUpdate")
function HandOfKil_OnSpawn(Unit)
	if Unit:GetEntry() == 25588 then
		Unit:DisableRespawn()
		for k,v in pairs(handaddtable) do
			if v == 1 then
				table.remove(handaddtable,k)
				table.insert(handaddtable,Unit)
				break
			end
		end
		for k,v in pairs(startbl) do
			if v == 1 then
				table.remove(startbl,k)
				table.insert(startbl,Unit:SpawnCreature(25608,1699.204712,628.331482,27.558220,Unit:GetO(),Unit:GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE),0))
				table.insert(startbl,Unit:SpawnCreature(26046,1698.896729,629.173546,55.558176,Unit:GetO(),35,0))
				break
			end
		end
		for k,v in pairs(startbl) do
			if v~= nil and v:GetEntry() == 25608 then
				kiljaedendummy = v;
			elseif v~=nil and v:GetEntry() == 26046 then
				anveena = v;
			end
		end
		for k,v in pairs(blueorbs) do
			if v == 1 then
				table.remove(blueorbs,k)
				for i = 1,table.getn(spawnpts.x) do
					table.insert(blueorbs,Unit:SpawnGameObject(188116,spawnpts.x[i],spawnpts.y[i],spawnpts.z[i],Unit:CalcRadAngle(spawnpts.x[i],spawnpts.y[i],kiljaedendummy:GetX(),kiljaedendummy:GetY()),0))
				end
				break
			end
		end
		for k,v in pairs(handaddtable) do
			if v~= nil and type(v) ~= "number" then
				v:SetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT,kiljaedendummy:GetGUID())
				v:SetUInt32Value(UNIT_CHANNEL_SPELL,46757)
			end
		end
	end
end
function HandOfKil_OnCombat(Unit)
	if Unit:GetEntry() == 25588 then
		Unit:SetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT,0)
		Unit:SetUInt32Value(UNIT_CHANNEL_SPELL,0)
		Unit:RegisterEvent("HandOfKil_ShadowBoltVolley",15000, 0)
		Unit:RegisterEvent("HandOfKil_FelFirePortal", 20000, 0)
		Unit:RegisterAIUpdateEvent(1000)
	end
end
function HandOfKil_OnDeath(Unit)
	if Unit:GetEntry() == 25588 then
		Unit:RemoveEvents()
		Unit:RemoveAIUpdateEvent()
		for k,v in pairs(handaddtable) do
			if v ==  Unit then
				table.remove(handaddtable, k)
			end
		end
		print(table.getn(handaddtable))
	end
end
function HandOfKil_OnWipe(Unit)
	if Unit:GetEntry() == 25588 then
		for k,v in pairs(handaddtable) do
			if v == Unit then
				table.remove(handaddtable,k)
			end
		end
		Unit:RemoveEvents()
		Unit:RemoveAIUpdateEvent()
		Unit:RemoveFromWorld()
	end
end
function HandOfKil_ShadowBoltVolley(Unit)
	if Unit:GetRandomPlayer(0) then
		Unit:FullCastSpell(45770)
	end
end
function HandOfKil_FelFirePortal(Unit)
	Unit:FullCastSpell(46875)
end

function HandOfKil_AIUpdate(Unit)
	if Unit:GetHealthPct() <= 20 then
		Unit:FullCastSpell(45772)
	end
end
RegisterUnitEvent(25608, 18, "InitializeKilJaedenDummy")
RegisterUnitEvent(25608, 21, "KilJaedenDummy_AIUpdate")
function InitializeKilJaedenDummy(Unit)
	if Unit:GetEntry() == 25608 then
		Unit:FullCastSpell(46410)
		Unit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_SELECTABLE+UNIT_FLAG_NOT_ATTACKABLE_9)
		--Unit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9)
		Unit:SetUInt32Value(UNIT_FIELD_MAXHEALTH,1000000)
		Unit:SetUInt32Value(UNIT_FIELD_HEALTH,1000000)
		Unit:DisableRespawn()
		Unit:Root()
		Unit:SetCombatCapable(1)
		Unit:SetCombatTargetingCapable(1)
		--Unit:IsInvisible(1,0)
		Unit:RegisterAIUpdateEvent(1000)
		Unit:RegisterEvent("KilJaeden_PeriodicOrders", 2000, 1)
	end
end
function KilJaeden_PeriodicOrders(Unit)
	local rand = tonumber(math.random(5))
	if rand == 1 then
		Unit:PlaySoundToSet(12495)
	end
	if rand == 2 then	
		Unit:PlaySoundToSet(12496)
	end
	if rand == 3 then
		Unit:PlaySoundToSet(12497)
	end
	if rand == 4 then
		Unit:PlaySoundToSet(12498)
	end
	if rand == 5 then
		Unit:PlaySoundToSet(12499)
	end
	Unit:RegisterEvent("KilJaeden_PeriodicOrders", 20000, 1)
end
function KilJaedenDummy_AIUpdate(Unit)
	if Unit:GetEntry() == 25608 then
		Unit:WipeThreatList()
		local no = table.getn(handaddtable)
		if no ~= nil and no == 0 then
			cn_kiljaeden = Unit:SpawnCreature(kiljaeden,1699.204712,628.331482,27.558220,Unit:GetO(),Unit:GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE),0)
			Unit:RemoveEvents()
			Unit:RemoveAIUpdateEvent()
			Unit:RemoveFromWorld()
		end
	end
end
RegisterUnitEvent(26046,18,"InitializeAnveena")
function InitializeAnveena(Unit)
	if Unit:GetEntry() == 26046 then
		Unit:EnableFlight()
		Unit:MoveTo(Unit:GetX(),Unit:GetY(),Unit:GetZ()-1,Unit:GetO())
		Unit:MoveTo(Unit:GetX(),Unit:GetY(),Unit:GetZ()+1,Unit:GetO())
		Unit:SetCombatCapable(1)
		Unit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9)
		Unit:CastSpell(46367)
	end
end
--[[
	FEL FIRE PORTAL
		]]
RegisterUnitEvent(25603,18,"InitializeFelFirePortal")
function InitializeFelFirePortal(Unit)
	if Unit:GetEntry() == 25603 then
		Unit:SetCombatTargetingCapable(1)
		Unit:SetCombatMeleeCapable(1)
		Unit:FullCastSpell(46464)
		Unit:WipeThreatList()
	end
end
--[[	
	VOLATILE FELFIRE FIEND
		]]
RegisterUnitEvent(25598,18,"VolatileFiend_OnSpawn")
RegisterUnitEvent(25598,21,"VolatileFiend_AIUpdate")
RegisterUnitEvent(25598,4,"VolatileFied_OnDeath")
function VolatileFiend_OnSpawn(Unit)
	Unit:DisableRespawn()
	Unit:SetCombatCapable(1)
	Unit:RegisterAIUpdateEvent(500)
	local plr = Unit:GetRandomPlayer(0)
	if plr~= nil then
		Unit:SetNextTarget(plr)
		Unit:SetUnitToFollow(plr,3,45)
	end
end
function VolatileFiend_AIUpdate(Unit)
	local tar = Unit:GetNextTarget()
	if tar ~= nil and Unit:CalcDistance(tar) <= 5 then
		Unit:FullCastSpell(45779)
		Unit:RemoveFromWorld()
	end
end
function VolatileFied_OnDeath(Unit)
	Unit:FullCastSpell(45779)
	Unit:RemoveAIUpdateEvent()
end
--[[
	KIL JAEDEN
	]]
RegisterUnitEvent(kiljaeden,18, "KilJaeden_OnSpawn")
RegisterUnitEvent(kiljaeden,1, "KilJaeden_OnCombat")
RegisterUnitEvent(kiljaeden,2, "KilJaeden_OnWipe")
RegisterUnitEvent(kiljaeden,3, "KilJaeden_OnKill")
RegisterUnitEvent(kiljaeden,4, "KilJaeden_OnDeath")
RegisterUnitEvent(kiljaeden,21,"KilJaeden_AIUpdate")
RegisterUnitEvent(kiljaeden,22,"KilJaeden_SoulDarkness")

local legion_light =nil
local soul_flay = nil
local fire_bloom =nil
local shadow_spike = nil
local flame_dart =nil
local meteor_timer = nil
local shield_orbs = nil
local shield_orbtable = {}
local shield_orbtimer = nil
local soul_darkness = nil
local cancast = true
local castmeteor = false
local m_phase = nil
local meteors = 0
local sinister_reflect = false;
local kalec = nil
local saytable = {phase3say = false,phase4say = false,phase5say = false, orbsay1 = false, orbsay2 = false, orbsay3 = false, kalecsay1 = false,kalecsay2 = false,kalecsay3 = false,kalecsay4 = false, anveenasay1 = false,anveenasay2 = false,
anveenasay3 = false, anveenasay4 = false,kalecsay5 = false,kalecintrosay = false,introsay = false}
--[[
	HOOKED EVENTS
	]]
	
function KilJaeden_OnSpawn(Unit)
	--Unit:Root()
	Unit:MoveTo(Unit:GetX(),Unit:GetY(),Unit:GetZ()-1,Unit:GetO())
	Unit:MoveTo(Unit:GetX(),Unit:GetY(),Unit:GetZ()+2,Unit:GetO())
	Unit:DisableRespawn()
	Unit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9+UNIT_FLAG_NOT_SELECTABLE)
	Unit:SetCombatCapable(1)
	Unit:SetCombatMeleeCapable(1)
	Unit:FullCastSpell(41600) -- birth emote
	Unit:CastSpell(47115)
	Unit:RegisterEvent("InitializeKilJaeden", 10000, 1)-- 10 secs should be enough to fully play birth animation
end
function InitializeKilJaeden(Unit)
	Unit:SetUInt32Value(UNIT_FIELD_FLAGS,0)
	Unit:SetCombatCapable(0)
end
function KilJaeden_OnCombat(Unit)
	Unit:RemoveEvents()
	Unit:RegisterAIUpdateEvent(1000)
	Unit:Root()
	m_phase = 1
	KilJaeden_PhaseCheck(Unit)
	saytable.introsay = true
	KilJaeden_Gossip(Unit)
	saytable.kalecintrosay = true
	Unit:RegisterEvent("KilJaeden_Gossip",25000,1 )
	kalec = Unit:SpawnCreature(25319,1699.204712,628.331482,27.558220+100.000000,Unit:GetO(),35,0)
end
function KilJaeden_OnKill(Unit,event,pMisc)
	if pMisc:IsPlayer() then
		if math.random(2) == 1 then
			Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Another step towards destruction!")
			Unit:PlaySoundToSet(12501)
		else
			Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Anak'kiri")
			Unit:PlaySoundToSet(12502)
		end
	end
end
function KilJaeden_OnDeath(Unit)
	Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Nooooooooooooo!")
	Unit:RemoveEvents()
	Unit:RemoveAIUpdateEvent()
	for k,v in pairs(shield_orbtable) do 
		if v ~= nil and v:IsInWorld() == true then
			v:RemoveFromWorld()
		end
	end
end
function KilJaeden_OnWipe(Unit)
	Unit:RemoveAIUpdateEvent()
	Unit:RemoveEvents()
	if Unit:IsAlive() == true then
		Unit:RemoveFromWorld()
	end
	for k,v in pairs(shield_orbtable) do 
		if v ~= nil and v:IsInWorld() == true then
			v:RemoveFromWorld()
		end
	end
end
--[[
	~HOOK EVENTS
		]]
function KilJaeden_Gossip(Unit)
	if saytable.introsay == true and m_phase == 1 then
		saytable.introsay = false
		Unit:PlaySoundToSet(12500)
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"The expendible have perished... So be it! Now I shall succeed where Sargeras could not! I will bleed this wretched world and secure my place as the true master of the Burning Legion. The end has come! Let the unraveling of this world commence!")
	end
	if saytable.kalecintrosay == true and kalec ~= nil and m_phase == 1 then
		Unit:PlaySoundToSet(12438)
		kalec:SetMovementType(3)
		kalec:MoveToWaypoint(1);
		kalec:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"You are not alone. The Blue Dragonflight shall help you vanquish the Deceiver.")
		saytable.kalecintrosay = false
	end
	if saytable.kalecsay1 == true and saytable.anveenasay1 == false and saytable.phase3say == false and kalec ~= nil and m_phase == 2 then
		saytable.anveenasay1 = true
		Unit:PlaySoundToSet(12445)
		kalec:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Anveena, you must awaken, this world needs you!")
		Unit:RegisterEvent("KilJaeden_Gossip",5000,1)
	elseif saytable.kalecsay1 == true and saytable.anveenasay1 == true and saytable.phase3say == false and anveena ~= nil and m_phase == 2 then
		saytable.phase3say = true
		Unit:PlaySoundToSet(12511)
		anveena:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"I serve only the master now.")
		Unit:RegisterEvent("KilJaeden_Gossip",5000,1)
	elseif saytable.kalecsay1 == true and saytable.anveenasay1 == true and saytable.phase3say == true and m_phase == 2 then
		saytable.phase3say = false
		saytable.kalecsay1 = false
		saytable.anveenasay1 = false
		saytable.orbsay1 = true
		Unit:PlaySoundToSet(12508)
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"I will not be denied! This world shall fall!")
		Unit:RegisterEvent("KilJaeden_Gossip",7000,1)
	elseif saytable.orbsay1 == true and kalec ~= nil and m_phase == 2 then
		Unit:PlaySoundToSet(12440)
		kalec:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"I will channel my power into the orbs, be ready!")
		saytable.orbsay1 = false
		kalec:SetMovementType(3)
		kalec:MoveToWaypoint(1)
		local chosenorb = blueorbs[math.random(1,table.getn(blueorbs))]
		if chosenorb ~= nil and type(chosenorb) == "userdata" and chosenorb:IsInWorld() == true then
			local guid = chosenorb:GetGUID()
			activatedorbs["guid"] = true
			print(activatedorbs["guid"])
			chosenorb:SpawnCreature(25640,chosenorb:GetX(),chosenorb:GetY(),chosenorb:GetZ(),chosenorb:GetO(),14,0)
		end
	end
	if saytable.kalecsay2 == true and saytable.anveenasay2 == false and saytable.phase4say == false and kalec ~= nil and m_phase == 3 then
		Unit:PlaySoundToSet(12446)
		kalec:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"You must let go! You must become what you were always meant to be! The time is now, Anveena!")
		saytable.anveenasay2 = true
		Unit:RegisterEvent("KilJaeden_Gossip",8000,1)
	elseif saytable.kalecsay2 == true and saytable.anveenasay2 == true and saytable.phase4say == false and anveena ~= nil and m_phase == 3  then
		Unit:PlaySoundToSet(12512)
		anveena:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"But I'm... lost. I cannot find my way back.")
		saytable.phase4say = true
		Unit:RegisterEvent("KilJaeden_Gossip",6000,1)
	elseif saytable.kalecsay2 == true and saytable.anveenasay2 == true and saytable.phase4say == true and m_phase == 3  then
		Unit:PlaySoundToSet(12509)
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Do not harbor false hope. You cannot win!")
		saytable.kalecsay2 = false
		saytable.anveenasay2 = false
		saytable.phase4say = false
		saytable.orbsay2 = true
		Unit:RegisterEvent("KilJaeden_Gossip",7000,1)
	elseif saytable.orbsay2 == true and kalec ~= nil and m_phase == 3  then
		if math.random(2) == 1 then
			kalec:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"I have empowered another orb! Use it quickly!")
			Unit:PlaySoundToSet(12441)
			kalec:SetMovementType(3)
			kalec:MoveToWaypoint(2)
			local chosenorb = blueorbs[math.random(1,table.getn(blueorbs))]
			if chosenorb ~= nil and type(chosenorb) == "userdata" and chosenorb:IsInWorld() == true  then
				local guid = chosenorb:GetGUID()
				activatedorbs["guid"] = true
				print(activatedorbs["guid"])
				chosenorb:SpawnCreature(25640,chosenorb:GetX(),chosenorb:GetY(),chosenorb:GetZ(),chosenorb:GetO(),14,0)
			end
		else
			kalec:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Another orb is ready! Make haste!")
			Unit:PlaySoundToSet(12442)
			kalec:SetMovementType(3)
			kalec:MoveToWaypoint(2)
			local chosenorb = blueorbs[math.random(1,table.getn(blueorbs))]
			if chosenorb ~= nil and type(chosenorb) == "userdata" and chosenorb:IsInWorld() == true then
				local guid = chosenorb:GetGUID()
				activatedorbs["guid"] = true
				print(activatedorbs["guid"])
				chosenorb:SpawnCreature(25640,chosenorb:GetX(),chosenorb:GetY(),chosenorb:GetZ(),chosenorb:GetO(),14,0)
			end
		end
		saytable.orbsay2 = false
	end
	if saytable.kalecsay3 == true and saytable.anveenasay3 == false and saytable.phase5say == false and m_phase == 4 and kalec ~= nil then
		Unit:PlaySoundToSet(12447)
		kalec:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Anveena, I love you! Focus on my voice, come back for me now! Only you can cleanse the Sunwell!")
		saytable.anveenasay3 = true
		Unit:RegisterEvent("KilJaeden_Gossip",10000,1)
	elseif saytable.kalecsay3 == true and saytable.anveenasay3 == true and saytable.phase5say == false and anveena ~= nil then
		Unit:PlaySoundToSet(12513)
		anveena:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Kalec...? Kalec?")
		saytable.kalecsay3 = false
		saytable.kalecsay4 = true
		saytable.anveenasay3 = false
		Unit:RegisterEvent("KilJaeden_Gossip",2200,1)
	elseif saytable.kalecsay4 == true and saytable.anveenasay4 == false and kalec ~= nil then
		Unit:PlaySoundToSet(12448)
		kalec:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Yes, Anveena! Let fate embrace you now!")
		saytable.anveenasay4 = true
		Unit:RegisterEvent("KilJaeden_Gossip",6500,1)
	elseif saytable.kalecsay4 == true and saytable.anveenasay4 == true and saytable.phase5say == false and anveena ~= nil then
		Unit:PlaySoundToSet(12514)
		anveena:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"The nightmare is over, the spell is broken! Goodbye, Kalec, my love!")
		saytable.phase5say = true
		Unit:RegisterEvent("KilJaeden_Gossip",6000,1)
	elseif saytable.kalecsay4 == true and saytable.anveenasay4 == true and saytable.phase5say == true and anveena ~= nil then
		Unit:RegisterEvent("KilJaeden_CanCast",5000,1)
		Unit:PlaySoundToSet(12510)
		Unit:Emote(409,0)
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Aggghh! The powers of the Sunwell... turn... against me! What have you done? What have you done???")
		Unit:FullCastSpellOnTarget(46474,Unit)
		anveena:RemoveFromWorld()
		saytable.orbsay3 = true
		saytable.kalecsay4 = false
		saytable.anveenasay4 = false
		saytable.phase5say = false
		Unit:RegisterEvent("KilJaeden_Gossip",11000,1)
	elseif saytable.orbsay3 == true and saytable.kalecsay4 == false and kalec ~= nil then
		kalec:SetMovementType(3)
		kalec:MoveToWaypoint(4)
		kalec:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"I have channeled all I can! The power is in your hands!")
		Unit:PlaySoundToSet(12443)
		saytable.orbsay3 = false
		saytable.kalecsay4 = false
		saytable.kalecsay5 = true
		for k,v in pairs(blueorbs) do
			if v ~= nil and type(v) == "userdata" and v:IsInWorld() == true then
				local guid = v:GetGUID()
				activatedorbs["guid"] = true
				v:SpawnCreature(25640,v:GetX(),v:GetY(),v:GetZ(),v:GetO(),14,0)
			end
		end
		Unit:RegisterEvent("KilJaeden_Gossip",6000,1)
	elseif saytable.kalecsay5 == true and saytable.orbsay3 == false and kalec ~= nil then
		if math.random(2) == 1 then
			Unit:PlaySoundToSet(12449)
			kalec:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Strike now, heroes, while he is weakened! Vanquish the Deceiver!")
		else
			Unit:PlaySoundToSet(12450)
			kalec:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Goodbye, Anveena, my love. Few will remember your name, yet this day you change the course of destiny. What was once corrupt is now pure. Heroes, do not let her sacrifice be in vain.")			
		end
	end
end
function KilJaeden_CanCast(Unit)
	if cancast == false then
		cancast = true
	end
	if castmeteor == false then
		castmeteor = true
	end
end
function KilJaeden_PhaseCheck(Unit)
	if m_phase == 1 then
		shield_orbs = 1
		shield_orbtimer = 15
		soul_flay = 4
		legion_light = 0
		fire_bloom = 0
	end
	if m_phase == 2 then
		shield_orbs = 2
		shield_orbtimer = 0
		shadow_spike = true
		flame_dart = 0
		soul_darkness = 0
		soul_flay = 0
		legion_light = 0
		fire_bloom = 0
	end
	if m_phase == 3 then
		shield_orbtimer = 0
		fire_bloom = 0
		legion_light = 0
		soul_flay = 0
		soul_darkness = 0
		flame_dart = 0
		meteors = 0
		shield_orbs = 3
		shadow_spike = true
		castmeteor = true
		meteor_timer = 0;
	end
	if m_phase == 4 then
		shield_orbtimer = nil
		shield_orbs = nil
		shield_spawn = nil
		meteor_timer = 0
		meteors = 0
		shadow_spike = true
		shield_orbs = 0
		legion_light = 0
		soul_flay = 0
		soul_darkness = 0
		flame_dart = 0
		fire_bloom = 0
	end
end
function KilJaeden_AIUpdate(Unit)
	if m_phase == 1 then
		legion_light = legion_light +1
		soul_flay = soul_flay +1
		fire_bloom = fire_bloom +1
		shield_orbtimer = shield_orbtimer+1
	end
	if m_phase == 2 then
		flame_dart =  flame_dart +1
		soul_darkness = soul_darkness +1
		legion_light = legion_light +1
		soul_flay = soul_flay +1
		fire_bloom = fire_bloom +1
		shield_orbtimer = shield_orbtimer+1
	end
	if m_phase == 3 then
		flame_dart =  flame_dart +1
		soul_darkness = soul_darkness +1
		legion_light = legion_light +1
		soul_flay = soul_flay +1
		fire_bloom = fire_bloom +1
		meteor_timer = meteor_timer+1
		shield_orbtimer = shield_orbtimer+1
	end
	if m_phase == 4 then
		flame_dart =  flame_dart +1
		soul_darkness = soul_darkness +1
		legion_light = legion_light +1
		soul_flay = soul_flay +1
		fire_bloom = fire_bloom +1
		meteor_timer = meteor_timer+1
	end
	local hp = Unit:GetHealthPct()
	local plr = Unit:GetRandomPlayer(0)
	local tank = Unit:GetMainTank()
	if hp > 85 then
		if soul_flay == 5 and cancast == true and tank~= nil then
			cancast = false
			Unit:FullCastSpellOnTarget(45442, tank)
			soul_flay = 0
			Unit:RegisterEvent("KilJaeden_CanCast",3500, 1)
		elseif soul_flay == 5 and cancast == false then
			soul_flay = soul_flay-1
		end
		if shield_orbtimer == 30 and table.getn(shield_orbtable) == 0 then
			for i = 1,shield_orbs do
				local angle = nil
				local radius = 10
				if math.random(math.random(2)) == 1 then
					angle = -math.pi
				else
					angle = math.pi
				end
				local newx,newy = math.cos(angle)*radius,math.sin(angle)*radius
				table.insert(shield_orbtable,Unit:SpawnCreature(25502,Unit:GetX()+newx,Unit:GetY()+newy,Unit:GetZ(),Unit:GetO(),16,0))
			end
			shield_orbtimer = 0
		elseif shield_orbtimer == 30 and table.getn(shield_orbtable) ~= 0 then
			shield_orbtimer = shield_orbtimer - 5
		end
		if legion_light == 10 and cancast == true and plr ~=nil then
			cancast = false
			Unit:FullCastSpellOnTarget(45664,plr)
			legion_light = 0
			Unit:RegisterEvent("KilJaeden_CanCast",3000,1)
		elseif legion_light == 10 and cancast == false and plr ~=nil then
			legion_light = legion_light-1
		end
		if fire_bloom == 30 and cancast == true then
			cancast = false
			Unit:FullCastSpell(45641)
			fire_bloom = 0
			Unit:RegisterEvent("KilJaeden_CanCast", 2000, 1)
		elseif fire_bloom == 30 and cancast == false then
			fire_bloom = fire_bloom-1
		end
	elseif hp <= 85 and hp > 55 then
		if m_phase < 2 then
			print "M_Phase was 1, now setting it to 2."
			m_phase = 2
			KilJaeden_PhaseCheck(Unit)
			sinister_reflect = true
		end
		if sinister_reflect == true and cancast == true then
			sinister_reflect = false
			cancast = false
			Unit:RegisterEvent("KilJaeden_SinisterReflect",1000,1)
		end
		if shadow_spike == true and cancast == true and plr ~=nil then
			cancast = false
			shadow_spike = false
			saytable.kalecsay1 = true
			Unit:FullCastSpellOnTarget(46680, plr)
			Unit:RegisterEvent("KilJaeden_CanCast", 29000, 1)
			Unit:RegisterEvent("KilJaeden_Gossip",18000, 1)
		end
		if shield_orbtimer == 30 and table.getn(shield_orbtable) == 0 then
			for i = 1,shield_orbs do
				local angle = nil
				local radius = 10
				if math.random(math.random(2)) == 1 then
					angle = -math.pi
				else
					angle = math.pi
				end
				local newx,newy = math.cos(angle)*radius,math.sin(angle)*radius
				table.insert(shield_orbtable,Unit:SpawnCreature(25502,Unit:GetX()+newx,Unit:GetY()+newy,Unit:GetZ(),Unit:GetO(),16,0))
			end
			shield_orbtimer = 0
		elseif shield_orbtimer == 30 and table.getn(shield_orbtable) ~= 0 then
			shield_orbtimer = shield_orbtimer - 5
		end
		if legion_light >= 15 and plr ~=nil and cancast == true then
			cancast = false
			Unit:FullCastSpellOnTarget(45664,plr)
			legion_light = 0
			Unit:RegisterEvent("KilJaeden_CanCast",3000, 1)
		elseif legion_light >= 15 and plr ~=nil and cancast == false then
			legion_light = legion_light-1
		end
		if soul_flay >= 5 and cancast == true and tank~= nil then
			cancast = false
			Unit:FullCastSpellOnTarget(45442,tank)
			Unit:RegisterEvent("KilJaeden_CanCast",3500, 1)
			soul_flay = 0
		elseif soul_flay >= 5 and cancast == false then
			soul_flay = soul_flay-1
		end
		if fire_bloom >= 30 and cancast == true then
			cancast = false
			Unit:FullCastSpell(45641)
			fire_bloom = 0
			Unit:RegisterEvent("KilJaeden_CanCast",2000, 1)
		elseif fire_bloom >= 30 and cancast == false then
			fire_bloom = fire_bloom-1
		end
		if flame_dart >= 20 and cancast == true and plr ~=nil then
			cancast = false
			Unit:FullCastSpellOnTarget(45737, plr)
			flame_dart = 0
			Unit:RegisterEvent("KilJaeden_CanCast",2000, 1)
		elseif  flame_dart >= 20 and cancast == false then
			flame_dart = flame_dart - 1
		end
		if soul_darkness >= 40 and cancast == true then
			cancast = false
			Unit:FullCastSpell(46605)
			Unit:SendChatMessage(CHAT_MSG_TEXT_EMOTE,LANG_UNIVERSAL,Unit:GetName().." begins to channel dark energy!")
			soul_darkness = 0
			Unit:RegisterEvent("KilJaeden_CanCast",10000, 1)
			Unit:RegisterEvent("KilJaeden_SoulDarkness",8800,1)
		elseif soul_darkness >= 40 and cancast == false then
			soul_darkness = soul_darkness-1
		end
	elseif hp <= 55 and hp > 25 then
		if m_phase < 3 then
			print "M_Phase was 2, now setting to 3"
			m_phase = 3
			KilJaeden_PhaseCheck(Unit)
			sinister_reflect = true
		end
		if sinister_reflect == true and cancast == true then
			sinister_reflect = false
			cancast = false
			Unit:RegisterEvent("KilJaeden_SinisterReflect",1000,1)
		end
		if shield_orbtimer == 30 and table.getn(shield_orbtable) == 0 then
			for i = 1,shield_orbs do
				if math.random(math.random(2)) == 1 then
					angle = math.pi
				else
					angle = -math.pi
				end
				local newx,newy = math.cos(angle)*math.random(15,17),math.sin(angle)*math.random(15,17)
				table.insert(shield_orbtable,Unit:SpawnCreature(25502,Unit:GetX()+newx,Unit:GetY()+newy,Unit:GetZ(),Unit:GetO(),16,0))
			end
			shield_orbtimer = 0
		elseif shield_orbtimer == 30 and table.getn(shield_orbtable) ~= 0 then
			shield_orbtimer = shield_orbtimer - 5
		end
		if meteor_timer >= 3 and meteors <= 3 and castmeteor == true and plr ~=nil then
			Unit:SpawnCreature(25735,plr:GetX(),plr:GetY(),plr:GetZ()+30,plr:GetO(),16,8000)
			Unit:SpawnCreature(60000,plr:GetX(),plr:GetY(),plr:GetZ(),plr:GetO(),16,8000)
			meteors = meteors + 1
			meteor_timer = 0
		elseif meteor_timer <= 3 and castmeteor == false then
			meteor_timer = meteor_timer-1
		end
		if shadow_spike == true and cancast == true and plr~= nil then
			Unit:SetUInt32Value(UNIT_NPC_EMOTESTATE,0)
			cancast = false
			Unit:FullCastSpellOnTarget(46680, plr)
			shadow_spike = false
			saytable.kalecsay2 = true
			Unit:RegisterEvent("KilJaeden_CanCast",29000, 1)
			Unit:RegisterEvent("KilJaeden_Gossip",15000,1)
		end
		if legion_light >= 15 and cancast == true and plr~= nil then
			cancast = false
			Unit:FullCastSpellOnTarget(45664,Unit:GetRandomPlayer(0))
			legion_light = 0
			Unit:RegisterEvent("KilJaeden_CanCast",3000, 1)
		elseif legion_light >= 15 and Unit:GetRandomPlayer(0) and cancast == false then
			legion_light = legion_light-1
		end
		if soul_flay >= 5 and cancast == true and tank~= nil then
			cancast = false
			Unit:FullCastSpellOnTarget(45442, tank)
			soul_flay = 0
			Unit:RegisterEvent("KilJaeden_CanCast",3500, 1)
		elseif soul_flay >= 5 and cancast == false and tank~= nil then
			soul_flay = soul_flay-1
		end
		if fire_bloom >= 20 and cancast == true then
			cancast = false
			Unit:FullCastSpell(45641)
			fire_bloom = 0
			Unit:RegisterEvent("KilJaeden_CanCast", 2000, 1)
		elseif fire_bloom >= 20 and cancast == false then
			fire_bloom = fire_bloom-1
		end
		if flame_dart >= 15 and cancast == true then
			cancast = false
			Unit:FullCastSpell(45737)
			flame_dart = 0
			Unit:RegisterEvent("KilJaeden_CanCast", 2000, 1)
		elseif  flame_dart >= 15 and cancast == false then
			flame_dart = flame_dart - 1
		end
		if soul_darkness >= 40 and cancast == true then
			cancast = false
			castmeteor = false
			Unit:FullCastSpell(46605)
			Unit:SendChatMessage(CHAT_MSG_TEXT_EMOTE,LANG_UNIVERSAL,Unit:GetName().." begins to channel dark energy!")
			soul_darkness = 0
			Unit:RegisterEvent("KilJaeden_CanCast",10000, 1)
			Unit:RegisterEvent("KilJaeden_SoulDarkness",8800,1)
		elseif soul_darkness >= 40 and cancast == false then
			soul_darkness = soul_darkness-1
		end
	elseif hp  <= 25 then
		if m_phase < 4 then
			print "m phase was 3 now setting to 4"
			m_phase = 4
			KilJaeden_PhaseCheck(Unit)
			sinister_reflect = true
		end
		if sinister_reflect == true and cancast == true then
			sinister_reflect = false
			cancast = false
			Unit:RegisterEvent("KilJaeden_SinisterReflect",1000,1)
		end
		if shadow_spike == true and cancast == true and plr~= nil then
			cancast = false
			Unit:FullCastSpellOnTarget(46680, plr)
			shadow_spike = false
			saytable.kalecsay3 = true
			Unit:RegisterEvent("KilJaeden_Gossip",6000,1)
		end
		if meteor_timer >= 3 and meteors <= 5 and castmeteor == true and plr~= nil then
			Unit:SpawnCreature(25735,plr:GetX(),plr:GetY(),plr:GetZ()+30,plr:GetO(),16,8000)
			Unit:SpawnCreature(60000,plr:GetX(),plr:GetY(),plr:GetZ(),plr:GetO(),16,8000)
			meteors = meteors + 1
			meteor_timer = 0
		elseif meteor_timer >= 3 and castmeteor == false then
			meteor_timer = meteor_timer-1
		end
		if cancast == true and soul_flay >= 5 and tank~= nil then
			cancast = false
			Unit:FullCastSpellOnTarget(45442,tank)
			soul_flay = 0
			Unit:RegisterEvent("KilJaeden_CanCast",3500, 1)
		elseif soul_flay >= 5 and cancast == false and tank~= nil then
			soul_flay = soul_flay-1
		end
		if legion_light >= 15 and cancast == true and plr~= nil then
			cancast = false
			Unit:FullCastSpellOnTarget(45664,plr)
			legion_light = 0
			Unit:RegisterEvent("KilJaeden_CanCast",3000, 1)
		elseif legion_light >= 15 and cancast == false and plr~= nil then
			legion_light = legion_light-1
		end
		if fire_bloom >= 30 and cancast == true then
			cancast = false
			Unit:FullCastSpell(45641)
			fire_bloom = 0
			Unit:RegisterEvent("KilJaeden_CanCast", 2000, 1)
		elseif fire_bloom >= 30 and cancast == false then
			fire_bloom = fire_bloom-1
		end
		if flame_dart >= 15 and cancast == true then
			cancast = false
			Unit:FullCastSpell(45737)
			flame_dart = 0
			Unit:RegisterEvent("KilJaeden_CanCast", 2000, 1)
		elseif  flame_dart >= 15 and cancast == false then
			flame_dart = flame_dart - 1
		end
		if soul_darkness >= 25 and cancast == true then
			cancast = false
			Unit:FullCastSpell(46605)
			Unit:SendChatMessage(CHAT_MSG_TEXT_EMOTE,LANG_UNIVERSAL,Unit:GetName().." begins to channel dark energy!")
			soul_darkness = 0
			Unit:RegisterEvent("KilJaeden_CanCast",10000, 1)
			Unit:RegisterEvent("KilJaeden_SoulDarkness",8800,1)
		elseif soul_darkness >= 25 and cancast == false then
			soul_darkness = soul_darkness-1
		end
	end
end
function KilJaeden_SinisterReflect(Unit)
	for i = 1,4,1 do
		local plr = Unit:GetRandomPlayer(0)
		if plr ~= nil then
			plr:PlayerCastSpell(45891) -- Using no. iterator seems to spawn more than 4.
		end
	end
	Unit:Emote(404,1)
	Unit:RegisterEvent("KilJaeden_SinisterReflectdeux",1500,1) -- the Wing fling thing
	Unit:RegisterEvent("KilJaeden_CanCast",2500,1)
	local rand = math.random(2)
	if rand == 1 then
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Who can you trust?")
		Unit:PlaySoundToSet(12503)
	else
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"The enemy is among you.")
		Unit:PlaySoundToSet(12504)
	end
end
function KilJaeden_SinisterReflectdeux(Unit)
	Unit:Emote(405,1)
end
function KilJaeden_SoulDarkness(Unit)
	Unit:FullCastSpell(45657)
	local rand = math.random(3)
	if rand == 1 then
		Unit:PlaySoundToSet(12505)
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Chaos!")
	end
	if rand == 2 then
		Unit:PlaySoundToSet(12506)
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Destruction!")
	end
	if rand == 3 then
		Unit:PlaySoundToSet(12507)
		Unit:SendChatMessage(CHAT_MSG_MONSTER_YELL,LANG_UNIVERSAL,"Oblivion!")
	end
end
RegisterUnitEvent(60000,18,"KilJaeden_OrbDummy")
RegisterUnitEvent(25735,18,"KilJaeden_InitiliazeMeteor")
RegisterUnitEvent(60000,21,"KilJaeden_OrbUpdate")
function KilJaeden_InitiliazeMeteor(Unit)
	Unit:EnableFlight()
	Unit:MoveTo(Unit:GetX(),Unit:GetY(),Unit:GetZ()+1,Unit:GetO())
	Unit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9+UNIT_FLAG_NOT_SELECTABLE)
	Unit:DisableRespawn()
	Unit:SetCombatCapable(1)
	Unit:SetCombatMeleeCapable(1)
	Unit:WipeThreatList()
	Unit:MonsterMove(Unit:GetX(),Unit:GetY(),Unit:GetZ()-25,8000,768)
	Unit:RegisterEvent("KilJaeden_MeteorStormFinish",8000,1)
end
function KilJaeden_MeteorStormFinish(Unit)
	Unit:SetPosition(Unit:GetX(),Unit:GetY(),Unit:GetZ()-40)
	Unit:StopMovement(0)
	Unit:RemoveEvents()
	Unit:RemoveFromWorld()
	meteors = meteors-1
end
function KilJaeden_OrbDummy(Unit)
	--Unit:SetUInt32Value(UNIT_FIELD_DISPLAYID,22452)
	Unit:EnableFlight()
	Unit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9+UNIT_FLAG_NOT_SELECTABLE)
	Unit:SetCombatCapable(1)
	Unit:WipeCurrentTarget()
	Unit:RegisterAIUpdateEvent(1000)
	--Unit:CastSpell(45911)
	Unit:RegisterEvent("KilJaeden_OrbFinish",9000,1)
end
function KilJaeden_OrbUpdate(Unit)
	Unit:PlaySpellVisual(Unit,844)
end
function KilJaeden_OrbFinish(Unit)
	Unit:RemoveEvents()
	Unit:RemoveAIUpdateEvent()
	Unit:RegisterEvent("KilJaeden_OrbFinish2",100,1)
end
function KilJaeden_OrbFinish2(Unit)
	Unit:FullCastSpell(45915)
	Unit:PlaySpellVisual(Unit,9312)
	Unit:RegisterEvent("KilJaeden_OrbDespawn",1000,1)
end
function KilJaeden_OrbDespawn(Unit)
	Unit:RemoveFromWorld()
end
--[[
	SINISTER REFLECTION AI
	]]
RegisterUnitEvent(25708,18,"InitializeSinisterReflection")
RegisterUnitEvent(25708, 2, "DespawnSinisterReflections")
function InitializeSinisterReflection(Unit)
	local tar = Unit:GetClosestPlayer()
	local class = tar:GetPlayerClass()
	if class == "Warlock" then
		Unit:WorldQuery("DELETE FROM ai_agents WHERE entry ='25708';")
		Unit:WorldQuery("INSERT INTO ai_agents (entry,type,event,chance,maxcount,spell,spelltype,targettype,cooldown,floatMisc1,Misc2) VALUES('25708','0','0','100','0','46190','11','0','15000','0','0');")
		Unit:WorldQuery("INSERT INTO ai_agents (entry,type,event,chance,maxcount,spell,spelltype,targettype,cooldown,floatMisc1,Misc2) VALUES('25708','0','0','100','0','47076','8','1','2000','0','0');")
	end
	if class == "Priest" then
		Unit:AddAISpell(47077,4,1000,1,8,100,0)
		Unit:AddAISpell(47079,4,1000,5,2,100,0)
		Unit:SetCurrentAgent(4)
	end
	if class == "Shaman" then
		Unit:AddAISpell(47071,4,6000,1,8,100,0)
		Unit:SetCurrentAgent(4)
	end
	if class == "Hunter" then
		Unit:AddAISpell(16496,2,1000,1,8,100,0)
		Unit:AddAISpell(48098,2,1000,1,8,100,0)
		Unit:AddAISpell(47168,1,1000,1,8,100,0)
		Unit:SetCurrentAgent(2)
	end
	if class == "Paladin" then
		Unit:AddAISpell(37369,4,45000,1,3,100,0)
		Unit:AddAISpell(38921,4,15000,1,8,100,0)
		Unit:SetCurrentAgent(4)
	end
	if class == "Mage" then
		Unit:AddAISpell(47074,4,4000,1,8,100,0)
		Unit:SetCurrentAgent(4)
	end
	if class == "Rogue" then
		Unit:AddAISpell(45897,1,3000,1,8,100,0)
		Unit:SetCurrentAgent(1)
	end
	if class == "Druid" then
		Unit:AddAISpell(47072,4,1000,1,8,0)
		Unit:SetCurrentAgent(4)
	end
	if class == "Warrior" then
		Unit:AddAISpell(17207,1,1000,1,8,0)
		Unit:AddAISpell(15576,1,1000,1,8,0)
		Unit:SetCurrentAgent(1)
	end
end
function DespawnSinisterReflections(Unit)
	Unit:RemoveFromWorld()
end
--[[
	~SINISTER REFLECTION AI
		]]
--[[
	SHIELD ORB AI
		]]
local radians = nil
local shieldx = nil
local shieldy = nil
local clockwise = false
RegisterUnitEvent(25502, 18, "Initialize_ShieldOrb")
RegisterUnitEvent(25502, 4, "ShieldOrb_OnDeath")
RegisterUnitEvent(25502, 2, "ShieldOrb_OnWipe")
RegisterUnitEvent(25502, 21, "ShieldOrb_Update")
RegisterUnitEvent(25502, 19, "ShieldOrb_OnReachWp")
function  Initialize_ShieldOrb(Unit)
	Unit:SetCombatCapable(1)
	Unit:SetCombatMeleeCapable(1)
	Unit:SetCombatTargetingCapable(1)
	Unit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9)
	Unit:EnableFlight()
	Unit:ModifyFlySpeed(4)
	Unit:MonsterMove(Unit:GetX(),Unit:GetY(),Unit:GetZ()+10,5000,768)
	Unit:RegisterEvent("ShieldOrb_AI",5000,1)
	Unit:RegisterEvent("ShieldOrb_Movement",5000,1)
	radians = 0
	shieldx = 0
	shieldy = 0
	if math.random(2) == 1 then
		clockwise = true
	else
		clockwise = false
	end
end
function ShieldOrb_AI(Unit)
	Unit:SetPosition(Unit:GetX(),Unit:GetY(),Unit:GetZ()+10)
	Unit:RegisterAIUpdateEvent(2000)
	Unit:SetUInt32Value(UNIT_FIELD_FLAGS,0)
	if Unit:GetRandomPlayer(0) then
		(Unit:GetRandomPlayer(0)):AttackReaction(Unit,1,0)
	end
end
function ShieldOrb_Movement(Unit)
	if clockwise == true then
		radians = radians + 0.62818
	else
		radians = radians - 0.62818
	end
	shieldx = cn_kiljaeden:GetX()+math.cos(radians)*math.random(13,17)
	shieldy = cn_kiljaeden:GetY()+math.sin(radians)*math.random(13,17)
	Unit:MoveTo(shieldx,shieldy,Unit:GetZ(),Unit:GetO())
	Unit:RegisterEvent("ShieldOrb_Movement",5000,1)
end
function ShieldOrb_Update(Unit)
	Unit:WipeThreatList()
	if Unit:GetRandomPlayer(0) ~= nil then
		Unit:FullCastSpell(45680)
	end
end
function ShieldOrb_OnWipe(Unit)
	if Unit:GetRandomPlayer(0) == nil then
		Unit:RemoveAIUpdateEvent()
		Unit:RemoveEvents()
		for k,v in pairs(shield_orbtable) do
			if v == Unit then
				table.remove(shield_orbtable,k)
			end
		end
		print "ShieldOrb_OnWipe \n"
		print ("Number of Shield_OrbTables Entries are \n"..table.getn(shield_orbtable))
		Unit:RemoveFromWorld()
	end
end
function ShieldOrb_OnDeath(Unit)
	Unit:RemoveAIUpdateEvent()
	Unit:RemoveEvents()
	for k,v in pairs(shield_orbtable) do
		if v == Unit then
			table.remove(shield_orbtable,k)
		end
	end
	Unit:RemoveFromWorld()
	print "ShieldOrb_OnDeath \n"
	print ("Number of Shield_OrbTables Entries are \n"..table.getn(shield_orbtable))
end
RegisterUnitEvent(25319,18,"HelperKalec_OnSpawn")
RegisterUnitEvent(25319,19,"HelperKalec_OnReachWp")
function HelperKalec_OnSpawn(Unit)
	Unit:EnableFlight()
	Unit:MoveTo(Unit:GetX(),Unit:GetY(),Unit:GetZ()-1,Unit:GetO())
	Unit:MoveTo(Unit:GetX(),Unit:GetY(),Unit:GetZ()+1,Unit:GetO())
	Unit:ModifyFlySpeed(5)
	Unit:CreateWaypoint(1702.738037,598.220520,129.048126,0.006295,2000,768,0)
	Unit:CreateWaypoint(1727.781128,625.956665,120.866765,1.467142,2000,768,0)
	Unit:CreateWaypoint(1693.672974,656.997437,124.239868,3.269632,2000,768,0)
	Unit:CreateWaypoint(1668.829346,627.792847,122.001450,4.274939,2000,768,0)
	Unit:SetMovementType(4)
	Unit:FullCastSpellOnTarget(45670,cn_kiljaeden)
end
function HelperKalec_OnReachWp(Unit)
	Unit:SetMovementType(4)
end
RegisterGameObjectEvent(188116,2,"InitializeOrbOfBlue")
RegisterGameObjectEvent(188116,5,"OrbOfBlueUpdate")
RegisterGameObjectEvent(188116,4,"OrbOfBlue_OnActivate")
function InitializeOrbOfBlue(Unit)
	Unit:SetUInt32Value(GAMEOBJECT_FACING,Unit:CalcRadAngle(Unit:GetX(),Unit:GetY(),1698.896729,629.173546))
	Unit:Respawn()
	local guid = Unit:GetGUID()
	print(activatedorbs["guid"])
	if activatedorbs["guid"] ~= true then
		table.insert(blueorbs,Unit)
	end
end
function OrbOfBlue_OnActivate(Unit,event,pActivator)
	local guid = Unit:GetGUID()
	print(activatedorbs["guid"])
	if activatedorbs["guid"] == true then
		print ("Spawning Drake "..table.getn(activatedorbs) "\n")
		local drake = Unit:SpawnCreature(25653,pActivator:GetX(),pActivator:GetY(),pActivator:GetZ(),pActivator:GetO(),35,0)
		pActivator:PlayerCastSpellOnTarget(45839,drake)
		--pActivator:IsInvisible(1,0)
		activatedorbs["guid"] = false
		Unit:Despawn(1,5000)
		table.remove(activatedorbs,k)
		local orbtar = Unit:GetCreatureNearestCoords(Unit:GetX(),Unit:GetY(),Unit:GetZ(),25640)
		if orbtar ~= nil then
			orbtar:RemoveFromWorld()
		end
	end
end
function OrbOfBlue_OnDespawn(Unit,event)
	for k,v in pairs(blueorbs) do
		local guid = v:GetGUID()
		if activatedorbs["guid"] == false then
			table.remove(blueorbs,k)
		end
	end
end
		
RegisterUnitEvent(25640,18,"OrbTarget_OnSpawn")
function OrbTarget_OnSpawn(Unit)
	Unit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_SELECTABLE)
	Unit:SetUInt32Value(UNIT_FIELD_DISPLAYID,21908)
	--Unit:SetUInt32Value(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_ATTACKABLE_9)
	Unit:Root()
	Unit:SetCombatMeleeCapable(1)
	Unit:SetCombatTargetingCapable(1)
	Unit:WipeThreatList()
	Unit:CastSpell(37964)
	Unit:CastSpell(42709)
end
RegisterUnitEvent(25653, 18,"InitBlueDrake")
function InitBlueDrake(Unit)
	-- Execute Query
	print "InitBlueDrake Function"
	Unit:WorldQuery("DELETE FROM ai_agents WHERE entry ='25653';")
	Unit:WorldQuery("INSERT INTO ai_agents (entry,type,event,chance,maxcount,spell,spelltype,targettype,cooldown,floatMisc1,Misc2) VALUES('25653','4','0','100','0','45848','11','0','20000','0','0');")
	Unit:WorldQuery("INSERT INTO ai_agents (entry,type,event,chance,maxcount,spell,spelltype,targettype,cooldown,floatMisc1,Misc2) VALUES('25653','4','0','100','0','45856','11','0','10000','0','0');")
	Unit:WorldQuery("INSERT INTO ai_agents (entry,type,event,chance,maxcount,spell,spelltype,targettype,cooldown,floatMisc1,Misc2) VALUES('25653','4','0','100','0','45860','11','0','10000','0','0');")
	Unit:WorldQuery("INSERT INTO ai_agents (entry,type,event,chance,maxcount,spell,spelltype,targettype,cooldown,floatMisc1,Misc2) VALUES('25653','4','0','100','0','45862','0','0','15000','0','0');")
	Unit:Despawn(1,1)
end