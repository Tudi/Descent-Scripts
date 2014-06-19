local Gormok = 0
local Acidmaw = 0
local Icehowl = 0
local Jaraxxus = 0
local Champion = 0
local Champion1 = 0
local Champion2 = 0
local Champion3 = 0
local TwinValkyr = 0
local Anubarak = 0
local HelperNPC = null;

function BarrettRamsey_OnGossipTalk(pUnit, event, player)
	HelperNPC= pUnit;
	pUnit:GossipCreateMenu(100, player, 0)
	if(Gormok == 0) then
		pUnit:GossipMenuAddItem(0, "I'm ready to face against Gormok the Impaler.", 1, 0)
	end
	if(Gormok == 1) and (Acidmaw == 0) then
		pUnit:GossipMenuAddItem(0, "I'm ready to face against Acidmaw & Dreadscale.", 2, 0)
	end
	if(Acidmaw == 1) and (Icehowl == 0) then
		pUnit:GossipMenuAddItem(0, "I'm ready to face against Icehowl.", 3, 0)
	end
	if(Icehowl == 1) and (Jaraxxus == 0) then
		pUnit:GossipMenuAddItem(0, "I'm ready to face against Lord Jaraxxus.", 4, 0)
	end
	if(Jaraxxus == 1) and (Champion == 0) then
		pUnit:GossipMenuAddItem(0, "I'm ready to face against my faction champions.", 5, 0)
	end
	if(Champion == 1) and (TwinValkyr == 0) then
		pUnit:GossipMenuAddItem(0, "I'm ready to face against the Twin Val'kyr: Fjola & Eydis.", 6, 0)
	end
	if(TwinValkyr == 1) and (Anubarak == 0) then
		pUnit:GossipMenuAddItem(0, "I'm ready to face against Anub'arak.", 7, 0)
	end
	pUnit:GossipSendMenu(player)
end

function BarrettRamsey_OnGossip_select(pUnit, event, player, id, intid, code)
	if(intid == 1) then
		local tirion = pUnit:GetCreatureNearestCoords(pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(),34996)
		tirion:SendChatMessage(12, 0, "Hailing from the deepest, darkest carverns of the storm peaks, Gormok the Impaler! Battle on, heroes!")
		pUnit:SetNPCFlags(0)
		pUnit:SpawnCreature(34796, 563.61, 170.44, 396, 4.704740, 16, 0)
	end
	if(intid == 2) then
		local tirion = pUnit:GetCreatureNearestCoords(pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(),34996)
		tirion:SendChatMessage(12, 0, "Hailing from the deepest, darkest carverns of the storm peaks, Gormok the Impaler! Battle on, heroes!")
		pUnit:SetNPCFlags(0)
		pUnit:SpawnCreature(35144, 551.47, 170.44, 396, 4.831190, 16, 0)
		pUnit:SpawnCreature(34799, 577.02, 168.74, 396, 4.45198, 16, 0)
	end
	if(intid == 3) then
		local tirion = pUnit:GetCreatureNearestCoords(pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(),34996)
		tirion:SendChatMessage(12, 0, "The air freezes with the introduction of our next combatant, Icehowl! Kill or be killed, champions!")
		pUnit:SetNPCFlags(0)
		pUnit:SpawnCreature(34797, 563.61, 170.44, 396, 4.704740, 16, 0)
	end
	if(intid == 4) then
		local tirion = pUnit:GetCreatureNearestCoords(pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(),34996)
		tirion:SendChatMessage(12, 0, "Quickly, heroes! Destroy the demon lord before it can open a portal to its twisted demonic realm!")
		pUnit:SetNPCFlags(0)
		pUnit:SpawnCreature(34780, 563.61, 170.44, 396, 4.704740, 16, 0)
	end
	if(intid == 5) then
		pUnit:SetNPCFlags(0)
		local tirion = pUnit:GetCreatureNearestCoords(pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(),34996)
		tirion:SendChatMessage(12, 0, "The next battle will be against the Argent Crusade's most powerful knights! Only by defeating them will you be deemed worthy...")
		local race = player:GetPlayerRace()
			if (race == 1 or race == 3 or race == 4 or race == 7 or race == 11) then
				pUnit:SpawnCreature(34453, 563.61, 170.44, 396, 4.704740, 16, 0)
				pUnit:SpawnCreature(34449, 567.87, 170.73, 396,  4.824122, 16, 0)
				pUnit:SpawnCreature(34459, 559.13, 169.75, 396, 4.824122, 16, 0)
			elseif (race == 2 or race == 5 or race == 6 or race == 8 or race == 10) then
				pUnit:SpawnCreature(34475, 563.61, 170.44, 396, 4.704740, 16, 0)
				pUnit:SpawnCreature(34468, 567.87, 170.73, 396,  4.824122, 16, 0)
				pUnit:SpawnCreature(34469, 559.13, 169.75, 396, 4.824122, 16, 0)
			end
	end
	if(intid == 6) then
		pUnit:SetNPCFlags(0)
		local tirion = pUnit:GetCreatureNearestCoords(pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(),34996)
		pUnit:SpawnCreature(34497, 551.47, 170.44, 396, 4.831190, 16, 0)
		pUnit:SpawnCreature(34496, 577.02, 168.74, 396, 4.45198, 16, 0)
	end
	if(intid == 7) then
		pUnit:SetNPCFlags(0)
		local tirion = pUnit:GetCreatureNearestCoords(pUnit:GetX(),pUnit:GetY(),pUnit:GetZ(),34996)
		tirion:SendChatMessage(12, 0, "A mighty blow has been dealt to the Lich King! You have proven yourselves able bodied champions of the Argent Crusade. Together we will strike at Icecrown Citadel and destroy what remains of the Scourge! There is no challenge that we cannot face united!")
		pUnit:SpawnCreature(34564, 563.61, 170.44, 396, 4.704740, 16, 0)
	end
