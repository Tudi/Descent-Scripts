---------------------------------------------
--- SCRIPT MADE BY QABOOMBA FOR WOWBEEZ -----
----- ALL CREDITS GO TO QABOOMBA ------------
---------------------------------------------

function Sartharion_OnCombat(Unit, Event)
	Unit:SendChatMessage(14, 0, "It is my charge to watch over these eggs. I will see you burn before any harm comes to them!")
	Unit:PlaySoundToSet(14093)
	Unit:RegisterEvent("Sartharion_Bomb", 2049, 0)
	Unit:RegisterEvent("Sartharion_Divebomb", 29039, 0)
	Unit:RegisterEvent("Sartharion_FunBomb", 33239, 0)
	Unit:RegisterEvent("Sartharion_Cleave", 17035, 0)
	Unit:RegisterEvent("Sartharion_FlameBreath", 37023, 0)
	Unit:RegisterEvent("Sartharion_Meteor", 124139, 0)
	Unit:RegisterEvent("Sartharion_Berserk", 420329, 1)
end

function Sartharion_Bomb(Unit, Event)
	local target = Unit:GetRandomPlayer(0)
	if (target ~= nil) then
		Unit:CastSpellOnTarget(55362, target)
		Unit:CastSpellOnTarget(20474, target)
	end
end

function Sartharion_Divebomb(Unit, Event)
	local target = Unit:GetRandomPlayer(7)
	if (target ~= nil) then
		local chance = math.random (1,2)
		if (chance == 1) then
			Unit:SendChatMessage(14, 0, "Such flammable little insects...")
			Unit:PlaySoundToSet(14099)
		elseif (chance == 2) then
			Unit:SendChatMessage(14, 0, "Your charred bones will litter the floor!")
			Unit:PlaySoundToSet(14100)
		end
		Unit:CastSpellOnTarget(35181, target)
	end
end


function Sartharion_FunBomb(Unit, Event)
	local target = Unit:GetRandomPlayer(7)
	if target ~= nil then
		Unit:SendChatMessage(14, 0, "How much heat can you take?")
		Unit:PlaySoundToSet(14101)
		Unit:CastSpellOnTarget(20547, target)
	end
end

function Sartharion_Cleave(Unit, Event)
	Unit:CastSpell(56909,Unit:GetMainTank())
end

function Sartharion_FlameBreath(Unit, Event)
	Unit:PlaySoundToSet(14098)
	Unit:SendChatMessage(14, 0, "Burn, you miserable wretches!")
	Unit:FullCastSpell(56908)
end

function Sartharion_Meteor(Unit, Event)
	Unit:SendChatMessage(14, 0, "All will be reduced to ash!")
	Unit:PlaySoundToSet(14102)
	local target = Unit:GetRandomPlayer(0)
	if (target ~= nil) then
		Unit:CastSpellAoF(target:GetX(), target:GetY(), target:GetZ(), 31340)
	end
end

function Sartharion_Berserk(Unit, Event)
	Unit:PlaySoundToSet(61254)
	Unit:SendChatMessage(14, 0, "This pathetic siege ends NOW!")
	Unit:CastSpell(61254)
end

function Sartharion_OnLeaveCombat(Unit, Event)
	Unit:RemoveEvents()
end

function Sartharion_OnKilledTarget(Unit, Event)
	local chance = math.random (1,3)
	if (chance == 1) then
		Unit:SendChatMessage(14, 0, "You will make a fine meal for the hatchlings.")
		Unit:PlaySoundToSet(14094)
	elseif (chance == 2) then
		Unit:SendChatMessage(14, 0, "You are at a grave disadvantage!")
		Unit:PlaySoundToSet(14096)
	elseif (chance == 3) then
		Unit:SendChatMessage(14, 0, "This is why we call you lesser beings.")
		Unit:PlaySoundToSet(14097)
	end
end

function Sartharion_OnDeath(Unit, Event)
	Unit:SendChatMessage(14, 0, "Such is the price... of failure...")
	Unit:PlaySoundToSet(14107)
	Unit:RemoveEvents()
end

RegisterUnitEvent(28860, 1, "Sartharion_OnCombat")
RegisterUnitEvent(28860, 2, "Sartharion_OnLeaveCombat")
RegisterUnitEvent(28860, 3, "Sartharion_OnKilledTarget")
RegisterUnitEvent(28860, 4, "Sartharion_OnDeath")

---------------------------------------------
--- SCRIPT MADE BY QABOOMBA FOR WOWBEEZ -----
----- ALL CREDITS GO TO QABOOMBA ------------
---------------------------------------------
