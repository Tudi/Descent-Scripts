#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305) // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_CORUPTING_TOUCH	104939
#define SPELL_ID_FEL_FLAMES			108193
#define SPELL_COOL_FEL_FLAMES		10000
#define SPELL_ID_FEL_DECAY			105544
#define SPELL_COOL_FEL_DECAY		15000
#define SPELL_ID_DRAIN_ESSENCE		104905
#define SPELL_ID_EASY_PREY			105493
#define SPELL_COOL_EASY_PREY		10000
#define SPELL_ID_FEL_QUICKENING		105526
#define SPELL_COOL_FEL_QUICKENING	5000
#define SPELL_ID_ENFEEBLED			105442
#define SPELL_COOL_ENFEEBLED		50000
#define SPELL_ID_ENDLESS_FRENZY		105521

char PerotharnYells[15][84] = {
    "Cower in hiding, heh.",
    "ENOUGH! It is time to end this game!",
    "He is near, lurking in the shadows... I can sense it.",
    "I can see you.",
    "I will find you!",
    "No mortal may stand before me and live!",
    "None will reach the palace without besting Peroth'arn, first of fel-touched!",
    "Nooooo... how can this be?",
    "The rest of you, secure the courtyard.",
    "The shadows serve ME, now...",
    "The shadows will not save you.",
    "Who shut down the portals? Clever little worms.",
    "You hide well, worms. But how long you can delay your doom?",
    "You, Felguard. Hold this area.",
    "Your essence... is MINE.",
};

class Perotharn : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Perotharn);
/* 
    Corrupting Touch – Peroth'arn's melee strikes apply Corrupting Touch on his current target, increasing damage taken by 10% for 20 sec. This effect stacks.
    Fel Flames – Peroth'arn calls down a fel fireball on a random player, inflicting 33250 to 36750 Fire damage and leaving behind fire that causes 11875 to 13125 Fire damage every second for 30 sec.
    Fel Decay – Peroth'arn's sickly touch causes a random target to decay from within, instantly inflicting 38000 to 42000 Shadow damage and 8075 to 8925 Shadow damage every second for 10 sec. Any healing done to the afflicted player causes the healer to suffer 100% of the amount healed as Shadow damage.
    Drain Essence – At 60% remaining health, Peroth'arn stuns all enemies and inflicts Shadow damage every second for 4 sec. 
ImportantThe Eyes of Peroth'arn
After Illidan interrupts Drain Essence, Peroth'arn returns to the shadows to ambush the heroes. Peroth'arn unleashes his Eyes of Peroth'arn to search for the players. Players gain the protection of their Shadowcloaks to help them avoid the detection range of the Eyes.If players avoid detection for 40 seconds, Peroth'arn becomes visible and weakened with the Enfeebled effect.If an Eye of Peroth'arn discovers a player, Peroth'arn marks the player with Easy Prey, leaps out of the shadows, and attacks the discovered player.
    Easy Prey – When an Eye of Peroth'arn discovers a player, Peroth'arn casts Fel Quickening and marks his new target as Easy Prey. The mark removes the player's Shadowcloak or stealth and stuns them for 8 sec.
    Fel Quickening – Peroth'arn increases his attack speed by 100% for 15 sec.
    Enfeebled – If after 40 seconds Peroth'arn has not detected a player with his Eyes, Peroth'arn can no longer remain hidden in the shadows and becomes Enfeebled. This effect stuns Peroth'arn and increases his damage taken by 25% damage for 15 sec. 
    Endless Frenzy – At 20% remaining health, Peroth'arn frenzies and increases his damage by 25%. 
*/
	uint32  FelFlamesTimer;
	uint32  FelDecayTimer;
	uint32	phase;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	Perotharn(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		FelFlamesTimer = 0;
		RegisterAIUpdateEvent( 1000 );
	}
	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			FelFlamesTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( FelFlamesTimer == 0 )
		{
			_unit->CastSpell( _unit, SPELL_ID_CORUPTING_TOUCH, false );	
			FelFlamesTimer = GetTickCount() + RandomUInt() % 25000;
			FelDecayTimer = GetTickCount() + RandomUInt() % 25000;
			phase = 0;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			TalkIndex = 0;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, PerotharnYells[ TalkIndex ]);
			TalkIndex = ( TalkIndex + 1 ) % 15;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

		//check if we should apply phase dependent spells
		if( phase == 0 && _unit->GetHealthPct() <= 60 )
		{
			_unit->CastSpell( tank, SPELL_ID_DRAIN_ESSENCE, false );	
			_unit->CastSpell( tank, SPELL_ID_FEL_QUICKENING, false );	
			phase = 1;
		}
		if( phase == 1 && _unit->GetHealthPct() <= 20 )
		{
			_unit->CastSpell( tank, SPELL_ID_ENDLESS_FRENZY, false );	
			_unit->CastSpell( tank, SPELL_ID_FEL_QUICKENING, false );	
			phase = 2;
		}

		if( FelFlamesTimer < TickNow && _unit->isCasting() == false )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 2.0f;
			sp.maxrange = 45.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			FelFlamesTimer = TickNow + (SPELL_COOL_FEL_FLAMES + RandomUInt() % SPELL_COOL_FEL_FLAMES);
			_unit->CastSpell( t, SPELL_ID_FEL_FLAMES, false );	
		}
		if( FelDecayTimer < TickNow && _unit->isCasting() == false )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 2.0f;
			sp.maxrange = 45.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			FelDecayTimer = TickNow + (SPELL_COOL_FEL_DECAY + RandomUInt() % SPELL_COOL_FEL_DECAY);
			_unit->CastSpell( t, SPELL_ID_FEL_DECAY, false );	
		}
	}
};


