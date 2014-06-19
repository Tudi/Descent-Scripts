function Mekkatorque_OnAggro(pUnit, Event)
	pUnit:PlaySoundToSet(5897)
	pUnit:SendChatMessage(12, 0,"I won't stand for that!!")
end

function Mekkatorque_OnKilledTarget (pUnit, Event)
	pUnit:PlaySoundToSet(7255)
	pUnit:SendChatMessage(12, 0,"Pick on someone your own size!")
end

RegisterUnitEvent(7937, 1, "Mekkatorque_OnAggro")
RegisterUnitEvent(7937, 3, "Mekkatorque_OnKilledTarget")