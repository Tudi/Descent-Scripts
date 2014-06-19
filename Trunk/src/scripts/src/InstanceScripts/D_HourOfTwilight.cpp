#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305) // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_HAND_OF_FROST		102593
#define SPELL_COOL_HAND_OF_FROST	5*1000
#define SPELL_ID_CHAINS_OF_FROST	102582
#define SPELL_COOL_CHAINS_OF_FROST	12*1000
#define NPC_ID_FROZEN_SERVITOR		54600
#define SPELL_COOL_FROZEN_SERVITOR	10*1000
#define SPELL_ID_ICY_BOULDER		102199
#define SPELL_ID_ICY_TOMB_STUN		103251
#define NPC_ID_ICY_TOMB				54995
#define SPELL_COOL_ICY_TOMB			45*1000	//thrall kills 4 servitor
#define SPELL_ID_TORRENT_OF_FROST	104050

char Arcurionyells[14][108] = {
   "Destroy them all, but bring the Shaman to me!",
   "Enough, Shaman!",
   "Mere mortals.",
   "None will survive!",
   "Shaman! The Demon Soul is not yours. Give it up, and you may yet walk away with your life.",
   "The Aspects misplaced their trust.",
   "The Shaman is mine, focus on his companions!",
   "Shaman! The Dragon Soul is not yours. Give it up, and you may yet walk away with your life.",
   "You will go nowhere, Shaman.",
   "You're a mere mortal. It is time you died like one.",
   "Nothing! Nothing....",
   "You're a fool if you think to take your place as the Aspect of Earth, shaman!",
   "Your Shaman can't protect you.",
   "The Hour of Twilight falls - the end of all things - you can't stop it. You are nothing. NOTHING!",
};

class Arcurion : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Arcurion);
/* 
    Hand of Frost – Arcurion's icy grip reaches out at his current target, inflicting 45000 to 55000 Frost damage.
    Chains of Frost – Arcurion unleashes a wave of ice, inflicting 9250 to 10750 Frost damage and immobilizing all enemies within 65 yards for 10 sec.
    Frozen Servitor – Arcurion's minions surround the canyon and throw Icy Boulders onto random players below.
        Icy Boulder – A Frozen Servitor assaults a random player with huge blocks of ice, inflicting 19000 to 21000 Frost damage. 
    Thrall – Thrall hurls molten lava at the Frozen Servitors above, keeping their numbers in check.
    ImportantIcy Tomb – Arcurion encases Thrall in an Icy Tomb. Destroying the Icy Tomb releases Thrall.
    Torrent of Frost – At 30% remaining health, Arcurion unleashes a devastating Torrent of Frost inflicting 13500 to 16500 Frost damage each second to all enemies and reducing their movement speed by 50% for 5 sec. 
*/
	uint32  HandOfFrostTimer;
	uint32  ChainsOfFrostTimer;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	uint32  FrozenServitorTimer;
	uint32  IcyTombTimer;
	uint32	phase;
	Arcurion(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		HandOfFrostTimer = 0;
		RegisterAIUpdateEvent( 1000 );
	}
	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			HandOfFrostTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( HandOfFrostTimer == 0 )
		{
			HandOfFrostTimer = GetTickCount();
			ChainsOfFrostTimer = GetTickCount() + RandomUInt() % 25000;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			FrozenServitorTimer = GetTickCount() + RandomUInt() % 25000;
			IcyTombTimer = GetTickCount() + RandomUInt() % 25000;
			phase = 0;
			TalkIndex = 0;
		}

		if( phase == 0 && _unit->GetHealthPct() <= 10 )
		{
			_unit->CastSpell( _unit, SPELL_ID_TORRENT_OF_FROST, false );	
			phase = 1;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, Arcurionyells[ TalkIndex ]);
			TalkIndex = ( TalkIndex + 1 ) % 14;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

		if( HandOfFrostTimer < TickNow && _unit->isCasting() == false )
		{
			HandOfFrostTimer = TickNow + (SPELL_COOL_HAND_OF_FROST + RandomUInt() % SPELL_COOL_HAND_OF_FROST);
			_unit->CastSpell( tank, SPELL_ID_HAND_OF_FROST, false );	
		}
		if( ChainsOfFrostTimer < TickNow && _unit->isCasting() == false )
		{
			ChainsOfFrostTimer = TickNow + (SPELL_COOL_CHAINS_OF_FROST + RandomUInt() % SPELL_COOL_CHAINS_OF_FROST);
			_unit->CastSpell( tank, SPELL_ID_CHAINS_OF_FROST, false );	
		}
		if( FrozenServitorTimer < TickNow && _unit->isCasting() == false )
		{
			FrozenServitorTimer = TickNow + (SPELL_COOL_FROZEN_SERVITOR + RandomUInt() % SPELL_COOL_FROZEN_SERVITOR);
			_unit->create_guardian( NPC_ID_FROZEN_SERVITOR, 10*60*1000 );	
		}
		if( IcyTombTimer < TickNow && _unit->isCasting() == false )
		{
			IcyTombTimer = TickNow + (SPELL_COOL_ICY_TOMB + RandomUInt() % SPELL_COOL_ICY_TOMB);
			//get thrall ID
			Creature *Thrall = NULL;
			InRangeSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
			{
				itr = itr2;
				itr2++;
				if( ( *itr )->GetEntry() != 54548 || !SafeUnitCast( *itr )->isAlive() ) //get Thrall
					continue;
				Thrall = SafeCreatureCast( *itr );
				break;
			}
			_unit->ReleaseInrangeLock();

			if( Thrall )
			{
//				_unit->CastSpell( Thrall, SPELL_ID_ICY_TOMB, true );	
				_unit->GetMapMgr()->CreateCreature( NPC_ID_ICY_TOMB, true, Thrall->GetPositionX(), Thrall->GetPositionY(), Thrall->GetPositionZ() );
				_unit->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Begins to freeze Thrall in a block of ice.");
			}
		}
	}
};

