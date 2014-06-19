function Sneed_OnDeath(pUnit, Event)
	local object = pUnit:GetGameObjectNearestCoords(pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), 16400)
	-- pUnit:SetObjectState(object,0) Such thing not supported yet.
end

RegisterUnitEvent(643, 4, "Sneed_OnDeath")