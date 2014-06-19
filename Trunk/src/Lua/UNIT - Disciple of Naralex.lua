function DiscipleofNaralex_OnGossipTalk(pUnit, event, player)
	local Fanglord1 = pUnit:GetCreatureNearestCoords(-151.139008, 414.367004, -72.629402, 3669);
	local Fanglord2 = pUnit:GetCreatureNearestCoords(36.807400, -241.063995, -79.498901, 3670);
	local Fanglord3 = pUnit:GetCreatureNearestCoords(-118.710999, -24.990999, -28.498501, 3673);
	local Fanglord4 = pUnit:GetCreatureNearestCoords(-70.788902, 120.072998, -89.673599, 3671);
	if(Fanglord1:isAlive() and Fanglord2:isAlive() and Fanglord3:isAlive() and Fanglord4:isAlive()) then
		pUnit:GossipCreateMenu(100, player, 0)
		pUnit:GossipMenuAddItem(0, "Let's go!", 1, 0)
		pUnit:GossipSendMenu(player)
	end
end

function DiscipleofNaralex_OnGossip_select(pUnit, event, player, id, intid, code)
	if(intid == 1) then
		pUnit:CreateCustomWaypointMap()
		pUnit:CreateWaypoint(-132.498077, 125.888153, -78.418182, 0.244260,0,256,0)
		pUnit:CreateWaypoint(-123.892235, 130.538422, -78.808937, 0.519935,0,256,0)
		pUnit:CreateWaypoint(-116.654480, 142.935806, -80.233383, 1.149039,0,256,0)
		pUnit:CreateWaypoint(-111.656868, 156.927307, -79.880676, 1.344603,0,256,0)
		pUnit:CreateWaypoint(-108.829506, 169.213165, -79.752487, 1.344603,0,256,0)
		pUnit:CreateWaypoint(-107.592789, 183.854782, -79.735558, 1.500112,0,256,0)
		pUnit:CreateWaypoint(-106.628258, 197.477676, -80.526184, 1.500112,0,256,0)
		pUnit:CreateWaypoint(-109.725700, 215.487885, -85.336075, 1.650287,0,256,0)
		pUnit:CreateWaypoint(-106.663147, 225.879135, -88.962914, 0.215201,0,256,0)
		pUnit:CreateWaypoint(-90.607216, 228.829071, -91.022133, 6.067203,0,256,0)
		pUnit:CreateWaypoint(-79.377800, 219.999466, -93.990906, 5.482866,0,256,0)
		pUnit:CreateWaypoint(-69.134697, 209.446045, -93.404358, 5.482866,0,256,0)
		pUnit:CreateWaypoint(-53.198994, 204.919601, -95.677971, 6.071915,0,256,0)
		pUnit:CreateWaypoint(-38.501598, 211.024460, -96.222626, 0.559205,0,256,0)
		pUnit:CreateWaypoint(-39.211544, 197.527161, -96.574646, 4.658991,0,256,0)
		pUnit:CreateWaypoint(-40.258022, 177.948105, -96.374756, 4.658991,0,256,0)
		pUnit:CreateWaypoint(-41.385948, 156.845230, -94.969429, 4.658991,0,256,0)
		pUnit:CreateWaypoint(-49.557240, 145.598206, -93.284225, 4.084079,0,256,0)
		pUnit:CreateWaypoint(-52.191185, 133.269424, -90.334198, 4.501911,0,256,0)
		pUnit:CreateWaypoint(-53.070702, 122.185814, -89.757874, 5.128569,0,256,0)
		pUnit:CreateWaypoint(-47.618214, 115.986847, -87.939827, 5.562199,0,256,0)
		pUnit:CreateWaypoint(-36.105568, 109.539597, -87.755760, 5.772686,0,256,0)
		pUnit:CreateWaypoint(-23.849794, 109.712982, -89.580704, 0.014146,0,256,0)
		pUnit:CreateWaypoint(-15.070121, 119.703346, -89.904770, 0.849840,0,256,0)
		pUnit:CreateWaypoint(-6.799855, 134.471298, -89.574089, 1.060297,0,256,0)
		pUnit:CreateWaypoint(1.530990, 143.322433, -89.091454, 0.589058,0,256,0)
		pUnit:CreateWaypoint(11.134405, 149.739365, -88.872955, 0.589058,0,256,0)
		pUnit:CreateWaypoint(21.220901, 156.479080, -89.180771, 0.802686,0,256,0)
		pUnit:CreateWaypoint(31.682161, 167.308456, -88.896530, 0.802686,0,256,0)
		pUnit:CreateWaypoint(43.933167, 179.990555, -88.922348, 0.802686,0,256,0)
		pUnit:CreateWaypoint(51.662514, 195.831421, -89.649101, 1.116846,0,256,0)
		pUnit:CreateWaypoint(70.554794, 204.757950, -92.880386, 0.441403,0,256,0)
		pUnit:CreateWaypoint(85.016724, 211.591156, -92.615730, 0.441403,0,256,0)
		pUnit:CreateWaypoint(99.523796, 213.738951, -96.214615, 0.047919,0,256,0)
		pUnit:CreateWaypoint(112.235191, 214.378525, -98.332832, 0.679379,0,256,0)
		pUnit:CreateWaypoint(118.665100, 220.504974, -98.305420, 1.254290,0,256,0)
		pUnit:CreateWaypoint(121.259758, 228.493378, -97.359711, 1.632852,0,256,0)
		pUnit:CreateWaypoint(116.031120, 236.451187, -96.007195, 3.089230,0,256,0)
		pUnit:SendChatMessage(11, 0,"Step back and be ready!, I'll try to Awake Naralex")
		pUnit:PlaySoundToSet(5786)
		pUnit:CastSpell(6271)
	end
end

RegisterUnitGossipEvent(3678,1,"DiscipleofNaralex_OnGossipTalk")
RegisterUnitGossipEvent(3678,2,"DiscipleofNaralex_OnGossip_select")