class FelFlames : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(FelFlames);

   uint32 TrapArmedStamp;
    FelFlames(Creature* pCreature) : CreatureAIScript(pCreature)
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
		TrapArmedStamp = GetTickCount() + 3*1000;
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
			_unit->setLevel( owner->getLevel() );
		}
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, 16925 );	//no idea about the real display :(. This is not good for sure !
		_unit->SetScale( 5.0f );
		_unit->CastSpell( _unit, 84161, true );	//Fel Flames
		_unit->CastSpell( _unit, 84234, true );	//Fel Flames
	}
    void AIUpdate()
    {
		if( TrapArmedStamp > GetTickCount() )
			return;
		float r = 4.0f * 4.0f;
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
				_unit->CastSpell( SafeUnitCast(*itr), 109824, true );	//Fel Flames
		}
		_unit->ReleaseInrangeLock();
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_SERVANT_OF_THE_QUEEN		104939
#define SPELL_COOL_SERVANT_OF_THE_QUEEN		20000
#define SPELL_ID_SERVANT_OF_THE_QUEEN_STUN	110096
#define NPC_ID_SERVANT_OF_THE_QUEEN			54728

char QueenAzsharaYells[19][95] = {
    "Ah, welcome. You are here to join us in the coming celebration? No? A pity.",
    "Bold of you, to strike a queen. A lesser monarch might be enraged.",
    "Dance for the Eternal Court.",
    "Do not ask for mercy, after such an act.",
    "Enough! As much as I adore playing hostess, I have more pressing matters to attend to.",
    "I am unimpressed.",
    "I beseech of you, my beloved subjects: Put an end to these miscreants.",
    "I have no time for such diversions. Keepers of Eternity, will you stand for your queen?",
    "If you intend to play the fool, you may as well look the part.",
    "Keepers of Eternity, will you stand for your queen?",
    "Riders, to me!",
    "Serve Azshara, and rejoice.",
    "Serve Azshara, puppets, and rejoice.",
    "Still these strangers would oppose your queen's will. Who will stop them?",
    "Such insolence. My temper grows short.",
    "To prepare for a world of perfection, the imperfect must be swept away.",
    "Unfortunate, but deserved.",
    "Your conduct was inexcusable.",
    "My noble Varo'then, do return and dispose of this murderous band."
};

class QueenAzshara : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(QueenAzshara);
/* 
    Servant of the Queen – Queen Azshara charms a random player, causing them to become a puppet. Destroying the puppet strings releases the player from Azshara's control.
    Total Obedience – Queen Azshara attempts to charm every player and turn them into puppets for 20 sec. 
	Queen Aszhara commands two of her Enchanted Magi to attack the players. When the players defeat an Enchanted Magus or enough time elapses, another two Magi enter combat. Each Enchanted Magi possess one of three schools of magic.When the players defeat all of the Enchanted Magi, Queen Azshara retreats from combat and the players complete the encounter.
*/
	uint32  ServantOfTheQueenTimer;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	uint64	Protectors[6];
	bool	NeedProtectorSearch;
	bool	SpawnedCache;
	QueenAzshara(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		ServantOfTheQueenTimer = 0;
		NeedProtectorSearch = true;
		SpawnedCache = false;
		memset( Protectors, NULL, sizeof( Protectors ) );
		RegisterAIUpdateEvent( 1000 );
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
	}
	void AIUpdate()
    {
		//we fight until all our 6 servents are alive
		if( NeedProtectorSearch )
		{
			NeedProtectorSearch = false;
			uint32 prot_counter = 0;
			InRangeSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
			{
				if( prot_counter >= 6 )
					break;
				itr = itr2;
				itr2++;
				if( ( *itr )->GetEntry() == 54884 )		//enchanted = arcane
					Protectors[ prot_counter++ ] = ( *itr )->GetGUID();
				else if( ( *itr )->GetEntry() == 54882 )	//enchanted = fire
					Protectors[ prot_counter++ ] = ( *itr )->GetGUID();
				else if( ( *itr )->GetEntry() == 54883 )	//enchanted = frost
					Protectors[ prot_counter++ ] = ( *itr )->GetGUID();
			}
			_unit->ReleaseInrangeLock();
		}
		uint32 servents_dead = 0;
		for( uint32 i=0;i<6;i++)
		{
			Creature *prot = _unit->GetMapMgr()->GetCreature( Protectors[ i ] );
			if( prot == NULL || prot->isAlive() == false )
			{
				servents_dead++;
				Protectors[ i ] = 0;
			}
		}
		if( servents_dead >= 6 )
		{
			_unit->GetAIInterface()->WipeHateList();
			_unit->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, _unit, 0 );
			if( SpawnedCache == false )
			{
				SpawnedCache = true;
				GameObject *cache = _unit->GetMapMgr()->CreateAndSpawnGameObject( 210025, 3464.78, -5244.43, 229.949, 4.53786, 1.5f);
				if( cache )
					cache->PushToWorld( _unit->GetMapMgr() );
				GameObject *tele = _unit->GetMapMgr()->CreateAndSpawnGameObject( 209999, 3454.78, -5244.43, 229.949, 4.53786, 1.5f);
				if( tele )
					tele->PushToWorld( _unit->GetMapMgr() );

				_unit->SetFaction( 35 );	//make her friendly to everyone
			}
			return;
		}
		//get a random player that is not yet charmed
		Player *p = NULL;
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( !SafeUnitCast( *itr )->isAlive() )
				continue;
			if( SafeUnitCast( *itr )->HasAura( SPELL_ID_SERVANT_OF_THE_QUEEN_STUN ) != false )
				continue;
			p = SafePlayerCast( *itr );
			break;
		}
		_unit->ReleaseInrangeLock();
		if( p == NULL )
			return;
		if( ServantOfTheQueenTimer == 0 )
		{
			ServantOfTheQueenTimer = GetTickCount() + RandomUInt() % 25000;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			TalkIndex = 0;
			return;
		}

		uint32 TickNow = GetTickCount();
		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, QueenAzsharaYells[ TalkIndex ] );
			TalkIndex = ( TalkIndex + 1 ) % 19;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

		if( ServantOfTheQueenTimer < TickNow )
		{
			ServantOfTheQueenTimer = TickNow + (SPELL_COOL_SERVANT_OF_THE_QUEEN + RandomUInt() % SPELL_COOL_SERVANT_OF_THE_QUEEN);
//			_unit->CastSpell( p, SPELL_ID_SERVANT_OF_THE_QUEEN, false );	
			_unit->GetMapMgr()->CreateCreature( NPC_ID_SERVANT_OF_THE_QUEEN, true, p->GetPositionX(), p->GetPositionY(), p->GetPositionZ() + 1.5f );
		}
	}
};


