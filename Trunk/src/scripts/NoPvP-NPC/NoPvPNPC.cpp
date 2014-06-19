/*                    ______     _   _     _       _
    //\\  //\\      ||-----||	| | | |	 _| |_   _| |_
   //  \\//  \\     ||_____||	| | | |	|_   _| |_   _|
  //          \\    ||------	| |_| |	  |_|     |_|
 //            \\   ||          |_____|


 * MPU++ Scripts for Arcemu MMORPG Server
 * Copyright (C) 2009 MPU++ Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"
#pragma warning(disable:4561)

#define NoPvP_NPC 123458
#define ZoneDefender_NPC 123460

class NoPVPAI : public CreatureAIScript
{
public:
    NoPVPAI(Creature* pCreature) : CreatureAIScript(pCreature) {}
	static CreatureAIScript * Create(Creature * c) { return new NoPVPAI(c); }
	
	void OnLoad()
    {
//		_unit->m_invisFlag = 10;
//		_unit->m_invisible = true;
//		_unit->UpdateVisibility();
		for(uint32 i=0;i<7;i++)
			_unit->SchoolImmunityAntiEnemy[i] = 999;
		for(uint32 i=0;i<31;i++)
			_unit->MechanicsDispels[i] = 999;
		//make him impossible to select or attack by normal players
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, _unit->GetUInt32Value( UNIT_FIELD_FLAGS ) | UNIT_FLAG_UNKNOWN_8 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 );

		RegisterAIUpdateEvent( 500 ); //unit update interval is 500 MS, why spam more ?
    }

	void AIUpdate()
	{		
		//adjust this to set range where he will look around himself to see if anyone fights
		static float range = 120;	//kinda like 1 visibility range = 60 yards
		PlayerStorageMap::iterator itr = _unit->GetMapMgr()->m_PlayerStorage.begin();
		PlayerStorageMap::iterator end = _unit->GetMapMgr()->m_PlayerStorage.end();
		for( ; itr != end; ++itr)
			if( itr->second )	//no way ?
			{
				Player* Plr = itr->second;
				//if player is in range 
				if( _unit->isInRange( Plr, range ) )
				{
					// method 1
					//in case player is dead or health is low we make him healthy and tasty. We are protecting them from getting killed
					if( Plr->IsDead() )
					{
						Plr->RemoteRevive();
						continue;
					}
					if( Plr->GetUInt32Value( UNIT_FIELD_HEALTH ) < Plr->GetUInt32Value( UNIT_FIELD_MAXHEALTH )
						&& Plr->IsInWorld() == true 
						&& Plr->GetUInt32Value( UNIT_FIELD_HEALTH ) != 0 
						&& Plr->isAlive() == true 
						)
					{
						Plr->SetUInt32Value( UNIT_FIELD_HEALTH, Plr->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
						if( Plr->HasAura( 6724 ) == false )
							Plr->CastSpell(Plr->GetGUID(), 6724, true);
					}
					//not sure if this is a bright idea, but it helps peole avoid the fear or mind crontrol abuse
					for(uint32 x=MAX_POSITIVE_AURAS;x<Plr->m_auras_neg_size;x++)
						if( Plr->m_auras[x] )
							Plr->m_auras[x]->Remove();
					/**/
					// method 2
					//in case he is in combat with another player -> this would be good if they do not port with combat flag stuck on them
/*					if( Plr->CombatStatus.IsInCombat() )
					{
						sChatHandler.RedSystemMessageToPlr(Plr, "PVP is forbidden here.");
						Plr->DealDamage(Plr, Plr->GetUInt32Value(UNIT_FIELD_HEALTH),0,0,0);
						Plr->KillPlayer();
					}
					/**/

/*
					// method 3
					//this would be good if sanctuary zone would be detected correctly 
					if( Plr->IsPvPFlagged() )
					{
						Plr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP);
						sChatHandler.RedSystemMessageToPlr(Plr, "PVP is forbidden. Your PVP flag is cleared.");
						Plr->DealDamage(Plr, Plr->GetUInt32Value(UNIT_FIELD_HEALTH),0,0,0);
						Plr->KillPlayer();
					}
					*/
				}
			}
	}

    void Destroy()
    {
        delete [] this;
    }
};