class FrozenServitor : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(FrozenServitor);

    FrozenServitor(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 2000 );
    }
	void OnLoad()
	{
		AI_Spell sp;
		sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
		sp.minrange = 1.0f;
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
//		_unit->CastSpell( tank, 107848 , false );		//maybe visual ?
		_unit->CastSpell( tank, SPELL_ID_ICY_BOULDER , false );	
	}
};

char Thrallyells[9][172] = {
    "It is done; at last the Destroyer has met his end. Now, we can begin to heal our world.",
    "Strength of the Earth, hear my call! Shield them in this dark hour, the last defenders of Azeroth!",
    "Heroes, this burden falls to you once again.",
    "It is no use, the power of the Dragon Soul is too great. I cannot wield it safely; the raging forces contained within it may be the doom of us all.",
    "Speak to me when you are ready to begin.",
    "Taretha... Cairne... Aggra... I will not fail you. I will not fail this world!",
    "We have no choice. We must have faith in our allies.",
    "Yes, I see. We can finally turn Deathwing's own power back against him.",
    "You must protect us from Deathwing's forces while we imbue the Dragon Soul with the power of the Aspects.",
};

class Thrall : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Thrall);

    Thrall(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 3000 );
    }
	uint32	TalkIndex;
	uint32	TalkCooldown;
	void OnLoad()
	{
		TalkIndex = 0;
		TalkCooldown = 0;
	}
	void AIUpdate()
	{
		//we should be near Arcurion. Trying to avoid exploiting Thrall in multiple ways
		if( Distance2DSq( 4764, 61, _unit->GetPositionX(), _unit->GetPositionY() ) < 70 * 70 )
		{
			Unit *tank = _unit->GetAIInterface()->GetNextTarget();
			if( tank == NULL || tank->GetEntry() != NPC_ID_FROZEN_SERVITOR )
			{
				InRangeSetRecProt::iterator itr,itr2;
				_unit->AquireInrangeLock(); //make sure to release lock before exit function !
				InrangeLoopExitAutoCallback AutoLock;
				for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
				{
					itr = itr2;
					itr2++;
					if( ( *itr )->GetEntry() != NPC_ID_FROZEN_SERVITOR || !SafeUnitCast( *itr )->isAlive() ) 
						continue;
					tank = SafeCreatureCast( *itr );
					break;
				}
				_unit->ReleaseInrangeLock();
			}
			if( tank == NULL )
				return;

			//someone agroed us, but we do not care !
			if( tank->GetEntry() != NPC_ID_FROZEN_SERVITOR )
			{
				_unit->GetAIInterface()->WipeHateList();
				return;
			}

			if( _unit->HasAura( SPELL_ID_ICY_TOMB_STUN ) )
				return;

			uint32 TickNow = GetTickCount();

			if( TalkCooldown < TickNow )
			{
				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, Thrallyells[ TalkIndex ]);
				TalkIndex = ( TalkIndex + 1 ) % 9;
				TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
			}

			_unit->CastSpell( tank, 102475 , false );	//Lava Burst
		}
	}
};

