function Mistress_OnCombat(Unit, Event)
	Unit:SendChatMessage(14, 0, "Nobody shall cross me!")
	Unit:PlaySoundToSet(16901)
	Unit:RegisterEvent("Mistress_GasSpore", 30000, 0)
	Unit:RegisterEvent("Mistress_ShadowPrison", 120000, 0)
	Unit:RegisterEvent("Mistress_GastricBloat", 10000, 0)
	Unit:RegisterEvent("Mistress_ShadowNova", 20000, 0)
end

function Mistress_GasSpore(Unit, Event)
	local playerCount = Unit:GetInRangePlayersCount()
	local player1 = Unit:GetRandomPlayer(7)
	local player2 = Unit:GetRandomPlayer(7)
	while ((player1 == player2) and player1 ~= nil and playerCount > 2) do
		player2 = Unit:GetRandomPlayer(7)
	end
	if player1 ~= nil then
		Unit:SendChatMessage(42, 0, "Mistress Faerlina casts Gas Spore on "..player1:GetName().."!")
		Unit:CastSpellOnTarget(71221, player1)
	end
	if player2 ~= nil then
		Unit:SendChatMessage(42, 0, "Mistress Faerlina casts Gas Spore on "..player2:GetName().."!")
		Unit:CastSpellOnTarget(71221, player2)
	end
	Unit:PlaySoundToSet(16911)
end


function Mistress_ShadowPrison(Unit, Event)
	Unit:FullCastSpell(40647)
	Unit:SendChatMessage(42, 0, "Hahahaha, you're all in my hands now!")
	Unit:ClearThreatList() 
end

function Mistress_GastricBloat(Unit, Event)
	Unit:CastSpellOnTarget(72219, Unit:GetMainTank())
	Unit:SendChatMessage(42, 0, "Mistress Faerlina casts Gastric Bloat. Avoid 10 stacks!")
end

function Mistress_ShadowNova(Unit, Event)
	Unit:FullCastSpell(86167)
end

function Mistress_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Mistress_OnKilledTarget(Unit, Event)
	local kill = math.random(1,2)
	if (kill == 1) then
		Unit:SendChatMessage(14, 0, "Woops, seems like I killed someone!")
		Unit:PlaySoundToSet(16902)
	elseif (kill == 2) then
		Unit:SendChatMessage(14, 0, "Dead, oh, so dead!")
	end
end

function Mistress_OnDeath(Unit, Event)
	Unit:SendChatMessage(14, 0, "You are all.. going.. to die.. nobody survived life yet!")
	Unit:RemoveEvents()
end



RegisterUnitEvent(270000, 1, "Mistress_OnCombat")
RegisterUnitEvent(270000, 2, "Mistress_OnLeaveCombat")
RegisterUnitEvent(270000, 3, "Mistress_OnKilledTarget")
RegisterUnitEvent(270000, 4, "Mistress_OnDeath")