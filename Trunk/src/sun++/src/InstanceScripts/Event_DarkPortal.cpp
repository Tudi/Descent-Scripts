/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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
#include "Event_DarkPortal.h"

/*
 * TO DO:
 *  2) Fix Timer for Alliance Summon (and remove double add - temporary way)
 *  I) Add spawning of special units On Creature AreaTrigger reach instead of specified unit states (eventually)
 *  D) Research aggro ranges for units and add modifiers for those
 *	D) Fix facing when going out of combat
 *  6) Check why Emote State is not applied in OnCombatStop Event
 *  7) Fix faction mess up and bow use of Darnassian Archers - DB related
 */

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Dark Portal Event Trigger AI

DarkPortalEventTriggerAI::DarkPortalEventTriggerAI( Creature *pCreature ) : MoonScriptCreatureAI( pCreature )
{
	// Trigger settings (added MoveTo to prevent situations when unit falls on first cell loading.
	MoveTo( _unit->GetSpawnX() + 5.0f, _unit->GetSpawnY(), _unit->GetSpawnZ() );
	SetCanEnterCombat(false);
	SetFlyMode(true);
	_unit->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );

	mAllianceSummonTimer = AddTimer( MIN_TIME_TO_SUMMON );
	mAllianceSummonTimer = AddTimer( MIN_TIME_TO_SUMMON );
	mHordeSummonTimer = AddTimer( MIN_TIME_TO_SUMMON );
	mInfernalSummonTimer = AddTimer( SUMMON_INFERNAL_TIME );
	mDemonRespawnTimer = AddTimer( DEMON_RESPAWN_TIME );
	mSpecialUnitCheckTimer = AddTimer( SPECIAL_UNIT_CHECK_TIME );
	mPitCommanderRespawnTimer = mAllianceMoveTimer = mHordeMoveTimer = mJustiMoveTimer = mJustiMoveTimer = INVALIDATE_TIMER;
	mPitCommander = mJusti = mMelgr = mAlliance = mHorde = NULL;
	mFormationChain = NULL;
	mInfernalSpawnPointId = 2;
	mHasSummonedInfernals = false;
	mResurrected = false;
	mResurrect = false;
	mHasLoadedCells = false;

	// AIUpdate registering
	RegisterAIUpdateEvent( 1000 );
}

