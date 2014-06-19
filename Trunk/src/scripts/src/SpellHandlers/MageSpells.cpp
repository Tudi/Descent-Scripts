/****************************************************************************
 *
 * SpellHandler Plugin
 * Copyright (c) 2007 Team arcemu
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons,
 * 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
 *
 * EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU
 * ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES
 * ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 */

#include "StdAfx.h"
#include "CollideInterface.h"
#include "Setup.h"

bool ColdSnap(uint32 i, Spell * pSpell)
{
    if(!pSpell->p_caster) 
		return true;
    pSpell->p_caster->ClearCooldownsOnLine(6, pSpell->GetProto()->Id);
	pSpell->p_caster->ClearCooldownForSpell( 82731 );	//fire orb
	pSpell->p_caster->ClearCooldownForSpell( 92283 );	//frostfire orb
    return true;
}

class MirrorImage : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(MirrorImage);

   AI_Spell owner_spells[10];
   uint32	stored_spells;
   uint64	OwnerGUID;	//just in case something will try to erase this
	bool casted_visual_spells;

   void SetupDefaultSpellbookSpell(uint32 page)
   {
		owner_spells[page].agent = AGENT_SPELL;
		owner_spells[page].entryId = 0;	
		owner_spells[page].global_cooldown = 2000;
		owner_spells[page].cooldown = 3000;
		owner_spells[page].initial_cooldown = 0;
		owner_spells[page].max_hp_pct = 0;
		owner_spells[page].maxrange = 30;
		owner_spells[page].min_hp_pct = 0;
		owner_spells[page].minrange = 0;
		owner_spells[page].procChance = 50;
		owner_spells[page].procCount = 0;
		owner_spells[page].shout_before_cast = NULL;
		owner_spells[page].spell = NULL;
		owner_spells[page].spelltargetType = TTYPE_SINGLETARGET;
   }
   //constructor
    MirrorImage(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		OwnerGUID = _unit->GetUInt64Value( UNIT_FIELD_CREATEDBY );
		Player *owner = _unit->GetMapMgr()->GetPlayer( OwnerGUID );
		if( owner )
		{
			_unit->SetUInt32Value( UNIT_FIELD_BYTES_0, owner->GetUInt32Value( UNIT_FIELD_BYTES_0 ) ); 
			_unit->SetUInt32Value( UNIT_FIELD_FLAGS, 8 );
			_unit->SetUInt32Value( UNIT_FIELD_FLAGS_2, 2064 );
			_unit->SetUInt32Value( UNIT_CREATED_BY_SPELL, 88088 );
			_unit->SetUInt32Value( UNIT_FIELD_BYTES_2, owner->GetUInt32Value( UNIT_FIELD_BYTES_2 ) );//weapon sheath and rest
			_unit->SetByte( UNIT_FIELD_BYTES_2, 0, 1 );//animation frozen, just to be blizzlike
			_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, owner->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, owner->GetUInt32Value( UNIT_FIELD_HEALTH ) );
			_unit->SetUInt32Value( UNIT_FIELD_MAXPOWER1, owner->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );
			_unit->SetUInt32Value( UNIT_FIELD_POWER1, owner->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );
			_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, owner->GetUInt32Value( UNIT_FIELD_DISPLAYID ) );
			_unit->SetUInt64Value( UNIT_FIELD_CREATEDBY, owner->GetGUID() );
			_unit->SetUInt64Value( OBJECT_FIELD_DATA, owner->GetUInt32Value( OBJECT_FIELD_DATA ) );
			_unit->SetFloatValue( UNIT_FIELD_BOUNDINGRADIUS, 0.383000f );
			_unit->SetFloatValue( UNIT_FIELD_COMBATREACH, 1.500000f );
			int mainhandfield = PLAYER_VISIBLE_ITEM_1_ENTRYID + (EQUIPMENT_SLOT_MAINHAND * (PLAYER_VISIBLE_ITEM_2_ENTRYID - PLAYER_VISIBLE_ITEM_1_ENTRYID));
			_unit->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_ID, owner->GetUInt32Value( mainhandfield ) );
			int moffhandfield = PLAYER_VISIBLE_ITEM_1_ENTRYID + (EQUIPMENT_SLOT_OFFHAND * (PLAYER_VISIBLE_ITEM_2_ENTRYID - PLAYER_VISIBLE_ITEM_1_ENTRYID));
			_unit->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_ID_1, owner->GetUInt32Value( moffhandfield ) );
			if( owner->GetUInt64Value( UNIT_FIELD_TARGET ) )
				_unit->SetUInt64Value( UNIT_FIELD_TARGET, owner->GetUInt64Value( UNIT_FIELD_TARGET ) );
		}

		casted_visual_spells = false;
		RegisterAIUpdateEvent( 2000 );
		if( _unit->GetAIInterface() )
			_unit->GetAIInterface()->disable_melee = true;
		stored_spells = 0;
		SetupDefaultSpellbookSpell( stored_spells );
		owner_spells[stored_spells].spell = dbcSpell.LookupEntryForced( 42842 );
		stored_spells++;
		AIUpdate(); //call update right away to instant do stuff
    }
    void AIUpdate()
    {
		//check if casting
		if( _unit->isCasting() )
			return;
		//check if we still have an owner
		Player *owner = _unit->GetMapMgr()->GetPlayer( OwnerGUID );
		if( owner == NULL )
			return;
		if( casted_visual_spells == false && _unit->IsInWorld() )
		{
			casted_visual_spells = true;
			owner->CastSpell( _unit, 45204, true ); //Clone Me!
			_unit->CastSpell( _unit, 58836, true );	//Initialize Images
			owner->CastSpell( _unit, 41054, true );	//Copy Weapon
/*
 {SERVER} Packet: (0xA632) SMSG_SPELL_START PacketSize = 37 TimeStamp = 65033103
8F D3 29 FA 03 01 
8F D3 29 FA 03 01 
00 
94 B0 00 00 
0B 00 00 00 
00 00 00 00 
02 00 00 00 
F7 B6 89 9D F0 79 30 F1 

{SERVER} Packet: (0x2A27) SMSG_SPELL_GO PacketSize = 47 TimeStamp = 65033103
8F D3 29 FA 03 01 - this is player 
8F D3 29 FA 03 01 
00 
94 B0 00 00 - ID
09 01 00 00 - flags - SPELL_GO_FLAGS_UNK1000 - SPELL_GO_FLAGS_LOCK_PLAYER_CAST_ANIM 
C5 54 21 1D - time 
01 - targets
B6 89 9D 00 F0 79 30 F1 - target is the mirror image
00 - no moderated targets
02 00 00 00 - unit target flag
F7 B6 89 9D F0 79 30 F1  - compressed guid of the target

{SERVER} Packet: (0x2A37) SMSG_AURA_UPDATE PacketSize = 23 TimeStamp = 65033119
F7 B6 89 9D F0 79 30 F1 
07 
94 B0 00 00 
13 00 4B 00 8F D3 29 FA 03 01 
*/
		}
		//do we have a target ? If not then try to agro owner selection :P
		Unit *target = _unit->GetAIInterface()->GetNextTarget();
		if( target == NULL && owner->GetSelection() )
		{
			target = _unit->GetMapMgr()->GetUnit( owner->GetSelection() );
			if( target && isAttackable( _unit, target ) )
				_unit->GetAIInterface()->AttackReaction( target, 1 );
		}
		if( owner->last_casted_enemy_spell != NULL 
			&& stored_spells < 10-1 
			&& ( owner->last_casted_enemy_spell->c_is_flags & SPELL_FLAG_IS_DAMAGING )
//			&& owner->last_casted_enemy_spell->Id != 2139
//			&& owner->last_casted_enemy_spell->Id != 55342
			&& ( owner->last_casted_enemy_spell->NameHash == SPELL_HASH_FIREBALL || owner->last_casted_enemy_spell->NameHash == SPELL_HASH_FROSTBOLT || owner->last_casted_enemy_spell->NameHash == SPELL_HASH_ARCANE_BLAST )
			)
		{
			//check if we have this spell
			bool have_it = false;
			for(uint32 i=0;i<stored_spells;i++)
				if( owner_spells[i].spell == owner->last_casted_enemy_spell )
				{
					have_it = true;
					break;
				}
			if( have_it == false )
			{
				//reset spellbook ? = cast owner last spell
//				stored_spells = 0;
				//keep building up a spellbook
				SetupDefaultSpellbookSpell( stored_spells );
				owner_spells[ stored_spells ].global_cooldown = owner->last_casted_enemy_spell->CategoryRecoveryTime;
				owner_spells[ stored_spells ].maxrange = sqrt( owner->last_casted_enemy_spell->base_range_or_radius_sqr );
				owner_spells[ stored_spells ].spell = owner->last_casted_enemy_spell;
				owner_spells[ stored_spells ].cooldown = owner->last_casted_enemy_spell->RecoveryTime;
				stored_spells++;
			}
		}
		uint32 tickNow = GetTickCount();
		bool found_spell = false;
		for(int32 i=stored_spells-1;i>=0;i--)
			if( owner_spells[ i ].cooldowntime > tickNow || RandomUInt() % 100 < owner_spells[ i ].procChance 
				)
				continue;
			else
			{
				_unit->GetAIInterface()->SetNextSpell( &owner_spells[ i ] );
				found_spell = true;
				break;
			}
		if( found_spell == false )
		{
			uint32 ind = RandomUInt() % stored_spells;
			_unit->GetAIInterface()->SetNextSpell( &owner_spells[ ind ] );
		}
	}
};

