function Icehowl_OnEnterCombat(pUnit, event)
	pUnit:RegisterEvent("Icehowl_ArcticBreath", 15000, 0)
	pUnit:RegisterEvent("Icehowl_Whirl", 21000, 0)
	pUnit:RegisterEvent("Icehowl_FrothingRage", 17000, 0)
	pUnit:RegisterEvent("Icehowl_MassiveCrash", 55000, 0)
end

function Icehowl_ArcticBreath(pUnit, event)
	pUnit:FullCastSpell(67650)
end

function Icehowl_Whirl(pUnit, event)
	pUnit:CastSpell(67664)
end

function Icehowl_FrothingRage(pUnit, event)
	pUnit:CastSpell(66759)
end

function Icehowl_MassiveCrash(pUnit, event)
	pUnit:FullCastSpell(67661)
	pUnit:Root()
	pUnit:RegisterEvent("Icehowl_Charge", 10000, 1)
end

function Icehowl_Charge(pUnit, event)
	local PlayersALllAround = pUnit:GetInRangePlayers()
	local count = pUnit:GetInRangePlayersCount()
	for i = 1, count do
		PlayersALllAround[i]:CastSpell(68667)
	end
	pUnit:Unroot()
	local selectedPlayer = pUnit:GetRandomPlayer(0)
	pUnit:SetMoveRunFlag(1)
	pUnit:MoveTo(selectedPlayer:GetX(),selectedPlayer:GetY(),selectedPlayer:GetZ())
	pUnit:RegisterEvent("Icehowl_ReachedTarget", 8000, 1)
end

function Icehowl_ReachedTarget(pUnit, event)
	pUnit:CastSpell(66734)
	pUnit:CastSpell(66758)
end

RegisterUnitEvent(34797, 1, "Icehowl_OnEnterCombat")