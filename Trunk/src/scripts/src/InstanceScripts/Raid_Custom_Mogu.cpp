#include "StdAfx.h"
#include "Setup.h"

/*
Fight Controller is immune whole time of the fight
Test : 
- Test DPS single : move slowly close to boss, unless killed fast enough bos does large AOE
- Test DPS AOE : move slowly close to boss, unless killed fast enough bos does large AOE
- Test heal single slow : mob will scale it's dmg to do 30% tank HP / hit. Dmg should be real dmg instead fixed
- Test heal single fast : mob will scale it's dmg to do 5% tank HP / hit. Dmg should be real dmg instead fixed
- Tank taunt capability hard hit : agro random unit and attack
- Tank taunt capability fast hit : agro random unit and attack
- Test heal AOE : lots of units target random players above the others
- Test movement : create fire spots that people need to avoid
Story : soul hungerer wants to swallow players. Every time a player dies boss will periodically cast shadow nova that damages everyone
*/

//this is because people are stating BOSS can't be killed or imba
#define LOG_CREATURE_DEATH( Entry, Killer, Duration ) { \
		Unit *towner = Killer->GetTopOwner(); \
		if( towner->IsPlayer() ) \
		{ \
			QueryResult *result = WorldDatabase.Query(	"INSERT INTO BOSSKILLS ( Entry, Player, KillStamp, group_size, diff, Duration ) values ( %d, %d, %d, %d, %d, %d )", Entry, towner->GetGUID(), time(NULL), SafePlayerCast(towner)->GetGroup()?SafePlayerCast(towner)->GetGroup()->MemberCount():0, SafePlayerCast(towner)->GetInstanceDifficulty(), Duration ); \
			if( result ) delete result; \
		} \
	}
#define LOG_CREATURE_DEATH2( Entry, Duration ) { \
			QueryResult *result = WorldDatabase.Query(	"INSERT INTO BOSSKILLS ( Entry, KillStamp, Duration ) values ( %d, %d, %d )", Entry, time(NULL), Duration ); \
			if( result ) delete result; \
	}

#define MAP_EXTENSION_MOGU_ENCOUNTER_STORE	"Mogu Encounter Store"

struct MoguEncounterStoreStruct
{
	float	StrongMobDmg;
	float	WeakMobDmg;
	int32	StrongMobHeal;
	int32	WeakMobHeal;
	bool	IsTestingPhase;
};

#define MAX_DMG_SERVER_WILL_IGNORE					190000
#define MAX_HEALTH_SERVER_WILL_BUGOUT				800000000
#define MAX_HEAL_TO_PROTECT_FROM_BUGS				(25*MAX_DMG_SERVER_WILL_IGNORE)

//9 seconds to kill a player
#define STRONG_HEAL_EXPECTED_SURVIVE_SECONDS		120
#define STRONG_HEAL_EXPECTED_HEALTH_REDUCE_PCT		40
#define STRONG_HEAL_ATTACK_SPEED_SECONDS			3
#define STRONG_HEAL_ENTRY							111200

class StrongHealTester : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(StrongHealTester);

	int32	HitsDone;
	int32	DamageSum;
	StrongHealTester(Creature* pCreature) : CreatureAIScript(pCreature)	{	}
	void OnLoad()
	{
		RegisterAIUpdateEvent( STRONG_HEAL_ATTACK_SPEED_SECONDS * 1000 );
		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, STRONG_HEAL_ATTACK_SPEED_SECONDS * 1000 );
		HitsDone = 0;
		DamageSum = 0;
		_unit->SetFloatValue( UNIT_FIELD_MINDAMAGE, 20000 );
		_unit->SetFloatValue( UNIT_FIELD_MAXDAMAGE, 20000 );

		MoguEncounterStoreStruct *store = (MoguEncounterStoreStruct*)_unit->GetMapMgr()->GetExtension( MAP_EXTENSION_MOGU_ENCOUNTER_STORE );
		if( store && store->IsTestingPhase == false )
		{
			float NewDMG = MIN( store->StrongMobDmg, MAX_DMG_SERVER_WILL_IGNORE );
			_unit->SetFloatValue( UNIT_FIELD_MINDAMAGE, NewDMG );
			_unit->SetFloatValue( UNIT_FIELD_MAXDAMAGE, NewDMG );
			uint32 NewHealth = MIN( STRONG_HEAL_EXPECTED_SURVIVE_SECONDS * store->WeakMobHeal / 10, MAX_HEALTH_SERVER_WILL_BUGOUT );
			_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, NewHealth );
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
		}
		_unit->BaseAttackType = SCHOOL_SHADOW;
	}
	void AIUpdate()
	{
		if( _unit->GetAIInterface()->DamageDoneLastMeleeStrike != 0 && _unit->GetAIInterface()->GetMostHated() != NULL )
		{
			//statistical because some auras temporary might reduce or boost our / their dmg
			HitsDone++;
			DamageSum += MIN( _unit->GetAIInterface()->DamageDoneLastMeleeStrike, _unit->GetAIInterface()->GetMostHated()->GetMaxHealth() * 3 );
			_unit->GetAIInterface()->DamageDoneLastMeleeStrike = 0;
			int32 EffectiveDamage = DamageSum / HitsDone;

			int32 ExpectedDmg = _unit->GetAIInterface()->GetMostHated()->GetMaxHealth() * STRONG_HEAL_EXPECTED_HEALTH_REDUCE_PCT / 100;
			float DamageNow = _unit->GetFloatValue( UNIT_FIELD_MINDAMAGE );
			float NeededScale = ExpectedDmg / EffectiveDamage;
			float NewDamage = MIN( MAX_DMG_SERVER_WILL_IGNORE, MAX( DamageNow, NeededScale * DamageNow ) );

			_unit->SetFloatValue( UNIT_FIELD_MINDAMAGE, NewDamage );
			_unit->SetFloatValue( UNIT_FIELD_MAXDAMAGE, NewDamage );
		}
	} 
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		MoguEncounterStoreStruct *store = (MoguEncounterStoreStruct*)_unit->GetMapMgr()->GetExtension( MAP_EXTENSION_MOGU_ENCOUNTER_STORE );
		if( store )
			store->StrongMobDmg = MAX( store->StrongMobDmg, _unit->GetFloatValue( UNIT_FIELD_MINDAMAGE ) );
    }
};

//4 second to kill
#define FAST_HEAL_EXPECTED_SURVIVE_SECONDS			60
#define FAST_HEAL_EXPECTED_HEALTH_REDUCE_PCT		15
#define FAST_HEAL_ATTACK_SPEED_SECONDS				1	//leave this to 1 sec !
#define FAST_HEAL_ENTRY								111201

class HOTHealTester : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(HOTHealTester);

	int32	HitsDone;
	int32	DamageSum;
	HOTHealTester(Creature* pCreature) : CreatureAIScript(pCreature)	{	}
	void OnLoad()
	{
		RegisterAIUpdateEvent( FAST_HEAL_ATTACK_SPEED_SECONDS * 1000 );
		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, FAST_HEAL_ATTACK_SPEED_SECONDS * 1000 );
		HitsDone = 0;
		DamageSum = 0;
		_unit->SetFloatValue( UNIT_FIELD_MINDAMAGE, 10000 );
		_unit->SetFloatValue( UNIT_FIELD_MAXDAMAGE, 10000 );

		MoguEncounterStoreStruct *store = (MoguEncounterStoreStruct*)_unit->GetMapMgr()->GetExtension( MAP_EXTENSION_MOGU_ENCOUNTER_STORE );
		if( store && store->IsTestingPhase == false )
		{
			float NewDMG = MIN( store->WeakMobDmg, MAX_DMG_SERVER_WILL_IGNORE );
			_unit->SetFloatValue( UNIT_FIELD_MINDAMAGE, NewDMG );
			_unit->SetFloatValue( UNIT_FIELD_MAXDAMAGE, NewDMG );
			uint32 NewHealth = MIN( FAST_HEAL_EXPECTED_SURVIVE_SECONDS * store->WeakMobHeal / 10, MAX_HEALTH_SERVER_WILL_BUGOUT );
			_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, NewHealth );
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
		}
		_unit->BaseAttackType = SCHOOL_SHADOW;
	}
	void AIUpdate()
	{
		if( _unit->GetAIInterface()->DamageDoneLastMeleeStrike != 0 && _unit->GetAIInterface()->GetMostHated() != NULL )
		{
			//statistical because some auras temporary might reduce or boost our / their dmg
			HitsDone++;
			DamageSum += MIN( _unit->GetAIInterface()->DamageDoneLastMeleeStrike, _unit->GetAIInterface()->GetMostHated()->GetMaxHealth() * 3 );
			_unit->GetAIInterface()->DamageDoneLastMeleeStrike = 0;
			int32 EffectiveDamage = DamageSum / HitsDone;

			int32 ExpectedDmg = _unit->GetAIInterface()->GetMostHated()->GetMaxHealth() * FAST_HEAL_EXPECTED_HEALTH_REDUCE_PCT / 100;
			float DamageNow = _unit->GetFloatValue( UNIT_FIELD_MINDAMAGE );
			float NeededScale = ExpectedDmg / EffectiveDamage;
			float NewDamage = MIN( MAX_DMG_SERVER_WILL_IGNORE, MAX( DamageNow, NeededScale * DamageNow ) );

			_unit->SetFloatValue( UNIT_FIELD_MINDAMAGE, NewDamage );
			_unit->SetFloatValue( UNIT_FIELD_MAXDAMAGE, NewDamage );
		}
	} 
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		MoguEncounterStoreStruct *store = (MoguEncounterStoreStruct*)_unit->GetMapMgr()->GetExtension( MAP_EXTENSION_MOGU_ENCOUNTER_STORE );
		if( store )
			store->WeakMobDmg = MAX( store->WeakMobDmg, _unit->GetFloatValue( UNIT_FIELD_MINDAMAGE ) );
    }
};

