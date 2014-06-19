#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"
//extern FILE *config_file;
uint32		tournament_supervisor_guid = 0;
Unit		*tournament_supervisor = NULL;
uint32		spectator_seet_refresh_stamp = 0;
uint64		spectator_guids[NUMBER_OF_SPECTATOR_SEETS];
char		team1name[ MAX_STRING_LENGTH ];
char		team2name[ MAX_STRING_LENGTH ];
uint32		cur_tournament_id = 1;
uint32		tournament_started = 0;	// only have spectators if there is a tournament ongoing

void MakeUnitSpectator(Unit *target, uint8 porttofight)
{
	//no supervisor was created. Don't bother with the fight :P
	if( !target || !tournament_supervisor )
		return;

	//no reason to double add him
	if( IsPlayerSpectator( target->GetGUID() ) )
		return;

	if( target->IsPlayer() )
	{	
		Player *ptarget = (Player *)target;
		//apply spectator aura
/*		Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
		spell->Init(this, spellInfo ,true, NULL);
		spell->pSpellId = SPECTATOR_AURA_ID; //just make sure it does not get saved on logout
		SpellCastTargets targets( tournament_supervisor_guid );
		spell->prepare( &targets );*/

		//remove auras to make sure they will not influence other players
		for(uint32 buffitr=0;buffitr<MAX_AURAS;buffitr++)
			if( ptarget->m_auras[ buffitr ] )
				ptarget->m_auras[ buffitr ]->Remove();

		//if he is dead then revive him
		if( ptarget->IsDead() )
			ptarget->ResurrectPlayer();

		//full health
		ptarget->SetUInt32Value(UNIT_FIELD_HEALTH, ptarget->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );

		//make them be able to see the arena. They can't unfocus this until logout
		if( tournament_supervisor )
			ptarget->SetUInt64Value(PLAYER_FARSIGHT, tournament_supervisor->GetGUID() );

		//add flags so player can not move cast spell or do anything
		ptarget->SetUInt32Value( UNIT_FIELD_FLAGS, ptarget->GetUInt32Value( UNIT_FIELD_FLAGS ) | SET_SPECTATOR_UNIT_FLAGS );

		//rooted (can not move)
		ptarget->Root();

		//make sure he could not find a way to escape lock
		ptarget->m_silenced++;
		ptarget->m_rooted++;

		//set fly cheat so they do not fall
		WorldPacket fly(SMSG_MOVE_SET_CAN_FLY, 13);
		ptarget->m_setflycheat = true;
		fly << target->GetNewGUID();
		fly << uint32(2);
		ptarget->SendMessageToSet(&fly, true);

		//find a spectator seet for him
		for( uint32 i = 0; i < NUMBER_OF_SPECTATOR_SEETS; i++ )
			if( spectator_guids[i] == 0 || spectator_guids[i] == ptarget->GetGUID() )
			{
				spectator_guids[i] = ptarget->GetGUID();
				break;
			}

		//get him near the arena
		if( porttofight )
//			ptarget->EventTeleport( TOURNAMENT_SPECTATOR_TEL_MAP, TOURNAMENT_SPECTATOR_TEL_X, TOURNAMENT_SPECTATOR_TEL_Y, TOURNAMENT_SPECTATOR_TEL_Z );		
//			ptarget->EventTeleport( tournament_supervisor->GetMapId(), tournament_supervisor->GetPositionX(), tournament_supervisor->GetPositionY(), tournament_supervisor->GetPositionZ() - 10 );		
			if(!sEventMgr.HasEvent(ptarget, EVENT_PLAYER_TELEPORT))
//				sEventMgr.AddEvent(ptarget, &Player::EventTeleport, tournament_supervisor->GetMapId(), tournament_supervisor->GetPositionX(), tournament_supervisor->GetPositionY(), tournament_supervisor->GetPositionZ() - 10, EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				sEventMgr.AddEvent(ptarget, &Player::EventTeleport, tournament_supervisor->GetMapId(), tournament_supervisor->GetPositionX(), tournament_supervisor->GetPositionY(), tournament_supervisor->GetPositionZ(), EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		//make sure he is muted 
//		if( ptarget->GetSession() )
//			ptarget->GetSession()->m_muted = UNIXTIME + MUTE_SPECTATORS_DISTURBING_FIGHT; 

		//make them small to not disturb the fight
		ptarget->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f );

		//make them imune in case someone decides to attack them
		for(uint32 i=0;i<7;i++)
			ptarget->SchoolImmunityList[i]++;
		for(uint32 i=0;i<31;i++)
			ptarget->MechanicsDispels[i]++;

		//remove PVP just in case
		ptarget->RemovePvPFlag();

		//make sure he understands the rulez
		ptarget->BroadcastMessage( TOURNAMENT_SPECTATOR_TELEPORT_NPC_HELP_VIEW );
	}
	else
	{
		//maybe just kill it ? :D
	}
}

