#include "StdAfx.h"

#define STATUE_NPC_COPY_DATA_FROM_ID					143333
#define STATUE_NPC_ANTI_SPAM_INTERVAL					(30*1000)
//#define PVPSTATUENPC_EXTENSION_BEST_PLAYER				"PVPSBestPlayer"
//#define PVPSTATUENPC_EXTENSION_BEST_PLAYER_KILLS		"PVPSBestPlayerKillsNeedToBeAVeryLongString"
//#define PVPSTATUENPC_EXTENSION_NPCTYPE					"PVPSNPCTYPE"
//#define PVPSTATUENPC_EXTENSION_MARKRESPAWN				"PVPSNPCRespawn"
#define SEARCH_FOR_NPC_ID_GAP							100000
#define PVPSCORE_KILL_COOLDOWN_INTERVAL					(5*60*60)
#define NEW_PLAYER_CREATE_ABUSE_TIME_REQ				(60*60*24*3)

enum PVPStatueTypes
{
	PVPSTATUE_FOREVER_COUNTING			= 0,
	PVPSTATUE_MONTHLY_COUNTING			= 1,
	PVPSTATUE_WEEKLY_COUNTING			= 2,
	PVPSTATUE_FOREVER_WARRIOR_COUNTING	= 3,
	PVPSTATUE_FOREVER_PALADIN_COUNTING	= 4,
	PVPSTATUE_FOREVER_HUNTER_COUNTING	= 5,
	PVPSTATUE_FOREVER_ROGUE_COUNTING	= 6,
	PVPSTATUE_FOREVER_PRIEST_COUNTING	= 7,
	PVPSTATUE_FOREVER_DK_COUNTING		= 8,
	PVPSTATUE_FOREVER_SHAMAN_COUNTING	= 9,
	PVPSTATUE_FOREVER_MAGE_COUNTING		= 10,
	PVPSTATUE_FOREVER_WARLOCK_COUNTING	= 11,
	PVPSTATUE_FOREVER_DRUID_COUNTING	= 12,
	PVPSTATUE_FOREVER_DUEL_COUNTING	= 13,
	PVPSTATUE_TYPE_COUNT,
};

static std::set<Creature*> StatueNPCs;
static uint32 BiggestNPCID = 0;
static uint64 PVPStatueNPCBestPlayerGUID[PVPSTATUE_TYPE_COUNT];
static uint64 PVPStatueNPCBestPlayerKills[PVPSTATUE_TYPE_COUNT];
static char PVPStatuePlayerName[PVPSTATUE_TYPE_COUNT][20];
static uint32 PVPStatueNPCAntiUpdateSpam[PVPSTATUE_TYPE_COUNT];
static int64 PVPStatueNPCBestPlayerSince[PVPSTATUE_TYPE_COUNT];
static int64 PVPStatueNPCLastResetStamp[PVPSTATUE_TYPE_COUNT];
//static uint32 PVPStatueNPCAntiUpdateSpamPendingChange = 0;

void UpdateStatueNPC( Player *pPlayer, uint32 NPCType );

