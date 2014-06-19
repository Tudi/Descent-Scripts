local Festergut
function Festergut_Define(Unit, Event)
	Festergut = Unit
end
RegisterUnitEvent(36626, 18, "Festergut_Define")


function Stinky_OnCombat(Unit, Event)
	Unit:RegisterEvent("Stinky_Decimate", 35000, 0)
	Unit:RegisterEvent("Stinky_MortalWound", 4000, 0)
	Unit:RegisterEvent("Stinky_PlagueStench", 10000, 0)
end

function Stinky_Decimate(Unit, Event)
	Unit:SendChatMessage(42, 0, "Stinky casts Decimate! Healers keep an eye!")
	Unit:FullCastSpell(71123)
end

function Stinky_MortalWound(Unit, Event)
	player = Unit:GetMainTank()
	Unit:SendChatMessage(42, 0, "Stinky casts Mortal Wound on "..player:GetName().."!")
	Unit:CastSpellOnTarget(71127, player)
end

function Stinky_PlagueStench(Unit, Event)
	Unit:CastSpell(71805)
end

function Stinky_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Stinky_OnDeath(Unit, Event)
	Festergut:SendChatMessage(14, 0, "Nooo! You kill Stinky! You pay!")
	Festergut:PlaySoundToSet(16907)
	player:AdvanceQuestObjective(24547, 0)
	Unit:RemoveEvents()
end

RegisterUnitEvent(37025, 1, "Stinky_OnCombat")
RegisterUnitEvent(37025, 2, "Stinky_OnLeaveCombat")
RegisterUnitEvent(37025, 4, "Stinky_OnDeath")