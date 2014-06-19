#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305) // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

#define MOB_ENTRY_FIRELANDS_TELEPORTER		    55274
#define EXTENSION_MAP_BOS_KILLS					MOB_ENTRY_FIRELANDS_TELEPORTER
#define EXTENSION_FLAG_BETHLIAC_DIE				1
#define EXTENSION_FLAG_BALEROC_DIED				2
#define EXTENSION_FLAG_SHANNOX_DIED				4

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_IMMOLATION_TRAP	99839
#define SPELL_COOL_IMMOLATION_TRAP	23000
#define SPELL_ID_IMMOLATION_TRAP_BURN 99838
#define SPELL_ID_CHRISTAL_TRAP		99836
#define SPELL_ID_CHRISTAL_TRAP_STUN	99837
#define MOB_ID_CHRISTAL_TRAP_STUN	53819
#define SPELL_COOL_CHRISTAL_TRAP	27000
#define SPELL_ID_ARCING_SLASH		99931
#define SPELL_COOL_ARCING_SLASH		3000
#define SPELL_ID_HURL_SPEAR			100002
#define SPELL_COOL_HURL_SPEAR		9000
#define SPELL_ID_MAGMA_RUPTURE		99840
#define SPELL_COOL_MAGMA_RUPTURE	15000
#define SPELL_ID_FRENZY				100522
#define SPELL_ID_LIMB_RIP			99832
#define SPELL_COOL_LIMB_RIP			6000
#define SPELL_ID_FACE_RAGE			99832
#define SPELL_COOL_FACE_RAGE		6000
#define SPELL_ID_SEPARATION_ANXIETY 99835
#define TRAP_ARMING_DURATION		3000

class Shannox : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Shannox);

   /* 
    Immolation Trap – Shannox launches a fiery trap at a random player's location. The trap arms after 2 seconds, then detonates when either a player or one of Shannox's pets steps over the trap. Immolation Trap inflicts 71250 to 78750 Fire damage, 19000 to 21000 Fire damage every 3 sec., and increases the target's damage taken by 40% for 9 sec.
    Crystal Prison Trap – Shannox launches a prison trap at a random player's location. The trap arms after 2 seconds, then detonates when either a player or one of Shannox's pets steps over the trap. The Crystal Prison trap encases the target in a block of magma crystal, preventing all movement and any other action. Destroying the crystal prison frees a trapped player.
    Arcing Slash – Shannox inflicts 125% of normal melee damage in a 120 degree cone up to 10 yards in front of him, and inflicts Jagged Tear on his current target.
        Jagged Tear – Shannox's Arcing Slash leaves a Jagged Tear that inflicts 3000 Physical damage every 3 sec. for 30 sec. This effect stacks. 
    Hurl Spear – Shannox hurls his spear at a location near Riplimb. When the spear lands it inflicts 117000 to 123000 Physical damage to all enemies within 3 yards and 46250 to 53750 Fire damage to all enemies within 50 yards. The spear strike also triggers a cascade of molten eruptions around the impact location, inflicting 76000 to 84000 Fire damage to enemies caught in an eruption. Riplimb will then break off from combat, fetch the spear, and return it to Shannox.
    Frenzy – When the players defeat Riplimb or Rageface, Shannox enters a frenzy. This increases his Physical damage by 30% and attack speed by 30%. This effects stacks twice.
    Magma Rupture – Once Riplimb has been slain, Shannox drives his spear into the ground instead of hurling it. This attack inflicts 47500 to 52500 Fire damage and increases the Fire damage taken for all players by 40% for 1 min. The spear strike also triggers a cascade of molten eruptions around the impact location, inflicting 76000 to 84000 Fire damage to enemies caught in an eruption.
    Riplimb – Riplimb will attack the target with the most threat.

    In Heroic Difficulty Riplimb cannot be permanently slain while his master lives. When his health reaches zero, he will collapse for 30 seconds, and then reanimate at full health to resume fighting.
        Limb Rip – Riplimb savagely bites his current target, inflicting 130% of normal melee damage and inflicting Jagged Tear.
            Jagged Tear – Riplimb's Limb Rip eaves a Jagged Tear that inflicts 3000 Physical damage every 3 sec. for 30 sec. This effect stacks. 
        Frenzied Devotion – Riplimb enters an unstoppable rage if he witnesses Shannox's health reach 30%. This effect increases Riplimb's damage dealt by 400%, attack speed by 200% and movement speed by 100%.
        HeroicFeeding Frenzy – Riplimb's successful melee attacks increase his physical damage dealt by 5% for 20 sec. This effect stacks. 
    Rageface – Rageface cannot be controlled, and darts about from enemy to enemy, changing targets periodically.
        Face Rage – Rageface leaps at a random player, stunning and knocking them to the ground. Rageface begins to viciously maul the player, inflicting 8000 Physical damage every 0.5 sec. and the damage dealt increases over time. While Rageface mauls the player, he is 50% more susceptible to critical strikes. Rageface will continue the mauling until his target is dead, or he receives a single attack that deals at least 30000 damage.
        Frenzied Devotion – Rageface enters an unstoppable rage if he witnesses Shannox's health reach 30%. This effect increases Riplimb's damage dealt by 400%, attack speed by 200% and movement speed by 100%.
        HeroicFeeding Frenzy – Rageface's successful melee attacks increase his physical damage dealt by 5% for 20 sec. This effect stacks. 
	*/
	uint32	phase;
	uint32	ImmolationTrapTimer;
	uint32	CristalPrisonTimer;
	uint32	ArcingSlashTimer;
	uint32	HurlSpearTimer;
	uint32	MagmaRuptureTimer;
	bool	StartedCombat;
	float	DifficultyScale;
	Shannox(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		phase = 0;
	}
	void OnLoad()
	{
		//make 25 man mode harder then 10 man mode ?
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
		{
			uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
			_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now - 500 ) );	//attack a bit faster cause there are more healers
		}
		StartedCombat = false;
		RegisterAIUpdateEvent( 500 );
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = 0.93f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = 0.86f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = 0.80f;
		//spear of shannox is an NPC : 53752, 54112
	}
    void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		uint32 TickNow = GetTickCount();
		if( StartedCombat == false )
		{
			StartedCombat = true;
			ImmolationTrapTimer = TickNow + RandomUInt() % 25000;
			CristalPrisonTimer = TickNow + RandomUInt() % 25000;
			ArcingSlashTimer = TickNow + RandomUInt() % 25000;
			HurlSpearTimer = TickNow + RandomUInt() % 25000;
			MagmaRuptureTimer = TickNow + RandomUInt() % 25000;
			//riplimb
			_unit->create_guardian( 53694, 0x0FFFFFFF, 0, 0, 0, 0, false );
			//Rageface
			_unit->create_guardian( 53695, 0x0FFFFFFF, 0, 0, 0, 0, false );
		}
		if( ImmolationTrapTimer < TickNow && _unit->isCasting() == false )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 2.0f;
			sp.maxrange = 45.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			if( t )
			{
				_unit->CastSpell( t, SPELL_ID_IMMOLATION_TRAP, false );		//Throw Immolation Trap
				ImmolationTrapTimer = TickNow + (SPELL_COOL_IMMOLATION_TRAP + RandomUInt() % SPELL_COOL_IMMOLATION_TRAP)*DifficultyScale;
			}
			else
				ImmolationTrapTimer = TickNow + 2000;
		}
		if( CristalPrisonTimer < TickNow && _unit->isCasting() == false )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 2.0f;
			sp.maxrange = 45.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			if( t && t->HasAura( 99837 ) == false )
			{
				_unit->CastSpell( t, SPELL_ID_CHRISTAL_TRAP, false );		//Throw Crystal Prison Trap
				CristalPrisonTimer = TickNow + (SPELL_COOL_CHRISTAL_TRAP + RandomUInt() % SPELL_COOL_CHRISTAL_TRAP)*DifficultyScale;
			}
			else
				CristalPrisonTimer = TickNow + 2000;		
		}
		if( ArcingSlashTimer < TickNow && _unit->isCasting() == false )
		{
			ArcingSlashTimer = TickNow + (SPELL_COOL_ARCING_SLASH + RandomUInt() % SPELL_COOL_ARCING_SLASH)*DifficultyScale;
			_unit->CastSpell( tank, SPELL_ID_ARCING_SLASH, true );	//Arcing Slash
		}
		if( HurlSpearTimer < TickNow && _unit->isCasting() == false )
		{
			HurlSpearTimer = TickNow + (SPELL_COOL_HURL_SPEAR + RandomUInt() % SPELL_COOL_HURL_SPEAR)*DifficultyScale;
			_unit->CastSpell( tank, SPELL_ID_HURL_SPEAR, false );	//Hurl Spear
		}
		if( MagmaRuptureTimer < TickNow && _unit->HasAura( SPELL_ID_FRENZY ) )
		{
			MagmaRuptureTimer = TickNow + (SPELL_COOL_MAGMA_RUPTURE + RandomUInt() % SPELL_COOL_MAGMA_RUPTURE)*DifficultyScale;
			_unit->CastSpell( tank, SPELL_ID_MAGMA_RUPTURE, false );	//Magma Rupture
		}
	}
	void OnDied(Unit *mKiller)
	{
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		*EncounterStatus |= EXTENSION_FLAG_SHANNOX_DIED;
	}
};

class Rageface : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Rageface);

	uint32 phase;
	uint32 FaceRageTimer;
	Rageface(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		phase = 0;
	}
	void OnLoad()
	{
		RegisterAIUpdateEvent( 2000 );
		FaceRageTimer = GetTickCount() + RandomUInt() % 25000;
		if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
		{
			SpellEntry *sp = dbcSpell.LookupEntryForced( 100656 );	//Feeding Frenzy
			ProcTriggerSpell pts( sp, NULL );
			pts.caster = _unit->GetGUID();
			pts.spellId = sp->Id;	
			pts.procFlags = PROC_ON_MELEE_ATTACK;
			pts.procCharges = 0;
			pts.procChance = 100;
			_unit->RegisterScriptProcStruct(pts);
		}
	}
	void OnDied(Unit *mKiller)
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner )
		{
			owner->CastSpell( owner, SPELL_ID_FRENZY, true );
		}
		RemoveAIUpdateEvent();
	}
    void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		Unit *owner = _unit->GetTopOwner();
		if( owner )
		{
			if( phase == 0 )
			{
				if( owner->GetHealthPct() < 30 )
				{
					phase = 1; 
					_unit->CastSpell( _unit, 100064, true );	//Frenzied Devotion
				}
			}
			if( owner->GetDistanceSq( _unit ) > 60.0f*60.0f )
				_unit->CastSpell( _unit, SPELL_ID_SEPARATION_ANXIETY, true );	//Separation Anxiety
		}
		if( FaceRageTimer < GetTickCount() && _unit->isCasting() == false )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 5.0f;
			sp.maxrange = 45.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			if( t )
			{
				_unit->CastSpell( t, SPELL_ID_FACE_RAGE, true );		//Face Rage
				FaceRageTimer = GetTickCount() + SPELL_COOL_FACE_RAGE;
			}
			else
				FaceRageTimer = GetTickCount() + 2000;		
		}
	}
};

class Riplimb : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Riplimb);

	uint32 phase;
	uint32 LimbRipTimer;
	Riplimb(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		phase = 0;
	}
	void OnLoad()
	{
		RegisterAIUpdateEvent( 2000 );
		LimbRipTimer = GetTickCount() + RandomUInt() % 25000;
		if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
		{
			SpellEntry *sp = dbcSpell.LookupEntryForced( 100656 );	//Feeding Frenzy
			ProcTriggerSpell pts( sp, NULL );
			pts.caster = _unit->GetGUID();
			pts.spellId = sp->Id;	
			pts.procFlags = PROC_ON_MELEE_ATTACK;
			pts.procCharges = 0;
			pts.procChance = 100;
			_unit->RegisterScriptProcStruct(pts);
		}
	}
	void OnDied(Unit *mKiller)
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner )
		{
			owner->CastSpell( owner, SPELL_ID_FRENZY, true );
		}
		RemoveAIUpdateEvent();
	}
    void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		Unit *owner = _unit->GetTopOwner();
		if( owner )
		{
			if( phase == 0 )
			{
				if( owner->GetHealthPct() < 30 )
				{
					phase = 1; 
					_unit->CastSpell( _unit, 100064, true );	//Frenzied Devotion
				}
			}
			if( owner->GetDistanceSq( _unit ) > 60.0f*60.0f )
				_unit->CastSpell( _unit, SPELL_ID_SEPARATION_ANXIETY, true );	//Separation Anxiety
		}
		if( LimbRipTimer < GetTickCount() && _unit->isCasting() == false )
		{
			_unit->CastSpell( tank, SPELL_ID_LIMB_RIP, true );		//Limb Rip
			LimbRipTimer = GetTickCount() + SPELL_COOL_LIMB_RIP;
		}
	}
};

class CristalPrisonTrap : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(CristalPrisonTrap);

   uint32 TrapArmedStamp;
   bool deleteme;
    CristalPrisonTrap(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 2000 );
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
		TrapArmedStamp = GetTickCount() + TRAP_ARMING_DURATION;
		deleteme = false;
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
		if( deleteme == true || TrapArmedStamp > GetTickCount() )
			return;
		float r = 5.0f * 5.0f;
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
				_unit->CastSpell( SafeUnitCast(*itr), SPELL_ID_CHRISTAL_TRAP_STUN, true );
				_unit->GetMapMgr()->CreateCreature( MOB_ID_CHRISTAL_TRAP_STUN, true, (*itr)->GetPositionX(), (*itr)->GetPositionY(), (*itr)->GetPositionZ() );
