function BarrettRamsey_OnSpawn(pUnit, event)
	pUnit:SetUInt32Variable(1,0) -- Northend Beasts
	pUnit:SetUInt32Variable(2,0) -- Lord Jaraxxus
	pUnit:SetUInt32Variable(3,0) -- Faction Champion
	pUnit:SetUInt32Variable(4,0) -- Twin Val'kyr
	pUnit:SetUInt32Variable(5,0) -- Anub'Arak
	pUnit:SetUInt32Variable(6,0) -- Acidmaw & Dreadscale
	pUnit:SetUInt32Variable(7,0) -- Champion1
	pUnit:SetUInt32Variable(8,0) -- Champion2
	pUnit:SetUInt32Variable(9,0) -- Champion3
	pUnit:SetUInt32Variable(10,0) -- Fjola & Eydis
	pUnit:SetMapUInt32Variable(1,pUnit:GetGUID(pUnit))
end

function TOCTirion_OnSpawn(pUnit, event)
	pUnit:PlaySoundToSet(16036)
	pUnit:SetMapUInt32Variable(2,pUnit:GetGUID(pUnit))
end

function TOCGarrosh_OnSpawn(pUnit, event)
	pUnit:SetMapUInt32Variable(3,pUnit:GetGUID(pUnit))
end

function TOCVarian_OnSpawn(pUnit, event)
	pUnit:SetMapUInt32Variable(4,pUnit:GetGUID(pUnit))
end

RegisterUnitEvent(34995, 18, "TOCGarrosh_OnSpawn")
RegisterUnitEvent(34990, 18, "TOCVarian_OnSpawn")

function BarrettRamsey_OnGossipTalk(pUnit, event, player)
	pUnit:GossipCreateMenu(100, player, 0)
	if(pUnit:GetUInt32Variable(1) == 0) then
		pUnit:GossipMenuAddItem(0, "I am ready for the next challenge.", 1, 0)
	elseif(pUnit:GetUInt32Variable(1) == 1) and (pUnit:GetUInt32Variable(2) == 0) then
		pUnit:GossipMenuAddItem(0, "I am ready for the next challenge.", 2, 0)
	elseif(pUnit:GetUInt32Variable(2) == 1) and (pUnit:GetUInt32Variable(3) == 0) then
		pUnit:GossipMenuAddItem(0, "I am ready for the next challenge.", 3, 0)
	elseif(pUnit:GetUInt32Variable(3) == 1) and (pUnit:GetUInt32Variable(4) == 0) then
		pUnit:GossipMenuAddItem(0, "I am ready for the next challenge.", 4, 0)
	elseif(pUnit:GetUInt32Variable(4) == 1) and (pUnit:GetUInt32Variable(5) == 0) then
		pUnit:GossipMenuAddItem(0, "I am ready for the next challenge.", 5, 0)
	end
	pUnit:GossipSendMenu(player)
end

function BarrettRamsey_OnGossip_select(pUnit, event, player, id, intid, code)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	pUnit:SetNPCFlags(0)
	if(intid == 1) then
		pUnit:RegisterEvent("Spawn_Gormok", 1000, 1)
	elseif(intid == 2) then	
		pUnit:RegisterEvent("Spawn_LordJaraxxus", 1000, 1)
	elseif(intid == 3) then
		local race = player:GetPlayerRace()
			if (race == 1 or race == 3 or race == 4 or race == 7 or race == 11) then
				pUnit:RegisterEvent("Spawn_HordeChampion", 1000, 1)
			elseif (race == 2 or race == 5 or race == 6 or race == 8 or race == 10) then
				pUnit:RegisterEvent("Spawn_AllianceChampion", 1000, 1)
			end
	elseif(intid == 4) then
		pUnit:RegisterEvent("Spawn_Twins", 1000, 1)
	elseif(intid == 5) then
		tirion:SendChatMessage(12, 0, "A mighty blow has been dealt to the Lich King! You have proven yourselves able bodied champions of the Argent Crusade. Together we will strike at Icecrown Citadel and destroy what remains of the Scourge! There is no challenge that we cannot face united!")
		pUnit:SpawnCreature(34564, 563.61, 170.44, 396, 4.704740, 16, 0)
	end
	tirion = null