class HandOfQueen : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(HandOfQueen);

   uint64 TargetGuid;
    HandOfQueen(Creature* pCreature) : CreatureAIScript(pCreature)
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
		TargetGuid = 0;
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
		}
		Player *p = NULL;
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( !SafeUnitCast( *itr )->isAlive() )
				continue;
			if( SafeUnitCast( *itr )->HasAura( SPELL_ID_SERVANT_OF_THE_QUEEN ) != false )
				continue;
			p = SafePlayerCast( *itr );
			break;
		}
		_unit->ReleaseInrangeLock();
		if( p == NULL )
			return;
		TargetGuid = p->GetGUID();
		if( p )
		{
			p->CastSpell( p, SPELL_ID_SERVANT_OF_THE_QUEEN_STUN, true );	//Total Obedience stun
			RemoveAIUpdateEvent();
		}
	}
	void OnDied(Unit *mKiller)
	{
		Player *p = _unit->GetMapMgr()->GetPlayer( TargetGuid );
		if( p )
			p->RemoveAura( SPELL_ID_SERVANT_OF_THE_QUEEN_STUN ); //Total Obedience stun
	}
};

class EnchantedMagusFrost : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(EnchantedMagusFrost);
	uint32  IceFlingTimer;
	uint32  ColdFlameTimer;
	uint32  BladesOfIceTimer;
	EnchantedMagusFrost(Creature* pCreature) : CreatureAIScript(pCreature){}
	void OnLoad()
	{
		IceFlingTimer = 0;
		RegisterAIUpdateEvent( 1000 );
	}
	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			IceFlingTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( IceFlingTimer == 0 )
		{
			IceFlingTimer = GetTickCount() + RandomUInt() % 25000;
			ColdFlameTimer = GetTickCount() + RandomUInt() % 25000;
			BladesOfIceTimer = GetTickCount() + RandomUInt() % 25000;
		}

		if( IceFlingTimer < TickNow && _unit->isCasting() == false )
		{
			IceFlingTimer = TickNow + (10000 + RandomUInt() % 10000);
			_unit->CastSpell( tank, 102478, false );	
		}
		if( ColdFlameTimer < TickNow && _unit->isCasting() == false )
		{
			ColdFlameTimer = TickNow + (15000 + RandomUInt() % 15000);
			_unit->CastSpell( tank, 102466, false );	
		}
		if( BladesOfIceTimer < TickNow && _unit->isCasting() == false )
		{
			BladesOfIceTimer = TickNow + (5000 + RandomUInt() % 5000);
			_unit->CastSpell( tank, 102468, false );	
		}
	}
};

class EnchantedMagusFire : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(EnchantedMagusFire);
	uint32  FireBallTimer;
	uint32  FireBombTimer;
	uint32  BlastWaveTimer;
	EnchantedMagusFire(Creature* pCreature) : CreatureAIScript(pCreature){}
	void OnLoad()
	{
		FireBallTimer = 0;
		RegisterAIUpdateEvent( 1000 );
	}
	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			FireBallTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( FireBallTimer == 0 )
		{
			FireBallTimer = GetTickCount() + RandomUInt() % 25000;
			FireBombTimer = GetTickCount() + RandomUInt() % 25000;
			BlastWaveTimer = GetTickCount() + RandomUInt() % 25000;
		}

		if( BlastWaveTimer < TickNow && _unit->isCasting() == false )
		{
			BlastWaveTimer = TickNow + (10000 + RandomUInt() % 10000);
			_unit->CastSpell( tank, 102482, false );	
		}
		if( FireBombTimer < TickNow && _unit->isCasting() == false )
		{
			FireBombTimer = TickNow + (15000 + RandomUInt() % 15000);
			_unit->CastSpell( tank, 102482, false );	
		}
		if( FireBallTimer < TickNow && _unit->isCasting() == false )
		{
			FireBallTimer = TickNow + (5000 + RandomUInt() % 5000);
			_unit->CastSpell( tank, 29925, false );	
		}
	}
};

