function Captain_BrandonsOnCompletion (pUnit, Event,player)
	player:AdvanceQuestObjective(12596, 0)
	player:AdvanceQuestObjective(12501, 0)
end

function Captain_RupertsOnCompletion (pUnit, Event,player)
	player:AdvanceQuestObjective(12596, 1)
	player:AdvanceQuestObjective(12501, 1)
end

function Captain_GrondelsOnCompletion (pUnit, Event,player)
	player:AdvanceQuestObjective(12596, 2)
	player:AdvanceQuestObjective(12501, 2)
end

function Alchemist_FinklesteinsOnCompletion (pUnit, Event,player)
	player:AdvanceQuestObjective(12596, 3)
	player:AdvanceQuestObjective(12501, 3)
end


RegisterQuestEvent(12588, 2,"Captain_BrandonsOnCompletion")
RegisterQuestEvent(12502, 2,"Captain_BrandonsOnCompletion")
RegisterQuestEvent(12564, 2,"Captain_BrandonsOnCompletion")
RegisterQuestEvent(12568, 2,"Captain_RupertsOnCompletion")
RegisterQuestEvent(12509, 2,"Captain_RupertsOnCompletion")
RegisterQuestEvent(12591, 2,"Captain_RupertsOnCompletion")
RegisterQuestEvent(12594, 2,"Captain_GrondelsOnCompletion")
RegisterQuestEvent(12585, 2,"Captain_GrondelsOnCompletion")
RegisterQuestEvent(12519, 2,"Captain_GrondelsOnCompletion")
RegisterQuestEvent(12541, 2,"Alchemist_FinklesteinsOnCompletion")