#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305) // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PLATFORM_SIZE				10 
#define LAVAPOOL_SIZE				60 
#define LAVAPOOL_X					4376 
#define LAVAPOOL_Y					1454
#define LAVAPOOL_Z					131
#define LAVA_DMG_PER_SEC			3000
#define PLATFORM_RESPAWN_TIME		20*1000	
#define PLATFORM_ENTRY				209670	
#define SPELL_ID_MOLTEN_AXE			101836
#define SPELL_ID_MOLTEN_FIST		101866
#define SPELL_ID_PULVERIZE			101626
#define SPELL_ID_THROW_TOTEM		101614
#define SPELL_COOL_THROW_TOTEM		10*1000

char EchoOfBaneYells[8][107] = {
    "A just punishment!",
    "My wrath knows no bounds!",
    "Suffer for your arrogance!",
    "There will be no escape!",
    "This is the price you pay!",
    "What dark horrors have you wrought in this place? By my ancestors' honor, I shall take you to task!",
    "Where... is this place? What... have I done? Forgive me, my father...",
    "You! Are you the ones responsible for what has happened here...?",
};
class EchoOfBane : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(EchoOfBane);
/* 
     Molten Axe – The Echo of Baine's Axe ignites for 10 sec. when he comes into contact with lava. The fiery axe causes melee attacks to inflict an additional 9750 to 10250 Fire damage.
        Molten Fists – Players' fists ignite for 20 sec. when they come into contact with lava. The ignited fists cause melee attacks to inflict an additional 9750 to 10250 Fire damage. 
    Pulverize – The Echo of Baine slams the ground with his totem, inflicting 30000 Physical damage to enemies within 15 yards and destroying the nearby platform.
    Throw Totem – The Echo of Baine throws his totem at a random player, inflicting 58500 to 61500 Physical damage and knocking them back. The totem then persists on the ground at the target's location for 20 sec.
        Throw Totem – A player may attempt to throw the totem back at the Echo of Baine. A successful throw inflicts 5% of the Echo of Baine's maximum health as Physical damage, stuns him, and increases his damage taken by 50% for 20 sec. 
*/
	uint32	PlatformTimer;
//	uint32	PlatformX,PlatformY,PlatformZ;
//	uint64	PlatformGUID;
	uint32  ThrowTotemTimer;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	EchoOfBane(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
//		PlatformGUID = 0;
//		PlatformX = _unit->GetPositionX();
//		PlatformY = _unit->GetPositionY();
//		PlatformZ = _unit->GetPositionZ();
		ThrowTotemTimer = 0;
		RegisterAIUpdateEvent( 1000 );
	}
	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		uint32 TickNow = GetTickCount();

		if( ThrowTotemTimer == 0 )
		{
			ThrowTotemTimer = GetTickCount() + RandomUInt() % 25000;
			PlatformTimer = GetTickCount() + RandomUInt() % 25000;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			TalkIndex = 0;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, EchoOfBaneYells[ TalkIndex ]);
			TalkIndex = ( TalkIndex + 1 ) % 8;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

		if( ThrowTotemTimer < TickNow && _unit->isCasting() == false )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 0.1f;
			sp.maxrange = 15.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			if( t == tank )
				t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			if( t == tank )
				t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			ThrowTotemTimer = TickNow + (SPELL_COOL_THROW_TOTEM + RandomUInt() % SPELL_COOL_THROW_TOTEM);
			_unit->CastSpell( t, SPELL_ID_THROW_TOTEM, false );	
		}

		//do we need to create a new platform ?
		if( PlatformTimer < TickNow )
		{
			_unit->CastSpell( tank, SPELL_ID_PULVERIZE , true );	//destroy platform
			_unit->CastSpell( _unit, SPELL_ID_MOLTEN_AXE , true );	
			_unit->CastSpell( _unit, SPELL_ID_MOLTEN_FIST , true );	
			PlatformTimer = TickNow + (PLATFORM_RESPAWN_TIME + RandomUInt() % PLATFORM_RESPAWN_TIME);

/*			GameObject *platform = _unit->GetMapMgr()->GetGameObject( PlatformGUID );
			if( platform )
				platform->Despawn( 1, 0 );
			//spawn a new platform
			platform = _unit->GetMapMgr()->CreateGameObject( PLATFORM_ENTRY );
			PlatformX = LAVAPOOL_X + LAVAPOOL_SIZE - ( RandomUInt() % ( 2*LAVAPOOL_SIZE ) );
			PlatformY = LAVAPOOL_Y + LAVAPOOL_SIZE - ( RandomUInt() % ( 2*LAVAPOOL_SIZE ) );
			PlatformZ = LAVAPOOL_Z + 3.0f;
			platform->SetPositionX( PlatformX );
			platform->SetPositionY( PlatformY );
			platform->SetPositionZ( PlatformZ );
			platform->PushToWorld( _unit->GetMapMgr() );
			PlatformGUID = platform->GetGUID();
			_unit->GetAIInterface()->MoveTo( PlatformX, PlatformY, PlatformZ+2.0f, 0.0f ); */
		}
		//burn players that are not on the platform
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( !SafeUnitCast( *itr )->isAlive() || ( *itr )->IsPlayer() == false )
				continue;
			if( IsInrange( _unit, (*itr), PLATFORM_SIZE * PLATFORM_SIZE ) == false 
//				&& IsInrange( LAVAPOOL_X, LAVAPOOL_Y, LAVAPOOL_Z, (*itr), LAVAPOOL_SIZE*LAVAPOOL_SIZE ) == true
				&& isAttackable( _unit, SafeUnitCast( *itr ), true ) )
			{
				WorldPacket data(SMSG_ENVIRONMENTALDAMAGELOG, 21);
				data << ( *itr )->GetGUID() << uint8(DAMAGE_LAVA) << LAVA_DMG_PER_SEC << uint64(0);
				( *itr )->SendMessageToSet(&data, true);
				( *itr )->DealDamage(( *itr ), LAVA_DMG_PER_SEC, 0);
			}
		}
		_unit->ReleaseInrangeLock();

		//check if we are in our beloved lava pool. If not then we move back to the middle of the platform
		if( IsInrange( LAVAPOOL_X, LAVAPOOL_Y, LAVAPOOL_Z, _unit, (LAVAPOOL_SIZE+5.0f) * (LAVAPOOL_SIZE+5.0f) ) == false )
		{
			_unit->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, _unit, 0 );
			_unit->GetAIInterface()->WipeTargetList();	//exit combat
			_unit->GetAIInterface()->MoveTo( LAVAPOOL_X, LAVAPOOL_Y, LAVAPOOL_Z+2.0f, 0.0f );
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TELEPORTPOOL_SIZE				15 
#define TELEPORTPOOL_X					3033 
#define TELEPORTPOOL_Y					521 
#define TELEPORTPOOL_Z					22
#define	TELEPORT_AT_HP_CHANGE			5
#define SPELL_ID_FLARECORE			101927
#define NPC_ID_FLARECORE			54446
#define SPELL_COOL_FLARECORE		10*1000
#define SPELL_ID_FROST_BLADES		101339
#define NPC_ID_FROST_BLADES			54494
#define SPELL_COOL_FROST_BLADES		20*1000
#define SPELL_ID_FROSTBOLT_VOLLEY	72015
#define SPELL_COOL_FROSTBOLT_VOLLEY	5*1000
#define SPELL_ID_PYROBLAST			11366
#define SPELL_COOL_PYROBLAST		5*1000

