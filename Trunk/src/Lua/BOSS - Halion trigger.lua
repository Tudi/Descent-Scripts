
function helion_OnSpawn(pUnit, Event)
	pUnit:SetMapUInt32Variable("Helion_GUID",pUnit:GetGUID( pUnit ))
	pUnit:SetFaction(35)
	pUnit:SetMapUInt32Variable("Generals_remaining",3)
end

function general_OnDied(pUnit, Event)
	pUnit:ModMapUInt32Variable( "Generals_remaining",-1)
	local t = pUnit:GetMapUInt32Variable("Generals_remaining")
	if t <= 0 then
		local guid = pUnit:GetMapUInt32Variable("Helion_GUID")
		if( guid ~= nil ) then
			local helion = pUnit:GetUnitByGUID(guid)
			helion:SetFaction(16)
		end
--	else
--		pUnit:SendChatMessage(14, 0, "generals var value: "..t.." !")
	end
	if t > 3 then
		local guid = pUnit:GetMapUInt32Variable("Helion_GUID")
		if( guid ~= nil ) then
			local helion = pUnit:GetUnitByGUID(guid)
			helion:SetFaction(16)
		end
--	else
--		pUnit:SendChatMessage(14, 0, "generals var value: "..t.." !")
	end
end

RegisterUnitEvent(39863, 18, "helion_OnSpawn")

RegisterUnitEvent(39747, 4, "general_OnDied")
RegisterUnitEvent(39751, 4, "general_OnDied")
RegisterUnitEvent(39746, 4, "general_OnDied")
