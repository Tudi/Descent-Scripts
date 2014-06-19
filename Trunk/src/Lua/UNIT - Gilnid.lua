function Gilnid_OnDeath(pUnit, Event)
	local object = pUnit:GetGameObjectNearestCoords(pUnit:GetX(), pUnit:GetY(), pUnit:GetZ(), 16399)
	-- pUnit:SetObjectState(object,0) Such thing not supported yet.
end

RegisterUnitEvent(1763, 4, "Gilnid_OnDeath")