char EchoOfJainaYells[10][112] = {
    "A little ice ought to quench the fire in your hearts...",
    "I didn't want to do that.",
    "I hate resorting to violence.",
    "I wish you'd surrendered.",
    "Perhaps this will cool your heads...",
    "Why won't you give up?!",
    "You asked for it.",
    "You forced my hand.",
    "I don't know who you are, but I'll defend this shrine with my life. Leave, now, before we come to blows.",
    "I understand, now. Farewell, and good luck.",
};

class EchoOfJaina : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(EchoOfJaina);
/* 
    Blink – The Echo of Jaina teleports to a nearby location.
    Flarecore – The Echo of Jaina hurls a growing Flarecore ember at the ground near an enemy. The ember explodes if touched, inflicting 5000-50000 Fire damage to enemies within 5 yds. If not exploded within 10 sec., the ember detonates and inflicts 90000 to 99000 Fire damage to all enemies.
    Frost Blades – The Echo of Jaina conjures three blades of ice and sends them flying forward. Enemies that collide with the blades are encased in a block of ice, stunning them for 5 sec.
    Frostbolt Volley – The Echo of Jaina inflicts 17000 to 23000 Frost damage to nearby enemies within 65 yards and reduces their movement speed by 30% for 4 sec.
    Pyroblast – The Echo of Jaina inflicts 47500 to 52500 Fire damage to her current target and an additional 5000 Fire damage every 3 sec. for 12 sec. 
*/
	int32	HealthPCTForTeleport;
//	uint32  FlareCoreTimer;
//	uint32  FrostBladesTimer;
	uint32  FrostboltVolleyTimer;
	uint32  PyroblastTimer;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	EchoOfJaina(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		HealthPCTForTeleport = 100;
		FrostboltVolleyTimer = 0;
		_unit->GetAIInterface()->m_canRangedAttack = true;
		RegisterAIUpdateEvent( 1000 );
	}
	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( FrostboltVolleyTimer == 0 )
		{
//			FlareCoreTimer = GetTickCount() + RandomUInt() % 25000;
//			FrostBladesTimer = GetTickCount() + RandomUInt() % 25000;
			FrostboltVolleyTimer = GetTickCount() + RandomUInt() % 25000;
			PyroblastTimer = GetTickCount() + RandomUInt() % 25000;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			TalkIndex = 0;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, EchoOfJainaYells[ TalkIndex ]);
			TalkIndex = ( TalkIndex + 1 ) % 10;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

		//general spells and their cooldown