//				LocationVector v = _unit->GetPosition();
//				_unit->create_guardian( 53819, 0x0FFFFFFF, 0, NULL, &v );
				deleteme = true;
				break;
			}
		}
		_unit->ReleaseInrangeLock();
		if( deleteme )
		{
			RemoveAIUpdateEvent();
			_unit->SafeDelete();
		}
	}
};

class CristalPrison : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(CristalPrison);

    CristalPrison(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 2000 );
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
		}
		_unit->CastSpell( _unit, 100265, true );	//Melt
	}
	void OnDied(Unit *mKiller)
	{
		Player *closestplayer = NULL;
		float closest_dist = 99999999.0f;
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( !SafeUnitCast( *itr )->isAlive() )
				continue;
			float tdist = ( *itr )->GetDistanceSq( _unit );
			if( tdist < closest_dist && (*itr)->HasAura( SPELL_ID_CHRISTAL_TRAP_STUN ) )
			{
				closest_dist = tdist;
				closestplayer = *itr;
			}
		}
		_unit->ReleaseInrangeLock();
		if( closestplayer )
		{
			closestplayer->RemoveAura( 99837 );
		}
		RemoveAIUpdateEvent();
	}
};

class ImmolationTrap : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(ImmolationTrap);

   uint32 TrapArmedStamp;
   bool deleteme;
    ImmolationTrap(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 2000 );
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
		TrapArmedStamp = GetTickCount() + TRAP_ARMING_DURATION;
		deleteme = false;
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
		if( deleteme == true || TrapArmedStamp > GetTickCount() )
			return;
		float r = 5.0f * 5.0f;
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
				_unit->CastSpell( SafeUnitCast(*itr), SPELL_ID_IMMOLATION_TRAP_BURN, true );
				deleteme = true;
				break;
			}
		}
		_unit->ReleaseInrangeLock();
		if( deleteme )
		{
			RemoveAIUpdateEvent();
			_unit->SafeDelete();
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_BLAZE_OF_GLORY 	99252				
#define SPELL_COOL_BLAZE_OF_GLORY 	20000				
#define SPELL_ID_INCENDIARY_SOUL 	99369				
#define SPELL_ID_DECIMATION_BLADE 	99352				
#define SPELL_ID_INFERNO_BLADE 		99350				
#define SPELL_COOL_BLADE_SWITCH		25000
#define SPELL_ID_SHARDS_OF_TORMENT 	99260				
#define SPELL_ID_SHARDS_OF_TORMENT_V 	99259				
#define SPELL_COOL_SHARDS_OF_TORMENT 	47000				
#define MOB_ID_SHARD_OF_TORMENT		53495
#define SPELL_ID_TORMENT		 	99256				
#define SPELL_ID_TORMENTED		 	99257				
#define SPELL_ID_WAVE_OF_TORMENT 	99261				
#define SPELL_ID_COUNTDOWN		 	99516
#define SPELL_ID_COUNTDOWN_EXPLODE 	99518
#define SPELL_COOL_COUNTDOWN	 	24000

class Baleroc : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Baleroc);

   /* 
   Blaze of Glory – Baleroc's assault periodically awakens a burning spark within his current target, increasing the target's physical damage taken by 20%, but also raising their maximum health by 20%.Every time Baleroc applies Blaze of Glory, he gains an application of Incendiary Soul.
   Incendiary Soul – Every time Baleroc applies Blaze of Glory, he gains an application of Incendiary Soul, increasing his Fire damage dealt by 20%. 
   Shards of Torment – Baleroc summons a crystal amongst his foes, which continually channels a shadowy beam on the nearest player.

    In 25 player raids, Baleroc summons two crystals.
        Torment – The Shard of Torment inflicts 4000 Shadow damage per application to the nearest player, stacking once per second.
        Tormented – When Torment fades from a player, they are afflicted by the Tormented effect, which increases their Shadow damage taken by 250% and reduces the player's healing done by 50% for 40 sec.

        Direct melee contact with any other player will apply a fresh copy of the Tormented effect to that player.
        Wave of Torment – If no player stands within 15 yards of a Shard of Torment, the Shard pulses this effect, inflicting 14250 to 15750 Shadow damage each second to all players.
        HealerVital Spark – If a player casts a direct heal on someone being damaged by Torment, the healer gains an application of Vital Spark for each three stacks of Torment on the target. Casting a single-target direct heal on a target affected by Blaze of Glory will trigger Vital Flame.
        HealerVital Flame – Vital Flame increases healing done to targets affected by Blaze of Glory by 5% per stack of Vital Spark consumed, lasting for 15 sec. When Vital Flame expires, it restores the Vital Spark stacks consumed to create the effect. 

	Blades of Baleroc
		Baleroc periodically empowers one of his blades with Shadow or Fire energy, and wields it alone for 15 sec.

    HeroicCountdown – Baleroc links two players to each other for 8 sec. If the chosen players move within 4 yards of each other, the effect dissipates harmlessly. If the effect runs its full course, then both players explode, inflicting 125000 Fire damage to all players within 100 yards. 
*/
	uint32 phase;
	uint32 BloazeOfGloryTimer;
	uint32 BladeSwitchTimer;
	uint32 BladeCounter;
	uint32 ShardsOfTormentTimer;
	uint32 CountDownTimer;
	float	DifficultyScale;
	Baleroc(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		phase = 0;
	}
	void OnLoad()
	{
		//make 25 man mode harder then 10 man mode ?
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
		{
			uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
			_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now - 500 ) );	//attack a bit faster cause there are more healers
		}
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = 0.93f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = 0.86f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = 0.80f;
		RegisterAIUpdateEvent( 500 );
	}
	void OnDied(Unit *mKiller)
	{
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		*EncounterStatus |= EXTENSION_FLAG_BALEROC_DIED;

		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( ( *itr )->isAlive() == false )
				continue;
			( *itr )->RemoveAura( SPELL_ID_BLAZE_OF_GLORY );
		}
		_unit->ReleaseInrangeLock();
		RemoveAIUpdateEvent();
	}
    void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		uint32 TickNow = GetTickCount();
		//init phase
		if( phase == 0 )
		{
			CountDownTimer = TickNow + RandomUInt() % 25000;
			BloazeOfGloryTimer = TickNow + RandomUInt() % 25000;
			ShardsOfTormentTimer = TickNow + RandomUInt() % 25000;
			BladeSwitchTimer = TickNow + RandomUInt() % 25000;
			BladeCounter = 0;
			phase = 1;
		}
		if( ShardsOfTormentTimer < TickNow && _unit->isCasting() == false )
		{
			ShardsOfTormentTimer = TickNow + (SPELL_COOL_SHARDS_OF_TORMENT + RandomUInt() % SPELL_COOL_SHARDS_OF_TORMENT)*DifficultyScale;
			uint32 maxcount = 1;
			if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
				maxcount = 2;
			Unit *selectedtarget = _unit;
			for(uint32 t=0;t<maxcount;t++)
			{
				InRangePlayerSetRecProt::iterator itr2;
				InrangeLoopExitAutoCallback AutoLock;
				for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();itr2++)
				{
					Player *p1 = ( *itr2 );
					if( !p1->isAlive() )
						continue;
					if( p1 == selectedtarget )
						continue;	//don't put 2 shards at the same location
					float dist = _unit->GetDistance2dSq( p1 );
					if( dist > 30.0f * 30.0f || dist < 2.0f*2.0f )
						continue;
					//this player is already near a shard, cut him some slack
					if( p1->HasAura( SPELL_ID_TORMENT ) )
						continue;
					selectedtarget = p1;
				}
				_unit->CastSpell( selectedtarget, SPELL_ID_SHARDS_OF_TORMENT, true );			//The sumon
				_unit->CastSpell( selectedtarget, SPELL_ID_SHARDS_OF_TORMENT_V, false );		//The visual
			}
		}
		if( BladeSwitchTimer < TickNow && _unit->isCasting() == false )
		{
			BladeSwitchTimer = TickNow + (SPELL_COOL_BLADE_SWITCH + RandomUInt() % SPELL_COOL_BLADE_SWITCH)*DifficultyScale;
			if( BladeCounter % 2 == 0 )
				_unit->CastSpell( _unit, SPELL_ID_DECIMATION_BLADE, true );		//Decimation Blade
			else
				_unit->CastSpell( _unit, SPELL_ID_INFERNO_BLADE, true );		//Inferno Blade
			BladeCounter++;
		}
		if( BloazeOfGloryTimer < TickNow && _unit->isCasting() == false )
		{
			BloazeOfGloryTimer = TickNow + (SPELL_COOL_BLAZE_OF_GLORY + RandomUInt() % SPELL_COOL_BLAZE_OF_GLORY)*DifficultyScale;
			_unit->CastSpell( tank, SPELL_ID_BLAZE_OF_GLORY, false );	//Blaze of Glory
			_unit->CastSpell( _unit, SPELL_ID_INCENDIARY_SOUL, false );	//Incendiary Soul
		}
		if( CountDownTimer < TickNow && _unit->isCasting() == false )
		{
			CountDownTimer = TickNow + (SPELL_COOL_COUNTDOWN + RandomUInt() % SPELL_COOL_COUNTDOWN)*DifficultyScale;
			float fard = 999999.0f;
			Player *tp1=NULL,*tp2=NULL;
			Player *tpvect[50];
			uint32 pc = 0;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InRangePlayerSetRecProt::iterator itr,itr2;
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();itr2++)
			{
				Player *p1 = ( *itr2 );
				if( !p1->isAlive() )
					continue;
				if( p1->HasAuraWithNameHash( SPELL_HASH_COUNTDOWN, 0, AURA_SEARCH_NEGATIVE ) != NULL )
					continue;
				tpvect[ pc++ ] = p1;
				if( pc >= 50-1 )
					break;
			}
			_unit->ReleaseInrangeLock();
			for( uint32 i=0;i<pc;i++)
			{
				Player *p1 = tpvect[i];
				for( uint32 j=i+1;j<pc;j++)
				{
					Player *p2 = tpvect[j];
					float distsq = p1->GetDistanceSq( p2 );
					if( distsq > fard && distsq < 60.0f*60.0f )
					{
						fard = distsq;
						tp1 = p1;
						tp2 = p2;
					}
				}
			}
			if( tp1 != NULL )
			{
				tp1->CastSpell( tp2, SPELL_ID_COUNTDOWN, true );	//periodical check
//				tp1->CastSpell( tp1, SPELL_ID_COUNTDOWN, true );	//periodical check
//				tp1->CastSpell( tp1, 99515, true ); //visual ?
//				tp1->CastSpell( tp2, 99519, true );	//visual ?
			}
		}
	}
};

class ShardOfTorment : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(ShardOfTorment);

    ShardOfTorment(Creature* pCreature) : CreatureAIScript(pCreature)
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
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
		}
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 16135 );	//no idea about the real display :(. This is not good for sure !
	}
	void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		float r = 15.0f * 15.0f;
		Player *closestplayer = NULL;
		float bestdist = 99999.0f;
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			Player *p1 = *itr;
			if( p1->isAlive() == false )
				continue;
			if( isAttackable( _unit, p1, true ) == false )
				continue;
			float tdist = p1->GetDistance2dSq( _unit );
			if( tdist < bestdist )
			{
				bestdist = tdist;
				closestplayer = p1;
			}
		}
		_unit->ReleaseInrangeLock();
		if( closestplayer && bestdist < 15.0f * 15.0f )
			_unit->CastSpell( closestplayer, SPELL_ID_TORMENT, false );	//Torment
		else
			_unit->CastSpell( _unit, SPELL_ID_WAVE_OF_TORMENT, false );	//Wave of Torment
	}
};

//triggered periodically by aura to check if player is nearby
bool PeriodicDummyCountdown(uint32 i, Aura * pAura, bool apply)
{
	Unit *ut = pAura->GetTarget();
	if( ut == NULL || ut->GetGUID() == pAura->GetCasterGUID() )
		return true;
	Unit *uc = pAura->GetUnitCaster();
	if( uc == NULL )
	{
		pAura->RemoveDelayed();
		return true;
	}
	float distsq = uc->GetDistanceSq( ut );
	if( distsq < 4.0f * 4.0f )
	{
		uc->RemoveAura( SPELL_ID_COUNTDOWN );
		pAura->RemoveDelayed();
		return true;
	}
	return true;
}

