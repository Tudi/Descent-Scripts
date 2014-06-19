local Bronjahm
function Bronjahm_OnEnterCombat(pUnit, event)
	Bronjahm = pUnit
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "More souls to power the engine!")
		pUnit:PlaySoundToSet(16595)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Finally...a captive audience!")
		pUnit:PlaySoundToSet(16595)
	end
	pUnit:RegisterEvent("Bronjahm_MagicsBane", 12000, 0)
	pUnit:RegisterEvent("Bronjahm_ShadowBolt", 5000, 0)
	pUnit:RegisterEvent("Bronjahm_CorruptSoul", 15000, 0)
	pUnit:RegisterEvent("Bronjahm_SoulStormCheck", 1000, 1)
end

function Bronjahm_SoulStormCheck(pUnit, event)
	if pUnit:GetHealthPct() <= 30 then
		pUnit:FullCastSpell(68988)
		pUnit:RegisterEvent("Bronjahm_SoulStorm", 1000, 1)
	else
		pUnit:RegisterEvent("Bronjahm_SoulStormCheck", 1000, 1)
	end
end

function Bronjahm_SoulStorm(pUnit, event)
	pUnit:SendChatMessage(14, 0, "The vortex of the harvested calls to you!")
	pUnit:Root()
	pUnit:PlaySoundToSet(16599)
	pUnit:FullCastSpell(68872)
	pUnit:RegisterEvent("Bronjahm_Fear", 9000, 0)
end

function Bronjahm_MagicsBane(pUnit, event)
	pUnit:CastSpell(68793)
end

function Bronjahm_ShadowBolt(pUnit, event)
	pUnit:FullCastSpellOnTarget(70043,pUnit:GetMainTank())
end

function Bronjahm_CorruptSoul(pUnit, event)
	local player = pUnit:GetRandomPlayer(7)
	if(player ~= nil) then
		pUnit:PlaySoundToSet(16600)
		pUnit:SendChatMessage(14, 0, "I will sever the soul from your body!")
		pUnit:FullCastSpellOnTarget(68839,player)
	end
end

function Bronjahm_Slay(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Another soul to strengthen the host!")
		pUnit:PlaySoundToSet(16596)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Fodder for the engine!")
		pUnit:PlaySoundToSet(16597)
	end
end

function Bronjahm_Death(pUnit, event)
	pUnit:SendChatMessage(14, 0, "My soul for you, master.")
	pUnit:PlaySoundToSet(16598)
	pUnit:RemoveEvents()
end

function Bronjahm_OnCombatExit(pUnit, event)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(36497, 1, "Bronjahm_OnEnterCombat")
RegisterUnitEvent(36497, 2, "Bronjahm_OnCombatExit")
RegisterUnitEvent(36497, 3, "Bronjahm_Slay")
RegisterUnitEvent(36497, 4, "Bronjahm_Death")

function CorruptedSoulFragment_OnSpawn(pUnit, event)
	pUnit:RegisterEvent("CorruptedSoulFragment_MoveTo", 1000, 0)
end

function CorruptedSoulFragment_MoveTo(pUnit, event)
	local x = Bronjahm:GetX()
	local y = Bronjahm:GetY()
	local z = Bronjahm:GetZ()
	pUnit:MoveTo(x,y,z,0)
	if ( (abs(pUnit:GetX() - x)) < 5 and (abs(pUnit:GetY() - y) < 5)) then
		pUnit:RemoveEvents()
		Bronjahm:CastSpell(68858)
		pUnit:Despawn(500,0)
	end
end

RegisterUnitEvent(36535, 18, "CorruptedSoulFragment_OnSpawn")