void RemoveUnitSpectator(Unit *target, uint8 porthome)
{
	if( !target )
		return;
	if( target->IsPlayer() )
	{		
		Player *ptarget = (Player *)target;
		ptarget->SetUInt64Value(PLAYER_FARSIGHT, 0 );

		//add flags so player can not move cast spell or do anything
		ptarget->SetUInt32Value( UNIT_FIELD_FLAGS, ptarget->GetUInt32Value( UNIT_FIELD_FLAGS ) & ~SET_SPECTATOR_UNIT_FLAGS );

		//rooted (can not move)
		ptarget->Unroot();

		//make sure he could not find a way to escape lock
		if( ptarget->m_silenced > 0 )
			ptarget->m_silenced--;
		if( ptarget->m_rooted > 0 )
			ptarget->m_rooted--;

		//remove fly cheat
		WorldPacket fly(836, 13);
		ptarget->m_setflycheat = false;
		fly << target->GetNewGUID();
		fly << uint32(5);
		ptarget->SendMessageToSet(&fly, true);

		//find a spectator seet for him
		for( uint32 i = 0; i < NUMBER_OF_SPECTATOR_SEETS; i++ )
			if( spectator_guids[i] == ptarget->GetGUID() )
			{
				spectator_guids[i] = 0;
				break;
			}

		//remove mute in case he still has it 
		if( ptarget->GetSession() )
			ptarget->GetSession()->m_muted = 0; 

		//normal penis size
		ptarget->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f );

		//remove imunity
		for(uint32 i=0;i<7;i++)
			ptarget->SchoolImmunityList[i] = 0;
		for(uint32 i=0;i<31;i++)
			ptarget->MechanicsDispels[i] = 0;

		//reset faction
		if( ptarget->GetCreateInfo() )
			ptarget->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, ptarget->GetCreateInfo()->factiontemplate );

		//and port home
		if( porthome )
//			ptarget->EventTeleport( TOURNAMENT_SPECTATOR_RET_MAP, TOURNAMENT_SPECTATOR_RET_X, TOURNAMENT_SPECTATOR_RET_Y, TOURNAMENT_SPECTATOR_RET_Z );
			if(!sEventMgr.HasEvent(ptarget, EVENT_PLAYER_TELEPORT))
				sEventMgr.AddEvent(ptarget, &Player::EventTeleport, TOURNAMENT_SPECTATOR_RET_MAP, TOURNAMENT_SPECTATOR_RET_X, TOURNAMENT_SPECTATOR_RET_Y, TOURNAMENT_SPECTATOR_RET_Z, EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void RefreshSpectators()
{
	//no function flood please
	if( getMSTime() < spectator_seet_refresh_stamp )
		return;

	spectator_seet_refresh_stamp = getMSTime() + SPECTATOR_SEET_REFRESH_INTERVAL;

	//see who is still using their seet
	uint32 seets_taken = 0;
	for( uint32 i = 0; i < NUMBER_OF_SPECTATOR_SEETS; i++ )
		if( spectator_guids[i] != 0 )
		{
			Player *chr = objmgr.GetPlayer( spectator_guids[ i ] );
			//check if player is still in spectator position
			if( !chr )
				spectator_guids[i] = 0;
			else 
				seets_taken++;
		}
#ifdef WRITE_DEBUG_INFO_TO_LOG
sLog.outDebug("Tournament SPectator Supervisor DEBUG : seats taken atm : %u ",seets_taken);
#endif
}

uint8 IsPlayerSpectator(uint32 pguid)
{
	for( uint32 i = 0; i < NUMBER_OF_SPECTATOR_SEETS; i++ )
		if( spectator_guids[i] == pguid )
			return 1;
	return 0;
}

bool IsInrangeSQ( Object *t1, Object *t2,float sq_r)
{
   if( !t1 || !t2 )
	  return false;

   float distx=t1->GetPositionX()-t2->GetPositionX();
   float disty=t1->GetPositionY()-t2->GetPositionY();
   float dist = distx*distx + disty*disty; 
   return dist <= sq_r;
}