end
RegisterUnitEvent(34996, 18, "TOCTirion_OnSpawn")
RegisterUnitEvent(34816, 18, "BarrettRamsey_OnSpawn")
RegisterUnitGossipEvent(34816,1,"BarrettRamsey_OnGossipTalk")
RegisterUnitGossipEvent(34816,2,"BarrettRamsey_OnGossip_select")

function Spawn_Gormok(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "Hailing from the deepest, darkest carverns of the storm peaks, Gormok the Impaler! Battle on, heroes!")
	pUnit:PlaySoundToSet(16038)
	pUnit:SpawnCreature(34796, 565.02, 211.62, 395.18, 4.63, 16, 0)
end

function Gormok_OnSpawn(pUnit, event)
	pUnit:SetUInt32Value(58, 6)
	pUnit:MoveTo(563.70, 171.34, 394.39, 4.67)
	pUnit:RegisterEvent("Gormok_OnSpawn1", 10000, 1)
end

function Gormok_OnSpawn1(pUnit, event)
	pUnit:PlaySoundToSet(16037)
	pUnit:SetUInt32Value(58, 0)
end

function Gormok_OnDeath(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	HelperNPC:RegisterEvent("Spawn_AcidmawDreadmaw", 10000, 1)
end


RegisterUnitEvent(34796, 18, "Gormok_OnSpawn")
RegisterUnitEvent(34796, 4, "Gormok_OnDeath")

--
-- Acidmaw
--

function Spawn_AcidmawDreadmaw(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "Steel yourselves, heroes, for the twin terrors Acidmaw and Dreadscale. Enter the arena!")
	pUnit:SpawnCreature(35144, 551.47, 170.44, 396, 4.831190, 16, 0)
	pUnit:SpawnCreature(34799, 577.02, 168.74, 396, 4.45198, 16, 0)
	pUnit:PlaySoundToSet(16039)
end

function Acidmaw_Dreadscale_OnDeath(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	if (HelperNPC:GetUInt32Variable(6) == 1) then
		HelperNPC:RegisterEvent("Spawn_Icehowl", 10000, 1)
	else
		HelperNPC:SetUInt32Variable(6,1)
	end
end


function Acidmaw_Dreadscale_Wipe(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	HelperNPC:SetNPCFlags(1)
	pUnit:Despawn(10000,0)
end

RegisterUnitEvent(35144, 4, "Acidmaw_Dreadscale_OnDeath")

--
-- Icehowl
--

function Spawn_Icehowl(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "The air freezes with the introduction of our next combatant, Icehowl! Kill or be killed, champions!")
	pUnit:PlaySoundToSet(16040)
	pUnit:SpawnCreature(34797, 565.02, 211.62, 395.18, 4.63, 16, 0)
end

function Icehowl_OnSpawn(pUnit, event)
	pUnit:SetUInt32Value(58, 6)
	pUnit:MoveTo(563.70, 171.34, 394.39, 4.67)
	pUnit:RegisterEvent("Icehowl_OnSpawn1", 10000, 1)
end

function Icehowl_OnSpawn1(pUnit, event)
	pUnit:PlaySoundToSet(16037)
	pUnit:SetUInt32Value(58, 0)
end

function Icehowl_OnDeath(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	HelperNPC:SetNPCFlags(1)
	HelperNPC:SetUInt32Variable(1,1)
end

RegisterUnitEvent(34797, 18, "Icehowl_OnSpawn")

--
-- Lord Jaraxxus
--

function Spawn_LordJaraxxus(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "Grand Warlock Wilfred Fizzlebang will summon forth your next challenge. Stand by for his entry!")
	pUnit:SpawnCreature(35458, 574.68, 143.04, 394, 0.819, 0, 35, 0)
	pUnit:PlaySoundToSet(16043)
end

function Fizzlebang_OnSpawn(pUnit, event)
	pUnit:RegisterEvent("Fizzlebang_OnSpawn1", 7500, 1)
end

function Fizzlebang_OnSpawn1(pUnit, event)
	pUnit:SendChatMessage(12, 0, "Thank you, Highlord! Now challengers, I will begin the ritual of summoning! When I am done, a fearsome Doomguard will appear!")
	pUnit:PlaySoundToSet(16268)
	pUnit:RegisterEvent("Fizzlebang_OnSpawn2", 10000, 1)
end

function Fizzlebang_OnSpawn2(pUnit, event)
	pUnit:SendChatMessage(12, 0, "Prepare for oblivion!")
	pUnit:CastSpell(46242)
	pUnit:PlaySoundToSet(16269)
	pUnit:RegisterEvent("Fizzlebang_OnSpawn3", 3500, 1)
end

function Fizzlebang_OnSpawn3(pUnit, event)
	pUnit:SendChatMessage(12, 0, "Ah ha! Behold the absolute power of Wilfred Fizzlebang, master summoner! You are bound to ME, demon!")
	pUnit:PlaySoundToSet(16270)
	pUnit:RegisterEvent("Fizzlebang_OnSpawn4", 12000, 1)
end

function Fizzlebang_OnSpawn4(pUnit, event)
	pUnit:SpawnCreature(34780, 563.81, 140.15, 393.92, 4.6286, 16, 0)
	pUnit:RegisterEvent("Fizzlebang_OnSpawn5", 5000, 1)
end

function Fizzlebang_OnSpawn5(pUnit, event)
	pUnit:SendChatMessage(12, 0, "But I'm in charge her-")
	pUnit:PlaySoundToSet(16271)
	pUnit:RegisterEvent("Fizzlebang_OnSpawn6", 1000, 1)
end

function Fizzlebang_OnSpawn6(pUnit, event)
	pUnit:KillTarget(pUnit)
end

function LordJaraxxus_OnSpawn(pUnit, event)
	pUnit:SetUInt32Value(58, 6)
	pUnit:RegisterEvent("LordJaraxxus_OnSpawn1", 4000, 1)
end

function LordJaraxxus_OnSpawn1(pUnit, event)
	pUnit:SendChatMessage(12, 0, "Trifling gnome, your arrogance will be your undoing!")
	pUnit:PlaySoundToSet(16143)
	pUnit:RegisterEvent("LordJaraxxus_OnSpawn2", 7000, 1)
end

function LordJaraxxus_OnSpawn2(pUnit, event)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "Quickly, heroes! Destroy the demon lord before it can open a portal to its twisted demonic realm!")
	pUnit:PlaySoundToSet(16044)
	pUnit:SetUInt32Value(58, 0)
end

function Jaraxxus_OnDeath(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "The loss of Wilfred Fizzlebang, while unfortunate, should be a lesson to those that dare dabble in dark magic. Alas, you are victorious and must now face the next challenge.")
	pUnit:PlaySoundToSet(16045)
	tirion:RegisterEvent("Jaraxxus_OnDeath1", 5000, 1)
	HelperNPC:SetNPCFlags(1)
	HelperNPC:SetUInt32Variable(2,1)
end

function Jaraxxus1_OnDeath(pUnit, event, miscUnit, Misc)
	local Garrosh = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(3))
	Garrosh:SendChatMessage(12, 0, "Treacherous Alliance dogs! You summon a demon lord against warriors of the Horde!? Your deaths will be swift!")
	pUnit:RegisterEvent("Jaraxxus_OnDeath2", 5000, 1)
end

function Jaraxxus_OnDeath2(pUnit, event, miscUnit, Misc)
	local Varian = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(4))
	Varian:SendChatMessage(12, 0, "The Alliance doesn't need the help of a demon lord to deal with Horde filth. Come, pig!")
	pUnit:RegisterEvent("Jaraxxus_OnDeath3", 5000, 1)
end

function Jaraxxus_OnDeath3(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "Everyone, calm down! Compose yourselves! There is no conspiracy at play here. The warlock acted on his own volition - outside of influences from the Alliance. The tournament must go on!")
	pUnit:PlaySoundToSet(16046)
end

RegisterUnitEvent(35458, 18, "Fizzlebang_OnSpawn")
RegisterUnitEvent(34780, 18, "LordJaraxxus_OnSpawn")

--
-- Alliance Champion
--
function Spawn_AllianceChampion(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "The next battle will be against the Argent Crusade's most powerful knights! Only by defeating them will you be deemed worthy...")
	pUnit:PlaySoundToSet(16047)
	pUnit:RegisterEvent("Spawn_AllianceChampion1", 8000, 1)
end

function Spawn_AllianceChampion1(pUnit, event, miscUnit, Misc)
	local Varian = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(4))
	Varian:SendChatMessage(12, 0, "Our honor has been besmirched! They make wild claims and false accusations against us. I demand justice! Allow my champions to fight in place of your knights, Tirion. We challenge the Horde!")
	pUnit:RegisterEvent("Spawn_AllianceChampion2", 5000, 1)