#define ZONE_DEFENDER_MAX_MONITORED_UNIT_COUNT		45
#define ZONE_DEFENDER_TAKE_ACTIONS_AFTER_DURATION	3000
#define TOWER_SUPPORT_BASE_RANGE					15

struct UnitWarnStateStore
{
   Unit		*unit;       //to identify who we saved state for
   uint32   WarnedAtStamp;	 //last time the health was like this
};

class ZoneDefender : public AIInterface
{
public:
   void         Init(Unit *un, AIType at, MovementType mt);
   void         Update(uint32 p_time);
private:
   UnitWarnStateStore   MonitoredUnits[ ZONE_DEFENDER_MAX_MONITORED_UNIT_COUNT ];
   float		spawn_x,spawn_y,spawn_z;
   float 		cur_range_sq;
   uint32		next_update;	
};

void ZoneDefender::Init(Unit *un, AIType at, MovementType mt)
{
   //run the original init and we add our custom stuff too
   AIInterface::Init(un,at,mt);
   //mark where we was spawned. Towers do not move
   spawn_x = un->GetPositionX();
   spawn_y = un->GetPositionY();
   spawn_z = un->GetPositionZ();
   cur_range_sq = TOWER_SUPPORT_BASE_RANGE * TOWER_SUPPORT_BASE_RANGE;
   memset( MonitoredUnits, 0, sizeof( MonitoredUnits ) );
   //somehow signal that we are a tower
   un->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_DEAD);
   un->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 9000000 );
   un->SetUInt32Value( UNIT_FIELD_HEALTH, 10000 );
   un->SetUInt32Value( UNIT_CHANNEL_SPELL, 50461 );
   un->SetUInt32Value( UNIT_FIELD_DISPLAYID, 11686 );
   next_update = 0;
   for(uint32 i=0;i<7;i++)
		un->SchoolImmunityAntiEnemy[i] = 999;
   for(uint32 i=0;i<31;i++)
		un->MechanicsDispels[i] = 999;
}

void ZoneDefender::Update(uint32 p_time)
{
	//spam update protection
	uint32 now_time = GetTickCount();
	if( next_update > now_time )
		return;
	next_update = now_time + 1000;

	//muhahaha, maxheal !
	m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH, m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );

	//sanity check
	if( m_Unit->GetMapMgr() == NULL )
		return;

   //in case they managed somehow to move the tower then just put it back. Ex : deathgrip
   if( spawn_x != m_Unit->GetPositionX() )
   {
      m_Unit->SetPosition( spawn_x,spawn_y,spawn_z,m_Unit->GetOrientation(), false );
//      WorldPacket data;
//      m_Unit->BuildHeartBeatMsg( &data );
//      m_Unit->SendMessageToSet( &data, false );
	  m_Unit->GetAIInterface()->StopMovement( 0 );
   }

	uint32 updated_slots[ ZONE_DEFENDER_MAX_MONITORED_UNIT_COUNT ];
	memset( updated_slots, 0, sizeof( updated_slots ) );
	//our max visible list
	InrangeLoopExitAutoCallback AutoLock;
	for( InRangeSetRecProt::iterator itr = m_Unit->GetInRangeSetBegin( AutoLock ); itr != m_Unit->GetInRangeSetEnd(); ++itr)
	{
		if( (*itr) == NULL 
			|| (*itr)->IsUnit() == false 
			|| (*itr)->IsInWorld() == false 
			|| (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH ) == 0 
			|| (*itr) == m_Unit
			|| static_cast<Unit *>(*itr)->isAlive() == false 
			|| IsInrange( m_Unit, (*itr), cur_range_sq ) == false
			)
			continue;

		//check if we are shielding this unit
		if( isHostile( m_Unit, (*itr) ) == true )
		{
			bool foundit = false;
			for( uint32 i=0;i<ZONE_DEFENDER_MAX_MONITORED_UNIT_COUNT;i++ )
				if( MonitoredUnits[i].unit == (*itr) )
				{
					//knock him back a few yards
					if( now_time <= MonitoredUnits[i].WarnedAtStamp + ZONE_DEFENDER_TAKE_ACTIONS_AFTER_DURATION )
					{
						if( (*itr)->IsPlayer() )
						{
							static_cast<Player*>(*itr)->SendKnockbackPacket( (float)cosf((*itr)->GetOrientation()+ 3.14f), (float)sinf((*itr)->GetOrientation()+ 3.14f), 20.0f, -15.0f );
//							static_cast<Player*>(*itr)->SendKnockbackPacket( (float)cosf((*itr)->GetOrientation()+ 3.14f), (float)sinf((*itr)->GetOrientation()+ 3.14f), -15.0f, 20.0f );
							static_cast<Player*>(*itr)->blinked = true;
							static_cast<Player*>(*itr)->SpeedCheatDelay( 5000 );
						}
					}
					foundit = true;
					break;
				}
			//not yet shielded -> add it
			if( foundit == false )
			   for( uint32 i=0;i<ZONE_DEFENDER_MAX_MONITORED_UNIT_COUNT;i++)
				  if( MonitoredUnits[i].unit == NULL )
				  {
					 MonitoredUnits[i].unit = static_cast<Unit*>(*itr);
					 if( static_cast<Unit*>(*itr)->IsPlayer() )
						 static_cast<Player*>(*itr)->BroadcastMessage("Please leave this protected place or prepare to be displaced");
					 MonitoredUnits[i].WarnedAtStamp = now_time;
					 updated_slots[ i ] = 1;
					 break;
				  }
		}
	}
	//remove units from our list that we are not shielded anymore
	for( uint32 i=0;i<ZONE_DEFENDER_MAX_MONITORED_UNIT_COUNT;i++)
		if( updated_slots[i] != 1 )
			MonitoredUnits[i].unit = NULL;
}