bool PeriodicDummyTormented(uint32 i, Aura * pAura, bool apply)
{
	//check if target has near him in melee range another player
	Unit *target = pAura->GetTarget();
	if( target == NULL || target->IsPlayer() == false )
		return true;
	Player *ptarget = SafePlayerCast( target );
	//only trigger it in this instance
	if( IS_INSTANCE(ptarget->GetMapId()) == 0 )
		return true;
	ptarget->AquireInrangeLock(); //make sure to release lock before exit function !
	InRangePlayerSetRecProt::iterator itr,itr2;
	InrangeLoopExitAutoCallback AutoLock;
	for( itr2 = ptarget->GetInRangePlayerSetBegin( AutoLock ); itr2 != ptarget->GetInRangePlayerSetEnd();itr2++)
	{
		Player *p1 = ( *itr2 );
		if( !p1->isAlive() )
			continue;
		//do not spread it on other players then the ones in our group
		if( p1->GetGroup() != ptarget->GetGroup() )
			continue;
		float dist = ptarget->GetDistance2dSq( p1 );
		if( dist > 2.0f*2.0f )
			continue;
		if( p1->HasAura( SPELL_ID_TORMENTED ) != false )
			continue;
		p1->CastSpell( p1, SPELL_ID_TORMENTED, true );
	}
	ptarget->ReleaseInrangeLock();

	return true;	//we handled the dummy tick
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_CONCUSSIVE_STOMP	97282
#define SPELL_COOL_CONCUSSIVE_STOMP 20000
#define SPELL_ID_IMMOLATION			98598
#define SPELL_COOL_THERMAL_IGNITION	30000
#define MOB_ENTRY_FRAGMENT_OF_RYOLITH	52620
#define MOB_ENTRY_SPARK_OF_RYOLITH	53211
//#define SPELL_ID_VOLCANO			98598
#define MOB_ENTRY_VOLCANO			52582
#define SPELL_ID_MOLTEN_ARMOR		101159
#define SPELL_COOL_VOLCANO			25000

class LordRhyolith : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(LordRhyolith);

   /* 
	Lord Rhyolith ignores players while his armor is intact, but they can attack his feet to control his movement.
		Obsidian Armor – Lord Rhyolith awakens encrusted in a thick coat of Obsidian Armor, which reduces all damage he takes by 80%. Lord Rhyolith's Obsidian Armor is reduced by 10% every time he steps on an active volcano.
		In Heroic Difficulty when Lord Rhyolith steps on an Active Volcano, 10 Liquid Obsidian creatures spawn around him.
			Liquid Obsidian – The Liquid Obsidian moves towards Lord Rhyolith, and casts Fuse when it reaches him.
				Fuse – The Liquid Obsidian fuses with Lord Rhyolith, increasing his damage reduction by 1%. 
		Concussive Stomp – Lord Rhyolith smashes the ground, inflicting 32375 to 37625 Fire damage to all players and knocking away targets within 0 yards. Each stomp creates two to three volcanoes.
			Important Volcano – Lord Rhyolith ignites a Volcano, causing it to inflict 12000 Fire damage to 3 players every 2 sec. When struck, the player takes 5% additional Fire damage for 15 sec. This effect stacks up to 20 times.
		In 25 player raids, the Volcano inflicts damage on 6 players.
		Crater – Lord Rhyolith creates a crater when he steps on an active volcano. Occasionally, Lord Rhyolith causes streams of lava to flow from a crater. The moving stream of lava inflicts 75000 Fire damage to any player in its path. After 10 sec. the stream of lava erupts, inflicting 150000 Fire damage to any player standing within a lava stream.
		Thermal Ignition – Lord Rhyolith releases a jet of cinders, inflicting 15000 Fire damage to players within 0 yards and creates either 5 Fragments of Rhyolith or 1 Spark of Rhyolith.
		In Heroic Difficulty Lord Rhyolith creates 10 Fragments of Rhyolith instead of 5.
			Deadly Fragment of Rhyolith – Fragments of Rhyolith have low health. If not slain within 30 sec, they inflict damage equal to their current health to a random player.
			In 25 person raids, they deal damage equal to half their current health to a random player.
			Spark of Rhyolith – Sparks of Rhyolith inflicts 8075 to 8925 Fire damage to all players within 12 yards.
				DeadlyInfernal Rage – Sparks of Rhyolith increase their damage dealt by 10% and damage taken by 10% every 5 seconds. This effect stacks up to 20 times. 
		Deadly Drink Magma – If Lord Rhyolith reaches the edge of his plateau, he drinks from the liquid magma and inflicts 35000 Fire damage every second on all players for 4 sec. 
	Stage Two: Now you will BURN!
	When Lord Rhyolith reaches 25% remaining health, his Obsidian armor shatters. He becomes attackable and no longer ignores players.
		Immolation – Lord Rhyolith's fiery presence inflicts 7000 to 9000 Fire damage to all players every second.
		Heroic Unleashed Flame – Lord Rhyolith unleashes beams of fire which pursue random players, inflicting 10000 Fire damage to all players within 5 yards of the beam's impact location. 
	*/
	uint32 phase;
	uint32 ConcussiveStompTimer;
	uint32 ThermalIgnitionTimer;
	uint32 ThermalIgnitionCounter;
	uint32 VolcanoTimer;
	float	DifficultyScale;
	LordRhyolith(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		phase = 0;
	}
	void OnLoad()
	{
		//make 25 man mode harder then 10 man mode ?
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
		{
			uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
			_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now - 500 ) );	//attack a bit faster cause there are more healers
		}
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = 0.93f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = 0.86f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = 0.80f;
		RegisterAIUpdateEvent( 500 );
	}
    void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		uint32 TickNow = GetTickCount();
		//init phase
		if( phase == 0 )
		{
			ConcussiveStompTimer = TickNow + RandomUInt() % 25000;
			ThermalIgnitionTimer = TickNow + RandomUInt() % 25000;
			VolcanoTimer = TickNow + RandomUInt() % 25000;
			ThermalIgnitionCounter = 0;
			phase = 1;
		}
		if( phase == 1 && _unit->GetHealthPct() <= 25 )
		{
			_unit->CastSpell( _unit, SPELL_ID_IMMOLATION, true );
			phase = 2;
		}
		if( ConcussiveStompTimer < TickNow && _unit->isCasting() == false )
		{
			ConcussiveStompTimer = TickNow + (SPELL_COOL_CONCUSSIVE_STOMP + RandomUInt() % SPELL_COOL_CONCUSSIVE_STOMP)*DifficultyScale;
			_unit->CastSpell( _unit, SPELL_ID_CONCUSSIVE_STOMP, false );	//Concussive Stomp
		}
		if( ThermalIgnitionTimer < TickNow && _unit->isCasting() == false )
		{
			ThermalIgnitionTimer = TickNow + (SPELL_COOL_THERMAL_IGNITION + RandomUInt() % SPELL_COOL_THERMAL_IGNITION)*DifficultyScale;
			uint32 spawn_entry;
			uint32 spawn_count;
			if( ThermalIgnitionCounter % 2 == 0 )
			{
				spawn_entry = MOB_ENTRY_FRAGMENT_OF_RYOLITH;
				spawn_count =  5;
			}
			else
			{
				spawn_entry = MOB_ENTRY_SPARK_OF_RYOLITH;
				spawn_count =  1;
			}

			if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
				spawn_count = spawn_count * 2;

			for( uint32 i=0;i<spawn_count;i++)
				_unit->create_guardian( spawn_entry, 30000, 0, 0, 0, 0, false );

			ThermalIgnitionCounter++;
		}
		if( VolcanoTimer < TickNow && _unit->isCasting() == false )
		{
			VolcanoTimer = TickNow + (SPELL_COOL_VOLCANO + RandomUInt() % SPELL_COOL_VOLCANO)*DifficultyScale;		
			_unit->create_guardian( MOB_ENTRY_VOLCANO, 30000, 0, 0, 0, 0, false );
		}
	}
};

class FragmentOfRyolith : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(FragmentOfRyolith);

    FragmentOfRyolith(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnRemoveFromWorld()
	{
		if( _unit->GetHealth() == 0 )
			return;
		//deal DMG to a random player equal to our current health
		Player *closestplayer = NULL;
		float bestdist = 99999.0f;
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			Player *p1 = *itr;
			if( p1->isAlive() == false )
				continue;
			if( isAttackable( _unit, p1, true ) == false )
				continue;
			float tdist = p1->GetDistance2dSq( _unit );
			if( tdist < bestdist )
			{
				bestdist = tdist;
				closestplayer = p1;
			}
		}
		_unit->ReleaseInrangeLock();
		if( closestplayer )
			_unit->CastSpell( closestplayer, 98649, true );
	}
};

class SparkOfRyolith : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(SparkOfRyolith);

    SparkOfRyolith(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		_unit->CastSpell( _unit, 98597, true );	//immolation
		RegisterAIUpdateEvent( 1000 );
	}
    void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		_unit->CastSpell( _unit, 98596, true );	//Infernal Rage
	}
};

class Volcano : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Volcano);

   uint32 MagmaCastStamp;
    Volcano(Creature* pCreature) : CreatureAIScript(pCreature)
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
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 38054 );	//has bad display also ?
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
			owner->CastSpell( owner, SPELL_ID_MOLTEN_ARMOR, true );
		}
		_unit->CastSpell( _unit, 98264, true );	//Eruption - This volcano is about to erupt.
		MagmaCastStamp = GetTickCount() + 25000;
	}
    void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		if( GetTickCount() > MagmaCastStamp )
		{
			_unit->RemoveAura( 98264 ); //eruption
			_unit->CastSpell( _unit, 98472, true );	//Magma - Deals Fire damage every 3 sec.
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_EMBER_FLAME		98934
#define SPELL_COOL_EMBER_FLAME		15000
#define SPELL_ID_METEOR_BURN		99076
#define SPELL_COOL_METEOR_BURN		25000
#define SPELL_ID_CONSUME			99304
#define SPELL_COOL_CONSUME			70000
#define SPELL_ID_SMOLDERING_DEVASTATION		99052
#define SPELL_COOL_SMOLDERING_DEVASTATION	70000
#define MOB_ID_CW_SPINNER			53642
#define MOB_ID_CW_DRONE				53635
#define MOB_ID_CW_SPIDERLING		53631
#define MOB_ID_CW_BROODLING			53753
#define SPELL_COOL_SPAWN_ADD		75000
#define SPELL_ID_FRENZY_BETH		47774
#define SPELL_COOL_FRENZY_BETH		60000
#define SPELL_ID_WIDOW_KISS			99506
#define SPELL_COOL_WIDOW_KISS		60000

class Bethtilac : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Bethtilac);

   /* 
	Stage One: The Cinderweb
	Beth'tilac retreats to the top of her web at the beginning of the encounter.
		Fire Energy
		Beth'tilac slowly loses Fire Energy over time. Cinderweb Drones also siphon some of her energy when their own energy is depleted. Whenever Beth'tilac runs out of Fire Energy, she will set herself ablaze, causing Smoldering Devastation.
		Ember Flare – Intense heat burns enemies near Beth'tilac, inflicting 16650 to 19350 Fire damage to players at the same level as Beth'tilac.
		Meteor Burn – Meteors crash down onto the web, inflicting 37000 to 43000 Fire damage to players within 7 yards of the impact. The meteor also burns a hole in the web through which players may fall.
		Consume – Beth'tilac consumes Cinderweb Spiderlings, healing Beth'tilac for 10% of her maximum health.
		DeadlySmoldering Devastation – When Beth'tilac's Fire Energy fully depletes, she will set herself ablaze and inflict 380000 to 420000 Fire damage on all players located at her same level.
		ImportantCinderweb Spinner – Cinderweb Spinners dangle from the web above on a filament strand. Using Taunt or a similar ability on a Spinner will cause them to drop to the ground. Once killed, their web filament remains. Players may then interact with the filament and move to the top of the web.
			Burning Acid – The Cinderweb Spinner spits burning venom at a random player, inflicting 16965 to 19035 Fire damage.
			HeroicInterruptibleFiery Web Spin – The Cinderweb Spinner channels a fiery web onto a random player, stunning them for 25 sec. 
		Cinderweb Drone – These large spiders climb out of caves below the Cinderweb. When their Fire Energy fully depletes, the Cinderweb Drone climbs up to Beth'tilac and siphons Fire Energy from her.
			Consume – Cinderweb Drones consume a Cinderweb Spiderling, healing itself for 20% of its maximum life, increasing damage dealt by 20%, and increasing movement speed by 20%.
			Boiling Splatter – The Cinderwb Drone spits burning venom in a 500 yard long 60 degree forward cone, inflicting 58968 to 68531 Fire damage on any enemy within the area.
			Burning Acid – The Cinderwb Drone spits Burning Acid at a random player, inflicting 16965 to 19035 Fire damage.
			HeroicFixate – The Cinderweb Drone fixates on a random player, ignoring all other targets but decreasing its damage dealt by 75% for 10 sec. 
		Cinderweb Spiderling – These tiny spiders climb out of caves below the Cinderweb. They instinctively move towards Cinderweb Drones.
			Seeping Venom – The Cinderweb Spiderling leaps onto a random player within 5 yards, injecting them with venom and inflicting 6937 to 8062 Fire damage every 2 seconds for 10 sec. 
		HeroicCinderweb Broodling – These unstable spiders fixate on a random player and cast Volatile Burst when they reach their target.
			HeroicVolatile Burst – Cinderweb Broodlings explode when they reach a player, inflicting 45787 to 53212 Fire damage to all enemies within 8 yards. 
	Stage Two: The Frenzy!
	After she has performed Smoldering Devastation three times, Beth'tilac enters a frenzy. She lowers herself from the safety of the top of her Cinderweb and no longer calls for aid from her brood.
		Frenzy – Beth'tilac periodically casts Frenzy, increasing her damage dealt by 5% until the end of the encounter. This effect stacks.
		The Widow's Kiss – Beth'tilac's deadly kiss boils the blood of her current target, reducing the healing dealt to the target by 14 to 22% every 2 seconds for 20 sec. The kiss also causes the target to inflict increasing Fire damage to their surrounding allies within 10 yards.
		Ember Flare – Beth'tilac emits a blazing heat, inflicting 16650 to 19350 Fire damage to enemies on the same level as Beth'tilac.
		Consume – Beth'tilac consumes Cinderweb Spiderlings, healing Beth'tilac for 10% of her maximum health. 
	*/
	uint32 phase;
	uint32 EmberFlameTimer;
	uint32 MeteorBurnTimer;
	uint32 ConsumeTimer;
	uint32 SmolderingDevastationTimer;
	uint32 CinderwebSpinnerTimer;
	uint32 CinderwebDroneTimer;
	uint32 CinderwebSpiderlingTimer;
	uint32 CinderwebBroodlingTimer;
	uint32 FrenzyTimer;
	uint32 WidowKissTimer;
	float	DifficultyScale;
	Bethtilac(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		phase = 0;
	}
	void OnLoad()
	{
		//make 25 man mode harder then 10 man mode ?
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
		{
			uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
			_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now - 500 ) );	//attack a bit faster cause there are more healers
		}
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = 0.93f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = 0.86f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = 0.80f;
		RegisterAIUpdateEvent( 500 );
	}
    void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		uint32 TickNow = GetTickCount();
		//init phase
		if( phase == 0 )
		{
			EmberFlameTimer = TickNow + RandomUInt() % 25000;
			MeteorBurnTimer = TickNow + RandomUInt() % 25000;
			ConsumeTimer = TickNow + RandomUInt() % 25000;
			SmolderingDevastationTimer = TickNow + RandomUInt() % 25000;
			CinderwebSpinnerTimer = TickNow + RandomUInt() % 25000;
			CinderwebDroneTimer = TickNow + RandomUInt() % 25000;
			CinderwebBroodlingTimer = TickNow + RandomUInt() % 25000;
			phase = 1;
		}
		//does not depend on phase
		if( EmberFlameTimer < TickNow && _unit->isCasting() == false )
		{
			EmberFlameTimer = TickNow + (SPELL_COOL_EMBER_FLAME + RandomUInt() % SPELL_COOL_EMBER_FLAME)*DifficultyScale;
			_unit->CastSpell( tank, SPELL_ID_EMBER_FLAME, false );	
		}
		//put this before spawn event or it will create a visual bug client side
		if( ConsumeTimer < TickNow && _unit->isCasting() == false )
		{
			ConsumeTimer = TickNow + (SPELL_COOL_CONSUME + RandomUInt() % SPELL_COOL_CONSUME)*DifficultyScale;
			//try to find a nearby drone that we can consume
			Unit *selected_drone = NULL;
			InRangeSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( (*itr)->IsUnit() == false || SafeUnitCast( *itr )->isAlive() == false )
					continue;
				if( (*itr)->GetEntry() != MOB_ID_CW_SPIDERLING )
					continue;
				selected_drone = SafeUnitCast( *itr );
				break;
			}
			_unit->ReleaseInrangeLock();
			if( selected_drone && selected_drone->IsCreature() )
			{
				_unit->CastSpell( selected_drone, SPELL_ID_CONSUME, false );
				SafeCreatureCast( selected_drone )->SummonExpire();
				selected_drone = NULL;// morron protection
			}
		}
		if( CinderwebSpinnerTimer < TickNow && _unit->isCasting() == false )
		{
			CinderwebSpinnerTimer = TickNow + (SPELL_COOL_SPAWN_ADD + RandomUInt() % SPELL_COOL_SPAWN_ADD)*DifficultyScale;
			_unit->create_guardian( MOB_ID_CW_SPINNER, 5*60*1000, 0, 0, 0, 0, false );
		}
		if( CinderwebDroneTimer < TickNow && _unit->isCasting() == false )
		{
			CinderwebDroneTimer = TickNow + (SPELL_COOL_SPAWN_ADD + RandomUInt() % SPELL_COOL_SPAWN_ADD)*DifficultyScale;
			_unit->create_guardian( MOB_ID_CW_DRONE, 5*60*1000, 0, 0, 0, 0, false );
		}
		if( CinderwebSpiderlingTimer < TickNow && _unit->isCasting() == false )
		{
			CinderwebSpiderlingTimer = TickNow + (SPELL_COOL_SPAWN_ADD + RandomUInt() % SPELL_COOL_SPAWN_ADD)*DifficultyScale;
			_unit->create_guardian( MOB_ID_CW_SPIDERLING, 5*60*1000, 0, 0, 0, 0, false );
		}
		if( CinderwebBroodlingTimer < TickNow && _unit->isCasting() == false )
		{
			CinderwebBroodlingTimer = TickNow + (SPELL_COOL_SPAWN_ADD + RandomUInt() % SPELL_COOL_SPAWN_ADD)*DifficultyScale;
			if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
				_unit->create_guardian( MOB_ID_CW_BROODLING, 5*60*1000, 0, 0, 0, 0, false );
		}
		if( phase == 1 )
		{
			if( MeteorBurnTimer < TickNow && _unit->isCasting() == false )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_HEALER;
				sp.minrange = 0.0f;
				sp.maxrange = 60.0f;
				sp.procChance = 100;
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				MeteorBurnTimer = TickNow + (SPELL_COOL_METEOR_BURN + RandomUInt() % SPELL_COOL_METEOR_BURN)*DifficultyScale;
				_unit->CastSpell( t, SPELL_ID_METEOR_BURN , false );	//Concussive Stomp
			}
			if( SmolderingDevastationTimer < TickNow && _unit->isCasting() == false )
			{
				SmolderingDevastationTimer = TickNow + (SPELL_COOL_SMOLDERING_DEVASTATION + RandomUInt() % SPELL_COOL_SMOLDERING_DEVASTATION)*DifficultyScale;
				_unit->CastSpell( tank, SPELL_ID_SMOLDERING_DEVASTATION , false );	//Concussive Stomp
			}
		}
		if( phase == 2 )
		{
			if( FrenzyTimer < TickNow && _unit->isCasting() == false )
			{
				FrenzyTimer = TickNow + (SPELL_COOL_FRENZY_BETH + RandomUInt() % SPELL_COOL_FRENZY_BETH)*DifficultyScale;
				_unit->CastSpell( _unit, SPELL_ID_FRENZY_BETH , false );	//Concussive Stomp
			}
			if( WidowKissTimer < TickNow && _unit->isCasting() == false )
			{
				WidowKissTimer = TickNow + (SPELL_COOL_WIDOW_KISS + RandomUInt() % SPELL_COOL_WIDOW_KISS)*DifficultyScale;
				_unit->CastSpell( tank, SPELL_ID_WIDOW_KISS , false );	//Concussive Stomp
			}
		}
		if( phase == 1 && _unit->GetHealthPct() <= 50 )
		{
			FrenzyTimer = TickNow + RandomUInt() % 25000;
			WidowKissTimer = TickNow + RandomUInt() % 25000;
			phase = 2;
		}
	}
	void OnDied(Unit *mKiller)
	{
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		*EncounterStatus |= EXTENSION_FLAG_BETHLIAC_DIE;
	}
};

