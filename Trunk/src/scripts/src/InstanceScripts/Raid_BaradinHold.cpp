#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305) // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

#define FELL_FLAME_SCALE_SIZE		3.0f
#define FELL_FLAME_HOVER_HEIGHT		2.0f
#define EYE_FLAME_SPOT_SCALE		1.0f

#define SPELL_ID_FEL_FIRESTORM		88972
#define SPELL_ID_FEL_FIRESTORM_AOE	89000
#define SPELL_ID_METEOR_SLASH		88942
#define SPELL_ID_CONSUME_DARKNESS	88954
#define SPELL_ID_FOCUSED_FIRE		96884
#define SPELL_ID_FOCUSED_FIRE_AOE	96882
#define SPELL_ID_SEARING_SHADOWS	96913
#define SPELL_ID_EYE_GAZE			96942
#define SPELL_ID_EYE_GAZE_DPS		96946
#define SPELL_ID_EYE_GAZE_AOE		96968

class Argaloth : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Argaloth);

   /*script is really simple : 
    Argaloth is a very straight-forward fight. Split your raid in two equally-size groups, one on each side of the boss. You can use the metal grid on the floor to align the groups.
    Every time Argaloth casts Meteor Slash, you want the tanks to switch. Half of your raid being stacked will help minimize the damage, and the tank rotation will make sure that nobody eats a second Meteor Slash while the increased Fire damage taken debuff is still on.
    Consuming Darkness is a fairly nasty DoT that should be removed as quickly as possible.
    At 66% and 33% Argaloth will start channeling Fel Firestorm. Simply run around (individually, not as a group) without stopping for the duration of the spell, making sure you are back near your assigned position when it's over.
*/
	uint32 phase;
	uint32 ConsuminDarknessTimer;
	uint32 MeteorSlashTimer;
	uint32 diff_mode;
	Argaloth(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		phase = 0;
	}
	void OnLoad()
	{
		diff_mode = 0;
		//make 25 man mode harder then 10 man mode ?
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
		{
			uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
			_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now - 500 ) );	//attack a bit faster cause there are more healers
			diff_mode = 1;
		}
		ConsuminDarknessTimer = 0;
		MeteorSlashTimer = 0;
		RegisterAIUpdateEvent( 500 );
	}
    void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		//check phase change 
		if( ( phase == 0 && _unit->GetHealthPct() < 66 ) || ( phase == 1 && _unit->GetHealthPct() < 33 ) )
		{
			//start casting Fel Firestorm, we cast it on self and target everyone around us
			_unit->CastSpell( _unit, SPELL_ID_FEL_FIRESTORM, false );	//Fel Firestorm
			//no idea why we even need to know what phase we are in
			phase++;
		}
		if( MeteorSlashTimer < GetTickCount() && _unit->isCasting() == false )
		{
			MeteorSlashTimer = GetTickCount() + 3000;	//cast it every 3 seconds - effect last only 20 seconds
			_unit->CastSpell( tank, SPELL_ID_METEOR_SLASH, true );	//Meteor Slash
		}
		if( ConsuminDarknessTimer < GetTickCount() && _unit->isCasting() == false )
		{
			ConsuminDarknessTimer = GetTickCount() + 45000;	//cast it every 45 seconds
			//pick N targets that do not have this dot yet
			uint32 need_casts = 3 + diff_mode*5;	//3 or 8 targets
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InRangeSetRecProt::iterator itr,itr2;
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( (*itr)->IsPlayer() == false	|| !SafeUnitCast( *itr )->isAlive() )
					continue;

				if( isAttackable( _unit, ( *itr ) ) 
					&& SafeUnitCast( *itr )->HasAuraWithNameHash( SPELL_HASH_CONSUMING_DARKNESS, 0, AURA_SEARCH_NEGATIVE ) == false )
				{
					_unit->CastSpell( SafeUnitCast(*itr), SPELL_ID_CONSUME_DARKNESS, false );	//Consuming Darkness
					need_casts--;
					if( need_casts == 0 )
						break;
				}
			}
			_unit->ReleaseInrangeLock();
		}/**/
	}
};

