function HoggerEnterCombat(pUnit, Event)
	if pUnit:GetClosestPlayer(0) then
		pUnit:SendChatMessage(16, 0,pUnit:GetName().." charges!")
		pUnit:CastSpellOnTarget(6268, pUnit:GetClosestPlayer(0));
	end
end

RegisterUnitEvent(448, 1, "HoggerEnterCombat");