void DarkPortalEventTriggerAI::AIUpdate()
{
	// Hack to prevent huge army spawning - not really sure if it should be here and not sure if it even works
	if ( _unit->GetMapMgr() == NULL )
		return;

	ParentClass::AIUpdate();

	if ( !mHasLoadedCells )
	{
		MapCell *pCell = _unit->GetMapMgr()->GetCellByCoords( _unit->GetPositionX(), _unit->GetPositionY() );
		if ( pCell != NULL )
		{
			// I'm lazy, slower, but simpler :P
			MapMgr *pMapMgr = _unit->GetMapMgr();
			float x, y;
			x = -453.933472f;
			while ( x < -28.010971f )
			{
				y = 882.065369f;
				while ( y < 1280.692871f )
				{
					MapCell *pCell = pMapMgr->GetCellByCoords( x, y );
					if ( pCell == NULL )
					{
						pCell = pMapMgr->CreateByCoords( x, y );
						if ( pCell != NULL )
						{
							pCell->Init( pMapMgr->GetPosX( x ), pMapMgr->GetPosY( y ), pMapMgr->GetMapId(), pMapMgr );
						}
					}

					if ( pCell != NULL && pMapMgr->m_forcedcells.find( pCell ) == pMapMgr->m_forcedcells.end() )
					{
						pMapMgr->AddForcedCell( pCell );
					}

					y += 40.0f;
				}

				x += 40.0f;
			}

			mHasLoadedCells = true;
		}
	}

	// Hack to prevent trigger falling from his default/spawn position
	if ( _unit->GetPositionX() != _unit->GetSpawnX() || _unit->GetPositionY() != _unit->GetSpawnY() || _unit->GetPositionZ() != _unit->GetSpawnZ() )
		MoveTo( _unit->GetSpawnX(), _unit->GetSpawnY(), _unit->GetSpawnZ() );

	// Function to ressurect Melgromm when he dies for the first time
	if ( mResurrect && !mResurrected )
	{
		// We check if he's in memory and is alive - if so we renew his HP and make him alive
		if ( mMelgr != NULL && mMelgr->GetUnit()->IsInWorld() && !mMelgr->IsAlive() )
		{
			sEventMgr.RemoveEvents( mMelgr->GetUnit(), EVENT_CREATURE_REMOVE_CORPSE );
			mMelgr->GetUnit()->SetUInt32Value( UNIT_FIELD_HEALTH, mMelgr->GetUnit()->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			mMelgr->GetUnit()->SetUInt32Value( UNIT_FIELD_POWER1, mMelgr->GetUnit()->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );
			mMelgr->GetUnit()->SetUInt32Value( UNIT_DYNAMIC_FLAGS, 0 );
			mMelgr->GetUnit()->setDeathState( ALIVE );
//			mMelgr->GetUnit()->Tagged = false;
			mMelgr->GetUnit()->TaggerGuid = false;
			mMelgr->GetUnit()->loot.gold = 0;
			mMelgr->GetUnit()->loot.looters.clear();
			mMelgr->GetUnit()->loot.items.clear();

			if ( mMelgr->GetWaypointCount() > 0 )
			{
				mMelgr->SetMoveType( Move_ForwardThenStop );
				if ( !mMelgr->GetCanEnterCombat() )
				{
					mMelgr->SetCanEnterCombat( true );
					mMelgr->SetWaypointToMove( 1 );
				}
				else
				{
					uint32 LastWP = static_cast< MelGrommHighmountainAI* >( mMelgr )->mLastWP;
					if ( LastWP == 0 )
						LastWP = 1;
					if ( LastWP >= mMelgr->GetWaypointCount() )
						LastWP = (uint32)(mMelgr->GetWaypointCount() - 1);

					mMelgr->SetWaypointToMove( LastWP );
				}
			}
		}

		// We set Ressed = true to prevent double ressurection
		mResurrected = true;
	}

	// Function written to set special units pointers as null
	if ( mJusti != NULL )
	{
		// We set mJusti = NULL when he's in memory, but is not alive or is not in world
		if ( !mJusti->GetUnit()->IsInWorld() || !mJusti->IsAlive() )
			mJusti = NULL;
	}
	if ( mMelgr != NULL )
	{
		// If Melgromm is in memory, but is not alive and wasn't ressurected yet we cast 'ress' spell on him
		if ( mMelgr->GetUnit()->IsInWorld() && !mMelgr->IsAlive() && !mResurrect && !mResurrected )
		{
			// We cast Self Ressurection spell on Melgromm
			mMelgr->GetUnit()->CastSpell( mMelgr->GetUnit(), dbcSpell.LookupEntry( DARK_PORTAL_EVENT_TRIGGER_REVIVE_SELF ), true );

			// We set Ress = true to prevent double spell casting
			mResurrect = true;
		}

		// If Melgromm's pointer is valid and unit is not alive or is not in world, but was resurrected we NULL it's pointer
		if ( ( !mMelgr->IsAlive() || !mMelgr->GetUnit()->IsInWorld() ) && mResurrect && mResurrected )
		{
			// We reset Melgromm ressurection settings

			mResurrected = false;
			mResurrect = false;

			// We null pointer
			mMelgr = NULL;
		}
	}

	// We check each 5 seconds (default setting) if special units should be spawned
	if ( IsTimerFinished( mSpecialUnitCheckTimer ) )
		SpecialUnitChecks();

	// We rise special units' move timer to let them move naturally (not right after being spawned)
	if ( IsTimerFinished( mJustiMoveTimer ) )
	{
		if ( mJusti != NULL && mJusti->IsAlive() )
		{
			mJusti->SetCanEnterCombat( true );
			mJusti->GetUnit()->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_PLAYER_CONTROLLED_CREATURE );
			mJusti->SetMoveType( Move_ForwardThenStop );
			mJusti->SetWaypointToMove( 1 );
		}

		RemoveTimer( mJustiMoveTimer );
	}
	if ( IsTimerFinished( mMelgrMoveTimer ) )
	{
		if ( mMelgr != NULL && mMelgr->IsAlive() )
		{
			mMelgr->SetCanEnterCombat( true );
			mMelgr->GetUnit()->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_PLAYER_CONTROLLED_CREATURE );
			mMelgr->SetMoveType( Move_ForwardThenStop );
			mMelgr->SetWaypointToMove( 1 );
		}

		RemoveTimer( mMelgrMoveTimer );
	}

	// If alliance portal unit was spawned we rise it's portal move timer to let it move naturally
	if ( IsTimerFinished( mAllianceMoveTimer ) )
	{
		if ( mAlliance != NULL )
		{
			if ( !mAlliance->GetUnit()->IsInWorld() || !mAlliance->IsAlive() )
			{
				mAlliance = NULL;
			}
			else if ( mAlliance->IsAlive() )
			{
				mAlliance->SetCanEnterCombat( true );
				mAlliance->GetUnit()->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_PLAYER_CONTROLLED_CREATURE );
				mAlliance->SetMoveType( Move_ForwardThenStop );
				mAlliance->SetWaypointToMove( 1 );
			}
		}

		RemoveTimer( mAllianceMoveTimer );
	}

	// If horde portal unit was spawned we rise it's portal move timer to let it move naturally
	if ( IsTimerFinished( mHordeMoveTimer ) )
	{
		if ( mHorde != NULL )
		{
			if ( !mHorde->GetUnit()->IsInWorld() || !mHorde->IsAlive() )
			{
				mHorde = NULL;
			}
			else if ( mHorde->IsAlive() )
			{
				mHorde->SetCanEnterCombat( true );
				mHorde->GetUnit()->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_PLAYER_CONTROLLED_CREATURE );
				mHorde->SetMoveType( Move_ForwardThenStop );
				mHorde->SetWaypointToMove( 1 );
			}
		}

		RemoveTimer( mHordeMoveTimer );
	}

	// We try to spawn units whenever timer is finihed or we hadn't spawn new unit before
	if ( IsTimerFinished( mAllianceSummonTimer ) )
	{
		SpawnAllianceUnit();
	}
	if ( IsTimerFinished( mHordeSummonTimer ) )
	{
		SpawnHordeUnit();
	}

	// If Infernal Summon timer is equal to 0 we let Pit Commander (if he exists) cast spell to summon infernals
	if ( IsTimerFinished( mInfernalSummonTimer ) )
	{
		if ( !mHasSummonedInfernals )
		{
			PitCommanderInfernalSummon();
		}

		// After 3 seconds of casting spells to summon infernals we spawn them manually
		else
		{
			for ( uint32 i = 0; i < 2; ++i )
			{
				MoonScriptCreatureAI *pInfernal = SpawnCreature( CN_INFERNAL_SIEGEBREAKER, Infernals[mInfernalSpawnPointId + i * 2].mX, Infernals[mInfernalSpawnPointId + i * 2].mY, Infernals[mInfernalSpawnPointId + i * 2].mZ, Infernals[mInfernalSpawnPointId + i * 2].mO );
				if ( pInfernal != NULL )
				{
					pInfernal->GetUnit()->GetAIInterface()->skip_reset_hp = true;
					pInfernal->GetUnit()->m_canRegenerateHP = false;
					pInfernal->GetUnit()->m_noRespawn = true;
					mInfernalTable.insert( pInfernal );
				}
			}

			ResetTimer( mInfernalSummonTimer, 47000 );
			mHasSummonedInfernals = false;
		}
	}

	if ( IsTimerFinished( mDemonRespawnTimer ) )
	{
		// After 20 seconds of spawning demons we command 5 of them (if it's 10-demon-formation) to attack
		// After 50 seconds of first 5-demon-formation attack we let another 5 demons to attack Dark Portal (only if 10-demon-formation was spawned)
		if ( mQueuedDemonSet.size() > 0 )
		{
			PitCommanderAttackCmd();
		}

		// If PitCommanderRespawn run out we spawn (if we can) another demon formation to take over Dark Portal
		else
		{
			PitCommanderSpawn();
		}
	}
}

// Function to check Commander/Lieutenant units' states and spawn special units if needed
void DarkPortalEventTriggerAI::SpecialUnitChecks()
{
	// If Justi hasn't joined fight yet we check if he should
	if ( mJusti == NULL )
	{
		// We get Justinius unit for further use
		mJusti = SpawnCreature( CN_JUSTINIUS_THE_HARBINGER, AHSPoints[0].mX, AHSPoints[0].mY, AHSPoints[0].mZ, AHSPoints[0].mO );
		if ( mJusti != NULL )
		{
			mJusti->SetCanEnterCombat( false );
			mJusti->SetMoveType( Move_DontMoveWP );
			mJusti->GetUnit()->GetAIInterface()->m_moveBackward = true;
			mJusti->GetUnit()->m_noRespawn = true;
			AddUnitToChainAggro( mJusti->GetUnit() );

			for ( uint32 x = 1; x < 11; ++x )
			{
				WayPoint *pWaypoint = NULL;
				if ( x == 10 )
				{
					pWaypoint = CreateWaypoint( x, 0, Flag_Run, SWP[0] );
				}
				else
				{
					pWaypoint = CreateWaypoint( x, 0, Flag_Run, AWP[x] );
				}

				if ( x == 9 )
				{
					pWaypoint->waittime = 3000;
				}

				mJusti->AddWaypoint( pWaypoint );
			}

			// We reset movement variable
			mJustiMoveTimer = AddTimer( 5000 );
		}
	}

	// If Melgr hasn't joined fight yet we check if he should
	if ( mMelgr == NULL )
	{
		// We get Melgromm unit for further use
		mMelgr = SpawnCreature( CN_MELGROMM_HIGHMOUNTAIN, AHSPoints[1].mX, AHSPoints[1].mY, AHSPoints[1].mZ, AHSPoints[1].mO );
		if ( mMelgr != NULL )
		{
			mMelgr->SetCanEnterCombat( false );
			mMelgr->SetMoveType( Move_DontMoveWP );
			mMelgr->GetUnit()->GetAIInterface()->m_moveBackward = true;
			mMelgr->GetUnit()->m_noRespawn = true;
			AddUnitToChainAggro( mMelgr->GetUnit() );

			for ( uint32 x = 1; x < 12; ++x )
			{
				WayPoint *pWaypoint = NULL;
				if ( x == 11 )
				{
					pWaypoint = CreateWaypoint( x, 0, Flag_Run, SWP[1] );
				}
				else
				{
					pWaypoint = CreateWaypoint( x, 0, Flag_Run, HWP[x] );
				}

				if ( x == 10 )
				{
					pWaypoint->waittime = 3000;
				}

				mMelgr->AddWaypoint( pWaypoint );
			}

			// We reset movement variable
			mMelgrMoveTimer = AddTimer( 5000 );
		}
	}

	// We reset special unit check timer to 0 to prevent check triggering every second
	ResetTimer( mSpecialUnitCheckTimer, SPECIAL_UNIT_CHECK_TIME );
}