class EnchantedMagusArcane : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(EnchantedMagusArcane);
	uint32  ArcaneShockTimer;
	uint32  ArcaneBombTimer;
	EnchantedMagusArcane(Creature* pCreature) : CreatureAIScript(pCreature){}
	void OnLoad()
	{
		ArcaneShockTimer = 0;
		RegisterAIUpdateEvent( 1000 );
	}
	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			ArcaneShockTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( ArcaneShockTimer == 0 )
		{
			ArcaneShockTimer = GetTickCount() + RandomUInt() % 25000;
			ArcaneBombTimer = GetTickCount() + RandomUInt() % 25000;
		}

		if( ArcaneBombTimer < TickNow && _unit->isCasting() == false )
		{
			ArcaneBombTimer = TickNow + (10000 + RandomUInt() % 10000);
			_unit->CastSpell( tank, 102455, false );	
		}
		if( ArcaneShockTimer < TickNow && _unit->isCasting() == false )
		{
			ArcaneShockTimer = TickNow + (5000 + RandomUInt() % 5000);
			_unit->CastSpell( tank, 102464, false );	
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_MAGISTRIKE		103669
#define SPELL_COOL_MAGISTRIKE	5000

char CaptainVarothenYells[5][71] = {
    "A deserved death!",
    "Highguard, to arms! For your queen! For Azshara!",
    "In Azshara's name!",
    "None may cross the Queen!",
    "For you, Azshara.",
};

class CaptainVarothen : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(CaptainVarothen);
/* 
    Captain Varo'then – Captain Varo'then attacks the players to prevent them from interfering with Mannoroth's concentration.
        Magistrike – Varo'then charges his sword with Arcane and Fire magics, dealing 27750 to 32250 Fire damage to an enemy and arcing to another nearby foe. Affects up to 3 total targets. 
*/
	uint32  MagistrikeTimer;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	CaptainVarothen(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		MagistrikeTimer = 0;
		RegisterAIUpdateEvent( 1000 );
	}
	void OnDied(Unit *mKiller)
	{
		_unit->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Light of lights...I have failed you. I am sorry...my Azshara..." );
	}

	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			MagistrikeTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( MagistrikeTimer == 0 )
		{
			MagistrikeTimer = GetTickCount() + RandomUInt() % 25000;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			TalkIndex = 0;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, CaptainVarothenYells[ TalkIndex ] );
			TalkIndex = ( TalkIndex + 1 ) % 5;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

		if( MagistrikeTimer < TickNow )
		{
			MagistrikeTimer = TickNow + (SPELL_COOL_MAGISTRIKE + RandomUInt() % SPELL_COOL_MAGISTRIKE);
			_unit->CastSpell( tank, SPELL_ID_MAGISTRIKE, false );	
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_FELSTORM		88972
#define SPELL_COOL_FELSTORM		15000
#define SPELL_ID_FELBLADE		103966
#define SPELL_COOL_FELBLADE		15000
#define SPELL_ID_FELDRAIN		104961
#define SPELL_COOL_FELDRAIN		5000
#define SPELL_ID_FELNOVA		105093	//this reduces Mannoroth health by 1%, 10 minutes battle ? 
#define SPELL_COOL_FELNOVA		6000
#define SPELL_ID_MAGISTRIKEARK	105523
#define NPC_ID_DOOMGUARD		57410
#define NPC_ID_DREADLORD		55762
#define NPC_ID_FELHOUND			56001
#define NPC_ID_FELGUARD			56002
#define NPC_ID_INFERNAL			56036

char MannorothYells[11][162] = {
	"Varo'then, see that I am not disrupted by this rabble!",
	"Come Stormrage, and I will show you what happens to those that betray the lord of the Legion!",
	"Your blood is MINE, elf!",
	"Fall and DIE before me!",
	"Lord Sargeras, I will not fail you! Sweep your molten fist through this world, so that it may be reborn in flames and darkness!",
	"No...no! This victory will not be ripped from my grasp! I will not return to him in failure! I will not be torn from this pitiful world! No...NOOOOOOOO!!!",
	"SQUIRM! SCREAM!",
	"Useless!",
	"Yes...yes! I can feel his burning eyes upon me, he is close...so close! And then your world will be unmade, your lives as nothing!",
	"Rrraaaghhh!!",
	"[Demonic] Amanare maev il azgalada zila ashj ashj zila enkil!",
};

class Mannoroth : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Mannoroth);
/* 
Players must defeat Captain Varo'then while Illidian and Tyrande fight off Mannoroth and the demonic forces entering from the Well of Eternity.
    Mannoroth – Mannaroth faces off in mortal combat against Illidan Stormrage.
        Fel Firestorm – Mannoroth unleashes a Fel Firestorm, rapidly creating 3 yard radius Fel Flames around the area for 12 sec. Fel Flames inflict 19500 to 20500 Fire damage every second to any player standing within them.
        Felblade – Mannoroth coats his mighty polearm in demonic fire, gaining a chance to deal 55500 to 64500 additional Fire damage when striking opponents in melee.
        Fel Drain – If the players inflict a significant amount of damage to Mannoroth while Captain Varo'then lives, Mannoroth sacrifices Captain Varo'then and heals himself to full health.
        Doomguard Devastator – These massive demons from the Well of Eternity seek to protect Mannoroth, but Tyrande stands in their way.
        Dreadlord Debilitator – These Dread Lords periodically enter from the Well of Eternity, then attempt to debilitate Tyrande.
            Debilitating Flay – The Debilitating Dreadlord disables Tyrande, preventing her from performing any actions and allowing Doomguards to swarm the area. 
    Captain Varo'then – Captain Varo'then attacks the players to prevent them from interfering with Mannoroth's concentration.
        Magistrike – Varo'then charges his sword with Arcane and Fire magics, dealing 27750 to 32250 Fire damage to an enemy and arcing to another nearby foe. Affects up to 3 total targets. 
    Illidan Stormrage – Illidan occupies Mannoroth in melee combat so the players are free to defeat Captain Varo'then.
        Darklance – Illidan conjures forth a spear of dark magical energy, piercing Mannoroth for 27975 to 32025 Shadow damage.
        Aura of Immolation – Illidan shrouds himself in magical flames, inflicting 20000 Fire damage to nearby enemies every 3 seconds.
        Demon Rush – Illidan charges at Mannoroth, coursing with Fel energies and inflicting 100% weapon damage to him.
        Demonic Sight – The Gift of Sargeras gives Illidan sight beyond sight, allowing him to dodge 90% of Mannoroth's attacks.
        Taunt – Illidan goads Mannoroth into focusing attacks on him.
        Waters of Eternity – After Mannoroth creates a Fel Firestorm, Illidan splashes himself with the waters of the Well of Eternity. The water provides substantial magical powers and reduces the Fire damage taken by all nearby allies by 90%. 
    Tyrande Whisperwind – Tyrande positions herself to intercept the rush of Doomguard Devastators exiting from the Well of Eternity with Lunar Shot.
        Lunar Shot – Tyrande holds off the demons exiting from the Well of Eternity with her Lunar Shot.
        Blessing of Elune – When a Dreadlord Debilitator casts Debilitating Flay upon her, Tyrande beseeches Elune for her blessing. This blessing affects all nearby allies, enchanting their attacks and spells to inflict additional Arcane damage to lesser demons. 
Stage Two: Flames and Darkness
After the players defeat Captain Varo'then, his enchanted Magistrike Blade lies on the ground. After a player throws and embeds the blade into Mannoroth, the demon begins to take massive damage from the stored magic within the blade.
    Mannoroth – The wounded Mannoroth continues to fight, opening a Nether Portal to bring demonic allies into the fray.
        Embedded Blade – Players may pick up Captain Varo'then's magical elven blade and throw it towards Mannoroth. The blade embeds itself into Mannoroth's demonic armor. The magical blade then causes strikes against Mannoroth to have a chance to create a Magistrike Arc inflicting 1000000 Fire damage to Mannoroth.
            Fel Fire Nova – After a player embeds the Magistrike blade within Mannoroth, the open wound emits a steady pulse of fel fire. Each pulse inflicts 27750 to 32250 Fire damage to enemies. 
        Nether Portal – Mannoroth summons a portal to the Twisting Nether, allowing him to call forth demonic allies. As his remaining health dwindles, additional types of demons exit through the portal.
            Felhound – At 85% remaining health, Mannoroth summons a weak felhound to aid him in combat from the Nether Portal.
            Felguard – At 70% remaining health, Mannoroth also summons a Felguard from the Nether Portal to aid him in combat.
            Doomguard Devastator – At 60% remaining health, additional Doomguard Devastators begin to pour from the Nether Portal.
            Infernal – At 50% remaining health, Mannoroth calls down a hail of Infernals to aid him in combat. 
    Illidan Stormrage – Illidan continues to fight Mannoroth with his host of Demon Hunter abilities.
    Tyrande Whisperwind – Tyrande runs out of arrows and beseeches Elune for help against the overwhelming number of demons.
        Hand of Elune – Tyrande is granted the full powers of the High Priestess of Elune, inflicting 27750 to 32250 Arcane damage to three neaby lesser demons every 3 seconds. 
Stage Three: Your World will be Unmade!
After Tyrande collapses upon destroying the rain of Infernals, Illidan then infuses himself to finish off Mannoroth.
    Mannoroth – Even heavily wounded Mannoroth continues to fight for his lord Sargeras.
        Embedded Blade – The embedded magical blade causes strikes against Mannoroth to have a chance to create a Magistrike Arc, inflicting 1000000 Fire damage to Mannoroth.
            Fel Fire Nova – After a player embeds the Magistrike blade within Mannoroth, the open wound emits a steady pulse of fel fire. Each pulse inflicts 27750 to 32250 Fire damage to enemies. 
    Illidan Stormrage – Illidan infuses himself with demonic energy to finish the fight.
        Gift of Sargeras – Illidan infuses himself and all nearby allies with the fel energies of the demon lord Sargeras, healing for 20% of maximum health every 2 seconds and dealing 30000 Fire damage to nearby enemies every 2 seconds. 
    Tyrande Whisperwind – Tyrande unleashes a massive blast of Arcane energy to destroy any nearby lesser demons, then collapses.
        Elune's Wrath – Tyrande unleases a blast of Arcane energy to destroy any nearby lesser demons. 
*/
	uint32  FelFirestormTimer;
	uint32  FelBladeTimer;
	uint32  FelNovaTimer;
	uint32  FelDrainTimer;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	uint32	phase;
	uint64	IllidanGUID;
	Mannoroth(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		FelFirestormTimer = 0;
		IllidanGUID = 0;
		RegisterAIUpdateEvent( 1000 );
	}

	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			FelFirestormTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( FelFirestormTimer == 0 )
		{
			FelFirestormTimer = GetTickCount() + RandomUInt() % 25000;
			FelBladeTimer = GetTickCount() + RandomUInt() % 25000;
			FelNovaTimer = GetTickCount() + RandomUInt() % 25000;
			FelDrainTimer = GetTickCount() + RandomUInt() % 25000;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			TalkIndex = 0;
			phase = 0;
			//spawn a dummy Illidan we will fight
			if( IllidanGUID == 0 )
			{
				Creature *Illidan = _unit->GetMapMgr()->CreateCreature( 55532, true, _unit->GetPositionX(), _unit->GetPositionY()-10.0f, _unit->GetPositionZ() + 1.5f );
				if( Illidan )
				{
					IllidanGUID = Illidan->GetGUID();
					Illidan->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_ID, 32065 );
					Illidan->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_ID_1, 32066 );
					Illidan->SetFaction( 1770 );
				}
			}
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, MannorothYells[ TalkIndex ] );
			TalkIndex = ( TalkIndex + 1 ) % 11;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);

		}

		//this is valid only in phase 1 ?
			if( FelBladeTimer < TickNow )
			{
				FelBladeTimer = TickNow + (SPELL_COOL_FELBLADE + RandomUInt() % SPELL_COOL_FELBLADE);
				_unit->CastSpell( _unit, SPELL_ID_FELBLADE, false );	
			}

		//phase when CaptainVarothen is alive
		if( phase < 2 )
		{
			if( FelFirestormTimer < TickNow )
			{
				FelFirestormTimer = TickNow + (SPELL_COOL_FELSTORM + RandomUInt() % SPELL_COOL_FELSTORM);
				_unit->CastSpell( tank, SPELL_ID_FELSTORM, false );	
			}
			if( FelDrainTimer < TickNow )
			{
				if( phase == 0 )
				{
					phase = 1;
					_unit->create_guardian( NPC_ID_DOOMGUARD, 60*10*1000 );
				}
				FelDrainTimer = TickNow + (SPELL_COOL_FELDRAIN + RandomUInt() % SPELL_COOL_FELDRAIN);
				Creature *CaptainVarothen = _unit->GetMapMgr()->GetCreatureByEntry( 55419 );
				if( CaptainVarothen == NULL )
				{
					FelDrainTimer = 0x7FFFFFFF;
					phase = 2;
				}
				else if( CaptainVarothen->isAlive() == false )
				{
					FelDrainTimer = 0x7FFFFFFF;
					phase = 2;
				}
				else if( ( CaptainVarothen->GetHealthPct() < 5 && _unit->GetHealthPct() < 100 ) //case when the captain is dieing
					|| _unit->GetHealthPct() < 80 ) //case when the boss is dieing
				{
					CaptainVarothen->SetHealth( 1 );
					_unit->CastSpell( CaptainVarothen, SPELL_ID_FELDRAIN, false );	
					FelDrainTimer = 0x7FFFFFFF;
					phase = 2;
				}
			}
		}
		//phase when CaptainVarothen is dead
		if( phase >= 2 )
		{
			if( FelNovaTimer < TickNow )
			{
				FelNovaTimer = TickNow + (SPELL_COOL_FELNOVA + RandomUInt() % SPELL_COOL_FELNOVA);
				_unit->CastSpell( _unit, SPELL_ID_MAGISTRIKEARK, true );	//does 2% health dmg
				_unit->CastSpell( tank, SPELL_ID_FELNOVA, false );	
				//try to avoid exploit when Illidan will solo mannorath
				Creature *ili = _unit->GetMapMgr()->GetCreature( IllidanGUID );
				if( ili )
					_unit->DealDamage( ili, tank->GetMaxHealth() * 3 / 100, SPELL_ID_FELNOVA );
			}
			if( phase == 2 && _unit->GetHealthPct() <= 85 )
			{
				_unit->create_guardian( NPC_ID_FELHOUND, 60*10*1000 );
				phase = 3;
			}
			else if( phase == 3 && _unit->GetHealthPct() <= 70 )
			{
				_unit->create_guardian( NPC_ID_FELGUARD, 60*10*1000 );
				phase = 4;
			}
			else if( phase == 4 && _unit->GetHealthPct() <= 60 )
			{
				_unit->create_guardian( NPC_ID_DOOMGUARD, 60*10*1000 );
				_unit->create_guardian( NPC_ID_DOOMGUARD, 60*10*1000 );
				phase = 5;
			}
			else if( phase == 5 && _unit->GetHealthPct() <= 50 )
			{
				_unit->create_guardian( NPC_ID_INFERNAL, 60*10*1000 );
				_unit->create_guardian( NPC_ID_INFERNAL, 60*10*1000 );
				_unit->create_guardian( NPC_ID_INFERNAL, 60*10*1000 );
				_unit->create_guardian( NPC_ID_INFERNAL, 60*10*1000 );
				phase = 6;
			}
		}
	}
};

