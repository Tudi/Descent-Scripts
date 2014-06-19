
--[[
-- Alliance
73762
73824
73825
73826
73827
73828

-- Horde
73816
73818
73819
73820
73821
73822

Zone's do not handle it, areatriggers might but that's a nightmare to use.
Instead we will handle it via on boss deaths and this.

EDIT: Aparently from reading wiki the buffs are the same throughout the raid - they got improved over time by blizzard making the raids easier.
Just apply the buff you want, I will start with the most recent and powerful since that is currently what blizzard are using. Change it to the value you want.
]]

function OnZone(event, player)
	if player:GetMapId() == 631 then -- ICC --
		SetDBCSpellVar(69127, "c_is_flags", 0x01000) -- Aparently the spells are not debuffing so we'll do it manually and automatically at the same time
		player:CastSpell(69127)
		player:CastSpell(69127) -- Must be cast two times to apply debuff
		local race = player:GetPlayerRace()
		if race == 1 or race == 3 or race == 4 or race == 7 or race == 11 then -- Alliance --
		player:CastSpell(73828)
		player:CastSpell(73828) -- Apply two times for debuff?
		else -- Horde --
		player:CastSpell(73822)
		player:CastSpell(73822) -- Apply two times for debuff?
		end
	else -- Were no longer in ICC --
		-- Removing buffs should not cause a crash if the player doesn't have them as this is checked in the core
		-- Misc --
		player:RemoveAura(69127)
		-- Allaince --
		--player:RemoveAura(73762)
		--player:RemoveAura(73762)
		--player:RemoveAura(73824)
		--player:RemoveAura(73825)
		--player:RemoveAura(73826)
		--player:RemoveAura(73827)
		player:RemoveAura(73828)
		-- Horde --
		--player:RemoveAura(73816)
		--player:RemoveAura(73818)
		--player:RemoveAura(73819)
		--player:RemoveAura(73820)
		--player:RemoveAura(73821)
		player:RemoveAura(73822)
	end
end

RegisterServerHook(4, "OnZone")