end

function Spawn_AllianceChampion2(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "Very well, I will allow it. Fight with honor!")
	pUnit:PlaySoundToSet(16048)
	pUnit:RegisterEvent("Spawn_AllianceChampion3", 5000, 1)
end

function Spawn_AllianceChampion3(pUnit, event, miscUnit, Misc)
	local Varian = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(4))
	Varian:SendChatMessage(12, 0, "Fight for the glory of the Alliance, heroes! Honor your king and your people!")
	pUnit:PlaySoundToSet(16065)
	pUnit:SpawnCreature(34475, 563.61, 170.44, 396, 4.704740, 16, 0)
	pUnit:SpawnCreature(34468, 567.87, 170.73, 396,  4.824122, 16, 0)
	pUnit:SpawnCreature(34469, 559.13, 169.75, 396, 4.824122, 16, 0)
end

--
-- Horde Champion
--
function Spawn_HordeChampion(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "The next battle will be against the Argent Crusade's most powerful knights! Only by defeating them will you be deemed worthy...")
	pUnit:PlaySoundToSet(16047)
	pUnit:RegisterEvent("Spawn_HordeChampion2", 8000, 1)
end

function Spawn_HordeChampion2(pUnit, event, miscUnit, Misc)
	local Garrosh = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(3))
	Garrosh:SendChatMessage(12, 0, "The Horde demands justice! We challenge the Alliance. Allow us to battle in place of your knights, paladin. We will show these dogs what it means to insult the Horde!")
	pUnit:RegisterEvent("Spawn_HordeChampion3", 5000, 1)