class CristalWebSpinner : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(CristalWebSpinner);

    CristalWebSpinner(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 15000 );
    }
	void OnLoad()
	{
		AI_Spell sp;
		sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
		sp.minrange = 10.0f;
		sp.maxrange = 60.0f;
		sp.procChance = 20;
		Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
		if( t )
			_unit->GetAIInterface()->AttackReaction( t, 10, 0 );
	}
	void AIUpdate()
	{
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		if( RandChance( 50 ) )
			_unit->CastSpell( tank, 98471 , false );	//Burning Acid
		else if( RandChance( 25 ) )
			_unit->CastSpell( tank, 97202 , false );	//Fiery Web Spin
	}
};

class CristalWebDrone : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(CristalWebDrone);

    CristalWebDrone(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 25000 );
    }
	void OnLoad()
	{
		AI_Spell sp;
		sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
		sp.minrange = 10.0f;
		sp.maxrange = 60.0f;
		sp.procChance = 20;
		Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
		if( t )
			_unit->GetAIInterface()->AttackReaction( t, 10, 0 );
	}
	void AIUpdate()
	{
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		_unit->CastSpell( tank, 98471 , false );	//Burning Acid
	}
};

class CristalWebSpiderling : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(CristalWebSpiderling);

    CristalWebSpiderling(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 7000 );
    }
	void OnLoad()
	{
		AI_Spell sp;
		sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
		sp.minrange = 10.0f;
		sp.maxrange = 60.0f;
		sp.procChance = 20;
		Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
		if( t )
			_unit->GetAIInterface()->AttackReaction( t, 10, 0 );
	}
	void AIUpdate()
	{
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		_unit->CastSpell( tank, 97079 , false );	//seeping venom
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_BLAZING_CLAW		101729
#define SPELL_COOL_BLAZING_CLAW		10000
#define MOB_ID_BLAZING_TALON		53896
#define SPELL_COOL_SPAWN_ADD2		45000
#define SPELL_ID_BLAZING_BUFFET		99756
#define SPELL_ID_FIERY_VORTEX		99793
#define SPELL_ID_HARSH_WINDS		100640
#define SPELL_COOL_HARSH_WINDS		25000
#define SPELL_ID_FIERY_TORNADO		99817
//#define SPELL_COOL_FIERY_TORNADO	27000
#define SPELL_ID_FIRESTORM			100744
#define SPELL_COOL_FIRESTORM		29000
#define SPELL_ID_VOCANIC_FIRE		98462
#define SPELL_ID_WINGS_OF_FLAME		98619

class Alysrazor : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Alysrazor);

   /* 
		Firestorm – At the beginning of the battle Alysrazor ascends into the sky, inflicting 30000 Fire damage to players and knocking them back. Alysrazor continues to inflict 10000 Fire damage to players every 1 sec. for 10 sec.
		Volcanic Fire – A massive eruption creates patches of Fire which block escape from Alysrazor's domain. Volcanic Fire patches inflict 92500 to 107500 Fire damage to players within 6 yards every 1 sec. 

	Stage One: I Will Burn You From the Sky!

	Alysrazor flies around the arena, allowing her minions to corner her foes far below. She periodically flies through the center of the arena to claw at foes.

		Blazing Claw – Alysrazor claws her way through the center of the arena, inflicting 92500 to 107500 Physical damage to enemies in a 0 yard long 90 degree cone every 1.5 seconds. Each swipe also increases the Fire and Physical damage dealt to the target by 10% for 15 sec.
		HeroicFirestorm – Alysrazor faces the center of the arena and kicks up a powerful, fiery wind. After 5 seconds the arena is bathed in flames, inflicting 100000 Fire damage every 1 seconds to all players within line of sight for 5 sec.
		Molting – Alysrazor molts, creating Molten Feathers nearby.
			ImportantDPSMolten Feather – Players can pick up to three Molten Feathers. While holding a Molten Feather, all spells can be cast while moving and movement speed increases by 30% per feather. Once a player obtains three Molten Feathers, they gain Wings of Flame.
				Wings of Flame – The Wings of Flame allow the player to fly for 20 sec. 
		Flying

		Players with Wings of Flame contend with additional elements of the battle.
			Blazing Power – Alysrazor periodically creates rings of fire which last for 3 seconds. Players passing through the ring gain Blazing Power, increasing haste by 8%. This effect stacks up to $99461U times. Each stack of Blazing Power restores mana, rage, energy, runic power, and holy power, and refreshes the duration of Wings of Flame.
			Alysra's Razor – If a player gains 25 stacks of Blazing Power, they gain Alysra's Razor. This increases the player's critical strike chance by 75% for 40 sec.
			Incendiary Cloud – Alysrazor periodically creates an increasing number of Incendiary Clouds lasting for 3 seconds. Players that pass through the cloud suffer 46250 to 53750 Fire damage every 1 sec.

			In Heroic Difficulty, Alysrazor always creates three Incindiary Clouds.
		Blazing Talon Initiate – Blazing Talon Initiates periodically fly in to assist Alysrazor in defeating enemy forces on the ground.
			Brushfire – The Blazing Talon Initiate conjures a fiery ball that moves across the arena, inflicting 37000 to 43000 damage every 1 sec to players within 3.5 yards.
			HeroicBlazing Shield – The Blazing Talon Initiate calls upon a familiar flame for protection from Alysrazor's Firestorm.
			InterruptibleMagicFieroblast – The Blazing Talon Initiate hurls a fiery boulder at an enemy, inflicting 37000 to 43000 Fire damage and 10000 Fire damage every 3 seconds for 12 sec.
			Fire it Up! – When a Blazing Talon Initiate casts Fieroblast, they gain a stack of Fire it Up! This effect increases the Initiate's damage dealt by 10% and casting speed by 10%. 
		TankVoracious Hatchling – Early in Stage 1, two Blazing Broodmothers drop off two Molten Eggs. After several seconds the eggs hatch into Voracious Hatchlings. Voracious Hatchlings throw a Tantrum if not fed Plump Lava Worms.
			Imprinted – Upon hatching, Voracious Hatchlings imprint on the nearest player. The hatchling only attacks that player, but they gain 1000% additional damage against the hatchling.
			Satiated – The Voracious Hatchling will not throw a Tantrum when Satiated, which lasts for 15 sec. Voracious Hatchlings hatch Satiated, and can become Satiated again if a player feeds them Plump Lava Worms.
			Hungry – A Voracious Hatchling that is no longer Satiated becomes Hungry. When Hungry, hatchlings have a 20% chance on hit to throw a Tantrum.
			Tantrum – The Voracious Hatchling throws a Tantrum, increasing damage dealt by 50% and haste by 50%.
			HealerGushing Wound – The Voracious Hatchling strikes all targets within a 10 yard 60 degree cone, causing them to bleed for 3000 Physical damage every 0.200000003 seconds for 1 min. or until the target's health falls below 50% of their maximum health. 
		TankPlump Lava Worm – During Stage 1, two sets of four Plump Lava Worms erupt from the molten ground. Players cannot attack Plump Lava Worms. Voracious Hatchlings near a Plump Lava Worm rush to devour it, becoming Satiated.
			Lava Spew – Plump Lava Worms spew a molten cone of fire, dealing 37000 to 43000 damage every 1 sec to all enemies within a 0 yard 35 degree cone. 
		HeroicHerald of the Burning End – During Stage 1, a Herald of the Burning End periodically appears and begins casting Cataclysm. The Herald is immune to all damage, but dies when he casts Cataclysm.
			HeroicCataclysm – The Herald of the Burning End summons a powerful Molten Meteor, inflicting 462500 to 537500 Flamestrike damage to enemies within 5 yards.
				HeroicMolten Meteor – Molten Meteors roll in one of 8 random directions, dealing 462500 to 537500 Flamestrike damage to enemies within 5 yards every 1 sec. If the meteor reaches a wall, it will break apart into three Molten Boulders, which ricochet back in the opposite direction. If destroyed before reaching a wall, the Molten Meteor becomes temporarily stationary and blocks line of sight.
				HeroicMolten Boulder – Three Molten Boulders form when a Molten Meteor hits a wall and breaks apart. Molten Boulders knock back and inflict 29600 to 34400 Flamestrike damage to players within 2 yards every 1 sec. 


	Stage Two: The Skies are Mine!

	Alysrazor flies in a tight circle, removes Wings of Flame from all players after 5 seconds, and then begins her ultimate attack.

		DeadlyFiery Vortex – A Fiery Vortex appears in the middle of the arena, inflicting 50000 Fire damage every 0.5 seconds to players within 15 yards.
		DeadlyHarsh Winds – Alysrazor's powerful wingstrokes cause harsh winds to scald the landscape around her nest, inflicting 50000 Fire damage every 1 seconds for 5 sec to players greater than 60 yards from the Fiery Vortex. This effect stacks.
		ImportantFiery Tornado – Fiery Tornadoes erupt from the Fiery Vortex and begin moving rapidly around Alysrazor's arena, inflicting 40000 Fire damage every 1 sec to enemies within 10 yards.
		Blazing Power – Alysrazor continues to create rings of fire that appear on the ground of the arena and last for 3 seconds. Players passing through the ring gain Blazing Power, increasing their haste by 8%. This effect stacks up to $99461U times. Each stack of Blazing Power also restores mana, rage, energy, runic power, and holy power. 


	Stage Three: Burnout!

	Alysrazor crashes to the ground with 0 Molten Power and becomes vulnerable. This stage lasts until Alyrazor's energy bar reaches 50 Molten Power.

		ImportantDPSBurnout – Alysrazor's fire burns out, immobilizing her and increasing her damage taken by 50%. When struck with a harmful spell Alysrazor emits Essence of the Green.
			ImportantHealerEssence of the Green – If Alysrazor is struck by a harmful spell she emits Essence of the Green, restoring 10% of the caster's maximum mana. 
		Spark – A bright spark burns within the heart of Alysrazor, restoring 3 Molten Power every 2 seconds.
		TankBlazing Talon Clawshaper – At the start of stage 2, two Blazing Talon Clawshapers fly in and re-energize Alysrazor.
			InterruptibleIgnition – Blazing Talon Clawshapers channel molten energy into Alysrazor, restoring 1 Molten Power every 1 sec. 


	Stage Four: Reborn in Flame!

	Alysrazor's fire reignites at 50 Molten Power. This stage lasts until Alysrazor reaches 100 Molten Power.

		Ignited – Alysrazor's fiery core combusts once again, rapidly restoring her Molten Power. The core restores 2 Molten Power every 1 seconds.
		Blazing Buffet – Alysrazor's fiery core emits powerful bursts of flame, inflicting 9250 to 10750 Fire damage to all players every 1 seconds while Alysrazor remains Ignited.
		TankBlazing Claw – Alysrazor claws at her current target, inflicing 92500 to 107500 Physical damage to enemies in a 0 yard long 90 degree cone every 1.5 seconds. Each swipe also increases the Fire and Physical damage dealt to the target by 10% for 15 sec.
		HealerFull Power – When Alysrazor reaches 100 Molten Power, she is at Full Power. This deals 50000 Fire damage to all players and knocks them back. Alysrazor then begins her Stage 1 activities again. 
	*/
	uint32 phase;
	uint32 BlazingClawTimer;
	uint32 BlazingTalonTimer;
	uint32 HarshWindsTimer;
