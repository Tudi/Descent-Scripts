#include "StdAfx.h"
#include "Setup.h"

class AI_DUMMY_TOWER : public AIInterface
{
public:
   void            Init(Unit *un, AIType at, MovementType mt);
   void            Update(uint32 p_time);
private:
	float			spawn_x,spawn_y,spawn_z;
};

void AI_DUMMY_TOWER::Init(Unit *un, AIType at, MovementType mt)
{
   //run the original init and we add our custom stuff too
   AIInterface::Init(un,at,mt);
   //mark where we was spawned. Towers do not move
   spawn_x = un->GetPositionX();
   spawn_y = un->GetPositionY();
   spawn_z = un->GetPositionZ();

   un->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 500000*2 );
   un->SetUInt32Value( UNIT_FIELD_MAXPOWER1, 500000*2 );

   un->SetPowerType( POWER_TYPE_HAPPINESS );   //no packet spams to client
	disable_combat = true;
	disable_melee = true;
	disable_ranged = true;
	disable_spell = true;
	disable_targeting = true;
}

void AI_DUMMY_TOWER::Update(uint32 p_time)
{
   //in case they managed somehow to move the tower then just put it back. Ex : deathgrip
   if( spawn_x != m_Unit->GetPositionX() )
   {
      m_Unit->SetPosition( spawn_x,spawn_y,spawn_z,m_Unit->GetOrientation(), false );
//      WorldPacket data;
//      m_Unit->BuildHeartBeatMsg( &data );
//      m_Unit->SendMessageToSet( &data, false );
	  m_Unit->GetAIInterface()->StopMovement( 0 );
   }

   m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH, 5000000 );
   m_Unit->SetUInt32Value( UNIT_FIELD_POWER1, 5000000 );

   //clear resistances so people can see real DMG
   if( m_Unit->IsCreature() )
		for(int32 i=0;i<SCHOOL_COUNT;i++)
			if( static_cast<Creature*>(m_Unit)->BaseResistance[i] != 0 && static_cast<Creature*>(m_Unit)->FlatResistanceMod[i] != 0 )
			{
				static_cast<Creature*>(m_Unit)->FlatResistanceMod[i]=0;
				static_cast<Creature*>(m_Unit)->BaseResistance[i]=0;
				static_cast<Creature*>(m_Unit)->CalcResistance(i);
			}
			

//   WipeTargetList();   //we will never get to fight back :(
}

//this NPC is spawned in the midle of the arena to do everything :P
class AIDummyTowerReplacer : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(AIDummyTowerReplacer);

   //constructor
    AIDummyTowerReplacer(Creature* pCreature) : CreatureAIScript(pCreature)
    {
      //we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
      AI_DUMMY_TOWER *new_interface = new AI_DUMMY_TOWER;
      new_interface->Init(_unit,AITYPE_SOCIAL,MOVEMENTTYPE_NONE);
      _unit->ReplaceAIInterface( (AIInterface *) new_interface );
	  _unit->m_walkSpeed = 0.01f;
	  _unit->m_runSpeed = 0.01f;
	  _unit->m_backWalkSpeed = 0.01f;
	  _unit->m_flySpeed = 0.01f;
	  _unit->m_turnRate = 0.01f;
	  _unit->m_base_runSpeed = 0.01f;
	  _unit->m_base_walkSpeed = 0.01f;
   }
};

void SetupDummyHandlers(ScriptMgr * mgr)
{
	mgr->register_creature_script( 31144, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 32666, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 32667, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 31146, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 17578, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 24792, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 30527, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 31143, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 32541, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 32542, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 32543, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 32545, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 32546, &AIDummyTowerReplacer::Create );
	mgr->register_creature_script( 44548, &AIDummyTowerReplacer::Create );
}