class RingOfFrost : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(RingOfFrost);

   uint32 ActivateAfterStamp;
   InRangeSet unique_targets;	//cast only once per target
    RingOfFrost(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner == NULL )
			return;
		RegisterAIUpdateEvent( 500 );
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
		}
		//tif we enable this mob to be unselectable then he will not be able to cast the spell on others
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_DEAD);
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
		_unit->_setFaction( );
		_unit->SetScale( 1.25f );
		ActivateAfterStamp = GetTickCount() + 3000;
    }
    void AIUpdate()
    {
		if( ActivateAfterStamp != 0 && GetTickCount() < ActivateAfterStamp )
			return;
		if( ActivateAfterStamp != 0 )
		{
			ActivateAfterStamp = 0;
//			_unit->CastSpell( _unit, 91264, true ); //same looking ring as the ring
//			_unit->CastSpell( _unit, 95910, true ); //sound ?
//			_unit->CastSpell( _unit, 96986, true ); //?
//			_unit->CastSpell( _unit, 88165, true ); //?				
		}
		float TotalRadius = 10.0f * 10.0f;
		float SafeZoneRadius = 5.0f * 5.0f;
		
		InRangeSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( !((*itr)->IsUnit()) || !SafeUnitCast( *itr )->isAlive() )
				continue;
			float Dist = Distance2DSq( _unit, (*itr) );
//if( (*itr)->IsPlayer() ) SafePlayerCast( (*itr) )->BroadcastMessage("Dist now %f",Dist);
			if( Dist <= TotalRadius 
				&& Dist > SafeZoneRadius
				&& isAttackable( _unit, SafeUnitCast( *itr ), true )
				&& unique_targets.find( *itr ) == unique_targets.end() )
			{
				unique_targets.insert( (*itr) );
				_unit->CastSpell( SafeUnitCast(*itr), 82691, true );
				Unit *u = _unit->GetTopOwner();
				u->SetPvPFlag();
			}
		}
		_unit->ReleaseInrangeLock();
	}
};

