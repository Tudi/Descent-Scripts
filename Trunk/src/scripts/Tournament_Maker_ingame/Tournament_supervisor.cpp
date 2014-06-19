#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"

#ifdef WIN32
  #pragma warning(disable:4305)        // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

class Tournament_Supervisor : public AIInterface
{
public:
	void				Init(Unit *un, AIType at, MovementType mt);
	void				Update(uint32 p_time);
private:
};

void Tournament_Supervisor::Init(Unit *un, AIType at, MovementType mt)
{
	AIInterface::Init(un,at,mt);//run the original init and we add our custom stuff too
}

void Tournament_Supervisor::Update(uint32 p_time)
{
/*	//method to pass commands to tournament via any supervizor
	debug_commands = m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH );

	//make him normal 
	m_Unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, SUPERVIZOR_MAX_HEALTH );*/

	//see if we need to turn anyone into a spectator

	//update spam will be filtered inside function
	UpdateTournaments( );

	//check for spectators and update their aura
	InrangeLoopExitAutoCallback AutoLock;
	for( InRangeSetRecProt::iterator itr = m_Unit->GetInRangeSetBegin( AutoLock ); itr != m_Unit->GetInRangeSetEnd(); ++itr)
	{
		if( !(*itr) || !(*itr)->IsInWorld() )
			continue;
		if( (*itr)->IsPlayer() )
		{
			if( IsPlayerSpectator( (Player*)(*itr) ) == false )
				continue;
		}
		else if( (*itr)->IsUnit() && (*itr)->GetUInt32Value( UNIT_FIELD_CREATEDBY ) )
		{
			Unit *Owner = m_Unit->GetMapMgr()->GetUnit( (*itr)->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
			if( Owner && Owner->IsPlayer() && IsPlayerSpectator( (Player*)(Owner) ) == false )
				continue;
			//what about totems that summon guardians ? Call GM ?
		}
		if( (*itr)->IsUnit() && ((Unit *)(*itr))->HasAura( SPECTATOR_AURA_ID, 0, AURA_SEARCH_NEGATIVE ) == false && IsUnitInsideAnyBattleRing( (*itr)->GetMapId(), (*itr)->GetPositionX(), (*itr)->GetPositionY() ) == true )
		{
			SpellEntry *spellInfo = dbcSpell.LookupEntry( SPECTATOR_AURA_ID );
			Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
			SpellCastTargets targets( (*itr)->GetGUID() );
			spell->Init((*itr), spellInfo ,true, NULL);
			spell->prepare(&targets);
		}
	}
}

//this NPC is spawned in the midle of the arena to do everything :P
class Tournament_supervisor : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Tournament_supervisor);

	//constructor
    Tournament_supervisor(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
		Tournament_Supervisor *new_interface = new Tournament_Supervisor;
		new_interface->Init(_unit,AITYPE_SOCIAL,MOVEMENTTYPE_NONE);
		_unit->ReplaceAIInterface( (AIInterface *) new_interface );

		//make us friendly
		_unit->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, SUPERVISOR_FACION );
		_unit->_setFaction();

		//make us imune to every spell we know of :P
		for(uint32 i=0;i<7;i++)
			_unit->SchoolImmunityAntiEnemy[i] = 999;
		for(uint32 i=0;i<31;i++)
			_unit->MechanicsDispels[i] = 999;

		//make him an NPC who can talk
		_unit->SetUInt32Value( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP );

		//put us in an IDLE state ( funcky )
//		_unit->SetUInt32Value(SUPERVISOR_VARIABLE_STATE_STORE,SUPERVISOR_HP_TO_IDLE);

		if( IsUnitInsideAnyBattleRing( _unit->GetMapId(), _unit->GetPositionX(), _unit->GetPositionY() ) )
		{
			//make him impossible to select or attack by normal players
			_unit->SetUInt32Value( UNIT_FIELD_FLAGS, _unit->GetUInt32Value( UNIT_FIELD_FLAGS ) | SUPERVISOR_UNIT_FLAGS );
			//make him invisible 
			_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, 15435 );
			//make us small. Like really small :P
			_unit->SetFloatValue( OBJECT_FIELD_SCALE_X , 0.01f );
		}

		//make him normal 
		_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, SUPERVIZOR_MAX_HEALTH );
    }
};

