function Miller_Yell(pUnit, Event)
	pUnit:SendChatMessage(12, 0, "Fresh bread, baked this very morning.")
	pUnit:RemoveEvents();
	pUnit:RegisterEvent("Miller2_Yell", 25000, 0)
end

function Miller2_Yell(pUnit, Event)
	pUnit:SendChatMessage(12 ,0, "Come get yer fresh bread.")
	pUnit:RemoveEvents();
	pUnit:RegisterEvent("Miller3_Yell", 25000, 0)
end

function Miller3_Yell(pUnit, Event)
	pUnit:SendChatMessage(12, 0, "Fresh bread for sale.")
	pUnit:RemoveEvents();
	pUnit:RegisterEvent("Miller_Yell", 120000, 0)
end

function Miller_Start(pUnit, Event)
	pUnit:RegisterEvent("Miller_Yell", 1000, 0)
end

function Miller_Died(pUnit, Event)
	pUnit:RemoveEvents();
end

RegisterUnitEvent(3518, 6, "Miller_Start")
RegisterUnitEvent(3518, 4, "Miller_Died")