void InitStatueNPCOnce( uint32 NPCType )
{
	if( NPCType >= PVPSTATUE_TYPE_COUNT )
		return;
	strcpy( PVPStatuePlayerName[ NPCType ], "" );
	char FileName[50];
	sprintf( FileName, "./script_vars/PvPStatueNPCVars_%d.txt", NPCType );
	FILE *f=fopen( FileName, "rt" );
	if( f )
	{
		int t;
		fscanf(f,"%d\n",&t);
		BiggestNPCID = t;
		fscanf(f,"%d\n",&t);
		PVPStatueNPCBestPlayerGUID[ NPCType ] = t;
		fscanf(f,"%d\n",&t);
		PVPStatueNPCBestPlayerKills[ NPCType ] = t;
		fscanf(f,"%s\n",PVPStatuePlayerName[ NPCType ]);
		fscanf(f,"%I64u\n",&PVPStatueNPCBestPlayerSince[ NPCType ]);
		fscanf(f,"%I64u\n",&PVPStatueNPCLastResetStamp[ NPCType ]);
		fclose( f );
		//was added later, old file did not contain it 
		if( PVPStatueNPCBestPlayerSince[ NPCType ] == 0 )
			PVPStatueNPCBestPlayerSince[ NPCType ] = UNIXTIME;
		if( PVPStatueNPCLastResetStamp[ NPCType ] == 0 )
			PVPStatueNPCLastResetStamp[ NPCType ] = UNIXTIME;

		if( ( NPCType == PVPSTATUE_WEEKLY_COUNTING && PVPStatueNPCLastResetStamp[ NPCType ] + WEEKLY < UNIXTIME )
			|| ( NPCType == PVPSTATUE_MONTHLY_COUNTING && PVPStatueNPCLastResetStamp[ NPCType ] + MONTHLY < UNIXTIME ) )
		{
			PVPStatueNPCBestPlayerGUID[ NPCType ] = 0;
			PVPStatueNPCBestPlayerKills[ NPCType ] = 0;
			PVPStatuePlayerName[ NPCType ][0] = 0;
			PVPStatueNPCBestPlayerSince[ NPCType ] = 0;
			PVPStatueNPCLastResetStamp[ NPCType ] = UNIXTIME;
			QueryResult * result = CharacterDatabase.Query( "delete from PVPStatueStatus where NPCType=%d", NPCType ); //reset the status in DB also
			if( result )
				delete result;
			result = CharacterDatabase.Query( "delete from PVPStatueKills where NPCType=%d", NPCType ); //reset the status in DB also
			if( result )
				delete result;
		}
	}
	else
	{
		QueryResult * result = CharacterDatabase.Query( "Select player,points from PVPStatueStatus where NPCType = %d order by points desc limit 0,1", NPCType );
		if( result )
		{
			Field *fields = result->Fetch();
			PVPStatueNPCBestPlayerGUID[ NPCType ] = fields[0].GetInt32();
			PVPStatueNPCBestPlayerKills[ NPCType ] = fields[1].GetInt32();
			delete result;
			result = NULL;
		}
		PlayerInfo *pi = objmgr.GetPlayerInfo( PVPStatueNPCBestPlayerGUID[ NPCType ] );
		if( pi )
			strcpy( PVPStatuePlayerName[ NPCType ], pi->name );
	}
	//get the smallest non yet used creature_name ID
/*	QueryResult * result = CharacterDatabase.Query( "Select setting_value from server_settings where setting_id='PVPStatueNBiggestPCID'" );
	if( result )
	{
		Field *fields = result->Fetch();
		BiggestNPCID = fields[0].GetInt32();
		delete result;
		result = NULL;
	} */
	if( BiggestNPCID == 0 )
	{
		StorageContainerIterator<CreatureInfo> * itr = CreatureNameStorage.MakeIterator();
		int PrevUsedID = 0;
		int FirstGapAt = 0;
		int BiggestGapAt = 0;
		int BiggestGapAmt = 0;
		while(!itr->AtEnd())
		{
			CreatureInfo *it = itr->Get();
			int GapSize = it->Id - PrevUsedID;
			if( GapSize > BiggestGapAmt )
			{
				BiggestGapAt = PrevUsedID;
				BiggestGapAmt = GapSize;
			}
			if( FirstGapAt == 0 && GapSize >= SEARCH_FOR_NPC_ID_GAP )
				FirstGapAt = PrevUsedID;
			if( it->Id > BiggestNPCID )
				BiggestNPCID = it->Id;

			PrevUsedID = it->Id;
			if(!itr->Inc())
				break;
		}
		itr->Destruct();
		//try to avoid collisions between multiple realms
//		BiggestNPCID = BiggestGapAt + MIN( BiggestGapAmt, 1 + ( RandomUInt() % 100 ) * 10 + ( RandomUInt() % 100 ) * 100 + ( RandomUInt() % 100 ) * 1000 );
		BiggestNPCID = BiggestGapAt + MIN( BiggestGapAmt, 1 + ( RandomUInt() % 100 ) * 10 + ( RandomUInt() % 100 ) * 100 );
		//update DB so next time we do not need to repeat this step
//		result = CharacterDatabase.Query( "replace into server_settings (setting_id,setting_value) values ('PVPStatueNBiggestPCID',%d)", BiggestNPCID);	
	}
	else
	{
		BiggestNPCID++; //make sure we somehow do not inherit previous value
	}
}

void SaveStatus( uint32 NPCType )
{
	if( NPCType >= PVPSTATUE_TYPE_COUNT )
		return;

	char FileName[50];
	sprintf( FileName, "./script_vars/PvPStatueNPCVars_%d.txt", NPCType );

	FILE *f=fopen(FileName,"wt");
	if( f )
	{
		fprintf(f,"%d\n",BiggestNPCID);
		fprintf(f,"%d\n",PVPStatueNPCBestPlayerGUID[ NPCType ]);
		fprintf(f,"%d\n",PVPStatueNPCBestPlayerKills[ NPCType ]);
		fprintf(f,"%s\n",PVPStatuePlayerName[ NPCType ]);
		fprintf(f,"%I64u\n",PVPStatueNPCBestPlayerSince[ NPCType ]);
		fprintf(f,"%I64u\n",PVPStatueNPCLastResetStamp[ NPCType ]);
		fclose( f );
	}
}

int64 *GetPVPPoints( Player *pPlayer, uint32 NPCType )
{
	int64 *CurPlayerKills = pPlayer->GetCreateIn64Extension( EXTENSION_ID_PVPSTATUE_BEST_PLAYER_KILLS_START + NPCType );
	if( *CurPlayerKills == 0 )
	{
		//get the score for the new killer
		QueryResult * result = CharacterDatabase.Query( "Select points from PVPStatueStatus where player=%d and NPCType=%d", pPlayer->GetLowGUID(), NPCType );
		if( result )
		{
			Field *fields = result->Fetch();
			*CurPlayerKills = fields[0].GetInt32();
			delete result;
			result = NULL;
		}
	}
	return CurPlayerKills;
}

uint32 GetNPCTypeFromCreature( Object *pCreature )
{
	if( pCreature == NULL )
		return PVPSTATUE_FOREVER_COUNTING;
	//is it base spawn ?
	if( pCreature->GetEntry() >= STATUE_NPC_COPY_DATA_FROM_ID && pCreature->GetEntry() < STATUE_NPC_COPY_DATA_FROM_ID + PVPSTATUE_TYPE_COUNT )
		return ( pCreature->GetEntry() - STATUE_NPC_COPY_DATA_FROM_ID );
	//maybe we registered type when we created it ?
	uint32 MaybeType = *( pCreature->GetCreateIn64Extension( EXTENSION_ID_PVPSTATUE_NPC_TYPE ) );
	if( MaybeType >= PVPSTATUE_TYPE_COUNT )
		return PVPSTATUE_FOREVER_COUNTING;
	return MaybeType;
}

