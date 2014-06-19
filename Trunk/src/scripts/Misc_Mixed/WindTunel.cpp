#include "StdAfx.h"
#include "Setup.h"

//this need probably some beatifull fly animation for char. For right now they will teleport
//the sucky part that same entry is used for multiple places. They need to guess where to take people

#define WINDTUNNEL_ENTRY 48092
#define SLIPSTREAM_ENTRY 45455
#define SLIPSTREAM_DEST_ENTRY 45504	//we are supposed to port to the nearest guy like him

class WindTunnelSetup : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WindTunnelSetup);

	//constructor
	WindTunnelSetup(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		//we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
		pCreature->SetFaction( 35 );
		pCreature->GetAIInterface()->m_canCallForHelp = false;
		pCreature->GetAIInterface()->m_canFlee = false;
		pCreature->GetAIInterface()->m_canRangedAttack = false;
		pCreature->GetAIInterface()->disable_combat = true;
		pCreature->GetAIInterface()->disable_melee = true;
		pCreature->GetAIInterface()->disable_spell = true;
		pCreature->GetAIInterface()->disable_targeting = true;
		pCreature->GetAIInterface()->m_moveFly = true;
		pCreature->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 9000000 );
		pCreature->SetUInt32Value( UNIT_FIELD_HEALTH, 9000000 );
		pCreature->SetUInt32Value( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP );

		for(uint32 i=0;i<7;i++)
			pCreature->SchoolImmunityAntiEnemy[i] = 999;
		for(uint32 i=0;i<31;i++)
			pCreature->MechanicsDispels[i] = 999;
	}
};


class SCRIPT_DECL WindTunnelActivate : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
	void Destroy()	{		delete this;	}
};

void WindTunnelActivate::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
	//let's guess who we are and where we take
	//format : where we are -> where we take player
	// map, x, y, z
	float best_dist = 99999.0f;
	int32 best_ind = -1;
	float locations[5][7]=
	{
		{657.0f,-769.0f,-52.0f,639.0f,
			   -906.0f,-176.0f,666.0f},	
		{657.0f,-1190.0f,125.0f,737.0f,
			   -1193.0f,472.0f,636.0f},	
		{755.0f,-10898.0f,-1382.0f,15.0f,
			   -10902.0f,-1382.0f,37.0f},	
		{755.0f,-10992.0f,-1360.0f,10.0f,
			   -10978.0f,-1383.0f,37.0f},	
		{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}	//last line is just 0
	};
	for(uint32 ind=0;ind<100; ind++)
	{
		if( locations[ind][0] == 0.0f )
			break;
		if( locations[ind][0] == Plr->GetMapId() )
		{
			float local_dist_x = locations[ind][1] - Plr->GetPositionX();
			float local_dist_y = locations[ind][2] - Plr->GetPositionY();
			float dist = local_dist_x * local_dist_x + local_dist_y * local_dist_y;
			if( dist < best_dist )
			{
				dist = best_dist;
				best_ind = ind;
			}
		}
	}
	if( best_ind != -1 )
		Plr->SafeTeleport( locations[best_ind][0], 0, locations[best_ind][4],locations[best_ind][5],locations[best_ind][6], Plr->GetOrientation() );
}

void RegisterWindtunnelHandlers(ScriptMgr * mgr)
{
	//this will be used to setup NPC flags and stuff in case creature_proto says something else
	mgr->register_creature_script(WINDTUNNEL_ENTRY, &WindTunnelSetup::Create);
	mgr->register_creature_script(SLIPSTREAM_ENTRY, &WindTunnelSetup::Create);

	GossipScript * gs = (GossipScript*) new WindTunnelActivate();
    mgr->register_gossip_script(WINDTUNNEL_ENTRY, gs);         
	gs = (GossipScript*) new WindTunnelActivate();
    mgr->register_gossip_script(SLIPSTREAM_ENTRY, gs);         
}