end

RegisterUnitGossipEvent(34816,1,"BarrettRamsey_OnGossipTalk")
RegisterUnitGossipEvent(34816,2,"BarrettRamsey_OnGossip_select")

function Gormok_OnDeath(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	Gormok = 1
end

function Acidmaw_Dreadscale_OnDeath(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	Acidmaw = 1
end

function Acidmaw_Dreadscale_Wipe(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	Acidmaw = 0
	pUnit:Despawn(10000,0)
end

function Icehowl_OnDeath(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	Icehowl = 1
end

function Jaraxxus_OnDeath(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	Jaraxxus = 1
end

function Champion1_OnDeath(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	Champion1 = 1
	if(Champion1 == 1) and (Champion2 == 1) and (Champion3 == 1) then
		Champion = 1
		pUnit:SpawnGameObject(195631, 563.52, 139.51, 394, 0, 0, 1)
	end
end

function Champion1_Wipe(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	Champion1 = 0
	Champion2 = 0
	Champion3 = 0
	Champion = 0
	pUnit:Despawn(10000,0)
end

function Champion2_OnDeath(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	Champion2 = 1
	if(Champion1 == 1) and (Champion2 == 1) and (Champion3 == 1) then
		Champion = 1
		pUnit:SpawnGameObject(195631, 563.52, 139.51, 394, 0, 0, 1)
	end
end

function Champion2_Wipe(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	Champion1 = 0
	Champion2 = 0
	Champion3 = 0
	Champion = 0
	pUnit:Despawn(10000,0)
end

function Champion3_OnDeath(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	Champion3 = 1
	if(Champion1 == 1) and (Champion2 == 1) and (Champion3 == 1) then
		Champion = 1
		pUnit:SpawnGameObject(195631, 563.52, 139.51, 394, 0, 0, 1)
	end
end

function Champion3_Wipe(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	Champion1 = 0
	Champion2 = 0
	Champion3 = 0
	Champion = 0
	pUnit:Despawn(10000,0)
end

function TwinValkyr_OnDeath(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	TwinValkyr = 1
end

function TwinValkyr_Wipe(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	TwinValkyr = 0
	pUnit:Despawn(10000,0)
end

function Anubarak_OnDeath(pUnit, event, miscUnit, Misc)
	HelperNPC:SetNPCFlags(1)
	Anubarak = 1
end

RegisterUnitEvent(34796, 4, "Gormok_OnDeath")
RegisterUnitEvent(35144, 4, "Acidmaw_Dreadscale_OnDeath")
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