/*		if( FlareCoreTimer < TickNow && _unit->isCasting() == false )
		{
			FlareCoreTimer = TickNow + (SPELL_COOL_FLARECORE + RandomUInt() % SPELL_COOL_FLARECORE);
			LocationVector dst;
			dst.x = TELEPORTPOOL_X + TELEPORTPOOL_SIZE - ( RandomUInt() % ( 2*TELEPORTPOOL_SIZE ) );
			dst.y = TELEPORTPOOL_Y + TELEPORTPOOL_SIZE - ( RandomUInt() % ( 2*TELEPORTPOOL_SIZE ) );
			dst.z = TELEPORTPOOL_Z + 3.0f;
//			_unit->CastSpellAoF( dst.x, dst.y, dst.z, dbcSpell.LookupEntryForced( SPELL_ID_FLARECORE ), false );	
			_unit->create_guardian( NPC_ID_FLARECORE, 1200000, 0, NULL, &dst );
		}
		if( FrostBladesTimer < TickNow && _unit->isCasting() == false )
		{
			FrostBladesTimer = TickNow + (SPELL_COOL_FROST_BLADES + RandomUInt() % SPELL_COOL_FROST_BLADES);
			_unit->create_guardian( NPC_ID_FROST_BLADES, 1200000 );
		}*/
		if( PyroblastTimer < TickNow && _unit->isCasting() == false )
		{
			PyroblastTimer = TickNow + (SPELL_COOL_PYROBLAST + RandomUInt() % SPELL_COOL_PYROBLAST);
			_unit->CastSpell( tank, SPELL_ID_PYROBLAST, false );	
		}
		if( FrostboltVolleyTimer < TickNow && _unit->isCasting() == false )
		{
			FrostboltVolleyTimer = TickNow + (SPELL_COOL_FROSTBOLT_VOLLEY + RandomUInt() % SPELL_COOL_FROSTBOLT_VOLLEY);
			_unit->CastSpell( tank, SPELL_ID_FROSTBOLT_VOLLEY, false );	
		}

		//do we need to teleport to a new location ?
		if( _unit->GetHealthPct() < HealthPCTForTeleport )
		{
			float m_destinationX = TELEPORTPOOL_X + TELEPORTPOOL_SIZE - ( RandomUInt() % ( 2*TELEPORTPOOL_SIZE ) );
			float m_destinationY = TELEPORTPOOL_Y + TELEPORTPOOL_SIZE - ( RandomUInt() % ( 2*TELEPORTPOOL_SIZE ) );
			float m_destinationZ = TELEPORTPOOL_Z + 3.0f;
			_unit->SetPosition( m_destinationX, m_destinationY, m_destinationZ, 0, true );
			_unit->GetAIInterface()->StopMovement(0);

			HealthPCTForTeleport = (int32)_unit->GetHealthPct() - TELEPORT_AT_HP_CHANGE;
		}
	}
};

class FlareCoreEmber : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(FlareCoreEmber);

   uint32 TrapArmedStamp;
   uint32 TrapSelfTriggerStamp;
    FlareCoreEmber(Creature* pCreature) : CreatureAIScript(pCreature)
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
		TrapArmedStamp = GetTickCount() + 1*1000;
		TrapSelfTriggerStamp = GetTickCount() + 15*1000;
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
				_unit->CastSpell( SafeUnitCast(*itr), 101980, true );	//Unstable Flare
				TrapSelfTriggerStamp = 0x7FFFFFFF;
				RemoveAIUpdateEvent();
				_unit->Despawn(1,0);
				break;
			}
		}
		_unit->ReleaseInrangeLock();
		if( TrapSelfTriggerStamp < GetTickCount() )
		{
			_unit->CastSpell( _unit, 101587, true );	//Flare
			TrapSelfTriggerStamp = 0x7FFFFFFF;
			RemoveAIUpdateEvent();
			_unit->Despawn(1,0);
		}
	}
};

class FrostBlade : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(FrostBlade);

    FrostBlade(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 3000 );
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
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
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		if( tank->GetDistance2dSq( _unit ) > 3.0f*3.0f )
			_unit->GetAIInterface()->MoveTo( tank->GetPositionX(), tank->GetPositionY(), tank->GetPositionZ() + 3.0f, 0.0f );
		//try to freez people near us
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( !SafeUnitCast( *itr )->isAlive() || ( *itr )->IsPlayer() == false )
				continue;
			if( IsInrange( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), (*itr), 5.0f*5.0f ) == false && isAttackable( _unit, SafeUnitCast( *itr ), true ) )
				_unit->CastSpell( tank, 101337 , false );	//Frost Blades
		}
		_unit->ReleaseInrangeLock();
	} 
}; 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define GHOUL_SPAWN_RADIUS			20
#define GHOUL_SPAWN_COOLDOWN		20000
#define NPC_GHOUL_SPAWN_ID			54191
#define SPELL_ID_SHRIEK				101412
#define SPELL_COOL_SHRIEK			5000
#define SPELL_ID_BLACK_ARROW		59712
#define SPELL_COOL_BLACK_ARROW		5000
#define SPELL_ID_UNHOLY_SHOT		101411
#define SPELL_COOL_UNHOLY_SHOT		5000
#define SPELL_ID_BLIGHTED_ARROWS	103171
#define SPELL_COOL_BLIGHTED_ARROWS	5000
#define SPELL_ID_WRACKING_PAIN		101221
#define SPELL_COOL_WRACKING_PAIN	2000

char EchoOfSylvanasYells[5][92] = {
    "And so ends your story.",
    "Another band of Deathwing's converts? I'll be sure your death is especially painful.",
    "Cry havoc!",
    "This ... isn't how it's supposed to ... end.",
    "Watch, heathens, as death surrounds you!",
};