//	uint32 FieryTornadoTimer;
	uint32 FireStormTimer;
	uint32 VolcanicFireRemoveTimer;
	float	DifficultyScale;
	Alysrazor(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		phase = 0;
	}
	void OnLoad()
	{
		//make 25 man mode harder then 10 man mode ?
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
		{
			uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
			_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now - 500 ) );	//attack a bit faster cause there are more healers
		}
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = 0.93f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = 0.86f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = 0.80f;
		RegisterAIUpdateEvent( 500 );
	}
	void OnDied(Unit *mKiller)
	{
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( ( *itr )->isAlive() == false )
				continue;
			( *itr )->CastSpell( ( *itr ), SPELL_ID_WINGS_OF_FLAME, true );
		}
		_unit->ReleaseInrangeLock();
		RemoveAIUpdateEvent();
	}
	void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		uint32 TickNow = GetTickCount();
		//init phase
		if( phase == 0 )
		{
			BlazingClawTimer = TickNow + RandomUInt() % 25000;
			FireStormTimer = TickNow + RandomUInt() % 25000;
			BlazingTalonTimer = TickNow + RandomUInt() % 25000;
	
			phase = 1;
			_unit->CastSpell( _unit, SPELL_ID_FIRESTORM, true );
			_unit->CastSpell( _unit, SPELL_ID_VOCANIC_FIRE, true );
			VolcanicFireRemoveTimer = TickNow + 30000;
		}
		if( VolcanicFireRemoveTimer < TickNow )
		{
			_unit->RemoveAura( SPELL_ID_VOCANIC_FIRE );
			VolcanicFireRemoveTimer = 0x7FFFFFFF;
		}
		if( phase == 1 )
		{
			if( BlazingClawTimer < TickNow && _unit->isCasting() == false )
			{
				BlazingClawTimer = TickNow + (SPELL_COOL_BLAZING_CLAW + RandomUInt() % SPELL_COOL_BLAZING_CLAW)*DifficultyScale;
				_unit->CastSpell( tank, SPELL_ID_BLAZING_CLAW, false );	
			}
			if( FireStormTimer < TickNow && _unit->isCasting() == false )
			{
				FireStormTimer = TickNow + (SPELL_COOL_FIRESTORM + RandomUInt() % SPELL_COOL_FIRESTORM)*DifficultyScale;
				_unit->CastSpell( tank, SPELL_ID_FIRESTORM, false );	
			}
			if( BlazingTalonTimer < TickNow && _unit->isCasting() == false )
			{
				BlazingTalonTimer = TickNow + (SPELL_COOL_SPAWN_ADD2 + RandomUInt() % SPELL_COOL_SPAWN_ADD2)*DifficultyScale;
				_unit->create_guardian( MOB_ID_BLAZING_TALON, 5*60*1000, 0, 0, 0, 0, false );
			}
		}
		if( phase == 2 )
		{
//			if( FieryTornadoTimer < TickNow && _unit->isCasting() == false )
			{
//				FieryTornadoTimer = TickNow + (SPELL_COOL_FIERY_TORNADO + RandomUInt() % SPELL_COOL_FIERY_TORNADO)*DifficultyScale;
//				_unit->CastSpell( _unit, SPELL_ID_FIERY_TORNADO, false );	
			}
			if( HarshWindsTimer < TickNow && _unit->isCasting() == false )
			{
				HarshWindsTimer = TickNow + (SPELL_COOL_HARSH_WINDS + RandomUInt() % SPELL_COOL_HARSH_WINDS)*DifficultyScale;
				_unit->CastSpell( tank, SPELL_ID_HARSH_WINDS, false );	
			}
		}
		if( phase == 4 )
		{
			if( BlazingClawTimer < TickNow && _unit->isCasting() == false )
			{
				BlazingClawTimer = TickNow + (SPELL_COOL_BLAZING_CLAW + RandomUInt() % SPELL_COOL_BLAZING_CLAW)*DifficultyScale;
				_unit->CastSpell( tank, SPELL_ID_BLAZING_CLAW, false );	
			}
		}

		if( phase == 1 && _unit->GetHealthPct() < 75 )
		{
			HarshWindsTimer = TickNow + RandomUInt() % 25000;
//			FieryTornadoTimer = TickNow + RandomUInt() % 25000;
			_unit->CastSpell( _unit, SPELL_ID_FIERY_VORTEX, true );
			_unit->CastSpell( _unit, SPELL_ID_FIERY_TORNADO, false );	
			phase = 2;
		}
		if( phase == 2 && _unit->GetHealthPct() < 50 )
		{
			Unit *selected_drone = NULL;
			InRangeSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( (*itr)->IsUnit() == false || SafeUnitCast( *itr )->isAlive() == false )
					continue;
				if( (*itr)->GetEntry() != 99794 )	//fiery vortex
					continue;
				selected_drone = SafeUnitCast( *itr );
				break;
			}
			_unit->ReleaseInrangeLock();
			if( selected_drone && selected_drone->IsCreature() )
			{
				SafeCreatureCast( selected_drone )->SummonExpire();
				selected_drone = NULL;// morron protection
			}
			phase = 3;
		}
		if( phase == 3 && _unit->GetHealthPct() < 25 )
		{
			_unit->CastSpell( _unit, SPELL_ID_BLAZING_BUFFET, true );
			phase = 4;
		}
	}
};