// Bool used to check if we can spawn any unit for this formation to prevent its messing
bool DarkPortalEventTriggerAI::IsRegimentFull(AIContainer VectorSet, size_t SetSize)
{
	// We check size of unit list - if it's smaller than meant size we return true - may be spawned
	if ( VectorSet.size() < SetSize )
		return false;

	// If not we check all units to find out if we can replace any
	for ( uint32 i = 0; i < SetSize; ++i )
	{
		// We check every unit from our set
		MoonScriptCreatureAI *pSetUnit = VectorSet[i];

		// If unit doesn't exist or exists, but is not alive or is not in world we return true and break loop
		if ( pSetUnit == NULL || !pSetUnit->IsAlive() || !pSetUnit->GetUnit()->IsInWorld() )
		{
			return false;
		}
	}

	// If it comes to this point it means regiment is full and does not need replacements
	return true;
}

// Function used to check if any alliance unit should be spawned
void DarkPortalEventTriggerAI::SpawnAllianceUnit()
{
	size_t Pos = 0;
	uint32 Entry = 0;
	uint32 Change = RandomUInt(6);
	AIContainer *pList = NULL;
	Coords *pCoords = NULL;

	// Default unit to spawn is basic close combat soldier
	if ( !IsRegimentFull( mAllianceMelee, 10 ) )
	{
		// We change Entry and set position count in which units may be spawned
		if ( Change == 1 )
			Entry = CN_IRONFORGE_PALADIN;
		else
			Entry = CN_STORMWIND_SOLDIER;
		pList = &mAllianceMelee;
		pCoords = AMWP;
		Pos = 10;
	}

	// Default unit to spawn is ranged combat soldier if melee regiment is full or if it's rolled
	if ( !IsRegimentFull( mAllianceRanged, 6 ) && ( Entry == 0 || Change == 2 ) )
	{
		// We change Entry and set position count in which units may be spawned
		Entry = CN_DARNASSIAN_ARCHER;
		pList = &mAllianceRanged;
		pCoords = ARWP;
		Pos = 6;
	}

	// Default unit to spawn is caster if melee and ranged regiments are full or if it's rolled
	if ( !IsRegimentFull( mAllianceCaster, 2 ) && ( Entry == 0 || Change == 3 ) )
	{
		// We change Entry and set position count in which units may be spawned
		Entry = CN_STORMWIND_MAGE;
		pList = &mAllianceCaster;
		pCoords = ACWP;
		Pos = 2;
	}

	// No need to go through rest of code if all regiments are full
	if ( Entry == 0 )
		return;

	// Adding false unit to expand our container
	if ( pList->size() < Pos )
	{
		if ( pList->size() == 0 || ( pList->size() > 0 && pList->back() != NULL ) )
		{
			pList->push_back( NULL );
		}
	}

	// We go through all positions and check units
	for ( uint32 i = 0; i < pList->size(); ++i )
	{
		// We set unit to check
		MoonScriptCreatureAI *SetUnit = (*pList)[i];

		// If unit does not exist or exists, but is not alive or in world and unit wasn't already spawned we spawn one
		if ( SetUnit == NULL || !SetUnit->IsAlive() || !SetUnit->GetUnit()->IsInWorld() )
		{
			SetUnit = mAlliance = SpawnCreature( Entry, ASPoints[0].mX, ASPoints[0].mY, ASPoints[0].mZ, ASPoints[0].mO );
			// We set move out timer to let those units move after exact time and stop looping though our units
			if ( SetUnit != NULL )
			{
				SetUnit->SetCanEnterCombat( false );
				SetUnit->SetMoveType( Move_DontMoveWP );
				SetUnit->GetUnit()->GetAIInterface()->m_moveBackward = true;
				SetUnit->GetUnit()->m_noRespawn = true;
				AddUnitToChainAggro( SetUnit->GetUnit() );

				for ( uint32 x = 1; x < 11; ++x )
				{
					WayPoint *pWaypoint = NULL;
					if ( x != 10 )
					{
						pWaypoint = CreateWaypoint( x, 0, Flag_Run, AWP[x] );
					}
					else
					{
						pWaypoint = CreateWaypoint( x, 0, Flag_Run, pCoords[i] );
					}

					if ( x == 9 )
					{
						pWaypoint->waittime = 3000;
					}

					SetUnit->AddWaypoint( pWaypoint );
				}
			}

			mAllianceMoveTimer = AddTimer( 3000 );
			(*pList)[i] = SetUnit;
			break;
		}
	}

	ResetTimer( mAllianceSummonTimer, SPAWN_NEW_UNIT_PORTAL );
}