#define SPELL_ID_DARKLANCE		104394
#define SPELL_COOL_DARKLANCE	10000
#define SPELL_ID_IMMOLATION		104379
#define SPELL_COOL_IMMOLATION	20000
#define SPELL_ID_RUSH			104205
#define SPELL_COOL_RUSH			10000
#define SPELL_ID_DEMONIC_SIGHT	104746
#define SPELL_ID_TAUNT			104461
#define SPELL_ID_WATERS_OF_ETERNITY			103952
#define SPELL_ID_GIFT_OF_SARGERAS			104998


char IllidanYells[40][303] = {
    "He is still connected to the Well somehow! Focus your attacks on Mannoroth, we must disrupt his concentration!",
    "I will be the savior of our people! I WILL FULFILL MY DESTINY!",
    "The artifact!",
    "The sword has pierced his infernal armor! Strike him down!",
    "You are not the sole wielder of Sargeras' power, Mannoroth! Behold!",
    "[Demonic] Revos ill ok mordanas archim maz naztheros! Archim xi ante maz-re mishun te nagas!",
    "Another demon, ready to be slaughtered.",
    "Are you here to help?",
    "Attack. I don't like to be kept waiting.",
    "Aye. It's been twisted and turned by too many spells. The fuss we - especially you - made with the portal was too much! The same spell that sent the Burning Legion back into their foul realm now works on the well! It's devouring itself and taking its surroundings with it! Fascinating, isn't it?",
    "Brother. A timely arrival...",
    "Come with me if you'd like to live long enough to see me save this world!",
    "Cut this one down from the shadows.",
    "Death to the Legion!",
    "Explain your presence.",
    "Handle Varo'then. Mannoroth is mine.",
    "I think we stand a better chance fighting alongside one another.",
    "I'll let you have the first kill. Don't make me regret that.",
    "If you've a way out of here, we should probably use it! I've tried casting myself and Tyrande out of here, but the well is too much in flux!",
    "Let them come.",
    "My blades hunger.",
    "My strength returns.",
    "Nothing will stop me. Not even you, demon.",
    "Oh this will be fun...",
    "Patience is not one of my virtues.",
    "Return to the shadows!",
    "So many demons, not enough time.",
    "Talk with me now.",
    "The hunter became the prey.",
    "The stench of sulfur and brimstone... These portals are as foul as the demons themselves.",
    "They come endlessly from the palace.",
    "They stand unaware that their deaths are moments away.",
    "They will get what they deserve, in due time.",
    "This won't hurt a bit.",
    "Too easy.",
    "Very well, we shall break it for you.",
    "Weak, pitiful creatures. Hardly worthy of being called a legion.",
    "What our people could not.",
    "Yes...YES. I can feel the raw power of the Well of Eternity! It is time to end this charade!",
    "Your magic is pathetic. Let me show you mine.",
};

