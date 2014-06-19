#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"

#ifdef WIN32
  #pragma warning(disable:4305)        // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

class JumpEventSupervisor : public AIInterface
{
public:
	void				Init(Unit *un, AIType at, MovementType mt);
	void				Update(uint32 p_time);
	~JumpEventSupervisor();
private:
	void				DesPawnPads();
	uint32				JumpEventSlot;
	uint32				JE_Cronometer;
	uint32				JE_next_pad_id;
	uint32				JE_pads_used_now;
	JumpPadStore		JE_pads[ MAX_JUMPPADS_IN_EVENT ];
};

JumpEventSupervisor::~JumpEventSupervisor()
{
	for( uint32 i=0;i< MAX_JUMPPADS_IN_EVENT; i++ )
		if( JE_pads[i].pad )
		{
			JE_pads[i].pad->Despawn(0);
			JE_pads[i].pad = NULL;
		}
}

void JumpEventSupervisor::Init(Unit *un, AIType at, MovementType mt)
{
	AIInterface::Init(un,at,mt);//run the original init and we add our custom stuff too
	//search for a slot in global list
	CreateLock.Acquire();
	JumpEventSlot = 0;
	for(uint32 i=0;i<MAX_NUMBER_OF_CONCURENT_JUMP_EVENT_HOLDERS;i++)
		if( JumpEventHolders[i].SupervizorNPC == 0 )
		{
			JumpEventHolders[i].SupervizorNPC = un->GetGUID();
			JumpEventSlot = i;
			break;
		}
	CreateLock.Release();
	//make sure to not have junk in state stores
	memset( &JE_pads, 0, sizeof( JE_pads ) );
}

void JumpEventSupervisor::DesPawnPads()
{
	for( uint32 i=0;i< MAX_JUMPPADS_IN_EVENT; i++ )
		if( JE_pads[i].pad )
			JE_pads[i].pad->RemoveFromWorld( false );
}