// Function used to check if any horde unit should be spawned
void DarkPortalEventTriggerAI::SpawnHordeUnit()
{
	// Variable initialization
	uint32 Pos = 0;
	uint32 Entry = 0;
	uint32 Change = RandomUInt(6);
	AIContainer *pList = NULL;
	Coords *pCoords = NULL;

	// Default unit to spawn is basic close combat soldier
	if ( !IsRegimentFull( mHordeMelee, 10 ) )
	{
		// We change Entry and set position count in which units may be spawned
		if ( Change == 1 )
			Entry = CN_ORGRIMMAR_SHAMAN;
		else
			Entry = CN_ORGRIMMAR_GRUNT;
		pList = &mHordeMelee;
		pCoords = HMWP;
		Pos = 10;
	}

	// Default unit to spawn is ranged combat soldier if melee regiment is full or if it's rolled
	if ( !IsRegimentFull( mHordeRanged, 6 ) && ( Entry == 0 || Change == 2 ) )
	{
		// We change Entry and set position count in which units may be spawned
		Entry = CN_DARKSPEAR_AXE_THROWER;
		pList = &mHordeRanged;
		pCoords = HRWP;
		Pos = 6;
	}

	// Default unit to spawn is caster if melee and ranged regiments are full or if it's rolled
	if ( !IsRegimentFull( mHordeCaster, 2 ) && ( Entry == 0 || Change == 3 ) )
	{
		// We change Entry and set position count in which units may be spawned
		Entry = CN_UNDERCITY_MAGE;
		pList = &mHordeCaster;
		pCoords = HCWP;
		Pos = 2;
	}

	// No need to go through rest of code if all regiments are full
	if ( Entry == 0 )
		return;

	// Adding false unit to expand our container
	if ( pList->size() < Pos )
	{
		if ( pList->size() == 0 || ( pList->size() > 0 && pList->back() != NULL ) )
		{
			pList->push_back( NULL );
		}
	}
	// We go through all positions and check units
	for ( uint32 i = 0; i < pList->size(); ++i )
	{
		// We set unit to check
		MoonScriptCreatureAI *SetUnit = (*pList)[i];

		// If unit does not exist or exists, but is not alive or in world and unit wasn't already spawned we spawn one
		if ( SetUnit == NULL || !SetUnit->IsAlive() || !SetUnit->GetUnit()->IsInWorld() )
		{
			SetUnit = mHorde = SpawnCreature( Entry, HSPoints[0].mX, HSPoints[0].mY, HSPoints[0].mZ, HSPoints[0].mO );
			// We set move out timer to let those units move after exact time and stop looping though our units
			if ( SetUnit != NULL )
			{
				SetUnit->SetCanEnterCombat( false );
				SetUnit->SetMoveType( Move_DontMoveWP );
				SetUnit->GetUnit()->GetAIInterface()->m_moveBackward = true;
				SetUnit->GetUnit()->m_noRespawn = true;
				AddUnitToChainAggro( SetUnit->GetUnit() );

				for ( uint32 x = 1; x < 12; ++x )
				{
					WayPoint *pWaypoint = NULL;
					if ( x != 11 )
					{
						pWaypoint = CreateWaypoint( x, 0, Flag_Run, HWP[x] );
					}
					else
					{
						pWaypoint = CreateWaypoint( x, 0, Flag_Run, pCoords[i] );
					}

					if ( x == 10 )
					{
						pWaypoint->waittime = 3000;
					}

					SetUnit->AddWaypoint( pWaypoint );
				}
			}

			mHordeMoveTimer = AddTimer( 3000 );
			(*pList)[i] = SetUnit;
			break;
		}
	}

	ResetTimer( mHordeSummonTimer, SPAWN_NEW_UNIT_PORTAL );
}

void DarkPortalEventTriggerAI::AddUnitToChainAggro(Unit* pUnit)
{
	if ( pUnit == NULL || !pUnit->isAlive() || !pUnit->IsInWorld() )
		return;

	if ( mFormationChain == NULL )
	{
		mFormationChain = new UnitChain( true, 40.0f );
		if ( mFormationChain == NULL )
			return;
	}

	mFormationChain->AddUnit( pUnit );
}

void DarkPortalEventTriggerAI::PitCommanderChecks()
{
	// We check if Pit Commander is on the battlefield, alive and ready to fight if not 
	if ( mPitCommander == NULL || !mPitCommander->IsAlive() || !mPitCommander->GetUnit()->IsInWorld() )
	{
		if ( mPitCommanderRespawnTimer == INVALIDATE_TIMER )
		{
			if ( mPitCommander != NULL )
			{
				mPitCommander = NULL;
			}
			else
			{
				Unit *pPitCommander = ForceCreatureFind( CN_PIT_COMMANDER, -249.119995f, 1169.209961f, 41.552101f, false );
				if ( pPitCommander != NULL && pPitCommander->isAlive() && static_cast< Creature* >( pPitCommander )->GetScript() != NULL )
				{
					mPitCommander = static_cast< MoonScriptCreatureAI* >( static_cast< Creature* >( pPitCommander )->GetScript() );
					static_cast< Creature* >( pPitCommander )->m_noRespawn = true;
					return;
				}
			}

			mPitCommanderRespawnTimer = AddTimer( 1200000 );
		}
		else if ( IsTimerFinished( mPitCommanderRespawnTimer ) )
		{
			mPitCommander = SpawnCreature( CN_PIT_COMMANDER, -249.119995f, 1169.209961f, 41.552101f, 4.698460f );
			if ( mPitCommander != NULL )
			{
				mPitCommander->GetUnit()->m_noRespawn = true;
			}

			RemoveTimer( mPitCommanderRespawnTimer );
		}
	}
}

// Function used to summon infernals
void DarkPortalEventTriggerAI::PitCommanderInfernalSummon()
{
	// We run through whole Infernal list
	if ( mInfernalTable.size() > 0 )
	{
		// We iterate through list and remove from it invalid, not in world or not dead creatures
		for ( AISet::iterator itr = mInfernalTable.begin(); itr != mInfernalTable.end(); )
		{
			if ( (*itr) == NULL || !(*itr)->GetUnit()->IsInWorld() || !(*itr)->IsAlive() )
			{
				if ( (*itr) != NULL && !(*itr)->IsAlive() )
				{
					(*itr)->Despawn( 1000 );
				}

				mInfernalTable.erase( itr++ );
				continue;
			}

			++itr;
		}
	}

	// We validate Pit Commander for further use
	PitCommanderChecks();

	// We check if Pit Commander pointer is valid and if he's alive and we have on battlefield less than 3 infernals
	if ( mPitCommander != NULL && mInfernalTable.size() <= 2 )
	{
		// We force Pit Commander to cast spell that looks like summoning infernals
		mPitCommander->GetUnit()->CastSpell( mPitCommander->GetUnit(), DARK_PORTAL_EVENT_TRIGGER_SUMMON_INFERNALS, true );

		// We roll Infernals' spawn positions
		uint32 i = mInfernalSpawnPointId;
		while ( i == mInfernalSpawnPointId )
		{
			i = RandomUInt(1);
		}

		// We force trigger to "throw huge fel stones" from it's actual position
		_unit->CastSpellAoF( NULL, Infernals[i].mX, Infernals[i].mY, Infernals[i].mZ, dbcSpell.LookupEntry(DARK_PORTAL_EVENT_TRIGGER_SUMMON_INFERNALS2), true );
		_unit->CastSpellAoF( NULL, Infernals[i + 2].mX, Infernals[i + 2].mY, Infernals[i + 2].mZ, dbcSpell.LookupEntry(DARK_PORTAL_EVENT_TRIGGER_SUMMON_INFERNALS2), true );

		// We mark infernals as spawned
		ResetTimer( mInfernalSummonTimer, 3000 );
		mHasSummonedInfernals = true;
		mInfernalSpawnPointId = i;
		return;							// will prevent adding infernals and letting demon army to move in the same time
	}

	// We reset InfernalSpawn timer to default value (50 sec)
	ResetTimer( mInfernalSummonTimer, SUMMON_INFERNAL_TIME );
}