class IllidanStormRage : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(IllidanStormRage);
/* 
	Just tank mannoroth
*/
	uint32  DarklanceTimer;
	uint32  ImmolationTimer;
	uint32  RushTimer;
	uint64	Mannoroth;
	uint32	phase;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	IllidanStormRage(Creature* pCreature) : CreatureAIScript(pCreature){}
	void OnLoad()
	{
		DarklanceTimer = 0;
		Mannoroth = 0;
		phase = 0;
		_unit->GetAIInterface()->disable_targeting = true;
		//Mannoroth
		Creature *c = _unit->GetMapMgr()->GetCreatureByEntry( 54969 );
		if( c )
		{
			Mannoroth = c->GetGUID();
			_unit->GetAIInterface()->AttackReaction( c, 0x0FFFFFFF );
			RegisterAIUpdateEvent( 1000 );
			_unit->CastSpell( c, SPELL_ID_TAUNT, true );	
			_unit->CastSpell( _unit, SPELL_ID_DEMONIC_SIGHT, true );	
		}
	}

	void AIUpdate()
    {
		Unit *tank = _unit->GetMapMgr()->GetCreature( Mannoroth );
		if( tank == NULL )
		{
			DarklanceTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( DarklanceTimer == 0 )
		{
			DarklanceTimer = GetTickCount() + RandomUInt() % 25000;
			ImmolationTimer = GetTickCount() + RandomUInt() % 25000;
			RushTimer = GetTickCount() + RandomUInt() % 25000;
			phase = 0;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			TalkIndex = 0;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, IllidanYells[ TalkIndex ]);
			TalkIndex = ( TalkIndex + 1 ) % 40;
			TalkCooldown = TickNow + (5000 + RandomUInt() % 5000);
		}

		if( DarklanceTimer < TickNow )
		{
			DarklanceTimer = TickNow + (SPELL_COOL_DARKLANCE + RandomUInt() % SPELL_COOL_DARKLANCE);
			_unit->CastSpell( tank, SPELL_ID_DARKLANCE, false );	
		}
		if( ImmolationTimer < TickNow )
		{
			ImmolationTimer = TickNow + (SPELL_COOL_IMMOLATION + RandomUInt() % SPELL_COOL_IMMOLATION);
			_unit->CastSpell( tank, SPELL_ID_IMMOLATION, false );	
			//we need this bullshit or Illidan will solo Mannorath
			tank->GetAIInterface()->RemoveThreatByPtr( _unit );
		}
		if( RushTimer < TickNow )
		{
			RushTimer = TickNow + (SPELL_COOL_RUSH + RandomUInt() % SPELL_COOL_RUSH);
			_unit->CastSpell( tank, SPELL_ID_RUSH, false );	
		}
		if( tank->GetHealthPct() < 20 && phase == 0 )
		{
			_unit->CastSpell( _unit, SPELL_ID_GIFT_OF_SARGERAS, true );	
			InRangePlayerSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( !SafeUnitCast( *itr )->isAlive() )
					continue;
				_unit->CastSpell( ( *itr ), SPELL_ID_GIFT_OF_SARGERAS, true );	
			}
			_unit->ReleaseInrangeLock();
			_unit->CastSpell( _unit, SPELL_ID_WATERS_OF_ETERNITY, true );	
			phase = 1;
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SCRIPT_DECL WellOfEternityTeleporter : public GossipScript
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

		Menu->AddItem(GOSSIP_ICON_CHAT, "Teleport to the other side", 3);

		if(AutoSend)
			Menu->SendTo(Plr);
	};
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char *Code)
	{
		if(IntId == 3 )
			Plr->SafeTeleport( Plr->GetMapId(), Plr->GetInstanceID(), 3062, -5566, 19, 0.0f ); 
		Plr->CloseGossip();
	};
	void GossipEnd(Object* pObject, Player* pPlayer)
	{
		GossipScript::GossipEnd(pObject, pPlayer);
	}
};

