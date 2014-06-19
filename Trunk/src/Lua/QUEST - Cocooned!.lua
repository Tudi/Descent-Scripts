function NerubianCocoon_OnDied(pUnit, event, player, Misc)
	if(math.random(5) == 2) then
		player:AdvanceQuestObjective(12304, 0)
		Unit:SpawnCreature(28415, Unit:GetX(), Unit:GetY(), Unit:GetZ(), Unit:GetO(), 35, 5000)
	else
		Unit:SpawnCreature(28414, Unit:GetX(), Unit:GetY(), Unit:GetZ(), Unit:GetO(), 14, 180000)
	end
end

RegisterUnitEvent(28413, 4, "NerubianCocoon_OnDied")