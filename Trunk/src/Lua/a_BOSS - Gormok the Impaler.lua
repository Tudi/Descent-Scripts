function GormoktheImpaler_OnEnterCombat(pUnit, event)
	pUnit:RegisterEvent("GormoktheImpaler_Impale", 10000, 0)
	pUnit:RegisterEvent("GormoktheImpaler_StaggeringStomp", 20000, 0)
	pUnit:RegisterEvent("GormoktheImpaler_RisingAnger", 30000, 5)
end

function GormoktheImpaler_Impale(pUnit, event)
	pUnit:CastSpellOnTarget(67477, pUnit:GetMainTank())
end

function GormoktheImpaler_StaggeringStomp(pUnit, event)
	pUnit:FullCastSpell(67648)
end

function GormoktheImpaler_RisingAnger(pUnit, event)
	local player = pUnit:GetRandomPlayer(4)
	if (player ~= nil) then
		pUnit:CastSpell(66636)
		pUnit:SpawnCreature(34800, player:GetX(), player:GetY(), player:GetZ(), player:GetO(), pUnit:GetFaction(), 0)
	end
end



RegisterUnitEvent(27975, 1, "GormoktheImpaler_OnEnterCombat")


function SnoboldVassal_OnSpawn(pUnit, event)
	local player = pUnit:GetClosestPlayer()
	pUnit:CastSpellOnTarget(66406,player)
	pUnit:SetTauntedBy(player)
	pUnit:RegisterEvent("SnoboldVassal_Batter", 8000, 0)
	pUnit:RegisterEvent("SnoboldVassal_FireBomb", 15000, 0)
	pUnit:RegisterEvent("SnoboldVassal_HeadCrack", 21000, 0)
end

function SnoboldVassal_Batter(pUnit, event)
	pUnit:CastSpellOnTarget(66408, pUnit:GetTantedBy())
end

function SnoboldVassal_FireBomb(pUnit, event)
	local target = pUnit:GetTantedBy()
	pUnit:CastSpellAoF(target:GetX(), target:GetY(), target:GetZ(), 66317)
end

function SnoboldVassal_HeadCrack(pUnit, event)
	pUnit:CastSpellOnTarget(66407, pUnit:GetTantedBy())
end

function SnoboldVassal_OnDied(pUnit, event)
	pUnit:RemoveEvents()
	pUnit:CastSpellOnTarget(66406,pUnit:GetTantedBy())
	pUnit:Despawn(1000,0)
end

RegisterUnitEvent(34800, 18, "SnoboldVassal_OnSpawn")
RegisterUnitEvent(34800, 4, "SnoboldVassal_Died")