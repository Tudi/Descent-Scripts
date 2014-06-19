function Festergut_OnCombat(Unit, Event)
	Unit:SendChatMessage(14, 0, "Fun time!")
	Unit:PlaySoundToSet(16901)
	Unit:RegisterEvent("Festergut_GasSpore", 18000, 0)
	Unit:RegisterEvent("Festergut_GassesCombination", 14000, 1)
	Unit:RegisterEvent("Festergut_GastricBloat", 11000, 0)
	Unit:RegisterEvent("FesterGut_VileGas", 21000, 0)
end

function Festergut_GasSpore(Unit, Event)
	local playerCount = Unit:GetInRangePlayersCount()
	local player1 = Unit:GetRandomPlayer(7)
	local player2 = Unit:GetRandomPlayer(7)
	while ((player1 == player2) and player1 ~= nil and playerCount > 2) do
		player2 = Unit:GetRandomPlayer(7)
	end
	if player1 ~= nil then
		Unit:SendChatMessage(42, 0, "Festergut casts Gas Spore on "..player1:GetName().."!")
		Unit:CastSpellOnTarget(71221, player1)
	end
	if player2 ~= nil then
		Unit:SendChatMessage(42, 0, "Festergut casts Gas Spore on "..player2:GetName().."!")
		Unit:CastSpellOnTarget(71221, player2)
	end
	Unit:PlaySoundToSet(16911)
end

function Festergut_GassesCombination(Unit, Event)
	Unit:CastSpell(70135)
	Unit:SendChatMessage(42, 0, "Festergut casts Gaseous Blight!")
	Unit:RegisterEvent("Festergut_GaseousBlight2", 14000, 1)
end		
function Festergut_GaseousBlight2(Unit, Event)
	Unit:CastSpell(70135)
	Unit:RegisterEvent("Festergut_GaseousBlight3", 14000, 1)
end
function Festergut_GaseousBlight3(Unit, Event)
	Unit:CastSpell(70135)
	Unit:RegisterEvent("Festergut_InhaleGaseousBlights", 33000, 1)
end
function Festergut_InhaleGaseousBlights(Unit, Event)
	Unit:FullCastSpell(69165)
	Unit:SendChatMessage(42, 0, "Festergut takes a deep breath, inhales the Gaseous Blight in the room, increasing damage dealt!")
	Unit:RegisterEvent("Festergut_PungentBlight", 34000, 1)
end
function Festergut_PungentBlight(Unit, Event)
	Unit:SendChatMessage(42, 0, "Festergut casts Pungent Blight!")
	Unit:SendChatMessage(14, 0, "Gyah! Uhhh, I not feel so good...")
	Unit:PlaySoundToSet(16906)
end

function Festergut_GastricBloat(Unit, Event)
	Unit:CastSpellOnTarget(72219, Unit:GetMainTank())
	Unit:SendChatMessage(42, 0, "Festergut casts Gastric Bloat. Avoid 10 stacks!")
end

function FesterGut_VileGas(Unit, Event)
	local player = Unit:GetRandomPlayer(7)
	if (player ~= nil) then
		local x, y, z = player:GetX(), player:GetY(), player:GetZ()
		Unit:CastSpellAoF(x, y, z, 71218)
	end
end

function Festergut_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Festergut_OnKilledTarget(Unit, Event)
	local kill = math.random(1,2)
	if (kill == 1) then
		Unit:SendChatMessage(14, 0, "Daddy, I did it!")
		Unit:PlaySoundToSet(16902)
	elseif (kill == 2) then
		Unit:SendChatMessage(14, 0, "Dead, dead, dead!")
		Unit:PlaySoundToSet(16903)
	end
end

-------- Professor Define ---------

local ProfessorPutricide
function ProfessorPutricide_Define(Unit, Event)
	ProfessorPutricide = Unit
end
RegisterUnitEvent(36678, 18, "ProfessorPutricide_Define")

-----------------------------------


function Festergut_OnDeath(Unit, Event, player) 
	Unit:SendChatMessage(14, 0, "Da.. Daddy...")
	ProfessorPutricide:RegisterEvent("ProfPutr_dead", 5000, 1)
	player:AdvanceQuestObjective(24547, 0)
	Unit:RemoveEvents()
end

function ProfPutr_dead(Unit, Event)
	Unit:SendChatMessage(14, 0, "Oh, Festergut. You were always my favorite. Next to Rotface. The good news is you left behind so much gas, I can practically taste it!")
	Unit:PlaySoundToSet(17124)
end

RegisterUnitEvent(36626, 1, "Festergut_OnCombat")
RegisterUnitEvent(36626, 2, "Festergut_OnLeaveCombat")
RegisterUnitEvent(36626, 3, "Festergut_OnKilledTarget")
RegisterUnitEvent(36626, 4, "Festergut_OnDeath")