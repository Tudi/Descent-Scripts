/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Blastenheimer 5000	#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class Blastenheimer5000 : public GameObjectAIScript
{
public:
	Blastenheimer5000(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new Blastenheimer5000(GO); }

	void OnActivate(Player * pPlayer)
	{
/*		GameObject * pCannon = NULL;
		pCannon = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 180515);
		
		// Disable the cannon when in use
		pCannon->SetUInt32Value(GAMEOBJECT_FLAGS,33);

		// Cannon prep spell (makes player invisible)
		pPlayer->CastSpell(pPlayer, 24832, true);
		
		// The cannon still doesnt animate like it should..
		//pCannon->SetUInt32Value(GAMEOBJECT_ANIMPROGRESS, 100);

		// Start timer to shoot player
		RegisterAIUpdateEvent(2400);
		
		// Play Cannon Shoot Sound
		pCannon->PlaySoundToSet(8476);

		Plr = pPlayer;
		Cannon = pCannon;*/

		float direction_to_fly = _gameobject->GetOrientation() + M_PI;
		pPlayer->SetOrientation( direction_to_fly );
		// Cannon prep spell (makes player invisible) then ads wings later
		pPlayer->CastSpell(pPlayer, 24832, true);
//		WorldPacket pdata( 100 );
//		pPlayer->BuildHeartBeatMsg( &pdata );
//		pPlayer->GetSession()->SendPacket( &pdata );
		pPlayer->GetAIInterface()->StopMovement( 0 );
		// Play Cannon Shoot Sound
		_gameobject->PlaySoundToSet(8476);
	}

/*	void AIUpdate()
	{
		// Teleport player to correct launching location before shooting them
		Plr->SafeTeleport(0, 0, -9569.150391f, -14.753426f, 68.051422f, 4.874008f); // Goldshire
		Plr->CastSpell(Plr,24742,true); //Cast Magic Wings on the player
		RemoveAIUpdateEvent(); // Disable GO timer (patch needed for this to work!)
		Cannon->SetUInt32Value(GAMEOBJECT_FLAGS,0); // Re-enable the cannon for other players
	}

protected:
	Player* Plr;
	GameObject* Cannon; */
};

void SetupBlastenheimer5000(ScriptMgr *mgr)
{
	mgr->register_gameobject_script(180515, &Blastenheimer5000::Create);
}