class IcyTomb : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(IcyTomb);

    IcyTomb(Creature* pCreature) : CreatureAIScript(pCreature)
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
		//get thrall ID
		Creature *Thrall = NULL;
		InRangeSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
		{
			itr = itr2;
			itr2++;
			if( ( *itr )->GetEntry() != 54548 || !SafeUnitCast( *itr )->isAlive() ) //get Thrall
				continue;
			Thrall = SafeCreatureCast( *itr );
			break;
		}
		_unit->ReleaseInrangeLock();
		if( Thrall )
		{
			Thrall->CastSpell( Thrall, SPELL_ID_ICY_TOMB_STUN, true );
			RemoveAIUpdateEvent();
		}
	}
	void OnDied(Unit *mKiller)
	{
		//get thrall ID
		Creature *Thrall = NULL;
		InRangeSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
		{
			itr = itr2;
			itr2++;
			if( ( *itr )->GetEntry() != 54548 || !SafeUnitCast( *itr )->isAlive() ) //get Thrall
				continue;
			Thrall = SafeCreatureCast( *itr );
			break;
		}
		_unit->ReleaseInrangeLock();
		if( Thrall )
			Thrall->RemoveAura( SPELL_ID_ICY_TOMB_STUN );
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_MARK_OF_SILENCE	102726
#define SPELL_ID_THROW_KNIFE		103597
#define SPELL_COOL_MARK_OF_SILENCE	10*1000
//#define SPELL_ID_SMOKE_BOMB			103558
//#define SPELL_COOL_SMOKE_BOMB		22*1000
#define SPELL_ID_BLADE_BARRIER			103419
#define SPELL_ID_LESSER_BLADE_BARRIER	103562
#define SPELL_COOL_BLADE_BARRIER		30*1000

char AsiraDawnslayeryells[12][320] = {
    "...and with that out of the way, you and your flock of fumbling friends are next on my list. Mmm, I thought you'd never get here!",
    "Bye-bye.",
    "Let's get to work, shall we?",
    "Now you see me...",
    "Surprise.",
    "Where do you think you're going, little lizard?",
    "You're much... better... than I thought...",
    "Ah. Well. That was even easier than I thought!",
    "I hope your friends can do better.",
    "Mmm, too much fun!",
    "Tch! So soon?",
    "Good night...",
};

class AsiraDawnslayer : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(AsiraDawnslayer);
/* 
    Mark of Silence – Asira marks a random enemy spellcaster for 20 sec. When a marked target casts a spell, Asira throws a knife in their direction.
    Throw Knife – Asira throws one of her knives toward a marked enemy spellcaster, inflicting 15000 Physical damage to the first player it collides with. If the knife hits a player afflicted with Mark of Silence, it also silences for 2 sec. 
    Choking Smoke Bomb – Asira creates a cloud of thick smoke in an 8 yard radius around her for 20 sec. The cloud prevents enemies from targeting into or out of the smoke cloud and inflicts 5000 Nature damage every 0.5 sec.
    Blade Barrier – At 30% remaining health, Asira begins a dazzling display of weapon mastery. Her Blade Barrier reduces any incoming amount of damage below 40000 to 1. The Blade Barrier ends when a player hits Asira with a damaging attack or spell above this threshold.
        Lesser Blade Barrier – Asira erects this weaker barrier when a player removes Blade Barrier. The Lesser Blade Barrier reduces any incoming amount of damage below 30000 to 1. The Lesser Blade Barrier ends when a player hits Asira with a damaging attack or spell above this threshold. 
    Thrall – Thrall assists the players in combat against Asira.
        Lava Burst – Thrall hurls molten lava at the target, inflicting 43750 to 56250 Fire damage.
        Rising Fire Totem – Thrall summons a Fire Totem that periodically increases the damage and maximum health of nearby players. The totem lasts 20 sec and its effect can stack up to 10 times.
        Healing Wave – Thrall calls upon Nature magic to heal himself. 
*/
	uint32  MarkOfSilenceTimer;
//	uint32  ChokingBombTimer;
	uint32  BarrierTimer;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	uint32	phase;
	AsiraDawnslayer(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		MarkOfSilenceTimer = 0;
		RegisterAIUpdateEvent( 1000 );
	}
	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			MarkOfSilenceTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( MarkOfSilenceTimer == 0 )
		{
			MarkOfSilenceTimer = GetTickCount() + RandomUInt() % 25000;
//			ChokingBombTimer = GetTickCount() + RandomUInt() % 25000;
			BarrierTimer = GetTickCount() + RandomUInt() % 25000;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			phase = 0;
			TalkIndex = 0;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, AsiraDawnslayeryells[ TalkIndex ]);
			TalkIndex = ( TalkIndex + 1 ) % 12;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

/*		if( phase == 0 && _unit->GetHealthPct() <= 30 )
		{
			_unit->CastSpell( tank, SPELL_ID_BLADE_BARRIER, false );
			phase = 1;
		}
		if( phase == 1 && _unit->HasAura( SPELL_ID_BLADE_BARRIER ) == false )
		{
			_unit->CastSpell( tank, SPELL_ID_LESSER_BLADE_BARRIER, false );
			phase = 2;
		}*/

		if( BarrierTimer < TickNow && _unit->isCasting() == false && _unit->GetHealthPct() <= 30  )
		{
			BarrierTimer = TickNow + (SPELL_COOL_BLADE_BARRIER + RandomUInt() % SPELL_COOL_BLADE_BARRIER);
			_unit->CastSpell( tank, SPELL_ID_BLADE_BARRIER, false );	
		}
		if( MarkOfSilenceTimer < TickNow && _unit->isCasting() == false )
		{
			MarkOfSilenceTimer = TickNow + (SPELL_COOL_MARK_OF_SILENCE + RandomUInt() % SPELL_COOL_MARK_OF_SILENCE);
			_unit->CastSpell( tank, SPELL_ID_MARK_OF_SILENCE, false );	
			_unit->CastSpell( tank, SPELL_ID_THROW_KNIFE, true );	
		}
/*		if( ChokingBombTimer < TickNow && _unit->isCasting() == false )
		{
			ChokingBombTimer = TickNow + (SPELL_COOL_SMOKE_BOMB + RandomUInt() % SPELL_COOL_SMOKE_BOMB);
			_unit->CastSpell( tank, SPELL_ID_SMOKE_BOMB, false );	
		}*/
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_RIGHTOUS_SHEAR		103161
#define SPELL_ID_PURIFYING_BLAST	103651
#define SPELL_ID_WAVE_OF_VIRTUE		103684
#define SPELL_ID_TWILIGHT_SHEAR		103526
#define SPELL_ID_TWILIGHT_BLAST		104504
#define SPELL_ID_WAVE_OF_TWILIGHT	103781
#define COOL_SHEAR		10000
#define COOL_BLAST		15000
#define COOL_WAVE		25000

char ArchbishopBenedictusyells[12][104] = {
    "And YOU! We will FEAST on your ashes! Now, DIE!",
    "Drown in Shadow!",
    "I looked into the eyes of The Dragon, and despaired...",
    "Let your faith guide you.",
    "Light be with you.",
    "The Light will consume you!",
    "Witness my POWER!",
    "And now, Shaman, you will give the Dragon Soul to ME.",
    "Get inside, quickly! I'll hold them off!",
    "I suppose it has to be this way, then. If only you'd seen what I've seen. THEN you'd understand.",
    "There is no good. No evil. No Light. There is only POWER!",
    "We serve the world's TRUE masters! When their rule begins, we will share in their glory!"
};

class ArchbishopBenedictus : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(ArchbishopBenedictus);
/* 
Stage One: The Light Will Consume You!
Benedictus assaults Thrall with the powers of the Holy Light.
    Righteous Shear – Archbishop Benedictus targets a random player with Righteous Shear, inflicting Holy damage to players within 10 yards of the target per stack of Righteous Shear.
    Purifying Light – Archbishop Benedictus forms three orbs of Purifying Light and raises them above his head, sending them to assault a nearby player and causing a Purifying Blast.
        Purifying Blast – Archbishop Benedictus' Purifying Blast inflicts 48750 to 51250 Holy damage to players within 6 yards. 
    Wave of Virtue – Archbishop Benedictus summons a Wave of Virtue to sweep across the platform, inflicting 97500 to 102500 Holy damage to nearby players and knocking them back. 
Stage Two: Drown in Shadow!
At 60% remaining health Archbishop Benedictus casts Twilight Epiphany, showing his true form, transforming into an agent of the Twilight and trapping Thrall in a Twilight Prison.
    Twilight Shear – Archbishop Benedictus targets a random player with Twilight Shear, inflicting Shadow damage to players within 10 yards of the target per stack of Twilight Shear.
    Corrupting Twilight – Archbishop Benedictus forms three orbs of Corrupting Twilight and raises them above his head, sending them to assault a nearby player and causing a Twilight Blast.
        Twilight Blast – Archbishop Benedictus' Twilight Blast inflicts 48750 to 51250 Shadow damage to players within 6 yards. 
    Wave of Twilight – Archbishop Benedictus summons a Wave of Twilight to sweep across the platform, inflicting 97500 to 102500 Shadow damage to nearby players, knocking them back. 
*/
	uint32  ShearTimer;
	uint32  BlastTimer;
	uint32  WaveTimer;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	uint32	phase;
	ArchbishopBenedictus(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
	void OnLoad()
	{
		ShearTimer = 0;
		RegisterAIUpdateEvent( 1000 );
	}
	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			ShearTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( ShearTimer == 0 )
		{
			ShearTimer = GetTickCount() + RandomUInt() % 25000;
			BlastTimer = GetTickCount() + RandomUInt() % 25000;
			WaveTimer = GetTickCount() + RandomUInt() % 25000;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			phase = 0;
			TalkIndex = 0;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, ArchbishopBenedictusyells[ TalkIndex ]);
			TalkIndex = ( TalkIndex + 1 ) % 12;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

		if( _unit->GetHealthPct() >= 60 )
		{
			if( ShearTimer < TickNow && _unit->isCasting() == false )
			{
				ShearTimer = TickNow + (COOL_SHEAR + RandomUInt() % COOL_SHEAR);
				_unit->CastSpell( tank, SPELL_ID_RIGHTOUS_SHEAR, false );	
			}
			if( BlastTimer < TickNow && _unit->isCasting() == false )
			{
				BlastTimer = TickNow + (COOL_BLAST + RandomUInt() % COOL_BLAST);
				_unit->CastSpell( tank, SPELL_ID_PURIFYING_BLAST, false );	
			}
			if( WaveTimer < TickNow && _unit->isCasting() == false )
			{
				WaveTimer = TickNow + (COOL_WAVE + RandomUInt() % COOL_WAVE);
				_unit->CastSpell( tank, SPELL_ID_WAVE_OF_VIRTUE, false );	
			}
		}
		else
		{
			if( ShearTimer < TickNow && _unit->isCasting() == false )
			{
				ShearTimer = TickNow + (COOL_SHEAR + RandomUInt() % COOL_SHEAR);
				_unit->CastSpell( tank, SPELL_ID_TWILIGHT_SHEAR, false );	
			}
			if( BlastTimer < TickNow && _unit->isCasting() == false )
			{
				BlastTimer = TickNow + (COOL_BLAST + RandomUInt() % COOL_BLAST);
				_unit->CastSpell( tank, SPELL_ID_TWILIGHT_BLAST, false );	
			}
			if( WaveTimer < TickNow && _unit->isCasting() == false )
			{
				WaveTimer = TickNow + (COOL_WAVE + RandomUInt() % COOL_WAVE);
				_unit->CastSpell( tank, SPELL_ID_WAVE_OF_TWILIGHT, false );	
			}
		}
	}
};


