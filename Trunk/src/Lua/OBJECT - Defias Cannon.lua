function DefiasCannon_OnUse(pGameObject, Event)
	local pUnit = GetCreatureNearestCoords(pGameObject:GetX(), pGameObject:GetY(), pGameObject:GetZ(), 646)
	pGameObject:PlaySoundToSet(5775)
	pUnit:SendChatMessage(12, 0,"You there, check out that noise!")
end

RegisterGameObjectEvent(16398, 4, "DefiasCannon_OnUse")