class EchoOfSylvanas : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(EchoOfSylvanas);
/* 
	Sylvanas ascends towards the sky, pulling all of her enemies below her. Her calling then summons eight Risen Ghouls in a circle around Sylvanas. A shadowy link forms between each ghoul, and the area behind each ghoul fills with shadows. The ghouls then march mindlessly towards Sylvanas. When a Risen Ghoul reaches Sylvanas, it performs the Sacrifice spell.Crossing any of the links between the Risen Ghouls or standing within the shadows behind the ghouls inflicts Wracking Pain every second on the player. Destroying a Risen Ghoul will sever the link to other nearby ghouls, and remove the area of shadows behind it.
    Wracking Pain – Crossing any of the links between the Risen Ghouls or standing within the shadows behind the ghouls inflicts Wracking Pain on the player every second. Wracking Pain inflicts 50875 to 59125 Shadow damage.
    Risen Ghoul – When a Risen Ghoul reaches Sylvanas, it performs the Sacrifice spell.Destroying a Risen Ghoul will sever the link to other nearby ghouls, and remove the area of shadows behind it.
        DeadlySacrifice – Sacrifice inflicts 277500 to 322500 Shadow damage to players within 11 yards of Sylvanas. 
    MagicShriek of the Highborne – Sylvanas shrieks with anguish, inflicting 67500 to 82500 Shadow damage on a random player and decreasing their movement speed by 50% for 30 sec.
    Black Arrow – Sylvanas fires an explosive black arrow at a random player, inflicting 55500 to 64500 Shadow damage players within 10 yards of the target.
    Unholy Shot – Sylvanas targets a barrage of missiles at a random player, striking up to 3 players within 10 yards of the target. Each missile inflicts 27750 to 32250 Shadow damage and an additional 23125 to 26875 Shadow damage every 2 sec. for 10 sec.
    Blighted Arrows – Sylvanas blights the ground in a 3 yard radius circle beneath a random player and leaps towards the sky. The blight inflicts 32375 to 37625 damage every 1 sec. and reduces the movement speed by 50% of any player standing within it.At the peak of her jump, she launches a volley of deadly arrows at the blighted ground. The arrows inflict 46250 to 53750 Shadow damage and knock back any player still standing within the blight. 
*/
	uint32  ShriekTimer;
	uint32  BlackArrowTimer;
	uint32  UnholyShotTimer;
	uint32  BlightedArrowTimer;
	uint32	GhoulSpawnTimer;
	uint32	WreckingPainTimer;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	EchoOfSylvanas(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		ShriekTimer = 0;
		_unit->GetAIInterface()->m_canRangedAttack = true;
		RegisterAIUpdateEvent( 1000 );
	}
	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( ShriekTimer == 0 )
		{
			ShriekTimer = GetTickCount() + RandomUInt() % 25000;
			BlackArrowTimer = GetTickCount() + RandomUInt() % 25000;
			UnholyShotTimer = GetTickCount() + RandomUInt() % 25000;
			BlightedArrowTimer = GetTickCount() + RandomUInt() % 25000;
			GhoulSpawnTimer = GetTickCount() + RandomUInt() % 25000;
			WreckingPainTimer = GetTickCount() + RandomUInt() % 25000;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			TalkIndex = 0;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, EchoOfSylvanasYells[ TalkIndex ]);
			TalkIndex = ( TalkIndex + 1 ) % 5;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

		if( ShriekTimer < TickNow && _unit->isCasting() == false )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 2.0f;
			sp.maxrange = 45.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			ShriekTimer = TickNow + (SPELL_COOL_SHRIEK + RandomUInt() % SPELL_COOL_SHRIEK);
			_unit->CastSpell( t, SPELL_ID_SHRIEK, false );	
		}
		if( BlackArrowTimer < TickNow && _unit->isCasting() == false )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 2.0f;
			sp.maxrange = 45.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			BlackArrowTimer = TickNow + (SPELL_COOL_BLACK_ARROW + RandomUInt() % SPELL_COOL_BLACK_ARROW);
			_unit->CastSpell( t, SPELL_ID_BLACK_ARROW, false );	
		}
		if( UnholyShotTimer < TickNow && _unit->isCasting() == false )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 2.0f;
			sp.maxrange = 45.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			UnholyShotTimer = TickNow + (SPELL_COOL_UNHOLY_SHOT + RandomUInt() % SPELL_COOL_UNHOLY_SHOT);
			_unit->CastSpell( t, SPELL_ID_UNHOLY_SHOT, false );	
		}
		if( BlightedArrowTimer < TickNow && _unit->isCasting() == false )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 2.0f;
			sp.maxrange = 45.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			BlightedArrowTimer = TickNow + (SPELL_COOL_BLIGHTED_ARROWS + RandomUInt() % SPELL_COOL_BLIGHTED_ARROWS);
			_unit->CastSpell( t, SPELL_ID_BLIGHTED_ARROWS, false );	
		}

		//do we need to create a new ghoul ring ?
		if( GhoulSpawnTimer < TickNow )
		{
			//create 8 new ghouls
			Unit *prev_ghoul = NULL;
			Unit *first_ghoul = NULL;
			for(int i=0;i<8;i++)
			{
				LocationVector dst;
				float m_fallowAngle=-(float(M_PI)*2/8*i);
				dst.x = _unit->GetPositionX() + (GHOUL_SPAWN_RADIUS*(cosf(m_fallowAngle)));
				dst.y = _unit->GetPositionY() + (GHOUL_SPAWN_RADIUS*(sinf(m_fallowAngle)));
				dst.z = tank->GetPositionZ() + 2.0f;	//use tank instead sylvanas since she is flying
				Unit *cur_ghoul = _unit->create_guardian( NPC_GHOUL_SPAWN_ID, 600000, 0, NULL, &dst );
				if( prev_ghoul && cur_ghoul )
				{
					prev_ghoul->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, cur_ghoul->GetGUID() );
					prev_ghoul->SetUInt32Value( UNIT_CHANNEL_SPELL, 31611 );	//blue lightning channel, no idea about real one
				}
				prev_ghoul = cur_ghoul;
				if( first_ghoul == NULL )
					first_ghoul = cur_ghoul;
			}
			//make first point to last
			if( first_ghoul && prev_ghoul )
			{
				prev_ghoul->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, first_ghoul->GetGUID() );
				prev_ghoul->SetUInt32Value( UNIT_CHANNEL_SPELL, 31611 );	//blue lightning channel, no idea about real one
			}
			GhoulSpawnTimer = TickNow + (GHOUL_SPAWN_COOLDOWN + RandomUInt() % GHOUL_SPAWN_COOLDOWN);
		}
		//burn players that are behind the ghouls
		if( WreckingPainTimer < TickNow )
		{
			WreckingPainTimer = TickNow + (SPELL_COOL_WRACKING_PAIN + RandomUInt() % SPELL_COOL_WRACKING_PAIN);
			//find closest ghoul
			float min_dst = 99999999;
			{
				InRangeSetRecProt::iterator itr,itr2;
				_unit->AquireInrangeLock(); //make sure to release lock before exit function !
				InrangeLoopExitAutoCallback AutoLock;
				for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
				{
					itr = itr2;
					itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
					if( ( *itr )->IsUnit() == false || SafeUnitCast( *itr )->isAlive() == false || ( *itr )->GetEntry() != NPC_GHOUL_SPAWN_ID )
						continue;
					float d = Distance2DSq( _unit, (*itr) );
					if( d < min_dst )
						min_dst = d;
				}
				_unit->ReleaseInrangeLock();
			}
			if( min_dst != 99999999 )
			{
				InRangePlayerSetRecProt::iterator itr,itr2;
				_unit->AquireInrangeLock(); //make sure to release lock before exit function !
				InrangeLoopExitAutoCallback AutoLock;
				for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
				{
					itr = itr2;
					itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
					if( !SafeUnitCast( *itr )->isAlive() || ( *itr )->IsPlayer() == false )
						continue;
					if( Distance2DSq( _unit, (*itr) ) >= min_dst )
						_unit->CastSpell( (*itr), SPELL_ID_WRACKING_PAIN, true );	
				}
				_unit->ReleaseInrangeLock();
			}
		} 
	}
};