class FelFirestorm : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(FelFirestorm);

	FelFirestorm(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->Init( _unit, AITYPE_LONER, MOVEMENTTYPE_NONE, NULL );	//do not follow owner
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
			_unit->SetScale( FELL_FLAME_SCALE_SIZE );
		}
		//we enable this mob to be unselectable then he will not be able to cast the spell on others
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PLUS_MOB | UNIT_FLAG_PLAYER_CONTROLLED );
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS_2, 0x8000 );
		_unit->SetUInt32Value( UNIT_FIELD_BYTES_1, 0x2000000 );
		RegisterAIUpdateEvent( 1000 );
	}
	void OnPrePushToWorld()
	{
		_unit->SetPositionZ( _unit->GetPositionZ() - FELL_FLAME_SCALE_SIZE * FELL_FLAME_HOVER_HEIGHT );
//		_unit->SetPositionZ( _unit->GetPositionZ() - 5.0f );	//why the fuck are they lifted ?
	}
    void AIUpdate()
    {
		//cast AOE spell
		_unit->CastSpell( _unit, SPELL_ID_FEL_FIRESTORM_AOE, true );
	}
};

class Occuthar : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Occuthar);

   /*
    Focused Fire – Occu'thar sets his gaze on the location of a random player, then inflicts 34125 to 35875 Fire damage every 1 sec. to players within 12 yards of the targeted location.
    Searing Shadows – Occu'thar inflicts 102375 to 107625 Shadow damage to players in a 60 degree cone in front of him, and increases Shadow damage taken by 100% for 30 sec.
    Eyes of Occu'thar – Occu'thar forms an Eye of Occu'thar on every player for 10 sec. The eyes bore into their target hosts with the Gaze of Occu'thar.
        Gaze of Occu'thar – The Eye of Occu'thar gazes at its target host, inflicting 5850 to 6150 Shadow damage every 1 sec. for 10 sec.
        Occu'thar's Destruction – After an Eye of Occu'thar remains attached to a player for 10 sec, it fully bores into the host and detonates with Occu'thar's Destruction, inflicting 24375 to 25625 Shadow damage on all players.
	*/
	uint32 phase;
	uint32 FocusedFireTimer;
	uint32 SearingShadowTimer;
	uint32 EyesOfOcuTimer;
	uint32 diff_mode;
	LocationVector  ffl;
	uint32 FocusedFireSpawnTimer;
//	Unit *DoNotPickMe;
	Occuthar(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		phase = 0;
	}
	void OnLoad()
	{
		diff_mode = 0;
		//make 25 man mode harder then 10 man mode ?
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
		{
			uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
			_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now - 500 ) );	//attack a bit faster cause there are more healers
			diff_mode = 1;
		}
		FocusedFireTimer = 0;
		SearingShadowTimer = 0;
		EyesOfOcuTimer = 0;
		FocusedFireSpawnTimer = 0xFFFFFFFF;
		RegisterAIUpdateEvent( 500 );
	}
    void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		if( FocusedFireTimer < GetTickCount() && _unit->isCasting() == false )
		{
			FocusedFireTimer = GetTickCount() + 35000;	//cast it every 35 seconds 
			//get a random DPS target
			int32 NthTarget = RandomUInt() % MAX( _unit->GetInRangeCount(), 1 );
			Unit *PickedTarget = NULL;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InRangeSetRecProt::iterator itr2;
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();itr2++)
			{
				if( NthTarget <= 0 
//					&& DoNotPickMe != *itr 
					&& (*itr2)->IsPlayer() 
					&& isAttackable( SafeUnitCast( *itr2 ), _unit )	)
				{
					PickedTarget = SafeUnitCast( *itr2 );
				}
				NthTarget--;
			}
			if( PickedTarget == NULL )
				PickedTarget = tank;