class BlazingTalon : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(BlazingTalon);

    BlazingTalon(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 7000 );
    }
	void AIUpdate()
	{
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		_unit->CastSpell( tank, 100094 , false );	//Fieroblast
		_unit->CastSpell( _unit, 100093 , false );	//Fire It Up!
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SCRIPT_DECL FirelandsTeleporter : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object*  pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

		uint32 boss_kills = 0;
/*		uint32 boss_alive = 0;
		for(uint32 i=0;i<Plr->GetMapMgr()->m_CreatureArraySize;i++)
		{
			Creature *tc = Plr->GetMapMgr()->m_CreatureStorage[i];
			if(	tc )
			{
				uint32 entry = tc->GetEntry();
				if( entry == 53691 || entry == 53494 || entry == 52498 )
				{
					if( tc->isAlive() == false )
						boss_kills++;
					else
						boss_alive++;
				}
			}
		} */
		int64 *EncounterStatus = pObject->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		if( *EncounterStatus & EXTENSION_FLAG_BETHLIAC_DIE )
			boss_kills++;
		if( *EncounterStatus & EXTENSION_FLAG_BALEROC_DIED )
			boss_kills++;
		if( *EncounterStatus & EXTENSION_FLAG_SHANNOX_DIED )
			boss_kills++;

		if( boss_kills >= 3 )
		{
			Menu->AddItem(2, "Teleport to the other side", 3);
		}
		else
		{
			Menu->AddItem(0, "You need to kill more bosses", 1);
			Menu->AddItem(1, "Come back later", 2);
		}
	    
		if(AutoSend)
			Menu->SendTo(Plr);
	};
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char *Code)
	{
		if(IntId == 3 )
			Plr->SafeTeleport(720, Plr->GetInstanceID(), 364, -93, 79, 1.35 ); 
	};
	void GossipEnd(Object* pObject, Player* pPlayer)
	{
		GossipScript::GossipEnd(pObject, pPlayer);
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_SULFURAS_SMASH			100890
#define SPELL_COOL_SULFURAS_SMASH		35000
#define SPELL_ID_WRATH_OF_RAGNAROS		98263
#define SPELL_COOL_WRATH_OF_RAGNAROS	20000
#define SPELL_ID_HAND_OF_RAGNAROS		98237
#define SPELL_COOL_HAND_OF_RAGNAROS		25000
#define SPELL_ID_MAGMA_TRAP				98164
#define SPELL_COOL_MAGMA_TRAP			75000
#define SPELL_ID_MAGMA_BLAST			98313
#define SPELL_COOL_MAGMA_BLAST			30000
#define SPELL_ID_LAVA_BOLT				98981
#define SPELL_COOL_LAVA_BOLT			30000
#define MOB_ID_ENGULFING_FLAMES			53485
#define SPELL_COOL_SPAWN_ADD3			120000
#define SPELL_ID_MOLTEN_SEED			98498
#define SPELL_COOL_MOLTEN_SEED			31000
#define SPELL_ID_MOLTEN_INFERNO			98518
#define SPELL_COOL_MOLTEN_INFERNO		27000
#define MOB_ID_MOLTEN_ELEMENTAL			53189
#define SPELL_COOL_SPAWN_ADD4			20000
#define MOB_ID_LAVA_SCION				53231

class Ragnaros2 : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Ragnaros2);

   /* 
Stage One: By Fire Be Purged!

    DeadlySulfuras Smash – Ragnaros faces the location of a random player and prepares to smash Sulfuras on the platform. The impact of Sulfuras inflicts 536250 to 563750 Fire damage to any player within 5 yards of the impact, and creates several Lava Waves which move out from the point of impact.
        MagicLava Wave – If a player touches a moving Lava Wave, the lava inflicts 97500 to 102500 Fire damage, 29250 to 30750 Fire damage every 1 sec. for 5 sec., and knocks the player back. 
    Wrath of Ragnaros – Ragnaros blasts the location of a random player, inflicting 58500 to 61500 Fire damage to all players within 6 yards and knocking them back.

    In 25 player raids, Ragnaros blasts three locations.
    Hand of Ragnaros – Ragnaros inflicts 29250 to 30750 Fire damage to all enemies within 55 yards, knocking them back.
    Magma Trap

    Ragnaros launches a Magma Trap at a random player's location. When the Magma Trap reaches the platform, it inflicts 58500 to 61500 Fire damage and knocks back players within 8 yards. The active Magma Trap persists for the duration of the encounter and triggers when stepped on, causing a Magma Trap Eruption.
        Magma Trap Eruption – A triggered Magma Trap erupts for 78000 to 82000 Fire damage to all players, and violently knocks the triggering player into the air.

        The Magma Trap causes the triggering player to take 50% more damage from other Magma Traps. This effect lasts for 45 sec and stacks.
    Magma Blast – Ragnaros blasts magma at his current target if he cannot reach and melee them, inflicting 73125 to 76875 Fire damage and increasing Fire damage taken by 50% for 6 sec. 


Intermission: Minions of Fire!

At 70% health Ragnaros casts Splitting Blow, burying Sulfuras into the platform and creating Sons of Flame across the platform. Ragnaros submerges for 45 seconds or until the players destroy all the Sons of Flame, whichever comes first.

    Splitting Blow – Ragnaros buries Sulfuras within the platform, inflicting 68250 to 71750 Fire damage every 1 second to players within 6 yards and creating eight Sons of Flame that attempt to reach the mighty hammer.
    Son of Flame – Sons of Flame cross the platform and attempt to reform with Sulfuras. A Son of Flame that reaches Sulfuras causes a Supernova.
        Burning Speed – For every 5% of their remaining health above 50%, Sons of Flame move 75% faster. When players reduce a Son of Flame to below 50% remaining health, the Son of Flame loses the Burning Speed effect.
        DeadlySupernova – When a Son of Flame reaches Sulfuras it explodes in a Supernova, inflicting 121875 to 128125 Fire damage to all players. 
    Lava Bolt – While Ragnaros lies submerged underneath the lava, bolts of hot magma fall on four random players every 4 seconds. A Lava Bolt inflicts 43875 to 46125 Fire Damage.

    In 25 player raids, bolts of hot magma fall on ten players.


Stage Two: Sulfuras Will Be Your End!

    DeadlySulfuras Smash – Ragnaros faces the location of a random player and prepares to smash Sulfuras on the platform. The impact of Sulfuras inflicts 536250 to 563750 Fire damage to any player within 5 yards of the impact, and creates several Lava Waves which move out from the point of impact.
        MagicLava Wave – If a player touches a moving Lava Wave, the lava inflicts 97500 to 102500 Fire damage, 29250 to 30750 Fire damage every 1 sec. for 5 sec., and knocks the player back. 
    Engulfing Flame – Ragnaros periodically engulfs a random third of the platform in flames, immediately inflicting 68250 to 71750 Fire damage to players caught in the conflagration then again inflicting 68250 to 71750 Fire damage one second later.
    HeroicWorld in Flames – Ragnaros periodically casts Engulfing Flames on a random section of the platform every 3 sec for 9 sec.
    Molten Seed – Ragnaros creates Molten Seed at the location of 10 random players from the nearby lava, inflicting 53625 to 56375 Fire damage to all players within 6 yards of a seed. After 10 sec the Molten Seed bursts into a Molten Inferno.

    In 25 player raids, Ragnaros targets 20 players.
        Molten Inferno – When Molten Seeds burst they create a Molten Inferno, inflicting 135000 Fire damage to all players and creating a Molten Elemental. The damage inflicted decreases the farther away the player stands from the seed.
        Molten Elemental – A Molten Inferno creates a Molten Elemental, which attacks and fixates on a random player.
            HeroicMolten Power – Molten Elementals within 6 yards empower other Molten Elementals, increasing their damage dealt by 25% and granting immunity to snare and stun effects. 
    Magma Blast – Ragnaros blasts magma at his current target if he cannot reach and melee them, inflicting 73125 to 76875 Fire damage and increasing Fire damage taken by 50% for 6 sec. 


Intermission: Denizens of Flame!

At 40% health Ragnaros casts Splitting Blow, burying Sulfuras into the platform and creating Sons of Flame across the platform. Ragnaros submerges for 45 seconds or until the players destroy all the Sons of Flame, whichever comes first.Ragnaros also creates two Lava Scions that attack the players.

    Splitting Blow – Ragnaros buries Sulfuras within the platform, inflicting 68250 to 71750 Fire damage every 1 second to players within 6 yards and creating eight Sons of Flame that attempt to reach the mighty hammer.
    Son of Flame – Sons of Flame cross the platform and attempt to reform with Sulfuras. A Son of Flame that reaches Sulfuras causes a Supernova.
        Burning Speed – For every 5% of their remaining health above 50%, Sons of Flame move 75% faster. When players reduce a Son of Flame to below 50% remaining health, the Son of Flame loses the Burning Speed effect.
        Supernova – When a Son of Flame reaches Sulfuras it explodes in a Supernova, inflicting 121875 to 128125 Fire damage to all players. 
    Lava Scion – One Lava Scion forms on each side of the platform.
        Blazing Heat – A Lava Scion inflicts a random player with Blazing Heat, causing the player to create a trail of Blazing Heat in their wake. Blazing Heat inflicts 48750 to 51250 Fire damage every 1 sec on players standing within the trail, and heals Sons of Flame and Lava Scions within the trail for 10% every 1 sec. 


Stage Three: Begone From My Realm!

    DeadlySulfuras Smash – Ragnaros faces the location of a random player and prepares to smash Sulfuras on the platform. The impact of Sulfuras inflicts 536250 to 563750 Fire damage to any player within 5 yards of the impact, and creates several Lava Waves which move out from the point of impact.
        MagicLava Wave – If a player touches a moving Lava Wave, the lava inflicts 97500 to 102500 Fire damage, 29250 to 30750 Fire damage every 1 sec. for 5 sec., and knocks the player back. 
    Engulfing Flame – Ragnaros periodically engulfs a random third of the platform in flames, immediately inflicting 68250 to 71750 Fire damage to players caught in the conflagration then again inflicting 68250 to 71750 Fire damage one second later.
    HeroicWorld in Flames – Ragnaros periodically casts Engulfing Flames on a random section of the platform every 3 sec for 9 sec.
    Summon Living Meteor – Ragnaros calls down an increasing number of Living Meteors. The Living Meteor's impact inflicts 63375 to 66625 Fire damage to players within 5 yards of the location.
        Living Meteor – The Living Meteor fixates on a random player and chases them.
            DeadlyMeteor Impact – Any player within 4 yards of a Living Meteor triggers a Meteor Impact, inflicting 487500 to 512500 Fire damage to players within 8 yards.
            Combustible – Attacking a Living Meteor causes Combustion, knocking it back several yards from the attacking player. Triggering Combustion removes the Combustible effect for 5 seconds.
                Combustion – Attacking a Living Meteor causes Combustion, knocking it back several yards from the attacking player.

                In Heroic Difficulty Combustion inflicts 2000 Fire damage every second for until cancelled on the triggering player. This effect stacks.
            Lavalogged – If a Living Meteor touches a Lava Wave, the meteor gains the Lavalogged effect for 1 min. 
    Magma Blast – Ragnaros blasts magma at his current target if he cannot reach and melee them, inflicting 73125 to 76875 Fire damage and increasing Fire damage taken by 50% for 6 sec. 


HeroicStage Four: The True Power of the Firelord!

The Firelord unleashes his full power and is able to move freely around the platform. Players have the aid of powerful heroes of Azeroth to support them.

    HeroicSuperheated – Ragnaros unleashes his full power and becomes Superheated, inflicting 7000 Fire damage every 1 on all players and increasing the damage dealt by Superheated by 10%. This effect stacks.
    HeroicEmpower Sulfuras – Ragnaros channels his flames upon Sulfuras. After 5 sec. of channeling, Sulfuras becomes Empowered and Ragnaros' melee attacks unleash the Flames of Sulfuras, inflicting 487500 to 512500 Fire damage to all players.
        HeroicDeadlyFlames of Sulfuras – With Sulfuras empowered, Ragnaros' melee attacks unleash the Flames of Sulfuras, inflicting 487500 to 512500 Fire damage to all players. 
    HeroicDreadflame – Ragnaros stomps and creates a Dreadflame at two nearby locations. The Dreadflame multiplies rapidly and spreads across the platform. Dreadflame inflicts 58500 to 61500 Fire damage and 4000 Fire damage every 1 sec for 30 sec if a player touches the fire.

    In 25 player raids, Ragnaros creates six Dreadflames.
    HeroicMagma Geyser – Ragnaros will target a Magma Geyser whenever he notices four players in a cluster together. The Magma Geyser inflicts 53625 to 56375 Fire damage every 1 sec., knocks players back, and destroys any Breadth of Frost within 5 yards.

    In 25 player raids, Ragnaros uses Magma Geyser on clusters of 10 players.
    HeroicCenarius – Although the son of Malorne and Elune, the mighty demigod and druid patron Cenarius was cared for in his youth by the Dragon Aspect Ysera. Cenarius supports the raid by using his powers over nature to freeze Living Meteors and reduce the damage caused when Ragnaros is Superheated.
        HeroicBreadth of Frost – Cenarius forms a Breadth of Frost at a nearby random location. Any Living Meteors that enter the Breadth of Frost freeze and take 50% additional damage from players. Breadth of Frost grants immunity to Superheated damage and removes the Superheated debuff from players standing within the area. 
    HeroicArch Druid Hamuul Runetotem – Apart from being revered by his fellow tauren, the greathearted archdruid Hamuul Runetotem is also a celebrated leader of the Cenarion Circle. The wise tauren assists the raid by harnessing nature's fury and causes roots to erupt from the ground to entrap Ragnaros.
        HeroicEntrapping Roots – Arch Druid Hamuul Runetotem forms Entrapping Roots at a nearby random location. If Ragnaros enters the area of the Entrapping Roots, he becomes stunned for 10 sec and takes 50% additional damage while stunned. 
    HeroicMalfurion Stormrage – One of the most powerful druids ever, Archdruid Malfurion Stormrage has been instrumental in protecting Mount Hyjal from Ragnaros's fiery elemental forces. The legendary archdruid helps the raid by protecting heroes from the all-consuming Dreadflame attacks.
        HeroicCloudburst – Malfurion forms a Cloudburst. The player who interacts with a Cloudburst become surrounded with a Deluge. Deluge grants the player immunity to Dreadflame damage, and allows the player to extinguish any nearby Dreadflame.

        In 25 player raids, up to three players may use a single Cloudburst.
            HeroicDeluge – The Deluge grants immunity to Dreadflame damage, and allows the player to extinguish any nearby Dreadflame. 
	*/
	uint32 phase;
	uint32 SulfurasSmashTimer;
	uint32 WrathOfRagnarosTimer;
	uint32 HandOfRagnarosTimer;
	uint32 MagmaTrapTimer;
	uint32 MagmaBlastTimer;
	uint32 LavaBoltTimer;
	uint32 EngulfingFlamesTimer;
	uint32 MoltenInfernoTimer;
	uint32 MoltenElementalTimer;
	uint32 MoltenSeedTimer;
	uint32 LavaScionTimer;
	float	DifficultyScale;
	Ragnaros2(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		phase = 0;
	}
	void OnLoad()
	{
		//make 25 man mode harder then 10 man mode ?
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
		{
			uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
			_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now - 500 ) );	//attack a bit faster cause there are more healers
		}
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = 0.93f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = 0.86f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = 0.80f;
		RegisterAIUpdateEvent( 1000 );
