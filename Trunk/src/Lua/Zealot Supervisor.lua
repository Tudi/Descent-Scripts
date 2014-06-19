function Zealot_OnCombat(Unit, Event)
	Unit:RegisterEvent("Zealot_ArcaneExplosion", 6000, 0)
	Unit:RegisterEvent("Zealot_FrostBolt", 2800, 0)
end

function Zealot_ArcaneExplosion(Unit, Event)
	Unit:FullCastSpell(91623)
end

function Zealot_FrostBolt(Unit, Event)
	local rand = Unit:GetRandomPlayer(0)
	if(rand ~= nil) then
	Unit:FullCastSpellOnTarget(109334, rand)
	end
end

function Zealot_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
	Unit:Despawn(4000, 0)
end

function Zealot_OnKilledTarget(Unit, Event)
end

function Zealot_OnDeath(Unit, Event)
	Unit:FullCastSpell(91623)
	Unit:RemoveEvents()
	Unit:Despawn(6000, 0)
end

RegisterUnitEvent(111020, 1, "Zealot_OnCombat")
RegisterUnitEvent(111020, 2, "Zealot_OnLeaveCombat")
RegisterUnitEvent(111020, 3, "Zealot_OnKilledTarget")
RegisterUnitEvent(111020, 4, "Zealot_OnDeath")