#include "StdAfx.h"
#include "Setup.h"


ARCEMU_INLINE uint32 GetSpellForLanguage(uint32 SkillID)
{
	switch(SkillID)
	{
	case SKILL_LANG_COMMON:
		return 668;
		break;

	case SKILL_LANG_ORCISH:
		return 669;
		break;

	case SKILL_LANG_TAURAHE:
		return 670;
		break;

	case SKILL_LANG_DARNASSIAN:
		return 671;
		break;

	case SKILL_LANG_DWARVEN:
		return 672;
		break;

	case SKILL_LANG_THALASSIAN:
		return 813;
		break;

	case SKILL_LANG_DRACONIC:
		return 814;
		break;

	case SKILL_LANG_DEMON_TONGUE:
		return 815;
		break;

	case SKILL_LANG_TITAN:
		return 816;
		break;

	case SKILL_LANG_OLD_TONGUE:
		return 817;
		break;

	case SKILL_LANG_GNOMISH:
		return 7430;
		break;

	case SKILL_LANG_TROLL:
		return 7341;
		break;

	case SKILL_LANG_GUTTERSPEAK:
		return 17737;
		break;

	case SKILL_LANG_WORGEN:
		return 69270;
		break;

	case SKILL_LANG_GOBLIN:
		return 69269;
		break;
	}

	return 0;
}

void ResetPlayerForTeam( Player * pPlayer, uint8 Team )
{
	//no more health regen unless we say so
	pPlayer->PctRegenModifier = 0;

	//remove all auras. Even talents
	pPlayer->RemoveAllAuras();

	//make player lvl 1 to disable BG and talents
	pPlayer->setLevel( 1 );

	//clear all quest log
	for( uint32 QuestSlot=0;QuestSlot<MAX_QUEST_LOG_SIZE;QuestSlot++)
	{
		QuestLogEntry *qEntry = pPlayer->GetQuestLogInSlot( QuestSlot );
		if( !qEntry )
			continue;
		Quest *qPtr = qEntry->GetQuest();
		qEntry->Finish();
		qEntry = NULL;
	}

	//disable XP gain
	pPlayer->SetFlag( PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN );

	//remove all spells except playercreate
	pPlayer->mSpells.clear();
//	PlayerCreateInfo *info = objmgr.GetPlayerCreateInfo( pPlayer->getRace(), pPlayer->getClass() );
//	for(std::set<uint32>::iterator sp = info->spell_list.begin();sp!=info->spell_list.end();sp++)
//		if(*sp)
//			pPlayer->mSpells.insert(*sp);
	pPlayer->addSpell( 6603, true, false );
	pPlayer->addSpell( 3018, true, false );

	pPlayer->setAction( 0, 6603, ACTION_BUTTON_SPELL );	//autoattack
	pPlayer->setAction( 1, 3018, ACTION_BUTTON_SPELL );	//shoot

	//make sure talents are not selected
	pPlayer->m_specs[0].talents.clear();
	pPlayer->m_specs[1].talents.clear();

	//make sure we remove all items that are charms
	std::list< uint32 >::iterator itr;
//	for( itr=RewardItemIdsAll.begin(); itr != RewardItemIdsAll.end(); itr++ )
//		pPlayer->GetItemInterface()->RemoveItemAmt( *itr, 0x00007FFF, true );

//uint32 a0 = pPlayer->GetUInt32Value( PLAYER_BYTES );
//uint32 a1 = pPlayer->GetUInt32Value( PLAYER_BYTES_2 );
//uint32 a2 = pPlayer->GetUInt32Value( UNIT_FIELD_BYTES_0 );
//uint32 a4 = pPlayer->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID );
	if( Team == 0 )
	{
		pPlayer->setRace( RACE_HUMAN );
		pPlayer->setClass( WARRIOR );
		pPlayer->setGender( 1 );
		pPlayer->SetFaction( CUSTOM_BG_FACTION_ALLIANCE );
		pPlayer->SetUInt32Value( PLAYER_BYTES, 101449987 );
		pPlayer->SetUInt32Value( PLAYER_BYTES_2, 33554435 );
		pPlayer->SetUInt32Value( UNIT_FIELD_BYTES_0, 16843009 );
		pPlayer->SetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID, 50 );
		pPlayer->SetUInt32Value( UNIT_FIELD_DISPLAYID, 50 );
	}
	else if( Team == 1 )
	{
		pPlayer->setRace( RACE_BLOODELF );
		pPlayer->setClass( WARRIOR );
		pPlayer->setGender( 1 );
		pPlayer->SetFaction( CUSTOM_BG_FACTION_HORDE );
		pPlayer->SetUInt32Value( PLAYER_BYTES, 84740105 );
		pPlayer->SetUInt32Value( PLAYER_BYTES_2, 33554440 );
		pPlayer->SetUInt32Value( UNIT_FIELD_BYTES_0, 16843018 );
		pPlayer->SetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID, 15475 );
		pPlayer->SetUInt32Value( UNIT_FIELD_DISPLAYID, 15475 );
	}

	//make sure we have a melee weapon and a ranged weapon
	if( pPlayer->GetItemInterface() )
	{
		//select * from items where inventorytype = 0xd and class = 2 and subclass = 7 and entry > 200000;
		//update items set quality = 8 where entry=25;
		if( pPlayer->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND ) == NULL && pPlayer->GetItemInterface()->GetItemCount( 25, false ) == 0 )
			pPlayer->GetItemInterface()->AddItemById( 25, 1, 0 );
		//select * from items where inventorytype = 0xf and class = 2 and subclass = 2 and entry > 200000;
		//update items set quality = 8 where entry=2504;
		if( pPlayer->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED ) == NULL && pPlayer->GetItemInterface()->GetItemCount( 2504, false ) == 0 )
			pPlayer->GetItemInterface()->AddItemById( 2504, 1, 0 );
	}
	//update playercreateinfo_classlevelstats set basehp = 100, basemana = 0;
	//update playercreateinfo_levelstats set strength=0,agility=0,stamina=0,intellect=0,spirit=0;
	pPlayer->SetGold( 0 );

	//be able to wield anything
	pPlayer->_AddSkillLine( SKILL_SWORDS, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_AXES, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_BOWS, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_GUNS, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_MACES, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_2H_SWORDS, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_STAVES, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_2H_MACES, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_2H_AXES, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_DAGGERS, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_THROWN, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_CROSSBOWS, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_POLEARMS, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_PLATE_MAIL, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_MAIL, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_LEATHER, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_CLOTH, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_FIST_WEAPONS, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_COMPANIONS, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );
	pPlayer->_AddSkillLine( SKILL_LANG_COMMON, PLAYER_SKILL_CAP, PLAYER_SKILL_CAP );

	/* no skills - reset to defaults */
	PlayerCreateInfo *info = objmgr.GetPlayerCreateInfo( pPlayer->getRace(), pPlayer->getClass() );
	for(std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss != info->skills.end(); ss++)
	{
		pPlayer->_AddSkillLine( ss->skillid, ss->currentval, ss->maxval );		
		uint32 NeedsSpell = GetSpellForLanguage( ss->skillid );
		if( NeedsSpell && pPlayer->HasSpell( NeedsSpell ) == false )
			pPlayer->addSpell( NeedsSpell );
	}

}

