function SnowplainSnobolds_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12144, 0)
end

function DragonblightMagnataur_OnDied(pUnit, event, player, Misc)
	player:AdvanceQuestObjective(12144, 1)
end

RegisterUnitEvent(27278, 4, "SnowplainSnobolds_OnDied")
RegisterUnitEvent(27279, 4, "SnowplainSnobolds_OnDied")
RegisterUnitEvent(34788, 4, "SnowplainSnobolds_OnDied")
RegisterUnitEvent(26295, 4, "DragonblightMagnataur_OnDied")
RegisterUnitEvent(26480, 4, "DragonblightMagnataur_OnDied")
RegisterUnitEvent(26836, 4, "DragonblightMagnataur_OnDied")
RegisterUnitEvent(27005, 4, "DragonblightMagnataur_OnDied")
RegisterUnitEvent(27004, 4, "DragonblightMagnataur_OnDied")
RegisterUnitEvent(27006, 4, "DragonblightMagnataur_OnDied")
RegisterUnitEvent(27007, 4, "DragonblightMagnataur_OnDied")
RegisterUnitEvent(27008, 4, "DragonblightMagnataur_OnDied")
RegisterUnitEvent(27009, 4, "DragonblightMagnataur_OnDied")