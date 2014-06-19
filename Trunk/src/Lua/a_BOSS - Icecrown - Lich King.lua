-- update creature_spawn set slot1item=36942 where entry=36597
-- update gameobject_names set type=5 where entry = 500005

---- Lich King ---------------------------------------------------

function LichKing_OnSpawn(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	if Lich ~= nil then -- Do a check
		Lich:RemoveFromWorld()
	end
	-- Lich = pUnit
	pUnit:SetMapUInt32Variable( "LK_guid", pUnit:GetGUID( pUnit ) )
	pUnit:SetFaction(35)
	-- mark event as it could start
	pUnit:SetMapUInt32Variable( "LK_EventStarted", 0 )
end

RegisterUnitEvent(36597, 18, "LichKing_OnSpawn")

---- Tlehl ------------------------------------------------------
function tleh1_OnSpawn(pUnit, Event)
	--tleh1 = pUnit
	pUnit:SetMapUInt32Variable( "LKWTF_guid",pUnit:GetGUID(pUnit))
	-- set virtual item ? :O
	pUnit:SetUInt32Value(58, 26)
end

RegisterUnitEvent(36002, 18, "tleh1_OnSpawn")

---- Tirion ------------------------------------------------------

function TirionFordring_Gossip(unit, event, player)
	local Lichguid = unit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  unit:GetUnitByGUID(Lichguid)
	end 
	local deny_gossip =  unit:GetMapUInt32Variable( "LK_EventStarted" )
	if deny_gossip == 0 then
		unit:GossipCreateMenu(150, player, 0)
		unit:GossipMenuAddItem(0, "Start the boss fight", 1, 0)
		unit:GossipSendMenu(player)
	end
end

function TirionFordring_Submenus(pUnit, event, player, id, intid, code)
	local deny_gossip =  pUnit:GetMapUInt32Variable( "LK_EventStarted" )
	if(intid == 1 and deny_gossip == 0) then
		-- event started, hang on to your balls
		pUnit:SetMapUInt32Variable( "LK_EventStarted", 1 )
		
		-- remove gossip flag 
		pUnit:SetUInt32Value(82, 0)

		pUnit:PlaySoundToSet(17458)
		pUnit:RemoveEvents()
		local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
		local Lich = nil
		if Lichguid ~= nil then
			Lich =  pUnit:GetUnitByGUID(Lichguid)
		end 
		Lich:RemoveEvents()
		Lich:MoveTo(461.47, -2123.73, 957, 0)
		pUnit:Emote(375, 60000)
		pUnit:PlaySoundToSet(17349)
		Lich:SendChatMessage(14, 0, "So...the Light's vaunted justice has finally arrived. Shall I lay down Frostmourne and throw myself at your mercy, Fordring?")
		pUnit:RegisterEvent("TirionFordring_LK_StartEvent_chat1", 13500, 1)
	end
end


-- this is only required to register unit as scripted !!
-- though this is empty the scriptmanager will do quite a few things ;)
function TirionFordring_OnSpawn(pUnit, Event)
	pUnit:SetMapUInt32Variable( "LK_EventStarted", 0 )
	pUnit:SetMapUInt32Variable( "Tirion_guid", pUnit:GetGUID(pUnit) )
end

-- this is only required to register unit as scripted !!
RegisterUnitEvent(37119, 18, "TirionFordring_OnSpawn")	
RegisterUnitGossipEvent(37119, 1, "TirionFordring_Gossip")
RegisterUnitGossipEvent(37119, 2, "TirionFordring_Submenus")

function TirionFordring_LK_StartEvent_chat1(pUnit, Event)
	pUnit:SendChatMessage(14,0,"We will grant you a swift death, Arthas. More than can be said for the thousands you've tortured and slain.")
	pUnit:PlaySoundToSet(17390)
	pUnit:RegisterEvent("TirionFordring_LK_StartEvent_chat2", 7500, 1)
end