class PVPStatueNPCScript : public CreatureAIScript
{
public:
	PVPStatueNPCScript(Creature* pCreature) : CreatureAIScript(pCreature) 
	{
	}
	void OnLoad()
	{
		//register this NPC as one of the guys who need to represent the hero of the day
//		if( _unit->GetMapMgr()->GetMapInfo()->type_flags == INSTANCE_FLAG_STATIC_MAP )
		{
//			StatueNPCs.push_front( _unit );
			StatueNPCs.insert( _unit );
		}
		//we are always friendly
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
		}
		_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, EMOTE_STATE_DANCE );
		//in case we are not yet initialized, we should later on despawn ourself and get an updated version
		RegisterAIUpdateEvent( 10000 );
		//create a new NPC type that we need to spawn on next update
		if( _unit->GetEntry() >= STATUE_NPC_COPY_DATA_FROM_ID && _unit->GetEntry() < STATUE_NPC_COPY_DATA_FROM_ID + PVPSTATUE_TYPE_COUNT )
		{
			uint32 NPCType = GetNPCTypeFromCreature( _unit );
			UpdateStatueNPC( NULL, NPCType );
		}
	}
	void AIUpdate() 
	{
		//works only for base spawns
		uint32 NPCType = GetNPCTypeFromCreature( _unit );
		if( NPCType >= PVPSTATUE_TYPE_COUNT )
			return;
		uint32 NewEntry = *(_unit->GetCreateIn64Extension( EXTENSION_ID_PVPSTATUE_MARK_RESPAWN ) );
		if( _unit->IsInWorld() && NewEntry != 0 )
		{
			//already in the process of despawning ? Do nothing
			if( sEventMgr.HasEvent( _unit, EVENT_CREATURE_RESPAWN ) == true )
				return;
			if( CreatureNameStorage.LookupEntry( NewEntry ) != NULL )
			{
				Creature *p = _unit->GetMapMgr()->CreateCreature( NewEntry, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
				*(p->GetCreateIn64Extension( EXTENSION_ID_PVPSTATUE_NPC_TYPE )) = NPCType;
				//self despawn
				_unit->Despawn( 1000, 0 );
			}
			//no need to run this function again
			RemoveAIUpdateEvent();
		}
	}
	void OnRemoveFromWorld() 
	{
		//try to avoid executing Update()
		RemoveAIUpdateEvent();
		//should always get executed
		std::set<Creature*>::iterator itr;
		for( itr = StatueNPCs.begin(); itr != StatueNPCs.end(); itr++ )
			if( *itr == _unit )
			{
				StatueNPCs.erase( itr );
				break;
			}
	}
	//default
	static CreatureAIScript *Create(Creature * c) { return new PVPStatueNPCScript(c); }
	void Destroy() { delete this; }
};

