function BloodFilledOrb_OnUse(pGameObject, Event)
	pGameObject:SpawnCreature(17830, -370.133, 162.519, -21.1299, -1.29154, 1606, 0) 
end

RegisterGameObjectEvent(182024, 4, "BloodFilledOrb_OnUse")