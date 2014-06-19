function Zultore_OnDied(pUnit, event, player)
	local add1 = pUnit:GetUnitBySqlId(132393);
	local add2 = pUnit:GetUnitBySqlId(132394);
	local boss = pUnit:GetUnitBySqlId(132390);
	add1:SetUInt32Value(58, 0);
	add2:SetUInt32Value(58, 0);
	boss:SetUInt32Value(58, 0);
	player:PlaySoundToSet(8574);
end

function RunokWildmane_OnDied(pUnit, event, player)
	local add1 = pUnit:GetUnitBySqlId(132397);
	local add2 = pUnit:GetUnitBySqlId(132398);
	local boss = pUnit:GetUnitBySqlId(132391);
	add1:SetUInt32Value(58, 0);
	add2:SetUInt32Value(58, 0);
	boss:SetUInt32Value(58, 0);
	player:PlaySoundToSet(8572);
end

function DeathstalkerVisceri_OnDied(pUnit, event, player)
	local tirion = pUnit:GetUnitBySqlId(132342);
	player:PlaySoundToSet(15882);
	player:SpawnGameObject(195710,744.721,618.307,411.089,0,180000);
	tirion:SendChatMessage(14, 0, "Well fought! Your next challenge comes from the Crusade's own ranks. You will be tested against their considerable prowess");
	local add1 = pUnit:GetUnitBySqlId(132210);
	local add2 = pUnit:GetUnitBySqlId(132211);
	local add3 = pUnit:GetUnitBySqlId(132212);
	local add4 = pUnit:GetUnitBySqlId(132214);
	local add5 = pUnit:GetUnitBySqlId(132215);
	local add6 = pUnit:GetUnitBySqlId(132216);
	local add7 = pUnit:GetUnitBySqlId(132217);
	local add8 = pUnit:GetUnitBySqlId(132218);
	local add9 = pUnit:GetUnitBySqlId(132219);
	local boss = pUnit:GetUnitBySqlId(132186);
	player:PlaySoundToSet(15852);
	player:PlaySoundToSet(8573);
	add1:SetUInt32Value(58, 0);
	add2:SetUInt32Value(58, 0);
	add3:SetUInt32Value(58, 0);
	add4:SetUInt32Value(58, 0);
	add5:SetUInt32Value(58, 0);
	add6:SetUInt32Value(58, 0);
	add7:SetUInt32Value(58, 0);
	add8:SetUInt32Value(58, 0);
	add9:SetUInt32Value(58, 0);
	boss:SetUInt32Value(58, 0);
	local object = pUnit:GetGameObjectNearestCoords(746.698, 677.469, 412.339, 195647);
	object:SetUInt32Value(9,0);
	object:SetUInt32Value(17,0);
end

function EadricthePure_OnDied(pUnit, event, player)
	player:SpawnCreature(35451,747.55,624.731,411.18,0,16,360000);
	player:PlaySoundToSet(16256);
end

function EadricthePure_OnAggro(pUnit, event, player)
	pUnit:SendChatMessage(12, 0, "Are you up to the challenge? I will not hold back.");
	player:PlaySoundToSet(16134);
end

function TheBlackKnight_OnDied(pUnit, event, player)
	local tirion = pUnit:GetUnitBySqlId(132342);
	local king = pUnit:GetUnitBySqlId(132346);
	player:PlaySoundToSet(15852);
	player:PlaySoundToSet(8573);
	tirion:SendChatMessage(14, 0, "My congratulations, champions. Through trials both planned and unexpected, you have triumphed.");
	tirion:SendChatMessage(14, 0, "Go now and rest; you've earned it.");
	king:SendChatMessage(14, 0, "You fought well.");
end

function TheBlackKnight_OnAggro(pUnit, event, player)
	local tirion = pUnit:GetUnitBySqlId(132342);
	player:PlaySoundToSet(16259);
	tirion:SendChatMessage(14, 0, "Don't just stand there; kill him!");
	pUnit:SendChatMessage(14, 0, "This farce ends here!");
end

function TheBlackKnight_TargetDied(pUnit, event, player)
	player:PlaySoundToSet(16260);
	pUnit:SendChatMessage(14, 0, "Pathetic");
end

RegisterUnitEvent(35571, 4, "RunokWildmane_OnDied")
RegisterUnitEvent(35617, 4, "DeathstalkerVisceri_OnDied")
RegisterUnitEvent(35570, 4, "Zultore_OnDied")
RegisterUnitEvent(35119, 1, "EadricthePure_OnAggro")
RegisterUnitEvent(35119, 4, "EadricthePure_OnDied")
RegisterUnitEvent(35451, 1, "TheBlackKnight_OnAggro")
RegisterUnitEvent(35451, 3, "TheBlackKnight_TargetDied")
RegisterUnitEvent(35451, 4, "TheBlackKnight_OnDied")