void UpdateStatueNPC( Player *pPlayer, uint32 NPCType )
{
	if( NPCType >= PVPSTATUE_TYPE_COUNT )
		return;
	if( pPlayer == NULL )
		goto DirectLoad;
#ifndef _DEBUG
	if( PVPStatueNPCAntiUpdateSpam[ NPCType ] > GetTickCount() )
	{
		int64 *CurPlayerKills = GetPVPPoints( pPlayer, NPCType );
		if( PVPStatueNPCBestPlayerGUID[ NPCType ] != pPlayer->GetGUID() )
		{
			if( *CurPlayerKills > PVPStatueNPCBestPlayerKills[ NPCType ] )
				pPlayer->BroadcastMessage("PVPStatue : Anti kill update spam is delaying PVP update");
		}
		return;
	}
	PVPStatueNPCAntiUpdateSpam[ NPCType ] = GetTickCount() + STATUE_NPC_ANTI_SPAM_INTERVAL;
#endif
	int64 *CurPlayerKills = GetPVPPoints( pPlayer, NPCType );
	if( PVPStatueNPCBestPlayerGUID[ NPCType ] != pPlayer->GetGUID() )
	{
		//is this a better killer ?
		if( *CurPlayerKills > PVPStatueNPCBestPlayerKills[ NPCType ] )
		{
			//update the status of the best killer
			PVPStatueNPCBestPlayerSince[ NPCType ] = UNIXTIME;
			PVPStatueNPCBestPlayerKills[ NPCType ] = *CurPlayerKills;
			PVPStatueNPCBestPlayerGUID[ NPCType ] = pPlayer->GetGUID();

			pPlayer->BroadcastMessage("Congratulations. You are realm best pvp killer. For now");
DirectLoad:
			//update DB
			while( CreatureNameStorage.LookupEntry( BiggestNPCID ) != NULL )
				BiggestNPCID++;
//			CharacterDatabase.Query( "replace into server_settings (setting_id,setting_value) values ('PVPStatueNBiggestPCID',%d)", BiggestNPCID);	

			//create a new temp : CreatureName + CreatureProto
			CreatureProto *protoOld = CreatureProtoStorage.LookupEntry( STATUE_NPC_COPY_DATA_FROM_ID + NPCType );
			CreatureInfo *infoOld = CreatureNameStorage.LookupEntry( STATUE_NPC_COPY_DATA_FROM_ID + NPCType );
			if( protoOld == NULL || infoOld == NULL )
			{
				sLog.outDetail("Warning : Missing summon creature template %u for PVP statue NPC. Exiting!", STATUE_NPC_COPY_DATA_FROM_ID + NPCType );
				return;
			}

			sScriptMgr.CreateEmtryCreatureNameAndProto( BiggestNPCID );

			CreatureProto *protoNew = CreatureProtoStorage.LookupEntry( BiggestNPCID );
			CreatureInfo *infoNew = CreatureNameStorage.LookupEntry( BiggestNPCID );
			if( infoNew == NULL || protoNew == NULL )
			{
				sLog.outDetail("Warning : Could not create new summon creature template %u for PVP statue NPC. Exiting!",BiggestNPCID);
				return;
			}
			sScriptMgr.register_creature_script( BiggestNPCID, &PVPStatueNPCScript::Create );
			memcpy( protoNew, protoOld, sizeof( CreatureProto ) );	//this will copy gossip script also. Clicking on the new creature will have same gossip
			memcpy( infoNew, infoOld, sizeof( CreatureInfo ) ); //copy gossip also
			protoNew->Id = BiggestNPCID;
			infoNew->Id = BiggestNPCID;
			char NewCreatureStrings[5000];
			if( pPlayer )
				strcpy( PVPStatuePlayerName[ NPCType ], pPlayer->GetName() );
//			sprintf( NewCreatureStrings, "Almighty PlayerSlayer |cffff0000%s|r", PVPStatuePlayerName );
			sprintf( NewCreatureStrings, "Almighty PlayerSlayer %s", PVPStatuePlayerName[ NPCType ] );
			infoNew->Name = strdup( NewCreatureStrings );
			if( NPCType == PVPSTATUE_MONTHLY_COUNTING )
				infoNew->SubName = strdup( "This month PVP champion of the realm" );
			else if( NPCType == PVPSTATUE_WEEKLY_COUNTING )
				infoNew->SubName = strdup( "This week PVP champion of the realm" );
			else if( NPCType == PVPSTATUE_FOREVER_WARRIOR_COUNTING )
				infoNew->SubName = strdup( "Best Warrior of the realm" );
			else if( NPCType == PVPSTATUE_FOREVER_PALADIN_COUNTING )
				infoNew->SubName = strdup( "Best Paladin of the realm" );
			else if( NPCType == PVPSTATUE_FOREVER_HUNTER_COUNTING )
				infoNew->SubName = strdup( "Best Hunter of the realm" );
			else if( NPCType == PVPSTATUE_FOREVER_ROGUE_COUNTING )
				infoNew->SubName = strdup( "Best Rogue of the realm" );
			else if( NPCType == PVPSTATUE_FOREVER_PRIEST_COUNTING )
				infoNew->SubName = strdup( "Best Priest of the realm" );
			else if( NPCType == PVPSTATUE_FOREVER_DK_COUNTING )
				infoNew->SubName = strdup( "Best DK of the realm" );
			else if( NPCType == PVPSTATUE_FOREVER_SHAMAN_COUNTING )
				infoNew->SubName = strdup( "Best Shaman of the realm" );
			else if( NPCType == PVPSTATUE_FOREVER_MAGE_COUNTING )
				infoNew->SubName = strdup( "Best Mage of the realm" );
			else if( NPCType == PVPSTATUE_FOREVER_WARLOCK_COUNTING )
				infoNew->SubName = strdup( "Best Warlock of the realm" );
			else if( NPCType == PVPSTATUE_FOREVER_DRUID_COUNTING )
				infoNew->SubName = strdup( "Best Druid of the realm" );
			else if( NPCType == PVPSTATUE_FOREVER_DUEL_COUNTING )
				infoNew->SubName = strdup( "Best Dueler of the realm" );
			else
				infoNew->SubName = strdup( "Glorious PVP champion of the realm" );
			infoNew->info_str = strdup( infoNew->info_str );
			protoNew->spells.clear();

			//duplicate the Statue list
			std::set<Creature*> tlist = StatueNPCs;
			StatueNPCs.clear();
			//now spawn a new Statue Everywhere
			std::set<Creature*>::iterator itr,itr2;
			for( itr2 = tlist.begin(); itr2 != tlist.end(); )
			{
				itr = itr2;
				itr2++;
				Creature *c = *itr;
				if( c->deleted != 0 || c->IsInWorld() != true )
				{
					continue;
				}
				if( GetNPCTypeFromCreature( c ) != NPCType )
				{
					StatueNPCs.insert( c );
					continue;
				}
/*				Creature *p = c->GetMapMgr()->CreateCreature( BiggestNPCID, true, c->GetPositionX(), c->GetPositionY(), c->GetPositionZ() );
				if( p )
				{
					*(p->GetCreateIn64Extension( EXTENSION_ID_PVPSTATUE_NPC_TYPE )) = NPCType;
				}
				//now despawn a old Statues
				c->Despawn( 1000, 0 );*/
				int64 *SwitchToEntry = c->GetCreateIn64Extension( EXTENSION_ID_PVPSTATUE_MARK_RESPAWN );
				*SwitchToEntry = BiggestNPCID;
			}
			//a new hero
			SaveStatus( NPCType );
		}
	}
	else
	{
		//our hero raised score
		PVPStatueNPCBestPlayerKills[ NPCType ] = *CurPlayerKills;
		SaveStatus( NPCType );
	}
}

