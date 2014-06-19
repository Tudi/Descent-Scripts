function MarshalIsildor_Talk(pUnit, Event)
	pUnit:SendChatMessage(12, 0, "Do you think that demons will play fair? These beasts are more savage than orcs and more cunning that the undead. Do not attempt to engage Legion by normal standards of war.")
end

function MarshalIsildor_Start(pUnit, Event)
	pUnit:RegisterEvent("MarshalIsildor_Talk", 30000, 0)
end

RegisterUnitEvent(19308, 18, "MarshalIsildor_Start")

function BCR_OnDied(pUnit,Event)
	pUnit:CastSpell(32723)
end

RegisterUnitEvent(16925, 4, "BCR_OnDied")