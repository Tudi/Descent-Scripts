local Feugen
function Feugen_Define(Unit, Event)
Feugen = Unit
end
RegisterUnitEvent(15930, 18, "Feugen_Define")

local Stalagg
function Stalagg_Define(Unit, Event)
Stalagg = Unit
end
RegisterUnitEvent(15929, 18, "Stalagg_Define")
------------------------


function Stalagg_OnCombat(Unit, Event)
Stalagg:RegisterEvent("Check", 5000, 1)
Stalagg:SendChatMessage(14, 0, "Stalagg crush you!")
Stalagg:PlaySoundToSet(8864)
Stalagg:RegisterEvent("Stalagg_Surge", 20000, 0)
Stalagg:RegisterEvent("MagneticPull", 25000, 0) 
end 

function Check(Unit, Event)
if Feugen:IsInCombat() == false then
local playerone = Feugen:GetClosestPlayer()
Feugen:MoveTo(playerone:GetX(), playerone:GetY(), playerone:GetZ(), playerone:GetO())
end
end

function Stalagg_Surge(Unit, Event)
Stalagg:CastSpell(28134)
Stalagg:SendChatMessage(42, 0, "Stalagg gains Power Surge!")
end

function MagneticPull(Unit, Event)
	local TankOne = Feugen:GetMainTank()
	Stalagg:CastSpellOnTarget(28338, TankOne)
	Stalagg:SendChatMessage(42, 0, "Tank-Throw!")
	Stalagg:ClearThreatList()
	Stalagg:GetClosestPlayer()

	local TankTwo = Stalagg:GetMainTank()
	Feugen:CastSpellOnTarget(28338, TankTwo)
	Feugen:SendChatMessage(42, 0, "Tank-Throw!")
	Feugen:ClearThreatList()
	Feugen:GetClosestPlayer()
	end

function Stalagg_OnLeaveCombat(Unit, Event)
Stalagg:RemoveEvents()
end

function Stalagg_OnKilledTarget(Unit, Event)
Stalagg:SendChatMessage(14, 0, "Stalagg kill!")
Stalagg:PlaySoundToSet(8866)
end

function Stalagg_OnDeath(Unit, Event)
Stalagg:SendChatMessage(42, 0, "Stalagg is dead!")
Stalagg:SendChatMessage(14, 0, "Master, save me!")
Stalagg:PlaySoundToSet(8865)
Stalagg:RegisterEvent("Stalagg_CHeckFeugen", 7000, 1)

function Stalagg_CheckFeugen(Unit, Event)
	Feugen:FullCastSpell(45855)
	Stalagg:FullCastSpell(45855)
	Feugen:RemoveEvents()
end
end



RegisterUnitEvent(15929, 1, "Stalagg_OnCombat")
RegisterUnitEvent(15929, 2, "Stalagg_OnLeaveCombat")
RegisterUnitEvent(15929, 3, "Stalagg_OnKilledTarget")
RegisterUnitEvent(15929, 4, "Stalagg_OnDeath")

--------------------------
function Feugen_OnCombat(Unit, Event)
Feugen:SendChatMessage(14, 0, "Feed you to master!")
Feugen:PlaySoundToSet(8802)
Feugen:RegisterEvent("Feugen_StaticField", 6000, 0)
Feugen:RegisterEvent("Checkk", 5000, 2)
end


function Checkk(Unit, Event)
if Stalagg:IsInCombat() == false then
local playerone = Stalagg:GetClosestPlayer()
Stalagg:MoveTo(playerone:GetX(), playerone:GetY(), playerone:GetZ(), playerone:GetO())
end
end

function Feugen_StaticField(Unit, Event)
Feugen:FullCastSpell(54528)
end

function Feugen_OnLeaveCombat(Unit, Event)
Feugen:RemoveEvents()
end

function Feugen_OnKilledTarget(Unit, Event)
Feugen:SendChatMessage(14, 0, "Feugen make master happy!")
Feugen:PlaySoundToSet(8804)
end

function Feugen_OnDeath(Unit, Event)
Feugen:SendChatMessage(42, 0, "Feugen is dead!")
Feugen:SendChatMessage(14, 0, "No... more... Feugen!")
Feugen:PlaySoundToSet(8803)
Feugen:RegisterEvent("Feugen_CHeckFeugen", 7000, 1)

function Feugen_CheckFeugen(Unit, Event)
	Feugen:FullCastSpell(45855)
	Stalagg:FullCastSpell(45855)
	Feugen:RemoveEvents()
end
end

RegisterUnitEvent(15930, 1, "Feugen_OnCombat")
RegisterUnitEvent(15930, 2, "Feugen_OnLeaveCombat")
RegisterUnitEvent(15930, 3, "Feugen_OnKilledTarget")
RegisterUnitEvent(15930, 4, "Feugen_OnDeath")