class FlameOrb : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(FlameOrb);

//   bool have_aura;
    bool started_moving;
    float dx,dy,dz;
    uint32 explode_spell;
    FlameOrb(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner->IsPlayer() == false )
			return;
		RegisterAIUpdateEvent( 1000 );
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->Init( _unit, AITYPE_LONER, MOVEMENTTYPE_NONE, NULL );	//do not follow owner
			_unit->GetAIInterface()->disable_melee = true;
//			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
			_unit->GetAIInterface()->m_moveFly = true;
		}
		//we enable this mob to be unselectable then he will not be able to cast the spell on others
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PLUS_MOB | UNIT_FLAG_PLAYER_CONTROLLED );
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS_2, 0x8000 );
		_unit->SetUInt32Value( UNIT_FIELD_BYTES_1, 0x2000000 );
		_unit->SetUInt32Value( UNIT_FIELD_BYTES_2, 0x1 );
		_unit->SetFloatValue( UNIT_FIELD_HOVERHEIGHT, 2.0f );
//UNIT_FIELD_FLAGS = 33554504 = 2000048 = UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PLUS_MOB | UNIT_FLAG_PLAYER_CONTROLLED
//UNIT_FIELD_FLAGS_2 = 32768 = 8000
//UNIT_FIELD_BYTES_1 = 33554432 

		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
		_unit->_setFaction( );
		//inherit dmg done mods