void SetupTournamentSupervisorIngame(ScriptMgr * mgr)
{
	//create an aura that acts as spectator mode
	CreateTournametSpectatorAura( mgr );

	//initialize global variables 
	cur_tournament_id = 0;
	for(uint32 i=0;i<TOURNAMENT_TYPE_COUNT;i++)
	{
		Tournaments[ i ].id = cur_tournament_id++;
		Tournaments[ i ].type = i;
		Tournaments[ i ].status = TS_NOT_STARTED;
		Tournaments[ i ].arena_range_sq = 75 * 75;	//gurubashi had 80 yards and if we count half from center that is enough + spectator spell range
		Tournaments[ i ].max_players_in_team = teamsize_limitations[ i ];
		Tournaments[ i ].next_state_change_stamp = 0;
		for(uint32 j=0;j<MAX_TEAMS_PER_TOURNAMENT;j++)
		{
			Tournaments[ i ].teams_registered[ j ].is_used = 0;
		}
	}
	//arena floor
	Tournaments[ TOURNAMENT_TYPE_2_VS_2 ].map_id = 530;
	Tournaments[ TOURNAMENT_TYPE_2_VS_2 ].center_x = -2043.0f;
	Tournaments[ TOURNAMENT_TYPE_2_VS_2 ].center_y = 6658.0f;
	Tournaments[ TOURNAMENT_TYPE_2_VS_2 ].center_z = 14.0f;
	//the maul
	Tournaments[ TOURNAMENT_TYPE_3_VS_3 ].map_id = 1;
	Tournaments[ TOURNAMENT_TYPE_3_VS_3 ].center_x = -3762.0f;
	Tournaments[ TOURNAMENT_TYPE_3_VS_3 ].center_y = 1091.0f;
	Tournaments[ TOURNAMENT_TYPE_3_VS_3 ].center_z = 132.0f;
	//cicle of blood 
	Tournaments[ TOURNAMENT_TYPE_4_VS_4 ].map_id = 530;
	Tournaments[ TOURNAMENT_TYPE_4_VS_4 ].center_x = 2839.0f;
	Tournaments[ TOURNAMENT_TYPE_4_VS_4 ].center_y = 5930.0f;
	Tournaments[ TOURNAMENT_TYPE_4_VS_4 ].center_z = 12.0f;
	//gurubashi arena 
	Tournaments[ TOURNAMENT_TYPE_5_VS_5 ].map_id = 0;
	Tournaments[ TOURNAMENT_TYPE_5_VS_5 ].center_x = -13205.0f;
	Tournaments[ TOURNAMENT_TYPE_5_VS_5 ].center_y = 275.0f;
	Tournaments[ TOURNAMENT_TYPE_5_VS_5 ].center_z = 22.0f;

	//supervize the 
    mgr->register_creature_script(CREATURE_ENTRY_FOR_IGTS_SUPERVIZOR, &Tournament_supervisor::Create);

/*
#replace into npc_gossip_textid values( 123459, 100000);
replace into npc_text (entry,text0_0) values (100000,"Tournaments are organized battles where weakest teams get eliminated. Winner team players get a new reward automatically.$BEach tournament has a limit to maximum team size(2,3,4,5).$BMinimum is 1 player / team.$BYou may only queue for 1 tournament type at a time.$BIf you join BG you are removed from tournament queue automatically.$BOther players may join your team by entering same team name as you did.$BThere is no level limitation for players.$BTournament status can be seen in queue menu.$BTournament starts as 10 teams joined the queue.You get ported to fight/watch automatically.$BYou may join as group if you are group leader");

REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `type`, `male_displayid`) VALUES
(123459, 'Tournament Organizer', 'are you ready boy ?', 'PVP', 7, 12679);

REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`,  `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`) VALUES
(123459, 96, 96, 35, 100000, 100000, 1.25, 0, 1000, 0, 100, 200, 0, 0, 0, 360000000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 1.8, 0.5, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);

REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,-1999.69,6677.65,13.0629,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,-2045.77,6698.02,13.843,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,-2079.9,6682.41,13.1593,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,-2085.75,6643.87,13.6843,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,-2065.48,6619.56,13.6986,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,-2032.03,6612.68,13.0967,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,-2008.26,6628.01,13.0744,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,-2003.29,6654.1,13.0699,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,-2038.14,6654.27,13.0536,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,2840.1,5928,0.891768,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,2887.41,5930.85,4.89608,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,2870.1,5901.99,12.1263,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,2842.51,5878.26,4.85697,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,2814.44,5901.18,4.07332,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,2784.52,5926.2,3.99697,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,2804.9,5954.69,12.5984,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,2821.49,5982.03,4.68684,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,530,2856.7,5964.72,4.62095,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,1,-3747,1089,131.969,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,1,-3683.92,1091.28,131.969,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,1,-3707.42,1061.31,132.771,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,1,-3750.99,1060.33,132.379,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,1,-3790.27,1061.49,131.993,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,1,-3815.53,1085.27,131.969,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,1,-3795.79,1122.84,131.961,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,1,-3752.54,1125.94,132.043,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,1,-3710.81,1124.5,131.969,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,0,-13190,273,22,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,0,-13162.3,273,21.8571,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,0,-13183.1,240.64,21.8571,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,0,-13214.6,239.263,21.8571,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,0,-13238.5,259.021,21.8571,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,0,-13239.1,292.133,21.8571,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,0,-13208.9,313.047,21.8571,35);
REPLACE INTO creature_spawns (entry,map,position_x,position_y,position_z,faction)VALUES(123459,0,-13183.2,307.106,21.8571,35);
*/
}