// Function we use to send 5 demons to attack Dark Portal defenders
void DarkPortalEventTriggerAI::PitCommanderAttackCmd()
{
	// We validate Pit Commander
	PitCommanderChecks();

	// If pointer is valid, Pit Commander is not in combat we randomize emoting
	if ( mPitCommander != NULL && mPitCommander->GetUnit()->GetAIInterface()->GetNextTarget() == NULL && RandomUInt(3) == 1 )
		mPitCommander->GetUnit()->Emote( EMOTE_ONESHOT_POINT );

	// If DemonTable size is equal to 5 we send all demons to fight
	bool MultiWaves = false;
	if ( mQueuedDemonSet.size() > 5 && mQueuedDemonSet.size() % 2 == 0 )
	{
		// Set to second wave stuff
		MultiWaves = true;
	}

	for ( AISet::iterator itr = mQueuedDemonSet.begin(); itr != mQueuedDemonSet.end(); )
	{
		// If Demon's data was not saved saved to memory, creature is not in world or is not alive we remove it from our set
		if ( (*itr) == NULL ||  !(*itr)->IsAlive() || !(*itr)->GetUnit()->IsInWorld() )
		{
			if ( (*itr) != NULL && !(*itr)->IsAlive() && (*itr)->GetUnit()->IsInWorld() )
				(*itr)->Despawn( 1000 );

			AISet::iterator it2 = mFightingDemonSet.find( (*itr) );
			if ( it2 != mFightingDemonSet.end() )
				mFightingDemonSet.erase( it2 );
		}
		else
		{
			(*itr)->SetMoveType( Move_ForwardThenStop );
			(*itr)->SetWaypointToMove( 3 );
		}

		mQueuedDemonSet.erase( itr++ );
		if ( itr != mQueuedDemonSet.end() && MultiWaves )
			++itr;
	}

	if ( mQueuedDemonSet.size() > 0 )
		ResetTimer( mDemonRespawnTimer, 50000 );
	else
		ResetTimer( mDemonRespawnTimer, 160000 );
}

// Void used to spawn demons
void DarkPortalEventTriggerAI::PitCommanderSpawn()
{
	// We reset Respawn timer and initialize other variables
	ResetTimer( mDemonRespawnTimer, 20000 );

	// We run through set of objects and check how many spawned demons are still fighting
	for ( AISet::iterator itr = mFightingDemonSet.begin(); itr != mFightingDemonSet.end(); )
	{
		if ( (*itr) != NULL || !(*itr)->IsAlive() || !(*itr)->GetUnit()->IsInWorld() )
		{
			if ( (*itr) != NULL && !(*itr)->IsAlive() && (*itr)->GetUnit()->IsInWorld() )
				(*itr)->Despawn( 1000 );

			mFightingDemonSet.erase( itr++ );
		}
	}

	// We validate Pit Commander
	PitCommanderChecks();

	if ( mPitCommander == NULL )
	{
		ResetTimer( mDemonRespawnTimer, 60000 );
		return;
	}

	// We count how many demons will be spawned
	uint32 WaveSize = 1;
	size_t DemonCount = mFightingDemonSet.size();
	if ( DemonCount > 0 )
	{
		float Rate = ( float )DemonCount / 5;
		if ( Rate > 2 )
		{
			ResetTimer( mDemonRespawnTimer, 60000 );
			return;
		}

		WaveSize = ( uint32 )Rate;
		if ( WaveSize < 1 )
			WaveSize = 1;
	}

	// 1 / 3 waves will include 10 demons
	if ( WaveSize == 1 )
	{
		if ( RandomUInt(3) > 0 )
			WaveSize = 2;
	}

	for ( int i = 0; i < 10; i += WaveSize )
	{
		uint32 Entry = CN_FEL_SOLDIER;
		if ( RandomUInt(3) == 1 )
			Entry = CN_WRATH_MASTER;

		MoonScriptCreatureAI *pAI = SpawnCreature( Entry, DSPoints[i].mX, DSPoints[i].mY, DSPoints[i].mZ, DSPoints[i].mO );
		if ( pAI != NULL )
		{
			pAI->SetMoveType( Move_ForwardThenStop );
			pAI->GetUnit()->GetAIInterface()->m_moveBackward = true;
			pAI->GetUnit()->GetAIInterface()->skip_reset_hp = true;
			pAI->GetUnit()->m_canRegenerateHP = false;
			pAI->GetUnit()->m_noRespawn = true;

			uint32 j = 8 * i;
			for ( int x = 1; x < 9; ++x )
			{
				++j;
				pAI->AddWaypoint( CreateWaypoint( x, 0, Flag_Run, PitDemonPaths[j] ) );
			}

			mFightingDemonSet.insert( pAI );
			mQueuedDemonSet.insert( pAI );
		}
	}		
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Mel Gromm Highmountain AI

MelGrommHighmountainAI::MelGrommHighmountainAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell( MELGROMM_HIGHMOUNTAIN_EARTH_SHOCK, Target_Current, 8, 0, 10, 0, 20 );
	AddSpell( MELGROMM_HIGHMOUNTAIN_CHAIN_LIGHTNING, Target_Current, 7, 3, 20, 0, 20 );
	AddSpell( MELGROMM_HIGHMOUNTAIN_CHAIN_HEAL, Target_WoundedFriendly, 7, 2.5, 20, 0, 40 );
	AddSpell( MELGROMM_HIGHMOUNTAIN_STRENGHT_OF_THE_STORM_TOTEM, Target_Self, 5, 0, 25 );
	AddSpell( MELGROMM_HIGHMOUNTAIN_MAGMA_FLOW_TOTEM, Target_Self, 5, 0, 25 );
	SetWieldWeapon( true );

	mLastWP = 0;
}

void MelGrommHighmountainAI::OnCombatStop(Unit *mTarget)
{
	ParentClass::OnCombatStop( mTarget );
	if ( RandomUInt(3) == 1 )
	{
		Emote( "The attackers shall not succeed!  Strength and honor to the Horde!", Text_Yell, 0 );
	}

	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	}
}

void MelGrommHighmountainAI::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	mLastWP = iWaypointId;
	switch ( iWaypointId )
	{
	case 10:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 11:{
		_unit->SetFacing( SWP[1].mO );
		WayPoint *pWaypoint = _unit->GetAIInterface()->getWayPoint( 11 );
		if ( pWaypoint != NULL )
		{
			_unit->SetSpawnPosition( pWaypoint->x, pWaypoint->y, pWaypoint->z, pWaypoint->o );
		}

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	}
}

void MelGrommHighmountainAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		if ( pAI->mMelgr == static_cast< MoonScriptCreatureAI* >( this ) )
		{
			pAI->mMelgr = NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Orgrimmar Grunt AI

OrgrimmarGruntAI::OrgrimmarGruntAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell( ORGRIMMAR_GRUNT_IRON_BITE, Target_Current, 15, 0, 5, 0, 5 );	// is 5 enough?
	SetWieldWeapon( true );

	mLastWP = 0;
}

void OrgrimmarGruntAI::OnCombatStop(Unit *mTarget)
{
	ParentClass::OnCombatStop( mTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	}
}

void OrgrimmarGruntAI::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	mLastWP = iWaypointId;
	switch ( iWaypointId )
	{
	case 10:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 11:{
		_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, 375 );
		WayPoint *pWaypoint = _unit->GetAIInterface()->getWayPoint( 11 );
		if ( pWaypoint != NULL )
		{
			_unit->SetSpawnPosition( pWaypoint->x, pWaypoint->y, pWaypoint->z, pWaypoint->o );
		}

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	}
}

void OrgrimmarGruntAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		for ( size_t i = 0; i < pAI->mHordeMelee.size(); ++i )
		{
			if ( pAI->mHordeMelee[i] == static_cast< MoonScriptCreatureAI* >( this ) )
			{
				pAI->mHordeMelee[i] = NULL;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Orgrimmar Shaman AI

OrgrimmarShamanAI::OrgrimmarShamanAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell( ORGRIMMAR_SHAMAN_FLAME_SHOCK, Target_Current, 8, 0, 15, 0, 20 );
	AddSpell( ORGRIMMAR_SHAMAN_BLOODLUST, Target_RandomFriendly, 6, 0, 10 );
	AddSpell( ORGRIMMAR_SHAMAN_LIGHTNING_SHIELD, Target_Self, 5, 0, 25 );
	AddSpell( ORGRIMMAR_SHAMAN_SLOW, Target_RandomUnit, 6, 0, 10, 0, 30, true );
	SetWieldWeapon( true );

	mLastWP = 0;
}

void OrgrimmarShamanAI::OnCombatStop(Unit *mTarget)
{
	ParentClass::OnCombatStop( mTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	}
}

void OrgrimmarShamanAI::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	mLastWP = iWaypointId;
	switch ( iWaypointId )
	{
	case 10:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 11:{
		_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, 45 );
		WayPoint *pWaypoint = _unit->GetAIInterface()->getWayPoint( 11 );
		if ( pWaypoint != NULL )
		{
			_unit->SetSpawnPosition( pWaypoint->x, pWaypoint->y, pWaypoint->z, pWaypoint->o );
		}

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	}
}

void OrgrimmarShamanAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		for ( size_t i = 0; i < pAI->mHordeMelee.size(); ++i )
		{
			if ( pAI->mHordeMelee[i] == static_cast< MoonScriptCreatureAI* >( this ) )
			{
				pAI->mHordeMelee[i] = NULL;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Darkspear Axe Thrower AI

DarkspearAxeThrowerAI::DarkspearAxeThrowerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell( DARKSPEAR_AXE_THROWER_THROW, Target_Current, 100, 0.5f, 0, 8, 35, true );
	SetWieldWeapon( true );

	mLastWP = 0;
}

void DarkspearAxeThrowerAI::OnCombatStart(Unit *mTarget)
{
	ParentClass::OnCombatStart( mTarget );
	if ( GetRangeToUnit( mTarget ) >= 8.0f && GetRangeToUnit( mTarget ) <= 35.0f )
	{
		SetBehavior( Behavior_Spell );
		SetCanMove( false );
	}
}

void DarkspearAxeThrowerAI::OnCombatStop(Unit *mTarget)
{
	ParentClass::OnCombatStop( mTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	}
}

void DarkspearAxeThrowerAI::AIUpdate()
{
	ParentClass::AIUpdate();
	Unit *pTarget = _unit->GetAIInterface()->GetNextTarget();
	if ( pTarget != NULL )
	{
		if ( GetRangeToUnit( pTarget ) >= 8.0f && GetRangeToUnit( pTarget ) <= 35.0f )
		{
			SetBehavior( Behavior_Spell);
			SetCanMove( false );
		}
	}
}

void DarkspearAxeThrowerAI::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	mLastWP = iWaypointId;
	switch ( iWaypointId )
	{
	case 10:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 11:{
		WayPoint *pWaypoint = _unit->GetAIInterface()->getWayPoint( 11 );
		if ( pWaypoint != NULL )
		{
			_unit->SetSpawnPosition( pWaypoint->x, pWaypoint->y, pWaypoint->z, pWaypoint->o );
		}

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	}
}

void DarkspearAxeThrowerAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		for ( size_t i = 0; i < pAI->mHordeRanged.size(); ++i )
		{
			if ( pAI->mHordeRanged[i] == static_cast< MoonScriptCreatureAI* >( this ) )
			{
				pAI->mHordeRanged[i] = NULL;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Undercity Mage AI

UndercityMageAI::UndercityMageAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell( UNDERCITY_MAGE_ICEBOLT, Target_Current, 60, 0, 0, 0, 100 );
	AddSpell( UNDERCITY_MAGE_FIREBALL, Target_Current, 25, 3, 10, 0, 100 );
	AddSpell( UNDERCITY_MAGE_BLIZZARD, Target_RandomDestination, 15, 2, 10, 0, 100 );
	SetWieldWeapon( true );

	mLastWP = 0;
}

void UndercityMageAI::OnCombatStart(Unit *mTarget)
{
	ParentClass::OnCombatStart( mTarget );
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
}

void UndercityMageAI::OnCombatStop(Unit *mTarget)
{
	ParentClass::OnCombatStop( mTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	}
}

void UndercityMageAI::AIUpdate()
{
	ParentClass::AIUpdate();
	Unit *pTarget = _unit->GetAIInterface()->GetNextTarget();
	if ( pTarget != NULL )
	{
		if ( GetRangeToUnit( pTarget ) <= 100.0f )
		{
			SetBehavior( Behavior_Spell );
			SetCanMove( false );
		}
		else
		{
			_unit->GetAIInterface()->RemoveThreatByPtr( pTarget );
		}
	}
}

void UndercityMageAI::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	mLastWP = iWaypointId;
	switch ( iWaypointId )
	{
	case 10:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 11:{
		WayPoint *pWaypoint = _unit->GetAIInterface()->getWayPoint( 11 );
		if ( pWaypoint != NULL )
		{
			_unit->SetFacing( pWaypoint->o );
			_unit->SetSpawnPosition( pWaypoint->x, pWaypoint->y, pWaypoint->z, pWaypoint->o );
		}

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	}
}

void UndercityMageAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		for ( size_t i = 0; i < pAI->mHordeCaster.size(); ++i )
		{
			if ( pAI->mHordeCaster[i] == static_cast< MoonScriptCreatureAI* >( this ) )
			{
				pAI->mHordeCaster[i] = NULL;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Justinius The Harbinger AI

JustiniusTheHarbingerAI::JustiniusTheHarbingerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell( JUSTINIUS_THE_HARBINGER_FLASH_OF_LIGHT, Target_WoundedFriendly, 8, 1.5, 10, 0, 40 );
	AddSpellFunc( &SpellFunc_Justinius_Consecration, Target_Self, 6, 0, 5 );
	mConsecration = AddSpell( JUSTINIUS_THE_HARBINGER_CONSECRATION, Target_Self, 0, 0, 15 );
	AddSpell( JUSTINIUS_THE_HARBINGER_JUDGEMENT_OF_COMMAND, Target_Current, 7, 0, 20, 0, 35 );
	AddSpell( JUSTINIUS_THE_HARBINGER_DIVINE_SHIELD, Target_Self, 7, 0, 20 );
	AddSpell( JUSTINIUS_THE_HARBINGER_GREATER_BLESSING_OF_MIGHT, Target_Self, 5, 0, 30 );
	SetWieldWeapon( true );

	mLastWP = 0;
}

void JustiniusTheHarbingerAI::OnCombatStart(Unit* mTarget)
{
	ParentClass::OnCombatStart( mTarget );
	Emote( "", Text_Yell, 9705 );
	_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, 375 );
}

void JustiniusTheHarbingerAI::OnCombatStop(Unit *mTarget)
{
	ParentClass::OnCombatStop( mTarget );
	if ( RandomUInt(3) == 1 )
	{
		Emote( "Behold the power of the Light! Grace and glory to the Alliance!", Text_Yell, 0 );
	}

	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	}
}

bool JustiniusTheHarbingerAI::CanCastConsecration()
{
	UnitArray TargetArray;
//	for ( InRangeSet::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); ++itr )
	InrangeLoopExitAutoCallback AutoLock;
	for (InRangeSetRecProt::iterator itr = _unit->GetInRangeSetBegin( AutoLock ); itr != _unit->GetInRangeSetEnd(); itr++)
	if( isAttackable(_unit, (*itr) ) )
	{
		if ( IsValidUnitTarget( *itr, TargetFilter_None ) && GetRangeToUnit( static_cast< Unit* >( *itr ) ) <= 8.0f  )	// Set from 5 to 8 for better effect
		{
			TargetArray.push_back( static_cast< Unit* >( *itr ) );
		}
	}

	if ( TargetArray.size() >= 5 )
		return true;

	return false;
}

void JustiniusTheHarbingerAI::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	mLastWP = iWaypointId;
	switch ( iWaypointId )
	{
	case 9:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 10:{
		_unit->SetFacing( SWP[0].mO );
		WayPoint *pWaypoint = _unit->GetAIInterface()->getWayPoint( 10 );
		if ( pWaypoint != NULL )
		{
			_unit->SetSpawnPosition( pWaypoint->x, pWaypoint->y, pWaypoint->z, pWaypoint->o );
		}

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	}
}

void JustiniusTheHarbingerAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		if ( pAI->mJusti == static_cast< MoonScriptCreatureAI* >( this ) )
		{
			pAI->mJusti = NULL;
		}
	}
}