//		_unit->ModDamageDone[ SCHOOL_FIRE ] = owner->GetDamageDoneMod( SCHOOL_FIRE ) / 4;
		_unit->ModDamageDone[ SCHOOL_FIRE ] = owner->GetDamageDoneMod( SCHOOL_FIRE ) / 6;
		_unit->SetUInt32Value( UNIT_FIELD_LEVEL, owner->getLevel() );
		//we need caster facing so once in world we will start moving that way
		float dist = 40.0f;
		dx = owner->GetPositionX() + dist * cosf( owner->GetOrientation() );
		dy = owner->GetPositionY() + dist * sinf( owner->GetOrientation() );
		dz = owner->GetMapMgr()->GetLandHeight( dx, dy, owner->GetPositionZ() );	
		if( dz == VMAP_VALUE_NOT_INITIALIZED )
			dz = _unit->GetPositionZ();
//		dz += 3.0f;	//it is floating above the ground !
		//we initialize once in world
//		have_aura = false;
		Aura *a = owner->HasAuraWithNameHash( SPELL_HASH_FIRE_POWER, 0, AURA_SEARCH_PASSIVE );
		if( a )
			explode_spell = 83619;	//spell id is in the spell description. Might change later
		else
			explode_spell = 0;

		a = owner->HasAuraWithNameHash( SPELL_HASH_CRITICAL_MASS, 0, AURA_SEARCH_PASSIVE );
		if( a )
			_unit->ModDamageDonePct[ SCHOOL_FIRE ] += a->GetSpellProto()->eff[1].EffectBasePoints;
		
		_unit->m_walkSpeed = 2.5f / 2;
		_unit->m_runSpeed = _unit->m_walkSpeed / 2;
		_unit->m_base_runSpeed = _unit->m_runSpeed;
		_unit->m_base_walkSpeed = _unit->m_walkSpeed;

		_unit->m_flySpeed = _unit->m_walkSpeed / 2;
		_unit->m_backFlySpeed = 4.5f / 2;

		_unit->m_backWalkSpeed = 4.5f / 2;	// this should really be named m_backRunSpeed
		_unit->m_swimSpeed = 4.722222f / 2;
		_unit->m_backSwimSpeed = 2.5f / 2;
		_unit->m_turnRate = 3.141593f / 2;
		_unit->m_pitchRate = 3.141593f / 2;
//		_unit->m_flySpeed = 2.0f;	//wtf ? why is this getting to be 7 even if proto says 1 ?
//		_unit->GetAIInterface()->Init( _unit, AITYPE_LONER, MOVEMENTTYPE_NONE, NULL );	//do not follow owner
		//this aura will start shooting laser beams to the closest 1 target
//		if( _unit->HasAura( 82690 ) == false )
//			_unit->CastSpell( _unit, 82690, true );	//this will be casted by default if you are using my db :P
		//the orb itself has speed 10, but it gets shnared to reduce it's speed to 1
//		_unit->CastSpell( _unit, 82736, true );	//this will be casted by default if you are using my db :P
		//flame orb has funky visual on it ?
		_unit->CastSpell( _unit, 82733, true );	//this will be casted by default if you are using my db :P
		
		//start moving in the direction caster faced