void OnPlayerKillPVPStatusUpdate(Player *pPlayer, Player *pVictim)
{
//	if( pPlayer->m_bg != NULL || pVictim->m_bg != NULL )
//		return;
#ifndef _DEBUG
	if( pPlayer->getLevel() < PLAYER_LEVEL_CAP_BLIZZLIKE || *(pPlayer->GetPlayedtime()) < NEW_PLAYER_CREATE_ABUSE_TIME_REQ )
		return;
#endif
	//do we have this player killed yet ?
	uint32 AlreadyKilled = 0;
	QueryResult * result;
	result = CharacterDatabase.Query( "Select count(*) from PVPStatueKills where killer=%d and victim=%d and lastkill>%d", pPlayer->GetLowGUID(), pVictim->GetLowGUID(), UNIXTIME - PVPSCORE_KILL_COOLDOWN_INTERVAL );
	if( result )
	{
		Field *fields = result->Fetch();
		AlreadyKilled = fields[0].GetInt32();
		delete result;
		result = NULL;
	}
	//already killed this player today
#ifndef _DEBUG
	if( AlreadyKilled == 1 )
	{
		return;
	}
#endif
	//mark him killed
	result = CharacterDatabase.Query( "replace into PVPStatueKills (killer,victim,lastkill) values (%d,%d,%d)", pPlayer->GetLowGUID(), pVictim->GetLowGUID(), UNIXTIME );
	if( result )
		delete result;
	for( uint32 i=0;i<PVPSTATUE_TYPE_COUNT;i++)
	{
		if( i == PVPSTATUE_FOREVER_WARRIOR_COUNTING && ( pVictim->getClass() != WARRIOR || pPlayer->getClass() != WARRIOR ) )
			continue;
		if( i == PVPSTATUE_FOREVER_PALADIN_COUNTING && ( pVictim->getClass() != PALADIN || pPlayer->getClass() != PALADIN ) )
			continue;
		if( i == PVPSTATUE_FOREVER_HUNTER_COUNTING && ( pVictim->getClass() != HUNTER || pPlayer->getClass() != HUNTER ) )
			continue;
		if( i == PVPSTATUE_FOREVER_ROGUE_COUNTING && ( pVictim->getClass() != ROGUE || pPlayer->getClass() != ROGUE ) )
			continue;
		if( i == PVPSTATUE_FOREVER_PRIEST_COUNTING && ( pVictim->getClass() != PRIEST || pPlayer->getClass() != PRIEST ) )
			continue;
		if( i == PVPSTATUE_FOREVER_DK_COUNTING && ( pVictim->getClass() != DEATHKNIGHT || pPlayer->getClass() != DEATHKNIGHT ) )
			continue;
		if( i == PVPSTATUE_FOREVER_SHAMAN_COUNTING && ( pVictim->getClass() != SHAMAN || pPlayer->getClass() != SHAMAN ) )
			continue;
		if( i == PVPSTATUE_FOREVER_MAGE_COUNTING && ( pVictim->getClass() != MAGE || pPlayer->getClass() != MAGE ) )
			continue;
		if( i == PVPSTATUE_FOREVER_WARLOCK_COUNTING && ( pVictim->getClass() != WARLOCK || pPlayer->getClass() != WARLOCK ) )
			continue;
		if( i == PVPSTATUE_FOREVER_DRUID_COUNTING && ( pVictim->getClass() != DRUID || pPlayer->getClass() != DRUID ) )
			continue;
		if( i == PVPSTATUE_FOREVER_DUEL_COUNTING )
			continue;
		//get player current score
		int64 *CurPlayerKills = GetPVPPoints( pPlayer, i );
		//update player kill counter
		*CurPlayerKills = *CurPlayerKills + 1;
		//check if NPC needs to get swapped out
		UpdateStatueNPC( pPlayer, i );
	}
}

void OnPlayerDuelWinPVPStatusUpdate(Player *pPlayer, Player *pVictim)
{
//	if( pPlayer->m_bg != NULL || pVictim->m_bg != NULL )
//		return;
#ifndef _DEBUG
	if( pPlayer->getLevel() < PLAYER_LEVEL_CAP_BLIZZLIKE || *(pPlayer->GetPlayedtime()) < NEW_PLAYER_CREATE_ABUSE_TIME_REQ )
		return;
#endif
	//do we have this player killed yet ?
	uint32 AlreadyKilled = 0;
	QueryResult * result;
	result = CharacterDatabase.Query( "Select count(*) from PVPStatueKills where killer=%d and victim=%d and lastkill>%d", pPlayer->GetLowGUID(), pVictim->GetLowGUID(), UNIXTIME - PVPSCORE_KILL_COOLDOWN_INTERVAL );
	if( result )
	{
		Field *fields = result->Fetch();
		AlreadyKilled = fields[0].GetInt32();
		delete result;
		result = NULL;
	}
	//already killed this player today
#ifndef _DEBUG
	if( AlreadyKilled == 1 )
	{
		return;
	}
#endif
	//mark him killed
	result = CharacterDatabase.Query( "replace into PVPStatueKills (killer,victim,lastkill) values (%d,%d,%d)", pPlayer->GetLowGUID(), pVictim->GetLowGUID(), UNIXTIME );
	if( result )
		delete result;
	//get player current score
	int64 *CurPlayerKills = GetPVPPoints( pPlayer, PVPSTATUE_FOREVER_DUEL_COUNTING );
	//update player kill counter
	*CurPlayerKills = *CurPlayerKills + 1;
	//check if NPC needs to get swapped out
	UpdateStatueNPC( pPlayer, PVPSTATUE_FOREVER_DUEL_COUNTING );
}

