#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305) // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

#define ID_ULDUAR_TELEPORTER 123457 // I have no idea what's the ID of the teleporter since I don't have access to the DB to get the GameObject ID.
#define ID_ULDUAR_TELEPORTER_GO 194569 

class SCRIPT_DECL UlduarTeleporter : public GossipScript
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

void UlduarTeleporter::GossipHello(Object*  pObject, Player* Plr, bool AutoSend)
{
	GossipMenu *Menu;
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

    Menu->AddItem(2, "Expedition Base Camp", 0);
    Menu->AddItem(2, "Formation Grounds", 1);
    Menu->AddItem(2, "Colossal Forge", 2);
    Menu->AddItem(2, "The Antechamber of Ulduar", 3);
    Menu->AddItem(2, "Spark of Imagination", 4);
    Menu->AddItem(2, "Observation Ring", 5);
    Menu->AddItem(2, "Antechamber", 6);
    Menu->AddItem(2, "The Descent into Madness", 7);

    if(AutoSend)
		Menu->SendTo(Plr);
};

void UlduarTeleporter::GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char *Code)
{
	switch(IntId)
	{
	case 0:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), -735.864075f, -93.616364f, 429.841797f, 0.079723f); //Expedition Base Camp
		break;

	case 1:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 130.710297f, -35.272095f, 409.804901f, 6.276515f); //Formation Grounds
		break;

	case 2:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 539.894897f, -11.009894f, 409.804749f, 0.021830f);
		break;

	case 3:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 1460, 120.585, 424, 0.021830f);
		break;
	case 4:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 2344, 2572.116, 420, 0.021830f);
		break;
	case 5:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 1858, 25.637, 434, 0.021830f);
		break;
	case 6:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 1498, -24, 421, 0.021830f);
		break;
	case 7:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 1821, 219, 361, 0.021830f);
		break;
	};
};

void UlduarTeleporter::GossipEnd(Object* pObject, Player* pPlayer)
{
	GossipScript::GossipEnd(pObject, pPlayer);
}

void SetupUlduarTeleporter(ScriptMgr* mgr)
{
	GossipScript * gs = (GossipScript*) new UlduarTeleporter();
//replace INTO `creature_names` (`entry`, `name`, `Subname`,  `type`,`Rank`, `male_displayid`) VALUES
//(123457, 'Displacement device', 'Ulduar Teleporter', 7, 4, 26937);
//replace INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`,`modImmunities`) VALUES
//(123457, 100, 100, 35, 700000, 700000, 1000000, 2, 1, 1000, 0, 10000, 10000, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, '0', 1, 0, 0, 0, 2.5, 8, 14, 0, 0);
	mgr->register_gossip_script(ID_ULDUAR_TELEPORTER, gs);
	mgr->register_go_gossip_script(ID_ULDUAR_TELEPORTER_GO, gs);
};