#define STRONG_DPS_EXPECTED_ALIVE_DURATION_SEC		60
#define STRONG_DPS_INITIAL_HEALTH					(200000*STRONG_DPS_EXPECTED_ALIVE_DURATION_SEC) //2M for 1 player ?
#define STRONG_DPS_PERIODIC_HEAL_INTERVAL_SEC		3
#define STRONG_DPS_ENTRY							111202

class StrongDPSTester : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(StrongDPSTester);

	uint32	SpawnTimeStamp;
	uint32	HealthPreviousHeal;
	uint32	HealthLostSinceSpawn;
	uint32	HealAmount;
	StrongDPSTester(Creature* pCreature) : CreatureAIScript(pCreature)	{	}
	void OnLoad()
	{
		RegisterAIUpdateEvent( STRONG_DPS_PERIODIC_HEAL_INTERVAL_SEC * 1000 );
		SpawnTimeStamp = GetTickCount();
		//scale it based on player count in instance
		uint32 PlayerCount = MIN( 50, MAX( 1, _unit->GetMapMgr()->GetPlayerCount() ) );
		uint32 ScaledInitialHealth = MIN( MAX_HEALTH_SERVER_WILL_BUGOUT, STRONG_DPS_INITIAL_HEALTH * PlayerCount );
		_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, ScaledInitialHealth );
		_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
		HealthPreviousHeal = _unit->GetUInt32Value( UNIT_FIELD_HEALTH );
		HealthLostSinceSpawn = 0;
		HealAmount = 100000;	//this is for STRONG_DPS_PERIODIC_HEAL_INTERVAL_SEC

		MoguEncounterStoreStruct *store = (MoguEncounterStoreStruct*)_unit->GetMapMgr()->GetExtension( MAP_EXTENSION_MOGU_ENCOUNTER_STORE );
		if( store && store->IsTestingPhase == false )
		{
			float NewDMG = MIN( ( store->WeakMobDmg + store->StrongMobDmg ) / 2, MAX_DMG_SERVER_WILL_IGNORE );
			_unit->SetFloatValue( UNIT_FIELD_MINDAMAGE, NewDMG );
			_unit->SetFloatValue( UNIT_FIELD_MAXDAMAGE, NewDMG );
			ScaledInitialHealth = MIN( STRONG_DPS_EXPECTED_ALIVE_DURATION_SEC * store->WeakMobHeal / 10, MAX_HEALTH_SERVER_WILL_BUGOUT );
			_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, ScaledInitialHealth );
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			HealAmount = store->StrongMobHeal;
		}
		_unit->BaseAttackType = SCHOOL_SHADOW;
	}
	void AIUpdate()
	{
		uint32 HealthNow = _unit->GetUInt32Value( UNIT_FIELD_HEALTH );
		uint32 HealthChange = HealthPreviousHeal - HealthNow;
		if( HealthChange > 0 )
		{
			HealthLostSinceSpawn += HealthChange;
			uint32 TimePasssedSinceSpawn = GetTickCount() - SpawnTimeStamp;
			if( TimePasssedSinceSpawn > 1000 )
			{
				uint32	HealthLostPerSecond = HealthLostSinceSpawn / ( TimePasssedSinceSpawn / 1000 );
				uint32	EstimatedAliveTime = _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) / HealthLostPerSecond;
				float	ScaleHealing = (float)EstimatedAliveTime / STRONG_DPS_EXPECTED_ALIVE_DURATION_SEC;
				HealAmount = MIN( MAX_HEAL_TO_PROTECT_FROM_BUGS, MAX( HealAmount, HealAmount * ScaleHealing ) );
			}
		}
		_unit->ModHealth( HealAmount );
	} 
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		MoguEncounterStoreStruct *store = (MoguEncounterStoreStruct*)_unit->GetMapMgr()->GetExtension( MAP_EXTENSION_MOGU_ENCOUNTER_STORE );
		if( store )
			store->StrongMobHeal = MIN( MAX_HEAL_TO_PROTECT_FROM_BUGS, MAX( store->StrongMobHeal, HealAmount ) );
    }
};

#define FAST_DPS_EXPECTED_ALIVE_DURATION_SEC		10	//leave this to 10 sec !
#define FAST_DPS_INITIAL_HEALTH						(100000*FAST_DPS_EXPECTED_ALIVE_DURATION_SEC) //100k for 1 player ?
#define FAST_DPS_PERIODIC_HEAL_INTERVAL_SEC			1	//leave this to 1 sec !
#define FAST_DPS_ENTRY								111203

class WeakDPSTester : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WeakDPSTester);

	uint32	SpawnTimeStamp;
	uint32	HealthPreviousHeal;
	uint32	HealthLostSinceSpawn;
	uint32	HealAmount;
	WeakDPSTester(Creature* pCreature) : CreatureAIScript(pCreature)	{	}
	void OnLoad()
	{
		RegisterAIUpdateEvent( FAST_DPS_PERIODIC_HEAL_INTERVAL_SEC * 1000 );
		SpawnTimeStamp = GetTickCount();
		//scale it based on player count in instance
		uint32 PlayerCount = MIN( 50, MAX( 1, _unit->GetMapMgr()->GetPlayerCount() ) );
		uint32 ScaledInitialHealth = MIN( MAX_HEALTH_SERVER_WILL_BUGOUT, FAST_DPS_INITIAL_HEALTH * PlayerCount );
		_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, ScaledInitialHealth );
		_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
		HealthPreviousHeal = _unit->GetUInt32Value( UNIT_FIELD_HEALTH );
		HealthLostSinceSpawn = 0;
		HealAmount = 50000;	//this is for FAST_DPS_EXPECTED_ALIVE_DURATION_SEC

		MoguEncounterStoreStruct *store = (MoguEncounterStoreStruct*)_unit->GetMapMgr()->GetExtension( MAP_EXTENSION_MOGU_ENCOUNTER_STORE );
		if( store && store->IsTestingPhase == false )
		{
			float NewDMG = MIN( ( store->WeakMobDmg + store->StrongMobDmg ) / 2, MAX_DMG_SERVER_WILL_IGNORE );
			_unit->SetFloatValue( UNIT_FIELD_MINDAMAGE, NewDMG );
			_unit->SetFloatValue( UNIT_FIELD_MAXDAMAGE, NewDMG );
			ScaledInitialHealth = MIN( FAST_DPS_EXPECTED_ALIVE_DURATION_SEC * store->WeakMobHeal / 10, MAX_HEALTH_SERVER_WILL_BUGOUT );
			_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, ScaledInitialHealth );
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			HealAmount = store->WeakMobHeal;
		}
		_unit->BaseAttackType = SCHOOL_SHADOW;
	}
	void AIUpdate()
	{
		uint32 HealthNow = _unit->GetUInt32Value( UNIT_FIELD_HEALTH );
		uint32 HealthChange = HealthPreviousHeal - HealthNow;
		if( HealthChange > 0 )
		{
			HealthLostSinceSpawn += HealthChange;
			uint32 TimePasssedSinceSpawn = GetTickCount() - SpawnTimeStamp;
			if( TimePasssedSinceSpawn > 1000 )
			{
				uint32	HealthLostPerSecond = HealthLostSinceSpawn / ( TimePasssedSinceSpawn / 1000 );
				uint32	EstimatedAliveTime = _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) / HealthLostPerSecond;
				float	ScaleHealing = (float)EstimatedAliveTime / STRONG_DPS_EXPECTED_ALIVE_DURATION_SEC;
				HealAmount = MIN( MAX_HEAL_TO_PROTECT_FROM_BUGS, MAX( HealAmount, HealAmount * ScaleHealing ) );
			}
		}
		_unit->ModHealth( HealAmount );
	} 
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		MoguEncounterStoreStruct *store = (MoguEncounterStoreStruct*)_unit->GetMapMgr()->GetExtension( MAP_EXTENSION_MOGU_ENCOUNTER_STORE );
		if( store )
			store->WeakMobHeal = MIN( MAX_HEAL_TO_PROTECT_FROM_BUGS, MAX( store->WeakMobHeal, HealAmount ) );
    }
};