void OnPlayerLogoutPVPStatusSave(Player *pPlayer)
{
	for( uint32 i=0;i<PVPSTATUE_TYPE_COUNT;i++)
	{
		//get player current score
		int64 *CurPlayerKills = pPlayer->GetCreateIn64Extension( EXTENSION_ID_PVPSTATUE_BEST_PLAYER_KILLS_START + i );
		if( *CurPlayerKills <= 0 )
			continue;
		if( i == PVPSTATUE_FOREVER_WARRIOR_COUNTING && pPlayer->getClass() != WARRIOR )
			continue;
		if( i == PVPSTATUE_FOREVER_PALADIN_COUNTING && pPlayer->getClass() != PALADIN )
			continue;
		if( i == PVPSTATUE_FOREVER_HUNTER_COUNTING && pPlayer->getClass() != HUNTER )
			continue;
		if( i == PVPSTATUE_FOREVER_ROGUE_COUNTING && pPlayer->getClass() != ROGUE )
			continue;
		if( i == PVPSTATUE_FOREVER_PRIEST_COUNTING && pPlayer->getClass() != PRIEST )
			continue;
		if( i == PVPSTATUE_FOREVER_DK_COUNTING && pPlayer->getClass() != DEATHKNIGHT )
			continue;
		if( i == PVPSTATUE_FOREVER_SHAMAN_COUNTING && pPlayer->getClass() != SHAMAN )
			continue;
		if( i == PVPSTATUE_FOREVER_MAGE_COUNTING && pPlayer->getClass() != MAGE )
			continue;
		if( i == PVPSTATUE_FOREVER_WARLOCK_COUNTING && pPlayer->getClass() != WARLOCK )
			continue;
		if( i == PVPSTATUE_FOREVER_DRUID_COUNTING && pPlayer->getClass() != DRUID )
			continue;
		QueryResult * result = CharacterDatabase.Query( "Replace into PVPStatueStatus (player,points,NPCType) values (%d,%d,%d)", pPlayer->GetLowGUID(),(uint32)( *CurPlayerKills ), i );
		if( result )
		{
			delete result;
			result = NULL;
		}
	}
}

/*
create table `PVPStatueStatus`( `player` int(10) UNSIGNED NOT NULL DEFAULT '0' , `points` int(10) UNSIGNED NOT NULL DEFAULT '0', `NPCType` int(10) UNSIGNED NOT NULL DEFAULT '0' , PRIMARY KEY (`player`)) ;
create table `PVPStatueKills`( `killer` int(10) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' , `victim` int(10) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' , `lastkill` int(10) UNSIGNED ZEROFILL NOT NULL DEFAULT '0', `NPCType` int(10) UNSIGNED NOT NULL DEFAULT '0' , PRIMARY KEY (`killer`, `victim`));
ALTER TABLE `pvpstatuestatus` ADD COLUMN `NPCType` INT(10) UNSIGNED DEFAULT '0' NOT NULL AFTER `points`;
alter table `pvpstatuestatus` change `NPCType` `NPCType` int(10) UNSIGNED default '0' NOT NULL, drop primary key,  add primary key(`player`, `NPCType`);

INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143333, 'PVP statue holder', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143333, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143334, 'PVP statue holder(monthly)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143334, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143335, 'PVP statue holder(weekly)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143335, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143336, 'PVP statue holder(wari)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143336, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143337, 'PVP statue holder(pal)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143337, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143338, 'PVP statue holder(hun)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143338, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143339, 'PVP statue holder(rog)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143339, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143340, 'PVP statue holder(priest)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143340, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143341, 'PVP statue holder(DK)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143341, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143342, 'PVP statue holder(sham)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143342, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143343, 'PVP statue holder(mag)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143343, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143344, 'PVP statue holder(warl)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143344, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143345, 'PVP statue holder(druid)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143345, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(143346, 'PVP statue holder(DUEL)', 'Will get updated with realm best killer(soon)', '', 0, 0, 7, 0, 4, 0, 0, 23523, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(143346, 86, 86, 35, 350000, 350000, 1000000, 6, 1, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
*/

