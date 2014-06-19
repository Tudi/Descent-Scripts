#include "StdAfx.h"
#include "Setup.h"

void ConvertToStaticSpawn( Creature *c )
{
	if( c && c->static_object == false )
	{
		c->static_object = true; //do not go out of visual range
		c->GetMapMgr()->PushStaticObject( c );
//		uint32 x = c->GetMapMgr()->GetPosX(c->GetPositionX());
//		uint32 y = c->GetMapMgr()->GetPosY(c->GetPositionY());
//		c->GetMapMgr()->UpdateCellActivity( x, y, 1, 2 );
	}
}

#define GOLD_GAIN_INTERVAL		1000
#define GOLD_GAIN_BANK_SEC		1

//lvl : generate gold for my faction
//lvl : lvl + killing mobs awards personal gold
//lvl : lvl + kill streak reward
//lvl : lvl + gold auto balancing
class BankBuilding : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION( BankBuilding );

    BankBuilding(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		ConvertToStaticSpawn( _unit );
		RegisterAIUpdateEvent( GOLD_GAIN_INTERVAL );
		if( _unit->GetAIInterface() )
		{
/*			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true; */
//			_unit->bInvincible = true;
		}
		_unit->m_canRegenerateHP = false;
	}
    void AIUpdate()
    {
		if( _unit->GetMapMgr() )
		{
			for( PlayerStorageMap::iterator itr = _unit->GetMapMgr()->m_PlayerStorage.begin(); itr != _unit->GetMapMgr()->m_PlayerStorage.end(); itr++ )
				if( GetTeamAnyMask( itr->second ) == GetTeamAnyMask( _unit ) )
					itr->second->ModGold( GOLD_GAIN_BANK_SEC );
		}
	}
};

////////////////////////////////////////////////

#define KING_OBJECT_MONITOR_UPDATE_INTERVAL		1000

class King : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION( King );
	
   uint32 SpawnStamp;
    King(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		ConvertToStaticSpawn( _unit );
		RegisterAIUpdateEvent( KING_OBJECT_MONITOR_UPDATE_INTERVAL );
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->m_canRangedAttack = true;
		}