function TirionFordring_LK_StartEvent_chat2(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:SendChatMessage(14,0,"You will learn of that first hand. When my work is complete, you will beg for mercy -- and I will deny you. Your anguished cries will be testament to my unbridled power.")
	pUnit:PlaySoundToSet(17350)
	Lich:Emote(397, 4000)
	pUnit:RegisterEvent("LK_StartEvent_Emote1", 4000, 1)
	pUnit:RegisterEvent("LK_StartEvent_Emote2", 12000, 1)
	pUnit:RegisterEvent("TirionFordring_LK_StartEvent_chat3", 23000, 1)
end

function LK_StartEvent_Emote1(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:Emote(1, 6000)
end

function LK_StartEvent_Emote2(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:Emote(392, 3500)
end

function TirionFordring_LK_StartEvent_chat3(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:SendChatMessage(14,0,"So be it. Champions, attack!")
	pUnit:PlaySoundToSet(17391)
	pUnit:Emote(397, 2000)
	pUnit:RegisterEvent("PATHETIC_RUN_TIRION", 2000, 1)
	pUnit:RegisterEvent("TirionFordring_LK_StartEvent_chat4", 3000, 1)
	pUnit:RegisterEvent("TirionFordring_LK_StartEvent_chat5_startfight", 4800, 1)
end

function PATHETIC_RUN_TIRION(pUnit, Event)
	pUnit:SetMovementFlags(1)
	pUnit:MoveTo(485.92, -2123.2, 957, 3.17)
end

function TirionFordring_LK_StartEvent_chat4(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:SendChatMessage(14,0,"I'll keep you alive to witness the end, Fordring. I would not want the Light's greatest champion to miss seeing this wretched world remade in my image.")
	pUnit:PlaySoundToSet(17351)
end

function TirionFordring_LK_StartEvent_chat5_startfight(pUnit, Event)

	-- cast chill of the throne to reduce dmg by 20%
	local PlayersAllAround = pUnit:GetInRangePlayers()
	for a, players in pairs(PlayersAllAround) do
		players:CastSpell(69127)
	end

	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 

	Tirion:CastSpell(45776)	-- ice block
	Tirion:CastSpell(71614) -- blizzlike ice block ?

	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:SetFaction(22)
	local o = Lich:GetO()
	Lich:SpawnCreature(37695, 468, -2130, 957, o, 14, 0)
	Lich:SpawnCreature(37695, 465, -2140, 957, o, 14, 0)
	Lich:SpawnCreature(37695, 468, -2115, 957, o, 14, 0)
	Lich:SpawnCreature(37695, 465, -2105, 957, o, 14, 0)
	local ghoulmove = Lich:GetCreatureNearestCoords(468, -2130, 957, 37695)
	ghoulmove:MoveTo(474, -2130, 957,0)
	local ghoulmove1 = Lich:GetCreatureNearestCoords(465, -2140, 957, 37695)
	ghoulmove1:MoveTo(469, -2138, 957,0)
	local ghoulmove2 = Lich:GetCreatureNearestCoords(468, -2115, 957, 37695)
	ghoulmove2:MoveTo(474, -2115, 957,0)
	local ghoulmove3 = Lich:GetCreatureNearestCoords(465, -2105, 957, 37695)
	ghoulmove3:MoveTo(469, -2105, 957,0)
	Lich:MoveTo(474, -2123, 957,0)
end

-------- Rage Counter ---------------------------------------------

function Rage_Counter_Lich_King(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local Count = pUnit:GetMapUInt32Variable( "LK_Rage_count")
	local Phase = pUnit:GetMapUInt32Variable( "LK_phase")
	if Lich == nil then
		pUnit:RemoveEvents()
	else
		if Count == 300 then
			if Phase == 2 then
				--Count = 0
				pUnit:SetMapUInt32Variable( "LK_Rage_count",0)
				--Phase = 0
				pUnit:SetMapUInt32Variable( "LK_phase",0)
				Lich:RemoveEvents()
				Lich:SendChatMessage(14,0,"Face now your tragic end!")
				Lich:PlaySoundToSet(17365)
			else
				Lich:FullCastSpell(72143) -- Enrage --
				--Phase = Phase + 1
				pUnit:ModMapUInt32Variable("LK_phase",1)
			end
		else
			--Count = Count + 5
			pUnit:ModMapUInt32Variable("LK_Rage_count",5)
		end
	end
end

-------- Fight ----------------------------------------------------

function LichKingHasEnteredCombatLawl_Phase_One(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 
	local Players_Wiped = pUnit:GetMapUInt32Variable( "PlayersWiped")
	if Tirion == nil then
		pUnit:RemoveEvents()
		pUnit:Despawn(1, 10000)
		--Lich = nil
		pUnit:SetMapUInt32Variable( "LK_guid",0)
	else
		if Players_Wiped == 1 then
			--Players_Wiped = 0
			pUnit:SetMapUInt32Variable( "PlayersWiped",0)
			Lich:SetHealthPct(10)
		else
			Lich:SetCombatCapable(0)
			Lich:RegisterEvent("Rage_Counter_Lich_King", 5432, 0)
			Lich:RegisterEvent("Adds_Spawning_Incoming_Dredge_Ghouls", 19800, 0)
			if pUnit:GetIstanceDifficulty() >= 3 then
				Lich:RegisterEvent("Adds_Spawning_Incoming_Shambling_Horror", math.random(67800,78900), 0)
				Lich:RegisterEvent("Necrotic_Plague_Incoming", 23400, 0)
				Lich:RegisterEvent("Infest_Big_AOE_Incoming", 34500, 0)
			else
				Lich:RegisterEvent("Adds_Spawning_Incoming_Shambling_Horror", math.random(157800,178900), 0)
				Lich:RegisterEvent("Necrotic_Plague_Incoming", 43400, 0)
				Lich:RegisterEvent("Infest_Big_AOE_Incoming", 74500, 0)
			end
			Lich:RegisterEvent("Phase_Two_Checker", 2500, 0)
		end
	end
end

function Adds_Spawning_Incoming_Dredge_Ghouls(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local x = Lich:GetX()
	local y = Lich:GetY()
	local z = Lich:GetZ()
	local o = Lich:GetO()
	if math.random(1,2) == 1 then
		Lich:SpawnCreature(37695, x+math.random(1,6), y+math.random(1,6), z, o, 15, 360000)
	else
		Lich:SpawnCreature(37695, x-math.random(1,6), y-math.random(1,6), z, o, 15, 360000)
	end
	-- heroic 25 men
	if pUnit:GetIstanceDifficulty() >= 5 and math.random(1,2) == 1 then
		Lich:SpawnCreature(37695, x+math.random(1,6), y+math.random(1,6), z, o, 15, 360000)
	else
		Lich:SpawnCreature(37695, x-math.random(1,6), y-math.random(1,6), z, o, 15, 360000)
	end
	-- normal 25 or heroic 10 men
	if pUnit:GetIstanceDifficulty() >= 3 and math.random(1,2) == 1 then
		Lich:SpawnCreature(37695, x+math.random(1,6), y+math.random(1,6), z, o, 15, 360000)
	else
		Lich:SpawnCreature(37695, x-math.random(1,6), y-math.random(1,6), z, o, 15, 360000)
	end
end

function Adds_Spawning_Incoming_Shambling_Horror(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local x = Lich:GetX()
	local y = Lich:GetY()
	local z = Lich:GetZ()
	local o = Lich:GetO()
	if math.random(1,2) == 1 then
		Lich:SpawnCreature(37698, x+math.random(1,6), y+math.random(1,6), z, o, 15, 360000)
	else
		Lich:SpawnCreature(37698, x-math.random(1,6), y-math.random(1,6), z, o, 15, 360000)
	end
end

function Necrotic_Plague_Incoming(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local plr = Lich:GetRandomPlayer(0)
	if plr ~= nil then
		Lich:FullCastSpellOnTarget(70337, plr)
	end
end

function Infest_Big_AOE_Incoming(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:FullCastSpell(70541)
end

RegisterUnitEvent(36597, 1, "LichKingHasEnteredCombatLawl_Phase_One")

function Phase_Two_Checker(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	if Lich:GetHealthPct() < 71 then
		Lich:RemoveEvents()
		Lich:SetCombatCapable(1)
		Lich:MoveTo(506.7, -2122.5, 957, 0)
		Lich:SetMovementFlags(1)
		Lich:RegisterEvent("Phase2_CastBigAOEInCenter", 9005, 1)
	end
end

function Phase2_CastBigAOEInCenter(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:RemoveEvents()
	Lich:Root()
	-- Remorseless Winter - AOE
	Lich:FullCastSpell(68981)
	Lich:SendChatMessage(14,0,"I will freeze you from within until all that remains is an icy husk!")
	Lich:PlaySoundToSet(17369)
	Lich:RegisterEvent("EndOfBigAOEPhase2", 9500, 1)
end

function EndOfBigAOEPhase2(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local x = Lich:GetX()
	local y = Lich:GetY()
	-- Ice Sphere
	Lich:SpawnCreature(36633, x+math.random(3,6), y+math.random(3,6), Lich:GetZ(), Lich:GetZ(), 21, 10000)
	Lich:SpawnCreature(36633, x-math.random(3,6), y-math.random(3,6), Lich:GetZ(), Lich:GetZ(), 21, 10000)
	Lich:RegisterEvent("Spam_Of_Lightning_That_Looks_Crap_But_Is_Blizzlike", 2000, 0)
	if pUnit:GetIstanceDifficulty() >= 3 then
		Lich:RegisterEvent("RandomRagingSpiritAddsThatAreNotRandom", 19990, 3)
	else
		Lich:RegisterEvent("RandomRagingSpiritAddsThatAreNotRandom", 19990, 1)
	end
	Lich:RegisterEvent("Test_Despawn_Outer_Shell", 59000, 0)
end

function RandomRagingSpiritAddsThatAreNotRandom(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local plr = Lich:GetRandomPlayer(0)
	if plr ~= nil then
		local x,y,z,o = plr:GetX(),plr:GetY(),plr:GetZ(),plr:GetO()
		Lich:SpawnCreature(36701,x+math.random(3,6), y+math.random(3,6),z,o , 21, 360000)
	end
end

function Spam_Of_Lightning_That_Looks_Crap_But_Is_Blizzlike(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local plr = Lich:GetRandomPlayer(0)
	if plr ~= nil then
		Lich:FullCastSpellOnTarget(72133, plr)
	end
end

function Test_Despawn_Outer_Shell(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:RemoveEvents()
	Lich:SendChatMessage(14,0,"Watch as the world around you collapses!")
	Lich:PlaySoundToSet(17370)
	Lich:CastSpell(59084) -- Frozen Throne goes boom --
	Lich:SpawnCreature(36000, 476.2, -2083.56, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 505.1, -2074.47, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 523.35, -2077.77, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 541, -2086.36, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 552.7, -2111.2, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 554, -2137, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 545.9, -2153.1, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 528, -2164, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 508.25, -2169.5, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 488.2, -2168, 957, 0, 814, 20000)
	Lich:RegisterEvent("DelayForDespawnSinceVisualTakesSomeTimeTehe_Zieghoszho", 1000, 1)
	Lich:RegisterEvent("DelayForDespawnSinceVisualTakesSomeTimeTehe", 2500, 1)
end

function DelayForDespawnSinceVisualTakesSomeTimeTehe_Zieghoszho(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	-- Makes the floor change state
	Lich:FullCastSpell(72262) -- Epic shit I got working :D
end

function DelayForDespawnSinceVisualTakesSomeTimeTehe(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local gObject = Lich:GetGameObjectNearestCoords(pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(), 2957197)
	if gObject ~= nil then
--		gObject:Despawn(1,0)
		Lich:CastSpell(46853) --Shake--
	end
	Lich:SetCombatCapable(0)
	Lich:Unroot()
	-- Phase 2
	if pUnit:GetIstanceDifficulty() >= 3 then
		Lich:RegisterEvent("Infest_Big_AOE_Incoming", 22000, 0)
		Lich:RegisterEvent("Summon_Valkyr_Shadowguard", 19500, 0)
		Lich:RegisterEvent("Soul_Reaper_On_Main_Tank", 30000, 0)
		Lich:RegisterEvent("Phase_TwoPointFive_Checker", 2500, 0)
	else
		Lich:RegisterEvent("Infest_Big_AOE_Incoming", 42000, 0)
		Lich:RegisterEvent("Summon_Valkyr_Shadowguard", 59500, 0)
		Lich:RegisterEvent("Soul_Reaper_On_Main_Tank", 70000, 0)
		Lich:RegisterEvent("Phase_TwoPointFive_Checker", 2500, 0)
	end
	-- Defile --
end

function Soul_Reaper_On_Main_Tank(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local tank = Lich:GetMainTank()
	if tank ~= nil then
		Lich:FullCastSpellOnTarget(69409, tank)
	end
end

function Summon_Valkyr_Shadowguard(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local x, y, z, o = Lich:GetX()+7, Lich:GetY(), Lich:GetZ(), Lich:GetO()
	Lich:SpawnCreature(36609, x, y, z, o, 22, 0)
	Lich:SendChatMessage(14,0,"Val'kyr, your master calls!")
	Lich:PlaySoundToSet(17373)
end

function Phase_TwoPointFive_Checker(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	if Lich:GetHealthPct() < 41 then
		Lich:RemoveEvents()
		Lich:MoveTo(506.7, -2122.5, 957, 0)
		Lich:SetMovementFlags(1)
		Lich:SetCombatCapable(1)
		Lich:RegisterEvent("hhPhase2_CastBigAOEInCenter", 9005, 1)
	end
end

function hhPhase2_CastBigAOEInCenter(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:RemoveEvents()
	Lich:Root()
	Lich:FullCastSpell(68981)
	Lich:SendChatMessage(14,0,"I will freeze you from within until all that remains is an icy husk!")
	Lich:PlaySoundToSet(17369)
	Lich:RegisterEvent("jjEndOfBigAOEPhase2", 8500, 1)
end

function jjEndOfBigAOEPhase2(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local x = Lich:GetX()
	local y = Lich:GetY()
	Lich:SpawnCreature(36633, x+math.random(3,6), y+math.random(3,6), Lich:GetZ(), Lich:GetZ(), 21, 10000)
	Lich:RegisterEvent("Spam_Of_Lightning_That_Looks_Crap_But_Is_Blizzlike", 1500, 0)
	Lich:RegisterEvent("NeedToWaitBecauseAddsBreakAndShizzleOrSomething", 3005, 1)
	Lich:RegisterEvent("RandomRagingSpiritAddsThatAreNotRandom", 14000, 3)
	Lich:RegisterEvent("zzzTest_Despawn_Outer_Shell", 59000, 0)
end

function zzzTest_Despawn_Outer_Shell(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:RemoveEvents()
	Lich:SendChatMessage(14,0,"Watch as the world around you collapses!")
	Lich:PlaySoundToSet(17370)
	Lich:CastSpell(59084) -- Frozen Throne goes boom --
	Lich:SpawnCreature(36000, 476.2, -2083.56, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 505.1, -2074.47, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 523.35, -2077.77, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 541, -2086.36, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 552.7, -2111.2, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 554, -2137, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 545.9, -2153.1, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 528, -2164, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 508.25, -2169.5, 957, 0, 814, 20000)
	Lich:SpawnCreature(36000, 488.2, -2168, 957, 0, 814, 20000)
	Lich:RegisterEvent("DelayForDespawnSinceVisualTakesSomeTimeTehe_Zieghoszho", 1000, 1)
	Lich:RegisterEvent("zzzDelayForDespawnSinceVisualTakesSomeTimeTehe", 2500, 1)
end

function zzzDelayForDespawnSinceVisualTakesSomeTimeTehe(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:SetCombatCapable(0)
	Lich:Unroot()
	Lich:RemoveEvents()
	Lich:RegisterEvent("Soul_Reaper_On_Main_Tank", 30000, 0)
	Lich:RegisterEvent("Harvest_Soul_Random_Raid_Member", 66000, 0)
	Lich:RegisterEvent("Vile_Spirits_Spawning_Inc", 28000, 0)
	Lich:RegisterEvent("Phase_Four_The_Ending_tehe", 1500, 0)
end

function Harvest_Soul_Random_Raid_Member(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local plr = Lich:GetRandomPlayer(0)
	if plr ~= nil then
		Lich:SendChatMessage(14,0,"Frostmourne hungers...")
		Lich:PlaySoundToSet(17366)
		Lich:FullCastSpellOnTarget(68980, plr)
	end
end

function Vile_Spirits_Spawning_Inc(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:RegisterEvent("Spawning_The_Vile_spirits_NoW", 1000, 10)
end

function Spawning_The_Vile_spirits_NoW(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local pla = Lich:GetRandomPlayer(0)
	if pla ~= nil then
		local x, y, z, o = pla:GetX(), pla:GetY(), pla:GetZ(), pla:GetO()
		Lich:SpawnCreature(37799, x, y, z, o, 22, 60000)
	end
end

function Phase_Four_The_Ending_tehe(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	if Lich:GetHealthPct() < 11 then
		--Count = 0
		pUnit:SetMapUInt32Variable( "LK_Rage_count",0)
		--Phase = 0
		pUnit:SetMapUInt32Variable( "LK_phase",0)
		Lich:RemoveEvents()
		Lich:SetFacing(0.050405)
		Lich:SendChatMessage(14,0,"You gnats actually hurt me! Perhaps I've toyed with you long enough, now taste the vengeance of the grave!")
		Lich:PlaySoundToSet(17359)
		Lich:MoveTo(506.7, -2122.5, 957, 0)
		Lich:SetMovementFlags(1)
		Lich:SetCombatCapable(1)
		Lich:RegisterEvent("hggzhPhase2_CastBigAOEInCenter", 5005, 1)
	end
end

function hggzhPhase2_CastBigAOEInCenter(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:RemoveEvents()
	-- Quake spell
	Lich:CastSpell(70063)

	-- try to kill everyone
	local dumbass = Lich:GetInRangePlayers()
	for k, v in pairs(dumbass) do
		v:Kill()
	end

	--Players_Wiped = 1
	pUnit:SetMapUInt32Variable( "PlayersWiped",1)
	Lich:RemoveEvents()
	Lich:RegisterEvent("End_Fight_Chat1", 10000, 1)
end

function End_Fight_Chat1(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:SendChatMessage(14,0,"No question remains unanswered. No doubts linger. You are Azeroth's greatest champions! You overcame every challenge I laid before you. My mightiest servants have fallen before your relentless onslaught, your unbridled fury...")
	Lich:PlaySoundToSet(17353)
	Lich:RegisterEvent("End_Fight_Chat2", 25000, 1)
end

function End_Fight_Chat2(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:SendChatMessage(14,0,"You trained them well, Fordring. You delivered the greatest fighting force this world has ever known... right into my hands -- exactly as I intended. You shall be rewarded for your unwitting sacrifice.")
	Lich:PlaySoundToSet(17355)
	local plr = Lich:GetRandomPlayer(0)
	if plr ~= nil then
		Lich:ChannelSpell(71769, plr)
	end
	Lich:RegisterEvent("End_Fight_Chat3", 24000, 1)
end

function End_Fight_Chat3(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:SendChatMessage(14,0,"Watch now as I raise them from the dead to become masters of the Scourge. They will shroud this world in chaos and destruction. Azeroth's fall will come at their hands -- and you will be the first to die.")
	Lich:PlaySoundToSet(17356)
	local plr = Lich:GetRandomPlayer(0)
	if plr ~= nil then
	-- Lich:RegisterEvent("End_Fight_Chat4", 26000, 1)
	Lich:RegisterEvent("End_Fight_Chat4", 2000, 1)
	Lich:RegisterEvent("lich_channel_players_raise_dead", 500, 1)
	--[[Lich:RegisterEvent("imakeyoumineservants", 1000, 1)
	Lich:RegisterEvent("lichchannelplayers", 3000, 1)]]--
	end
end

function lich_channel_players_raise_dead(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local dumbass = Lich:GetInRangePlayers()
	for k, v in pairs(dumbass) do
	end
end

--[[function imakeyoumineservants(pUnit, Event)  -- Doesn't work like this no idea why--
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	for k, v in pairs(pUnit:GetInRangePlayers()) do
	if v:IsDead() then
		if npc[v:GetName()] ==  nil then
			npc[v:GetName()] = {};
			npc[v:GetName()].position = {v:GetX(), v:GetY(), v:GetZ(), v:GetO()}
			npc[v:GetName()].npcSpawned = true;			local x, y, z, o = pos;
			Lich:SpawnCreature(36001, npc[v:GetName()].position.x, npc[v:GetName()].position.y, npc[v:GetName()].position.z, npc[v:GetName()].position.o, 35, 0)
			break;
		end
	break;
	end
end
end

function lichchannelplayers(pUnit, Event)
end
--]]

function End_Fight_Chat4(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:PlaySoundToSet(17357)
	Lich:SendChatMessage(14,0,"I delight in the irony.")
	Lich:RegisterEvent("End_Fight_Chat5", 6000, 1)
	--Lich:StopChannel()
end

function End_Fight_Chat5(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 
	if Tirion == nil then
		Lich:Despawn(100,0)
	else
		Tirion:SendChatMessage(14,0,"LIGHT, GRANT ME ONE FINAL BLESSING. GIVE ME THE STRENGTH... TO SHATTER THESE BONDS!")
		Tirion:PlaySoundToSet(17392)
		Lich:RegisterEvent("Breakoutofice", 9000, 1)
	end
end

function Breakoutofice(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 
	-- Tirion:RemoveAura(45776)	-- looks blizzlike
	-- Tirion:RemoveAura(71614)	-- invincible

	-- probably looks great
	Tirion:CastSpell(71797)

	-- Tirion:CastSpell(71614)
	Lich:RegisterEvent("End_Fight_Chat6", 5000, 1)
end

function End_Fight_Chat6(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 

	Tirion:RemoveAura(45776)	-- looks blizzlike
	Tirion:RemoveAura(71614)	-- invincible

	-- probably looks great
	Tirion:CastSpell(71773)

	Tirion:MoveTo(495, -2123, 957,0)
	Lich:RegisterEvent("End_Fight_Chat7", 1700, 1)
end

function End_Fight_Chat7(pUnit, Event) --Teh holy jump--
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 
	Tirion:SetMovementFlags(2)
	Tirion:SetFlying()
	Tirion:Emote(375, 60000)
	Tirion:MoveTo(511, -2121, 957,0)
	Tirion:Emote(375, 60000)
	Tirion:RegisterEvent("lichisdead", 3900, 1)
	Lich:RegisterEvent("BrokeFrostmourne", 1900, 1)
end

function lichisdead(pUnit, Event)
	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 
	Tirion:MoveTo(514.135254, -2122.514160, 957,0)
end

function BrokeFrostmourne(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	-- local dumbasss = Lich:GetInRangePlayers()
	-- for k, v in pairs(dumbasss) do
	--	v:StopChannel()
	-- end

	Lich:StopChannel()
	Lich:CastSpell(37592) -- Knockdown
	Lich:CastSpell(72726) -- Boom visual
	Lich:FullCastSpell(72398) -- Broken Frostmourne (On the floor)
	Lich:EquipWeapons(0,0,0)
	Lich:CastSpell(73017) --Frostmourne Drops (Off the hands)

	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 
	Tirion:SetMovementFlags(1)
	Tirion:MoveTo(533.5, -2121, 957, 3.15)
	Lich:RegisterEvent("End_Fight_Chat9", 1000, 1)
end

function End_Fight_Chat9(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local tlehlguid = pUnit:GetMapUInt32Variable( "LKWTF_guid")
	local tleh1 = nil
	if tlehlguid ~= nil then
		tleh1 =  pUnit:GetUnitByGUID(tlehlguid)
	end 
	Lich:SendChatMessage(14,0,"Impossible...")
	Lich:PlaySoundToSet(17358)
	Lich:Emote(473, 120000)
	if(tleh1 ~= nil) then
		tleh1:CastSpell(72405)
	end
	Lich:RegisterEvent("End_Fight_Chat10", 3500, 1)
end

function End_Fight_Chat10(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 
	Tirion:SendChatMessage(14,0,"No more, Arthas! No more lives will be consumed by your hatred!")
	Tirion:PlaySoundToSet(17393)
	Tirion:SetMovementFlags(0)
	Lich:Emote(473, 120000)

	-- spawning Terenas
	Lich:SpawnCreature(36823, 518.8, -2115.8, 957, 3.780169, 35, 0)
	local Terenasguid = pUnit:GetMapUInt32Variable( "Terenas_guid")
	local Terenas = nil
	if Terenasguid ~= nil then
		Terenas =  pUnit:GetUnitByGUID(Terenasguid)
	end 

	Terenas:EquipWeapons(23456, 18825, 0)
	Tirion:MoveTo(525.654663, -2121.921143, 957, 3)
	Tirion:RemoveEvents()
	Lich:RegisterEvent("End_Fight_Chat11", 5500, 1)
end

function End_Fight_Chat11(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 
	local Terenasguid = pUnit:GetMapUInt32Variable( "Terenas_guid")
	local Terenas = nil
	if Terenasguid ~= nil then
		Terenas =  pUnit:GetUnitByGUID(Terenasguid)
	end 
	if Terenas == nil then
		Lich:Despawn(100,0)
		--Lich = nil
		pUnit:SetMapUInt32Variable( "LK_guid",0)
		Tirion:Despawn(100,0)
		--Tirion = nil
		pUnit:SetMapUInt32Variable( "Terenas_guid",0)
	else
		Tirion:RemoveEvents()
		Lich:Emote(473, 120000)
		Terenas:SendChatMessage(14,0,"Free at last! It is over, my son. This is the moment of reckoning.")
		Terenas:PlaySoundToSet(17397)
		Terenas:ChannelSpell(51361, Terenas)
		Lich:RegisterEvent("epiashgyophopapea_Lich_King_Is_Not_A_Happy_Bunny", 10000, 1)
	end
end

function epiashgyophopapea_Lich_King_Is_Not_A_Happy_Bunny(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local Terenasguid = pUnit:GetMapUInt32Variable( "Terenas_guid")
	local Terenas = nil
	if Terenasguid ~= nil then
		Terenas =  pUnit:GetUnitByGUID(Terenasguid)
	end 
	Terenas:SendChatMessage(14,0,"Rise up, champions of the Light!")
	Terenas:PlaySoundToSet(17398)
	Terenas:StopChannel()

	-- spell animation to raise all players
	Terenas:CastSpell(37755)

	Terenas:RemoveEvents()
	Lich:Emote(473, 120000)
	local PlayersAllAround = Terenas:GetInRangePlayers()
	for a, players in pairs(PlayersAllAround) do
		players:ResurrectPlayer(players)
		-- resurrection impact visual
		players:CastSpell(24171)
	end

	Lich:SetHealthPct(10)
	Lich:RegisterEvent("zLich_King_Is_Not_A_Happy_Bunny", 5000, 1)
end

function zLich_King_Is_Not_A_Happy_Bunny(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 
	local Terenasguid = pUnit:GetMapUInt32Variable( "Terenas_guid")
	local Terenas = nil
	if Terenasguid ~= nil then
		Terenas =  pUnit:GetUnitByGUID(Terenasguid)
	end 
	Tirion:MoveTo(509, -2125, 957,0)
	Terenas:MoveTo(509, -2121, 957,0)

	-- they will start attacking LK
	Tirion:FullCastSpellOnTarget(6603, Lich)
	Terenas:FullCastSpellOnTarget(6603, Lich)

	Lich:Emote(473, 120000)
	Lich:SendChatMessage(14,0,"Now I stand, the lion before the lambs... and they do not fear.")
	Lich:PlaySoundToSet(17361)
	Lich:RegisterEvent("zLich_King_Is_Not_A_Happy_Bunnyz", 8000, 1)
end

function zLich_King_Is_Not_A_Happy_Bunnyz(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	Lich:SendChatMessage(14,0,"They cannot fear.")
	Lich:PlaySoundToSet(17362)
	Lich:Emote(473, 120000)
end

---- Terenas -----------------------------------------------------

function Terenas_Spawned_root_Hes_the_realm_of_My_RETAIL_Char(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 
	if Tirion == nil or Lich == nil then
		pUnit:Despawn(100,0)
	else
		--Terenas = pUnit
		pUnit:SetMapUInt32Variable( "Terenas_guid",pUnit:GetGUID(pUnit))
	end
end

RegisterUnitEvent(36823, 18, "Terenas_Spawned_root_Hes_the_realm_of_My_RETAIL_Char")

-------------------------------------------------------------------

function LichKingHasDiedSoWePlayMovie(pUnit, Event, player)
	local Terenasguid = pUnit:GetMapUInt32Variable( "Terenas_guid")
	local Terenas = nil
	if Terenasguid ~= nil then
		Terenas =  pUnit:GetUnitByGUID(Terenasguid)
	end 
	local tlehlguid = pUnit:GetMapUInt32Variable( "LKWTF_guid")
	local tleh1 = nil
	if tlehlguid ~= nil then
		tleh1 =  pUnit:GetUnitByGUID(tlehlguid)
	end 
	if (tleh1 ~= nil) then
		tleh1:Despawn(100,0)
	end
	if (Terenas ~= nil) then
		Terenas:Despawn(100,0)
	end
	local Tirionguid = pUnit:GetMapUInt32Variable( "Tirion_guid")
	local Tirion = nil
	if Tirionguid ~= nil then
		Tirion =  pUnit:GetUnitByGUID(Tirionguid)
	end 
	Tirion:RemoveEvents()
	Tirion:RegisterEvent("IDespawn", 11000, 1)
	Lich = nil
	pUnit:RemoveEvents()
	pUnit:PlaySoundToSet(17374) -- Freak makes dying sound --
	pUnit:SpawnCreature(36003, pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), pUnit:GetO(), 35, 10000)
	player:AdvanceQuestObjective(24547, 0)
end

RegisterUnitEvent(36597, 4, "LichKingHasDiedSoWePlayMovie")

function IDespawn(pUnit, Event)
	-- Tirion = pUnit
	-- pUnit:SetMapUInt32Variable( "Tirion_guid",pUnit:GetGUID(pUnit))
	pUnit:Despawn(100,0)
	--Tirion = nil
	pUnit:SetMapUInt32Variable( "Tirion_guid",0)
end

function TriggerHasSpawnedSoPlayMovie(pUnit, Event)
	pUnit:RegisterEvent("PlayMovieOnceDeathAnimationHasPlayed", 9000, 1)
end

function PlayMovieOnceDeathAnimationHasPlayed(pUnit, Event)
	-- Reset Fight --
	--Tirion = nil
	pUnit:SetMapUInt32Variable( "Tirion_guid",0)
	--Lich = nil
	pUnit:SetMapUInt32Variable( "LK_guid",0)
	--Terenas = nil
	pUnit:SetMapUInt32Variable( "Terenas_guid",0)
	--package = nil
	pUnit:SetMapUInt32Variable( "Package_guid",0)
	--Phase = 0
	pUnit:SetMapUInt32Variable( "PlayersWiped",0)
	--Count = 0
	pUnit:SetMapUInt32Variable( "LK_guid",0)
	-- Movie --
	pUnit:SetMapUInt32Variable( "PlayersWiped",0)
	--Players_Wiped = 0
	local PlayersAllAround = pUnit:GetInRangePlayers()
	for a, players in pairs(PlayersAllAround) do
		players:PlayCinematic( 16 ) -- Send to players
	end
	-- Bolvar --
	local Bolvar = pUnit:GetGameObjectNearestCoords(428.758, -2124.4, 1594.69, 2508221) -- Bolvar in chains
	if Bolvar ~= nil then
		if Bolvar ~= nil then
			Bolvar:Despawn(100,0)
		end
	end
	pUnit:SpawnGameObject(2508220, 428.758, -2124.4, 1594.69, 0, 360000) -- Chains
	--pUnit:SpawnGameObject(2508219, 430.103, -2124, 1595.93, 3.3, 360000) -- Bolvar
	pUnit:SpawnGameObject(500005, 431.19, -2135.51, 1596.1, 3.3, 360000) -- Bolvar
end

RegisterUnitEvent(36003, 18, "TriggerHasSpawnedSoPlayMovie")


------- Adds ------------------------------------------------------

function Dredge_Ghouls_OnSpawn(pUnit, Event)
	pUnit:RegisterEvent("TestRootFunctionNextSecond", 1, 1)
end

function TestRootFunctionNextSecond(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	if Lich == nil then
		pUnit:RemoveFromWorld()
	else
		pUnit:Emote(449, 4000)
		pUnit:Root()
		pUnit:RegisterEvent("Delay_A_Second_And_See_WhatHappens_Tehe", math.random(1,1000), 1)
		pUnit:RegisterEvent("SetFactionToHostileForTheEmote", 4500, 1)
	end
end

function Delay_A_Second_And_See_WhatHappens_Tehe(pUnit, Event)
	pUnit:CastSpell(55719)
end

function SetFactionToHostileForTheEmote(pUnit, Event)
	pUnit:Unroot()
	pUnit:SetFaction(21)
end

RegisterUnitEvent(37695, 18, "Dredge_Ghouls_OnSpawn")

---------------------------------------------------------------------

function Shambling_Horrors_OnSpawn(pUnit, Event)
	pUnit:RegisterEvent("zzTestRootFunctionNextSecond", 1, 1)
end

function zzTestRootFunctionNextSecond(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	if Lich == nil then
		pUnit:RemoveFromWorld()
	else
		pUnit:Emote(449, 4000)
		pUnit:Root()
		pUnit:CastSpell(55719)
		pUnit:RegisterEvent("zzSetFactionToHostileForTheEmote", 4500, 1)
	end
end

function zzSetFactionToHostileForTheEmote(pUnit, Event)
	pUnit:Unroot()
	pUnit:SetFaction(21)
end

RegisterUnitEvent(37698, 18, "Shambling_Horrors_OnSpawn")

function Shambling_Horrors_OnCombat(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	if Lich == nil then
		pUnit:RemoveFromWorld()
	else
		pUnit:RegisterEvent("Enrage_Spam_Incoming_Varied_Time", math.random(20000,25000), 0)
		pUnit:RegisterEvent("Shockwave_thingy_time_notsure_on_timer", 15000, 0)
	end
end

function Enrage_Spam_Incoming_Varied_Time(pUnit, Event)
	pUnit:FullCastSpell(72143)
end

function Shockwave_thingy_time_notsure_on_timer(pUnit, Event)
	pUnit:FullCastSpell(72149)
end

function Shambling_Horrors_OnLeave(pUnit, Event)
	pUnit:RemoveEvents()
end

function Shambling_Horrors_OnDead(pUnit, Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(37698, 1, "Shambling_Horrors_OnCombat")
RegisterUnitEvent(37698, 2, "Shambling_Horrors_OnLeave")
RegisterUnitEvent(37698, 4, "Shambling_Horrors_OnDead")

----- Ice Sphere --------------------------------------------------

function IceShard_OnSpawn(pUnit, Event)
	pUnit:RegisterEvent("rage_rage_rage_rage_Rage_rage", 1, 1)
end

function rage_rage_rage_rage_Rage_rage(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	if Lich == nil then
		pUnit:RemoveFromWorld()
	else
		pUnit:FullCastSpell(69090) -- Visual
		local pla = pUnit:GetRandomPlayer(0)
		if pla ~= nil then
			pUnit:MoveTo(pla:GetX(), pla:GetY(), pla:GetZ(), pla:GetO())
			pUnit:SetMovementFlags(1)
			pUnit:RegisterEvent("rage_rage_rage_rage_Rage_rage_r", 1000, 8)
		else
			pUnit:RemoveFromWorld()
		end
	end
end

function rage_rage_rage_rage_Rage_rage_r(pUnit, Event)
	local plm = pUnit:GetClosestPlayer()
	if plm ~= nil then
		pUnit:FullCastSpell(69090)
		if pUnit:GetDistance(plm) < 4 then
			pUnit:FullCastSpellOnTarget(70827, plm) -- May be buggy if player dies, maybe not knockbacked and players try to exploit by ressing?
			--pUnit:FullCastSpellOnTarget(11027, plm) -- Knockback
			pUnit:CastSpell(56135)
			pUnit:RemoveEvents()
		end
	end
end

RegisterUnitEvent(36633, 18, "IceShard_OnSpawn")

---- Raging Spirit ------------------------------------------------

function RagingSpirit_OnCombat(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	if Lich == nil then
		pUnit:RemoveFromWorld()
	else
		pUnit:RegisterEvent("Soul_Shriek_Random_Time_Depending_On_Mob", math.random(7500, 15000), 0)
		pUnit:RegisterEvent("Soul_Shriek_Random_Visual", 1000, 1)
	end
end

function Soul_Shriek_Random_Visual(pUnit, Event)
	pUnit:FullCastSpell(69198) -- Visual
end

function Soul_Shriek_Random_Time_Depending_On_Mob(pUnit, Event)
	pUnit:FullCastSpell(69242) -- Shriek
end

function RagingSpirit_OnLeave(pUnit, Event)
	pUnit:RemoveEvents()
end

function RagingSpirit_OnDead(pUnit, Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(36701, 1, "RagingSpirit_OnCombat")
RegisterUnitEvent(36701, 2, "RagingSpirit_OnLeave")
RegisterUnitEvent(36701, 4, "RagingSpirit_OnDead")

---- Flying Dudes ------------------------------------------------------

function zhgzFlying_Dude_OnCombat(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	if Lich == nil then
		pUnit:RemoveFromWorld()
	else
		pUnit:RegisterEvent("zhgzFlying_Dude_Find_Player", 1, 1)
	end
end

function zhgzFlying_Dude_Find_Player(pUnit, Event)
	local package = pUnit:GetClosestPlayer()
	pUnit:SetMapUInt32Variable( "Package_guid",tpackage:GetGUID())
	if package ~= nil then
		pUnit:SetCombatCapable(1)
		package:SetPlayerLock(1)
		pUnit:SetMovementFlags(2)
		pUnit:ChannelSpell(55520, package) -- visual
		pUnit:MoveTo(pUnit:GetX(), pUnit:GetY(), pUnit:GetZ()+6, pUnit:GetO(), 12288)
		package:MovePlayerTo(package:GetX(), package:GetY(), package:GetZ()+6, package:GetO(), 12288)
		pUnit:RegisterEvent("now_we_have_loaded_preset_We_can_move_on", 2000, 1)
	else
		pUnit:RemoveFromWorld()
	end
end

function now_we_have_loaded_preset_We_can_move_on(pUnit, Event)
	local packageguid = pUnit:GetMapUInt32Variable( "Package_guid")
	local package = nil
	if packageguid ~= nil then
		package =  pUnit:GetUnitByGUID(packageguid)
	end 
	if package ~= nil then
		package:CastSpell(52241) -- Choke visual
		package:CastSpell(53658) -- Bubble to stop stupid animations
		pUnit:ChannelSpell(29172, package) -- visual
		pUnit:RegisterEvent("now_we_have_loaded_preset_We_can_move_on_two", 1500, 0)
	else
		pUnit:RemoveEvents()
		pUnit:RemoveFromWorld()
	end
end

function now_we_have_loaded_preset_We_can_move_on_two(pUnit, Event)
	local packageguid = pUnit:GetMapUInt32Variable( "Package_guid")
	local package = nil
	if packageguid ~= nil then
		package =  pUnit:GetUnitByGUID(packageguid)
	end 
	if package ~= nil then
		if pUnit:GetX() == 572 and pUnit:GetY() == -2121.5 then
			pUnit:RemoveEvents()
			pUnit:StopChannel()
			package:RemoveAura(52241) -- choke
			package:RemoveAura(53658) -- buble
			package:SetPlayerLock(0)
			package = nil
			--pUnit:RemoveFromWorld() -- Causes crash :(
		else
			pUnit:SetMovementFlags(2)
			package:ModifyFlySpeed(2.5)
			pUnit:MoveTo(572, -2121.5, 960, 0, 12288, 2.5)
			package:MovePlayerTo(pUnit:GetX(), pUnit:GetY(), pUnit:GetZ() ,pUnit:GetO(), 12288)
		end
	else
		pUnit:RemoveEvents()
		pUnit:RemoveFromWorld()
	end
end

function zhgzFlying_Dude_OnDead(pUnit, Event)
	pUnit:RemoveEvents()
	pUnit:StopChannel()
	local packageguid = pUnit:GetMapUInt32Variable( "Package_guid")
	local package = nil
	if packageguid ~= nil then
		package =  pUnit:GetUnitByGUID(packageguid)
	end 
	if(package ~= nil) then
		package:RemoveAura(52241) -- choke
		package:RemoveAura(53658) -- buble
		package:SetPlayerLock(0)
		package = nil
	end
end

RegisterUnitEvent(36609, 1, "zhgzFlying_Dude_OnCombat")
RegisterUnitEvent(36609, 4, "zhgzFlying_Dude_OnDead")

---- Vile Spirits ------------------------------------------------------

function VileSpiirt_OnCombat(pUnit, Event)
	local Lichguid = pUnit:GetMapUInt32Variable( "LK_guid" )
	local Lich = nil
	if Lichguid ~= nil then
		Lich =  pUnit:GetUnitByGUID(Lichguid)
	end 
	if Lich == nil then
		pUnit:RemoveFromWorld()
	else
		pUnit:RegisterEvent("deoaopighophgeoaugozh_delay", 500, 1)
	end
end

function deoaopighophgeoaugozh_delay(pUnit, Event)
	pUnit:Root()
	pUnit:SetCombatCapable(1)
	pUnit:RegisterEvent("Tick_Tick_Tick_dot_dot_dot_BOOM", 29000, 1)
end

function Tick_Tick_Tick_dot_dot_dot_BOOM(pUnit, Event)
	pUnit:RemoveEvents()
	pUnit:CastSpell(37106)
	pUnit:RegisterEvent("Kill_Self_Suicide_Or_Whathaveyou_vile", 1000, 1)
end

function Kill_Self_Suicide_Or_Whathaveyou_vile(pUnit, Event)
	pUnit:Despawn(100,0)
end

function VileSpiirt_OnLeave(pUnit, Event)
	pUnit:RemoveEvents()
end

function VileSpiirt_OnDead(pUnit, Event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(37799, 1, "VileSpiirt_OnCombat")
RegisterUnitEvent(37799, 2, "VileSpiirt_OnLeave")
RegisterUnitEvent(37799, 4, "VileSpiirt_OnDead")

------------------------------------------------------------------------