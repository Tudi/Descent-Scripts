function DarkRuneWarbringer_OnSpawn(pUnit, event)
	pUnit:CastSpell(62320)
end

function Snaplasher_OnSpawn(pUnit, event)
	pUnit:CastSpell(62664)
end

function LightningChargedIronDwarf_OnSpawn(pUnit, event)
	pUnit:CastSpell(64889)
end

RegisterUnitEvent(32877, 18, "DarkRuneWarbringer_OnSpawn")
RegisterUnitEvent(32916, 18, "Snaplasher_OnSpawn")
RegisterUnitEvent(34199, 18, "LightningChargedIronDwarf_OnSpawn")