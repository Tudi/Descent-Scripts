function Acidmaw_OnEnterCombat(pUnit, event)
	pUnit:RegisterEvent("Acidmaw_Stationary", 1000, 1)
end

function Acidmaw_OnMobile(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:Unroot()
	pUnit:RegisterEvent("Acidmaw_AcidicSpew", 21000, 0)
	pUnit:RegisterEvent("Acidmaw_ParalyticBite", 25000, 0)
	pUnit:RegisterEvent("Acidmaw_SlimePool", 12000, 0)
	pUnit:RegisterEvent("Acidmaw_ChangeToStationary", 60000, 1)
end

function Acidmaw_Stationary(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:Root()
	pUnit:RegisterEvent("Acidmaw_AcidSpit", 15000, 0)
	pUnit:RegisterEvent("Acidmaw_ParalyticSpray", 21000, 0)
	pUnit:RegisterEvent("Acidmaw_Sweep", 17000, 0)
	pUnit:RegisterEvent("Acidmaw_ChangeToMobile", 60000, 1)
end

function Acidmaw_ChangeToMobile(pUnit, event)
	pUnit:RegisterEvent("Acidmaw_OnMobile", 1000, 1)
end

function Acidmaw_ChangeToStationary(pUnit, event)
	pUnit:RegisterEvent("Acidmaw_Stationary", 1000, 1)
end

function Acidmaw_AcidicSpew(pUnit, event)
	pUnit:CastSpell(67610)
end

function Acidmaw_ParalyticBite(pUnit, event)
	pUnit:CastSpellOnTarget(67612,pUnit:GetMainTank())
end

function Acidmaw_SlimePool(pUnit, event)
	pUnit:CastSpell(66883)
end

function Acidmaw_AcidSpit(pUnit, event)
	local target = pUnit:GetRandomPlayer(0)
	if (target ~= nil) then
		pUnit:FullCastSpellOnTarget(66880,target)
	end
end

function Acidmaw_ParalyticSpray(pUnit, event)
	pUnit:FullCastSpell(66901)
end

function Acidmaw_Sweep(pUnit, event)
	pUnit:FullCastSpell(66794)
end

RegisterUnitEvent(35144, 1, "Acidmaw_OnEnterCombat")

function Dreadscale_OnEnterCombat(pUnit, event)
	pUnit:RegisterEvent("Dreadscale_OnMobile", 1000, 1)
end

function Dreadscale_OnMobile(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:Unroot()
	pUnit:RegisterEvent("Dreadscale_BurningBite", 15000, 0)
	pUnit:RegisterEvent("Dreadscale_MoltenSpew", 21000, 0)
	pUnit:RegisterEvent("Dreadscale_SlimePool", 12000, 0)
	pUnit:RegisterEvent("Dreadscale_ChangeToStationary", 60000, 1)
end

function Dreadscale_Stationary(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:Root()
	pUnit:RegisterEvent("Dreadscale_FireSpit", 15000, 0)
	pUnit:RegisterEvent("Dreadscale_BurningSpray", 21000, 0)
	pUnit:RegisterEvent("Dreadscale_Sweep", 17000, 0)
	pUnit:RegisterEvent("Dreadscale_ChangeToMobile", 60000, 1)
end

function Dreadscale_ChangeToMobile(pUnit, event)
	pUnit:RegisterEvent("Dreadscale_OnMobile", 1000, 1)
end

function Dreadscale_ChangeToStationary(pUnit, event)
	pUnit:RegisterEvent("Dreadscale_Stationary", 1000, 1)
end

function Dreadscale_BurningBite(pUnit, event)
	pUnit:CastSpellOnTarget(66879, pUnit:GetMainTank())
end

function Dreadscale_MoltenSpew(pUnit, event)
	pUnit:CastSpell(66821)
end

function Dreadscale_SlimePool(pUnit, event)
	pUnit:CastSpell(66883)
end

function Dreadscale_FireSpit(pUnit, event)
	local target = pUnit:GetRandomPlayer(0)
	if (target ~= nil) then
		pUnit:FullCastSpellOnTarget(66796,target)
	end
end

function Dreadscale_BurningSpray(pUnit, event)
	pUnit:FullCastSpell(66902)
end

function Dreadscale_Sweep(pUnit, event)
	pUnit:FullCastSpell(66794)
end

RegisterUnitEvent(34799, 1, "Dreadscale_OnEnterCombat")

function SlimePool_onSpawn(pUnit, event)
	pUnit:Root()
	pUnit:Despawn(30000,0)
end

RegisterUnitEvent(35176, 18, "SlimePool_onSpawn")