function Myra_Yell(pUnit, Event)
	pUnit:SendChatMessage(12, 0, "Fresh bread, baked this very morning.")
	pUnit:RemoveEvents();
	pUnit:RegisterEvent("Myra2_Yell", 25000, 0)
end

function Myra2_Yell(pUnit, Event)
	pUnit:SendChatMessage(12 ,0, "Come get yer fresh bread.")
	pUnit:RemoveEvents();
	pUnit:RegisterEvent("Myra3_Yell", 25000, 0)
end

function Myra3_Yell(pUnit, Event)
	pUnit:SendChatMessage(12, 0, "Fresh bread for sale.")
	pUnit:RemoveEvents();
	pUnit:RegisterEvent("Myra_Yell", 120000, 0)
end

function Myra_Start(pUnit, Event)
	pUnit:RegisterEvent("Myra_Yell", 1000, 0)
end

function Myra_Died(pUnit, Event)
	pUnit:RemoveEvents();
end

RegisterUnitEvent(5109, 6, "Myra_Start")
RegisterUnitEvent(5109, 4, "Myra_Died")