void JumpEventSupervisor::Update(uint32 p_time)
{
	//sanity check. We are registered correctly
	if( JumpEventHolders[ JumpEventSlot ].SupervizorNPC != m_Unit->GetGUID() )
		return;
	//nothing to do ?
	if( JumpEventHolders[ JumpEventSlot ].PlayerGUID == 0 )
		return;
	Player *Plr = m_Unit->GetMapMgr()->GetPlayer( JumpEventHolders[ JumpEventSlot ].PlayerGUID );
	if( Plr == NULL 
		|| JumpEventHolders[ JumpEventSlot ].EventExpireStamp < GetTickCount()
		)
	{
		JumpEventHolders[ JumpEventSlot ].PlayerGUID = 0;
		JumpEventHolders[ JumpEventSlot ].EventState = JES_NOTHING_INTELIGENT;
		DesPawnPads();
		return;
	}
	//time to start the event
	if( JumpEventHolders[ JumpEventSlot ].EventState == JES_WAITING_TO_START )
	{
		//generate pads randomly but respecting rulez
		float pad_cur_x = m_Unit->GetPositionX();
		float pad_cur_y = m_Unit->GetPositionY();
		float pad_cur_z = m_Unit->GetPositionZ()+1.0f;	//lift it above ground ?
		float pad_min_x = m_Unit->GetPositionX() - MAX_JUMPPAD_DISTANCE_SINCE_SOURCE/2;
		float pad_min_y = m_Unit->GetPositionY() - MAX_JUMPPAD_DISTANCE_SINCE_SOURCE/2;
		float pad_min_z = m_Unit->GetPositionZ() - MAX_JUMPPAD_DISTANCE_SINCE_SOURCE/2;
		float pad_max_x = pad_min_x + MAX_JUMPPAD_DISTANCE_SINCE_SOURCE;
		float pad_max_y = pad_min_x + MAX_JUMPPAD_DISTANCE_SINCE_SOURCE;
		JE_pads_used_now = MIN_JUMPPADS_IN_EVENT + Plr->getLevel();
		if( JE_pads_used_now >= MAX_JUMPPADS_IN_EVENT )
			JE_pads_used_now = MAX_JUMPPADS_IN_EVENT - 1;
		for( uint32 i=0; i<= JE_pads_used_now; i++ )
		{
			uint32 rand_x_PCT = RandomUInt() % 100;	//[0-99]
			int32 rand_x_sign = (RandomUInt() % 2)*2 - 1;
			uint32 rand_y_PCT = RandomUInt() % (100 - rand_x_PCT);	//[0-99]
			int32 rand_y_sign = (RandomUInt() % 2)*2 - 1;
			uint32 rand_z_PCT = RandomUInt() % (100 - rand_y_sign - rand_x_PCT );
			int32 rand_z_sign = (RandomUInt() % 2)*2 - 1;
			pad_cur_x = pad_cur_x + MAX_JUMPPAD_DISTANCE_FROM_LAST * rand_x_PCT / 100.0f * rand_x_sign;
			if( pad_cur_x < pad_min_x )
				pad_cur_x = pad_min_x;
			if( pad_cur_x > pad_max_x )
				pad_cur_x = pad_max_x;
			pad_cur_y = pad_cur_y + MAX_JUMPPAD_DISTANCE_FROM_LAST * rand_y_PCT / 100.0f * rand_y_sign;
			if( pad_cur_y < pad_min_y )
				pad_cur_y = pad_min_y;
			if( pad_cur_y > pad_max_y )
				pad_cur_y = pad_max_y;
			pad_cur_z = pad_cur_z + MAX_JUMPPAD_DISTANCE_FROM_LAST * rand_z_PCT / 100.0f * rand_z_sign;
			if( pad_cur_z < pad_min_z )
				pad_cur_z = pad_min_z;
			JE_pads[i].x = pad_cur_x;
			JE_pads[i].y = pad_cur_y;
			JE_pads[i].z = pad_cur_z;
//			JE_pads[i].state = JPS_NOT_SPAWNED;
		}
		//despawn old pads
		//in case pads are not yet created then create them
		GOSpawn ts;
		ts.entry = JUMP_PAD_GO_ENTRY;
		ts.facing = 0;
		ts.faction = SUPERVISOR_FACION;
		ts.scale = 1.0f;
		ts.state = 0.0f;
		for( uint32 i=0;i< JE_pads_used_now; i++ )
			if( JE_pads[i].pad == NULL )
			{
				JE_pads[i].pad = m_Unit->GetMapMgr()->CreateGameObject( JUMP_PAD_GO_ENTRY );
				ts.x = JE_pads[i].x;
				ts.y = JE_pads[i].y;
				ts.z = JE_pads[i].z;
				JE_pads[i].pad->Load( &ts );
			}
			else
				JE_pads[i].pad->SetPosition( JE_pads[i].x, JE_pads[i].y, JE_pads[i].z, 0.0f, false );
		//push out first pad
		JE_pads[0].pad->PushToWorld( m_Unit->GetMapMgr() );
		//start monitoring progress
		JE_next_pad_id = 1;
		//set our event state 
		JumpEventHolders[ JumpEventSlot ].EventState = JES_WAITING_ON_PLAYER_MOVE;
		//start monitoring player progress
		JE_Cronometer = GetTickCount();
	}
	//monitor if player jumped on next pad
	if( JumpEventHolders[ JumpEventSlot ].EventState == JES_WAITING_TO_START 
		&& JE_pads[JE_next_pad_id].pad != NULL	//sanity check
		)
	{
		float dist_x = abs(JE_pads[JE_next_pad_id].x - Plr->GetPositionX());
		float dist_y = abs(JE_pads[JE_next_pad_id].y - Plr->GetPositionY());
		float dist_z = abs(JE_pads[JE_next_pad_id].z - Plr->GetPositionZ());
		if( dist_x + dist_y + dist_z < JUMP_PAD_ACCEPTED_PLAYER_POSITION_ERROR )
		{
			if( JE_next_pad_id == JE_pads_used_now )
			{
				Event_Reward_Winner_Player( Plr, GetTickCount()-JE_Cronometer );
				JumpEventHolders[ JumpEventSlot ].PlayerGUID = 0;
				JumpEventHolders[ JumpEventSlot ].EventState = JES_NOTHING_INTELIGENT;
				DesPawnPads();
			}
			else
			{
				JE_next_pad_id++;
				JE_pads[0].pad->PushToWorld( m_Unit->GetMapMgr() );
			}
		}
	}
}

//this NPC is spawned in the midle of the arena to do everything :P
class JumpEventsupervisor : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(JumpEventsupervisor);

	//constructor
    JumpEventsupervisor(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
		JumpEventSupervisor *new_interface = new JumpEventSupervisor;
		new_interface->Init(_unit,AITYPE_SOCIAL,MOVEMENTTYPE_NONE);
		_unit->ReplaceAIInterface( (AIInterface *) new_interface );

		//make us friendly
		_unit->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, SUPERVISOR_FACION );
		_unit->_setFaction();

		//make us imune to every spell we know of :P
		for(uint32 i=0;i<7;i++)
			_unit->SchoolImmunityList[i] = 999;
		for(uint32 i=0;i<31;i++)
			_unit->MechanicsDispels[i] = 999;

		//make him an NPC who can talk
		_unit->SetUInt32Value( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP );

		//make him normal 
		_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, SUPERVIZOR_MAX_HEALTH );
    }
};

void SetupJumpEventSupervisorIngame(ScriptMgr * mgr)
{
	//supervize the 
    mgr->register_creature_script(CREATURE_ENTRY_FOR_SUPERVIZOR, &JumpEventsupervisor::Create);
}