class SylvanasRisenGhoul : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(SylvanasRisenGhoul);

   uint64 SylvanasGUID;
    SylvanasRisenGhoul(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 1000 );
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
    }
	void OnLoad()
	{
		SylvanasGUID = 0;
		InRangeSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
		{
			itr = itr2;
			itr2++;
			if( ( *itr )->GetEntry() != 54123 || !SafeUnitCast( *itr )->isAlive() ) //get sylvanas
				continue;
			SylvanasGUID = ( *itr )->GetGUID();
			break;
		}
		_unit->ReleaseInrangeLock();
	}
	void AIUpdate()
	{
		Unit *sylvanas = _unit->GetMapMgr()->GetUnit( SylvanasGUID );
		if( sylvanas == NULL )
			return;
		float d = Distance2DSq( _unit, sylvanas );
		if( d < 5.0f )
		{
			_unit->CastSpell( _unit, 101348 , false );	//Sacrifice
			SylvanasGUID = 0;
			_unit->Despawn( 2000, 0 );
		}
		else
//			_unit->GetAIInterface()->MoveTo( sylvanas->GetPositionX(), sylvanas->GetPositionY(), sylvanas->GetPositionZ(), 0.0f );
			_unit->GetAIInterface()->MoveTo( sylvanas->GetPositionX(), sylvanas->GetPositionY(), _unit->GetPositionZ(), 0.0f );
	} 
}; 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_MOONBOLT			102193
#define SPELL_COOL_MOONBOLT			10000
#define SPELL_ID_DARK_MOONLIGHT		102414
#define SPELL_COOL_DARK_MOONLIGHT	10000
#define SPELL_ID_STARDUST			102173
#define SPELL_COOL_STARDUST			15000
#define SPELL_ID_MOONLANCE			102149
#define SPELL_COOL_MOONLANCE		10000
#define SPELL_ID_LUNAR_GUIDANCE		33589
#define SPELL_ID_TEARS_OF_ELUNE		102241
#define SPELL_ID_EYES_OF_GODDES		102606
#define SPELL_COOL_EYES_OF_GODDES	20000

char EchoOfTyrandeYells[13][122] = {
    "Eyes of night, pierce this darkness!",
    "Moon goddess, your light dims! I am lost without your guidance!",
    "Mother moon, I can no longer see your light! Your daughter is alone in the darkness!",
    "Spear of Elune, drive back the night!",
    "The darkness closes in...my vision is clouded...",
    "Elune guide you through the night.",
    "Give yourselves to the night, Elune will guide you from this mortal prison.",
    "I can...see the light of the moon...so clearly now. It is...beautiful...",
    "Let the peaceful light of Elune soothe your souls in this dark time.",
    "The darkness surrounds you, the light of Elune is your only salvation.",
    "The moonlight can bring rest to your weary souls in this forgotten place.",
    "There is nothing left for you here, nothing but death and sorrow.",
    "You have chosen a path of darkness. Mother moon, guide my hand; allow me to bring rest to these misbegotten souls.",
};