//		_unit->m_canRegenerateHP = false;
		SpawnStamp = GetTickCount();
	}
	void DecreaseKillCountForGroup( uint32 MyTeamMask )
	{
		std::list< QuestKillObjective* >::iterator itr;
		for( itr = QuestObjectiveList.begin(); itr != QuestObjectiveList.end(); itr++ )
		{
			if( ( (*itr)->Group & MyTeamMask ) != MyTeamMask )
				continue;

			//try to spawn this objective
			if( (*itr)->KillCountNow >= (*itr)->KillCountRequired )
			{
				(*itr)->KillCountTotal += (*itr)->KillCountRequired;
				(*itr)->KillCountNow -= (*itr)->KillCountRequired;
			}
		}
	}
	void SendMessageToTeam( char *Msg )
	{
		for( PlayerStorageMap::iterator itr = _unit->GetMapMgr()->m_PlayerStorage.begin(); itr != _unit->GetMapMgr()->m_PlayerStorage.end(); itr++ )
			if( GetTeamAnyMask( itr->second ) == GetTeamAnyMask( _unit ) )
			{
				itr->second->BroadcastMessage( Msg );
			}
	}
	void PingMinimapForTeam( float x, float y, char *Msg )
	{
		for( PlayerStorageMap::iterator itr = _unit->GetMapMgr()->m_PlayerStorage.begin(); itr != _unit->GetMapMgr()->m_PlayerStorage.end(); itr++ )
			if( GetTeamAnyMask( itr->second ) == GetTeamAnyMask( _unit ) )
			{
				itr->second->Gossip_SendPOI( x, y, 6, 6, 0, Msg );
			}
	}
    void AIUpdate()
    {
		if( _unit->GetMapMgr() )
		{
			uint32 TickNow = GetTickCount();

			//check and try to spawn new objectives
			std::set< uint32 > AlreadyPrinted;
			uint32 MyTeamMask = GetTeamAnyMask( _unit );
			std::list< QuestKillObjective* >::iterator itr;
			for( itr = QuestObjectiveList.begin(); itr != QuestObjectiveList.end(); itr++ )
			{
				if( ( (*itr)->Group & MyTeamMask ) != MyTeamMask )
					continue;

				//try to spawn this objective
				if( (*itr)->KillCountNow >= (*itr)->KillCountRequired )
				{
					if( _unit->GetMapMgr()->GetCreatureByEntry( (*itr)->Entry ) != NULL )
						continue;
					_unit->GetMapMgr()->CreateCreature( (*itr)->Entry, true, (*itr)->Spawn.x, (*itr)->Spawn.y, (*itr)->Spawn.z, (*itr)->Spawn.o );
					DecreaseKillCountForGroup( MyTeamMask );
					char buff[BROADCAST_MAX_MSG_LEN];
					sprintf_s( buff, BROADCAST_MAX_MSG_LEN, "Objective %s has been completed : %s", (*itr)->Name, (*itr)->Description );
					SendMessageToTeam( buff );
					PingMinimapForTeam( (*itr)->Spawn.x, (*itr)->Spawn.y, (*itr)->Name );
				}
			}

			//check for dead players and resurrect them
			for( PlayerStorageMap::iterator itr = _unit->GetMapMgr()->m_PlayerStorage.begin(); itr != _unit->GetMapMgr()->m_PlayerStorage.end(); itr++ )
				if( GetTeamAnyMask( itr->second ) == GetTeamAnyMask( _unit ) 
					&& itr->second->isAlive() == false
					)
				{
					//do we have a timer yet ?
					int64 *ResStamp = itr->second->GetCreateIn64Extension( EXTENSION_ID_CUSTOM_BG_RESURRECT_STAMP );
					if( *ResStamp < TickNow && *ResStamp + 2 * KING_OBJECT_MONITOR_UPDATE_INTERVAL > TickNow )
					{
						itr->second->ResurrectPlayer();
						itr->second->SafeTeleportDelayed( _unit->GetMapId(), _unit->GetInstanceID(), _unit->GetPosition() );
					}
					//queue for resurrect
					else
					{
						int64 TimePassed = TickNow - SpawnStamp;
						*ResStamp = TickNow + MIN( RESPAWN_MAX_WAIT, TimePassed / 10 );	//100 minutes play = 10 minute respawn
					}
				}
		}
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		for( PlayerStorageMap::iterator itr = _unit->GetMapMgr()->m_PlayerStorage.begin(); itr != _unit->GetMapMgr()->m_PlayerStorage.end(); itr++ )
		{
			itr->second->BroadcastMessage( "King has died. Match has ended" );
			if( GetTeamAnyMask( itr->second ) != GetTeamAnyMask( _unit ) )
			{
				itr->second->ModCurrencyCount( PLAYER_WIN_CURRENCY_TYPE, PLAYER_WIN_CURRENCY_AMT );
				LocationVector ret( itr->second->m_bgEntryPointX, itr->second->m_bgEntryPointY, itr->second->m_bgEntryPointZ );
				itr->second->SafeTeleportDelayed( itr->second->m_bgEntryPointMap, 0, ret );
			}
		}
    }
};

////////////////////////////////////////////////

#define HP_GAIN_INTERVAL	10000
#define HP_GAIN_AMT_FLAT	1
#define HP_GAIN_PCT			1

//lvl : generate HP for my faction
//lvl : lvl + generate HP pct for my faction
class HospitalBuilding : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION( HospitalBuilding );

    HospitalBuilding(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		ConvertToStaticSpawn( _unit );
		RegisterAIUpdateEvent( HP_GAIN_INTERVAL );
		if( _unit->GetAIInterface() )
		{
/*			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true; */
//			_unit->bInvincible = true;
		}
		_unit->m_canRegenerateHP = false;
	}
    void AIUpdate()
    {
		if( _unit->GetMapMgr() )
		{
			for( PlayerStorageMap::iterator itr = _unit->GetMapMgr()->m_PlayerStorage.begin(); itr != _unit->GetMapMgr()->m_PlayerStorage.end(); itr++ )
				if( GetTeamAnyMask( itr->second ) == GetTeamAnyMask( _unit ) )
					itr->second->ModHealth( HP_GAIN_AMT_FLAT );
		}
	}
};

////////////////////////////////////////////////

void SetupBuildings( ScriptMgr* mgr )
{
	mgr->register_creature_script( 1, &BankBuilding::Create);	//
	mgr->register_creature_script( 1, &HospitalBuilding::Create);	//
	mgr->register_creature_script( 1, &King::Create);	//
}