const char * szDayNames[7] = {
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

const char * szMonthNames[12] = {
	"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

void MakeIntString(char * buf, int num)
{
	if(num<10)
	{
		buf[0] = '0';
		//itoa(num, &buf[1], 10);
		sprintf(&buf[1], "%u", num);
	}
	else
	{
		//itoa(num,buf,10);
		sprintf(buf,"%u",num);
	}
}

string ConvertTimeStampToDataTime(uint32 timestamp)
{
	char szTempBuf[100];
	time_t t = (time_t)timestamp;
	tm * pTM = localtime(&t);

	string szResult;
//	szResult += szDayNames[pTM->tm_wday];
//	szResult += ", ";

	MakeIntString(szTempBuf, pTM->tm_mday);
	szResult += szTempBuf;
	szResult += " ";

	szResult += szMonthNames[pTM->tm_mon];
	szResult += " ";

	MakeIntString(szTempBuf, pTM->tm_year+1900);
	szResult += szTempBuf;
	szResult += " ";
//	MakeIntString(szTempBuf, pTM->tm_hour);
//	szResult += szTempBuf;
//	szResult += ":";
//	MakeIntString(szTempBuf, pTM->tm_min);
//	szResult += szTempBuf;
//	szResult += ":";
//	MakeIntString(szTempBuf, pTM->tm_sec);
//	szResult += szTempBuf;

	return szResult;
}

class SCRIPT_DECL ChangeLooksStatue : public GossipScript
{
public:
	ChangeLooksStatue() 
	{ 
	};
	void Destroy()	{		delete this;	}
	void send_MainPage(Object * pObject, Player* Plr)
	{
		if( Plr == NULL )
			return; 

		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		char MyPointCount[ 500 ];

		uint32 NPCType = GetNPCTypeFromCreature( pObject );
//		time_t epch = PVPStatueNPCBestPlayerSince[ NPCType ];
//		const tm *mytm = gmtime(&epch);

		if( ( NPCType == PVPSTATUE_FOREVER_WARRIOR_COUNTING && Plr->getClass() != WARRIOR )
			||( NPCType == PVPSTATUE_FOREVER_PALADIN_COUNTING && Plr->getClass() != PALADIN )
			||( NPCType == PVPSTATUE_FOREVER_HUNTER_COUNTING && Plr->getClass() != HUNTER )
			||( NPCType == PVPSTATUE_FOREVER_ROGUE_COUNTING && Plr->getClass() != ROGUE )
			||( NPCType == PVPSTATUE_FOREVER_PRIEST_COUNTING && Plr->getClass() != PRIEST )
			||( NPCType == PVPSTATUE_FOREVER_DK_COUNTING && Plr->getClass() != DEATHKNIGHT )
			||( NPCType == PVPSTATUE_FOREVER_SHAMAN_COUNTING && Plr->getClass() != SHAMAN )
			||( NPCType == PVPSTATUE_FOREVER_MAGE_COUNTING && Plr->getClass() != MAGE )
			||( NPCType == PVPSTATUE_FOREVER_WARLOCK_COUNTING && Plr->getClass() != WARLOCK )
			||( NPCType == PVPSTATUE_FOREVER_DRUID_COUNTING && Plr->getClass() != DRUID ) )
		{
			Menu->AddItem( 0, "You can't participate in this scoreboard", 2);
			string tsstr = ConvertTimeStampToDataTime( PVPStatueNPCBestPlayerSince[ NPCType ] );
			sprintf( MyPointCount, "Leader has %d. Unbeatable score since %s",(uint32)PVPStatueNPCBestPlayerKills[ NPCType ], tsstr.c_str() );
			Menu->AddItem( 0, MyPointCount, 3);
		}
		else
		{
//			sprintf( MyPointCount, "You have %d PVP points. Leader has %d. Unbeatable score since %d-%d-%d",(uint32)(*GetPVPPoints( Plr, NPCType )),(uint32)PVPStatueNPCBestPlayerKills[ NPCType ], 1900+mytm->tm_year, mytm->tm_mon, mytm->tm_mday );
			string tsstr = ConvertTimeStampToDataTime( PVPStatueNPCBestPlayerSince[ NPCType ] );
			sprintf( MyPointCount, "You have %d PVP points. Leader has %d. Unbeatable score since %s",(uint32)(*GetPVPPoints( Plr, NPCType )),(uint32)PVPStatueNPCBestPlayerKills[ NPCType ], tsstr.c_str() );
			Menu->AddItem( 0, MyPointCount, 2);
			Menu->AddItem( 0, "Earn points by killing unique high level players daily", 3);
			if( NPCType == PVPSTATUE_WEEKLY_COUNTING  )
			{
				string tsstr = ConvertTimeStampToDataTime( PVPStatueNPCLastResetStamp[ NPCType ] + WEEKLY );
				sprintf( MyPointCount, "Ladder resets on %s", tsstr.c_str() );
				Menu->AddItem( 0, MyPointCount, 3 );
			}
			else if( NPCType == PVPSTATUE_MONTHLY_COUNTING  )
			{
				string tsstr = ConvertTimeStampToDataTime( PVPStatueNPCLastResetStamp[ NPCType ] + MONTHLY );
				sprintf( MyPointCount, "Ladder resets on %s", tsstr.c_str() );
				Menu->AddItem( 0, MyPointCount, 3 );
			}
			if( Plr->GetGUID() == PVPStatueNPCBestPlayerGUID[ NPCType ] )
			{
				Menu->AddItem( 0, "Orc Guard", 4);
				Menu->AddItem( 0, "Human Guard", 5);
				Menu->AddItem( 0, "Flame On", 6);
				Menu->AddItem( 0, "Sylvanas", 7);
				Menu->AddItem( 0, "Illidan 1", 8);
				Menu->AddItem( 0, "Illidan 2", 9);
				Menu->AddItem( 0, "Ragnaros", 10);
				Menu->AddItem( 0, "Santa", 11);
			}
		}
		Menu->SendTo(Plr);
	}
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend)
	{
		if( AutoSend )
			send_MainPage(pObject,Plr);
	}
	void GossipEnd(Object * pObject, Player* Plr)	{		GossipScript::GossipEnd(pObject, Plr);	}
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature * pCreature = (pObject->IsCreature())?((Creature*)pObject):NULL;
		if(pCreature==NULL)
			return;
		uint32 SetNewDisplay = 0;
		uint32 SetNewEmote = 0;
		float SetNewSize = 1.0f;
		switch(IntId)
		{
			// info menu
			case 1:		send_MainPage(pObject,Plr);													break;
			// if he is redirected here then menu should be closed
			case 2:		Plr->CloseGossip();															break;
			case 3:		Plr->CloseGossip();															break;
			case 4:	//orc
				{
					SetNewDisplay = 8001;
					SetNewSize = 2.0f;
					SetNewEmote = EMOTE_ONESHOT_FLEX;
				}break;
			case 5:	//H guard
				{
					SetNewDisplay = 3167;
					SetNewSize = 2.0f;
					SetNewEmote = EMOTE_ONESHOT_FLEX;
				}break;
			case 6:	//flame
				{
					SetNewDisplay = 23523;
					SetNewEmote = EMOTE_STATE_DANCE;
					SetNewSize = 5.0f;
				}break;
			case 7:	//sylvanas
				{
					SetNewDisplay = 28213;
					SetNewEmote = EMOTE_STATE_DANCE;
					SetNewSize = 2.0f;
				}break;
			case 8:	//Illidan Shadow
				{
					SetNewDisplay = 35909;
					SetNewEmote = 0;
					SetNewSize = 0.8f;
				}break;
			case 9:	//Illidan
				{
					SetNewDisplay = 27571;
					SetNewEmote = 0;
					SetNewSize = 0.8f;
				}break;
			case 10:	//Ragnaros
				{
					SetNewDisplay = 37875;
					SetNewEmote = 0;
					SetNewSize = 0.07f;
				}break;
			case 11:	//Female Santa
				{
					SetNewDisplay = 18785;
					SetNewEmote = EMOTE_STATE_DANCE;
					SetNewSize = 2.0f;
				}break;
			default:	send_MainPage(pObject,Plr);													break;
		}
		uint32 NPCType = GetNPCTypeFromCreature( pObject );
		if( SetNewDisplay != 0 && Plr->GetGUID() == PVPStatueNPCBestPlayerGUID[ NPCType ] )
		{
			Plr->CloseGossip();
			std::set<Creature*>::iterator itr;
			for( itr = StatueNPCs.begin(); itr != StatueNPCs.end(); itr++ )
			{
				Creature *c = *itr;
				if( c->deleted != 0 || c->IsInWorld() != true )
				{
					continue;
				}
				if( GetNPCTypeFromCreature( c ) != NPCType )
					continue;
				c->SetUInt32Value( UNIT_FIELD_DISPLAYID, SetNewDisplay );
				c->SetUInt32Value( UNIT_NPC_EMOTESTATE, SetNewEmote );
				if( SetNewSize != 1.0f )
					c->SetScale( SetNewSize );
			}
		}
	}
};

