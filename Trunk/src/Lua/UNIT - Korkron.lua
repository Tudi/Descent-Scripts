function Korkron(Unit, event, player)
	Unit:RegisterEvent("Korkron_Say",69000, 0)
end

function Korkron_Say(Unit, event, player)
	Unit:SendChatMessage(12, 0, "Any time any of you peons want to take a break, the graveyard is right over there!")
end

function Korkron_Died(Unit, event, player)
	Unit:RemoveEvents()
end

RegisterUnitEvent(19362, 18, "Korkron")
RegisterUnitEvent(19362, 4, "Korkron_Died")