class EchoOfTyrande : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(EchoOfTyrande);
/* 
    Moonbolt – Tyrande launches a powerful blast of moon energy at her current target, inflicting 37000 to 43000 Arcane damage.
    Dark Moonlight – A pool of dark moonlight surrounds Tyrande, slowing spell-casting speed by 50% for all enemies within 15 yards.
    Stardust – Tyrande scatters a diffuse trail of star energy into the wind, inflicting 46250 to 53750 Arcane damage to all players.
    MagicMoonlance – Tyrande summons forth a powerful lance of moon energy that travels along the ground and splits into three parts after several seconds. Each Moonlance inflicts 46250 to 53750 Arcane damage to all players within 2 yards and stuns them for 5 sec. 
Eyes of the Goddess
Tyrande summons forth the twin eyes of Elune, the Moon Goddess. Players cannot attack the Eyes of the Goddess.
    Lunar Guidance – At 80% remaining health, Tyrande beseeches Elune for guidance. Tyrande is imbued with Lunar Guidance, allowing her spells to cast 25% faster. Tyrande gains a second stack of Lunar Guidance when she reaches 55% remaining health.
    Tears of Elune – At 30% remaining health, Tyrande cries out for aid from the Moon Goddess. Tears of Elune rain from the sky for the remainder of the battle, inflicting 27750 to 32250 Arcane damage to players within 4 yards. 
*/
	uint32  MoonBoltTimer;
	uint32  DarnMoonlightTimer;
	uint32  StardustTimer;
	uint32  MoonlanceTimer;
	uint32  EyesTimer;
	uint32	phase;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	EchoOfTyrande(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		_unit->GetAIInterface()->m_canRangedAttack = true;
		RegisterAIUpdateEvent( 1000 );
		MoonBoltTimer = 0;
	}
	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			MoonBoltTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( MoonBoltTimer == 0 )
		{
			MoonBoltTimer = GetTickCount() + RandomUInt() % 25000;
			DarnMoonlightTimer = GetTickCount() + RandomUInt() % 25000;
			StardustTimer = GetTickCount() + RandomUInt() % 25000;
			MoonlanceTimer = GetTickCount() + RandomUInt() % 25000;
			EyesTimer = GetTickCount() + RandomUInt() % 25000;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			TalkIndex = 0;
			phase = 0;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, EchoOfTyrandeYells[ TalkIndex ]);
			TalkIndex = ( TalkIndex + 1 ) % 13;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

		//check if we should apply phase dependent spells
		if( ( phase == 0 && _unit->GetHealthPct() <= 80 )
			|| ( phase == 1 && _unit->GetHealthPct() <= 55 )
			)
		{
			_unit->CastSpell( _unit, SPELL_ID_LUNAR_GUIDANCE, false );	
			phase += 1;
		}
		if( phase == 2 && _unit->GetHealthPct() <= 30 )
		{
			_unit->CastSpell( _unit, SPELL_ID_TEARS_OF_ELUNE, false );	
			phase = 2;
		}

		if( EyesTimer < TickNow && _unit->isCasting() == false )
		{
			EyesTimer = TickNow + (SPELL_COOL_EYES_OF_GODDES + RandomUInt() % SPELL_COOL_EYES_OF_GODDES);
//			_unit->CastSpell( tank, SPELL_ID_EYES_OF_GODDES, false );	
			_unit->CastSpell( tank, 102183, false );	//Piercing Gaze of Elune

		}
		if( MoonBoltTimer < TickNow && _unit->isCasting() == false )
		{
			MoonBoltTimer = TickNow + (SPELL_COOL_MOONBOLT + RandomUInt() % SPELL_COOL_MOONBOLT);
			_unit->CastSpell( tank, SPELL_ID_MOONBOLT, false );	
		}
		if( DarnMoonlightTimer < TickNow && _unit->isCasting() == false )
		{
//			DarnMoonlightTimer = TickNow + (SPELL_COOL_DARK_MOONLIGHT + RandomUInt() % SPELL_COOL_DARK_MOONLIGHT);
			DarnMoonlightTimer = 0x7FFFFFFF;
			_unit->CastSpell( _unit, SPELL_ID_DARK_MOONLIGHT, false );	
		}
		if( StardustTimer < TickNow && _unit->isCasting() == false )
		{
			StardustTimer = TickNow + (SPELL_COOL_STARDUST + RandomUInt() % SPELL_COOL_STARDUST);
			_unit->CastSpell( tank, SPELL_ID_STARDUST, false );	
		}
		if( MoonlanceTimer < TickNow && _unit->isCasting() == false )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 2.0f;
			sp.maxrange = 45.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			MoonlanceTimer = TickNow + (SPELL_COOL_MOONLANCE + RandomUInt() % SPELL_COOL_MOONLANCE);
			_unit->CastSpell( t, SPELL_ID_MOONLANCE, false );	
		}
	}
};

