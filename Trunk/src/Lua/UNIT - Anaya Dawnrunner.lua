-- Anaya Dawnrunner
function Anaya_Dawnrunner_onDeath (pUnit, Event)
	pUnit:SendChatMessage (12, 0, "Finally, my soul may rest... Oh, dearest Cerellean...")
end
RegisterUnitEvent (3667, 4, "Anaya_Dawnrunner_onDeath")