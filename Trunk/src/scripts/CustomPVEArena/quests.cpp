#include "StdAfx.h"
#include "Setup.h"

class SCRIPT_DECL CityBuilder : public GossipScript
{
public:
	CityBuilder()
	{
	}
    void GossipHello( Object * pObject, Player* Plr, bool AutoSend )
	{
		if( Plr->CombatStatus.IsInCombat() )
		{
			Plr->BroadcastMessage("You are not allowed to use this NPC while in combat");
			return;
		}
		if( AutoSend )
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem( 6, "Queue for 10v10 arena", 1 );
			Menu->AddItem( 6, "Close", 3 );
			Menu->SendTo(Plr);
		}
	}
    void GossipSelectOption(Object * pObject, Player *Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Plr->CloseGossip();
		if( IntId == 1 )
		{
			Plr->BroadcastMessage("You are already queued ?");
		}
	}
    void GossipEnd(Object * pObject, Player* Plr)
	{
		GossipScript::GossipEnd(pObject, Plr);
	}
	void Destroy()	{		delete this;	}
};

QuestKillObjective *NewKillObjective( uint32 Group, uint32 Entry, uint32 KillCount, char *Name, char *Description, float x, float y, float z, float o )
{
	QuestKillObjective *ret = ( QuestKillObjective * )malloc( sizeof( QuestKillObjective ) );
	memset( ret, 0, sizeof( QuestKillObjective ) );
	ret->Group = Group;
	ret->Entry = Entry;
	ret->KillCountRequired = KillCount;
	strcpy_s( ret->Name, BROADCAST_MAX_MSG_LEN, Name );
	strcpy_s( ret->Description, BROADCAST_MAX_MSG_LEN, Description );
	ret->Spawn.x = x;
	ret->Spawn.y = y;
	ret->Spawn.z = z;
	ret->Spawn.o = o;
	return ret;
}

void SetupQuests( ScriptMgr* mgr )
{
	GossipScript *gs;
	gs = (GossipScript*) new CityBuilder();
	mgr->register_gossip_script( 1 , gs );

	QuestObjectiveList.push_back( NewKillObjective( QO_BUILD_BANK + QO_BUILD_TEAM_SHIFTER1, 132, 10, "Bank lvl 1", "Gain Gold periodically", 1, 1, 1, 1 ) );
	QuestObjectiveList.push_back( NewKillObjective( QO_BUILD_BANK + QO_BUILD_TEAM_SHIFTER2, 132, 10, "Bank lvl 1", "Gain Gold periodically", 1, 1, 1, 1 ) );

	QuestObjectiveList.push_back( NewKillObjective( QO_BUILD_VENDOR + QO_BUILD_TEAM_SHIFTER1, 133, 10, "Vendor lvl 1", "Sells basic items", 1, 1, 1, 1 ) );
	QuestObjectiveList.push_back( NewKillObjective( QO_BUILD_VENDOR + QO_BUILD_TEAM_SHIFTER2, 133, 10, "Vendor lvl 1", "Sells basic items", 1, 1, 1, 1 ) );

	QuestObjectiveList.push_back( NewKillObjective( QO_BUILD_DEFENSE + QO_BUILD_TEAM_SHIFTER1, 134, 10, "Defense lvl 1", "Spawn defensive units", 1, 1, 1, 1 ) );
	QuestObjectiveList.push_back( NewKillObjective( QO_BUILD_DEFENSE + QO_BUILD_TEAM_SHIFTER2, 134, 10, "Defense lvl 1", "Spawn defensive units", 1, 1, 1, 1 ) );

	QuestObjectiveList.push_back( NewKillObjective( QO_BUILD_HEALER + QO_BUILD_TEAM_SHIFTER1, 135, 10, "Healer lvl 1", "Regenerate health over time", 1, 1, 1, 1 ) );
	QuestObjectiveList.push_back( NewKillObjective( QO_BUILD_HEALER + QO_BUILD_TEAM_SHIFTER2, 135, 10, "Healer lvl 1", "Regenerate health over time", 1, 1, 1, 1 ) );
}