#define LOCAL_DPS_ENTRY							111205

class BurningFire : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(BurningFire);

   uint32 TrapArmedStamp;
    BurningFire(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 1000 );
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
		}
		//if we enable this mob to be unselectable then he will not be able to cast the spell on others
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_DEAD);
		TrapArmedStamp = GetTickCount() + 5000;
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 38466 );	//No idea about the display :(
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
		}
	}
    void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		if( TrapArmedStamp > GetTickCount() )
			return;
		float r = 3.0f * 3.0f;
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( !SafeUnitCast( *itr )->isAlive() )
				continue;
			if( IsInrange( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), (*itr), r ) 
				&& isAttackable( _unit, SafeUnitCast( *itr ), true ) )
			{
				uint32 DmgToDo = SafePlayerCast( *itr )->GetMaxHealth() * 5 / 100;
				SafePlayerCast( *itr )->SendEnvironmentalDamageLog( SafePlayerCast( *itr )->GetGUID(), DAMAGE_LAVA, DmgToDo );
				SafePlayerCast( *itr )->DealDamage( SafePlayerCast( *itr ), DmgToDo, true );
			}
		}
		_unit->ReleaseInrangeLock();
	}
};

#define SOUL_HUNGERER							111204

class SoulHungerer : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SoulHungerer);

	uint64	MonitoredMinionGUID[4];
	uint32	Phase;
	uint32	PhaseStartStamp;
	uint32	LastSpawnStamp;
	uint32	SpawnCooldown;
	uint32	SpawnsMade;
	uint32	TrapSpawnCooldown;
	uint32	EncounterStartUStamp;
	SoulHungerer(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		MoguEncounterStoreStruct *store = new MoguEncounterStoreStruct;
		memset( store, 0, sizeof( store ) );
		store->IsTestingPhase = true;
		_unit->GetMapMgr()->SetExtension( MAP_EXTENSION_MOGU_ENCOUNTER_STORE, store );
		Phase = 0;

		RegisterAIUpdateEvent( 1000 );
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
		_unit->bInvincible = true;
		_unit->GetAIInterface()->m_canMove = false;
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		LOG_CREATURE_DEATH( _unit->GetEntry(), mKiller, UNIXTIME - EncounterStartUStamp );
	}
	void AIUpdate()
	{
		//we are idle now. Check for nearby players to see if we can begin encounter
		if( Phase == 0 )
		{
			InrangeLoopExitAutoCallback AutoLock;
			InRangePlayerSetRecProt::iterator itr = _unit->GetInRangePlayerSetBegin( AutoLock );
			if( itr != _unit->GetInRangePlayerSetEnd( ) && (*itr)->isAlive() && _unit->isInRange( *itr, 40 )  )
				Phase = 1;
		}

		//We are in encounter. Do we have allive player left near ourself ?
		if( Phase > 1 )
		{
			//nearby alive players
			uint32 InrangePlayers = 0;
			InRangePlayerSetRecProt::iterator itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
			{
				if( (*itr2)->isAlive() == false )
					continue;
				float dist =  (*itr2)->GetDistance2dSq( _unit );
				if( dist <= ( 40.0f * 40.0f ) )
					InrangePlayers++;
			}
			_unit->ReleaseInrangeLock();
			if( InrangePlayers == 0 )
			{
				Phase = 0;
				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The ritual is not yet complete. Need more souls !" );
				_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, "If uber team is not working, try a different approach" );
				//despawn all nearby mobs
				uint32 EntriesToDespawn[] = { STRONG_HEAL_ENTRY, FAST_HEAL_ENTRY, STRONG_DPS_ENTRY, FAST_DPS_ENTRY, 0 };
				InRangeSetRecProt::iterator itr;
				_unit->AquireInrangeLock(); //make sure to release lock before exit function !
				InrangeLoopExitAutoCallback AutoLock;
				for( itr = _unit->GetInRangeSetBegin( AutoLock ); itr != _unit->GetInRangeSetEnd(); itr++)
				{	 
					if( ( *itr )->IsCreature() == false )
						continue;
					for(uint32 i=0;i<20 && EntriesToDespawn[i]!=0; i++)
						if( ( *itr )->GetEntry() == EntriesToDespawn[i] )
						{
							SafeCreatureCast( *itr )->Despawn( 1, 0 );
							continue;
						}
				}
				_unit->ReleaseInrangeLock();
			}
			EncounterStartUStamp = UNIXTIME;
		}
		
		//test player tanking capability
		if( Phase == 1 )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Uhm, new souls to feed my hunger. Goood. Get them boy" );
			Creature *c = _unit->GetMapMgr()->CreateCreature( STRONG_HEAL_ENTRY, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
			if( c )
				MonitoredMinionGUID[0] = c->GetGUID();
			Phase = 2;
		}
		else if( Phase == 2 )
		{
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			Unit *u = _unit->GetMapMgr()->GetUnit( MonitoredMinionGUID[0] );
			if( u == NULL || u->IsDead() )
				Phase = 3;
		}
		else if( Phase == 3 )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hmm, was expecting human souls. Slave, work time" );
			Creature *c = _unit->GetMapMgr()->CreateCreature( FAST_HEAL_ENTRY, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
			if( c )
				MonitoredMinionGUID[0] = c->GetGUID();
			Phase = 4;
		}
		else if( Phase == 4 )
		{
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			Unit *u = _unit->GetMapMgr()->GetUnit( MonitoredMinionGUID[0] );
			if( u == NULL || u->IsDead() )
				Phase = 5;
		}
		else if( Phase == 5 )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Usless slave. Next !" );
			Creature *c = _unit->GetMapMgr()->CreateCreature( STRONG_DPS_ENTRY, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
			if( c )
				MonitoredMinionGUID[0] = c->GetGUID();
			Phase = 6;
		}
		else if( Phase == 6 )
		{
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			Unit *u = _unit->GetMapMgr()->GetUnit( MonitoredMinionGUID[0] );
			if( u == NULL || u->IsDead() )
				Phase = 7;
		}
		else if( Phase == 7 )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What was that slave thinking ? More" );
			Creature *c = _unit->GetMapMgr()->CreateCreature( FAST_DPS_ENTRY, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
			if( c )
				MonitoredMinionGUID[0] = c->GetGUID();
			Phase = 8;
		}
		else if( Phase == 8 )
		{
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			Unit *u = _unit->GetMapMgr()->GetUnit( MonitoredMinionGUID[0] );
			if( u == NULL || u->IsDead() )
			{
				Phase = 9;
				MoguEncounterStoreStruct *store = (MoguEncounterStoreStruct*)_unit->GetMapMgr()->GetExtension( MAP_EXTENSION_MOGU_ENCOUNTER_STORE );
				if( store )
					store->IsTestingPhase = false;
			}
		}
		else if( Phase == 9 )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "K. So 1 slave is no slave, how about 4 slaves ?" );
			Creature *c = _unit->GetMapMgr()->CreateCreature( STRONG_HEAL_ENTRY, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
			if( c )
				MonitoredMinionGUID[0] = c->GetGUID();
			c = _unit->GetMapMgr()->CreateCreature( FAST_HEAL_ENTRY, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
			if( c )
				MonitoredMinionGUID[1] = c->GetGUID();
			c = _unit->GetMapMgr()->CreateCreature( STRONG_DPS_ENTRY, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
			if( c )
				MonitoredMinionGUID[2] = c->GetGUID();
			c = _unit->GetMapMgr()->CreateCreature( FAST_DPS_ENTRY, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
			if( c )
				MonitoredMinionGUID[3] = c->GetGUID();
			Phase = 10;
		}
		else if( Phase == 10 )
		{
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			bool AllDead = true;
			for( uint32 i=0;i<4;i++)
			{
				Unit *u = _unit->GetMapMgr()->GetUnit( MonitoredMinionGUID[0] );
				if( u != NULL && u->IsDead() == false )
					AllDead = false;
			}
			if( AllDead )
			{
				Phase = 11;
				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hmm, all this counting makes me confused. Spamming is easy" );
				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll show you a tank and spank encounter you will remember" );
				PhaseStartStamp = GetTickCount();
				SpawnCooldown = 60000;
				LastSpawnStamp = GetTickCount() - SpawnCooldown;
				SpawnsMade = 0;
			}
		}
		else if( Phase == 11 || Phase == 12 )
		{
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			uint32 TickNow = GetTickCount();
			if( TickNow > PhaseStartStamp + 6 * 60 * 1000 )
//			if( TickNow > PhaseStartStamp + 1 * 60 * 1000 )
			{
				PhaseStartStamp = TickNow;
				PhaseStartStamp = TickNow + 10 * 60 * 10000;
				if( Phase == 11 )
				{
					_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Run around little ants" );
					TrapSpawnCooldown = 0;
				}
				Phase++;
			}
			if( LastSpawnStamp + SpawnCooldown < TickNow )
			{
				LastSpawnStamp = TickNow;
				uint32 EntriesToSpawn[] = { STRONG_HEAL_ENTRY, FAST_HEAL_ENTRY, FAST_HEAL_ENTRY, FAST_HEAL_ENTRY, FAST_HEAL_ENTRY, STRONG_DPS_ENTRY, FAST_DPS_ENTRY, FAST_DPS_ENTRY, FAST_DPS_ENTRY, FAST_DPS_ENTRY, 0 };
				uint32 ListSize = sizeof( EntriesToSpawn ) / sizeof( uint32 );
				uint32 SpawnCount;
				uint32 PlayerCount = 0;

				{
					InRangePlayerSetRecProt::iterator itr2;
					InrangeLoopExitAutoCallback AutoLock;
					for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
						PlayerCount++; 
				}
				SpawnCount = PlayerCount * 3 * 120000 / SpawnCooldown;

				//if it is too easy then scale mob count up
				uint32 ActiveMobCount = 0;
				{
					uint32 EntriesToCheck[] = { STRONG_HEAL_ENTRY, FAST_HEAL_ENTRY, STRONG_DPS_ENTRY, FAST_DPS_ENTRY, 0 };
					InRangeSetRecProt::iterator itr;
					InrangeLoopExitAutoCallback AutoLock;
					for( itr = _unit->GetInRangeSetBegin( AutoLock ); itr != _unit->GetInRangeSetEnd();itr++)
					{						 
						if( ( *itr )->IsCreature() == false || SafeUnitCast( *itr )->isAlive() == false )
							continue;
						for(uint32 i=0;i<20 && EntriesToCheck[i]!=0; i++)
							if( ( *itr )->GetEntry() == EntriesToCheck[i] )
								ActiveMobCount++;
					}
				}
				uint32 ExpectedMobCount = SpawnsMade / 2;
				if( ExpectedMobCount > ActiveMobCount ) 
					SpawnCount = MAX( SpawnCount, SpawnCount * ExpectedMobCount / SpawnsMade );


				InrangeLoopExitAutoCallback AutoLock;
				InRangePlayerSetRecProt::iterator itr1, itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); 
				itr1 = itr2;
				for( uint32 i=0;i<SpawnCount; i++)
				{
					uint32 SelectedEntry = EntriesToSpawn[ i % ListSize ];
					Creature *c = _unit->GetMapMgr()->CreateCreature( SelectedEntry, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
					if( itr2 == _unit->GetInRangePlayerSetEnd() )
						itr2 = itr1;
					c->GetAIInterface()->AttackReaction( *itr2, 1000000 );
					itr2++;
					SpawnsMade++;
				}

				SpawnCooldown = MAX( 30000, SpawnCooldown * 90 / 100 );

				if( Phase == 12 && TrapSpawnCooldown < TickNow )
				{
					TrapSpawnCooldown = TickNow + 20000;
					InRangePlayerSetRecProt::iterator itr,itr2;
					_unit->AquireInrangeLock(); //make sure to release lock before exit function !
					InrangeLoopExitAutoCallback AutoLock;
					for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
					{
						itr = itr2;
						itr2++;
						if( !SafeUnitCast( *itr )->isAlive() ) 
							continue;
						Creature *c = _unit->GetMapMgr()->CreateCreature( LOCAL_DPS_ENTRY , true, ( *itr )->GetPositionX(), ( *itr )->GetPositionY(), ( *itr )->GetPositionZ() );
						if( c )
							c->Despawn( 10000, 0 );
					}
					_unit->ReleaseInrangeLock();
				}
			}
		}
		else if( Phase == 13 )
		{
//			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Noooo. What is happening. This can't be" );
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hmm, you did tank and spank it. Time to get my hands dirty" );
			_unit->GetAIInterface()->disable_melee = false;
			_unit->GetAIInterface()->disable_combat = false;
			_unit->GetAIInterface()->disable_targeting = false;
			_unit->bInvincible = false;
			_unit->GetAIInterface()->m_canMove = true;
			Phase = 14;
		}
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define SEASONAL_SPONGE							111206
#define SEASONAL_SPONGE_CLONE					111207

class DMGSponge : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DMGSponge);

	uint64	MonitoredMinionGUID[3];
	uint32	Phase;
	uint32	SwitchStamp;
	uint32	UsingSchool;
	float	SpawnX,SpawnY,SpawnZ;
	bool	IAmClone;
	uint32	EncounterStartUStamp;
	DMGSponge(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		Phase = 0;

		SpellEntry *sp = dbcSpell.LookupEntryForced( 36650 );	//Phase Slip Vulnerability - dummy spell. We will overwrite it
		ProcTriggerSpell pts( sp, NULL );
		pts.caster = _unit->GetGUID();
		pts.spellId = sp->Id;
		pts.procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
		pts.created_with_value = 0;
		pts.procCharges = 0;
		pts.procChance = 100;
		pts.procInterval = 0;
		_unit->RegisterScriptProcStruct( pts );

		RegisterAIUpdateEvent( 1000 );
		UsingSchool = RandomUInt() % SCHOOL_COUNT;
		SpawnX = _unit->GetPositionX();
		SpawnY = _unit->GetPositionY();
		SpawnZ = _unit->GetPositionZ();
		IAmClone = ( _unit->GetEntry() == SEASONAL_SPONGE_CLONE );
	}
	void AIUpdate()
	{
		//we are idle now. Check for nearby players to see if we can begin encounter
		if( Phase == 0 )
		{
			InrangeLoopExitAutoCallback AutoLock;
			InRangePlayerSetRecProt::iterator itr = _unit->GetInRangePlayerSetBegin( AutoLock );
			if( itr != _unit->GetInRangePlayerSetEnd( ) && (*itr)->isAlive() && _unit->isInRange( *itr, 40 )  )
				Phase = 1;
		}

		//We are in encounter. Do we have allive player left near ourself ?
		if( Phase > 1 && IAmClone == false )
		{
			//nearby alive players
			uint32 InrangePlayers = 0;
			InRangePlayerSetRecProt::iterator itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
			{
				if( (*itr2)->isAlive() == false )
					continue;
				float dist =  (*itr2)->GetDistance2dSq( _unit );
				if( dist <= ( 40.0f * 40.0f ) )
					InrangePlayers++;
			}
			_unit->ReleaseInrangeLock();
			if( InrangePlayers == 0 )
			{
				Phase = 0;
				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hahaha, sponge absorbs all !" );
				_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, "You can hurt sponge only with 1 school in a specific form" );
				//despawn all nearby mobs
				uint32 EntriesToDespawn[] = { SEASONAL_SPONGE_CLONE, 0 };
				InRangeSetRecProt::iterator itr;
				_unit->AquireInrangeLock(); //make sure to release lock before exit function !
				InrangeLoopExitAutoCallback AutoLock;
				for( itr = _unit->GetInRangeSetBegin( AutoLock ); itr != _unit->GetInRangeSetEnd(); itr++)
				{	 
					if( ( *itr )->IsCreature() == false )
						continue;
					for(uint32 i=0;i<20 && EntriesToDespawn[i]!=0; i++)
						if( ( *itr )->GetEntry() == EntriesToDespawn[i] )
						{
							SafeCreatureCast( *itr )->Despawn( 1, 0 );
							continue;
						}
				}
				_unit->ReleaseInrangeLock();
			}
		}
		//spawn a few dmg dealers
		if( Phase == 1 )
		{
			if( IAmClone == false  )
			{
				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Puny mortals can't hurt the SPONGE. Sponge sucks all damage up." );
				for( uint32 i=0;i<4;i++)
				{
					Creature *c = _unit->GetMapMgr()->CreateCreature( SEASONAL_SPONGE_CLONE, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
					if( c )
						MonitoredMinionGUID[i] = c->GetGUID();
				}
				_unit->SetFloatValue( UNIT_FIELD_MINDAMAGE, 50000 );
				_unit->SetFloatValue( UNIT_FIELD_MAXDAMAGE, 50000 );
				_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, 3000 );
			}
			else
			{
				_unit->SetFloatValue( UNIT_FIELD_MINDAMAGE, 20000 );
				_unit->SetFloatValue( UNIT_FIELD_MAXDAMAGE, 20000 );
				_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, 1500 );
				_unit->SetFloatValue( OBJECT_FIELD_SCALE_X, 0.75f * _unit->GetFloatValue( OBJECT_FIELD_SCALE_X ) );
			}
			Phase = 2;
			SwitchStamp = 0;
			EncounterStartUStamp = UNIXTIME;
		}
		else if( Phase == 2 )
		{
			uint32 TickNow = GetTickCount();
			//change absorb school from time to time
			int64 *DamageTakenSinceSwitch = _unit->GetCreateIn64Extension( EXTENSION_ID_VULNERABILITY_DMG );
			if( SwitchStamp < TickNow || *DamageTakenSinceSwitch > _unit->GetMaxHealth() * 10 / 100 )
			{
				*DamageTakenSinceSwitch = 0;
				SwitchStamp = TickNow + 2 * 60 * 1000;
				UsingSchool = ( UsingSchool + 1 ) % SCHOOL_COUNT;
				_unit->BaseAttackType = UsingSchool;
				int64 *CanDamageSchool = _unit->GetCreateIn64Extension( EXTENSION_ID_VULNERABILITY_TYPE );
				*CanDamageSchool = UsingSchool;
				uint32 Displays[ SCHOOL_COUNT ]={ 31138, 26166, 1405, 1923, 25475, 10631, 14253 };
				_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, Displays[ UsingSchool ] );
			}
			//increase our dmg from time to time : 10 minutes = 50k => 1 sec 
			_unit->SetFloatValue( UNIT_FIELD_MINDAMAGE, _unit->GetFloatValue( UNIT_FIELD_MINDAMAGE ) + 83.0f );
			_unit->SetFloatValue( UNIT_FIELD_MAXDAMAGE, _unit->GetFloatValue( UNIT_FIELD_MAXDAMAGE ) + 83.0f );
		}
	} 
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		_unit->Despawn( 1000, 0 );
		if( IAmClone == false )
		{
			LOG_CREATURE_DEATH( _unit->GetEntry(), mKiller, UNIXTIME - EncounterStartUStamp );
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time to show my true form. Your own mirror." );
			Creature *c = _unit->GetMapMgr()->CreateCreature( SOUL_HUNGERER, true, SpawnX, SpawnY, SpawnZ );
		}
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define EXTENSION_ID_PACMAN_WORM				111208	//spawn/despawn worms fruits
#define PACMAN_WORM_SEGMENT						111209	
#define EXTENSION_ID_PACMAN_WORM_FRUIT						111210

