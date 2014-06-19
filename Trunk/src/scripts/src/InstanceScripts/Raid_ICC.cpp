#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305) // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

#define ID_ICC_TELEPORTER 123459 // I have no idea what's the ID of the teleporter since I don't have access to the DB to get the GameObject ID.

class SCRIPT_DECL ICCTeleporter : public GossipScript
{
public:
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
	void GossipEnd(Object * pObject, Player* Plr);
	void Destroy()
	{
		delete this;
	}
};

void ICCTeleporter::GossipHello(Object*  pObject, Player* Plr, bool AutoSend)
{
	GossipMenu *Menu;
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

    Menu->AddItem(2, "Teleport to Start", 0);
    Menu->AddItem(2, "Deathbringer's Rise", 1);
    Menu->AddItem(2, "The Upper Reaches", 2);
    
    if(AutoSend)
		Menu->SendTo(Plr);
};

void ICCTeleporter::GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char *Code)
{
	switch(IntId)
	{
	case 0:
		Plr->SafeTeleport(631, Plr->GetInstanceID(), -16.119860, 2211.639160, 30.1157, 0); //Start Zone
		break;

	case 1:
		Plr->SafeTeleport(631, Plr->GetInstanceID(), -540.666138, 2211.729980, 539.391077, 0); //Deathbringer's Rise
		break;

	case 2:
		Plr->SafeTeleport(631, Plr->GetInstanceID(), 4346.56, 2769.516, 356.035, 0); //The Upper Reaches"
		break;
	};
};

void ICCTeleporter::GossipEnd(Object* pObject, Player* pPlayer)
{
	GossipScript::GossipEnd(pObject, pPlayer);
}

void SetupICCTeleporter(ScriptMgr* mgr)
{
	GossipScript * gs = (GossipScript*) new ICCTeleporter();
//replace INTO `creature_names` (`entry`, `name`, `Subname`,  `type`,`Rank`, `male_displayid`) VALUES
//(123459, 'Displacement device', 'ICC Teleporter', 7, 4, 26937);
//replace INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`,`modImmunities`) VALUES
//(123459, 100, 100, 35, 700000, 700000, 1000000, 2, 1, 1000, 0, 10000, 10000, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0, 1, 0, 0, 0, 2.5, 8, 14, 0, 0);
	mgr->register_gossip_script(ID_ICC_TELEPORTER, gs);
};