void SpellFunc_Justinius_Consecration(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, Unit *pTarget, TargetType pType)
{
	JustiniusTheHarbingerAI *pJustiniusAI = ( pCreatureAI ) ? ( JustiniusTheHarbingerAI* )pCreatureAI : NULL;
	if ( pJustiniusAI != NULL && pJustiniusAI->mConsecration != NULL )
	{
		if ( pJustiniusAI->mConsecration->mLastCastTime + pJustiniusAI->mConsecration->mCooldown <= ( uint32 )time( NULL ) )
		{
			if ( pJustiniusAI->CanCastConsecration() && !pJustiniusAI->IsCasting() )	// That's just for 'freaking out' occasions
			{
				pJustiniusAI->CastSpellNowNoScheduling( pJustiniusAI->mConsecration );
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Stormwind Soldier AI

StormwindSoldierAI::StormwindSoldierAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell( STORMWIND_SOLDIER_STRIKE, Target_Current, 15, 0, 5, 0, 5 );	// 5 enough?
	AddSpell( STORMWIND_SOLDIER_DEMORALIZING_SHOUT, Target_Self, 7, 0, 20 );
	SetWieldWeapon( true );

	mLastWP = 0;
}

void StormwindSoldierAI::OnCombatStop(Unit *mTarget)
{
	ParentClass::OnCombatStop( mTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	}
}

void StormwindSoldierAI::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	mLastWP = iWaypointId;
	switch ( iWaypointId )
	{
	case 9:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 10:{
		_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, 45 );
		WayPoint *pWaypoint = _unit->GetAIInterface()->getWayPoint( 10 );
		if ( pWaypoint != NULL )
		{
			_unit->SetSpawnPosition( pWaypoint->x, pWaypoint->y, pWaypoint->z, pWaypoint->o );
		}

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	}
}

void StormwindSoldierAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		for ( size_t i = 0; i < pAI->mAllianceMelee.size(); ++i )
		{
			if ( pAI->mAllianceMelee[i] == static_cast< MoonScriptCreatureAI* >( this ) )
			{
				pAI->mAllianceMelee[i] = NULL;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Ironforge Paladin AI

IronforgePaladinAI::IronforgePaladinAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell( IRONFORGE_PALADIN_SEAL_OF_SACRIFICE, Target_RandomFriendly, 8, 0, 10 );
	AddSpell( IRONFORGE_PALADIN_EXORCISM, Target_Current, 7, 0, 0 );
	AddSpell( IRONFORGE_PALADIN_HAMMER_OF_JUSTICE, Target_Current, 7, 0, 10, 0, 10 );
	SetWieldWeapon( true );

	mLastWP = 0;
}

void IronforgePaladinAI::OnCombatStop(Unit *mTarget)
{
	ParentClass::OnCombatStop( mTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	}
}

void IronforgePaladinAI::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	mLastWP = iWaypointId;
	switch ( iWaypointId )
	{
	case 9:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 10:{
		_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, 45 );
		WayPoint *pWaypoint = _unit->GetAIInterface()->getWayPoint( 10 );
		if ( pWaypoint != NULL )
		{
			_unit->SetSpawnPosition( pWaypoint->x, pWaypoint->y, pWaypoint->z, pWaypoint->o );
		}

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	}
}

void IronforgePaladinAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		for ( size_t i = 0; i < pAI->mAllianceMelee.size(); ++i )
		{
			if ( pAI->mAllianceMelee[i] == static_cast< MoonScriptCreatureAI* >( this ) )
			{
				pAI->mAllianceMelee[i] = NULL;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Darnassian Archer AI

DarnassianArcherAI::DarnassianArcherAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell( DARNASSIAN_ARCHER_SHOOT, Target_Current, 100, 0.5f, 0, 8, 35, true );
	SetWieldWeapon( true );

	mLastWP = 0;
}

void DarnassianArcherAI::OnCombatStart(Unit *mTarget)
{
	ParentClass::OnCombatStart( mTarget );
	if ( GetRangeToUnit( mTarget ) >= 8.0f && GetRangeToUnit( mTarget ) <= 35.0f )
	{
		SetBehavior( Behavior_Spell );
		SetCanMove( false );
	}
}

void DarnassianArcherAI::OnCombatStop(Unit *mTarget)
{
	ParentClass::OnCombatStop( mTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	}
}

void DarnassianArcherAI::AIUpdate()
{
	ParentClass::AIUpdate();
	Unit *pTarget = _unit->GetAIInterface()->GetNextTarget();
	if ( pTarget != NULL )
	{
		if ( GetRangeToUnit( pTarget ) >= 8.0f && GetRangeToUnit( pTarget ) <= 35.0f )
		{
			SetBehavior( Behavior_Spell );
			SetCanMove( false );
		}
	}
}

void DarnassianArcherAI::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	mLastWP = iWaypointId;
	switch ( iWaypointId )
	{
	case 9:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 10:{
		WayPoint *pWaypoint = _unit->GetAIInterface()->getWayPoint( 10 );
		if ( pWaypoint != NULL )
		{
			_unit->SetSpawnPosition( pWaypoint->x, pWaypoint->y, pWaypoint->z, pWaypoint->o );
		}

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	}
}

void DarnassianArcherAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		for ( size_t i = 0; i < pAI->mAllianceRanged.size(); ++i )
		{
			if ( pAI->mAllianceRanged[i] == static_cast< MoonScriptCreatureAI* >( this ) )
			{
				pAI->mAllianceRanged[i] = NULL;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Stormwind Mage AI

StormwindMageAI::StormwindMageAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell( STORMWIND_MAGE_FIREBALL, Target_Current, 70, 3, 0, 0, 100 );
	AddSpell( STORMWIND_MAGE_ARCANE_MISSILES, Target_Current, 15, 5, 10, 0, 100 );
	AddSpell( STORMWIND_MAGE_BLIZZARD, Target_RandomDestination, 15, 2, 10, 0, 100 );
	SetWieldWeapon( true );

	mLastWP = 0;
}

void StormwindMageAI::OnCombatStart(Unit *mTarget)
{
	ParentClass::OnCombatStart( mTarget );
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
}

void StormwindMageAI::OnCombatStop(Unit *mTarget)
{
	ParentClass::OnCombatStop( mTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	}
}

void StormwindMageAI::AIUpdate()
{
	ParentClass::AIUpdate();
	Unit *pTarget = _unit->GetAIInterface()->GetNextTarget();
	if ( pTarget != NULL )
	{
		if ( GetRangeToUnit( pTarget ) <= 100.0f )
		{
			SetBehavior( Behavior_Spell );
			SetCanMove( false );
		}
		else
		{
			_unit->GetAIInterface()->RemoveThreatByPtr( pTarget );
		}
	}
}

void StormwindMageAI::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	mLastWP = iWaypointId;
	switch ( iWaypointId )
	{
	case 9:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 10:{
		WayPoint *pWaypoint = _unit->GetAIInterface()->getWayPoint( 10 );
		if ( pWaypoint != NULL )
		{
			_unit->SetFacing( pWaypoint->o );
			_unit->SetSpawnPosition( pWaypoint->x, pWaypoint->y, pWaypoint->z, pWaypoint->o );
		}

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	}
}

void StormwindMageAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		for ( size_t i = 0; i < pAI->mAllianceCaster.size(); ++i )
		{
			if ( pAI->mAllianceCaster[i] == static_cast< MoonScriptCreatureAI* >( this ) )
			{
				pAI->mAllianceCaster[i] = NULL;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Pit Commander AI

PitCommanderAI::PitCommanderAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell( PIT_COMMANDER_RAIN_OF_FIRE, Target_RandomDestination, 7, 9, 15 );
	AddSpell( PIT_COMMANDER_CLEAVE, Target_Current, 9, 0, 15, 0, 10 );			// Added 10 coz of size of unit
	SetWieldWeapon( true );
}

void PitCommanderAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		if ( pAI->mPitCommander == static_cast< MoonScriptCreatureAI* >( this ) )
		{
			pAI->mPitCommander = NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Infernal Siegebreaker AI

InfernalSiegebreakerAI::InfernalSiegebreakerAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	mSuicide = AddSpell( INFERNAL_SIEGEBREAKER_SUICIDE, Target_Self, 5, 0, 0 );
}

void InfernalSiegebreakerAI::OnCombatStart(Unit *pTarget)
{
	ParentClass::OnCombatStart( pTarget );
	mSuicide->mEnabled = false;
}

void InfernalSiegebreakerAI::AIUpdate()
{
	if ( !mSuicide->mEnabled && GetHealthPercent() <= 5 )
	{
		mSuicide->mEnabled = true;
	}

	ParentClass::AIUpdate();
}

void InfernalSiegebreakerAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		for ( AISet::iterator itr = pAI->mInfernalTable.begin(); itr != pAI->mInfernalTable.end(); ++itr )
		{
			if ( (*itr) == static_cast< MoonScriptCreatureAI* >( this ) )
			{
				pAI->mInfernalTable.erase( itr );
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Fel Soldier AI

FelSoldierAI::FelSoldierAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddSpell( FEL_SOLDIER_CUTDOWN, Target_Current, 8, 0, 15, 0, 8 );
	AddSpell( FEL_SODLIER_CLEAVE, Target_Current, 8, 0, 20, 0, 8 );
	SetWieldWeapon( true );

	mLastWP = 0;
}

void FelSoldierAI::OnCombatStop(Unit *pTarget)
{
	ParentClass::OnCombatStop( pTarget );
	MoveTo( _unit->GetPositionX(), _unit->GetPositionY() + 3.0f, _unit->GetPositionZ() );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	}
}

void FelSoldierAI::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	mLastWP = iWaypointId;
	switch ( iWaypointId )
	{
	case 3:
		SetMoveType( Move_DontMoveWP );
		_unit->SetFacing( 4.708117f );
		break;
	case 8:
		Despawn();
		break;
	}
}

void FelSoldierAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		AISet::iterator itr = pAI->mQueuedDemonSet.find( static_cast< MoonScriptCreatureAI* >( this ) );
		if ( itr != pAI->mQueuedDemonSet.end() )
			pAI->mQueuedDemonSet.erase( itr );

		itr = pAI->mFightingDemonSet.find( static_cast< MoonScriptCreatureAI* >( this ) );
		if ( itr != pAI->mFightingDemonSet.end() )
			pAI->mFightingDemonSet.erase( itr );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Wrath Master AI

WrathMasterAI::WrathMasterAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	SetWieldWeapon( true );

	mLastWP = 0;
}

void WrathMasterAI::OnCombatStop(Unit *pTarget)
{
	ParentClass::OnCombatStop( pTarget );
	MoveTo( _unit->GetPositionX(), _unit->GetPositionY() + 3.0f, _unit->GetPositionZ() );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	}
}

void WrathMasterAI::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	mLastWP = iWaypointId;
	switch ( iWaypointId )
	{
	case 3:
		SetMoveType( Move_DontMoveWP );
		_unit->SetFacing( 4.708117f );
		break;
	case 8:
		Despawn();
		break;
	}
}

void WrathMasterAI::Destroy()
{
	Creature *pTrigger = static_cast< Creature* >( ForceCreatureFind( CN_DARK_PORTAL_EVENT_TRIGGER, -263.719f, 1101.18f, 73.7167f, false ) );
	if ( pTrigger != NULL && pTrigger->GetScript() != NULL )
	{
		DarkPortalEventTriggerAI *pAI = static_cast< DarkPortalEventTriggerAI* >( pTrigger->GetScript() );
		AISet::iterator itr = pAI->mQueuedDemonSet.find( static_cast< MoonScriptCreatureAI* >( this ) );
		if ( itr != pAI->mQueuedDemonSet.end() )
			pAI->mQueuedDemonSet.erase( itr );

		itr = pAI->mFightingDemonSet.find( static_cast< MoonScriptCreatureAI* >( this ) );
		if ( itr != pAI->mFightingDemonSet.end() )
			pAI->mFightingDemonSet.erase( itr );
	}
}
