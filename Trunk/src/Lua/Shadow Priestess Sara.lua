function Sara_OnCombat(Unit, Event)
	Unit:FullCastSpell(15473)
	Unit:SendChatMessage(14, 0, "Some things are better left alone!")
	Unit:PlaySoundToSet(15473)
	Unit:FullCastSpell(45813)
	Unit:RegisterEvent("Sara_ShadowLance", 6600, 0)
	Unit:RegisterEvent("Sara_ShadowVoid", 40000, 0)
	Unit:RegisterEvent("Sara_ShadowNova1", 14000, 0)
	Unit:RegisterEvent("Sara_ShadowReflection", 22000, 0)
	Unit:RegisterEvent("Sara_ShadowVortex", 4820, 0)
	Unit:RegisterEvent("Sara_ShadowBoltVolley", 12000, 0)
	Unit:RegisterEvent("Sara_Berserk", 320000, 0)
	Unit:RegisterEvent("Sara_Check1", 100, 0)
	Unit:GetGameObjectNearestCoords(-4509.291504, -2665.127197, 31.967117, 45540):Activate()
end

function Sara_ShadowBoltVolley(Unit, Event)
	Unit:FullCastSpell(55851)
end

function Sara_ShadowVoid(Unit, Event)
	Unit:PlaySoundToSet(16787)
	Unit:SendChatMessage(14, 0, "Can you handle this!")
	x = Unit:GetX()
	y = Unit:GetY()
	z = Unit:GetZ()
	o = Unit:GetO()
	Unit:SpawnCreature(111015, x+8, y+8, z, o, 16, 600000)
	Unit:SpawnCreature(111015, x+9, y+9, z, o, 16, 600000)
	Unit:SpawnCreature(111015, x+10, y+10, z, o, 16, 600000)
end

function Sara_ShadowVortex(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(91325, rand)
	end
end

function Sara_ShadowReflection(Unit, Event)
	Unit:PlaySoundToSet(16788)
	Unit:SendChatMessage(14, 0, "Here it comes!")
	Unit:FullCastSpell(51764)
end

function Sara_ShadowLance(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(104849, rand)
	end
end

function Sara_ShadowNova1(Unit, Event)
	Unit:FullCastSpell(65209)
end

function Sara_Berserk(Unit, Event)
	Unit:PlaySoundToSet(16793)
	Unit:SendChatMessage(14, 0, "This ends now!")
	Unit:FullCastSpell(26662)
end

function Sara_OnLeaveCombat(Unit, Event)
	Unit:PlaySoundToSet(16789)
	Unit:SendChatMessage(14, 0, "How…Unfortunate…")
Unit:GetGameObjectNearestCoords(-4509.291504, -2665.127197, 31.967117, 3.13, 45540):Activate()
	Unit:RemoveAllAuras()
	Unit:ReturnToSpawnPoint()
end

function Sara_OnKilledTarget(Unit, Event)
	Unit:PlaySoundToSet(15474)
	Unit:SendChatMessage(14, 0, "The secret dies with you!")
end

function Sara_Check1(Unit, Event)
    if Unit:GetHealthPct() <= 1 then
        Unit:GetGameObjectNearestCoords(-4550.796387, -2613.854736, 22.325476, 195327):Despawn(1, 1)
    end
end

function Sara_OnDeath(Unit, Event)
	Unit:PlaySoundToSet(15476)
	Unit:SendChatMessage(12, 0, "Sara screams in agony. You hear the sounds of a door crumbling.")
	Unit:RemoveAllAuras()
	Unit:GetGameObjectNearestCoords(-4509.291504, -2665.127197, 31.967117, 45540):Activate()
end

RegisterUnitEvent(111014, 1, "Sara_OnCombat")
RegisterUnitEvent(111014, 2, "Sara_OnLeaveCombat")
RegisterUnitEvent(111014, 3, "Sara_OnKilledTarget")
RegisterUnitEvent(111014, 4, "Sara_OnDeath")