//		_unit->GetAIInterface()->MoveTo( dx, dy, dz, 0.0f );
		//no need to update again ? Comment out this if we are going to cast the dmg spell ourself
//		RemoveAIUpdateEvent();
		started_moving = false;
	}
    void AIUpdate()
    {
		if( started_moving == false )
		{
			_unit->GetAIInterface()->MoveTo( dx, dy, dz, 0.0f );
			started_moving = true;
		}
//		if( have_aura == false && _unit->IsInWorld() )
//		{
//			have_aura = true;
//		}
		//just in case spell cast refuzes to work because we are invincible
		Unit *Target = NULL;
		float best_dist = 9999.0f;
		InRangeSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
		{
			itr = itr2;
			itr2++;
			if( !(*itr)->IsUnit() 
				|| !SafeUnitCast((*itr))->isAlive()
				|| isAttackable( _unit, SafeUnitCast(*itr), true ) == false
				)
				continue;

			float tdist = Distance2DSq( (*itr)->GetPositionX(), (*itr)->GetPositionY(), _unit->GetPositionX(), _unit->GetPositionY() );
			if( tdist < best_dist )
			{
				if( CheckLOS( _unit->GetMapId(), _unit->GetPositionNC(), (*itr)->GetPositionNC() ) ) 
				{
					best_dist = tdist;
					Target = SafeUnitCast(*itr);
				}
			}
		}
		_unit->ReleaseInrangeLock();
		if( Target && best_dist < 30.0f * 30.0f )
		{
//			_unit->CastSpell( Target, 82734, true );
//			_unit->CastSpell( Target, 86719, true );		
			_unit->CastSpell( Target, 55927, true );		
			/**/
			Unit *owner = _unit->GetTopOwner();
			if( owner->IsPlayer() == true )
			{
				int32 Dmg = 0;
				int32 Abs = 0;
				owner->HandleProc( PROC_ON_CAST_SPELL, Target, dbcSpell.LookupEntryForced( 55927 ), &Dmg, &Abs );
			}
		}
	}
	void OnRemoveFromWorld()
	{
		if( explode_spell )
			_unit->CastSpell( _unit, explode_spell, true );
	}
};

