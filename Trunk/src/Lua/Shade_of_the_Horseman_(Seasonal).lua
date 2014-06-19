function Shade_OnSpawn(Unit, Event)
	Unit:SetFlying()
	Unit:CreateCustomWaypointMap()
	Unit:CreateWaypoint(-4754.313477, 3054.630859, 106.802383, 5.266832, 0, 768, 0)
	Unit:CreateWaypoint(-4733.625977, 3036.882568, 106.802383, 6.025527, 0, 768, 0)
	Unit:CreateWaypoint(-4721.438965, 3054.563477, 99.481377, 0.515861, 0, 768, 0)
	Unit:CreateWaypoint(-4708.997559, 3062.179199, 104.979156, 0.894423, 0, 768, 0)
	Unit:CreateWaypoint(-4715.622070, 3094.625000, 108.784836, 2.825106, 0, 768, 0)
	Unit:CreateWaypoint(-4747.462402, 3095.436523, 108.107590, 3.736344, 0, 768, 0)
	Unit:RegisterEvent("Shade_Intro1", 4000, 1)
	Unit:RegisterEvent("Shade_FireSpawns", 18000, 1)
	Unit:RegisterEvent("Shade_Chat1", 35000, 1)
	Unit:RegisterEvent("Shade_Chat2", 55000, 1)	
	Unit:RegisterEvent("Shade_Chat3", 80000, 1)	
	Unit:RegisterEvent("Shade_Chat4", 100000, 1)	
	Unit:RegisterEvent("Shade_Adds01", 120000, 1)	
	Unit:RegisterEvent("Shade_Chat6", 140000, 1)	
	Unit:RegisterEvent("Shade_Chat7", 160000, 1)	
	Unit:RegisterEvent("Shade_Chat8", 180000, 1)
	Unit:RegisterEvent("Shade_Chat9", 190000, 1)
	Unit:RegisterEvent("Shade_Chat10", 200000, 1)
	Unit:RegisterEvent("Shade_FailedEvent", 220000, 1)
end

function Shade_Intro1(Unit, Event)
	Unit:PlaySoundToSet(11966)
	Unit:SendChatMessage(14, 0, "Prepare yourselves, the bells have tolled! Shelter your weak, your young and your old! Each of you shall pay the final sum. Cry for mercy, the reckoning has come!")
	Unit:SetUInt32Value( 63, 25159 )
--	Unit:SetNPCFlags(4)
end