end

function Spawn_HordeChampion3(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "Very well, I will allow it. Fight with honor!")
	pUnit:PlaySoundToSet(16048)
	pUnit:RegisterEvent("Spawn_HordeChampion4", 5000, 1)
end

function Spawn_HordeChampion4(pUnit, event, miscUnit, Misc)
	local Garrosh = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(3))
	Garrosh:SendChatMessage(12, 0, "Show them no mercy, Horde champions! LOK'TAR OGAR!")
	pUnit:PlaySoundToSet(16066)
	pUnit:SpawnCreature(34453, 563.61, 170.44, 396, 4.704740, 16, 0)
	pUnit:SpawnCreature(34449, 567.87, 170.73, 396,  4.824122, 16, 0)
	pUnit:SpawnCreature(34459, 559.13, 169.75, 396, 4.824122, 16, 0)
end

function Champion1_OnDeath(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	HelperNPC:SetNPCFlags(1)
	HelperNPC:SetUInt32Variable(7,1)
	if(HelperNPC:GetUInt32Variable(7) == 1) and (HelperNPC:GetUInt32Variable(8) == 1) and (HelperNPC:GetUInt32Variable(9) == 1) then
		HelperNPC:SetUInt32Variable(3,1)
		pUnit:SpawnGameObject(195631, 563.52, 139.51, 394, 0, 0, 1)
		local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
		tirion:SendChatMessage(12, 0, "A shallow and tragic victory. We are weaker as a whole from the losses suffered today. Who but the Lich King could benefit from such foolishness? Great warriors have lost their lives. And for what? The true threat looms ahead - the Lich King awaits us all in death.")
		pUnit:PlaySoundToSet(16049)
	end
end

function Champion1_Wipe(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	HelperNPC:SetNPCFlags(1)
	HelperNPC:SetUInt32Variable(7,0)
	HelperNPC:SetUInt32Variable(8,0)
	HelperNPC:SetUInt32Variable(9,0)
	pUnit:Despawn(10000,0)
end

function Champion2_OnDeath(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	HelperNPC:SetNPCFlags(1)
	HelperNPC:SetUInt32Variable(8,1)
	if(HelperNPC:GetUInt32Variable(7) == 1) and (HelperNPC:GetUInt32Variable(8) == 1) and (HelperNPC:GetUInt32Variable(9) == 1) then
		HelperNPC:SetUInt32Variable(3,1)
		pUnit:SpawnGameObject(195631, 563.52, 139.51, 394, 0, 0, 1)
		local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
		tirion:SendChatMessage(12, 0, "A shallow and tragic victory. We are weaker as a whole from the losses suffered today. Who but the Lich King could benefit from such foolishness? Great warriors have lost their lives. And for what? The true threat looms ahead - the Lich King awaits us all in death.")
		pUnit:PlaySoundToSet(16049)
	end
end

function Champion2_Wipe(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	HelperNPC:SetNPCFlags(1)
	HelperNPC:SetUInt32Variable(7,0)
	HelperNPC:SetUInt32Variable(8,0)
	HelperNPC:SetUInt32Variable(9,0)
	pUnit:Despawn(10000,0)
end

function Champion3_OnDeath(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	HelperNPC:SetNPCFlags(1)
	HelperNPC:SetUInt32Variable(9,1)
	if(HelperNPC:GetUInt32Variable(7) == 1) and (HelperNPC:GetUInt32Variable(8) == 1) and (HelperNPC:GetUInt32Variable(9) == 1) then
		HelperNPC:SetUInt32Variable(3,1)
		pUnit:SpawnGameObject(195631, 563.52, 139.51, 394, 0, 0, 1)
		local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
		tirion:SendChatMessage(12, 0, "A shallow and tragic victory. We are weaker as a whole from the losses suffered today. Who but the Lich King could benefit from such foolishness? Great warriors have lost their lives. And for what? The true threat looms ahead - the Lich King awaits us all in death.")
		pUnit:PlaySoundToSet(16049)
	end
end

function Champion3_Wipe(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	HelperNPC:SetNPCFlags(1)
	HelperNPC:SetUInt32Variable(7,0)
	HelperNPC:SetUInt32Variable(8,0)
	HelperNPC:SetUInt32Variable(9,0)
	pUnit:Despawn(10000,0)
end

--
-- Fjola
--
		
function Spawn_Twins(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "Only by working together will you overcome the final challenge. From the depths of Icecrown come two of the Scourge's most powerful lieutenants: fearsome val'kyr, winged harbingers of the Lich King!")
	pUnit:PlaySoundToSet(16050)
	pUnit:SpawnCreature(34497, 551.47, 170.44, 396, 4.831190, 16, 0)
	pUnit:SpawnCreature(34496, 577.02, 168.74, 396, 4.45198, 16, 0)
	pUnit:RegisterEvent("Spawn_Twins1", 12000, 1)
end

function Spawn_Twins1(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "Let the games begin!")
	pUnit:PlaySoundToSet(16037)
end

function Twin_onSpawn(pUnit, event)
	pUnit:SetUInt32Value(58, 6)
	pUnit:RegisterEvent("Twin_onSpawn1", 12000, 1)
end

function Twin1_onSpawn(pUnit, event)
	pUnit:SetUInt32Value(58, 0)
end

function TwinValkyr_OnDeath(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	HelperNPC:SetNPCFlags(1)
	if (HelperNPC:GetUInt32Variable(10) == 1) then
		HelperNPC:SetUInt32Variable(4,1)
		tirion:RegisterEvent("TwinValkyr_Win", 1000, 1)
	else
		HelperNPC:SetUInt32Variable(10,1)
	end
end

function TwinValkyr_Wipe(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	HelperNPC:SetNPCFlags(1)
	HelperNPC:SetUInt32Variable(10,0)
	HelperNPC:SetUInt32Variable(4,0)
	pUnit:Despawn(10000,0)
end

function TwinValkyr_Win(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "A mighty blow has been dealt to the Lich King! You have proven yourselves able bodied champions of the Argent Crusade. Together we will strike at Icecrown Citadel and destroy what remains of the Scourge! There is no challenge that we cannot face united!")
	pUnit:PlaySoundToSet(16051)
	pUnit:RegisterEvent("Spawn_Anubarak", 20000, 1)
end


RegisterUnitEvent(34496, 18, "Twin_onSpawn")
RegisterUnitEvent(34497, 18, "Twin_onSpawn")

--
-- AnubArak
--
function Spawn_Anubarak(pUnit, event, miscUnit, Misc)
	pUnit:SpawnCreature(35877, 577.02, 168.74, 396, 4.45198, 35, 0)
end

function LichKing_onSpawn(pUnit, event, miscUnit, Misc)
	pUnit:SendChatMessage(12, 0, "You will have your challenge, Fordring.")
	pUnit:PlaySoundToSet(16321)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:RegisterEvent("LichKing_onSpawn1", 5000, 1)
	pUnit:RegisterEvent("LichKing_onSpawn2", 13000, 1)
end

function LichKing_onSpawn1(pUnit, event, miscUnit, Misc)
	local tirion = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(2))
	tirion:SendChatMessage(12, 0, "Arthas! You are hopelessly outnumbered! Lay down Frostmourne and I will grant you a just death.")
	pUnit:PlaySoundToSet(16052) -- 8 seconds
end

function LichKing_onSpawn2(pUnit, event, miscUnit, Misc)
	pUnit:SendChatMessage(12, 0, "The Nerubians built an empire beneath the frozen wastes of Northrend. An empire that you so foolishly built your structures upon. MY EMPIRE.")
	pUnit:PlaySoundToSet(16322) -- 20 seconds
	pUnit:RegisterEvent("LichKing_onSpawn3", 20000, 1)
end

function LichKing_onSpawn3(pUnit, event, miscUnit, Misc)
	pUnit:SendChatMessage(12, 0, "The souls of your fallen champions will be mine, Fordring.")
	pUnit:PlaySoundToSet(16323) -- 9 seconds
	pUnit:RegisterEvent("LichKing_onSpawn4", 9000, 1)
	pUnit:Despawn(9000,0)
	-- Time to remove the gameobject
	local floorz = pUnit:GetGameObjectNearestCoords(pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(),195527)
	floorz:Despawn(1000,0)
end

function LichKing_onSpawn4(pUnit, event, miscUnit, Misc)
	pUnit:PlaySoundToSet(16235)
end

RegisterUnitEvent(35877, 18, "LichKing_onSpawn")

function Anubarak_OnDeath(pUnit, event, miscUnit, Misc)
	pUnit:RemoveEvents()
	local HelperNPC = pUnit:GetUnitByGUID(pUnit:GetMapUInt32Variable(1))
	HelperNPC:SetNPCFlags(1)
	HelperNPC:SetUInt32Variable(5,1)
end

--

RegisterUnitEvent(35144, 2, "Acidmaw_Dreadscale_Wipe")
RegisterUnitEvent(34799, 4, "Acidmaw_Dreadscale_OnDeath")
RegisterUnitEvent(34799, 2, "Acidmaw_Dreadscale_Wipe")
RegisterUnitEvent(34797, 4, "Icehowl_OnDeath")
RegisterUnitEvent(34780, 4, "Jaraxxus_OnDeath")
RegisterUnitEvent(34453, 4, "Champion1_OnDeath")
RegisterUnitEvent(34453, 2, "Champion1_Wipe")
RegisterUnitEvent(34449, 4, "Champion2_OnDeath")
RegisterUnitEvent(34449, 2, "Champion2_Wipe")
RegisterUnitEvent(34459, 4, "Champion3_OnDeath")
RegisterUnitEvent(34459, 2, "Champion3_Wipe")
RegisterUnitEvent(34475, 4, "Champion1_OnDeath")
RegisterUnitEvent(34475, 2, "Champion1_Wipe")
RegisterUnitEvent(34468, 4, "Champion2_OnDeath")
RegisterUnitEvent(34468, 2, "Champion2_Wipe")
RegisterUnitEvent(34469, 4, "Champion3_OnDeath")
RegisterUnitEvent(34469, 2, "Champion3_Wipe")
RegisterUnitEvent(34497, 4, "TwinValkyr_OnDeath")
RegisterUnitEvent(34496, 4, "TwinValkyr_OnDeath")
RegisterUnitEvent(34497, 2, "TwinValkyr_Wipe")
RegisterUnitEvent(34496, 2, "TwinValkyr_Wipe")
RegisterUnitEvent(34564, 4, "Anubarak_OnDeath")