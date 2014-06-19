function Iceshalanda_OnCombat(Unit, Event)
	Unit:SendChatMessage(14, 0, "I've been waiting for you all!")
	Unit:PlaySoundToSet(11466)
	Unit:RegisterEvent("Iceshalanda_FabulousFlames", 3000, 0)
	Unit:RegisterEvent("Iceshalanda_DevouringFlames", 20000, 0)
	Unit:RegisterEvent("Iceshalanda_Shockwave", 45000, 0)
	Unit:RegisterEvent("Iceshalanda_ShatteredIce", 25000, 0)
	Unit:RegisterEvent("Iceshalanda_WindBurst", 40000, 0)
	Unit:RegisterEvent("Iceshalanda_Stomp", 35000, 0)
end

function Iceshalanda_WindBurst(Unit, Event)
	Unit:FullCastSpell(93261)
	Unit:SendChatMessage(14, 0, "Gone like the wind!")
end

function Iceshalanda_FabulousFlames(Unit, Event)
	Unit:FullCastSpell(92908)
end

function Iceshalanda_Stomp(Unit, Event)
	Unit:FullCastSpell(109033)
	Unit:SendChatMessage(14, 0, "Feel my hatred!")
	Unit:ClearThreatList() 
end

function Iceshalanda_DevouringFlames(Unit, Event)
	Unit:FullCastSpell(86840)
	Unit:SendChatMessage(14, 0, "Boom!")
end

function Iceshalanda_Shockwave(Unit, Event)
	Unit:FullCastSpell(108046)
	Unit:SendChatMessage(14, 0, "Stunned!")
end

function Iceshalanda_ShatteredIce(Unit, Event)
	Unit:CastSpellOnTarget(110888, Unit:GetMainTank())
	Unit:SendChatMessage(14, 0, "Feel the ice flowing through your veins!")
end

function Iceshalanda_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Iceshalanda_OnKilledTarget(Unit, Event)
	local kill = math.random(1,2)
	if (kill == 1) then
		Unit:SendChatMessage(14, 0, "Hahahaha, dead!")
		Unit:PlaySoundToSet(16902)
	elseif (kill == 2) then
		Unit:SendChatMessage(14, 0, "I see that somebody can't stand the cold!")
		Unit:PlaySoundToSet(16903)
	end
end

function Iceshalanda_OnDeath(Unit, Event)
	Unit:SendChatMessage(14, 0, "Noooooooooooo, The Frozen Legion will go on, there are more forsaken powers waiting for my death!")
	Unit:PlaySoundToSet(11964)
	Unit:RemoveEvents()
end



RegisterUnitEvent(270002, 1, "Iceshalanda_OnCombat")
RegisterUnitEvent(270002, 2, "Iceshalanda_OnLeaveCombat")
RegisterUnitEvent(270002, 3, "Iceshalanda_OnKilledTarget")
RegisterUnitEvent(270002, 4, "Iceshalanda_OnDeath")