#ifdef _DEBUG
	#define WORM_MIN_PLAYERS_FOR_ENCOUNTER			1
#else
	#define WORM_MIN_PLAYERS_FOR_ENCOUNTER			1
#endif

//#define WORM_MIN_DISTANCE_FOR_ENCOUNTER_START		10.0f
//#define WORM_MAX_DISTANCE_FOR_ENCOUNTER_END		40.0f
//#define WORM_MARGINS_NO_FRUIT_SPAWN				15.0f
#define WORM_MIN_DISTANCE_FOR_ENCOUNTER_START	5.0f
#define WORM_MAX_DISTANCE_FOR_ENCOUNTER_END		20.0f
#define WORM_MARGINS_NO_FRUIT_SPAWN				5.0f
#define WORM_MAX_SEGMENTS						20
#define WORM_INITIAL_SEGMENT_COUNT				3
#define WORM_INITIAL_SEGMENT_ALIVE_DUR			500		//every 1 second spawn a new segment so last one will despawn
#define WORM_SEGMENT_SIZE_YARDS					1.0f	//visually, a segment has this size. Radius

// players loose health, they need to reach fruit in time or they die
// fruit will heal full players and make worms longer
// if player touches worm, they die
// if less then 5 players are alive, encounter resets

class WormSpawnController : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WormSpawnController);

	struct WormStoreStruct
	{
		uint64	ChasedPlayerGUID;
		uint64	SegmentGuids[WORM_MAX_SEGMENTS];
		float	PrevSpawnX,PrevSpawnY,PrevSpawnZ;
	};
	uint32	Phase;
	uint32	SpeachCooldown;
	WormStoreStruct	Worms[WORM_MIN_PLAYERS_FOR_ENCOUNTER];	
	uint32	SegmentAliveDuration;
	uint32	SnakeLengthNow;
	uint64	FruitGuid;
	float	SpawnX,SpawnY,SpawnZ;
	uint32	EncounterStartUStamp;
	WormSpawnController(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		Phase = 0;
		SpeachCooldown = 0;
		memset( Worms, 0, sizeof( Worms ) );
		RegisterAIUpdateEvent( WORM_INITIAL_SEGMENT_ALIVE_DUR );
		SpawnX = _unit->GetPositionX();
		SpawnY = _unit->GetPositionY();
		SpawnZ = _unit->GetPositionZ();
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
		_unit->bInvincible = true;
	}
	void SpawnFruit( float DeniedZone = WORM_MARGINS_NO_FRUIT_SPAWN )
	{
		float Randx = RandomUInt() % float2int32( WORM_MAX_DISTANCE_FOR_ENCOUNTER_END - DeniedZone );
		float Randy = RandomUInt() % float2int32( WORM_MAX_DISTANCE_FOR_ENCOUNTER_END - DeniedZone );
		if( RandChance( 50 ) )
			Randx = -Randx - DeniedZone;
		else
			Randx = Randx + DeniedZone;
		if( RandChance( 50 ) )
			Randy = -Randy - DeniedZone;
		else
			Randy = Randy + DeniedZone;

		ASSERT( abs( Randx ) < WORM_MAX_DISTANCE_FOR_ENCOUNTER_END && abs( Randy ) < WORM_MAX_DISTANCE_FOR_ENCOUNTER_END );

		float x = _unit->GetPositionX() + Randx;
		float y = _unit->GetPositionY() + Randy;
		Creature *c = _unit->GetMapMgr()->CreateCreature( EXTENSION_ID_PACMAN_WORM_FRUIT, true, x, y, _unit->GetPositionZ() );
		if( c )
			FruitGuid = c->GetGUID();
	}
	void AIUpdate()
	{
		//we are idle now. Check for nearby players to see if we can begin encounter
		InrangeLoopExitAutoCallback AutoLock;
		if( Phase == 0 )
		{
			uint32 PlayersInrange = 0;
			Player *PrevPlayer = NULL;
			InRangePlayerSetRecProt::iterator itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
			{
				if( (*itr2)->isAlive() == false )
					continue;
				if( PrevPlayer != NULL && (*itr2)->GetDistance2dSq( _unit ) < 4.0f * 4.0f )
					continue;
				float dist =  (*itr2)->GetDistance2dSq( _unit );
				if( dist > ( WORM_MAX_DISTANCE_FOR_ENCOUNTER_END * WORM_MAX_DISTANCE_FOR_ENCOUNTER_END ) )
					continue;
				PlayersInrange++;
			}
			if( PlayersInrange > 0 && PlayersInrange < WORM_MIN_PLAYERS_FOR_ENCOUNTER && SpeachCooldown < GetTickCount() )
			{
				SpeachCooldown = GetTickCount() + 10000;
				_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, "Encounter can be active with at least 3 alive players. Have some distance between players and boss!" );
			}
			if( PlayersInrange >= WORM_MIN_PLAYERS_FOR_ENCOUNTER )
			{
				Phase = 1;
				int64 *CannibalismDetected = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_ID_PACMAN_WORM );
				*CannibalismDetected = 0;
			}
		}

		//We are in encounter. Do we have allive player left near ourself ?
		if( Phase > 0 )
		{
			//nearby alive players
			uint32 InrangePlayers = 0;
			InRangePlayerSetRecProt::iterator itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
			{
				if( (*itr2)->isAlive() == false )
					continue;
				float dist =  (*itr2)->GetDistance2dSq( _unit );
				if( dist <= ( WORM_MAX_DISTANCE_FOR_ENCOUNTER_END * WORM_MAX_DISTANCE_FOR_ENCOUNTER_END ) )
					InrangePlayers++;
			}
			_unit->ReleaseInrangeLock();

			int64 *CannibalismDetected = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_ID_PACMAN_WORM );

			if( *CannibalismDetected != 0 || Phase == 666 || Phase == 667 || ( InrangePlayers > 0 && InrangePlayers < WORM_MIN_PLAYERS_FOR_ENCOUNTER ) )
			{
				Phase = 0;
//				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Not enough kids to chase :(" );
				_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, "Don't let the snakes starve or eat each other." );
				if( InrangePlayers < WORM_MIN_PLAYERS_FOR_ENCOUNTER )
					_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, "Not enough players nearby." );
				if( *CannibalismDetected )
					_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, "Snake ate each other." );

				//despawn all nearby mobs
				uint32 EntriesToDespawn[] = { PACMAN_WORM_SEGMENT, EXTENSION_ID_PACMAN_WORM_FRUIT, 0 };
				InRangeSetRecProt::iterator itr;
				_unit->AquireInrangeLock(); //make sure to release lock before exit function !
				for( itr = _unit->GetInRangeSetBegin( AutoLock ); itr != _unit->GetInRangeSetEnd(); itr++)
				{	 
					if( ( *itr )->deleted != OBJ_AVAILABLE || ( *itr )->IsCreature() == false )
						continue;
					for(uint32 i=0;i<20 && EntriesToDespawn[i]!=0; i++)
						if( ( *itr )->GetEntry() == EntriesToDespawn[i] )
						{
							SafeCreatureCast( *itr )->Despawn( 500, 0 );
							continue;
						}
				}
				_unit->ReleaseInrangeLock();
				if( Phase == 667 )
				{
					RemoveAIUpdateEvent();
					_unit->Despawn( 500, 0 );
					LOG_CREATURE_DEATH2( _unit->GetEntry(), UNIXTIME - EncounterStartUStamp );
				}
			}
		}
		if( Phase == 1 )
		{
			//select 3 player
			uint32 SelectedPlayers = 0;
			Player *PrevPlayer = NULL;
			InRangePlayerSetRecProt::iterator itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
			{
				if( (*itr2)->isAlive() == false )
					continue;
				float dist =  (*itr2)->GetDistance2dSq( _unit );
				if( dist <= ( WORM_MAX_DISTANCE_FOR_ENCOUNTER_END * WORM_MAX_DISTANCE_FOR_ENCOUNTER_END ) )
				{
					if( PrevPlayer != NULL && (*itr2)->GetDistance2dSq( _unit ) < 2.0f * 2.0f )
						continue;
					PrevPlayer = SafePlayerCast( *itr2 );
					Worms[ SelectedPlayers ].ChasedPlayerGUID = (*itr2)->GetGUID();
					memset( Worms[ SelectedPlayers ].SegmentGuids, 0, sizeof( Worms[ SelectedPlayers ].SegmentGuids ) );
					Worms[ SelectedPlayers ].PrevSpawnX = _unit->GetPositionX();
					Worms[ SelectedPlayers ].PrevSpawnY = _unit->GetPositionY();
					Worms[ SelectedPlayers ].PrevSpawnZ = _unit->GetPositionZ();
					SelectedPlayers++;
					if( SelectedPlayers >= WORM_MIN_PLAYERS_FOR_ENCOUNTER )
						break;
					_unit->SendChatMessageToPlayer( CHAT_MSG_MONSTER_WHISPER, LANG_UNIVERSAL, "You have been selected to lead the poor starving worm to feed", SafePlayerCast( *itr2 ) );
				}
			}
			_unit->ReleaseInrangeLock();
			if( SelectedPlayers >= WORM_MIN_PLAYERS_FOR_ENCOUNTER )
			{
				Phase = 2;
				SnakeLengthNow = WORM_INITIAL_SEGMENT_COUNT;
				SpawnFruit();
				EncounterStartUStamp = UNIXTIME;
			}
		}
		if( Phase == 2 )
		{
			for( uint32 i=0;i<WORM_MIN_PLAYERS_FOR_ENCOUNTER;i++)
			{
				Player *p = _unit->GetMapMgr()->GetPlayer( Worms[i].ChasedPlayerGUID );
				if( p == NULL || p->isAlive() == false || _unit->isInRange( p, WORM_MAX_DISTANCE_FOR_ENCOUNTER_END ) == false )
				{
					Phase = 666;
					return;
				}
				//get the location of the new spawn : X yards closer to player from last spawn
				float x,y,z;
				if( p->GetPositionX() < Worms[ i ].PrevSpawnX )
					x = Worms[ i ].PrevSpawnX - WORM_SEGMENT_SIZE_YARDS * 2.0f;
				else if( p->GetPositionX() > Worms[ i ].PrevSpawnX )
					x = Worms[ i ].PrevSpawnX + WORM_SEGMENT_SIZE_YARDS * 2.0f;
				else
					x = Worms[ i ].PrevSpawnX;
				if( p->GetPositionY() < Worms[ i ].PrevSpawnY )
					y = Worms[ i ].PrevSpawnY - WORM_SEGMENT_SIZE_YARDS * 2.0f;
				else if( p->GetPositionY() > Worms[ i ].PrevSpawnY )
					y = Worms[ i ].PrevSpawnY + WORM_SEGMENT_SIZE_YARDS * 2.0f;
				else
					y = Worms[ i ].PrevSpawnY;

				if( x != Worms[ i ].PrevSpawnX || y != Worms[ i ].PrevSpawnY )
				{
					z = p->GetPositionZ();
					Creature *c = _unit->GetMapMgr()->CreateCreature( PACMAN_WORM_SEGMENT, true, x, y, z );
					c->Despawn( SnakeLengthNow * WORM_INITIAL_SEGMENT_ALIVE_DUR, 0 );
					Worms[ i ].PrevSpawnX = x;
					Worms[ i ].PrevSpawnY = y;
					Worms[ i ].PrevSpawnZ = z;
				}
			}
			//de we have a spawned fruit ? If no, then spawn a new one
			Creature *Fruit  = _unit->GetMapMgr()->GetCreature( FruitGuid );
			if( Fruit == NULL )
			{
				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yesss, yessss, grow my little ones. More food for the pets" );
				SnakeLengthNow += 2;
				if( SnakeLengthNow >= WORM_MAX_SEGMENTS )
					Phase = 3;
				else
				{
					SpawnFruit( 0 );
				}
			}
		}
		if( Phase == 3 )
		{
			//spawn the sponge and suicide
			Creature *c = _unit->GetMapMgr()->CreateCreature( SEASONAL_SPONGE, true, SpawnX, SpawnY, SpawnZ );
			Phase = 667;
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Burp. Time to hatch the worms" );
			_unit->Despawn( 10000, 0 );
		}
	} 
};