function Shade_FireSpawns(Unit, Event)
	local FireSpawn;
	FireSpawn = Unit:SpawnCreature(23537, -4738.5517, 3043.2282, 98.4838, 5.16, 14, 0)
	Unit:SetUInt32Variable( 1, FireSpawn:GetGUID( FireSpawn ) )
	FireSpawn = Unit:SpawnCreature(23537, -4744.6757, 3042.5920, 99.1200, 4.64, 14, 0)	
	Unit:SetUInt32Variable( 2, FireSpawn:GetGUID( FireSpawn ) )
	FireSpawn = Unit:SpawnCreature(23537, -4750.1523, 3042.7460, 99.5432, 4.49, 14, 0)	
	Unit:SetUInt32Variable( 3, FireSpawn:GetGUID( FireSpawn ) )
	FireSpawn = Unit:SpawnCreature(23537, -4763.8984, 3051.1660, 92.0082, 3.52, 14, 0)	
	Unit:SetUInt32Variable( 4, FireSpawn:GetGUID( FireSpawn ) )
	FireSpawn = Unit:SpawnCreature(23537, -4717.3305, 3052.7258, 103.6008, 4.19, 14, 0)
	Unit:SetUInt32Variable( 5, FireSpawn:GetGUID( FireSpawn ) )
	FireSpawn = Unit:SpawnCreature(23537, -4713.3325, 3053.1906, 101.7098, 4.94, 14, 0)	
	Unit:SetUInt32Variable( 6, FireSpawn:GetGUID( FireSpawn ) )
	FireSpawn = Unit:SpawnCreature(23537, -4709.7094, 3057.4831, 104.2179, 5.50, 14, 0)	
	Unit:SetUInt32Variable( 7, FireSpawn:GetGUID( FireSpawn ) )
	FireSpawn = Unit:SpawnCreature(23537, -4720.4321, 3052.0017, 94.6480, 4.78, 14, 0)	
	Unit:SetUInt32Variable( 8, FireSpawn:GetGUID( FireSpawn ) )
	FireSpawn = Unit:SpawnCreature(23537, -4711.6694, 3052.9018, 94.5998, 4.74, 14, 0)
	Unit:SetUInt32Variable( 9, FireSpawn:GetGUID( FireSpawn ) )
	FireSpawn = Unit:SpawnCreature(23537, -4751.7822, 3044.3315, 92.0086, 4.57, 14, 0)
	Unit:SetUInt32Variable( 10, FireSpawn:GetGUID( FireSpawn ) )
	FireSpawn = Unit:SpawnCreature(23537, -4720.2436, 3036.0263, 95.6232, 2.30, 14, 0)
	Unit:SetUInt32Variable( 11, FireSpawn:GetGUID( FireSpawn ) )
	FireSpawn = Unit:SpawnCreature(23537, -4738.6166, 3034.6926, 93.9272, 3.07, 14, 0)
	Unit:SetUInt32Variable( 12, FireSpawn:GetGUID( FireSpawn ) )
end
			
function Shade_Chat1(Unit, Event)
	Unit:PlaySoundToSet(11965)
	Unit:RegisterEvent("Shade_CheckFires", 2800, 0)
end

function Shade_Chat2(Unit, Event)
	Unit:PlaySoundToSet(12570)
	Unit:SendChatMessage(14, 0, "The sky is dark. The fire burns. You strive in vain as Fate's wheel turns.")
end

function Shade_Chat3(Unit, Event)
	Unit:PlaySoundToSet(12573)
	Unit:SendChatMessage(14, 0, "Harken, cur! Tis you I spurn! Now feel… the burn!")
end

function Shade_Chat4(Unit, Event)
	Unit:PlaySoundToSet(11962)
	Unit:SendChatMessage(14, 0, "Your body lies beaten, battered and broken. Let my curse be your own, fate has spoken.")
end

function Shade_Adds01(Unit, Event)
	Unit:PlaySoundToSet(11963)
	Unit:SendChatMessage(14, 0, "Soldiers arise, stand and fight! Bring victory at last to this fallen knight!")
end

function Shade_Chat6(Unit, Event)
	Unit:PlaySoundToSet(11965)
end

function Shade_Chat7(Unit, Event)
	Unit:PlaySoundToSet(12570)
	Unit:SendChatMessage(14, 0, "The sky is dark. The fire burns. You strive in vain as Fate's wheel turns.")
end

function Shade_Chat8(Unit, Event)
	Unit:PlaySoundToSet(12573)
	Unit:SendChatMessage(14, 0, "Harken, cur! Tis you I spurn. Now feel… the burn!")
end

function Shade_Chat9(Unit, Event)
	Unit:PlaySoundToSet(12571)
	Unit:SendChatMessage(14, 0, "The town still burns, a cleansing fire! Time is short, I'll soon retire!")
end

function Shade_Chat10(Unit, Event)
	Unit:PlaySoundToSet(11965)
end

function Shade_CheckFires(Unit, Event)
	local FireCount=0
	for i=1, 12 do
--local FireGuid=Unit:GetUInt32Variable( i )
--Unit:SendChatMessage(14, 0,  "fire guid"..FireGuid.." !")
--Unit:GetUnitByGUID( FireGuid ):SendChatMessage(14, 0,  "fire guid"..FireGuid.." !")
		local Fire = Unit:GetUnitByGUID( Unit:GetUInt32Variable( i ) )
		if Fire ~= nil then
			FireCount = FireCount + 1
		end
	end