//Launches a Frostfire Orb forward from the Mage's position, dealing ${$82739m1} 
//Frostfire damage every second to the closest enemy target for 15 secs.  
//Targets damaged by the Frostfire Orb are slowed by $84721s2% for $84721d.
//#http://www.wowhead.com/spell=82731 flame orb
//replace INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `lootid`,modImmunities) 
//VALUES("45322", "74", "74", "1610", "904800", "904800", "0", "1", "0", "2000", "0", "1", "1", "500", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1", "0.5", "84716 0 ", "0", "0", "0", "0", "1", "1", "1", 0x40000001, "2", "45322", 0x0FFFFFFF);
/*
Debug A9Reader : block(3)->type = 2 start at 318 
Debug A9Reader : Got into create object at pos = 318 
Debug A9Reader : read guid at pos = 326 
Debug A9Reader : object type = 3 
Moveblock (010): Read F1 = 0060 
Moveblock (020): Has flag 0x20 Flags1 , flags2 = 48000001 -> reading 10 bytes
Moveblock (030): Has flag 0x40 -> reading 4 floats 
Moveblock (100): In speed block -> reading 9 floats = 36 bytes
Moveblock (110): In Spline block -> reading 4 bytes
Moveblock (150): In Spline block -> reading 32 bytes, splines count 7
Moveblock (160 + 0): In Spline block -> reading 12 bytes
Moveblock (160 + 1): In Spline block -> reading 12 bytes
Moveblock (160 + 2): In Spline block -> reading 12 bytes
Moveblock (160 + 3): In Spline block -> reading 12 bytes
Moveblock (160 + 4): In Spline block -> reading 12 bytes
Moveblock (160 + 5): In Spline block -> reading 12 bytes
Moveblock (160 + 6): In Spline block -> reading 12 bytes
Moveblock (160 + 8): In Spline block -> destination related spineflags Flags1, reading 1 bytes
Moveblock (160 + 8): In Spline block -> reading 12 bytes (last as dest)
Debug A9Reader : read Move Block end at pos = 524 
Debug Datareader : Masklen(24) for mask 0x06 at position 525 (expecting 0x06)
Debug Datareader : The mask : 7F 00 0C E6 40 0E 00 00 00 63 7B 10 0C 00 00 00 00 10 00 00 40 00 00 00 
Debug Datareader : updatemask requires 132 bytes and we have 132 in packet (total 681 and used 549 )
Debug A9Reader : read data end at pos = 681 
Debug A9Reader : block(3)->type = 2 end at 681 
Debug A9Reader : block content :  02 F7 4F 4E 41 0A B1 30 F1 03 60 00 01 00 00 48 00 00 5C 93 14 4E C5 16 9B 44 6D E7 94 C5 0D FD 8F 41 AA F1 8E 3F 00 00 20 41 00 00 20 41 00 00 90 40 6D 1C 97 40 00 00 20 40 00 00 E0 40 00 00 90 40 DB 0F 49 40 DB 0F 49 40 00 00 00 30 11 00 00 00 A7 11 00 00 98 AE 08 11 00 00 80 3F 00 00 80 3F 00 00 00 00 00 00 00 00 07 00 00 00 C0 08 9B 44 9D EE 94 C5 62 52 8F 41 C8 16 9B 44 6C E7 94 C5 62 52 8F 41 28 BF 9B 44 2C 91 94 C5 62 52 96 41 40 E9 9B 44 9C 7B 94 C5 62 52 9D 41 F0 1D 9D 44 7C DD 93 C5 16 33 99 41 40 8E 9D 44 E3 A3 93 C5 16 33 96 41 40 8E 9D 44 E3 A3 93 C5 16 33 96 41 00 40 8E 9D 44 E3 A3 93 C5 16 33 96 41 06 7F 00 0C E6 40 0E 00 00 00 63 7B 10 0C 00 00 00 00 10 00 00 40 00 00 00 4F 4E 41 00 0A B1 30 F1 09 00 01 00 0A B1 00 00 00 00 80 3F F6 B3 C0 00 00 00 F1 1F E9 D7 38 03 00 00 00 01 00 01 00 01 60 CE 0D 00 64 00 00 00 64 00 00 00 40 42 0F 00 60 CE 0D 00 64 00 00 00 64 00 00 00 40 42 0F 00 4A 00 00 00 4A 06 00 00 48 00 00 02 00 80 00 00 D0 07 00 00 D0 07 00 00 00 00 00 3F 00 00 80 3F 64 86 00 00 64 86 00 00 00 00 00 02 00 00 80 3F EA 4A 01 00 01 00 00 00 00 00 00 40
	 OBJECT_FIELD_GUID = 4279887 
	 OBJECT_FIELD_GUID_1 = 4046500106 
	 OBJECT_FIELD_TYPE = 65545 
	 OBJECT_FIELD_ENTRY = 45322 
	 OBJECT_FIELD_SCALE_X = 1.000000 
	 OBJECT_FIELD_DATA = 12628982 
	 OBJECT_FIELD_DATA_1 = 535887872 
	 UNIT_FIELD_CREATEDBY = 54056937 
	 UNIT_FIELD_CREATEDBY_1 = 16777216 
	 UNIT_FIELD_BYTES_0 = 16777472 
	 UNIT_FIELD_HEALTH = 904800 
	 UNIT_FIELD_POWER3 = 100 
	 UNIT_FIELD_POWER4 = 100 
	 UNIT_FIELD_POWER5 = 1000000 
	 UNIT_FIELD_MAXHEALTH = 904800 
	 UNIT_FIELD_MAXPOWER3 = 100 
	 UNIT_FIELD_MAXPOWER4 = 100 
	 UNIT_FIELD_MAXPOWER5 = 1000000 
	 UNIT_FIELD_LEVEL = 74 
	 UNIT_FIELD_FACTIONTEMPLATE = 1610 
	 UNIT_FIELD_FLAGS = 33554504 
	 UNIT_FIELD_FLAGS_2 = 32768 
	 UNIT_FIELD_BASEATTACKTIME = 2000 
	 UNIT_FIELD_BASEATTACKTIME_1 = 2000 
	 UNIT_FIELD_BOUNDINGRADIUS = 0.500000 
	 UNIT_FIELD_COMBATREACH = 1.000000 
	 UNIT_FIELD_DISPLAYID = 34404 
	 UNIT_FIELD_NATIVEDISPLAYID = 34404 
	 UNIT_FIELD_BYTES_1 = 33554432 
	 UNIT_MOD_CAST_SPEED = 1.000000 
	 UNIT_CREATED_BY_SPELL = 84714 
	 UNIT_FIELD_BYTES_2 = 1 
	 UNIT_FIELD_HOVERHEIGHT = 2.000000
*/
class FrostfireOrb : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(FrostfireOrb);