class WormSegment : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(WormSegment);

    WormSegment(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( WORM_INITIAL_SEGMENT_ALIVE_DUR );
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
		}
		//if we enable this mob to be unselectable then he will not be able to cast the spell on others
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_DEAD);
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 549 ); 
    }
    void AIUpdate()
    {
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 5781 ); 
		if( _unit->isAlive() == false )
			return;
		float r = 2.0f * WORM_SEGMENT_SIZE_YARDS * 2.0f * WORM_SEGMENT_SIZE_YARDS;
		float REat = 3.0f + 2.0f * WORM_SEGMENT_SIZE_YARDS * 2.0f * WORM_SEGMENT_SIZE_YARDS;	//eat 1 to the sides due to the jumpy movement
		InRangeSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( ( *itr )->IsUnit() == false || !SafeUnitCast( *itr )->isAlive() )
				continue;
			float Dist = (*itr)->GetDistance2dSq( _unit );
/*
if( (*itr)->GetEntry() == EXTENSION_ID_PACMAN_WORM_FRUIT && (*itr)->IsCreature() )
{
	char msg[500];
	sprintf( msg, "dist to egg %f need %f", Dist, REat );
	_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg );
}/**/
			//let's not be clumsy 
			if( Dist < REat )
			{
				//eat up fruits
				if( (*itr)->GetEntry() == EXTENSION_ID_PACMAN_WORM_FRUIT && (*itr)->IsCreature() )
				{
					int64 *FruitIsRiped = (*itr)->GetCreateIn64Extension( EXTENSION_ID_PACMAN_WORM_FRUIT );
					if( *FruitIsRiped )
						SafeCreatureCast(*itr)->Despawn( 500, 0 );
				}
			}
			if( Dist < r )
			{
				//kill players that get near us
				if( (*itr)->IsPlayer() )
				{
					SafePlayerCast( *itr )->SetHealth( 1 );
					SafePlayerCast( *itr )->SendEnvironmentalDamageLog( SafePlayerCast( *itr )->GetGUID(), DAMAGE_LAVA, 10 );
					SafePlayerCast( *itr )->DealDamage( SafePlayerCast( *itr ), 10, true );
				}
				//eat ourself ?
				if( (*itr)->GetEntry() == PACMAN_WORM_SEGMENT && (*itr)->IsCreature() )
				{
/*{
	char msg[500];
	sprintf( msg, "dist to segment %f need %f", Dist, r );
	_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg );
}/**/
					int64 *CannibalismDetected = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_ID_PACMAN_WORM );
					*CannibalismDetected = 1;
				}
			}
		}
		_unit->ReleaseInrangeLock();
		//give a snake feeling
		_unit->SetScale( _unit->GetScale() * 0.95f );
	}
};