void OnEnterWorld(Player * pPlayer)
{
	if( pPlayer && pPlayer->GetMapId() == CUSTOM_BG_MAP_ID )
	{
		ResetPlayerForTeam( pPlayer, -1 );
		pPlayer->BroadcastMessage( "Welcome to PVEArena. Your main objective is to kill the enemy king" );
		pPlayer->BroadcastMessage( "Get your current objectives by typing : #objectives" );
	}
}

int GetCreatureGroupFromEntry( uint32 Entry )
{
	std::list< QuestKillObjective* >::iterator itr;
	for( itr = QuestObjectiveList.begin(); itr != QuestObjectiveList.end(); itr++ )
	{
		if( (*itr)->Entry == Entry )
			return (*itr)->Group;
	}
	return 0;
}

void OnKilledUnit(Unit *Killer, Unit *Victim)
{
	if( Killer && Killer->GetMapId() == CUSTOM_BG_MAP_ID && Victim )
	{
		//this is an NPC kill
		if( Victim->GetEntry() )
		{
			uint32 CGroup = GetCreatureGroupFromEntry( Victim->GetEntry() );
			//check if quest objective
			std::list< QuestKillObjective* >::iterator itr;
			for( itr = QuestObjectiveList.begin(); itr != QuestObjectiveList.end(); itr++ )
				if( (*itr)->Group == CGroup )
					(*itr)->KillCountNow++;
		}
	}
}

//UPDATE playercreateinfo SET mapid=530,positionx=-1863,positiony=5429,positionz=-9,basestrength=0,baseagility=0,basestamina=0,BaseIntellect=0,BaseSpirit=0,BaseHealth=100,BaseMana=0;
//UPDATE playercreateinfo_classlevelstats SET basehp=100,basemana=0;
//UPDATE playercreateinfo_levelstats SET strength=0,agility=0,stamina=0,intellect=0,spirit=0;

void SetupPlayers( ScriptMgr* mgr )
{
	//0 HP regen for players
//	sWorld.setRate( RATE_HEALTH, 0.0f );
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_ENTER_WORLD, &OnEnterWorld );
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_PRE_DIE, &OnKilledUnit );
}