//   bool have_aura;
    bool started_moving;
    float dx,dy,dz;
    uint32 slow_spell;
    FrostfireOrb(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner->IsPlayer() == false )
			return;
		//we initialize once in world
		if( SafePlayerCast( owner )->HasSpell( 84727 ) )
			slow_spell = 84721;
		else
			slow_spell = 82739;

		RegisterAIUpdateEvent( 1000 );
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->Init( _unit, AITYPE_LONER, MOVEMENTTYPE_NONE, NULL );	//do not follow owner
			_unit->GetAIInterface()->disable_melee = true;
//			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
		}
		//we enable this mob to be unselectable then he will not be able to cast the spell on others
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PLUS_MOB | UNIT_FLAG_PLAYER_CONTROLLED );
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS_2, 0x8000 );
		_unit->SetUInt32Value( UNIT_FIELD_BYTES_1, 0x2000000 );
		_unit->SetUInt32Value( UNIT_FIELD_BYTES_2, 0x1 );
		_unit->SetFloatValue( UNIT_FIELD_HOVERHEIGHT, 2.0f );
//UNIT_FIELD_FLAGS = 33554504 = 2000048 = UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PLUS_MOB | UNIT_FLAG_PLAYER_CONTROLLED
//UNIT_FIELD_FLAGS_2 = 32768 = 8000
//UNIT_FIELD_BYTES_1 = 33554432 

		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
		_unit->_setFaction( );
		//inherit dmg done mods
//		_unit->ModDamageDone[ SCHOOL_FIRE ] = owner->GetDamageDoneMod( SCHOOL_FIRE ) / 2;
//		_unit->ModDamageDone[ SCHOOL_FROST ] = owner->GetDamageDoneMod( SCHOOL_FROST ) / 4;	//25% is the frost specialization bonus
		_unit->ModDamageDone[ SCHOOL_FIRE ] = owner->GetDamageDoneMod( SCHOOL_FIRE ) / 6;
		_unit->ModDamageDone[ SCHOOL_FROST ] = owner->GetDamageDoneMod( SCHOOL_FROST ) / 6;	//25% is the frost specialization bonus
		Aura *a = owner->HasAuraWithNameHash( SPELL_HASH_CRITICAL_MASS, 0, AURA_SEARCH_PASSIVE );
		if( a )
			_unit->ModDamageDonePct[ SCHOOL_FIRE ] += a->GetSpellProto()->eff[1].EffectBasePoints;

		_unit->SetUInt32Value( UNIT_FIELD_LEVEL, owner->getLevel() );
		//we need caster facing so once in world we will start moving that way
		float dist = 40.0f;
		dx = owner->GetPositionX() + dist * cosf( owner->GetOrientation() );
		dy = owner->GetPositionY() + dist * sinf( owner->GetOrientation() );
		dz = owner->GetMapMgr()->GetLandHeight( dx, dy, owner->GetPositionZ() );	
		if( dz == VMAP_VALUE_NOT_INITIALIZED )
			dz = _unit->GetPositionZ();
