function justin_Talk(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "And so the knights stood before the charging Horde and held their ground as a thousand berserk orcs came through the valley.")
	pUnit:RegisterEvent("justin_Talk1",97000, 1)
end


function brandon_Talk1(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Oh yeah, I heard about that.")
	pUnit:RegisterEvent("brandon_Talk2",70000, 1)
end

function brandon_Talk2(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Oh, c'mon thats not true.")
	pUnit:RegisterEvent("brandon_Talk3",72500, 1)
end

function roman_Talk(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "I got worm guts on my shoes.")
	pUnit:RegisterEvent("roman_Talk1",65000, 1)
end

function timmy(Unit, event, player)
	Unit:RegisterEvent("timmy_Say",68000, 0)
end

function timmy_Say(Unit, event, player)
	local chance = math.random(1,6)
	if(chance == 1) then
		Unit:SendChatMessage(12, 0, "Kitten for sale, looking for a good home." )
	end
	if(chance == 2) then
		Unit:SendChatMessage(12, 0, "I can't believe dad won't let me keep your sister.")
	end
	if(chance == 3) then
		Unit:SendChatMessage(12, 0, "Can anyone give my adorable, extra little kitty a home?")
	end
	if(chance == 4) then
		Unit:SendChatMessage(12, 0, "What does allergic mean anyway? And what does it have to do with either of my kitties?")
	end
	if(chance == 5) then
		Unit:SendChatMessage(12, 0, "Will someone please give my little kitten a good home?")
	end
	if(chance == 6) then
		Unit:SendChatMessage(12, 0, "Don't worry, I'll find a good home for ya.")
	end
end

function roman_Talk1(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "When he was given to us by 'Prince' Kael'thas, we believed his power would help us lead our people into a new age.")
	pUnit:RegisterEvent("roman_Talk2",65000, 1)
end


function roman_Talk2(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "I thought I heard something.")
	pUnit:RegisterEvent("roman_Talk3",65000, 1)
end

function brandon_Talk3(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Really?")
	pUnit:RegisterEvent("brandon_Talk4",64000, 1)
end

function brandon_Talk4(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "WoW.")
	pUnit:RegisterEvent("brandon_Talk5",64000, 1)
end

function roman_Talk3(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Something smells funny.")
	pUnit:RegisterEvent("roman_Talk4",65000, 1)
end

function justin_Talk1(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "You know there are crocolisks in the canals. They were brought from the swamp as pets but got thrown in the canals.")
	pUnit:RegisterEvent("justin_Talk2",64000, 1)
end

function justin_Talk2(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Abd that's how Lothar killed thirty six orcs with his bare hands!")
	pUnit:RegisterEvent("justin_Talk3",70000, 1)
end

function roman_Talk4(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "I think I see something.")
	pUnit:RegisterEvent("roman_Talk5",68000, 1)
end

function roman_Talk5(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Worm goes on the hook, hook goes in the water. Fish is in the water, our fish.")
	pUnit:RegisterEvent("roman_Talk6",70500, 1)
end

function roman_Talk6(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "There is no spoon")
	pUnit:RegisterEvent("roman_Talk7",65000, 1)
end

function justin_Talk3(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "And then the rabbit just bit his head off...I swear.")
	pUnit:RegisterEvent("justin_Talk4",61000, 1)
end

function justin_Talk4(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "You know why orc eyes glow red? It's because they drink blood!")
	pUnit:RegisterEvent("justin_Talk5",64000, 1)
end

function brandon_Talk5(pUnit,Event)
	pUnit:RegisterEvent("brandon_Talk1",66666, 1)
end

function justin_Talk5(pUnit,Event)
	pUnit:RegisterEvent("justin_Talk",66666, 1)
end

function roman_Talk7(pUnit,Event)
	pUnit:RegisterEvent("roman_Talk",66666, 1)
end

function billy_Talk1(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "I heard that there are these huge fish that can walk on land to hunt, and eat people!")
	pUnit:RegisterEvent("billy_Talk2",41000, 1)
end

function adam_Talk(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "My daddy says that in the ocean, there are fish so big they could swallow a whole ship.")
	pUnit:RegisterEvent("adam_Talk1",40000, 1)
end

function billy_Talk2(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "I heard a story about this golden fish, and if you caught it you would get three wishes!")
	pUnit:RegisterEvent("billy_Talk3",41000, 1)
end


function adam_Talk1(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "And one time, at camp, I caught a fish that was bigger than I am!!")
	pUnit:RegisterEvent("adam_Talk2",40000, 1)
end


function billy_Talk3(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "I caught a big one last week, it had three eyes!")
	pUnit:RegisterEvent("billy_Talk4",41000, 1)
end

function adam_Talk2(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "My daddy can catch more fish than your dady!")
	pUnit:RegisterEvent("adam_Talk4",40000, 1)
end

function billy_Talk4(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Look! Look! I caught something! Aww...it's just a stinky ol' boot.")
	pUnit:RegisterEvent("billy_Talk1",41000, 1)
end

function adam_Talk4(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Think there are any fish in here?")
	pUnit:RegisterEvent("adam_Talk",40000, 1)
end

function gil(Unit, event, player)
	Unit:RegisterEvent("gil_Say",18000, 0)
end

function gil_Say(Unit, event, player)
	local chance = math.random(1,6)
	if(chance == 1) then
		Unit:SendChatMessage(12, 0, "I wanna see the Mage Tower." )
	end
	if(chance == 2) then
		Unit:SendChatMessage(12, 0, "s it true? Are there crocolisks in the canal?")
	end
	if(chance == 3) then
		Unit:SendChatMessage(12, 0, "Why do we always go the same way?")
	end
	if(chance == 4) then
		Unit:SendChatMessage(12, 0, "Are we there yet?")
	end
	if(chance == 5) then
		Unit:SendChatMessage(12, 0, "I have to pee.")
	end
	if(chance == 6) then
		Unit:SendChatMessage(12, 0, "My feet hurt.")
	end
end

function donna(Unit, event, player)
	Unit:RegisterEvent("donna_Say",45000, 0)
end

function donna_Say(Unit, event, player)
	local chance = math.random(1,3)
	if(chance == 1) then
		Unit:SendChatMessage(12, 0, "Hey you, did you see my puppet ?")
	end
	if(chance == 2) then
		Unit:SendChatMessage(12, 0, "Guard ? Can you find my puppet for me ?")
	end
	if(chance == 3) then
		Unit:SendChatMessage(12, 0, "Hmmm ... Where is my puppet ?")
	end
end

function donna_Died(Unit, event, player)
	Unit:RemoveEvents()
end

function billy_Died(pUnit, Event)
	pUnit:RemoveEvents()
end

function adam_Died(pUnit, Event)
	pUnit:RemoveEvents()
end

function justin_Died(pUnit, Event)
	pUnit:RemoveEvents()
end

function roman_Died(pUnit, Event)
	pUnit:RemoveEvents()
end

function brandon_Died(pUnit, Event)
	pUnit:RemoveEvents()
end

function timmy_Died(Unit, event, player)
	Unit:RemoveEvents()
end

function gil_Died(Unit, event, player)
	Unit:RemoveEvents()
end

RegisterUnitEvent(1366, 18, "adam_Talk")
RegisterUnitEvent(1366, 4, "adam_Died")
RegisterUnitEvent(1367, 18, "billy_Talk1")
RegisterUnitEvent(1367, 4, "billy_Died")
RegisterUnitEvent(1368, 18, "justin_Talk")
RegisterUnitEvent(1368, 4, "justin_Died")
RegisterUnitEvent(1370, 18, "brandon_Talk1")
RegisterUnitEvent(1370, 4, "brandon_Died")
RegisterUnitEvent(1371, 18, "roman_Talk")
RegisterUnitEvent(1371, 4, "roman_Died")
RegisterUnitEvent(2532, 18, "donna")
RegisterUnitEvent(2532, 4, "donna_Died")
RegisterUnitEvent(3504, 18, "gil")
RegisterUnitEvent(3504, 4, "gil_Died")
RegisterUnitEvent(8666, 18, "timmy")
RegisterUnitEvent(8666, 4, "timmy_Died")