class ThrallBenedict : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(ThrallBenedict);

    ThrallBenedict(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent( 3000 );
    }
	uint32	TalkIndex;
	uint32	TalkCooldown;
	void OnLoad()
	{
		TalkIndex = 0;
		TalkCooldown = 0;
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
	}
	void AIUpdate()
	{
		//we should be near Archbishop Benedictus. Trying to avoid exploiting Thrall in multiple ways
//		if( Distance2DSq( 3549, 275, _unit->GetPositionX(), _unit->GetPositionY() ) < 70 * 70 )
		{
			Unit *tank = _unit->GetAIInterface()->GetNextTarget();
			InRangePlayerSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( !SafeUnitCast( *itr )->isAlive() || ( *itr )->IsPlayer() == false )
					continue;
				_unit->CastSpell( ( *itr ), 103744 , false );	//Water Shell
			}
			_unit->ReleaseInrangeLock();
		}
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SCRIPT_DECL HourOfTwilightTeleporter : public GossipScript
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
		uint32 boss_alive = 0;
		for(uint32 i=0;i<Plr->GetMapMgr()->m_CreatureArraySize;i++)
		{
			Creature *tc = Plr->GetMapMgr()->m_CreatureStorage[i];
			if(	tc )
			{
				uint32 entry = tc->GetEntry();
				if( entry == 54590 || entry == 54968 )
				{
					if( tc->isAlive() == false )
						boss_kills++;
					else
						boss_alive++;
				}
			}
		}

		if( boss_kills >= 1 )
			Menu->AddItem(GOSSIP_ICON_CHAT, "Teleport to Archbishop Benedictus", 1);
		else
			Menu->AddItem(GOSSIP_ICON_CHAT, "You should kill someone before talking to me", 2);
	    
		if(AutoSend)
			Menu->SendTo(Plr);
	};
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char *Code)
	{
		if( IntId == 2 )
			Plr->SafeTeleport(940, Plr->GetInstanceID(), 3944, 290, 14, 3.2 ); 
		Plr->CloseGossip();
	};
	void GossipEnd(Object* pObject, Player* pPlayer)
	{
		GossipScript::GossipEnd(pObject, pPlayer);
	}
};