void RegisterPVPStatue( ScriptMgr* mgr )
{
	QueryResult *result = CharacterDatabase.Query( "delete from PVPStatueKills where lastkill<%d", UNIXTIME - PVPSCORE_KILL_COOLDOWN_INTERVAL );
	if( result )
		delete result;
	result = CharacterDatabase.Query( "delete from pvpstatuestatus where player not in ( select guid from characters )" );
	if( result )
		delete result;
	//	CharacterDatabase.Query( "delete from PVPStatueStatus where player not in ( select guid from characters)" );
	//load next usable real specific NPC ID...
	for( uint32 i=0;i<PVPSTATUE_TYPE_COUNT;i++)
	{
		PVPStatueNPCBestPlayerGUID[i] = 0;
		PVPStatueNPCBestPlayerKills[i] = 0;
		PVPStatuePlayerName[i][0] = 0;
		PVPStatueNPCBestPlayerSince[i] = 0;
		PVPStatueNPCAntiUpdateSpam[i] = 0;
		PVPStatueNPCLastResetStamp[0] = 0;
		InitStatueNPCOnce( i );
		//in case we spawn a statue. He should start functioning
		mgr->register_creature_script( STATUE_NPC_COPY_DATA_FROM_ID+i, &PVPStatueNPCScript::Create );
	}
	//when player kills, he's score should increase
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_KILL_PLAYER, &OnPlayerKillPVPStatusUpdate );
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_DUEL_ENDED, &OnPlayerDuelWinPVPStatusUpdate );
	//save to DB our status
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_LOGOUT, &OnPlayerLogoutPVPStatusSave );
	//make him have a gossip to change looks
	for( uint32 i=0;i<PVPSTATUE_TYPE_COUNT;i++)
	{
		GossipScript * gs = (GossipScript*) new ChangeLooksStatue();
		mgr->register_gossip_script( STATUE_NPC_COPY_DATA_FROM_ID+i, gs );
	}
}
