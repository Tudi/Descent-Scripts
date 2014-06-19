function RhahkZor_OnAggro(pUnit, Event)
	pUnit:SendChatMessage(12, 0,"Van Cleef pay big for your heads!")
	pUnit:PlaySoundToSet(5774)
end

function RhahkZor_OnDeath(pUnit, Event)
	local object = pUnit:GetGameObjectNearestCoords(pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), 13965)
	-- pUnit:SetObjectState(object,0) Such thing not supported yet.
	
	-- Miner Johnson Rare Spawn
	if(math.random(1,3) == 1) then
		pUnit:SpawnCreature(3586, -150.723999, -531.729004, 49.725700, 1.918860, 17, 0)
	end
end

RegisterUnitEvent(644, 1, "RhahkZor_OnAggro")
RegisterUnitEvent(644, 4, "RhahkZor_OnDeath")