//			DoNotPickMe = PickedTarget;
			if( PickedTarget )
			{
				//spawn a fire place at target location
				ffl = PickedTarget->GetPosition();
				FocusedFireSpawnTimer = GetTickCount() + 2500;
				_unit->CastSpell( PickedTarget, SPELL_ID_FOCUSED_FIRE, false );	//Focused Fire - visual of the gaze
			}
		}
		if( FocusedFireSpawnTimer < GetTickCount() )
		{
			_unit->create_guardian( 52369, 20000, 0, NULL, &ffl );
			FocusedFireSpawnTimer = 0xFFFFFFFF;
		}
		if( EyesOfOcuTimer < GetTickCount() )
		{
			EyesOfOcuTimer = GetTickCount() + 20000;	//cast it every 25 seconds 
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InRangeSetRecProt::iterator itr,itr2;
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( (*itr)->IsPlayer() == false	|| !SafeUnitCast( *itr )->isAlive() )
					continue;

				if( isAttackable( _unit, ( *itr ) ) )
				{
					//spawn an eye and force the eye to agro that specific player
					Unit *ng = _unit->create_guardian( 52428, 11000 );
					if( ng )
						ng->GetAIInterface()->AttackReaction( SafeUnitCast( *itr ), 999999 );
				}
			}
			_unit->ReleaseInrangeLock();
		}
		if( SearingShadowTimer < GetTickCount() && _unit->isCasting() == false )
		{
			SearingShadowTimer = GetTickCount() + 25000;	//cast it every 25 seconds 
			_unit->CastSpell( tank, SPELL_ID_SEARING_SHADOWS, false );	//Searing Shadows
		}
	}
};

class EyeOfOccuthar : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(EyeOfOccuthar);

	EyeOfOccuthar(Creature* pCreature) : CreatureAIScript(pCreature)
	{
//		RegisterAIUpdateEvent( 1000 );
	}
	void OnLoad()
	{
		//Gaze of Occu'thar aura, this should DPS our target
		_unit->CastSpell( _unit, SPELL_ID_EYE_GAZE, true );
	}
 /*   void AIUpdate()
   {
		//periodically we dmg our
		//DPS spell
		Unit *u = _unit->GetAIInterface()->GetNextTarget();
		if( u )
			_unit->CastSpell( u, SPELL_ID_EYE_GAZE_DPS, false );	//Gaze of Occu'thar
	}/**/
    void OnRemoveFromWorld()
    {
		if( _unit->isAlive() == false )
			return;
		//cast AOE spell - Occu'thar's Destruction
		_unit->CastSpell( _unit, SPELL_ID_EYE_GAZE_AOE, true );
	}
};


class FocusedFire : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(FocusedFire);

	FocusedFire(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->Init( _unit, AITYPE_LONER, MOVEMENTTYPE_NONE, NULL );	//do not follow owner
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->bInvincible = true;
			_unit->SetScale( EYE_FLAME_SPOT_SCALE );
		}
		//we enable this mob to be unselectable then he will not be able to cast the spell on others
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PLUS_MOB | UNIT_FLAG_PLAYER_CONTROLLED );
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS_2, 0x8000 );
		_unit->SetUInt32Value( UNIT_FIELD_BYTES_1, 0x2000000 );
