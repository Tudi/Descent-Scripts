#include "StdAfx.h"
#include "Setup.h"

class ForceOfNature : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(ForceOfNature);

   //constructor
    ForceOfNature(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		//only handle spell created mobs
		if( pCreature->GetUInt32Value( UNIT_FIELD_CREATEDBY ) == 0 )
			return;

		// !! not anymore in cattaclysm
		//check if difficulty proto has "wrath" as ai_agent. If not then we will add it
/*		CreatureProto *proto = pCreature->GetProto();
		if( proto->spells.empty() == true )
		{
			AI_Spell *sp = new AI_Spell;
			memset( sp, 0, sizeof( AI_Spell ) );
			sp->entryId = 1964;
			sp->agent = AGENT_SPELL;
			sp->minrange = 2;
			sp->procChance = 100;
			sp->procCount = MAX_INT;
			sp->spell = dbcSpell.LookupEntryForced( 5176 );
			sp->maxrange = 30;
			sp->spelltargetType = TTYPE_SINGLETARGET;
			sp->cooldown = 2000;
			sp->min_hp_pct = 0;
			sp->autocast_type=AUTOCAST_EVENT_NONE;
			sp->max_hp_pct = 100;
			sp->initial_cooldown = 0;
			sp->global_cooldown = 0; //have this value in milliseconds
			sp->difficulty_mask = 0x00FFFFFF; //prev field is shout
			proto->spells.push_back( sp );
			//also add it to us
			pCreature->GetAIInterface()->addSpellToList( sp );
		} */
		//low dmg
		Player *owner = _unit->GetMapMgr()->GetPlayer( _unit->GetUInt32Value( UNIT_FIELD_CREATEDBY ) );
		if( owner != NULL )
		{
			//health from wowhead
//			uint32 health_per_level = 300/61;
//			pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH, health_per_level * pCreature->getLevel() + 100 );
			pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH, owner->GetMaxHealth() / 5 );
			pCreature->SetUInt32Value(UNIT_FIELD_HEALTH,pCreature->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

			float base_dmg = 5.0f + owner->GetDamageDoneMod(SCHOOL_NATURE) * 15.0f / 100.0f;	//around 800-1000 dmg
			if( base_dmg < 150 )
				base_dmg = owner->getLevel() * 1000.0f / 80.0f;
/*			Aura *a = owner->FindAuraPassiveByNameHash( SPELL_HASH_BRAMBLES );
			if( a )
			{
//				base_dmg += base_dmg * a->m_modList[2] / 100;
				base_dmg += base_dmg * (a->GetSpellProto()->eff[0].EffectBasePoints + 1) / 100;
				//register spell that will stun target
				SpellEntry *sp = dbcSpell.LookupEntryForced( CREATURE_SPELL_TO_DAZE );	
				ProcTriggerSpell pts( sp );
				pts.caster = _unit->GetGUID();
				pts.spellId = sp->Id;	
				pts.procFlags = PROC_ON_MELEE_ATTACK;
				pts.created_with_value = sp->eff[0].EffectBasePoints + 1;
				pts.procCharges = 0;
				pts.procChance = 15;
				_unit->RegisterScriptProcStruct(pts);
			}*/
			pCreature->SetFloatValue( UNIT_FIELD_MINDAMAGE, base_dmg );
			pCreature->SetFloatValue( UNIT_FIELD_MAXDAMAGE, base_dmg * 3 / 2 );
		}
//		pCreature->BaseAttackType = SCHOOL_NATURE;
   }
	void OnDied(Unit *mKiller)
	{
		Unit *o = _unit->GetTopOwner();
		if( o->HasAuraWithNameHash( SPELL_HASH_FUNGAL_GROWTH, 0, AURA_SEARCH_PASSIVE ) )
		{
			SpellCastTargets targets( NULL );
			targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
			targets.m_destX = _unit->GetPositionX();
			targets.m_destY = _unit->GetPositionY();
			targets.m_destZ = _unit->GetPositionZ();
			SpellEntry *spellInfo = dbcSpell.LookupEntry( 94339 ); //Fungal Growth Visual
			Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
			spell->Init( o, spellInfo ,true, NULL);
			spell->prepare(&targets);
		}
	}
};


class WildMushroom : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(WildMushroom);

    WildMushroom(Creature* pCreature) : CreatureAIScript(pCreature)
    {
//		RegisterAIUpdateEvent( 1000 );
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
		}
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
			//inherit full spell power ?
			for(uint32 i=0;i<SCHOOL_COUNT;i++)
			{
				_unit->ModDamageDone[i] = owner->GetDamageDoneMod( i );
				_unit->ModDamageDonePct[i] = owner->GetDamageDonePctMod( i );
			}
			//let's make sure owner does not have more then 3 mushrooms. If he has more then we should try to despawn some
			int64 *counter = owner->GetCreateIn64Extension( EXTENSION_ID_WILD_MUSHROOM_INDEX );
			*counter = (*counter + 1) % 3;	//next mushroom is stored on next index
			uint32 our_index = 1 + (uint32)*counter;
			int64 *MushroomGuidStore = owner->GetCreateIn64Extension( EXTENSION_ID_WILD_MUSHROOM_GUID1 + our_index );
			//let's remove old guid if there was one
			if( *MushroomGuidStore )
			{
				Creature *c = owner->GetMapMgr()->GetCreature( (uint64)(*MushroomGuidStore) );
				if( c )
					c->SummonExpire();
			}
			//let's store guid so 
			*MushroomGuidStore = _unit->GetGUID();
		}
		_unit->CastSpellDelayed( _unit->GetGUID() ,1784 ,6000 );
	}
/*	void OnRemoveFromWorld()
	{
		//detonate anyway on expire
		if( _unit->isAlive() )
			_unit->CastSpell( _unit, 78777, true );		//Wild Mushroom - dmg
	} */
	void OnDied(Unit *mKiller)
	{
		Unit *o = _unit->GetTopOwner();
		if( o->HasAuraWithNameHash( SPELL_HASH_FUNGAL_GROWTH, 0, AURA_SEARCH_PASSIVE ) )
		{
			SpellCastTargets targets( NULL );
			targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
			targets.m_destX = _unit->GetPositionX();
			targets.m_destY = _unit->GetPositionY();
			targets.m_destZ = _unit->GetPositionZ();
			SpellEntry *spellInfo = dbcSpell.LookupEntry( 94339 ); //Fungal Growth Visual
			Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
			spell->Init( o, spellInfo ,true, NULL);
			spell->prepare(&targets);
		}
	}
};


void SetupDruidSpells(ScriptMgr * mgr)
{
	mgr->register_creature_script( 1964, &ForceOfNature::Create);	//force of nature
	mgr->register_creature_script( 47649, &WildMushroom::Create);	//wild mushroom
}