class WormFood : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(WormFood);
    WormFood(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 5000 );
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
		}
		//if we enable this mob to be unselectable then he will not be able to cast the spell on others
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_DEAD);
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 20245 );	
		_unit->SetScale( 0.5f );	
    }
    void AIUpdate()
    {
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 20345 );	
		_unit->SetScale( 3.5f );	
		int64 *FruitIsRiped = _unit->GetCreateIn64Extension( EXTENSION_ID_PACMAN_WORM_FRUIT );
		*FruitIsRiped = 1;
		RemoveAIUpdateEvent();
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define FROSTGUARD_ENTRY 111211

//frost ( melee ):
//- encapse some random player in ice
//- buble at pct health loss or timer
//- crush armor. Stacks
class FrostGuard : public CreatureAIScript
{
public:
	uint32	EncapsStamp;
	int32	BubbleAtPCT;
	uint32	phase;
	ADD_CREATURE_FACTORY_FUNCTION(FrostGuard);
    FrostGuard(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 4000 );
		phase = 0;
	}
    void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			phase = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		// Init phase
		if( phase == 0 )
		{
            EncapsStamp = TickNow + RandomUInt() % 25000;
			BubbleAtPCT = 90;
            phase = 1;
		}

		if( _unit->IsCrowdControlledNoAttack() )
			return;

		if( _unit->GetHealthPct() < BubbleAtPCT )
		{
			BubbleAtPCT -= 10;
			_unit->CastSpell( _unit, 13007 , true ); //Divine Protection
		}

		if( EncapsStamp < TickNow )
		{
			Unit *secondary = _unit->GetAIInterface()->GetSecondHated();
			if( secondary )
			{
				EncapsStamp = TickNow + RandomUInt() % 25000;
				_unit->CastSpell( secondary, 36911 , true ); //Ice Block
			}
		}

		_unit->CastSpell( tank, 59350 , true ); //Sunder Armor
	}
};