//		RegisterAIUpdateEvent( 1000 );
	}
	void OnPrePushToWorld()
	{
//		_unit->SetPositionZ( _unit->GetPositionZ() - FELL_FLAME_SCALE_SIZE * FELL_FLAME_HOVER_HEIGHT );
//		_unit->SetPositionZ( _unit->GetPositionZ() - 5.0f );	//why the fuck are they lifted ?
	}
	void OnLoad()
	{
		//Focused Fire aura, this should DPS our target
		//this is also a very beatifull fire animation aura
		_unit->CastSpell( _unit, SPELL_ID_FOCUSED_FIRE_AOE, true );
	}
	void AIUpdate()
    {
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_SKEWER					104936
#define SPELL_COOL_SKEWER				20000
#define SPELL_ID_SEETHING_HATE			105067
#define SPELL_COOL_SEETHING_HATE		25000
#define SPELL_ID_BLADE_DANCE			104995
#define SPELL_COOL_BLADE_DANCE			15000
#define SPELL_ID_BERSERK_ALIZ			47008
#define SPELL_COOL_BERSERK_ALIZ			5*60*1000
#define SPELL_COOL_ARMSTRIKE			2000


char AlizabalYells[15][134] = {
    "Do you hate me? Good.",
    "Feel my hatred!",
    "How I HATE this place. My captors may be long-dead, but don't think I won't take it all out on you miserable treasure-hunters.",
    "I didn't hate that.",
    "I hate adventurers.",
    "I hate armor.",
    "I hate incompetent raiders.",
    "I hate martyrs.",
    "I hate mercy.",
    "I hate standing still.",
    "I hate you all!",
    "I hate... every one of you...",
    "I still hate you.",
    "My hate will consume you!",
    "My hatred burns!",
};

class Alizabal : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Alizabal);
/* 
    Skewer – Alizabal skewers and stuns her current target for 8 sec. Skewer also inflicts 9000 to 11000 Physical damage every 2 sec. and increases the target's damage taken by 150%.
    Seething Hate – Alizabal incites Seething Hate in a random player, inflicting 195000 to 205000 total Fire damage to all players within 6 yards of the target. The damage is split amongst all of the nearby players.
    Blade Dance – Alizabal enters a Blade Dance and inflicts 11875 to 13125 Physical damage every sec. to every player within 13 yards. The Blade Dance also deflects all incoming attacks. 
*/
	uint32  SkewerTimer;
	uint32  SeethingHateTimer;
	uint32  BladeDanceTimer;
	uint32  BerserkTimer;
	uint32	Arm1Timer;
	uint32	Arm2Timer;
	uint32	Arm3Timer;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	uint32	phase;
	float	DifficultyScale;
	Alizabal(Creature* pCreature) : CreatureAIScript(pCreature){}
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = 0.93f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = 0.86f;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = 0.75f;
		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, SPELL_COOL_ARMSTRIKE );	
		RegisterAIUpdateEvent( 1000 );

		SkewerTimer = 0;
	}

	void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			SkewerTimer = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( SkewerTimer == 0 )
		{
			SkewerTimer = GetTickCount() + RandomUInt() % 25000;
			SeethingHateTimer = GetTickCount() + RandomUInt() % 25000;
			BladeDanceTimer = GetTickCount() + RandomUInt() % 25000;
			BerserkTimer = GetTickCount() + SPELL_COOL_BERSERK_ALIZ;
			Arm1Timer = GetTickCount() + SPELL_COOL_ARMSTRIKE * DifficultyScale;
			Arm2Timer = GetTickCount() + SPELL_COOL_ARMSTRIKE * DifficultyScale;
			Arm3Timer = GetTickCount() + SPELL_COOL_ARMSTRIKE * DifficultyScale;
			TalkCooldown = GetTickCount() + RandomUInt() % 25000;
			TalkIndex = 0;
			phase = 0;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, AlizabalYells[ TalkIndex ] );
			TalkIndex = ( TalkIndex + 1 ) % 15;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

		//make her strike with offhand also
		if( Arm1Timer < TickNow && _unit->isCasting() == false )
		{
			Arm1Timer = TickNow + (SPELL_COOL_ARMSTRIKE + RandomUInt() % SPELL_COOL_ARMSTRIKE)*DifficultyScale;
			_unit->Strike( tank, MELEE, NULL, 0, 50, 0, false, false );
		}
		if( Arm2Timer < TickNow && _unit->isCasting() == false )
		{
			Arm2Timer = TickNow + (SPELL_COOL_ARMSTRIKE + RandomUInt() % SPELL_COOL_ARMSTRIKE)*DifficultyScale;
			_unit->Strike( tank, MELEE, NULL, 0, -50, 0, false, false );
		}
		if( Arm3Timer < TickNow && _unit->isCasting() == false )
		{
			Arm3Timer = TickNow + (SPELL_COOL_ARMSTRIKE + RandomUInt() % SPELL_COOL_ARMSTRIKE)*DifficultyScale;
			_unit->Strike( tank, MELEE, NULL, 0, -50, 0, false, false );
		}

		if( SkewerTimer < TickNow && _unit->isCasting() == false )
		{
			SkewerTimer = TickNow + (SPELL_COOL_SKEWER + RandomUInt() % SPELL_COOL_SKEWER)*DifficultyScale;
			_unit->CastSpell( tank, SPELL_ID_SKEWER, false );	
		}
		if( SeethingHateTimer < TickNow && _unit->isCasting() == false )
		{
			SeethingHateTimer = TickNow + (SPELL_COOL_SEETHING_HATE + RandomUInt() % SPELL_COOL_SEETHING_HATE)*DifficultyScale;
			_unit->CastSpell( tank, SPELL_ID_SEETHING_HATE, false );	
		}
		if( BladeDanceTimer < TickNow && _unit->isCasting() == false )
		{
			BladeDanceTimer = TickNow + (SPELL_COOL_BLADE_DANCE + RandomUInt() % SPELL_COOL_BLADE_DANCE)*DifficultyScale;
			_unit->CastSpell( tank, SPELL_ID_BLADE_DANCE, false );	
		}
		if( BerserkTimer < TickNow && _unit->isCasting() == false )
		{
			BerserkTimer = TickNow + SPELL_COOL_BERSERK_ALIZ;
			_unit->CastSpell( _unit, SPELL_ID_BERSERK_ALIZ, false );	
		}
	}
};