//tel to : 3060 -5564 18
// hammer of divinity casts arcane bomb to make the whole screen shine
// spawn 54728 - hand of the queen for the charm
// heaven of divinity can also have small size shine
// piercing thorns add proc
// enbeded blade 55838
void SetupWellOfEternity(ScriptMgr* mgr)
{
	SpellEntry *sp;
	/*
	delete from ai_agents where entry in ( 55085, 57329, 54853, 54882, 54883, 54884, 55532 );
	delete from loot_creature where entry = 54853; #-> need to move loot to gamobject with sound1=40353
	INSERT INTO `loot_gameobjects` (entryid, `itemid`, percentchance, heroicpercentchance) VALUES
	('40353', 71635, 16.607, 16.607),
	('40353', 72833, 27.2437, 27.2437),
	('40353', 72834, 22.5901, 22.5901),
	('40353', 72835, 22.2066, 22.2066),
	('40353', 72836, 23.0759, 23.0759),
	('40353', 72837, 14.7277, 14.7277),
	('40353', 72838, 23.1015, 23.1015);
	*/
	sp = dbcSpell.LookupEntryForced( SPELL_ID_CORUPTING_TOUCH );
	//if( sp != NULL )
	{
		sp->procChance = 100;	
		sp->proc_interval = 1000;
		sp->procFlags = PROC_ON_MELEE_ATTACK;
	}
	sp = dbcSpell.LookupEntryForced( 108101 );
	//if( sp != NULL )
		sp->rangeIndex = 2;//5 yards
	sp = dbcSpell.LookupEntryForced( SPELL_ID_FEL_DECAY );	
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[2].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
	}
		
	mgr->register_creature_script(55085, &Perotharn::Create);
	mgr->register_creature_script(57329, &FelFlames::Create);
	////////////////////////////
	mgr->register_creature_script(54853, &QueenAzshara::Create);
	mgr->register_creature_script(54728, &HandOfQueen::Create);
	mgr->register_creature_script(54882, &EnchantedMagusFire::Create);
	mgr->register_creature_script(54883, &EnchantedMagusFrost::Create);
	mgr->register_creature_script(54884, &EnchantedMagusArcane::Create);
	sp = dbcSpell.LookupEntryForced( SPELL_ID_SERVANT_OF_THE_QUEEN_STUN );	//Total Obedience
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_DUEL;
		sp->DurationIndex = 6;	//10 minutes ?
	}
	////////////////////////////
	GossipScript * gs = (GossipScript*) new WellOfEternityTeleporter();
	mgr->register_go_gossip_script(209999, gs);	//Time Transit Device
	////////////////////////////
	mgr->register_creature_script(55419, &CaptainVarothen::Create);
	////////////////////////////
	mgr->register_creature_script(54969, &Mannoroth::Create);	
	mgr->register_creature_script(55532, &IllidanStormRage::Create);	
	sp = dbcSpell.LookupEntryForced( SPELL_ID_FELBLADE );
	//if( sp != NULL )
	{
		sp->procChance = 100;	
		sp->proc_interval = 1000;
		sp->procFlags = PROC_ON_MELEE_ATTACK;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_GIFT_OF_SARGERAS );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_DUEL;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_DUEL;
		sp->eff[2].EffectImplicitTargetA = EFF_TARGET_DUEL;
		//debugging why the hell this gets removed almost instantly
		sp->rangeIndex = 13;//Anywhere
	}
	sp = dbcSpell.LookupEntryForced( 105006 );	//Fel Immolation -> periodic trigger from SPELL_ID_GIFT_OF_SARGERAS
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		if( sp->eff[0].EffectRadiusIndex == 0 || sp->eff[0].EffectRadiusIndex == 36 )
			sp->eff[0].EffectRadiusIndex = 8;	//5 yards
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_WATERS_OF_ETERNITY );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_FRIENDLY_IN_AREA;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_ALL_FRIENDLY_IN_AREA;
		sp->eff[2].EffectImplicitTargetA = EFF_TARGET_ALL_FRIENDLY_IN_AREA;
		sp->DurationIndex = dbcSpell.LookupEntryForced( SPELL_ID_GIFT_OF_SARGERAS )->DurationIndex;
	}
}