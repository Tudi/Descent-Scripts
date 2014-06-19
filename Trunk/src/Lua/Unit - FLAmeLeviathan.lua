---------------------------------------------
--- SCRIPT MADE BY QABOOMBA FOR WOWBEEZ -----
----- ALL CREDITS GO TO QABOOMBA ------------
---------------------------------------------

function FlameLev_OnCombat(Unit, Event)
	Unit:SendChatMessage(14, 0, "Hostile entities detected. Threat assessment protocol active. Primary target engaged. Time minus thirty seconds to re-evaluation.") 
	Unit:PlaySoundToSet(15506)
	Unit:RegisterEvent("FlameLev_SecondMessage", 12000, 1)
	Unit:RegisterEvent("FlameLev_Phase1", 20000, 1)
end

function FlameLev_SecondMessage(Unit, Event)
	Unit:PlaySoundToSet(15516)
	Unit:SendChatMessage(14, 0, "Unauthorized entity attempting circuit overload. Activating anti-personnel countermeasures.")
end

function FlameLev_Phase1(Unit, Event)
	Unit:RemoveEvents()
	Unit:CastSpellOnTarget(62374, Unit:GetRandomPlayer(0))
	Unit:CastSpell(62374)
	Unit:SendChatMessage(14, 0, "Pursuit Objective modified. Changing course.")
	Unit:PlaySoundToSet(15508)
	Unit:RegisterEvent("FlameLev_MissileBarrage", 6000, 0)
	Unit:RegisterEvent("FlameLev_Vents", 73000, 0)
	Unit:RegisterEvent("FlameLev_PSvoice", 34000, 0)
	Unit:RegisterEvent("FlameLev_Pursued", 34000, 0)
	Unit:RegisterEvent("FlameLev_Phase2", 5000, 0)
end

function FlameLev_MissileBarrage(Unit, Event)
	Unit:CastSpellOnTarget(62400, Unit:GetRandomPlayer(0))
end

function FlameLev_PSvoice(Unit, Event)
	Unit:PlaySoundToSet(15508)
	Unit:SendChatMessage(14, 0, "Pursuit Objective modified. Changing course.")
end

function FlameLev_Pursued(Unit, Event)
	Unit:CastSpellOnTarget(62374, Unit:GetRandomPlayer(0))
end

function FlameLev_Vents(Unit, Event)
	Unit:FullCastSpell(62396)
	Unit:PlaySoundToSet(15510)
	Unit:SendChatMessage(14, 0, "Orbital countermeasures enabled.")
end

function FlameLev_Phase2(Unit, Event)
	if Unit:GetHealthPct() <= 80 then
		Unit:RemoveEvents()
		Unit:FullCastSpell(62680)
		Unit:RegisterEvent("FlameLev_VoicePhase2", 1000, 1)
		Unit:RegisterEvent("FlameLev_PSvoice", 35000, 0)
		Unit:RegisterEvent("FlameLev_Pursued", 35000, 0)
		Unit:RegisterEvent("FlameLev_FlameJets", 43000, 0)
		Unit:RegisterEvent("FlameLev_FireNova", 8493, 0)
		Unit:RegisterEvent("FlameLev_Vents", 73000, 0)
		Unit:RegisterEvent("FlameLev_MissileBarrage", 9000, 0)
		Unit:RegisterEvent("FlameLev_Phase3", 1000, 0)
	end
end

function FlameLev_MissileBarrage(Unit, Event)
	Unit:CastSpellOnTarget(62400, Unit:GetRandomPlayer(0))
end

function FlameLev_VoicePhase2(Unit, Event)
	Unit:SendChatMessage(14, 0, "System malfunction. Diverting power to support systems.")
	Unit:PlaySoundToSet(15517)
end

	function FlameLev_PSvoice(Unit, Event)
	Unit:SendChatMessage(14, 0, "Pursuit Objective modified. Changing course.")
	Unit:PlaySoundToSet(15508)
end

function FlameLev_Pursued(Unit, Event)
	Unit:CastSpellOnTarget(62374, Unit:GetRandomPlayer(0))
end

function FlameLev_FlameJets(Unit, Event)
	Unit:FullCastSpell(62680)
end

function FlameLev_Vents(Unit, Event)
	Unit:FullCastSpell(62396)
	Unit:PlaySoundToSet(15510)
	Unit:SendChatMessage(14, 0, "Orbital countermeasures enabled.")
end

function FlameLev_FireNova(Unit, Event)
Unit:FullCastSpell(38728)
end

function FlameLev_Phase3(Unit, Event)
	if Unit:GetHealthPct() <= 15 then
		Unit:RemoveEvents()
		Unit:SendChatMessage(14, 0, "System restart required. Deactivating weapon systems.")
		Unit:RegisterEvent("FlameLev_SystemVoice", 1000, 1)
		Unit:RegisterEvent("FlameLev_Battering", 25517, 10)
		Unit:RegisterEvent("FlameLev_SearingFlame", 30284, 0)
		Unit:RegisterEvent("FlameLev_FlameJets", 62000, 0)
		Unit:RegisterEvent("FlameLev_MissileBarrage", 9000, 0)
	end
end

function FlameLev_MissileBarrage(Unit, Event)
	Unit:CastSpellOnTarget(62400, Unit:GetRandomPlayer(0))
end

function FlameLev_SystemVoice(Unit, Event)
	Unit:PlaySoundToSet(15519)
end

function FlameLev_Battering(Unit, Event)
	Unit:CastSpellOnTarget(62376, Unit:GetRandomPlayer(0))
end

function FlameLev_FlameJets(Unit, Event)
	Unit:FullCastSpell(62680)
end

function FlameLev_SearingFlame(Unit, Event)
	Unit:FullCastSpellOnTarget(62661, Unit:GetRandomPlayer(0))
end

function FlameLev_OnLeaveCombat(Unit, Event)
	Unit:PlaySoundToSet(15518)
	Unit:SendChatMessage(14, 0, "Combat matrix overload. Powering do-o-own...")
	Unit:RemoveEvents()
end

function FlameLev_OnKilledTarget(Unit, Event)
	Unit:PlaySoundToSet(15521)
	Unit:SendChatMessage(14, 0, "Threat assessment routine modified. Current target threat level: zero. Acquiring new target.")
end

function FlameLev_OnDeath(Unit, Event)
	Unit:PlaySoundToSet(15520)
	Unit:SendChatMessage(14, 0, "Total systems failure. Defense protocols breached. Leviathan Unit shutting down.")
	Unit:RemoveEvents()
end 

RegisterUnitEvent (33113, 1, "FlameLev_OnCombat")
RegisterUnitEvent (33113, 2, "FlameLev_OnLeaveCombat")
RegisterUnitEvent (33113, 3, "FlameLev_OnKilledTarget")
RegisterUnitEvent (33113, 4, "FlameLev_OnDeath")

---------------------------------------------
--- SCRIPT MADE BY QABOOMBA FOR WOWBEEZ -----
----- ALL CREDITS GO TO QABOOMBA ------------
---------------------------------------------