//		_unit->GetAIInterface()->m_canMove = false;
	}
	void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		uint32 TickNow = GetTickCount();
		if( MagmaBlastTimer < TickNow && tank->GetDistance2dSq( _unit ) >= 15.0f*15.0f )
		{
			MagmaBlastTimer = TickNow + 3000;
			_unit->CastSpell( tank, SPELL_ID_MAGMA_BLAST, true );
		}

		//init phase
		if( phase == 0 )
		{
			SulfurasSmashTimer = TickNow + RandomUInt() % 25000;
			WrathOfRagnarosTimer = TickNow + RandomUInt() % 25000;
			HandOfRagnarosTimer = TickNow + RandomUInt() % 25000;
			MagmaTrapTimer = TickNow + RandomUInt() % 25000;
			MagmaBlastTimer = TickNow + RandomUInt() % 25000;

			LavaBoltTimer = TickNow + RandomUInt() % 25000;

			EngulfingFlamesTimer = TickNow + RandomUInt() % 25000;
			MoltenInfernoTimer = TickNow + RandomUInt() % 25000;
			MoltenElementalTimer = TickNow + RandomUInt() % 25000;
			MoltenSeedTimer = TickNow + RandomUInt() % 25000;
			LavaScionTimer = TickNow + RandomUInt() % 25000;
			phase = 1;
		}
		if( phase == 1 )
		{
			if( SulfurasSmashTimer < TickNow && _unit->isCasting() == false )
			{
				SulfurasSmashTimer = TickNow + (SPELL_COOL_SULFURAS_SMASH + RandomUInt() % SPELL_COOL_SULFURAS_SMASH)*DifficultyScale;
 				_unit->CastSpell( tank, SPELL_ID_SULFURAS_SMASH, false );	
			}
			if( WrathOfRagnarosTimer < TickNow && _unit->isCasting() == false )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
				sp.minrange = 2.0f;
				sp.maxrange = 45.0f;
				sp.procChance = 100;
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				WrathOfRagnarosTimer = TickNow + (SPELL_COOL_WRATH_OF_RAGNAROS + RandomUInt() % SPELL_COOL_WRATH_OF_RAGNAROS)*DifficultyScale;
				_unit->CastSpell( t, SPELL_ID_WRATH_OF_RAGNAROS, false );	
			}
			if( HandOfRagnarosTimer < TickNow && _unit->isCasting() == false )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
				sp.minrange = 2.0f;
				sp.maxrange = 45.0f;
				sp.procChance = 100;
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				HandOfRagnarosTimer = TickNow + (SPELL_COOL_HAND_OF_RAGNAROS + RandomUInt() % SPELL_COOL_HAND_OF_RAGNAROS)*DifficultyScale;
				_unit->CastSpell( t, SPELL_ID_HAND_OF_RAGNAROS, false );	
			}
			if( MagmaTrapTimer < TickNow && _unit->isCasting() == false )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
				sp.minrange = 2.0f;
				sp.maxrange = 45.0f;
				sp.procChance = 100;
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				MagmaTrapTimer = TickNow + (SPELL_COOL_MAGMA_TRAP + RandomUInt() % SPELL_COOL_MAGMA_TRAP)*DifficultyScale;
				_unit->CastSpell( t, SPELL_ID_MAGMA_TRAP, false );	
			}
		}
		if( phase == 2 )
		{
			if( LavaBoltTimer < TickNow && _unit->isCasting() == false )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
				sp.minrange = 2.0f;
				sp.maxrange = 45.0f;
				sp.procChance = 100;
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				LavaBoltTimer = TickNow + (SPELL_COOL_LAVA_BOLT + RandomUInt() % SPELL_COOL_LAVA_BOLT)*DifficultyScale;
				_unit->CastSpell( t, SPELL_ID_LAVA_BOLT, false );	
			}
		}
		if( phase == 3 )
		{
			if( SulfurasSmashTimer < TickNow && _unit->isCasting() == false )
			{
				SulfurasSmashTimer = TickNow + (SPELL_COOL_SULFURAS_SMASH + RandomUInt() % SPELL_COOL_SULFURAS_SMASH)*DifficultyScale;
				_unit->CastSpell( tank, SPELL_ID_SULFURAS_SMASH, false );	
			}
			if( EngulfingFlamesTimer < TickNow && _unit->isCasting() == false )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
				sp.minrange = 2.0f;
				sp.maxrange = 45.0f;
				sp.procChance = 100;
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				EngulfingFlamesTimer = TickNow + (SPELL_COOL_SPAWN_ADD2 + RandomUInt() % SPELL_COOL_SPAWN_ADD2)*DifficultyScale;
				if( t )
				{
					LocationVector v = t->GetPosition();
					_unit->create_guardian( MOB_ID_ENGULFING_FLAMES, 0x0FFFFFFF, 0, NULL, &v, 0, false );
				}
			}
			if( MoltenSeedTimer < TickNow && _unit->isCasting() == false )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
				sp.minrange = 2.0f;
				sp.maxrange = 45.0f;
				sp.procChance = 100;
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				MoltenSeedTimer = TickNow + (SPELL_COOL_MOLTEN_SEED + RandomUInt() % SPELL_COOL_MOLTEN_SEED)*DifficultyScale;
				_unit->CastSpell( t, SPELL_ID_MOLTEN_SEED, false );	
			}
			if( MoltenInfernoTimer < TickNow && _unit->isCasting() == false )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
				sp.minrange = 2.0f;
				sp.maxrange = 45.0f;
				sp.procChance = 100;
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				MoltenInfernoTimer = TickNow + (SPELL_COOL_MOLTEN_INFERNO + RandomUInt() % SPELL_COOL_MOLTEN_INFERNO)*DifficultyScale;
				_unit->CastSpell( t, SPELL_ID_MOLTEN_INFERNO, false );	
			}
			if( MoltenElementalTimer < TickNow && _unit->isCasting() == false )
			{
				MoltenElementalTimer = TickNow + (SPELL_COOL_SPAWN_ADD4 + RandomUInt() % SPELL_COOL_SPAWN_ADD4)*DifficultyScale;
				_unit->create_guardian( MOB_ID_MOLTEN_ELEMENTAL, 600000, 0, 0, 0, 0, false );	
			}
		}
		if( phase == 4 )
		{
			if( LavaScionTimer < TickNow && _unit->isCasting() == false )
			{
				LavaScionTimer = TickNow + (SPELL_COOL_SPAWN_ADD3 + RandomUInt() % SPELL_COOL_SPAWN_ADD3)*DifficultyScale;
				_unit->create_guardian( MOB_ID_LAVA_SCION, 600000, 0, 0, 0, 0, false );	
			}
		}
		if( phase == 5 )
		{
			if( SulfurasSmashTimer < TickNow && _unit->isCasting() == false )
			{
				SulfurasSmashTimer = TickNow + (SPELL_COOL_SULFURAS_SMASH + RandomUInt() % SPELL_COOL_SULFURAS_SMASH)*DifficultyScale;
				_unit->CastSpell( tank, SPELL_ID_SULFURAS_SMASH, false );	
			}
			if( EngulfingFlamesTimer < TickNow && _unit->isCasting() == false )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
				sp.minrange = 2.0f;
				sp.maxrange = 45.0f;
				sp.procChance = 100;
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				EngulfingFlamesTimer = TickNow + (SPELL_COOL_SPAWN_ADD2 + RandomUInt() % SPELL_COOL_SPAWN_ADD2)*DifficultyScale;
				if( t )
				{
					LocationVector v = t->GetPosition();
					_unit->create_guardian( MOB_ID_ENGULFING_FLAMES, 0x0FFFFFFF, 0, NULL, &v, 0, false );
				}
			}
		}

		if( phase == 1 && _unit->GetHealthPct() < 70 )
		{
			LavaBoltTimer = TickNow + RandomUInt() % 25000;
			phase = 2;
		}
		if( phase == 2 && _unit->GetHealthPct() < 60 )
		{
			SulfurasSmashTimer = TickNow + RandomUInt() % 25000;
			EngulfingFlamesTimer = TickNow + RandomUInt() % 25000;
			MoltenInfernoTimer = TickNow + RandomUInt() % 25000;
			MoltenElementalTimer = TickNow + RandomUInt() % 25000;
			MoltenSeedTimer = TickNow + RandomUInt() % 25000;
			phase = 3;
		}
		if( phase == 3 && _unit->GetHealthPct() < 40 )
		{
			LavaScionTimer = TickNow + RandomUInt() % 25000;
			phase = 4;
		}
		if( phase == 4 && _unit->GetHealthPct() < 30 )
		{
			SulfurasSmashTimer = TickNow + RandomUInt() % 25000;
			EngulfingFlamesTimer = TickNow + RandomUInt() % 25000;
			phase = 5;
		}
	}
};


class MagmaTrap : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(MagmaTrap);

   uint32 TrapArmedStamp;
    MagmaTrap(Creature* pCreature) : CreatureAIScript(pCreature)
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
		TrapArmedStamp = GetTickCount() + TRAP_ARMING_DURATION;
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
		float r = 5.0f * 5.0f;
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
				_unit->CastSpell( SafeUnitCast(*itr), 100108, true );	//magma trap erruption
				break;
			}
		}
		_unit->ReleaseInrangeLock();
	}
};

class EngulfingFlames : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(EngulfingFlames);

   uint32 TrapArmedStamp;
    EngulfingFlames(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 8000 );
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
		TrapArmedStamp = GetTickCount() + TRAP_ARMING_DURATION;
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
		}
		_unit->CastSpell( _unit, 100184, true );	//Engulfing Flames - AOE
	}
    void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		if( TrapArmedStamp > GetTickCount() )
			return;
		_unit->CastSpell( _unit, 100184, true );	//Engulfing Flames - AOE
/*		float r = 5.0f * 5.0f;
		InRangePlayerSet::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		for( itr2 = _unit->GetInRangePlayerSetBegin(); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( !SafeUnitCast( *itr )->isAlive() )
				continue;
			if( IsInrange( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), (*itr), r ) 
				&& isAttackable( _unit, SafeUnitCast( *itr ), true ) )
			{
				_unit->CastSpell( SafeUnitCast(*itr), 99224, true );	//magma trap erruption
				break;
			}
		}
		_unit->ReleaseInrangeLock();*/
	} 
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_CAT_FORM			98374
#define SPELL_ID_CAT_FORM2			97678	//not blizzlike but cool :P !
#define SPELL_ID_SCORPION_FORM		98379
#define SPELL_ID_SCORPION_FORM2		97679	//not blizzlike but cool :P !
#define SPELL_COOL_SHAPESHIFT		120000
#define SPELL_ID_LEAPING_FLAMES		98535
#define SPELL_COOL_LEAPING_FLAMES	15000
#define SPELL_ID_ADRENALINE			97238
#define SPELL_ID_FURY				97235
#define SPELL_ID_FLAME_SCYTH		98474
#define SPELL_COOL_FLAME_SCYTH		10000
#define SPELL_ID_FIERY_CYCLONE		98443
//#define SPELL_COOL_FIERY_CYCLONE	40000
#define MOB_ID_SPIRIT_OF_FLAME		52593
#define MOB_ID_BURNING_ORB			53216
#define SPELL_COOL_SPIRIT_OF_FLAME	30000
#define SPELL_ID_BURNING_ORBS		98565
//#define SPELL_COOL_BURNING_ORBS		30000
#define SPELL_ID_SEARING_SEEDS		98450
//#define SPELL_COOL_SEARING_SEEDS	25000

class MajorDomo : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(MajorDomo);

   /* 
    HeroicConcentration – Each player who engages Fandral on Heroic Difficulty gains a Concentration power bar. This bar fills over time, increasing damage and healing done by 25% for every 25 Concentration up to 100. Players hit by a damaging attack or spell lose all Concentration. 
Shapeshifting
Fandral transforms into a Cat when his enemies are not clustered together or into a Scorpion when 7 or more of his enemies are clustered together.On every third transform, Fandral briefly pauses in human form.
    Cat Form – Fandral transforms into a Cat when his enemies are not clustered together.
        Leaping Flames – Fandral leaps at an enemy, leaving behind a Spirit of the Flame. He lands in a blaze of glory, igniting the ground at his destination and causing it to burn enemy enemy units for 23562 to 26437 Fire damage every 0.5 sec. for 1 min. This attack costs 100 energy.
        Spirit of the Flame – These images of Fandral attack enemies until defeated.
        Adrenaline – Fandral gains a stack of Adrenaline each time he performs Leaping Flames. Adrenaline increases his energy regeneration rate by 20% per application. Fandral loses all stacks of Adrenaline when he switches forms.
        Fury – Fandral gains a stack of Fury each time he transforms into a Cat or Scorpion, increasing the damage of Leaping Flames and Flame Scythe by 8% per application. This effect stacks. 
    Scorpion Form – Fandral transforms into a Scorpion when 7 or more of his enemies are clustered together.

    In 25 player raids, Fandral transforms when 18 or more of his enemies are clustered together.
        Flame Scythe – Fandral inflicts 750000 Fire damage to enemies in front of him. Damage is split equally among targets hit. This attack costs 100 energy.
        Adrenaline – Fandral gains a stack of Adrenaline each time he performs Flame Scythe. Adrenaline increases his energy regeneration rate by 20% per application. Fandral loses all stacks of Adrenaline when he switches forms.
        Fury – Fandral gains a stack of Fury each time he transforms into a Cat or Scorpion, increasing the damage of Leaping Flames and Flame Scythe by 8% per application. This effect stacks. 
    Human Form – When Fandral enters Human form, he briefly envelops his enemies in a Fiery Cyclone and casts an additional spell.When switching from Cat form to Scorpion form, Fandral unleashes Searing Seeds.When switching switching from Scorpion form to Cat form, Fandral unleashes Burning Orbs.
        Fiery Cyclone – Fiery Cyclone tosses all enemy targets into the air, preventing all action but making them invulnerable for 3 sec.
        Searing Seeds – Searing Seeds implants fiery seeds in Fandral's enemies. Each seed grows at a different rate. When fully grown, the seeds explode, inflicting 60000 Fire damage to players within 12 yards.
        Burning Orbs – Fandral summons several orbs around the room. Each orb attacks the nearest player, burning them for 5000 Fire damage every 2 sec. This effect stacks. 
*/
	uint32 phase;
	uint32 FormChangeTimer;
	uint32 FormChangeCounter;
	uint32 FormChangeCounter2;	//not blizzlike !
	uint32 LeapingFlamesTimer;
	uint32 FlameScythTimer;
	uint32 SpiritOfTheFlameTimer;
	float	DifficultyScale;
	MajorDomo(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		phase = 0;
	}
	void OnLoad()
	{
		//make 25 man mode harder then 10 man mode ?
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
		{
			uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
			_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now - 500 ) );	//attack a bit faster cause there are more healers
		}
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = 0.93f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = 0.86f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = 0.80f;
		RegisterAIUpdateEvent( 1000 );