class EyeOfElune : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(EyeOfElune);

    EyeOfElune(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 7000 );
    }
	void OnLoad()
	{
		AI_Spell sp;
		sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
		sp.minrange = 10.0f;
		sp.maxrange = 60.0f;
		sp.procChance = 100;
		Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
		if( t )
			_unit->GetAIInterface()->AttackReaction( t, 10, 0 );
		_unit->GetAIInterface()->disable_melee = true;
	}
	void AIUpdate()
	{
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		_unit->CastSpell( tank, 102183 , false );	//Piercing Gaze of Elune
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_TEMPORAL_BLAST		102381
#define SPELL_COOL_TEMPORAL_BLAST	15000
#define SPELL_ID_DISTORSION_BOMB	101984
#define SPELL_COOL_DISTORSION_BOMB	20000
#define SPELL_ID_INFINIT_BREATH		102569
#define SPELL_COOL_INFINIT_BREATH	5000

char MurozondYells[13][188] = {
    "Again...? Is this your plot, your scheme?",
    "Another chance will make no difference. You will fail.",
    "So be it.",
    "The 'End Time,' I once called this place, this strand. I had not seen, by then; I did not know. You hope to... what? Stop me, here? Change the fate I worked so tirelessly to weave?",
    "The 'End Time,' I once called this place. I had not seen, by then; I did not know. You hope to... what? Stop me, here? Change the fate I worked so tirelessly to weave?",
    "The Hourglass' power is exhausted. No more games, mortals. Relent, or perish.",
    "The powers of the Hourglass do nothing to me!",
    "The sand has run out.",
    "Time ends.",
    "To repeat the same action and expect different results is madness.",
    "You crawl unwitting, like a blind, writhing worm, towards endless madness and despair. I have witnessed the true End Time. This? This is a blessing you simply cannot comprehend.",
    "You know not what you have done. Aman'Thul... What I... have... seen...",
    "Your time has come.",
};

class Murozond : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Murozond);
/* 
Temporal Blast – Murozond inflicts 25000 Arcane damage to all nearby enemies and increases their Arcane damage taken by 10% for 20 sec.
Distortion Bomb – Murozond launches an orb of temporal energy at the location of a random player. When the orb reaches this location, it explodes and leaves an 8 yard radius area of temporal distortion. The distortion area inflicts 25000 Arcane damage every second to all enemies standing within the area.
Infinite Breath – Inflicts 71250 to 78750 Shadow damage to all enemies in front of the caster.
*/
	uint32  TemporalBlastTimer;
	uint32  DistorsionBombTimer;
	uint32  InfinitBreathTimer;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	Murozond(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		RegisterAIUpdateEvent( 1000 );
		TemporalBlastTimer = 0;
	}
	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			TemporalBlastTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( TemporalBlastTimer == 0 )
		{
			TemporalBlastTimer = GetTickCount() + RandomUInt() % 25000;
			DistorsionBombTimer = GetTickCount() + RandomUInt() % 25000;
			InfinitBreathTimer = GetTickCount() + RandomUInt() % 25000;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			TalkIndex = 0;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, MurozondYells[ TalkIndex ]);
			TalkIndex = ( TalkIndex + 1 ) % 13;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

		if( TemporalBlastTimer < TickNow && _unit->isCasting() == false )
		{
			TemporalBlastTimer = TickNow + (SPELL_COOL_TEMPORAL_BLAST + RandomUInt() % SPELL_COOL_TEMPORAL_BLAST);
			_unit->CastSpell( tank, SPELL_ID_TEMPORAL_BLAST, false );	
		}
		if( DistorsionBombTimer < TickNow && _unit->isCasting() == false )
		{
			DistorsionBombTimer = TickNow + (SPELL_COOL_DISTORSION_BOMB + RandomUInt() % SPELL_COOL_DISTORSION_BOMB);
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 5.0f;
			sp.maxrange = 60.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			if( t )
			{
				SpellEntry * ent = dbcSpell.LookupEntry( SPELL_ID_DISTORSION_BOMB );
				Spell *newSpell = SpellPool.PooledNew( __FILE__, __LINE__ );
				newSpell->Init( _unit, ent, false, 0);
				SpellCastTargets targets(0);
				targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
				targets.m_destX = t->GetPositionX();
				targets.m_destY = t->GetPositionY();
				targets.m_destZ = t->GetPositionZ();
				newSpell->prepare(&targets);
			}
		}
		if( InfinitBreathTimer < TickNow && _unit->isCasting() == false )
		{
			InfinitBreathTimer = TickNow + (SPELL_COOL_INFINIT_BREATH + RandomUInt() % SPELL_COOL_INFINIT_BREATH);
			_unit->CastSpell( tank, SPELL_ID_INFINIT_BREATH, false );	
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SCRIPT_DECL EndTimeTeleporter : public GossipScript
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

		Menu->AddItem(GOSSIP_ICON_CHAT, "Teleport to Echo Of Bane", 3);
		Menu->AddItem(GOSSIP_ICON_CHAT, "Teleport to Echo of Jaina", 4);
		Menu->AddItem(GOSSIP_ICON_CHAT, "Teleport to Echo of Sylvanas", 5);
		Menu->AddItem(GOSSIP_ICON_CHAT, "Teleport to Echo of Tyrande", 6);

		uint32 boss_kills = 0;
		uint32 boss_alive = 0;
		for(uint32 i=0;i<Plr->GetMapMgr()->m_CreatureArraySize;i++)
		{
			Creature *tc = Plr->GetMapMgr()->m_CreatureStorage[i];
			if(	tc )
			{
				uint32 entry = tc->GetEntry();
				if( entry == 54431 || entry == 54445 || entry == 54123 || entry == 54544 )
				{
					if( tc->isAlive() == false )
						boss_kills++;
					else
						boss_alive++;
				}
			}
		}

		if( boss_kills > 3 )
			Menu->AddItem(GOSSIP_ICON_CHAT, "Teleport to Murozond", 7);
	    
		if(AutoSend)
			Menu->SendTo(Plr);
	};
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char *Code)
	{
		if(IntId == 3 )
			Plr->SafeTeleport(938, Plr->GetInstanceID(), 4351, 1289, 149, 1.35 ); 
		else if(IntId == 4 )
			Plr->SafeTeleport(938, Plr->GetInstanceID(), 3000, 571, 25, 1.35 ); 
		else if(IntId == 5 )
			Plr->SafeTeleport(938, Plr->GetInstanceID(), 3822, 1102, 85, 1.35 ); 
		else if(IntId == 6 )
			Plr->SafeTeleport(938, Plr->GetInstanceID(), 2951, 74, 10, 1.35 ); 
		else if(IntId == 7 )
			Plr->SafeTeleport(938, Plr->GetInstanceID(), 4039, -355, 122, 1.35 ); 
		Plr->CloseGossip();
	};
	void GossipEnd(Object* pObject, Player* pPlayer)
	{
		GossipScript::GossipEnd(pObject, pPlayer);
	}
};