--Unit:SendChatMessage(14, 0,  "fire count"..FireCount.." !")
	if FireCount == 0 then
		Unit:RemoveEvents()
		Unit:DestroyCustomWaypointMap()
		Unit:PlaySoundToSet(11968)
		Unit:SendChatMessage(14, 0, "My flames have died, left not a spark. I shall send you myself, to the lifeless dark.")
		Unit:MoveTo(-4737.9790, 3065.3122, 92.1040, 0.286670)
		Unit:RegisterEvent("Shade_Ground", 10000, 1)
		Unit:RegisterEvent("Shade_Land", 11000, 1)
		Unit:RegisterEvent("Shade_ChangeFlags", 12000, 1)
	end
end

function Shade_FailedEvent(Unit, Event)
	Unit:RemoveEvents()
	Unit:PlaySoundToSet(11967)
	Unit:SendChatMessage(14, 0, "Fire consumes! You've tried and failed. Let there be no doubt, justice prevailed!")
	Unit:GetCreatureNearestCoords(-4738.5517, 3043.2282, 98.4838, 23537):Despawn(1000, 1)
	Unit:GetCreatureNearestCoords(-4744.6757, 3042.5920, 99.1200, 23537):Despawn(1000, 1)
	Unit:GetCreatureNearestCoords(-4750.1523, 3042.7460, 99.5432, 23537):Despawn(1000, 1)
	Unit:GetCreatureNearestCoords(-4763.8984, 3051.1660, 92.0082, 23537):Despawn(1000, 1)
	Unit:GetCreatureNearestCoords(-4717.3305, 3052.7258, 103.6008, 23537):Despawn(1000, 1)
	Unit:GetCreatureNearestCoords(-4713.3325, 3053.1906, 101.7098, 23537):Despawn(1000, 1)
	Unit:GetCreatureNearestCoords(-4709.7094, 3057.4831, 104.2179, 23537):Despawn(1000, 1)
	Unit:GetCreatureNearestCoords(-4720.4321, 3052.0017, 94.6480, 23537):Despawn(1000, 1)
	Unit:GetCreatureNearestCoords(-4711.6694, 3052.9018, 94.5998, 23537):Despawn(1000, 1)
	Unit:GetCreatureNearestCoords(-4751.7822, 3044.3315, 92.0086, 23537):Despawn(1000, 1)
	Unit:GetCreatureNearestCoords(-4720.2436, 3036.0263, 95.6232, 23537):Despawn(1000, 1)
	Unit:GetCreatureNearestCoords(-4738.6166, 3034.6926, 93.9272, 23537):Despawn(1000, 1)
	Unit:Despawn(4000, 600000)
end

function Shade_Ground(Unit, Event)
	Unit:Dismount()
end

function Shade_Land(Unit, Event)
	Unit:Land()
end

function Shade_ChangeFlags(Unit, Event)
	Unit:SetFaction(16)
end

function Shade_OnCombat(Unit, Event)
end

function Shade_OnKilledTarget(Unit, Event)
end

function Shade_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Shade_OnDeath(Unit, Event)
	Unit:SetFaction(35)
	Unit:PlaySoundToSet(11969)
	Unit:SendChatMessage(14, 0, "So eager you are, for my blood to spill. Yet to vanquish me, 'tis my head you must kill!")
	Unit:SpawnCreature(111051, -4738.9482, 3069.1228, 92.1040, 1.1969, 35, 0)
end

RegisterUnitEvent(111047, 1, "Shade_OnCombat")
RegisterUnitEvent(111047, 2, "Shade_OnLeaveCombat")
RegisterUnitEvent(111047, 3, "Shade_OnKilledTarget")
RegisterUnitEvent(111047, 4, "Shade_OnDeath")
RegisterUnitEvent(111047, 18, "Shade_OnSpawn")