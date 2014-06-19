/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Tonk Control Console	#
#################################
*/

/* 
Spells:
=====================
Cannon - 24933
Mortor - 25003
Drop Mine - 39685, 25024
Nitrous Boost - 27746
*/

class TonkControlConsole : public GameObjectAIScript
{
public:
	TonkControlConsole(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new TonkControlConsole(GO); }

	// Click the Console
	void OnActivate(Player * pPlayer)
	{
		// Pre-flight checks
		GameObject * tonkConsole = NULL;
		tonkConsole = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 180524);

		// Open and disable the Tonk Console
		tonkConsole->SetUInt32Value(GAMEOBJECT_FLAGS, 33);
		tonkConsole->SetUInt32Value(GAMEOBJECT_STATE, 0);

		// Spawn Steam Tonk
		pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(19405, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), true, false, 0, 0)->Despawn(310000, 0);;

		// Store the tonk just spawned
		Creature * pTonk = NULL;
		pTonk = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 19405);

		// Cast the tonk control spell on the tonk
		pPlayer->CastSpell(pTonk, 33849, false);

		// Start checks to see if player still has aura
		RegisterAIUpdateEvent(1000);

		Plr = pPlayer;
		Tonk = pTonk;
		Console = tonkConsole;
	}

	void AIUpdate()
	{
		if (!Plr->HasAura(33849) || Tonk->isDead())
		{
			// Kill then Despawn Tonk after 10 seconds
			Plr->CastSpell(Tonk, 5, false); // Kill spell
			Plr->CastSpell(Plr, 2880, false); // Stun Player
			Plr->RemoveAura(33849);
			Tonk->Despawn(10000,0);

			// Close the console so others can access it
			Console->SetUInt32Value(GAMEOBJECT_FLAGS, 0);
			Console->SetUInt32Value(GAMEOBJECT_STATE, 1);
			RemoveAIUpdateEvent();
		}
	}

protected:
	Player* Plr;
	Creature* Tonk;
	GameObject* Console;
};

// mgr->register_gameobject_script(180524, &TonkControlConsole::Create);