void SetupEndTime(ScriptMgr* mgr)
{
	SpellEntry *sp;
	//delete from ai_agents where entry in ( 54431, 54445, 54446, 54494, 54544, 54941, 54942, 54432 );
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	mgr->register_creature_script(54431, &EchoOfBane::Create);	
//	mgr->register_creature_script(54434, &EchoOfBaneWeaponTotem::Create);	
	sp = dbcSpell.LookupEntryForced( SPELL_ID_MOLTEN_AXE );
	//if( sp != NULL )
	{
		sp->procChance = 100;	
		sp->proc_interval = 3000;
		sp->procFlags = PROC_ON_MELEE_ATTACK;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_MOLTEN_FIST );
	//if( sp != NULL )
	{
		sp->procChance = 100;	
		sp->proc_interval = 1200;
		sp->procFlags = PROC_ON_MELEE_ATTACK;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	GossipScript * gs = (GossipScript*) new EndTimeTeleporter();
	mgr->register_go_gossip_script(209441, gs);	//Time Transit Device
	mgr->register_go_gossip_script(209438, gs);	//Time Transit Device
	mgr->register_go_gossip_script(209439, gs);	//Time Transit Device
	mgr->register_go_gossip_script(209442, gs);	//Time Transit Device
	mgr->register_go_gossip_script(209443, gs);	//Time Transit Device
	mgr->register_go_gossip_script(209437, gs);	//Time Transit Device
	mgr->register_go_gossip_script(209440, gs);	//Time Transit Device
	mgr->register_go_gossip_script(209998, gs);	//Time Transit Device
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	mgr->register_creature_script(54445, &EchoOfJaina::Create);	
	mgr->register_creature_script(54446, &FlareCoreEmber::Create);	
	mgr->register_creature_script(54494, &FrostBlade::Create);	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	mgr->register_creature_script(54123, &EchoOfSylvanas::Create);	
	mgr->register_creature_script(54191, &SylvanasRisenGhoul::Create);	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	mgr->register_creature_script(54544, &EchoOfTyrande::Create);	
	mgr->register_creature_script(54941, &EyeOfElune::Create);		
	mgr->register_creature_script(54942, &EyeOfElune::Create);	
	sp = dbcSpell.LookupEntryForced( SPELL_ID_EYES_OF_GODDES );
	//if( sp != NULL )
		sp->rangeIndex = 13;//Anywhere
	sp = dbcSpell.LookupEntryForced( 102183 );	//Piercing Gaze of Elune
	//if( sp != NULL )
	{
		sp->rangeIndex = 4;//30 yards
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 || sp->eff[0].EffectRadiusIndex == 36 )
			sp->eff[0].EffectRadiusIndex = 11;	//45 yards
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[1].EffectRadiusIndex == 0 || sp->eff[1].EffectRadiusIndex == 36 )
			sp->eff[1].EffectRadiusIndex = 11;	//45 yards
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	mgr->register_creature_script(54432, &Murozond::Create);	
	//for some reason in front of the caster targetting is not working properly
	sp = dbcSpell.LookupEntryForced( 102569 );	//Infinite Breath
	//if( sp != NULL )
	{
		sp->rangeIndex = 4;//30 yards
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[1].Effect = SPELL_EFFECT_NULL;
	}
	//for some reason in front of the caster targetting is not working properly
	sp = dbcSpell.LookupEntryForced( 102381 );	//Temporal Blast
	//if( sp != NULL )
	{
		sp->rangeIndex = 4;//30 yards
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
	}
	sp = dbcSpell.LookupEntryForced( 101984 );	//Distortion Bomb
	//if( sp != NULL )
	{
		sp->rangeIndex = 4;//30 yards
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 || sp->eff[0].EffectRadiusIndex == 36 )
			sp->eff[0].EffectRadiusIndex = 8;	//5 yards
		sp->c_is_flags |= SPELL_FLAG_IS_SUMMONING_AT_TARGET;
	}
}