void SetupBaradinHold(ScriptMgr* mgr)
{
	//delete from ai_agents where entry in ( 47120,47829,52363,52428,52369,55869 );
	mgr->register_creature_script(47120, &Argaloth::Create);	//Argaloth
	mgr->register_creature_script(47829, &FelFirestorm::Create);	//small flame on the ground to DMG players near it

	mgr->register_creature_script(52363, &Occuthar::Create);	//Occu'thar
	mgr->register_creature_script(52428, &EyeOfOccuthar::Create);	//Occu'thar
	mgr->register_creature_script(52369, &FocusedFire::Create);	//Occu'thar

	mgr->register_creature_script(55869, &Alizabal::Create);	//Argaloth

	//Consuming Darkness - is supposed to scale over time, for now it only scales to avg value
	SpellEntry *sp = dbcSpell.LookupEntryForced( SPELL_ID_CONSUME_DARKNESS );
	//if( sp != NULL )
	{
		sp->eff[0].EffectBasePoints = sp->eff[0].EffectBasePoints * 30;	//30 ticks, no idea how fast it is supposed to increase
		sp->eff[0].EffectAmplitude = 1000;	//from 500 cause it is a real killer
	}
	//Fel Firestorm - should be casted on all players
	sp = dbcSpell.LookupEntryForced( 88977 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = 25;	//70 yards, i hope it covers the whole raid
		sp->eff[0].EffectRadiusMaxIndex = 25;	//70 yards, i hope it covers the whole raid
//		sp->eff[0].EffectAmplitude = 300;	//every second a new flame ? 15 flames / player = 25*15 = 375 spawns
		sp->MaxAffectedTargets = 25 * 2;	//whole raid
		sp->c_is_flags2 |= SPELL_FLAG2_IS_TRIGGERED_ON_TARGET;
	}
	//Fel Firestorm - the AOE part
	sp = dbcSpell.LookupEntryForced( SPELL_ID_FEL_FIRESTORM_AOE );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
//		sp->eff[0].EffectRadiusIndex = 8;	//5 yards, no idea about real value
//		sp->eff[0].EffectRadiusIndex = 15;	//3 yards, no idea about real value
		sp->eff[0].EffectRadiusIndex = 37;	//7 yards, no idea about real value. The idiot mob right now is 5 yards underground for some reason		
	}
	//Gaze of Occu'thar
	sp = dbcSpell.LookupEntryForced( SPELL_ID_EYE_GAZE );
	//if( sp != NULL )
	{
		sp->eff[0].Effect = SPELL_EFFECT_NULL;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
	}
	//Gaze of Occu'thar
	sp = dbcSpell.LookupEntryForced( SPELL_ID_EYE_GAZE_DPS );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
	}
	//Focused Fire - gaze at the target location
	sp = dbcSpell.LookupEntryForced( 96884 );
	//if( sp != NULL )
	{
		sp->c_is_flags2 |= SPELL_FLAG2_IS_TRIGGERED_ON_TARGET;
		sp->eff[0].Effect = SPELL_EFFECT_NULL;
//		sp->eff[0].EffectTriggerSpell = 96873;	//spawns a location of fire
	}
	//Focused Fire - should be casted on all players
	sp = dbcSpell.LookupEntryForced( 96883 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
	}
	//Occu'thar's Destruction
	sp = dbcSpell.LookupEntryForced( 96968 );
	//if( sp != NULL )
	{
//		sp->eff[0].EffectRadiusIndex = 15;	//3 yards, previous was too small
		sp->eff[0].EffectRadiusIndex = 8;	//5 yards, previous was too small
	}
	//Searing Shadows
	sp = dbcSpell.LookupEntryForced( 96913 );
	//if( sp != NULL )
	{
		sp->rangeIndex = 4;	//30 yards, previous was too small
	}
	//Focused Fire
	sp = dbcSpell.LookupEntryForced( 96883 );
	//if( sp != NULL )
	{
		sp->rangeIndex = 4;	//30 yards, previous was too small
		sp->eff[0].EffectRadiusIndex = 9;	//20 yards, no idea about real value. 
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
	}
};