//		dz += 3.0f;	//it is floating above the ground !

		_unit->m_walkSpeed = 2.5f / 2;
		_unit->m_runSpeed = _unit->m_walkSpeed / 2;
		_unit->m_base_runSpeed = _unit->m_runSpeed;
		_unit->m_base_walkSpeed = _unit->m_walkSpeed;

		_unit->m_flySpeed = _unit->m_walkSpeed / 2;
		_unit->m_backFlySpeed = 4.5f / 2;

		_unit->m_backWalkSpeed = 4.5f / 2;	// this should really be named m_backRunSpeed
		_unit->m_swimSpeed = 4.722222f / 2;
		_unit->m_backSwimSpeed = 2.5f / 2;
		_unit->m_turnRate = 3.141593f / 2;
		_unit->m_pitchRate = 3.141593f / 2;

//		_unit->m_flySpeed = 2.0f;	//wtf ? why is this getting to be 7 even if proto says 1 ?
		//this aura will start shooting laser beams to the closest 1 target
//		if( _unit->HasAura( 84716 ) == false )
//			_unit->CastSpell( _unit, 84716, true );	//this will be casted by default if you are using my db :P
		//the orb itself has speed 10, but it gets shnared to reduce it's speed to 1
//		_unit->CastSpell( _unit, 82736, true );	//this will be casted by default if you are using my db :P
		//flame orb has funky visual on it ?
		_unit->CastSpell( _unit, 82733, true );	//this will be casted by default if you are using my db :P
		
		//start moving in the direction caster faced
		started_moving = false;
//		_unit->GetAIInterface()->MoveTo( dx, dy, dz, 0.0f );

		//no need to update again ? Comment out this if we are going to cast the dmg spell ourself
//		RemoveAIUpdateEvent();
	}
    void AIUpdate()
    {
		if( started_moving == false )
		{
			_unit->GetAIInterface()->MoveTo( dx, dy, dz, 0.0f );
			started_moving = true;
		}
//		if( have_aura == false && _unit->IsInWorld() )
//		{
//			have_aura = true;
//		}
		//just in case spell cast refuzes to work because we are invincible
		Unit *Target = NULL;
		float best_dist = 9999.0f;
		InRangeSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
		{
			itr = itr2;
			itr2++;
			if( !(*itr)->IsUnit() 
				|| !SafeUnitCast((*itr))->isAlive()
				|| isAttackable( _unit, SafeUnitCast(*itr), true ) == false
				)
				continue;

			float tdist = Distance2DSq( (*itr)->GetPositionX(), (*itr)->GetPositionY(), _unit->GetPositionX(), _unit->GetPositionY() );
			if( tdist < best_dist )
			{
				if( CheckLOS( _unit->GetMapId(), _unit->GetPositionNC(), (*itr)->GetPositionNC() ) ) 
				{
					best_dist = tdist;
					Target = SafeUnitCast(*itr);
				}
			}
		}
		_unit->ReleaseInrangeLock();
		if( Target && best_dist < 30.0f * 30.0f )
		{
			_unit->CastSpell( Target, slow_spell, true );
			Unit *owner = _unit->GetTopOwner();
			if( owner->IsPlayer() == true )
			{
				int32 Dmg = 0;
				int32 Abs = 0;
				owner->HandleProc( PROC_ON_CAST_SPELL, Target, dbcSpell.LookupEntryForced( slow_spell ), &Dmg, &Abs );
				Target->HandleProc( PROC_ON_SPELL_LAND_VICTIM, owner, dbcSpell.LookupEntryForced( slow_spell ), &Dmg, &Abs );
			}
		}
	}
	void OnRemoveFromWorld()
	{
	}
};

void SetupMageSpells(ScriptMgr * mgr)
{
    mgr->register_dummy_spell(11958, &ColdSnap);
	mgr->register_creature_script( 47244, &MirrorImage::Create);	//mirror image
	mgr->register_creature_script( 53438, &MirrorImage::Create);	//mirror image 2 from T12
	mgr->register_creature_script( 44199, &RingOfFrost::Create);	//Ring of Frost
	mgr->register_creature_script( 44214, &FlameOrb::Create);		//Flaming Orb
	mgr->register_creature_script( 45322, &FrostfireOrb::Create);	//FrostFire Orb
}