bool HOTTeleporter(uint32 i, Spell * pSpell)
{
    if(!pSpell->p_caster)
		return true;
	uint32 boss_kills = 0;
	uint32 boss_alive = 0;
	for(uint32 i=0;i<pSpell->p_caster->GetMapMgr()->m_CreatureArraySize;i++)
	{
		Creature *tc = pSpell->p_caster->GetMapMgr()->m_CreatureStorage[i];
		if(	tc )
		{
			uint32 entry = tc->GetEntry();
			if( entry == 54590 || entry == 54968 )
			{
				if( tc->isAlive() == false )
					boss_kills++;
				else
					boss_alive++;
			}
		}
	}
	if( boss_kills >= 1 )
		pSpell->p_caster->SafeTeleport(940, pSpell->p_caster->GetInstanceID(), 3944, 290, 14, 3.2 ); 
	return true;
}

void SetupHourOfTwilight(ScriptMgr* mgr)
{
	SpellEntry *sp;
	//delete from ai_agents where entry in ( 54590, 54600, 54548, 54995, 54968, 54971, 54938 );
	////////////////////////////
	mgr->register_creature_script(54590, &Arcurion::Create);	
	mgr->register_creature_script(54600, &FrozenServitor::Create);	
	mgr->register_creature_script(54548, &Thrall::Create);	
	mgr->register_creature_script(54995, &IcyTomb::Create);	
	sp = dbcSpell.LookupEntryForced( 103904 );	//Torrent of Frost periodic tick
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 || sp->eff[0].EffectRadiusIndex == 36 )
			sp->eff[0].EffectRadiusIndex = 11;	//45 yards
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_ICY_TOMB_STUN );	
	//if( sp != NULL )
	{
		sp->eff[2].Effect = SPELL_EFFECT_NULL;
	}
	sp = dbcSpell.LookupEntryForced( 102475 );	//Lava Burst
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
	}
	////////////////////////////
	mgr->register_creature_script(54968, &AsiraDawnslayer::Create);	
	mgr->register_creature_script(54971, &ThrallBenedict::Create);	
	
	sp = dbcSpell.LookupEntryForced( SPELL_ID_BLADE_BARRIER );
	//if( sp != NULL )
	{
		sp->eff[0].EffectTriggerSpell = SPELL_ID_BLADE_BARRIER;
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL;
		sp->procChance = 100;	
		sp->proc_interval = 1;
		sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_LESSER_BLADE_BARRIER );
	//if( sp != NULL )
	{
		sp->eff[0].EffectTriggerSpell = SPELL_ID_BLADE_BARRIER;
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL;
		sp->procChance = 100;	
		sp->proc_interval = 1;
		sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_THROW_KNIFE );
	//if( sp != NULL )
	{
		sp->eff[1].Effect = SPELL_EFFECT_TRIGGER_SPELL;
		sp->eff[1].EffectTriggerSpell = 103587;
	}
	////////////////////////////
	mgr->register_creature_script(54938, &ArchbishopBenedictus::Create);	
	sp = dbcSpell.LookupEntryForced( SPELL_ID_PURIFYING_BLAST );	
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[1].Effect = SPELL_EFFECT_NULL;
		sp->eff[2].Effect = SPELL_EFFECT_NULL;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_RIGHTOUS_SHEAR );	
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 || sp->eff[0].EffectRadiusIndex == 36 )
			sp->eff[0].EffectRadiusIndex = 11;	//45 yards
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_TWILIGHT_SHEAR );	
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 || sp->eff[0].EffectRadiusIndex == 36 )
			sp->eff[0].EffectRadiusIndex = 11;	//45 yards
	}
	sp = dbcSpell.LookupEntryForced( 103744 );	
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_DUEL;
	}
	////////////////////////////
	GossipScript * gs = (GossipScript*) new HourOfTwilightTeleporter();
	mgr->register_go_gossip_script(210026, gs);	//Time Transit Device
    mgr->register_dummy_spell(108925, &HOTTeleporter);
}