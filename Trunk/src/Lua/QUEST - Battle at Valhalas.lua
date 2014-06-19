function Battle_at_Valhalas_Carnage_Quest_onStart (pUnit, Event)
	pUnit:SpawnCreature(31271, 8202.396, 3485.595, 625.8, 1.064711, 16, 540000)
end

function Battle_at_Valhalas_Khitrix_the_Dark_Master_Quest_onStart (pUnit, Event)
	pUnit:SpawnCreature(31222, 8202.396, 3485.595, 625.8, 1.064711, 16, 540000)
end

function Battle_at_Valhalas_Thane_Deathblow_Quest_onStart (pUnit, Event)
	pUnit:SpawnCreature(31277, 8202.396, 3485.595, 625.8, 1.064711, 16, 540000)
end

function Battle_at_Valhalas_The_Return_of_Sigrid_Iceborn_Quest_onStart (pUnit, Event)
	pUnit:SpawnCreature(31242, 8202.396, 3485.595, 625.8, 1.064711, 16, 540000)
end

function Battle_at_Valhalas_Final_Challenge_Quest_onStart (pUnit, Event)
	pUnit:SpawnCreature(14688, 8202.396, 3485.595, 625.8, 1.064711, 16, 540000)
end

RegisterQuestEvent(13217, 1, "Battle_at_Valhalas_Carnage_Quest_onStart")
RegisterQuestEvent(13215, 1, "Battle_at_Valhalas_Khitrix_the_Dark_Master_Quest_onStart")
RegisterQuestEvent(13218, 1, "Battle_at_Valhalas_Thane_Deathblow_Quest_onStart")
RegisterQuestEvent(13216, 1, "Battle_at_Valhalas_The_Return_of_Sigrid_Iceborn_Quest_onStart")
RegisterQuestEvent(13219, 1, "Battle_at_Valhalas_Final_Challenge_Quest_onStart")