#define NATUREGUARD_ENTRY		111212
#define NATUREGUARDSUMMON_ENTRY	111213

//nature ( passive ):
//- spawn ads that bind life to others( lots of health, small dmg )
//- root players
class NatureGuard : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NatureGuard);
	uint64 MonitoredGUIDs[8];
    NatureGuard(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 4000 );	//4 seconds with 1M life, you need at least 250k DPS focused on this guy
		_unit->GetAIInterface()->setCurrentAgent( AGENT_FLEE );
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
	}
	void AIUpdate()
    {
		if( _unit->IsCrowdControlledNoAttack() )
			return;

		bool InrangeCombat = false;
		InRangePlayerSetRecProt::iterator itr2;
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
		{
			float dist =  (*itr2)->GetDistance2dSq( _unit );
			if( dist <= 30.0f * 30.0f && (*itr2)->CombatStatus.IsInCombat() )
			{
				InrangeCombat = true;
				break;
			}
		}
		if( InrangeCombat == false )
			return;

		for( uint32 i = 0; i < 6; i++ )
		{
			Unit *u = _unit->GetMapMgr()->GetUnit( MonitoredGUIDs[ i ] );
			if( u == NULL || u->GetHealthPct() <= 3 )
			{
				if( u && u->IsCreature() )
					SafeCreatureCast( u )->Despawn( 500, 0 );
				Creature *c = _unit->GetMapMgr()->CreateCreature( NATUREGUARDSUMMON_ENTRY, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
				if( c )
				{
					MonitoredGUIDs[ i ] = c->GetGUID();
					c->SetUInt64Value( UNIT_FIELD_CREATEDBY, _unit->GetGUID() );
					c->Despawn( 6 * 60000, 0 );
					break;
				}
			}
		}
	}
};

class NatureGuardSummon : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(NatureGuardSummon);
    NatureGuardSummon(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 2000 );
	}
	void AIUpdate()
    {
		Unit	*MinhealthUnit = NULL;
		uint32	LowestHPPCT = 100;
		InRangeSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
		{
			itr = itr2;
			itr2++;
			if( !(*itr)->IsCreature() )
				continue;
			//temp spawns
			if( (*itr)->GetUInt32Value( UNIT_FIELD_CREATEDBY ) )
				continue;
			//do not heal our creator forever :)
			if( (*itr)->GetGUID() == _unit->GetUInt64Value( UNIT_FIELD_CREATEDBY ) )
				continue;
			uint32 HelathPCTNow = SafeUnitCast( *itr )->GetHealthPct();
			if( HelathPCTNow < LowestHPPCT )
			{
				if( isAttackable( _unit, (*itr) ) )
					continue;
				LowestHPPCT = HelathPCTNow;
				MinhealthUnit = SafeUnitCast( *itr );
			}
		}
		_unit->ReleaseInrangeLock();
		if( MinhealthUnit )
		{
			_unit->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, MinhealthUnit->GetGUID() );
			_unit->SetUInt32Value( UNIT_CHANNEL_SPELL, 689 );
			int32 MissingHealth = MinhealthUnit->GetMaxHealth() - MinhealthUnit->GetHealth();
			int32 CanHeal = _unit->GetHealth() - 1;
			int32 WillHeal;
			if( MissingHealth < CanHeal )
				WillHeal = MissingHealth;
			else
				WillHeal = CanHeal / 2;
			_unit->Heal( MinhealthUnit, 2061, WillHeal );	//Flash Heal
			_unit->SetHealth( CanHeal + 1 - WillHeal );
		}
	}
};

#define SHADOWGUARD_ENTRY		111214
#define SHADOWGUARDSUMMON_ENTRY	111215

