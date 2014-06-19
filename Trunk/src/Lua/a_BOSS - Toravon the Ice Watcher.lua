function Toravon_OnEnterCombat(pUnit, event)
	pUnit:CastSpell(71993)
	pUnit:RegisterEvent("Toravon_FreezingGround", 20000, 0)
	pUnit:RegisterEvent("Toravon_FrozenOrb", 32000, 0)
	pUnit:RegisterEvent("Toravon_Whiteout", 38000, 0)
end

function Toravon_FreezingGround(pUnit, event)
	pUnit:CastSpell(72104)
end

function Toravon_FrozenOrb(pUnit, event) 
	pUnit:CastSpell(72093)
	pUnit:CastSpell(72093)
	pUnit:CastSpell(72093)
end

function Toravon_Whiteout(pUnit, event)
	pUnit:FullCastSpell(72096)
end

function Toravon_OnCombatExit(pUnit, event, player)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(38433, 1, "Toravon_OnEnterCombat")
RegisterUnitEvent(38433, 2, "Toravon_OnCombatExit")
RegisterUnitEvent(38433, 4, "Toravon_OnCombatExit")

function FrozenOrb_OnSpawn(pUnit, event)
	pUnit:CastSpell(72081)
	pUnit:RegisterEvent("FrozenOrb_Move", 5000, 0)

end

function FrozenOrb_Move(pUnit, event)
	local target = pUnit:GetRandomPlayer(7)
	pUnit:MoveTo(target:GetX(), target:GetY(), target:GetZ(), 0)
end

function FrozenOrb_OnDeath(pUnit, event)
	pUnit:CastSpell(71292)
end


RegisterUnitEvent(38456, 18, "FrozenOrb_OnSpawn")
RegisterUnitEvent(38456, 4, "FrozenOrb_OnDeath")