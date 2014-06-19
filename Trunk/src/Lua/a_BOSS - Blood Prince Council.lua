function PrinceKeleseth_OnEnterCombat(pUnit, event)
	pUnit:RegisterEvent("PrinceKeleseth_ShadowResonance", 10500, 0)
	pUnit:RegisterEvent("PrinceKeleseth_ShadowLance", 3000, 0)
	pUnit:RegisterEvent("PrinceKeleseth_InvocationofBlood", 120000, 0)
end

function PrinceKeleseth_InvocationofBlood(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Such wondrous power! The Darkfallen Orb has made me INVINCIBLE!")
	pUnit:PlaySoundToSet(16727)
	pUnit:CastSpell(70981)
end

function PrinceKeleseth_ShadowResonance(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Blood will flow!")
	pUnit:PlaySoundToSet(16728)
	pUnit:CastSpell(71943)
end

function PrinceKeleseth_ShadowLance(pUnit, event)
	if pUnit:HasAura(70981) == true then
		pUnit:FullCastSpellOnTarget(72810,pUnit:GetMainTank())
		pUnit:RemoveAura(70981) 
	else
		pUnit:FullCastSpellOnTarget(72805,pUnit:GetMainTank())
	end
end

function PrinceKeleseth_Slay(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Were you ever a threat?")
		pUnit:PlaySoundToSet(16723)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Truth is found in death.")
		pUnit:PlaySoundToSet(16724)
	end
end

function PrinceKeleseth_Death(pUnit, event, player, Misc)
	pUnit:SendChatMessage(14, 0, "My queen... they come...")
	pUnit:PlaySoundToSet(16725)
	pUnit:RemoveAura(70981)
	player:AdvanceQuestObjective(24547, 0)
	pUnit:RemoveEvents()
end

function PrinceKeleseth_OnCombatExit(pUnit, event)
	pUnit:RemoveAura(70981) 
	pUnit:RemoveEvents()
end

function PrinceKeleseth_Spawn(pUnit, event)
	pUnit:SetCombatMeleeCapable(0)
end

function PrinceTaldaram_OnEnterCombat(pUnit, event)
	pUnit:RegisterEvent("PrinceTaldaram_GlitteringSparks", 10000, 0)
	pUnit:RegisterEvent("PrinceTaldaram_ConjureFlame", 20000, 0)
	pUnit:RegisterEvent("PrinceTaldaram_InvocationofBlood", 80000, 0)
end

function PrinceTaldaram_InvocationofBlood(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Tremble before Taldaram, mortals, for the power of the orb flows through me!")
	pUnit:PlaySoundToSet(16857)
	pUnit:CastSpell(70982)
end

function PrinceTaldaram_GlitteringSparks(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Delight in the pain!")
	pUnit:PlaySoundToSet(16858)
	pUnit:CastSpell(71807)
end

function PrinceTaldaram_ConjureFlame(pUnit, event)
	if pUnit:HasAura(70982) == true then
		pUnit:FullCastSpell(72040)
		pUnit:RemoveAura(70982)
	else
		pUnit:FullCastSpell(71719)
	end
end

function PrinceTaldaram_Slay(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Worm food.")
		pUnit:PlaySoundToSet(16853)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Beg for mercy!")
		pUnit:PlaySoundToSet(16854)
	end
end

function PrinceTaldaram_Death(pUnit, event, player, Misc)
	pUnit:SendChatMessage(9, 0, "Prince Taldaram gurgles and dies.")
	pUnit:PlaySoundToSet(16855)
	pUnit:RemoveAura(70982)
	player:AdvanceQuestObjective(24547, 0)
	pUnit:RemoveEvents()
end

function PrinceTaldaram_OnCombatExit(pUnit, event)
	pUnit:RemoveAura(70982)
	pUnit:RemoveEvents()
end

function PrinceValanar_OnEnterCombat(pUnit, event)
	pUnit:RegisterEvent("PrinceValanar_ShockVortex", 16500, 0)
	pUnit:RegisterEvent("PrinceValanar_InvocationofBlood", 40000, 0)
end

function PrinceValanar_InvocationofBlood(pUnit, event)
	pUnit:SendChatMessage(14, 0, "Naxxanar was merely a setback! With the power of the orb, Valanar will have his vengeance!")
	pUnit:PlaySoundToSet(16685)
	pUnit:CastSpell(70952)
end

function PrinceValanar_ShockVortex(pUnit, event)
	pUnit:SendChatMessage(14, 0, "My cup runneth over.")
	pUnit:PlaySoundToSet(16686)
	if pUnit:HasAura(70952) == true then
		pUnit:CastSpellOnTarget(72816,pUnit:GetInRangePlayers())
		pUnit:RemoveAura(70952)
	else
		pUnit:CastSpellOnTarget(72813,pUnit:GetRandomPlayer(7))
	end
end


function PrinceValanar_Slay(pUnit, event)
	local rand = math.random(1,2)
	if rand == 1 then
		pUnit:SendChatMessage(14, 0, "Dinner... is served.")
		pUnit:PlaySoundToSet(16681)
	elseif rand == 2 then
		pUnit:SendChatMessage(14, 0, "Do you see NOW the power of the Darkfallen?")
		pUnit:PlaySoundToSet(16682)
	end
end

function PrinceValanar_Death(pUnit, event, player, Misc)
	pUnit:SendChatMessage(14, 0, "...why...?")
	pUnit:PlaySoundToSet(16683)
	pUnit:RemoveAura(70952)
	player:AdvanceQuestObjective(24547, 0)
	pUnit:RemoveEvents()
end

function PrinceValanar_OnCombatExit(pUnit, event)
	pUnit:RemoveAura(70952)
	pUnit:RemoveEvents()
end

RegisterUnitEvent(37972, 1, "PrinceKeleseth_OnEnterCombat")
RegisterUnitEvent(37972, 2, "PrinceKeleseth_OnCombatExit")
RegisterUnitEvent(37972, 3, "PrinceKeleseth_Slay")
RegisterUnitEvent(37972, 4, "PrinceKeleseth_Death")
RegisterUnitEvent(37972, 18, "PrinceKeleseth_Spawn")
RegisterUnitEvent(37973, 1, "PrinceTaldaram_OnEnterCombat")
RegisterUnitEvent(37973, 2, "PrinceTaldaram_OnCombatExit")
RegisterUnitEvent(37973, 3, "PrinceTaldaram_Slay")
RegisterUnitEvent(37973, 4, "PrinceTaldaram_Death")
RegisterUnitEvent(37970, 1, "PrinceValanar_OnEnterCombat")
RegisterUnitEvent(37970, 2, "PrinceValanar_OnCombatExit")
RegisterUnitEvent(37970, 3, "PrinceValanar_Slay")
RegisterUnitEvent(37970, 4, "PrinceValanar_Death")

function BallofFlame_Spawn(pUnit, event)
	local player = pUnit:GetRandomPlayer(0)
	pUnit:GetUnitToFollow(player)
	pUnit:Despawn(30000,0)
end

function BallofFlame_OnReach(pUnit, event)
	pUnit:CastSpell(71393)
end

RegisterUnitEvent(38332, 18, "BallofFlame_Spawn")
RegisterUnitEvent(38332, 19, "BallofFlame_OnReach")

function BallofInfernoFlame_Spawn(pUnit, event)
	local player = pUnit:GetRandomPlayer(0)
	pUnit:CreateWaypoint(player:GetX(), player:GetY(), player:GetZ(), 0, 0, 256, 0)
	pUnit:RegisterEvent("BallofInfernoFlame_Flare", 1000, 0)
end

function BallofInfernoFlame_Flare(pUnit, event)
	pUnit:CastSpell(72786)
end

function BallofInfernoFlame_OnReach(pUnit, event)
	pUnit:CastSpell(71393)
	pUnit:Despawn(1000,0)
end

RegisterUnitEvent(38451, 18, "BallofInfernoFlame_Spawn")
RegisterUnitEvent(38451, 19, "BallofInfernoFlame_OnReach")

function DarkNucleus_Spawn(pUnit, event)
	pUnit:Despawn(25000,0)
	pUnit:CreateWaypoint(4629, 2768, 361.165, 0, 0, 256, 0)
	pUnit:RegisterEvent("DarkNucleus_ShadowResonance", 1000, 0)
end

function DarkNucleus_ShadowResonance(pUnit, event)
	pUnit:CastSpellOnTarget(71822,pUnit:GetClosestPlayer())
end

RegisterUnitEvent(38369, 18, "DarkNucleus_Spawn")