//this NPC is spawned in the midle of the arena to do everything :P
class AIZoneDefender : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(AIZoneDefender);

   //constructor
    AIZoneDefender(Creature* pCreature) : CreatureAIScript(pCreature)
    {
      //we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
      ZoneDefender *new_interface = new ZoneDefender;
      new_interface->Init(_unit,AITYPE_SOCIAL,MOVEMENTTYPE_NONE);
      _unit->ReplaceAIInterface( (AIInterface *) new_interface );
      _unit->m_canRegenerateHP = false;
   }
};

void SetupNoPVP(ScriptMgr* mgr)
{
//insert into `creature_names` (`entry`, `name`, `Subname`, `info_str`, `Flags1`, `type`, `Family`, `Rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `Leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `civilian`) values('123458','Nonpvp','WOWBEEZ boot','','0','7','0','4','0','0','21666','0','0','0','1','1','0','0','0','0','0','0','0','0','0');
//insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `modImmunities`) values('123458','100','100','35','700000','700000','1000000','0.01','12417','1000','0','10000','10000','0','0','0','3600000','0','0','0','0','0','0','0','1.5','0','0','1','0','0','0','2.5','8','14','0','0');
	mgr->register_creature_script(NoPvP_NPC, &NoPVPAI::Create);
//insert into `creature_names` (`entry`, `name`, `Subname`, `info_str`, `Flags1`, `type`, `Family`, `Rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `Leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `civilian`) values('123460','Zone defender','WOWBEEZ boot','','0','7','0','4','0','0','21666','0','0','0','1','1','0','0','0','0','0','0','0','0','0');
//insert into `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `modImmunities`) values('123460','100','100','35','700000','700000','1000000','4.0','0','1000','0','10000','10000','0','0','0','3600000','0','0','0','0','0','0','0','1.5','0','0','1','0','0','0','2.5','8','14','0','0');
//INSERT INTO `creature_spawns` (`id`, `entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes0`, `bytes1`, `bytes2`, `emote_state`, `standstate`, `mountdisplayid`, `slot1item`, `slot2item`, `slot3item`, `difficulty_mask`, `phase_mask`) VALUES
//(558299, 123460, 489, 1419, 1558.2, 342.751, 3.09055, 0, 11686, 11, 16642, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65535, 2147483647),
//(558300, 123460, 489, 1032.2, 1383.4, 340.939, 6.19679, 0, 11686, 29, 16642, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65535, 2147483647);
	mgr->register_creature_script(ZoneDefender_NPC, &AIZoneDefender::Create);
}