//shadow ( passive ):
//- fear players that attack him
//- spawn lots and lots of skeletons with weak dmg and life
class ShadowGuard : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ShadowGuard);
	uint64 MonitoredGUIDs[50];
    ShadowGuard(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 10000 );	//100k life every 10 secondsis 10k dps to keep them all dead
		_unit->GetAIInterface()->setCurrentAgent( AGENT_FLEE );
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
	}
	void AIUpdate()
    {
		if( _unit->IsCrowdControlledNoAttack() )
			return;

		bool InrangeCombat = false;
		InRangePlayerSetRecProt::iterator itr2;
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
		{
			float dist =  (*itr2)->GetDistance2dSq( _unit );
			if( dist <= 30.0f * 30.0f && (*itr2)->CombatStatus.IsInCombat() )
			{
				InrangeCombat = true;
				break;
			}
		}
		if( InrangeCombat == false )
			return;

		for( uint32 i = 0; i < 50; i++ )
		{
			Unit *u = _unit->GetMapMgr()->GetUnit( MonitoredGUIDs[ i ] );
			if( u == NULL )
			{
				if( u && u->IsCreature() )
					SafeCreatureCast( u )->Despawn( 500, 0 );
				Creature *c = _unit->GetMapMgr()->CreateCreature( SHADOWGUARDSUMMON_ENTRY, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
				if( c )
				{
					MonitoredGUIDs[ i ] = c->GetGUID();
					float RandomScaler = 1.0f + ( RandomUInt() % 50 ) / 100.0f;
					c->SetMaxHealth( c->GetMaxHealth() * RandomScaler );
					c->SetHealth( c->GetMaxHealth() );
					c->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, c->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME ) * RandomScaler );
					c->SetFloatValue( UNIT_FIELD_MINDAMAGE, c->GetFloatValue( UNIT_FIELD_MINDAMAGE ) * RandomScaler );
					c->SetFloatValue( UNIT_FIELD_MAXDAMAGE, c->GetFloatValue( UNIT_FIELD_MINDAMAGE ) );
					c->SetUInt64Value( UNIT_FIELD_CREATEDBY, _unit->GetGUID() );
					c->Despawn( 50 * 10000, 0 );
					break;
				}
			}
		}
	}
};
/*
//rogue ( melee )
//- fast small poison dmg
//- spawn near player
//- increase dmg on target
class RogueGuard : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(RogueGuard);
    RogueGuard(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
    void AIUpdate()
    {
	}
};

//shaman ( ranged ):
//- deal elemental dmg
//- heal
class ShamanGuard : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ShamanGuard);
    ShamanGuard(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
    void AIUpdate()
    {
	}
};
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SetupCustomMoguRaid(ScriptMgr* mgr)
{
	/*
	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111204, 'Soul Hungerer', 'CC & Tank & Heal & Spank', '', 0, 0, 7, 1, 3, 0, 0, 18953, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111204, 88, 88, 14, 500000, 500000, 100, 2, 0, 1000, 0, 666, 666, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111204, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111200, 'Large Tank', 'Hits Hard', '', 0, 0, 7, 1, 20, 0, 0, 26174, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111200, 88, 88, 14, 4000000, 4000000, 100, 1.5, 0, 3000, 0, 66666, 66666, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 7, 7, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111201, 'Small Tank', 'Hits Fast', '', 0, 0, 7, 1, 20, 0, 0, 24905, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111201, 88, 88, 14, 2000000, 2000000, 100, 0.7, 0, 500, 0, 33333, 33333, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111202, 'Large Heal', 'Heals Large', '', 0, 0, 7, 1, 20, 0, 0, 26382, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111202, 88, 88, 14, 8000000, 8000000, 100, 1.5, 0, 3000, 0, 36666, 36666, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 7, 7, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111203, 'Small Heal', 'Heals Fast', '', 0, 0, 7, 1, 20, 0, 0, 26617, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111203, 88, 88, 14, 6000000, 6000000, 100, 0.7, 0, 500, 0, 23333, 23333, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111205, 'Burns your feet', '', '', 0, 0, 7, 1, 20, 0, 0, 21666, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111205, 88, 88, 14, 6000000, 6000000, 100, 1, 0, 500, 0, 23333, 23333, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111206, 'THE SPONGE', 'heals from almost everything', '', 0, 0, 7, 1, 20, 0, 0, 26166, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111206, 88, 88, 14, 20000000, 20000000, 100, 1.5, 0, 2000, 0, 66666, 66666, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111207, 'Minie Sponge', 'heals from almost everything', '', 0, 0, 7, 1, 20, 0, 0, 26166, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111207, 88, 88, 14, 10000000, 10000000, 100, 1.5, 0, 2000, 0, 66666, 66666, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111208, 'Worm feeder', 'hatches egs', '', 0, 0, 7, 0, 4, 0, 0, 1062, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111208, 88, 88, 14, 10000000, 10000000, 100, 2.5, 0, 2000, 0, 66666, 66666, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111209, 'Worm segment', 'kills & eats', '', 0, 0, 7, 0, 4, 0, 0, 549, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111209, 88, 88, 14, 10000000, 10000000, 100, 1.5, 0, 2000, 0, 66666, 66666, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111210, 'Worm food', 'NPC seed', '', 0, 0, 7, 0, 4, 0, 0, 1056, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111210, 88, 88, 14, 10000000, 10000000, 100, 2.5, 0, 2000, 0, 66666, 66666, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111211, 'Frost guard', 'tanky', '', 0, 0, 7, 0, 4, 0, 0, 23726, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111211, 88, 88, 14, 2000000, 2000000, 100, 1, 0, 2000, 0, 50000, 50000, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111212, 'Nature guard', 'support', '', 0, 0, 7, 0, 4, 0, 0, 37163, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111212, 88, 88, 14, 8000000, 8000000, 100, 1, 0, 4000, 0, 20000, 20000, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111213, 'Nature keeper', 'support', '', 0, 0, 7, 0, 4, 0, 0, 864, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111213, 88, 88, 14, 1000000, 1000000, 100, 1, 0, 2000, 0, 10000, 10000, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111214, 'Shadow guard', 'support', '', 0, 0, 7, 0, 4, 0, 0, 278, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111214, 88, 88, 14, 6000000, 6000000, 100, 1, 0, 4000, 0, 20000, 20000, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	REPLACE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(111215, 'DPS food', 'tank', '', 0, 0, 7, 0, 4, 0, 0, 7550, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(111215, 88, 88, 14, 100000, 100000, 100, 1, 0, 1000, 3, 10000, 10000, 0, 0, 0, 36000000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 111200, 0, 0, 0, 0, 0);

	update creature_proto set modImmunities=0x7FFFFFFF,respawntime=0x7FFFFFFF where entry in ( 111206, 111207 );
	*/

	// scales perfectly to players
    mgr->register_creature_script(FAST_DPS_ENTRY,			&WeakDPSTester::Create);
    mgr->register_creature_script(STRONG_DPS_ENTRY,			&StrongDPSTester::Create);
    mgr->register_creature_script(FAST_HEAL_ENTRY,			&HOTHealTester::Create);
    mgr->register_creature_script(STRONG_HEAL_ENTRY,		&StrongHealTester::Create);
    mgr->register_creature_script(LOCAL_DPS_ENTRY,			&BurningFire::Create);
    mgr->register_creature_script(SOUL_HUNGERER,			&SoulHungerer::Create);

	//hard to DPS it
	mgr->register_creature_script(SEASONAL_SPONGE,			&DMGSponge::Create);
    mgr->register_creature_script(SEASONAL_SPONGE_CLONE,	&DMGSponge::Create);

	//coordination between players
	mgr->register_creature_script(EXTENSION_ID_PACMAN_WORM,&WormSpawnController::Create);
    mgr->register_creature_script(PACMAN_WORM_SEGMENT,		&WormSegment::Create);
    mgr->register_creature_script(EXTENSION_ID_PACMAN_WORM_FRUIT,		&WormFood::Create);

	//some semi weak guards
	mgr->register_creature_script(FROSTGUARD_ENTRY,			&FrostGuard::Create);
	mgr->register_creature_script(NATUREGUARD_ENTRY,		&NatureGuard::Create);
	mgr->register_creature_script(NATUREGUARDSUMMON_ENTRY,	&NatureGuardSummon::Create);
	mgr->register_creature_script(SHADOWGUARD_ENTRY,		&ShadowGuard::Create);
}