//		_unit->GetAIInterface()->m_canMove = false;
	}
	void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		uint32 TickNow = GetTickCount();
		//init phase
		if( phase == 0 )
		{
			FormChangeTimer = TickNow + RandomUInt() % 25000;
			LeapingFlamesTimer = TickNow + RandomUInt() % 25000;
			FlameScythTimer = TickNow + RandomUInt() % 25000;
			SpiritOfTheFlameTimer = TickNow + RandomUInt() % 25000;
			FormChangeCounter = 0;
			FormChangeCounter2 = 0;
			phase = 1;
		}
		//time to change form
		if( FormChangeTimer < TickNow && _unit->isCasting() == false )
		{
			FormChangeTimer = TickNow + (SPELL_COOL_SHAPESHIFT + RandomUInt() % SPELL_COOL_SHAPESHIFT)*DifficultyScale;
//			_unit->CastSpell( _unit, SPELL_ID_BURNING_ORBS, false );		//not blizzlike !
			_unit->create_guardian( MOB_ID_BURNING_ORB, 600000, 0, 0, 0, 0, false );	
			_unit->CastSpell( _unit, SPELL_ID_FIERY_CYCLONE, false );
			//form cycle : Human, cat, human, scorpion
			if( FormChangeCounter == 1 ) //from cat to scorpion
			{
				_unit->RemoveAura( SPELL_ID_CAT_FORM );
				_unit->RemoveAura( SPELL_ID_CAT_FORM2 );
//				_unit->CastSpell( _unit, SPELL_ID_BURNING_ORBS, false );
				_unit->create_guardian( MOB_ID_BURNING_ORB, 600000, 0, 0, 0, 0, false );	
			}
			else if( FormChangeCounter == 3 ) //from scorpion to cat
			{
				_unit->RemoveAura( SPELL_ID_SCORPION_FORM );
				_unit->RemoveAura( SPELL_ID_SCORPION_FORM2 );
//				_unit->CastSpell( _unit, SPELL_ID_BURNING_ORBS, false );
				_unit->create_guardian( MOB_ID_BURNING_ORB, 600000, 0, 0, 0, 0, false );	
			}
			else if( FormChangeCounter == 0 )
			{
				if( FormChangeCounter2 % 2 == 0 )
					_unit->CastSpell( _unit, SPELL_ID_CAT_FORM, false );
				else
					_unit->CastSpell( _unit, SPELL_ID_CAT_FORM2, false );
				_unit->CastSpell( _unit, SPELL_ID_FURY, true );	
			}
			else if( FormChangeCounter == 2 )
			{
				if( FormChangeCounter2 % 2 == 0 )
					_unit->CastSpell( _unit, SPELL_ID_SCORPION_FORM, false );
				else
					_unit->CastSpell( _unit, SPELL_ID_SCORPION_FORM2, false );
				_unit->CastSpell( _unit, SPELL_ID_FURY, true );	
				FormChangeCounter2++;
			}
			FormChangeCounter = ( FormChangeCounter + 1 ) % 4;
		}
		///cat form
		if( FormChangeCounter == 1 )
		{
			if( LeapingFlamesTimer < TickNow && _unit->isCasting() == false )
			{
				LeapingFlamesTimer = TickNow + (SPELL_COOL_LEAPING_FLAMES + RandomUInt() % SPELL_COOL_LEAPING_FLAMES)*DifficultyScale;
 				_unit->CastSpell( tank, SPELL_ID_LEAPING_FLAMES, false );	
 				_unit->CastSpell( _unit, SPELL_ID_ADRENALINE, true );	
			}
			if( SpiritOfTheFlameTimer < TickNow && _unit->isCasting() == false )
			{
				SpiritOfTheFlameTimer = TickNow + (SPELL_COOL_SPIRIT_OF_FLAME + RandomUInt() % SPELL_COOL_SPIRIT_OF_FLAME)*DifficultyScale;
				_unit->create_guardian( MOB_ID_SPIRIT_OF_FLAME, 600000, 0, 0, 0, 0, false );	
			}
		}
		//scrotpion form
		if( FormChangeCounter == 3 )
		{
			if( FlameScythTimer < TickNow && _unit->isCasting() == false )
			{
				FlameScythTimer = TickNow + (SPELL_COOL_FLAME_SCYTH + RandomUInt() % SPELL_COOL_FLAME_SCYTH)*DifficultyScale;
 				_unit->CastSpell( tank, SPELL_ID_FLAME_SCYTH, true );	
			}
		}
	}
};

class BurningOrb : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(BurningOrb);

    BurningOrb(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 3000 );
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
    }
	uint32 SpellId;
	void OnLoad()
	{
		AI_Spell sp;
		sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
		sp.minrange = 10.0f;
		sp.maxrange = 60.0f;
		sp.procChance = 20;
		Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
		if( t )
		{
			_unit->GetAIInterface()->AttackReaction( t, 10, 0 );
//			_unit->SetPosition( t->GetPosition() );
		}
//		else
//		{
//			_unit->SetPositionZ( _unit->GetPositionZ() + 3.0f );
//			_unit->GetAIInterface()->StopMovement( 0 );
//		}

		SpellId = 98584;	//normal 10 man
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			SpellId = 100210;	
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			SpellId = 100209;	
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			SpellId = 100211;	
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 33853 );	//No idea about the display :(
//		_unit->CastSpell( _unit, 98583 , false );				//Burning Orb
	}
	void AIUpdate()
	{
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		if( tank->GetDistance2dSq( _unit ) > 3.0f*3.0f )
			_unit->GetAIInterface()->MoveTo( tank->GetPositionX(), tank->GetPositionY(), tank->GetPositionZ() + 3.0f, 0.0f );
		_unit->CastSpell( tank, SpellId , false );	//Burning Orb
	} 
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SetupFireLands(ScriptMgr* mgr)
{
	SpellEntry *sp;
	//update creature_proto set respawntime=60*60*60*1000 where entry in  ( 52571,52409,52530,52498,52558,53691,53494 );
	//Majordomo Staghelm <Archdruid of the Flame>
	///////////////////////////////////////////////////////
	mgr->register_creature_script(52571, &MajorDomo::Create);	
	mgr->register_creature_script(53216, &BurningOrb::Create);	
	//Burning Orb
	sp = dbcSpell.LookupEntryForced( 98584 );
	//if( sp != NULL )
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
	//Leaping Flames
	sp = dbcSpell.LookupEntryForced( 98535 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[0].EffectRadiusIndex = 37; //7 yards
	}
	//Ragnaros
	///////////////////////////////////////////////////////
	mgr->register_creature_script(52409, &Ragnaros2::Create);	
	mgr->register_creature_script(53086, &MagmaTrap::Create);	
	mgr->register_creature_script(53485, &EngulfingFlames::Create);	
	//Sulfuras Smash
	sp = dbcSpell.LookupEntryForced( SPELL_ID_SULFURAS_SMASH );
	//if( sp != NULL )
	{
//		sp->eff[0].Effect = SPELL_EFFECT_TRIGGER_SPELL;
//		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->c_is_flags2 |= SPELL_FLAG2_IS_TRIGGERED_ON_TARGET;
	}
	//Magma Trap
	sp = dbcSpell.LookupEntryForced( 98170 );
	//if( sp != NULL )
		sp->DurationIndex = 6;	//10 minutes ?
	//Engulfing Flames
	sp = dbcSpell.LookupEntryForced( 99224 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 || sp->eff[0].EffectRadiusIndex == 36 )
			sp->eff[0].EffectRadiusIndex = 13;	//10 yards
	}
	///////////////////////////////////////////////////////
	GossipScript * gs = (GossipScript*) new FirelandsTeleporter();
	mgr->register_gossip_script(54299, gs);
	//Harsh Winds
	sp = dbcSpell.LookupEntryForced( SPELL_ID_HARSH_WINDS );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 || sp->eff[0].EffectRadiusIndex == 36 )
			sp->eff[0].EffectRadiusIndex = 11;	//45 yards
	}
	//Firestorm
	sp = dbcSpell.LookupEntryForced( SPELL_ID_FIRESTORM );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 || sp->eff[0].EffectRadiusIndex == 36 )
			sp->eff[0].EffectRadiusIndex = 11;	//45 yards
	}
	//Blazing Claw
	sp = dbcSpell.LookupEntryForced( SPELL_ID_BLAZING_CLAW );
	//if( sp != NULL )
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
	//Fiery Vortex
	sp = dbcSpell.LookupEntryForced( 99794 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 || sp->eff[0].EffectRadiusIndex == 36 )
			sp->eff[0].EffectRadiusIndex = 9;	//20 yards
		sp->eff[0].EffectAmplitude = 5000;	//was 1 sec, but seems to be a killer
	}
	//Fiery Tornado
	sp = dbcSpell.LookupEntryForced( 99816 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 || sp->eff[0].EffectRadiusIndex == 36 )
			sp->eff[0].EffectRadiusIndex = 9;	//20 yards
		sp->eff[0].EffectAmplitude = 5000;	//was 1 sec, but seems to be a killer
	}
	//Alysrazor
	///////////////////////////////////////////////////////
	mgr->register_creature_script(52530, &Alysrazor::Create);	
	mgr->register_creature_script(53896, &BlazingTalon::Create);	
	//Beth'tilac <The Red Widow>
	///////////////////////////////////////////////////////
	mgr->register_creature_script(52498, &Bethtilac::Create);	
	mgr->register_creature_script(53642, &CristalWebSpinner::Create);	
	mgr->register_creature_script(53635, &CristalWebDrone::Create);	
	mgr->register_creature_script(53631, &CristalWebSpiderling::Create);	
	//Lord Rhyolith
	///////////////////////////////////////////////////////
	mgr->register_creature_script(52558, &LordRhyolith::Create);	
	mgr->register_creature_script(52620, &FragmentOfRyolith::Create);	
	mgr->register_creature_script(53211, &SparkOfRyolith::Create);	
	mgr->register_creature_script(52582, &Volcano::Create);	
	sp = dbcSpell.LookupEntryForced( SPELL_ID_MOLTEN_ARMOR );
	//if( sp != NULL )
	{
		sp->eff[1].Effect = SPELL_EFFECT_NULL;
		sp->eff[2].Effect = SPELL_EFFECT_NULL;
	}
	//Lava strike
	sp = dbcSpell.LookupEntryForced( 98276 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 )
			sp->eff[0].EffectRadiusIndex = 11;	//45 yards
	}
	//Lava strike
	sp = dbcSpell.LookupEntryForced( 98489 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 )
			sp->eff[0].EffectRadiusIndex = 11;	//45 yards
	}
	//Magma
	sp = dbcSpell.LookupEntryForced( 98472 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 )
			sp->eff[0].EffectRadiusIndex = 11;	//45 yards
	}
	//shannox event
	///////////////////////////////////////////////////////
	mgr->register_creature_script(53691, &Shannox::Create);	
	mgr->register_creature_script(53694, &Riplimb::Create);	
	mgr->register_creature_script(53695, &Rageface::Create);	
	mgr->register_creature_script(53713, &CristalPrisonTrap::Create);	
	mgr->register_creature_script(53819, &CristalPrison::Create);	
	mgr->register_creature_script(53724, &ImmolationTrap::Create);	
	//Throw Crystal Prison Trap
	sp = dbcSpell.LookupEntryForced( SPELL_ID_CHRISTAL_TRAP );
	//if( sp != NULL )
		sp->DurationIndex = 6;	//10 minutes ?
	sp = dbcSpell.LookupEntryForced( 99837 );
	//if( sp != NULL )
		sp->DurationIndex = 6;	//10 minutes ?		
	//Throw Immolation Trap
	sp = dbcSpell.LookupEntryForced( SPELL_ID_IMMOLATION_TRAP );
	//if( sp != NULL )
		sp->DurationIndex = 6;	//10 minutes ?
	//Jagged Tear
	sp = dbcSpell.LookupEntryForced( 99937 );
	//if( sp != NULL )
		sp->rangeIndex = 2;//5 yards
	sp = dbcSpell.LookupEntryForced( 99936 );
	//if( sp != NULL )
		sp->rangeIndex = 13;//Anywhere
	///////////////////////////////////////////////////////

	//balroc event
	mgr->register_creature_script(53494, &Baleroc::Create);	
	mgr->register_creature_script(53495, &ShardOfTorment::Create);	
//	mgr->register_dummy_spell(99516, &DummySpellHandlerCountdown);
	//Blaze of Glory
	sp = dbcSpell.LookupEntryForced( SPELL_ID_BLAZE_OF_GLORY );
	//if( sp != NULL )
		sp->DurationIndex = 3; // 1 minute to avoid people from exploiting it too much
	//decimation blade
	sp = dbcSpell.LookupEntryForced( SPELL_ID_DECIMATION_BLADE );
	//if( sp != NULL )
	{
		sp->procChance = 100;	
		sp->proc_interval = 2500;
		sp->procFlags = PROC_ON_MELEE_ATTACK;
	}
	//decimation strike
	sp = dbcSpell.LookupEntryForced( 99353 );
	//if( sp != NULL )
		sp->eff[1].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
	//inferno blade
	sp = dbcSpell.LookupEntryForced( SPELL_ID_INFERNO_BLADE );
	//if( sp != NULL )
	{
		sp->procChance = 100;	
		sp->proc_interval = 2500;
		sp->procFlags = PROC_ON_MELEE_ATTACK;
	}
	//Torment
	sp = dbcSpell.LookupEntryForced( SPELL_ID_TORMENT );
	//if( sp != NULL )
	{
		sp->eff[1].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL;
		sp->eff[1].EffectTriggerSpell = 99262;	//vital spark
		sp->procFlags = PROC_ON_HEAL_SPELL;	//we need a direct heal spell
	}
	//Tormented
	sp = dbcSpell.LookupEntryForced( SPELL_ID_TORMENTED );
	//if( sp != NULL )
	{
		sp->eff[2].Effect = SPELL_EFFECT_APPLY_AURA;
		sp->eff[2].EffectApplyAuraName = SPELL_AURA_DRINK_NEW;
		sp->eff[2].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->eff[2].EffectAmplitude = 1000;	
		sp->AuraPeriodicDummyTickHook = PeriodicDummyTormented;
		sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
	}
	//wave of torment
	sp = dbcSpell.LookupEntryForced( SPELL_ID_WAVE_OF_TORMENT );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 )
			sp->eff[0].EffectRadiusIndex = 12;	//100 yards
	}

	//Countdown
	sp = dbcSpell.LookupEntryForced( SPELL_ID_COUNTDOWN );
	//if( sp != NULL )
	{
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_DRINK_NEW;
		sp->eff[0].EffectAmplitude = 1000;	
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_DUEL;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->AuraPeriodicDummyTickHook = PeriodicDummyCountdown;

		sp->eff[1].Effect = SPELL_EFFECT_APPLY_AURA;
		sp->eff[1].EffectApplyAuraName = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->eff[1].EffectTriggerSpell = SPELL_ID_COUNTDOWN_EXPLODE;
		sp->eff[1].EffectAmplitude = 8000;	//at the end of the aura cast AOE
	}

	sp = dbcSpell.LookupEntryForced( SPELL_ID_COUNTDOWN_EXPLODE );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_FRIENDLY_IN_AREA;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 )
			sp->eff[0].EffectRadiusIndex = 12;	//100 yards
	}
}