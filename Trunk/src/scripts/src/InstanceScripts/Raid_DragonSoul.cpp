#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305) // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

#define MOB_ENTRY_DRAGONSOUL_TELEPORTER		    55274
#define EXTENSION_MAP_BOS_KILLS					MOB_ENTRY_DRAGONSOUL_TELEPORTER
#define EXTENSION_FLAG_MORCHOCK_DIED			1
#define EXTENSION_FLAG_ZONOZZ_DIED				2
#define EXTENSION_FLAG_YORSAHJ_DIED				4
#define EXTENSION_FLAG_HAGARA_DIED				8
#define EXTENSION_FLAG_ULTRAXION_DIED			16
#define EXTENSION_FLAG_BLACKHORN_DIED			32
#define EXTENSION_FLAG_SPINE_DIED				64
#define EXTENSION_FLAG_MADNESS_DIED				128

struct CreatureYell
{
	char	txt[200];
	uint32	soundID;
};

//this is because people are stating BOSS can't be killed or imba
#define LOG_CREATURE_DEATH( Entry, Killer ) { \
		Unit *towner = Killer->GetTopOwner(); \
		if( towner->IsPlayer() ) \
		{ \
			QueryResult *result = WorldDatabase.Query(	"INSERT INTO BOSSKILLS ( Entry, Player, KillStamp, group_size, diff ) values ( %d, %d, %d, %d, %d )", Entry, towner->GetGUID(), time(NULL), SafePlayerCast(towner)->GetGroup()?SafePlayerCast(towner)->GetGroup()->MemberCount():0, SafePlayerCast(towner)->GetInstanceDifficulty() ); \
			if( result ) delete result; \
		} \
	}
//#define LOG_CREATURE_DEATH( entry, guid )

#define SPEED_SCALE_DIFF_10HEROIC	0.90f
#define SPEED_SCALE_DIFF_25NORMAL	0.80f
#define SPEED_SCALE_DIFF_25HEROIC	0.70f

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPELL_ID_STOMP					103414
#define SPELL_ID_STOMP_HEROIC			109034
#define SPELL_COOL_STOMP				20000
#define SPELL_ID_CRUSH_ARMOR			103687
#define SPELL_COOL_CRUSH_ARMOR			15000
#define SPELL_ID_RESONATING_CHRISTAL	103494
#define SPELL_ID_FURIOUS				103846
#define SPELL_ID_FALLING_FRAGMENTS		103176
#define SPELL_ID_BLACK_BLOOD			103785

CreatureYell MorchokYells[20] = {
	{ "...and the black blood of the earth consumes you.", 26278 },
	{ "...and the rage of the true gods follows.", 26280 },
	{ "...and there is no escape from the Old Gods.", 26279 },
	{ "...and you drown in the hate of The Master.", 26281 },
	{ "Cowards. Weaklings. Come down and fight, or I will bring you down.", 26270 },
	{ "Flee, and die.", 26283 },
	{ "Ground to dust.", 26287 },
	{ "I am unstoppable.", 26285 },
	{ "I will turn this tower to rubble, and scatter it across the wastes.", 26272 },
	{ "Impossible. This cannot be. The tower... must... fall...", 26269 },
	{ "It was inevitable.", 26286 },
	{ "No mortal shall turn me from my task.", 26282 },
	{ "Run, and perish.", 26284 },
	{ "The ground shakes...", 26275 },
	{ "The rocks tremble...", 26276 },
	{ "The stone calls...", 26274 },
    { "The surface quakes...", 26277 },
    { "Wyrmrest will fall. All will be dust.", 26273 },
    { "You seek to halt an avalanche. I will bury you.", 26268 },
    { "You thought to fight me alone? The earth splits to swallow and crush you.", 0 },
};

class Morchok : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Morchok);
/* 
    Stomp – Morchok performs a massive stomp, splitting 750000 damage between all players within 25 yards. The two closest targets take a double share of the damage.
    In Heroic Difficulty Stomp also increases the Physical damage taken by affected players by 100% for 10 sec.
    Crush Armor – Morchok strikes his current target, inflicting 120% normal melee damage and reducing the target's armor by 10% for 20 sec. This effect stacks up to 10 times.
    Resonating Crystal – Morchok conjures an explosive crystal onto the battlefield that explodes after 12 sec. for massive Shadow damage. The explosion splits the damage between three random players, and the total damage inflicted increases depending on the distance of each player from the crystal.
    In 25 Player raids, the damage splits between seven random enemies.
    Furious – At 20% remaining health, Morchok becomes furious and increases his attack speed by 30% and his damage inflicted by 20%. 
	
	Morchok uses the power of the earth to draw all players to him, inflicting 5% of their total health as Physical damage every sec. for 5 sec. While the power of the earth draws the players to him, Morchok also summons Earth's Vengeance.
    Earth's Vengeance – Shards of Earth's Vengeance erupt from the ground, inflicting 15000 Physical damage to nearby players within 2 yards and leaving behind obstacles.
    Black Blood of the Earth – Morchok causes the Black Blood of the Earth to erupt outwards from him, inflicting 5000 Nature damage and increasing Nature damage taken by 100% every sec. while a player stands within the blood. This effect stacks up to 20 times. 
	Kohcrom – Upon reaching 90% remaining health, Morchok tears himself in half and creates his twin Kohcrom. He casts the same spells as Morchok, but a few seconds later. 
*/
	uint32  StompTimer;
	uint32  CrushArmorTimer;
	uint32	TalkIndex;
	uint32	TalkCooldown;
	uint32	phase;
	float	DifficultyScale;
    
	Morchok(Creature* pCreature) : CreatureAIScript(pCreature)
	{
	}
    
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		//make him attack faster as difficulty increases
		uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now * DifficultyScale ) );	//attack a bit faster cause there are more healers

		RegisterAIUpdateEvent( 1000 );

		StompTimer = 0;
	}

	void OnDied(Unit *mKiller)
	{
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		*EncounterStatus |= EXTENSION_FLAG_MORCHOCK_DIED;
		if( _unit->GetEntry() == 55265 )
		{
			Creature *c = _unit->GetMapMgr()->CreateCreature( MOB_ENTRY_DRAGONSOUL_TELEPORTER, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
			if( c )
				c->SetScale( c->GetScale() + 3 );
		}
		RemoveAIUpdateEvent();
//		LOG_CREATURE_DEATH( _unit->GetEntry(), mKiller->GetGUID(), (mKiller->IsPlayer() && SafePlayerCast(mKiller)->GetGroup() )?SafePlayerCast(mKiller)->GetGroup()->MemberCount():0, mKiller->IsPlayer()?SafePlayerCast(mKiller)->GetInstanceDifficulty():0);
		LOG_CREATURE_DEATH( _unit->GetEntry(), mKiller );
		if( mKiller && mKiller->IsPlayer() && SafePlayerCast( mKiller )->GetGuild() )
			SafePlayerCast( mKiller )->GetGuild()->GuildNewsAdd( GUILD_NEWS_LOG_DUNGEON_ENCOUNTER, mKiller->GetGUID(), 1292 );
    }

	void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			StompTimer = 0;
			phase = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		//start spell cooldowns
		if( StompTimer == 0 )
		{
			StompTimer = TickNow + RandomUInt() % 25000;
			CrushArmorTimer = TickNow + RandomUInt() % 25000;
			TalkCooldown = TickNow + RandomUInt() % 25000;
			TalkIndex = 0;
			phase = 0;
		}

		if( TalkCooldown < TickNow )
		{
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, MorchokYells[ TalkIndex ].txt );
			if( MorchokYells[ TalkIndex ].soundID > 0 )
				_unit->PlaySoundTarget( MorchokYells[ TalkIndex ].soundID, _unit->GetGUID() );
			TalkIndex = ( TalkIndex + 1 ) % 20;
			TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
		}

		if( StompTimer < TickNow && _unit->isCasting() == false )
		{
			StompTimer = TickNow + (SPELL_COOL_STOMP + RandomUInt() % SPELL_COOL_STOMP)*DifficultyScale;
			if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
				_unit->CastSpell( tank, SPELL_ID_STOMP_HEROIC, true );	
			else
				_unit->CastSpell( tank, SPELL_ID_STOMP, true );	
		}

		if( CrushArmorTimer < TickNow && _unit->isCasting() == false )
		{
			CrushArmorTimer = TickNow + (SPELL_COOL_CRUSH_ARMOR + RandomUInt() % SPELL_COOL_CRUSH_ARMOR)*DifficultyScale;
			_unit->CastSpell( tank, SPELL_ID_CRUSH_ARMOR, true );	
		}

		if( phase == 0 && _unit->GetHealthPct() <= 90  && _unit->isCasting() == false 
//			&& IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty )
			)
		{
			phase = 1;
			if( _unit->GetEntry() == 55265 )
			{
//				_unit->CastSpell( _unit, 109017, false ); //Summon Kohcrom
				_unit->create_guardian( 57773, 2*60*60*1000, 0, NULL, NULL, NULL, false );
			}
		}
		if( phase == 1 && _unit->GetHealthPct() <= 20 )
		{
			phase = 2;
			_unit->CastSpell( _unit, SPELL_ID_FURIOUS, false );
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MOB_ENTRY_ZONOZZ				    55308
#define SPELL_ID_FOCUSED_ANGER              104543
#define SPELL_ID_FOCUSED_ANGER_HEROIC       109411
#define SPELL_COOL_FOCUSED_ANGER            10000
#define SPELL_ID_PSYCHIC_DRAIN              104322
#define SPELL_ID_PSYCHIC_DRAIN_HEROIC       104608
#define SPELL_COOL_PSYCHIC_DRAIN            55000	//can heal for 1M-1.6M health
#define SPELL_ID_DISRUPTING_SHADOWS         103434  //strong DOT
#define SPELL_ID_DISRUPTING_SHADOWS_HEROIC  104601
#define SPELL_COOL_DISRUPTING_SHADOWS       20000
#define MOB_ENTRY_VOID_OF_THE_UNMAKING      58473
#define EXTENSION_ID_VOID_STATUS			MOB_ENTRY_VOID_OF_THE_UNMAKING
#define MOB_ENTRY_CLAW_OF_GORATH            55418
#define MOB_ENTRY_EYE_OF_GORATH             57875
#define MOB_ENTRY_FLAIL_OF_GORATH           55417
#define SPELL_COOL_TENTACLE_SPAWN	        ( 30000 / 3 ) //needs to be much less then 30 seconds = phase timer
#define SPELL_ID_BLACK_BLOOD_OF_GORATH		104378
#define SPELL_ID_INCREASE_DAMAGE_TAKEN_ZONOZZ	50660

char ZonozzYells[11][78] = {
    "Gul'kafh an'qov N'Zoth.",
    "N'Zoth ga zyqtahg iilth.",
    "Sk'magg yawifk hoq.",
    "Sk'shgn eqnizz hoq.",
    "Sk'shuul agth vorzz N'Zoth naggwa'fssh.",
    "Sk'tek agth nuq N'Zoth yyqzz.",
    "Sk'uuyat guulphg hoq.",
    "Sk'yahf agth huqth N'Zoth qornaus.",
    "Uovssh thyzz... qwaz...",
    "Vwyq agth sshoq'meg N'Zoth vra zz shfk qwor ga'halahs agthu. Uulg'ma, ag qam.",
    "Zzof Shuul'wah. Thoq fssh N'Zoth!",
};

CreatureYell ZonozzWhispers[11] = {
	{ "Gaze into the heart of N'Zoth.", 26345 },
    { "Once more shall the twisted flesh-banners of N'Zoth chitter and howl above the fly-blown corpse of this world. After millennia, we have returned.", 26337 },
    { "The will of N'Zoth corrupts you.", 26344 },
    { "To have waited so long... for this...", 0 },
    { "Victory for Deathwing. For the glory of N'Zoth!", 26335 },
    { "Your agony sustains me.", 26343 },
    { "Your deaths shall sing of N'Zoth's unending glory.", 26339 },
    { "Your fear drives me.", 26340 },
    { "Your skulls shall adorn N'Zoth's writhing throne.", 26338 },
    { "Your souls shall sate N'Zoth's endless hunger.", 26341 },
    { "Your suffering strengthens me.", 26342 }
};

class Zonozz : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Zonozz);
/* 
    Focused Anger – Warlord Zon'ozz increasingly focuses his anger and rage, increasing his Physical damage done by 10% and his attack speed by 10% . This effect stacks.
    Psychic Drain – Warlord Zon'ozz channels a wave of psychic force in a 30 degree cone in front of himself, inflicting 111000 to 129000 Shadow damage and leeching life for ten times the amount of damage dealt.
    Disrupting Shadows – Warlord Zon'ozz covers random players with Disrupting Shadows. The effect inflicts 33392 to 38807 Shadow damage every 2 seconds. The shadows also deal 55592 to 64608 Shadow damage and knockback the targeted player if the effect is dispelled.
    In Heroic Difficulty the effect deals 55592 to 64608 Shadow damage and knocks back all players within 0 yards.

    Void of the Unmaking
    Warlord Zon'ozz summons a Void of the Unmaking. This void travels forward until it encounters a soul to absorb.
        Void Diffusion – When the Void collides with a player it diffuses, inflicting 180000 Shadow damage split evenly between nearby players. The force of this diffusion causes the void to ricochet in the opposite direction, and the absorbed souls increase the damage the Void inflicts by 20% per diffusion.
        Void Diffusion – When the Void collides with a player it diffuses, inflicting 50,000 Shadow damage. The force of this diffusion causes the void to ricochet in the opposite direction, and the absorbed souls increase the damage the Void inflicts by 5% per diffusion.
        Black Blood Eruption – If the Void of Unmaking reaches the outer edges of the Maw of Go'rath, it will spark a violent reaction. This eruption deals 59700 to 60300 Shadow damage to all targets within the maw, knocking them into the air.
        Zon'ozz Diffused
            When the Void collides with Warlord Zon'ozz, it inflicts a distracting shock to him. The Void Diffusion increases the damage Zon'ozz takes by 5% for every time the Void of the Unmaking bounces between players. This collision enrages Warlord Zon'ozz, causing him to awaken the Maw of Go'rath and lose his Focused Anger.
            In Heroic Difficulty the awakening of the Maw of Go'rath causes many different types of tentacles to emerge and attack the players.
            Black Blood of Go'rath – Warlord Zon'ozz inflicts 12187 to 12812 Shadow damage every 1 sec. to all players for 30 sec.
            In Heroic Difficulty each tentacle of Go'rath inflicts 4300 Shadow damage every 2 sec. to all nearby enemies. The damage is multiplied by the number of tentacles alive.
        Claw of Go'rath 
            Ooze Spit – When not tanked the Claw of Go'rath spits ooze at a random player, inflicting 97500 to 102500 Shadow damage to the target and any allies within 4 yards.
        Eye of Go'rath 
            Shadow Gaze – The Eye of Go'rath casts its Shadowy Gaze at a random player, inflicting 23310 to 27090 Shadow damage.
        Flail of Go'rath 
            Wild Flail – The Flail of Go'rath whips about wildly, inflicting 14725 to 16275 Physical damage and knocking back nearby enemies within 15 yards.
            Sludge Spew – The Flail of Go'rath spews sludge at a random player, inflicting 22837 to 24009 Shadow damage.
*/
	uint32  FocusedAngerTimer;
	uint32  PsyhicDrainTimer;
    uint32  DisruptingShadowsTimer;
	uint32	TentacleTimer;
	uint32	TentacleIndex;
	uint32	YellIndex;
	uint32	WhisperIndex;
	uint32	TalkCooldown;
	uint32	phase;
	float	DifficultyScale;
	uint32	phase2Timer;
	uint32	VoidCooldown;
    
	Zonozz(Creature* pCreature) : CreatureAIScript(pCreature)
	{
        phase = 0;
	}

	void OnDied(Unit *mKiller)
	{
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		*EncounterStatus |= EXTENSION_FLAG_ZONOZZ_DIED;
		_unit->GetMapMgr()->CreateCreature( MOB_ENTRY_DRAGONSOUL_TELEPORTER, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
		RemoveAIUpdateEvent();
		//despawn tentacles to avoid bugs
		InRangeSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
		{
			itr = itr2;
			itr2++;
			if( !(*itr)->IsCreature() )
				continue;
			if( (*itr)->GetEntry() == MOB_ENTRY_CLAW_OF_GORATH || (*itr)->GetEntry() == MOB_ENTRY_EYE_OF_GORATH || (*itr)->GetEntry() == MOB_ENTRY_FLAIL_OF_GORATH )
				SafeCreatureCast( (*itr) )->Despawn( 1, 0 );
		}
		_unit->ReleaseInrangeLock();
		//try to see players lie about BOSS difficulty
		LOG_CREATURE_DEATH( _unit->GetEntry(), mKiller );
		if( mKiller && mKiller->IsPlayer() && SafePlayerCast( mKiller )->GetGuild() )
			SafePlayerCast( mKiller )->GetGuild()->GuildNewsAdd( GUILD_NEWS_LOG_DUNGEON_ENCOUNTER, mKiller->GetGUID(), 1294 );
    }
    
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->IsCreature() && SafeCreatureCast( _unit )->GetProto()->MinHealth == 68198240 )
		{
			if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN )
			{
				_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 59000000 );
				_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			}
			else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			{
				DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
				_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 69000000 );
				_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			}
			else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			{
				DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
				_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 169000000 );
				_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			}
			else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			{
				DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
				_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 290000000 );
				_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			}
		}
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		//make him attack faster as difficulty increases
		uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now * DifficultyScale ) );	//attack a bit faster cause there are more healers

		int64 *VoidStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_ID_VOID_STATUS );
		*VoidStatus = -1;

		RegisterAIUpdateEvent( 1000 );
	}

	void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			phase = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		// Init phase
		if( phase == 0 )
		{
            FocusedAngerTimer = TickNow + RandomUInt() % 25000;
            PsyhicDrainTimer = TickNow + RandomUInt() % 25000;
            DisruptingShadowsTimer = TickNow + RandomUInt() % 25000;
			TentacleTimer = TickNow + RandomUInt() % 25000;
			TentacleIndex = 0;
			TalkCooldown = TickNow + RandomUInt() % 25000;
			YellIndex = 0;
			WhisperIndex = 0;
			phase2Timer = 0;
            phase = 1;
			VoidCooldown = TickNow + 10000; //give enough time for players to start the combat
		}

		int64 *VoidStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_ID_VOID_STATUS );

        if (phase == 1)
        {
            if( TalkCooldown < TickNow )
            {
                if( ( RandomUInt() % 3 ) == 0 )
                {
                    _unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, ZonozzYells[ YellIndex ] );
                    YellIndex = ( YellIndex + 1 ) % 11;
                    TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
                }
                else
                {                
                    _unit->SendChatMessage( CHAT_MSG_MONSTER_WHISPER, LANG_UNIVERSAL, ZonozzWhispers[ WhisperIndex ].txt );
					if( ZonozzWhispers[ WhisperIndex ].soundID > 0 )
						_unit->PlaySoundTarget( ZonozzWhispers[ WhisperIndex ].soundID, _unit->GetGUID() );
                    WhisperIndex = ( WhisperIndex + 1 ) % 11;
                    TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
                }
            }
            if( FocusedAngerTimer < TickNow && _unit->isCasting() == false )
            {
                FocusedAngerTimer = TickNow + (SPELL_COOL_FOCUSED_ANGER + RandomUInt() % SPELL_COOL_FOCUSED_ANGER)*DifficultyScale;
                if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
                    _unit->CastSpell( tank, SPELL_ID_FOCUSED_ANGER_HEROIC, false );	
                else
                    _unit->CastSpell( tank, SPELL_ID_FOCUSED_ANGER, false );	
            }
            if( PsyhicDrainTimer < TickNow && _unit->isCasting() == false )
            {
                PsyhicDrainTimer = TickNow + (SPELL_COOL_PSYCHIC_DRAIN + RandomUInt() % SPELL_COOL_PSYCHIC_DRAIN)*DifficultyScale;
                if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
                    _unit->CastSpell( tank, SPELL_ID_PSYCHIC_DRAIN_HEROIC, false );	
                else
                    _unit->CastSpell( tank, SPELL_ID_PSYCHIC_DRAIN, false );	
            }
            if( DisruptingShadowsTimer < TickNow && _unit->isCasting() == false )
            {
                DisruptingShadowsTimer = TickNow + (SPELL_COOL_DISRUPTING_SHADOWS + RandomUInt() % SPELL_COOL_DISRUPTING_SHADOWS)*DifficultyScale;
				AI_Spell sp;
				sp.spelltargetType = TTYPE_ENEMY_FOR_BUFF;
				sp.minrange = 0.0f;
				sp.maxrange = 30.0f;
				sp.procChance = 100;
				sp.spell = dbcSpell.LookupEntryForced( SPELL_ID_DISRUPTING_SHADOWS_HEROIC );
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
                if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
                    _unit->CastSpell( t, SPELL_ID_DISRUPTING_SHADOWS_HEROIC, false );	
                else
                    _unit->CastSpell( t, SPELL_ID_DISRUPTING_SHADOWS, false );	
            }
			if( VoidCooldown < TickNow )
			{
				//create the first Void or recreate after phase 2
				if( *VoidStatus == -1 )
				{
					_unit->RemoveAura( SPELL_ID_INCREASE_DAMAGE_TAKEN_ZONOZZ, 0, AURA_SEARCH_ALL, MAX_PASSIVE_AURAS );
					//remove all tentacles also
					*VoidStatus = 1;
					_unit->GetMapMgr()->CreateCreature( MOB_ENTRY_VOID_OF_THE_UNMAKING, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() + 0.5f );
//_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, "Debug MSG : Spawned Void ball !" );
					//maybe cast http://www.wowhead.com/spell=109413 ?
					//http://www.wowhead.com/spell=103953 ?
				}
				//did our VoidOfTheUnmaking die ?
//				else if( *VoidStatus == 0 )
				else if( *VoidStatus + 2000 < TickNow )
				{
					phase = 2;
					phase2Timer = TickNow + 30000;
					_unit->CastSpell( _unit, 109413, true );	//darkness
					_unit->CastSpell( _unit, SPELL_ID_BLACK_BLOOD_OF_GORATH, false );
					_unit->GetAIInterface()->disable_melee = true;
				}
				VoidCooldown = TickNow + 10000;
			}
        }
		//channel black blood of gorath for 30 sec and create random tentacles
		if( phase == 2 )
		{
			if( TentacleTimer < TickNow )
			{
				TentacleTimer = TickNow + (SPELL_COOL_TENTACLE_SPAWN + RandomUInt() % SPELL_COOL_TENTACLE_SPAWN)*DifficultyScale;
				LocationVector v = _unit->GetPosition();
#define TentacleSpawnLocationsCount 8
static float spawn_x[TentacleSpawnLocationsCount]={-1700,-1745,-1802,-1840,-1834,-1790,-1733,-1696};
static float spawn_y[TentacleSpawnLocationsCount]={-1882,-1884,-1850,-1894,-1951,-1988,-1984,-1940};
				int index = TentacleIndex % TentacleSpawnLocationsCount;
				v.x = spawn_x[index];
				v.y = spawn_y[index];
				v.z = -221;
/*
#define FLAIL_SPAWN_RADIUS	20
				if( Rand( 50 ) )
					v.x += FLAIL_SPAWN_RADIUS + RandomUInt( ) % FLAIL_SPAWN_RADIUS;
				else
					v.x += -( FLAIL_SPAWN_RADIUS + RandomUInt( ) % FLAIL_SPAWN_RADIUS );
				if( Rand( 50 ) )
					v.y += FLAIL_SPAWN_RADIUS + RandomUInt( ) % FLAIL_SPAWN_RADIUS;
				else
					v.y += -( FLAIL_SPAWN_RADIUS + RandomUInt( ) % FLAIL_SPAWN_RADIUS );
					*/
				int tindex = ( TentacleIndex + 1 ) % 3;
				if( tindex == 0 )
					_unit->create_guardian( MOB_ENTRY_CLAW_OF_GORATH, 1200000, 0, NULL, &v );
				else if( tindex == 1 )
					_unit->create_guardian( MOB_ENTRY_EYE_OF_GORATH, 1200000, 0, NULL, &v );
				else if( tindex == 2 )
					_unit->create_guardian( MOB_ENTRY_FLAIL_OF_GORATH, 1200000, 0, NULL, &v );
				TentacleIndex++;
			}
			//end of channel
			if( phase2Timer < TickNow )
			{
				phase = 1;
				*VoidStatus = -1;
				_unit->GetAIInterface()->disable_melee = false;
			}
		}
	}
};

#define SPELL_ID_VOID_DIFFUSION             103527
#define SPELL_COOL_VOID_DIFFUSION           50000
#define SPELL_ID_BLACK_BLOOD_ERUPTION       108794
#define SPELL_COOL_BLACK_BLOOD_ERUPTION     20000

class VoidOfTheUnmaking : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VoidOfTheUnmaking);

    bool	started_moving;
	uint32	NotReadyToBounce;
	uint32	move_to_index;
	float	bounce_x,bounce_y,bounce_z;
//	uint32	MovetoDesyncForceUpdate;
    VoidOfTheUnmaking(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
		}
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
		_unit->bInvincible = true;
		_unit->GetAIInterface()->m_moveFly = true;
		//we enable this mob to be unselectable then he will not be able to cast the spell on others
		_unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );
		_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, 39108 );	//this has an 50% chance to be spawned invisible if using blizz templates
//		_unit->SetUInt32Value( UNIT_FIELD_FLAGS_2, 0x8000 );
//		_unit->SetUInt32Value( UNIT_FIELD_BYTES_1, 0x2000000 );
//		_unit->SetUInt32Value( UNIT_FIELD_BYTES_2, 0x1 );
#define	VOID_TARGET_BOUNCE_COOLDOWN	8000
		_unit->m_walkSpeed = 2.5f;
		_unit->m_runSpeed = _unit->m_walkSpeed;
		_unit->m_base_runSpeed = _unit->m_runSpeed;
		_unit->m_base_walkSpeed = _unit->m_walkSpeed;
		_unit->m_flySpeed = _unit->m_walkSpeed;
		_unit->m_backFlySpeed = _unit->m_walkSpeed;
		_unit->m_backWalkSpeed = _unit->m_walkSpeed;	// this should really be named m_backRunSpeed
		_unit->m_swimSpeed = _unit->m_walkSpeed;
		_unit->m_backSwimSpeed = _unit->m_walkSpeed;
		_unit->m_turnRate = 3.141593f;
		_unit->m_pitchRate = 3.141593f;
		RegisterAIUpdateEvent( 3000 );
		started_moving = false;
		NotReadyToBounce = GetTickCount() + VOID_TARGET_BOUNCE_COOLDOWN;
		move_to_index = 0;
	}
    void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		//check if we should simply despawn
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		if( *EncounterStatus & EXTENSION_FLAG_ZONOZZ_DIED )
		{
			_unit->Despawn( 1, 0 );
		}

		uint32 TickNow = GetTickCount();
		//tell the world we are ok and alive
		int64 *VoidStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_ID_VOID_STATUS );
		*VoidStatus = TickNow; 

		if( NotReadyToBounce > TickNow )
			_unit->SetScale( 1.0f );
		else
			_unit->SetScale( 3.0f );

#define VOID_TARGET_LOCATIONS_COUNT	4
		static float move_x[VOID_TARGET_LOCATIONS_COUNT]={-1880,-1824,-1658,-1713};
		static float move_y[VOID_TARGET_LOCATIONS_COUNT]={-1976,-1809,-1857,-2024};
		static float move_z[VOID_TARGET_LOCATIONS_COUNT]={-215,-215,-215,-215};
		//first time we pick a self distrat destination
		if( started_moving == false )
		{
			uint32 preindex = move_to_index;
			move_to_index = RandomUInt() % VOID_TARGET_LOCATIONS_COUNT;
			if( preindex == move_to_index )
				move_to_index = ( move_to_index + 1 ) % VOID_TARGET_LOCATIONS_COUNT;
			bounce_x = move_x[move_to_index];
			bounce_y = move_y[move_to_index];
			bounce_z = move_z[move_to_index];
//			bounce_z = MAX( _unit->GetPositionZ(), move_z[move_to_index] );
			_unit->GetAIInterface()->MoveTo( bounce_x, bounce_y, bounce_z, 0.0f );
//_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, "Debug MSG : Void ball started moving" );
//char msg[500];
//sprintf( msg, "Debug MSG : Void ball started moving to from %f %f %f to %f %f %f", _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(),bounce_x,bounce_y,bounce_z );
//_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, msg );
//			_unit->GetAIInterface()->MoveTo( bounce_x, bounce_y, _unit->GetPositionZ(), 0.0f );
			started_moving = true;
			NotReadyToBounce = TickNow + VOID_TARGET_BOUNCE_COOLDOWN;
		}/**/
		//make sure we do not have any client server desync. Update unit position by move spam
		else if( started_moving == true )
		{
//			_unit->GetAIInterface()->StopMovement( 0 );
//			_unit->GetAIInterface()->SendMoveToPacket( bounce_x, bounce_y, bounce_z, 0.0f, _unit->GetAIInterface()->m_TimeRemainingReachDestination, _unit->GetAIInterface()->getMoveFlags() );
			_unit->GetAIInterface()->SendCurrentMove( NULL );
//char msg[500];
//sprintf( msg, "Debug MSG : Void ball started moving to from %f %f %f to %f %f %f", _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(),bounce_x,bounce_y,bounce_z );
//_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, msg );
		}
		//hmm this seems to bug out for some reason. Make sure to update properly always
//		_unit->GetAIInterface()->Update( 1 );
		//time to explode ?
		if( NotReadyToBounce < TickNow && _unit->GetDistanceSq( bounce_x, bounce_y, bounce_z ) < MAX( 3.0f, DISTANCE_TO_SMALL_TO_WALK ) )
		{
_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, "Debug MSG : Exploded" );

			Unit *OneTarget = NULL;
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

				_unit->DealDamage( SafeUnitCast(*itr), SafeUnitCast(*itr)->GetMaxHealth() / 2, SPELL_ID_BLACK_BLOOD_ERUPTION );
				_unit->DealDamage( SafeUnitCast(*itr), SafeUnitCast(*itr)->GetMaxHealth() / 2, SPELL_ID_BLACK_BLOOD_ERUPTION );
//				_unit->DealDamage( SafeUnitCast(*itr), SafeUnitCast(*itr)->GetMaxHealth() / 2, SPELL_ID_BLACK_BLOOD_ERUPTION );
				OneTarget = SafeUnitCast(*itr);
	            _unit->CastSpell( OneTarget, SPELL_ID_BLACK_BLOOD_ERUPTION, true );	
			}
			_unit->ReleaseInrangeLock();

            _unit->CastSpell( OneTarget, SPELL_ID_BLACK_BLOOD_ERUPTION, true );	

			//set map status so zonoz will enter phase 2
			_unit->Despawn( 1, 0 );
			RemoveAIUpdateEvent();
			return;
		}/**/
		//check if we are bouncing but not double bouncing or hitting zonozz
//		if( _unit->GetDistanceSq( bounce_x, bounce_y, bounce_z ) > MAX( 5.0f, DISTANCE_TO_SMALL_TO_WALK ) )
		float min_dist = 9999999.9f;
		float zonozdist = 0;
		if( NotReadyToBounce < TickNow )
		{
			Unit *Target = NULL;
			InRangeSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
			{
				itr = itr2;
				itr2++;
				if( !(*itr)->IsUnit() 
					|| !SafeUnitCast((*itr))->isAlive()
					)
					continue;

				float tdist = Distance2DSq( (*itr)->GetPositionX(), (*itr)->GetPositionY(), _unit->GetPositionX(), _unit->GetPositionY() );
				if( tdist < min_dist && (*itr)->IsPlayer() )
					min_dist = tdist;
				if( SafeUnitCast(*itr)->GetEntry() == MOB_ENTRY_ZONOZZ )
					zonozdist = tdist;
				if( tdist < MAX( 20.0f * 20.0f, DISTANCE_TO_SMALL_TO_WALK ) )
				{
					//is this a player bounce ? Pick a new target location on next update
					if( (*itr)->IsPlayer() )
						Target = SafeUnitCast(*itr);

					//time to start phase 2 cause we hit zonozz?
					if( SafeUnitCast(*itr)->GetEntry() == MOB_ENTRY_ZONOZZ )
					{
						uint32 SpellIdHeroicAnger;
						if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
							SpellIdHeroicAnger = SPELL_ID_FOCUSED_ANGER_HEROIC;	
						else
							SpellIdHeroicAnger = SPELL_ID_FOCUSED_ANGER;	
						uint32 count = SafeUnitCast(*itr)->RemoveAura( SpellIdHeroicAnger, 0, AURA_SEARCH_ALL, MAX_PASSIVE_AURAS );
						//deal dmg equal to the focus anger count
						for(uint32 i=0;i<count;i++)
							SafeUnitCast(*itr)->CastSpell( SafeUnitCast(*itr), SPELL_ID_INCREASE_DAMAGE_TAKEN_ZONOZZ, true ); // Focused - not sure about the real name of the spell :(
						//set map status so zonoz will enter phase 2
						//no need to continue. We just died
_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, "Debug MSG : Bounced on zonozz" );
						RemoveAIUpdateEvent();
						_unit->Despawn( 1, 0 );
						break;
					}
				}
			}
			_unit->ReleaseInrangeLock();
			//we will bounce
			if( Target )
			{
				started_moving = false;
				//damage at least one target that was close enough to bounce us
	            _unit->CastSpell( Target, SPELL_ID_VOID_DIFFUSION, true );	
				_unit->GetAIInterface()->StopMovement( 0 );
_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, "Debug MSG : Bounced on player" );
			}
			else if( NotReadyToBounce < TickNow )
			{
char msg[500];
sprintf( msg, "Debug MSG : Closest player at %d. Zonozz distance %d", (int32)sqrt(min_dist),(int32)sqrt(zonozdist) );
_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, msg );
			}
		}/**/
    }
};

#define SPELL_ID_BLACK_BLOOD_OF_GORATH_DMG		104377
#define SPELL_DMG_BLACK_BLOOD_OF_GORATH			4300

class ClawOfGorath : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ClawOfGorath);

	uint32 CreatedAtStamp;
    ClawOfGorath(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
		}
		if( _unit->GetAIInterface() )
			_unit->GetAIInterface()->m_canMove = false;
		float DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		RegisterAIUpdateEvent( 5000 * DifficultyScale );
		CreatedAtStamp = GetTickCount();
	}
    void AIUpdate()
    {
		//do we have a tank nearby ?
		Player *InRangeTank = NULL;
		SpellEntry *sp = dbcSpell.LookupEntryForced( SPELL_ID_BLACK_BLOOD_OF_GORATH ); //Black Blood of Go'rath
		InRangePlayerSetRecProt::iterator itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
		{
			float dist =  (*itr2)->GetDistance2dSq( _unit );
			if( dist <= (4.0f*4.0f) )
				InRangeTank = (*itr2);
			if( dist < 30.0f * 30.0f )
			{
				dealdamage tdmg;
				tdmg.base_dmg = SPELL_DMG_BLACK_BLOOD_OF_GORATH;
				_unit->SpellNonMeleeDamageLog( SafeUnitCast(*itr2), sp, &tdmg ); //Black Blood of Go'rath
			}
		}
		_unit->ReleaseInrangeLock();
		if( InRangeTank == NULL )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER_RANDOM;
			sp.minrange = 2.0f;
			sp.maxrange = 45.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			if( t )
				_unit->CastSpell( t, 109396, false );	//Ooze Spit
		}
	}
};

class EyeOfGorath : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EyeOfGorath);

    EyeOfGorath(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
		}
		if( _unit->GetAIInterface() )
			_unit->GetAIInterface()->m_canMove = false;
		float DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		RegisterAIUpdateEvent( 5000 * DifficultyScale );
	}
    void AIUpdate()
    {
		SpellEntry *spb = dbcSpell.LookupEntryForced( SPELL_ID_BLACK_BLOOD_OF_GORATH_DMG ); //Black Blood of Go'rath
		InRangePlayerSetRecProt::iterator itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
		{
			float dist =  (*itr2)->GetDistance2dSq( _unit );
			if( dist < 30.0f * 30.0f )
			{
				dealdamage tdmg;
				tdmg.base_dmg = SPELL_DMG_BLACK_BLOOD_OF_GORATH;
				_unit->SpellNonMeleeDamageLog( SafeUnitCast(*itr2), spb, &tdmg ); //Black Blood of Go'rath
			}
		}
		_unit->ReleaseInrangeLock();

		AI_Spell sp;
		sp.spelltargetType = TTYPE_AT_RANGE_PLAYER_RANDOM;
		sp.minrange = 2.0f;
		sp.maxrange = 60.0f;
		sp.procChance = 100;
		Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
		if( t )
			_unit->CastSpell( t, 109391, false );	//Shadow Gaze
	}
};

class FlailOfGorath : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FlailOfGorath);

	uint32 GlobalTimer;
	float DifficultyScale;
    FlailOfGorath(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
		}
		if( _unit->GetAIInterface() )
			_unit->GetAIInterface()->m_canMove = false;
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		RegisterAIUpdateEvent( 5000 * DifficultyScale );
		GlobalTimer = 0;
	}
    void AIUpdate()
    {
		SpellEntry *sp = dbcSpell.LookupEntryForced( SPELL_ID_BLACK_BLOOD_OF_GORATH_DMG ); //Black Blood of Go'rath
		InRangePlayerSetRecProt::iterator itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
		{
			float dist =  (*itr2)->GetDistance2dSq( _unit );
			if( dist < 30.0f * 30.0f )
			{
				dealdamage tdmg;
				tdmg.base_dmg = SPELL_DMG_BLACK_BLOOD_OF_GORATH;
				_unit->SpellNonMeleeDamageLog( SafeUnitCast(*itr2), sp, &tdmg ); //Black Blood of Go'rath
			}
		}
		_unit->ReleaseInrangeLock();

		uint32 TickNow = GetTickCount();
		if( GlobalTimer > TickNow )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank != NULL )
		{
			GlobalTimer = TickNow + 10000 * DifficultyScale;
			_unit->CastSpell( tank, 109199, false );	//Wild Flail
		}
		else
		{
			GlobalTimer = TickNow + 3000 * DifficultyScale;
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER_RANDOM;
			sp.minrange = 2.0f;
			sp.maxrange = 45.0f;
			sp.procChance = 100;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			if( t )
				_unit->CastSpell( t, 110102, false );	//Sludge Spew
		}
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MOB_ENTRY_YORSAHJ			55312
#define MOB_ENTRY_GLOWING_G			55864
#define MOB_ENTRY_COBALT_G			55866
#define MOB_ENTRY_CRIMSON_G			55865
#define MOB_ENTRY_DARK_G			55867
#define MOB_ENTRY_ACID_G			55862
#define MOB_ENTRY_SHADOWED_G		55863    
#define MOB_ENTRY_MANA_VOID			56231    
#define MOB_ENTRY_FORGOTTEN_ONE		56265    
#define SPELL_ID_VOID_BOLT			104849
#define SPELL_COOL_VOID_BOLT		7000	//max 10 seconds so it can stack
#define SPELL_COOL_GLOBULE_SPAWN	20000   //every 1 minute 3 globules
#define SPELL_ID_FUSING_VAPORS		103635
#define SPELL_ID_GLOWING_BLOOD		104901
#define SPELL_ID_SEARING_BLOOD		105033
#define SPELL_ID_PSYCHIC_SLICE		108922
#define SPELL_ID_DIGESTIVE_ACID		105573
#define SPELL_ID_DEEP_CORRUPTION	105171


char YorsahjYells[8][89] = {
    "Ak'agthshi ma uhnish, ak'uq shg'cul vwahuhn! H'iwn iggksh Phquathi gag OOU KAAXTH SHUUL!",
    "Ez, Shuul'wah! Sk'woth'gl yu'gaz yoh'ghyl iilth!",
    "H'iwn zaix Shuul'wah, PHQUATHI!",
    "Iilth qi'uothk shn'ma yeh'glu Shath'Yar! H'IWN IILTH!",
    "KYTH ag'xig yyg'far IIQAATH ONGG!",
    "Shkul an'zig qvsakf KSSH'GA, ag'THYZAK agthu!",
    "Sk'yahf qi'plahf PH'MAGG!",
    "UULL lwhuk H'IWN!",
};

CreatureYell YorsahjWhispers[8] = {
	{ "All praise Deathwing, THE DESTROYER!", 26330 },
    { "From its BLEAKEST DEPTHS, we RECLAIM this world!", 26331 },
    { "O, Deathwing! Your faithful servant has failed you!", 26327 },
    { "Our numbers are endless, our power beyond reckoning! All who oppose the Destroyer will DIE A THOUSAND DEATHS!", 26328 },
    { "SEE how we pour from the CURSED EARTH!", 26334 },
    { "The DARKNESS devours ALL!", 26333 },
    { "You will drown in the blood of the Old Gods! ALL OF YOU!", 26326 },
    { "Your soul will know ENDLESS TORMENT!", 26329 }
};

class Yorsahj : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Yorsahj);
/* 
    Void Bolt – Yor'sahj the Unsleeping blasts his primary target with dark energy, dealing 83250 to 96750 Shadow damage initially and then 4625 to 5375 Shadow damage every 2 seconds. This effect stacks.
    Call Blood of Shu'ma – Yor'sahj the Unsleeping calls to the might of Shu'ma, summoning three differently colored globules of his blood. These globules path slowly towards Yor'sahj, infusing him with additional powers if they reach him.
    In Heroic difficulty four slimes answer Yor'sahj's call.
    
    Fusing Vapors – The blood globules emit a fusing vapor when damaged below 50% health that heals all other active globules for 5% of their maximum life.When one globule dies, the surviving globules completely fuse and gain immunity to all forms of damage.
    Glowing Globule – When a Glowing Globule reaches Yor'sahj, it infuses Yor'sahj with the Glowing Blood of Shu'ma.
        Glowing Blood of Shu'ma – Yor'sahj's Void Bolt now hits all nearby targets for 33300 to 38700 Shadow damage in addition to the primary target component, Yor'sahj uses his abilities twice as often, and his attack speed increases by 50%.
    Cobalt Globule – When a Cobalt Globule reaches Yor'sahj, it infuses Yor'sahj with the ability to summon Mana Voids.
        Mana Void – A Mana Void leeches all the mana from every ranged spell caster and healer, storing the mana within itself. Destroying the Mana Void evenly returns the total of the leeched mana via Mana Diffusion to players within 30 yards.
        Mana Void – A Mana Void leeches all the mana from every ranged spell caster and healer, storing the mana within itself. Destroying the Mana Void evenly returns the total of the leeched mana via Mana Diffusion to players within 100 yards.
    Crimson Globule – When a Crimson Globule reaches Yor'sahj, it infuses Yor'sahj with the Searing Blood ability.
        Searing Blood – Yor'sahj sears the blood of three random players, inflicting a base of 35000 Fire damage. The further the target stands from Yor'sahj, the greater shock received from the blast.
        In 25 player raids, Yor'sahj sears the blood of eight random players.
    Dark Globule – When a Dark Globule reaches Yor'sahj, the corruption of the blood seeps out into the Maw of Shu'ma. This vile corruption then periodically creates Forgotten Ones that attack the players.
        Forgotten One – These creatures fixate on a random target.
        Psychic Slice – The Forgotten One emits a wave of psychic force at its current target, inflicting 55770 to 58630 Shadow damage.
    Acidic Globule – When an Acidic Globule reaches Yor'sahj, the Maw of Shu'ma infuses with the acidic blood and stimulates further Digestive Acid production.
        Digestive Acid – Digestive Acid periodically secretes from the stomach lining, inflicting 58500 to 61500 Nature damage to a player and nearby allies within 4 yards.
        Digestive Acid – Digestive Acid periodically secretes from the stomach lining, inflicting 58500 to 61500 Nature damage to a random player.
    Shadowed Globule – When a Shadowed Globule reaches Yor'sahj, it infuses Yor'sahj with the Deep Corruption ability.
        Deep Corruption – Yor'sahj's Deep Corruption spreads to every player, causing every fifth healing or absorbtion effect cast on a player to trigger a violent detonation, inflicting 46250 to 53750 Shadow damage to all players.
*/
	uint32	YellIndex;
	uint32	WhisperIndex;
	uint32	TalkCooldown;
	uint32	phase;
	float	DifficultyScale;
	uint32	GlobuleSpawnTimer;
	uint32	GlobuleSpawnIndex;
	uint32	VoidBoltTimer;
    
	Yorsahj(Creature* pCreature) : CreatureAIScript(pCreature)
	{
        phase = 0;
	}
    
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		//make him attack faster as difficulty increases
		uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now * DifficultyScale ) );	//attack a bit faster cause there are more healers

		RegisterAIUpdateEvent( 1000 );
	}

	void OnDied(Unit *mKiller)
	{
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		*EncounterStatus |= EXTENSION_FLAG_YORSAHJ_DIED;
		_unit->GetMapMgr()->CreateCreature( MOB_ENTRY_DRAGONSOUL_TELEPORTER, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
		RemoveAIUpdateEvent();
		//try to see players lie about BOSS difficulty
		LOG_CREATURE_DEATH( _unit->GetEntry(), mKiller );
		if( mKiller && mKiller->IsPlayer() && SafePlayerCast( mKiller )->GetGuild() )
			SafePlayerCast( mKiller )->GetGuild()->GuildNewsAdd( GUILD_NEWS_LOG_DUNGEON_ENCOUNTER, mKiller->GetGUID(), 1295 );
    }

	void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			phase = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		if( phase == 0 )
		{
			TalkCooldown = TickNow + RandomUInt() % 25000;
			YellIndex = 0;
			WhisperIndex = 0;
			GlobuleSpawnIndex = 0;
			VoidBoltTimer = TickNow + RandomUInt() % 25000;
			GlobuleSpawnTimer = TickNow + RandomUInt() % 25000;
			phase = 1;
		}
		// Init phase
		if( phase == 1 )
		{
            if( TalkCooldown < TickNow )
            {
                if ( ( RandomUInt() % 3 ) == 0 )
                {
                    _unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, YorsahjYells[ YellIndex ] );
                    YellIndex = ( YellIndex + 1 ) % 8;
                    TalkCooldown = TickNow + (20000 + RandomUInt() % 10000);
                }
                else
                {                
                    _unit->SendChatMessage( CHAT_MSG_MONSTER_WHISPER, LANG_UNIVERSAL, YorsahjWhispers[ WhisperIndex ].txt );
					if( YorsahjWhispers[ WhisperIndex ].soundID > 0 )
						_unit->PlaySoundTarget( YorsahjWhispers[ WhisperIndex ].soundID, _unit->GetGUID() );
                    WhisperIndex = ( WhisperIndex + 1 ) % 8;
                    TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
                }
            }
            if( VoidBoltTimer < TickNow && _unit->isCasting() == false )
            {
				if( _unit->HasAura( SPELL_ID_GLOWING_BLOOD ) )
				{
					InRangePlayerSetRecProt::iterator itr,itr2;
					_unit->AquireInrangeLock(); //make sure to release lock before exit function !
					InrangeLoopExitAutoCallback AutoLock;
					for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
					{
						itr = itr2;
						itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
						if( !SafeUnitCast( *itr )->isAlive() )
							continue;
						_unit->CastSpell( *itr, SPELL_ID_VOID_BOLT, false );	
					}
					_unit->ReleaseInrangeLock();
	                VoidBoltTimer = TickNow + (SPELL_COOL_VOID_BOLT + RandomUInt() % SPELL_COOL_VOID_BOLT)*DifficultyScale;
				}
				else
				{
	                _unit->CastSpell( tank, SPELL_ID_VOID_BOLT, false );	
	                VoidBoltTimer = TickNow + ( (SPELL_COOL_VOID_BOLT + RandomUInt() % SPELL_COOL_VOID_BOLT)*DifficultyScale ) / 2;
				}
            }
			if( GlobuleSpawnTimer < TickNow )
			{
				_unit->RemoveAura( SPELL_ID_GLOWING_BLOOD, 0, AURA_SEARCH_ALL, MAX_PASSIVE_AURAS );
				for( uint32 i=0; i<3; i++ )
				{
					GlobuleSpawnIndex++;
#define GlobuleSpawnLocationCount 8
//					static float GlobuleSpawnX[GlobuleSpawnLocationCount] = { -1807, -1720, -1658, -1662, -1720, -1805, -1867, -1865 };
//					static float GlobuleSpawnY[GlobuleSpawnLocationCount] = { -3138, -3140, -3708, -2990, -2931, -2934, -2990, -3077 };
					static float GlobuleSpawnX[GlobuleSpawnLocationCount] = { -1788, -1743, -1722, -1716, -1747, -1783, -1823, -1820 };
					static float GlobuleSpawnY[GlobuleSpawnLocationCount] = { -2975, -2979, -3011, -3054, -3079, -3081, -3057, -3006 };
					LocationVector dst;
					dst.x = GlobuleSpawnX[ GlobuleSpawnIndex % GlobuleSpawnLocationCount ];
					dst.y = GlobuleSpawnY[ GlobuleSpawnIndex % GlobuleSpawnLocationCount ];
//					dst.z = -172;
					dst.z = -179;
					uint32 GlobulePicked = GlobuleSpawnIndex % 6;
					uint32 GlobuleTypes[ 6 ] = {MOB_ENTRY_GLOWING_G,MOB_ENTRY_COBALT_G,MOB_ENTRY_CRIMSON_G,MOB_ENTRY_DARK_G,MOB_ENTRY_ACID_G,MOB_ENTRY_SHADOWED_G};
					char GlobuleTypesNames[ 6 ][10] = { "Glowing", "Cobalt", "Crimson", "Dark", "Acid", "Shadowed" };
					uint32 GlobuleEntry = GlobuleTypes[ GlobulePicked ];
					Unit *cur_globule = _unit->create_guardian( GlobuleEntry, 600000, 0, NULL, &dst, NULL, false );
					char msg[50];
					sprintf( msg, " Spawning %s Globule", GlobuleTypesNames[ GlobulePicked ] );
					_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg );
				}
				GlobuleSpawnTimer = TickNow + (SPELL_COOL_GLOBULE_SPAWN + RandomUInt() % SPELL_COOL_GLOBULE_SPAWN);
			}
        }
	}
};

class Globule
{
public:
	void MakeOtherGlobulesImmune(Unit *_unit)
	{
		//make other globules immune
		InRangeSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( ( *itr )->IsUnit() == false || SafeUnitCast( *itr )->isAlive() == false )
				continue;
			Unit *tu = SafeUnitCast( ( *itr ) );
			//no self cast
			if( tu == _unit )
				continue;
			//any other globule
			if( tu->GetEntry() != MOB_ENTRY_GLOWING_G && tu->GetEntry() != MOB_ENTRY_CRIMSON_G
				 && tu->GetEntry() != MOB_ENTRY_COBALT_G && tu->GetEntry() != MOB_ENTRY_DARK_G
				 && tu->GetEntry() != MOB_ENTRY_ACID_G && tu->GetEntry() != MOB_ENTRY_SHADOWED_G )
				 continue;
			tu->bInvincible = true;
			tu->SetScale( tu->GetScale() / 2.0f );
		}
		_unit->ReleaseInrangeLock();
	}
	void CheckFuseWithOtherGlobules(Unit *_unit)
	{
		if( CastedFusingVapors == 0 && _unit->GetHealthPct() < 50 )
		{
			CastedFusingVapors = 1;
			InRangeSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( ( *itr )->IsUnit() == false || SafeUnitCast( *itr )->isAlive() == false )
					continue;
				Unit *tu = SafeUnitCast( ( *itr ) );
				//no self cast
				if( tu == _unit )
					continue;
				//any other globule
				if( tu->GetEntry() != MOB_ENTRY_GLOWING_G && tu->GetEntry() != MOB_ENTRY_CRIMSON_G
					 && tu->GetEntry() != MOB_ENTRY_COBALT_G && tu->GetEntry() != MOB_ENTRY_DARK_G
					 && tu->GetEntry() != MOB_ENTRY_ACID_G && tu->GetEntry() != MOB_ENTRY_SHADOWED_G )
					 continue;
				tu->CastSpell( tu, SPELL_ID_FUSING_VAPORS, true ); //Fusing Vapors - periodic max health heal
			}
			_unit->ReleaseInrangeLock();
		}
	}
	void InitGlobule( Unit *_unit )
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
		}
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
//		_unit->GetAIInterface()->SetFollowDistance( 1.0f );
		_unit->m_walkSpeed = 3.0f;
		_unit->m_runSpeed = _unit->m_walkSpeed;
		CastedFusingVapors = 0;
	}
	uint32 CastedFusingVapors;
};

class GlowingGlobule : public Globule, public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GlowingGlobule);

    GlowingGlobule(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 2000 );
		InitGlobule( _unit );
	}
	void OnDied(Unit *mKiller)
	{
		MakeOtherGlobulesImmune( _unit );
		RemoveAIUpdateEvent();
    }
    void AIUpdate()
    {
//    Fusing Vapors – The blood globules emit a fusing vapor when damaged below 50% health that heals all other active globules for 5% of their maximum life.When one globule dies, the surviving globules completely fuse and gain immunity to all forms of damage.
//    Glowing Globule – When a Glowing Globule reaches Yor'sahj, it infuses Yor'sahj with the Glowing Blood of Shu'ma.
//        Glowing Blood of Shu'ma – Yor'sahj's Void Bolt now hits all nearby targets for 33300 to 38700 Shadow damage in addition to the primary target component, Yor'sahj uses his abilities twice as often, and his attack speed increases by 50%.
		CheckFuseWithOtherGlobules( _unit );
		Unit *Yorsahj = _unit->GetTopOwner();
		//infuse our master ?
		if( Yorsahj != NULL && _unit->GetDistance2dSq( Yorsahj ) < 4.0f*4.0f )
		{
			Yorsahj->CastSpell( Yorsahj, SPELL_ID_GLOWING_BLOOD, true );
			_unit->Despawn( 1, 0 );
			RemoveAIUpdateEvent();
		}
	}
};

class CobaltGlobule : public Globule, public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CobaltGlobule);

	uint32 CastedFusingVapors;
    CobaltGlobule(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 2000 );
		InitGlobule( _unit );
	}
	void OnDied(Unit *mKiller)
	{
		MakeOtherGlobulesImmune( _unit );
		RemoveAIUpdateEvent();
    }
	void AIUpdate()
    {
		CheckFuseWithOtherGlobules( _unit );
		Unit *Yorsahj = _unit->GetTopOwner();
		//infuse our master ?
		if( Yorsahj != NULL && _unit->GetDistance2dSq( Yorsahj ) < 4.0f*4.0f )
		{
			Yorsahj->create_guardian( MOB_ENTRY_MANA_VOID, 600000, 0 , 0, NULL, NULL, false );
			_unit->Despawn( 1, 0 );
			RemoveAIUpdateEvent();
		}
	}
};

class CrimsonGlobule : public Globule, public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CrimsonGlobule);

	uint32 CastedFusingVapors;
    CrimsonGlobule(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 2000 );
		InitGlobule( _unit );
	}
	void OnDied(Unit *mKiller)
	{
		MakeOtherGlobulesImmune( _unit );
		RemoveAIUpdateEvent();
    }
	void AIUpdate()
    {
		CheckFuseWithOtherGlobules( _unit );
		Unit *Yorsahj = _unit->GetTopOwner();
		//infuse our master ?
		if( Yorsahj != NULL && _unit->GetDistance2dSq( Yorsahj ) < 4.0f*4.0f )
		{
			int32 CastCount = 3;
			if( _unit->GetMapMgr()->instance_difficulty & ( INSTANCE_FLAG_RAID_10_MEN_HEROIC | INSTANCE_FLAG_RAID_25_MEN_HEROIC ) )
				CastCount = 5;
			InRangePlayerSetRecProt::iterator itr,itr2;
			int32 TargetChance = 100 * 3 / ( 1 + _unit->GetInRangePlayerSet()->size() );
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( !SafeUnitCast( *itr )->isAlive() )
					continue;
				if( RandChance( TargetChance ) == false )
					continue;
				Yorsahj->CastSpell( *itr, SPELL_ID_SEARING_BLOOD, true );	
				CastCount--;
				if( CastCount<= 0 )
					break;
			}
			_unit->ReleaseInrangeLock();

			_unit->Despawn( 1, 0 );
			RemoveAIUpdateEvent();
		}
	}
};

class DarkGlobule : public Globule, public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DarkGlobule);

	uint32 CastedFusingVapors;
    DarkGlobule(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 2000 );
		InitGlobule( _unit );
	}
	void OnDied(Unit *mKiller)
	{
		MakeOtherGlobulesImmune( _unit );
		RemoveAIUpdateEvent();
    }
	void AIUpdate()
    {
		CheckFuseWithOtherGlobules( _unit );
		Unit *Yorsahj = _unit->GetTopOwner();
		//infuse our master ?
		if( Yorsahj != NULL && _unit->GetDistance2dSq( Yorsahj ) < 4.0f*4.0f )
		{
			Yorsahj->create_guardian( MOB_ENTRY_FORGOTTEN_ONE, 600000, 0, 0, 0, 0, false );
			Yorsahj->create_guardian( MOB_ENTRY_FORGOTTEN_ONE, 600000, 0, 0, 0, 0, false );
			Yorsahj->create_guardian( MOB_ENTRY_FORGOTTEN_ONE, 600000, 0, 0, 0, 0, false );
			Yorsahj->create_guardian( MOB_ENTRY_FORGOTTEN_ONE, 600000, 0, 0, 0, 0, false );
			if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			{
				Yorsahj->create_guardian( MOB_ENTRY_FORGOTTEN_ONE, 600000, 0, 0, 0, 0, false );
				Yorsahj->create_guardian( MOB_ENTRY_FORGOTTEN_ONE, 600000, 0, 0, 0, 0, false );
			}
			else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			{
				Yorsahj->create_guardian( MOB_ENTRY_FORGOTTEN_ONE, 600000, 0, 0, 0, 0, false );
				Yorsahj->create_guardian( MOB_ENTRY_FORGOTTEN_ONE, 600000, 0, 0, 0, 0, false );
				Yorsahj->create_guardian( MOB_ENTRY_FORGOTTEN_ONE, 600000, 0, 0, 0, 0, false );
				Yorsahj->create_guardian( MOB_ENTRY_FORGOTTEN_ONE, 600000, 0, 0, 0, 0, false );
			}
			_unit->Despawn( 1, 0 );
			RemoveAIUpdateEvent();
		}
	}
};

class AcidicGlobule : public Globule, public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AcidicGlobule);

	uint32 CastedFusingVapors;
    AcidicGlobule(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 2000 );
		InitGlobule( _unit );
	}
	void OnDied(Unit *mKiller)
	{
		MakeOtherGlobulesImmune( _unit );
		RemoveAIUpdateEvent();
    }
	void AIUpdate()
    {
		CheckFuseWithOtherGlobules( _unit );
		Unit *Yorsahj = _unit->GetTopOwner();
		//infuse our master ?
		if( Yorsahj != NULL && _unit->GetDistance2dSq( Yorsahj ) < 4.0f*4.0f )
		{
			InRangePlayerSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( !SafeUnitCast( *itr )->isAlive() )
					continue;
				Yorsahj->CastSpell( *itr, SPELL_ID_DIGESTIVE_ACID, true );
			}
			_unit->ReleaseInrangeLock();
			_unit->Despawn( 1, 0 );
			RemoveAIUpdateEvent();
		}
	}
};

class ShadowedGlobule : public Globule, public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShadowedGlobule);

	uint32 CastedFusingVapors;
    ShadowedGlobule(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 2000 );
		InitGlobule( _unit );
	}
	void OnDied(Unit *mKiller)
	{
		MakeOtherGlobulesImmune( _unit );
		RemoveAIUpdateEvent();
    }
	void AIUpdate()
    {
		CheckFuseWithOtherGlobules( _unit );
		Unit *Yorsahj = _unit->GetTopOwner();
		//infuse our master ?
		if( Yorsahj != NULL && _unit->GetDistance2dSq( Yorsahj ) < 4.0f*4.0f )
		{
			InRangePlayerSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( !SafeUnitCast( *itr )->isAlive() )
					continue;
				Yorsahj->CastSpell( *itr, SPELL_ID_DEEP_CORRUPTION, true );	
			}
			_unit->ReleaseInrangeLock();

			_unit->Despawn( 1, 0 );
			RemoveAIUpdateEvent();
		}
	}
};

class ManaVoid : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ManaVoid);

	uint32 StoredMana,StoredPlayerCount;
    ManaVoid(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->m_canFlee = true;
		_unit->GetAIInterface()->setCurrentAgent( AGENT_FLEE );
		StoredMana = 0;
		StoredPlayerCount = 0;
		float min_dst = 30.0f * 30.0f;
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( !SafeUnitCast( *itr )->isAlive() || ( *itr )->IsPlayer() == false )
				continue;
			if( ( *itr )->GetPowerType() == POWER_TYPE_MANA && Distance2DSq( _unit, (*itr) ) < min_dst )
			{
				StoredPlayerCount++;
				int32 ManaStealNow = (*itr)->GetPower( POWER_TYPE_MANA );
				StoredMana += ManaStealNow;
				_unit->Energize( (*itr), 105539, -ManaStealNow, POWER_TYPE_MANA, 0 ); //mana diffusion
				(*itr)->SetPower( POWER_TYPE_MANA, 0 );
			}
		}
		_unit->ReleaseInrangeLock();
	}
	void OnDied(Unit *mKiller)
	{
		if( StoredMana > 0 && StoredPlayerCount > 0 )
		{
			uint32 ManaPerPlayer = StoredMana / StoredPlayerCount;
			float min_dst = 30.0f * 30.0f;
			InRangePlayerSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( !SafeUnitCast( *itr )->isAlive() || ( *itr )->IsPlayer() == false )
					continue;
				if( ( *itr )->GetPowerType() == POWER_TYPE_MANA && Distance2DSq( _unit, (*itr) ) < min_dst )
					(*itr)->Energize( (*itr), 110742, ManaPerPlayer, POWER_TYPE_MANA, 0 ); //Mana Diffusion
			}
			_unit->ReleaseInrangeLock();
		}
		StoredMana = 0;
		RemoveAIUpdateEvent();
    }
};

class ForgottenOne : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ForgottenOne);

    ForgottenOne(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
		}
		AI_Spell sp;
		sp.spelltargetType = TTYPE_AT_RANGE_PLAYER_RANDOM;
		sp.minrange = 10.0f;
		sp.maxrange = 60.0f;
		sp.procChance = 20;
		Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
		if( t )
			_unit->GetAIInterface()->AttackReaction( t, 10, 0 );
		RegisterAIUpdateEvent( 2000 );
	}
    void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
			return;
		if( _unit->isAlive() == false )
			return;
		if( tank->GetDistance2dSq( _unit ) <= 4.0f*4.0f )
			_unit->CastSpell( tank, SPELL_ID_PSYCHIC_SLICE, true );
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SPELL_ID_ICE_LANCE_NORMAL		105313
#define SPELL_ID_ICE_LANCE_HEROIC		105313
#define SPELL_COOL_ICE_LANCE			10000
#define MOB_ENTRY_ICE_TOMB				36980
#define SPELL_ID_ICE_TOMB_STUN			103251
#define SPELL_COOL_ICE_TOMB				60000	//not sure this is casted more then once
#define SCRIPT_TYPE_HAGARA				SPELL_HASH_ICE_LANCE
#define SPELL_ID_SHATTERED_ICE			105289
#define SPELL_COOL_SHATTERED_ICE		10000
#define SPELL_ID_FOCUSED_ASSAULT		107851
#define SPELL_COOL_FOCUSED_ASSAULT		10000
#define NPC_ID_BINDING_CRYSTAL			56136
#define NPC_ID_ICE_WAVE					56104
#define SPELL_ID_FROZEN_TEMPEST			105256
#define SPELL_ID_FROZEN_TEMPEST_JUST_IMMUNE			109552
#define SPELL_ID_WATERY_ENCHANTMENT		110317
#define SPELL_COOL_WATERY_ENTRANCHMENT	1000	//site says 5 seconds, aura says 1 second
#define SPELL_ID_ICICLE					69425 //spawn mob 36847 that deals dmg near him 
#define SPELL_COOL_ICICLE				20000
#define SPELL_ID_FROST_FLAKE			109325
#define SPELL_COOL_FROST_FLAKE			20000
#define NPC_ID_LIGHTNING_CONDUIT		56165	
#define NPC_ID_BOUND_LIGHTNING_ELEMENTAL 56700
#define SPELL_ID_LIGHTNING_STORM		108568
#define SPELL_COOL_LIGHTNING_STORM		5000
#define SPELL_ID_LIGHTNING_CHAIN		105367
#define SPELL_ID_LIGHTNING_PILLAR		109557
#define SPELL_COOL_LIGHTNING_PILLAR		11000
#define SPELL_ID_LIGHTNING_OVERLOAD		105481 //target hagara
#define SPELL_COOL_LIGHTNING_ELEMENTAL	10000

CreatureYell HagaraYells[34] = {
	{ "A waste of my time.", 26257 },
    { "Again?!", 26239 },
    { "Aughhhh!", 26238 },
    { "Cowards! You pack of weakling... dogs...", 26243 },
    { "Die beneath the ice.", 26248 },
    { "Don't preen just yet, little pups. We'll cleanse this world of your kind.", 26224 },
    { "Down, pup.", 26256 },
    { "Enough of your games! You won't live to do it again.", 0 },
    { "Enough!", 26237 },
    { "Even with the Aspect of Time on your side, you stumble foolishly into a trap?", 26223 },
    { "Feel a chill up your spine...?", 26246 },
    { "Feh!", 26254 },
    { "Hold still.", 26250 },
    { "I'll finish you now, pups!", 0 },
    { "Impossible!", 26231 },
    { "Impudent pup!", 26240 },
    { "No! More... lightning...", 0 },
    { "Not one of you will live to see the final cataclysm! Finish them!", 26226 },
    { "See what becomes of those who stand before me!", 26245 },
    { "Stay, pup.", 26249 },
    { "Suffer the storm's wrath!", 26252 },
    { "Swagger all you like; you pups don't stand a chance. Flee now, while you can.", 26251 },
    { "The one remaining is still enough to finish you.", 26241 },
    { "The time I spent binding that, WASTED!", 26235 },
    { "Thunder and lightning dance at my call!", 26234 },
    { "What are you doing?", 26228 },
    { "You can't outrun the storm.", 26247 },
    { "You cross the Stormbinder! I'll slaughter you all.", 26227 },
    { "You face more than my axes, this close.", 26244 },
    { "You should have run, dog.", 26255 },
    { "You think you can play with my lightning?", 26230 },
    { "You'll not leave this place alive!", 26225 },
    { "You'll PAY for that.", 26236 },
    { "You're toying with death.", 26229 }
};

class Hagara : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Hagara);
/* 
    Ice Lance – Hagara conjures crystals of ice that launch Ice Lance missiles towards a random player standing near Hagara. Ice Lances inflict 15000 Frost damage in a 3 yard area around the first player the missile impacts.
        In Normal difficulty, Ice Lance reduces the attack speed of all targets struck by 25%.
        In Heroic difficulty, Ice Lance increases frost damage taken by 25% per hit.
    Ice Tomb – Hagara traps two random players in a tomb of ice, stunning the targets until other players destroy the Ice Tombs.
        In 25 player Normal difficulty Hagara traps five random players.
        In 25 player Heroic difficulty Hagara traps six random players.
    Shattered Ice – Hagara launches a blast of ice at a random player, inflicting 72150 to 83850 Frost damage and reducing the player's movement speed for 4 sec.
    Focused Assault – Hagara rapidly slices at her current target, swinging for 50% normal weapon damage every 0.5 sec. for 5 sec.
    Frozen Tempest – Hagara channels and fills the platform with a tempest of water and ice. The Tempest protects Hagara from all attacks and spells.
        Binding Crystal – Destroying all four of the binding crystals stops Hagara's channeling of Frozen Tempest.
        Watery Entrenchment – Any player standing within Hagara's water bubble drowns, inflicting damage equal to 12% of their maximum health every second and reducing their movement speed by 50%.
        Ice Wave – Four equidistant lines of ice extend out from Hagara and sweep across the platform. The Ice Wave inflicts 190000 to 210000 Frost damage on any player caught by its movement and increases their Frost damage taken by 50% for 2 sec.
        Icicle – Snowpacked Icicles fall around the edge of the platform. Where the Icicle impacts the ground, it inflicts 100000 Frost damage and knocksback any nearby players within 7 yards.
        HeroicFrostflake – Hagara places a Frostflake on a random player, reducing their movement speed by 10% and an additional 10% every 1 sec for 15 sec. When the Frostflake effect expires, a 10 yard radius area of frost forms underneath that player. The frost reduces any player's movement speed standing within the area by 50%.
    Lightning Storm – Hagara channels a lightning storm to electrocute the players on the platform. A Water Shield protects Hagara from all attacks and spells.When players overload every Crystal Conductor, the feedback stops her channeling and causes her to suffer an Electrocution.
        Electrocution – The feedback from the overloaded Crystal Conductors stuns Hagara and increases her damage taken by 100% for 15 sec.
        Crystal Conductor – Hagara conjures four Crystal Conductors to protect herself from the lightning storm.
            In 10 player Heroic Difficulty, Hagara conjures eight Crystal Conductors.
        Lightning Conduit – A Crystal Conductor charged by the death of a Bound Lightning Elemental casts Lightning Conduit on the nearest player, inflicting 10000 Nature damage every 1 sec.Lightning Conduit also connects the affected player to another player or Crystal Conductor within 10 yards.
        Bound Lightning Elementals – Hagara calls elementals to aid her during the Lightning Storm. When a player kills an elemental, it causes a nearby Crystal Conductor to overload and electrocute nearby players.
        HeroicStorm Pillars – Hagara targets the location of a random player, then after 3 seconds a pillar of lightning erupts. The lightning inflicts 35000 Nature damage to all players within 5 yards.    
*/
	uint32	YellIndex;
	uint32	TalkCooldown;
	uint32	phase;
	float	DifficultyScale;
	uint32	IceLanceTimer;
	uint32	IceTombTimer;
	uint32	ShatteredIceTimer;
	uint32	FocusedAssaultTimer;
	uint32	IcicleTimer;
	uint32	SnowFlakeTimer;
	uint32	WateryEntranchmentTimer;
	uint32	RootTimer;
	uint64	IceWaveGUID;
	uint32	LightningPillarTimer;
	uint32	LightningStormTimer,LightningStormTargetIndex;
	uint32	LightningElementalTimer;
    
	Hagara(Creature* pCreature) : CreatureAIScript(pCreature)
	{
        phase = 0;
		ScriptTypeID = SCRIPT_TYPE_HAGARA;
	}
    
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		//make him attack faster as difficulty increases
		uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now * DifficultyScale ) );	//attack a bit faster cause there are more healers
		IceWaveGUID = 0;

		_unit->GetAIInterface()->setOutOfCombatRange( 100.0f * 100.0f );

		RegisterAIUpdateEvent( 1000 );
	}

	void OnDied(Unit *mKiller)
	{
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		*EncounterStatus |= EXTENSION_FLAG_HAGARA_DIED;
		_unit->GetMapMgr()->CreateCreature( MOB_ENTRY_DRAGONSOUL_TELEPORTER, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
		RemoveAIUpdateEvent();
		//try to see players lie about BOSS difficulty
		LOG_CREATURE_DEATH( _unit->GetEntry(), mKiller );
		if( mKiller && mKiller->IsPlayer() && SafePlayerCast( mKiller )->GetGuild() )
			SafePlayerCast( mKiller )->GetGuild()->GuildNewsAdd( GUILD_NEWS_LOG_DUNGEON_ENCOUNTER, mKiller->GetGUID(), 1296 );
    }

	void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			phase = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		// Init phase
		if( phase == 0 )
		{
			TalkCooldown = TickNow + RandomUInt() % 25000;
			YellIndex = 0;
            phase = 1;
			IceLanceTimer = TickNow + RandomUInt() % 25000;
			IceTombTimer = TickNow + RandomUInt() % 25000;
			ShatteredIceTimer = TickNow + RandomUInt() % 25000;
			FocusedAssaultTimer = TickNow + RandomUInt() % 25000;
			IcicleTimer = TickNow + RandomUInt() % 25000;
			SnowFlakeTimer = TickNow + RandomUInt() % 25000;
			LightningPillarTimer = TickNow + RandomUInt() % 25000;
			LightningStormTimer = TickNow + RandomUInt() % 25000;			
			LightningElementalTimer = TickNow + RandomUInt() % 25000;
			WateryEntranchmentTimer = TickNow + RandomUInt() % 25000;
			LightningStormTargetIndex = 0;
			RootTimer = 0;
			int64 *PhaseStatus = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_BINDING_CRYSTAL );
			*PhaseStatus = 0;
//			int64 *PillarsRemaining = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_BINDING_CRYSTAL + 1 );
//			*PillarsRemaining = 0;
			PhaseStatus = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_LIGHTNING_CONDUIT );
			*PhaseStatus = 0;
//			PillarsRemaining = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_LIGHTNING_CONDUIT + 1 );
//			*PillarsRemaining = 0;
			_unit->GetAIInterface()->m_canMove = true;
			//cancel any in cast progress spells
			_unit->InterruptSpell( 0, 0 );
			_unit->RemoveAura( SPELL_ID_FROZEN_TEMPEST, 0, AURA_SEARCH_ALL, MAX_PASSIVE_AURAS );
			_unit->RemoveAura( SPELL_ID_LIGHTNING_STORM, 0, AURA_SEARCH_ALL, MAX_PASSIVE_AURAS );
			_unit->RemoveAura( SPELL_ID_FROZEN_TEMPEST_JUST_IMMUNE, 0, AURA_SEARCH_ALL, MAX_PASSIVE_AURAS );
			Creature *IceWave = _unit->GetMapMgr()->GetCreature( IceWaveGUID );
			if( IceWave )
				IceWave->Despawn( 1, 0 );		}

		if( TalkCooldown < TickNow )
        {
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, HagaraYells[ YellIndex ].txt );
			if( HagaraYells[ YellIndex ].soundID > 0 )
				_unit->PlaySoundTarget( HagaraYells[ YellIndex ].soundID, _unit->GetGUID() );
            YellIndex = ( YellIndex + 1 ) % 34;
            TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
        }

		//time to progress to next phase ?
		uint32 healthPCT = _unit->GetHealthPct();
		if( phase == 1 && healthPCT < 80 && healthPCT > 50 )
			phase = 2;
		else if( phase == 3 && healthPCT < 30 )
			phase = 4;

		//general AI phase
        if ( phase == 1 || phase == 3 || phase == 5 )
        {
			//double DPS but rooted in place
			if( FocusedAssaultTimer < TickNow && _unit->isCasting() == false )
			{
				_unit->CastSpell( tank, SPELL_ID_FOCUSED_ASSAULT, false );	
				FocusedAssaultTimer = TickNow + (SPELL_COOL_FOCUSED_ASSAULT + RandomUInt() % SPELL_COOL_FOCUSED_ASSAULT)*DifficultyScale;
				RootTimer = TickNow + 4000;
				_unit->GetAIInterface()->m_canMove = false;
			}
			//comes from focused asault
			if( RootTimer < TickNow )
				_unit->GetAIInterface()->m_canMove = true;
			//disable 5 players until iceblock gets destroyed
			if( IceTombTimer < TickNow && _unit->isCasting() == false )
			{
				//get N player that has not yet been tommed
				uint32 PlayersToFreez = 3;
				if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
					PlayersToFreez = 5;
				else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
					PlayersToFreez = 6;

				//select players that were not yet entombed. Get max freezed number
				uint32 MaxFreezCounter = 0, MaxFreezCounterPrev = 0;
				InRangePlayerSetRecProt::iterator itr,itr2;
				_unit->AquireInrangeLock(); //make sure to release lock before exit function !
				InrangeLoopExitAutoCallback AutoLock;
				for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
				{
					itr = itr2;
					itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
					if( !SafeUnitCast( *itr )->isAlive() || ( *itr )->IsPlayer() == false )
						continue;
					int64 *t = ( *itr )->GetCreateIn64Extension( SPELL_HASH_ICE_TOMB );
					if( *t > MaxFreezCounter )
						MaxFreezCounter = *t;
					if( *t > MaxFreezCounterPrev && *t < MaxFreezCounter )
						MaxFreezCounterPrev = *t;
				}
				//now cast the spell on some players that do not have this max value
				for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
				{
					itr = itr2;
					itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
					if( !SafeUnitCast( *itr )->isAlive() || ( *itr )->IsPlayer() == false )
						continue;
					int64 *t = ( *itr )->GetCreateIn64Extension( SPELL_HASH_ICE_TOMB );
					if( *t <= MaxFreezCounterPrev && ( *itr )->HasAura( SPELL_ID_ICE_TOMB_STUN ) == false )
					{
						_unit->GetMapMgr()->CreateCreature( MOB_ENTRY_ICE_TOMB, true, ( *itr )->GetPositionX(), ( *itr )->GetPositionY(), ( *itr )->GetPositionZ() );
						PlayersToFreez--;
						if( PlayersToFreez <= 0 )
							break;
					}
				}
				_unit->ReleaseInrangeLock();
				IceTombTimer = TickNow + (SPELL_COOL_ICE_TOMB + RandomUInt() % SPELL_COOL_ICE_TOMB)*DifficultyScale;
			}/**/
			//large damage
			if( ShatteredIceTimer < TickNow && _unit->isCasting() == false )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_AT_RANGE_PLAYER_RANDOM;
				sp.minrange = 0.0f;
				sp.maxrange = 40.0f;
				sp.procChance = 100;
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				if( t )
				{
					_unit->CastSpell( t, SPELL_ID_SHATTERED_ICE, false );	
					ShatteredIceTimer = TickNow + (SPELL_COOL_SHATTERED_ICE + RandomUInt() % SPELL_COOL_SHATTERED_ICE)*DifficultyScale;
				}
				else
					ShatteredIceTimer = TickNow + 2000;
			}
			//slow targets
			if( IceLanceTimer < TickNow && _unit->isCasting() == false )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_AT_RANGE_PLAYER_RANDOM;
				sp.minrange = 0.0f;
				sp.maxrange = 60.0f;
				sp.procChance = 100;
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				if( t )
				{
					if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
						_unit->CastSpell( t, SPELL_ID_ICE_LANCE_HEROIC, false );	
					else
						_unit->CastSpell( t, SPELL_ID_ICE_LANCE_NORMAL, false );	
					IceLanceTimer = TickNow + (SPELL_COOL_ICE_LANCE + RandomUInt() % SPELL_COOL_ICE_LANCE)*DifficultyScale;
				}
				else
					IceLanceTimer = TickNow + 2000;
			}
        }
		//ice phase : 
		//- move to the center of the room
		//- Cast huge buble that has a very strong DOT
		//- summon an ice spike that circles around hagara. Insta kills anyone who gets near it
		//- summon 4 pillons that need to get destroyed
        else if (phase == 2)
        {
			int64 *PhaseStatus = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_BINDING_CRYSTAL );
			int64 *PillarsRemaining = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_BINDING_CRYSTAL + 1 );
			//init phase
			if( *PhaseStatus == 0 )
			{
				//move to the center
				if( _unit->GetAIInterface() )
				{
					//move her to the center of the room
					_unit->GetAIInterface()->MoveTo( 13588, 13612, 123, 0 );
					_unit->SetPosition( 13588, 13612, 123, 0, true );
					_unit->GetAIInterface()->StopMovement( 0 );
					_unit->GetAIInterface()->m_canMove = false;
				}
				//the objective to get these destroyed
				_unit->GetMapMgr()->CreateCreature( NPC_ID_BINDING_CRYSTAL, true, 13587, 13570, 124 );
				_unit->GetMapMgr()->CreateCreature( NPC_ID_BINDING_CRYSTAL, true, 13627, 13611, 124 );
				_unit->GetMapMgr()->CreateCreature( NPC_ID_BINDING_CRYSTAL, true, 13587, 13653, 124 );
				_unit->GetMapMgr()->CreateCreature( NPC_ID_BINDING_CRYSTAL, true, 13546, 13612, 124 );
				*PillarsRemaining = 4;
				//interrupt channeled focus assault or it will visual bug
				_unit->InterruptSpell( 0, 0 );
				//buble with insane DOT
				_unit->CastSpellDelayed( _unit->GetGUID(), SPELL_ID_FROZEN_TEMPEST, 1000 ); //this will last for like 30 seconds ( channeled )
				_unit->SetUInt32Value( UNIT_CHANNEL_SPELL, SPELL_ID_FROZEN_TEMPEST );
				//the NPC that moves clockwise at the edge of the platform
				Creature *IceWave = _unit->GetMapMgr()->CreateCreature( NPC_ID_ICE_WAVE, true, 13628, 13612, 124 );
				if( IceWave )
					IceWaveGUID = IceWave->GetGUID();
				//cast spells phase
				*PhaseStatus = 1;
			}
			//cast spells phase
			if( *PillarsRemaining > 0 )
			{
				//10% weak slow with knockback
/*				if( IcicleTimer < TickNow && _unit->isCasting() == false )
				{
					AI_Spell sp;
					sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
					sp.minrange = 0.0f;
					sp.maxrange = 60.0f;
					sp.procChance = 100;
					Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
					if( t )
					{
						_unit->CastSpell( t, SPELL_ID_ICICLE, true );	
						IcicleTimer = TickNow + (SPELL_COOL_ICICLE + RandomUInt() % SPELL_COOL_ICICLE)*DifficultyScale;
					}
					else
						IcicleTimer = TickNow + 2000;
				}/**/
				//max 50% strong slow
				if( SnowFlakeTimer < TickNow && _unit->isCasting() == false )
				{
					AI_Spell sp;
					sp.spelltargetType = TTYPE_AT_RANGE_PLAYER_RANDOM;
					sp.minrange = 0.0f;
					sp.maxrange = 60.0f;
					sp.procChance = 100;
					Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
					if( t )
					{
						_unit->CastSpell( t, SPELL_ID_FROST_FLAKE, true );	
						SnowFlakeTimer = TickNow + (SPELL_COOL_FROST_FLAKE + RandomUInt() % SPELL_COOL_FROST_FLAKE)*DifficultyScale;
					}
					else
						SnowFlakeTimer = TickNow + 2000;
				}/**/
				if( WateryEntranchmentTimer < TickNow && _unit->isCasting() == false )
				{
					_unit->CastSpell( tank, SPELL_ID_WATERY_ENCHANTMENT, true );	
					_unit->CastSpell( tank, SPELL_ID_WATERY_ENCHANTMENT, true );	
					WateryEntranchmentTimer = TickNow + (SPELL_COOL_WATERY_ENTRANCHMENT + RandomUInt() % SPELL_COOL_WATERY_ENTRANCHMENT)*DifficultyScale;
				}
			}
			//all the cristalls have been destroyed : deinit
			if( *PillarsRemaining <= 0 )
			{
				Creature *IceWave = _unit->GetMapMgr()->GetCreature( IceWaveGUID );
				if( IceWave )
					IceWave->Despawn( 1, 0 );
				//enter normal combat phase again
				phase = 3;
				_unit->GetAIInterface()->m_canMove = true;
				//cancel any in cast progress spells
				_unit->InterruptSpell( 0, 0 );
				_unit->RemoveAura( SPELL_ID_FROZEN_TEMPEST, 0, AURA_SEARCH_ALL, MAX_PASSIVE_AURAS );
				_unit->SetUInt32Value( UNIT_CHANNEL_SPELL, 0 );
			}
        }
		//electric phase
		//- move to the center of the room
		//- Cast buble that grants immunity
		//- summon 4 pillons that need to get destroyed
		//- channel lightning storm that adamages players
		else if( phase == 4 )
		{
			int64 *PhaseStatus = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_LIGHTNING_CONDUIT );
			int64 *PillarsRemaining = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_LIGHTNING_CONDUIT + 1 );
			//init phase
			if( *PhaseStatus == 0 )
			{
				//move to the center
				if( _unit->GetAIInterface() )
				{
					//move her to the center of the room
					_unit->GetAIInterface()->MoveTo( 13588, 13612, 123, 0 );
					_unit->SetPosition( 13588, 13612, 123, 0, true );
					_unit->GetAIInterface()->StopMovement( 0 );
					_unit->GetAIInterface()->m_canMove = false;
				}
				//the objective to get these destroyed
				if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
				{
					_unit->GetMapMgr()->CreateCreature( NPC_ID_LIGHTNING_CONDUIT, true, 13587, 13570, 124 );
					_unit->GetMapMgr()->CreateCreature( NPC_ID_LIGHTNING_CONDUIT, true, 13627, 13611, 124 );
					_unit->GetMapMgr()->CreateCreature( NPC_ID_LIGHTNING_CONDUIT, true, 13587, 13653, 124 );
					_unit->GetMapMgr()->CreateCreature( NPC_ID_LIGHTNING_CONDUIT, true, 13546, 13612, 124 );

					_unit->GetMapMgr()->CreateCreature( NPC_ID_LIGHTNING_CONDUIT, true, 13616, 13581, 124 );
					_unit->GetMapMgr()->CreateCreature( NPC_ID_LIGHTNING_CONDUIT, true, 13558, 13581, 124 );
					_unit->GetMapMgr()->CreateCreature( NPC_ID_LIGHTNING_CONDUIT, true, 13557, 13643, 124 );
					_unit->GetMapMgr()->CreateCreature( NPC_ID_LIGHTNING_CONDUIT, true, 13617, 13643, 124 );
					*PillarsRemaining = 8;
				}
				else
				{
					_unit->GetMapMgr()->CreateCreature( NPC_ID_LIGHTNING_CONDUIT, true, 13587, 13570, 124 );
					_unit->GetMapMgr()->CreateCreature( NPC_ID_LIGHTNING_CONDUIT, true, 13627, 13611, 124 );
					_unit->GetMapMgr()->CreateCreature( NPC_ID_LIGHTNING_CONDUIT, true, 13587, 13653, 124 );
					_unit->GetMapMgr()->CreateCreature( NPC_ID_LIGHTNING_CONDUIT, true, 13546, 13612, 124 );
					*PillarsRemaining = 4;
				}
				//buble that shields us from dmg
//				_unit->CastSpell( _unit, SPELL_ID_LIGHTNING_STORM, true );
//				_unit->SetUInt32Value( UNIT_CHANNEL_SPELL, SPELL_ID_LIGHTNING_STORM );
//				_unit->SetUInt32Value( UNIT_CHANNEL_SPELL, 105466 ); //Lightning Storm Cosmetic
				//put immunity buble on us. But avoid having water bubble
				_unit->CastSpellDelayed( _unit->GetGUID(), SPELL_ID_FROZEN_TEMPEST_JUST_IMMUNE, 1000 ); //this will last for like 30 seconds ( channeled )
				_unit->SetUInt32Value( UNIT_CHANNEL_SPELL, SPELL_ID_FROZEN_TEMPEST_JUST_IMMUNE );
				//cast spells phase
				*PhaseStatus = 1;
			}
			//cast spells phase
			if( *PillarsRemaining > 0 )
			{
				//10% weak slow with knockback
				if( LightningPillarTimer < TickNow )
				{
					AI_Spell sp;
					sp.spelltargetType = TTYPE_AT_RANGE_PLAYER_RANDOM;
					sp.minrange = 0.0f;
					sp.maxrange = 60.0f;
					sp.procChance = 100;
					Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
					if( t )
					{
//						_unit->CastSpell( t, SPELL_ID_LIGHTNING_PILLAR, true );	
						t->CastSpell( t, 109563, true );	//dmg
//						_unit->CastSpell( t, 109557, true );	//dummy trigger
//						_unit->CastSpell( t, 109541, true );	//trigger the dmg spell
						LightningPillarTimer = TickNow + (SPELL_COOL_LIGHTNING_PILLAR + RandomUInt() % SPELL_COOL_LIGHTNING_PILLAR)*DifficultyScale;
					}
					else
						LightningPillarTimer = TickNow + 2000;
				}
				//mostly cosmetic lightning storm - makes lightning spark from you as you move
				if( LightningStormTimer < TickNow )
				{
					uint32 TargetsHits = 0;
					uint32 RequiredTargetHits = 4;
					LightningStormTargetIndex = LightningStormTargetIndex % ( 1 + _unit->GetInRangeCount() );
					for( uint32 i=0;i<8;i++)
					{
/*						AI_Spell sp;
						sp.spelltargetType = TTYPE_AT_RANGE_PLAYER_RANDOM;
						sp.minrange = 0.0f;
						sp.maxrange = 60.0f;
//						sp.procChance = 100 / 8;
						Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
						if( t )
							_unit->CastSpell( t, SPELL_ID_LIGHTNING_STORM, true );	
						*/
						uint32 SkippsMade = 0;
						for(uint32 LoopTargets=0;LoopTargets<8;LoopTargets++)
						{
							InRangeSetRecProt::iterator itr,itr2;
							_unit->AquireInrangeLock(); //make sure to release lock before exit function !
							InrangeLoopExitAutoCallback AutoLock;
							for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
							{
								itr = itr2;
								itr2++; 
								if( (*itr)->IsUnit() == false || SafeUnitCast( *itr )->isAlive() == false )
									continue;
								//try avoiding striking the same target over and over again
								if( SkippsMade < LightningStormTargetIndex )
								{
									SkippsMade++;
									continue;
								}
								TargetsHits++;
								_unit->CastSpell( SafeUnitCast( (*itr) ), SPELL_ID_LIGHTNING_STORM, true );
								if( TargetsHits > RequiredTargetHits )
									break;
							}
							_unit->ReleaseInrangeLock();
							if( TargetsHits > RequiredTargetHits )
								break;
						}
						if( TargetsHits > RequiredTargetHits )
							break;
					}
					LightningStormTargetIndex += TargetsHits;
					LightningStormTimer = TickNow + (SPELL_COOL_LIGHTNING_STORM + RandomUInt() % SPELL_COOL_LIGHTNING_STORM)*DifficultyScale;
				}/**/
				//once this is killed he will trigger a chain shock that can dmg conductors
				if( LightningElementalTimer < TickNow )
				{
					//avoid overspawning the map, count the number of active elementals near us
					uint32 AliveElementals = 0;
					InRangeSetRecProt::iterator itr,itr2;
					_unit->AquireInrangeLock(); //make sure to release lock before exit function !
					InrangeLoopExitAutoCallback AutoLock;
					for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
					{
						itr = itr2;
						itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
						if( (*itr)->IsUnit() == false || SafeUnitCast( *itr )->isAlive() == false )
							continue;
						if( (*itr)->GetEntry() != NPC_ID_BOUND_LIGHTNING_ELEMENTAL )	
							continue;
						AliveElementals++;
					}
					_unit->ReleaseInrangeLock();
					if( AliveElementals < 20 / DifficultyScale )
						_unit->GetMapMgr()->CreateCreature( NPC_ID_BOUND_LIGHTNING_ELEMENTAL, true, 13628 - 5 + RandomUInt( 10 ), 13612 - 5 + RandomUInt( 10 ), 124 );
					LightningElementalTimer = TickNow + (SPELL_COOL_LIGHTNING_ELEMENTAL + RandomUInt() % SPELL_COOL_LIGHTNING_ELEMENTAL)*DifficultyScale;
				}
			}
			//all the cristalls have been destroyed : deinit
			if( *PillarsRemaining <= 0 )
			{
				//enter normal combat phase again
				phase = 5;
				_unit->GetAIInterface()->m_canMove = true;
				_unit->RemoveAura( SPELL_ID_LIGHTNING_STORM, 0, AURA_SEARCH_ALL, MAX_PASSIVE_AURAS );
				_unit->RemoveAura( SPELL_ID_FROZEN_TEMPEST_JUST_IMMUNE, 0, AURA_SEARCH_ALL, MAX_PASSIVE_AURAS );
			}
		}
	}
};


class IceTomb : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(IceTomb);

   uint64 MyStunTarget;
    IceTomb(Creature* pCreature) : CreatureAIScript(pCreature)
    {
    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 2000 );
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
		}
		MyStunTarget = 0;
		Unit *owner = _unit->GetTopOwner();
		if( owner != NULL )
		{
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, owner->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			_unit->_setFaction( );
		}
		Player *ClosestP = NULL;
		float distance = 100000;
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++;
			if( !SafeUnitCast( *itr )->isAlive() )
				continue;
			float tdist = (*itr)->GetDistance2dSq( _unit );
			if( tdist < distance && ( *itr )->HasAura( SPELL_ID_ICE_TOMB_STUN ) == false )
			{
				distance = tdist;
				ClosestP = (*itr);
			}
			break;
		}
		_unit->ReleaseInrangeLock();
		if( ClosestP )
		{
			ClosestP->CastSpell( ClosestP, SPELL_ID_ICE_TOMB_STUN, true ); //self stun to avoid resist
			MyStunTarget = ClosestP->GetGUID();
			RemoveAIUpdateEvent();
		}
	}
	void OnDied(Unit *mKiller)
	{
		if( MyStunTarget )
		{
			Player *p = _unit->GetMapMgr()->GetPlayer( MyStunTarget );
			if( p )
				p->RemoveAura( SPELL_ID_ICE_TOMB_STUN, 0, AURA_SEARCH_ALL, MAX_PASSIVE_AURAS );
		}
		RemoveAIUpdateEvent();
	}
};

//just stand around and wait until we get destroyed :(
class FrozenBindingCrystal : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FrozenBindingCrystal);

    FrozenBindingCrystal(Creature* pCreature) : CreatureAIScript(pCreature) { }
	void OnLoad()
	{
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
		}
		if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
		{
			_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, 40058 );	//this has an 30% chance to be spawned invisible if using blizz templates
			_unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 2 * _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			_unit->SetUInt32Value( UNIT_FIELD_HEALTH, _unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
		}
		else
			_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, 39335 );	//this has an 30% chance to be spawned invisible if using blizz templates
	}
	void OnDied(Unit *mKiller)
	{
		int64 *PillarsRemaining = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_BINDING_CRYSTAL + 1 );
		*PillarsRemaining = *PillarsRemaining - 1;
		//no need to leave the dead body behind
		_unit->Despawn( 1, 0 );
    }
};

#define SPELL_ID_ICE_WAVE_VISIBLE	105265
#define SPELL_ID_ICE_WAVE_DAMAGE	105314

//move in a circle and kill players that get close
class IceWave : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(IceWave);

    IceWave(Creature* pCreature) : CreatureAIScript(pCreature) { }
	void OnLoad()
	{
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->GetAIInterface()->setMoveType( MOVEMENTTYPE_CIRCLEWP );
			float center_x = 13588.0f;
			float center_y = 13612.0f;
			float radius = 40.0f;
			//add waypoints so that the NPC will move in a circle
			WayPointMap WPM;
			WPM.reserve( 16 );
			float AnglePerStep = (2*M_PI) / 16;
			for( int32 i=15;i>=0;i--) //clockwise movement
			{
				float angle = i * AnglePerStep;
				float cur_X = center_x+(radius*(cosf(angle)));
				float cur_Y = center_y+(radius*(sinf(angle)));
				WayPoint *wp = new WayPoint;
				memset( wp, 0, sizeof( wp ) );
				wp->id = i;
				wp->x = cur_X;
				wp->y = cur_Y;
				wp->z = _unit->GetPositionZ();
				WPM.push_back( wp );
			}/**/
			_unit->GetAIInterface()->SetWaypointMap( &WPM );
		}
		_unit->bInvincible = true;
		_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, 11686 );	//invisible
		_unit->CastSpell( _unit, SPELL_ID_ICE_WAVE_VISIBLE, true );	//ice wave = will make the target have some visible anymation
		RegisterAIUpdateEvent( 1000 );
	}
    void AIUpdate()
    {
		_unit->GetAIInterface()->Update( 1 );

		SpellEntry *sp = dbcSpell.LookupEntryForced( SPELL_ID_ICE_WAVE_DAMAGE ); //Ice Wave
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++;
			if( !SafeUnitCast( *itr )->isAlive() ) 
				continue;
			float tdist = (*itr)->GetDistance2dSq( _unit );
			if( tdist < 20.0f * 20.0f  )
			{
//				_unit->SpellNonMeleeDamageLog( SafeUnitCast(*itr), sp, (*itr)->GetMaxHealth() * 30 / 100, true ); //Ice Wave
				//allow players to cast immunity spells
				_unit->CastSpell( (*itr), sp, true );
				_unit->CastSpell( (*itr), sp, true );
				_unit->CastSpell( (*itr), sp, true );
				_unit->CastSpell( (*itr), sp, true );
			}
		}
		_unit->ReleaseInrangeLock();
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
    }
};

//this will start a chain lightning between players until it reaches a pillar or worst case closest player
class BoundLightningElemental : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BoundLightningElemental);

    BoundLightningElemental(Creature* pCreature) : CreatureAIScript(pCreature) {}
	void OnLoad()
	{
		//make him slow or players will insta kite him to the conductor
		_unit->m_walkSpeed = 1.5f;
		_unit->m_runSpeed = _unit->m_walkSpeed;
		_unit->m_base_runSpeed = _unit->m_runSpeed;
		_unit->m_base_walkSpeed = _unit->m_walkSpeed;
		_unit->m_flySpeed = _unit->m_walkSpeed;
		_unit->m_backFlySpeed = _unit->m_walkSpeed;
		_unit->m_backWalkSpeed = _unit->m_walkSpeed;	// this should really be named m_backRunSpeed
		_unit->m_swimSpeed = _unit->m_walkSpeed;
		_unit->m_backSwimSpeed = _unit->m_walkSpeed;
	}
	void OnDied(Unit *mKiller)
	{
		Player *ClosestP = NULL;
		float distance = 100000;
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++;
			if( !SafeUnitCast( *itr )->isAlive() )
				continue;
			float tdist = (*itr)->GetDistance2dSq( _unit );
			if( tdist < distance )
			{
				distance = tdist;
				ClosestP = (*itr);
			}
			break;
		}
		_unit->ReleaseInrangeLock();
		if( ClosestP )
			ClosestP->CastSpell( ClosestP, SPELL_ID_LIGHTNING_CHAIN, true ); 
		RemoveAIUpdateEvent();
    }
};

class CrystalConductor : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CrystalConductor);

    CrystalConductor(Creature* pCreature) : CreatureAIScript(pCreature) { }
	void OnLoad()
	{
		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, 39575 );	//this has an 30% chance to be spawned invisible if using blizz templates
		}
		//resist all dmg unless we are script dmg-ing it
		for(uint32 i=0;i<SCHOOL_COUNT;i++)
		{
			_unit->DamageTakenPctMod[i] = 0; //this will null out dmg unless imba 
			_unit->SchoolImmunityAntiEnemy[i] = 66;
			_unit->SchoolImmunityAntiFriend[i] = 66;
		}
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		int64 *PillarsRemaining = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_LIGHTNING_CONDUIT + 1 );
		*PillarsRemaining = *PillarsRemaining - 1;
		//no need to leave the dead body behind
		_unit->Despawn( 1, 0 );
    }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char UltraxionYells[10][103] = {
    "Fall before Ultraxion!",
    "For this moment ALONE was I made. Look upon your death, mortals, and despair!",
    "Hahahahaha!",
    "I am the beginning of the end...the shadow which blots out the sun...the bell which tolls your doom...",
    "I WILL DRAG YOU WITH ME INTO FLAME AND DARKNESS!",
    "Now is the hour of twilight!",
    "The final shred of light fades, and with it, your pitiful mortal existence!",
    "Through the pain and fire my hatred burns!",
    "You have no hope!",
    "But...but...I am...Ul...trax...ionnnnnn...",
};

#define SPELL_ID_UNSTABLE_MONSTROSITY		106372
#define SPELL_ID_UNSTABLE_MONSTROSITY_DMG	106375
#define SPELL_COOL_UNSTABLE_MONSTROSITY		6000	//every 6 seconds at the start of the fight, then cooldown diminishes every second
#define SPELL_ID_TWILIGHT_SHIFT				106368	//target all players even out of phase and change their phase 
#define SPELL_ID_HEROIC_WILL				106108	//gives us 5 seconds out of twilight phase 
#define SPELL_ID_FADING_LIGHT				105925	//if the aura is not removed by script then it will kill the player
#define SPELL_ID_FADED_INTO_LIGHT			109461	//wipe agro
#define SPELL_COOL_FADING_LIGHT				50000	//should be larger then 1 minute 
#define SPELL_ID_HOUR_OF_TWILIGHT			103327	//needs to kill N players or raid is wiped
#define SPELL_COOL_HOUR_OF_TWILIGHT			2*60*1000
#define SPELL_ID_LOOMING_DARKNESS			106498
#define SPELL_ID_TWILIGHT_BURST				106415	//if not in spell range he will use this
#define SPELL_COOL_TWILIGHT_BURST			6000
#define SPELL_ID_TWILIGHT_ERRUPTION			106388	//wipe after 6 minutes
#define SPELL_COOL_TWILIGHT_ERRUPTION		7*60*1000
#define SPELL_ID_GIFT_OF_LIFE				105896
#define SPELL_ID_ESSENCE_OF_DREAMS			105900
#define SPELL_ID_SOURCE_OF_MAGIC			105903
#define SPELL_ID_TIMELOOP					105984
#define SPELL_ID_LAST_DEFENDER_OF_AZEROTH	106080

#define ULTRAXION_SIZE						25		//add this to spell ranges and other 

class Ultraxion : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Ultraxion);
/* 
    Unstable Monstrosity – Growing Twilight instability causes arcs of Twilight energy to erupt from Ultraxion's hide. The arcs inflict 300000 Shadow damage every 6 seconds, split evenly amongst enemy players in the Twilight Realm and within 30 yards of the target.Every minute that Ultraxion remains in combat reduces the time between arcs by 1 second, to a maximum rate of one arc per second. In addition, the instability prevents Ultraxion from parrying attacks.
    Twilight Shift – At the beginning of the battle, Ultraxion pulls all enemies with him into the Twilight Realm where they can bear the full brunt of his powerful Twilight assaults. The Twilight Realm grants players the Heroic Will ability.
        Heroic Will – The player's Heroic Will steels the caster's mind to resist the forces of Twilight. This pulls the caster out of the Twilight realm for up to 5 sec, but renders them unable to attack, cast spells, or move while concentrating.
    Fading Light – Ultraxion sucks the light out of his current target and one random player, drawing them into Twilight over 5-10 seconds. Upon expiration, players in the Twilight Realm instantly die. Players in the normal realm forcibly enter the Twilight Realm, and are afflicted with Faded into Twilight.
        In 25 player Difficulty, Ultraxion sucks the light out of three random players.
        In 10 player Heroic Difficulty, Ultraxion sucks the light out of two random players.
        In 25 player Heroic Difficulty, Ultraxion sucks the light out of six random players.
        In Looking for Raid Difficulty, Ultraxion only sucks the light out of his current target.
        Faded into Twilight – Residual Twilight effects cause all threat against Ultraxion to be reset. In addition, no threat will be generated for 5 sec.
            In Heroic Difficulty, players take 100% more Physical damage for 5 sec, instead of generating no threat.
    Hour of Twilight – The Hour of Twilight falls upon Ultraxion's foes. Enemies caught within the Twilight Realm take 300000 unresistable Shadow damage. Players within the normal realm forcibly enter the Twilight Realm. If Hour of Twilight does not hit a player, the Aspects take the full brunt of the attack. This disrupts the shield protecting the Aspects and heralds the end of all life on Azeroth.
        In 25 player Normal Difficulty, if Hour of Twilight hits less than three players, the Aspects take the full brunt of the attack.
        In 10 player Heroic Difficulty, if Hour of Twilight hits less than two players, the Aspects take the full brunt of the attack.
        In 25 player Heroic Difficulty, if Hour of Twilight hits less than five players, the Aspects take the full brunt of the attack.
        In Looking for Raid Difficulty, the Aspects can never take the full brunt of the attack.
        Looming Darkness – When struck by Hour of Twilight, powerful waves of Twilight energy course through the victim, rendering them more susceptible to the harmful effects of Hour of Twilight. Players instantly die if Hour of Twilight inflicts additional damage to them.
    Hour of Twilight – The Hour of Twilight falls upon Ultraxion's foes. Enemies caught within the Twilight Realm take 300000 unresistable Shadow damage. Players within the normal realm forcibly enter the Twilight Realm.
    Twilight Burst – If every player stands outside the range of his powerful claws, Ultraxion expels a powerful burst of Twilight energy. This burst inflicts 73125 to 76875 unresistable Shadow damage on all players and increases their magical damage taken by 50% for 6 sec. This effect stacks.
    Twilight Eruption – After 6 minutes have passed, Ultraxion's instability reaches uncontainable levels and erupts outwards as a massive explosion that destroys all nearby foes and Ultraxion himself.If the shields surrounding the Aspects should fall at any point during the encounter, Ultraxion will immediately trigger Twilight Eruption.
    
    Protection of the Aspects
    The Aspects provide limited aid to the players as they fight Ultraxion.
        Alexstrasza 
            Gift of Life – Alexstrasza creates a crystal of pure life energy. Activating the crystal imbues one healer with the essence of life itself, increasing healing done by 100% for the duration of the battle. The crystal has one charge. Only one buff granted by Alexstrasza, Ysera, or Kalecgos can be present on a player at any time.
            In 25 player raids, Alexstrasza creates two crystals.
        Ysera 
            Essence of Dreams – Ysera creates a crystal of dream energy. Activating the crystal imbues one healer with the power of dreams, which causes all healing done to be duplicated and distributed evenly amongst all friendly targets within 50 yards for the remainder of the battle. The crystal has one charge. Only one buff granted by Alexstrasza, Ysera, or Kalecgos can be present on a player at any time.
            In 25 player raids, Ysera creates two crystals.
        Kalecgos 
            Source of Magic – Kalecgos creates a crystal of amplified arcane energy. Activating the crystal imbues one healer with the essence of magic, reducing the mana cost of all spells by 75% and increasing spell haste by 100% for the remainder of the battle. The crystal has one charge. Only one buff granted by Alexstrasza, Ysera, or Kalecgos can be present on a player at any time.
            In 25 player raids, Kalecgos creates two crystals.
        Nozdormu 
            Timeloop – Nozdormu captures all players within a finite timeloop. Any attack which would otherwise kill the player brings them to 100% of maximum health and removes the Timeloop.
        Thrall 
            Last Defender of Azeroth – Thrall, now the Earth-Warder of Azeroth, blesses all tanks with the immovable strength of the ancient stones of the world. The blessing reduces defensive cooldowns by 50% and increases defensive ability durations by 100%.
*/
	uint32	YellIndex;
	uint32	TalkCooldown;
	uint32	phase;
	float	DifficultyScale;
	uint32	TwilightShiftTimer;
	uint32	LightningArcTimer;
	uint32	LightningArcStartedAttackingAtStamp;
	uint32	FadingLightTimer;
	uint32	HourOfTwilightTimer;
	uint32	TwilightBurstTimer;
	uint32	TwilightErruptionTimer;
	uint64	HourOfTwilightMarked[8];
    
	Ultraxion(Creature* pCreature) : CreatureAIScript(pCreature)
	{
        phase = 0;
	}
    
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		//make him attack faster as difficulty increases
		uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now * DifficultyScale ) );	//attack a bit faster cause there are more healers

		if( _unit->GetAIInterface() )
		{
			_unit->GetAIInterface()->m_canMove = false;
//			_unit->GetAIInterface()->disable_melee = true;
		}

		RegisterAIUpdateEvent( 1000 );
	}

	void OnDied(Unit *mKiller)
	{
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		*EncounterStatus |= EXTENSION_FLAG_ULTRAXION_DIED;
		_unit->GetMapMgr()->CreateCreature( MOB_ENTRY_DRAGONSOUL_TELEPORTER, true, _unit->GetPositionX() - 30, _unit->GetPositionY(), _unit->GetPositionZ() );
		RemoveAIUpdateEvent();

		InRangePlayerSetRecProt::iterator itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
		{
			Player *Plr = (*itr2);
			Plr->RemoveAura( SPELL_ID_GIFT_OF_LIFE );				//Gift of Life
			Plr->RemoveAura( SPELL_ID_ESSENCE_OF_DREAMS );			//Essence of Dreams
			Plr->RemoveAura( SPELL_ID_SOURCE_OF_MAGIC );			//Source of Magic
			Plr->RemoveAura( SPELL_ID_TIMELOOP );					//Timeloop
			Plr->RemoveAura( SPELL_ID_LAST_DEFENDER_OF_AZEROTH );	//Last Defender of Azeroth
			Plr->RemoveAura( SPELL_ID_TWILIGHT_SHIFT );	
			Plr->RemoveAura( SPELL_ID_LOOMING_DARKNESS );	
			Plr->RemoveAura( SPELL_ID_FADING_LIGHT );				
		}
		_unit->ReleaseInrangeLock();
		//try to see players lie about BOSS difficulty
		LOG_CREATURE_DEATH( _unit->GetEntry(), mKiller );
		if( mKiller && mKiller->IsPlayer() && SafePlayerCast( mKiller )->GetGuild() )
			SafePlayerCast( mKiller )->GetGuild()->GuildNewsAdd( GUILD_NEWS_LOG_DUNGEON_ENCOUNTER, mKiller->GetGUID(), 1297 );
    }

	void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			phase = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		// Init phase
		if( phase == 0 )
		{
			TalkCooldown = TickNow + RandomUInt() % 25000;
			LightningArcTimer = TickNow + RandomUInt() % 25000;
			LightningArcStartedAttackingAtStamp = TickNow;
			TwilightErruptionTimer = TickNow + SPELL_COOL_TWILIGHT_ERRUPTION * DifficultyScale;
			FadingLightTimer = TickNow + RandomUInt() % 25000;
			HourOfTwilightTimer = TickNow + RandomUInt() % 25000;
			TwilightBurstTimer = TickNow + RandomUInt() % 25000;
			TwilightShiftTimer = TickNow;

			YellIndex = 0;
            phase = 1;
		}

        if( TalkCooldown < TickNow )
        {
            _unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, UltraxionYells[ YellIndex ] );
            YellIndex = ( YellIndex + 1 ) % 10;
            TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
        }

		//from time to time check if our raid group has twilight shift on them
		if( TwilightShiftTimer < TickNow )
		{
			InRangePlayerSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
			{
				itr = itr2;
				itr2++;
				if( !SafeUnitCast( *itr )->isAlive() || (*itr)->HasAura( SPELL_ID_HEROIC_WILL ) || (*itr)->HasAura( SPELL_ID_TWILIGHT_SHIFT ) ) 
					continue;
				(*itr)->CastSpell( (*itr), SPELL_ID_TWILIGHT_SHIFT, true );
			}
			_unit->ReleaseInrangeLock();
			TwilightShiftTimer = TickNow + 5000;
		}

		//some lightning that will split dmg between guys that do not have heroic will
		if( LightningArcTimer < TickNow && _unit->isCasting() == false )
		{
//			InRangePlayerSet TargetList;
			uint32 SplitTo = 0;
			InRangePlayerSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
			{
				itr = itr2;
				itr2++;
				if( !SafeUnitCast( *itr )->isAlive() ) 
					continue;
//				float tdist = Distance2DSq( (*itr)->GetPositionX(), (*itr)->GetPositionY(), _unit->GetPositionX(), _unit->GetPositionY() );
				float tdist = _unit->GetDistanceSq( (*itr) );
				if( tdist > (30+ULTRAXION_SIZE)*(30+ULTRAXION_SIZE) )
					continue;
				SplitTo++;
//				TargetList.insert( (*itr) );
			}
			_unit->ReleaseInrangeLock();

			if( SplitTo > 0 )
			{
				_unit->CastSpell( tank, SPELL_ID_UNSTABLE_MONSTROSITY_DMG, true );
				uint32 MSPassed = TickNow - LightningArcStartedAttackingAtStamp;
				uint32 MinutesPassedMS = MSPassed / 60;
				uint32 spell_cool;
				if( MinutesPassedMS >= SPELL_COOL_UNSTABLE_MONSTROSITY )
					spell_cool = 500;
				else
					spell_cool = SPELL_COOL_UNSTABLE_MONSTROSITY - MinutesPassedMS;
				LightningArcTimer = TickNow + (spell_cool + RandomUInt() % spell_cool)*DifficultyScale;
			}
			//cast twilight erruption, no matter of the distance
			else 
			{
				_unit->CastSpell( tank, SPELL_ID_TWILIGHT_BURST, true );
			}
        }

		//fading light : kill target if he does not "heroic will" in time ( at expiration )
		if( FadingLightTimer < TickNow )
		{
			uint32 RequiredTargetCount = 2;
			if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
				RequiredTargetCount = 3;
			else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
				RequiredTargetCount= 6;

			InRangePlayerSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); 
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
			{
				itr = itr2;
				itr2++;
				if( !SafeUnitCast( *itr )->isAlive() || ( *itr )->HasAura( SPELL_ID_FADING_LIGHT ) ) 
					continue;
				(*itr)->CastSpell( (*itr), SPELL_ID_FADING_LIGHT, true );
				RequiredTargetCount--;
				if( RequiredTargetCount == 0 )
					break;
			}
			_unit->ReleaseInrangeLock();

			FadingLightTimer = TickNow + (SPELL_COOL_FADING_LIGHT + RandomUInt() % SPELL_COOL_FADING_LIGHT)*DifficultyScale;
		}

		//from time to time check if our raid group has twilight shift on them
		if( HourOfTwilightTimer < TickNow + 4000 )
		{
			if( HourOfTwilightTimer > TickNow )
			{
				char msg[500];
				sprintf( msg, "Hour of Twilight is approaching in %ds",(999+HourOfTwilightTimer-TickNow)/1000);
				_unit->SendChatMessage( CHAT_MSG_MONSTER_WHISPER, LANG_UNIVERSAL, msg );
			}
			if( HourOfTwilightTimer < TickNow )
			{
				uint32 PeopleKilled = 0;
				InRangePlayerSetRecProt::iterator itr,itr2;
				_unit->AquireInrangeLock(); //make sure to release lock before exit function !
				InrangeLoopExitAutoCallback AutoLock;
				for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
				{
					itr = itr2;
					itr2++;
					if( !SafeUnitCast( *itr )->isAlive() || (*itr)->HasAura( SPELL_ID_HEROIC_WILL ) ) 
						continue;

					uint32 InitialHealth = (*itr)->GetHealth();
					uint32 LoopCounter = 0;
					int32 NeedToDealDamage = 300000;
					int32 CanDealDamageDueToLimitations = 100000;
					do{
						(*itr)->DealDamage( (*itr), CanDealDamageDueToLimitations, SPELL_ID_HOUR_OF_TWILIGHT, true );
						LoopCounter++;
						NeedToDealDamage = 300000 - ( InitialHealth - (*itr)->GetHealth() );
					}while( NeedToDealDamage > 0 && (*itr)->isAlive() && LoopCounter < 10 );

					if( (*itr)->HasAura( SPELL_ID_LOOMING_DARKNESS ) )
					{
						if( ( *itr )->isAlive() )
							(*itr)->DealDamage( (*itr), MIN( CanDealDamageDueToLimitations, (*itr)->GetMaxHealth() ), SPELL_ID_HOUR_OF_TWILIGHT, true );
						if( ( *itr )->isAlive() )
							(*itr)->DealDamage( (*itr), MIN( CanDealDamageDueToLimitations, (*itr)->GetMaxHealth() ), SPELL_ID_HOUR_OF_TWILIGHT, true );
						if( ( *itr )->isAlive() )
							(*itr)->DealDamage( (*itr), MIN( CanDealDamageDueToLimitations, (*itr)->GetMaxHealth() ), SPELL_ID_HOUR_OF_TWILIGHT, true );
					}
					if( ( *itr )->isAlive() )
						(*itr)->CastSpell( (*itr), SPELL_ID_LOOMING_DARKNESS, true );
					PeopleKilled++;
				}
				_unit->ReleaseInrangeLock();

				uint32 RequiredTargetCount = 1;
				if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
					RequiredTargetCount = 3;
				else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
					RequiredTargetCount= 2;
				else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
					RequiredTargetCount= 5;

				//mark the end of the fight
				if( PeopleKilled < RequiredTargetCount )
				{
		            _unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All Azeroth is lost! Need more sacrifice !" );
					TwilightErruptionTimer = 0;
				}
				HourOfTwilightTimer = TickNow + (SPELL_COOL_HOUR_OF_TWILIGHT + RandomUInt() % SPELL_COOL_HOUR_OF_TWILIGHT)*DifficultyScale;
			}
		}

		//kill the whole raid. Cannot be resisted
		if( TwilightErruptionTimer < TickNow )
		{
			_unit->CastSpell( tank, SPELL_ID_TWILIGHT_ERRUPTION, true  );
			InRangePlayerSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
			{
				itr = itr2;
				itr2++;
				if( !SafeUnitCast( *itr )->isAlive() ) 
					continue;
				_unit->DealDamage( (*itr), (*itr)->GetMaxHealth(), SPELL_ID_TWILIGHT_ERRUPTION );
				if( ( *itr )->isAlive() )
					_unit->DealDamage( (*itr), (*itr)->GetMaxHealth(), SPELL_ID_TWILIGHT_ERRUPTION );
				if( ( *itr )->isAlive() )
					_unit->DealDamage( (*itr), (*itr)->GetMaxHealth(), SPELL_ID_TWILIGHT_ERRUPTION );
			}
			_unit->ReleaseInrangeLock();
			//if we can repeat this then not everyone died yet. Else timer will get reseted
			TwilightErruptionTimer = TickNow + 1000;
		}
	}
};

class SCRIPT_DECL UltraxionHelperGossip : public GossipScript
{
public:
	uint32	OfferedSpell;
	void Destroy()
	{
		delete this;
	}
	void GossipHello(Object*  pObject, Player* Plr, bool AutoSend)
	{
		//we only work near a specific location
		if( 60 * 60 < Distance2DSq( -1786, -2393, pObject->GetPositionX(), pObject->GetPositionY() ) )
			return;
		//we only buff for the ultraxion encounter
		int64 *EncounterStatus = pObject->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		if( *EncounterStatus & EXTENSION_FLAG_ULTRAXION_DIED )
			return;
		//time to init ?
		int64 *Inited = pObject->GetCreateIn64Extension( SPELL_HASH_UNSTABLE_MONSTROSITY );
		int64 *RemainingCasts = pObject->GetCreateIn64Extension( SPELL_HASH_UNSTABLE_MONSTROSITY );
		if( *Inited == 0 )
		{
			*Inited = 1;
			*RemainingCasts = 2;
			if( pObject->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
				(*RemainingCasts)++;
			else if( pObject->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
				(*RemainingCasts)++;
		}
		//can we still cast aura on players ?
		if( *RemainingCasts <= 0 || OfferedSpell <= 0 )
			return;
		//double click protection for morrons
		if( Plr->HasAura( OfferedSpell ) )
			return;
		//1 less charge
		*RemainingCasts--;
		//these cannot be stacked
		Plr->RemoveAura( SPELL_ID_GIFT_OF_LIFE );				//Gift of Life
		Plr->RemoveAura( SPELL_ID_ESSENCE_OF_DREAMS );			//Essence of Dreams
		Plr->RemoveAura( SPELL_ID_SOURCE_OF_MAGIC );			//Source of Magic
//		Plr->RemoveAura( SPELL_ID_TIMELOOP );					//Timeloop
//		Plr->RemoveAura( SPELL_ID_LAST_DEFENDER_OF_AZEROTH );	//Last Defender of Azeroth
		//add the new buff
		Plr->CastSpell( Plr, OfferedSpell, true );
	};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char BlackhornYells[10][81] = {
    "Down you go!",
    "Get up! Oh... weakling!",
    "Goriona! Give them hell!",
    "Hah! I was hoping you'd make it this far. You'd best be ready for a real fight.",
    "How's THIS?",
    "Looks like I'm doing this myself. Good!",
    "Mess with the bull....",
    "We're flying a little too close. It's been a good fight, but I'm ending it, now.",
    "Well... done, heh. But I wonder if you're good enough... to best him.",
    "You won't get near the Master. Dragonriders, attack!",
};

#define NPC_ID_ASSAULT_DRAKE			56587
#define NPC_ID_GORINA					56781
#define SPELL_COOL_SPAWN_ADD_PHASE1		30000
#define SPELL_ID_DECK_FIRE				109445
#define SPELL_ID_DEVASTATE				57795
#define SPELL_COOL_DEVASTATE			2000
#define SPELL_ID_DISRUPTING_ROAR		108044
#define SPELL_COOL_DISRUPTING_ROAR		10000
#define SPELL_ID_SHOCKWAVE				46968
#define SPELL_COOL_SHOCKWAVE			10000
#define SPELL_ID_VENGEANCE				108045	//we will handle this manually
#define SPELL_COOL_VENGEANCE			5000
#define SPELL_ID_SIPHON_HP				110312
#define SPELL_COOL_SIPHON_HP			60000

#define BLACKHORN_MAX_ADS_ACTIVE		20	//trick to crash the server

#define SHIP_CENTER_X				13434.0f
#define SHIP_CENTER_Y				-12132.0f
#define SHIP_CENTER_Z				160.0f

class Blackhorn : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(Blackhorn);
/* 
    Stage One: Dragonriders, Attack!
    Flying alongside the Skyfire and surveying the scene while his massive twilight drake lobs balls of destruction at the gunship, Warmaster Blackhorn gives the order for his elite dragonriders to swoop into battle.
        The Skyfire
        Sky Captain Swayze helms this massive gunship equipped with numerous cannons and a pair of harpoon guns for its defense. The gunship is sturdy but not indestructible, and if it sustains too much damage the pursuit of Deathwing will come to an untimely end.
            Harpoon Guns
                Alliance engineers equipped the Skyfire with two repurposed harpoon guns, seized during its prior service in Northrend. Harpoon gunners spear the enemy's Assault Drakes and reel them in to bring them within reach of ranged attackers on the deck. The drakes strain against the line, eventually breaking free and returning to a safe distance. After a pause to reload, the harpoon gunners then spear their target anew.
                In Looking for Raid Difficulty, the drakes do not break free from the Harpoons.
            HeroicDeck Fire – As the Skyfire takes damage, destructive fires break out on the ship and spread rapidly across its deck. Crewmembers rush to extinguish these blazes and keep them in check. Any player standing within a fire suffers 42750 to 47250 Fire damage per second.
        Goriona – Warmaster Blackhorn takes to the skies with this massive twilight drake.
            Twilight Onslaught – Goriona unleashes a massive blast of dark energy at a random location on the deck of the Skyfire. Twilight Onslaught inflicts 800000 Shadow damage, divided evenly among all players within its 10 yard radius and the Skyfire. If the attack strikes the deck of the Skyfire without hitting a player, the gunship suffers the full damage.
           Heroic Broadside – Goriona periodically unleashes a vicious barrage of fireballs that slam into the side of the Skyfire, causing the ship to suffer damage equal to 20% of its remaining structural integrity.
        Twilight Assault Drake – These drakes strafe across the deck of the ship, deposit their fierce riders onto the deck, and then retreat to a safe distance to bombard the gunship.
            Twilight Barrage – The twilight drakes launch bursts of dark energy at a random location on the deck of the Skyfire. Twilight Barrage inflicts 140000 Shadow damage, divided evenly among all players within its 5 yard radius. If the attack strikes the deck of the Skyfire without hitting a player, the gunship suffers the full damage instead.
            On Heroic Difficulty, Twilight Barrage also increases Shadow damage taken by 140000% for until cancelled. This effect stacks.
        Twilight Elite Dreadblade 
            Blade Rush – The Dreadblade charges at the location of a random distant player, inflicting 92500 to 107500 Physical damage to all players in its path.
            Degeneration – The Dreadblade carves a swath of destruction with its dark sword. The sword inflicts 38000 to 42000 Shadow damage to enemies in a frontal cone, and 3000 Shadow damage every 2 sec for 1 min. This effect stacks.
        Twilight Elite Slayer 
            Blade Rush – The Elite Slayer charges at the location of a random distant player, inflicting 92500 to 107500 Physical damage to all players in its path.
            Brutal Strike – The Slayer strikes a vicious blow with its jagged sword. The strike inflicts 150 Physical damage, and 3000 Physical damage every 2 sec for 1 min. This effect stacks.
        Twilight Sapper – Sleek infiltrator drakes airdrop goblins strapped with powerful explosives onto the deck of the ship. These Sappers rush to breach the gunship bridge and detonate their destructive payload.
            Detonate – Upon reaching the Skyfire's bridge, the Sapper detonates his explosives. The blast inflicts 243750 to 256250 Fire damage to players within 8 yards, damages the Skyfire for 20% of its total durability, and kills the Sapper.

    Stage Two: Looks Like I'm Doing This Myself
    Once the Skyfire's defenders defeat three waves of dragonriders, Warmaster Blackhorn leaps down onto the deck of the gunship.
        Warmaster Blackhorn 
            Devastate – Blackhorn sunders his current target's armor, inflicting 120% weapon damage and reducing the player's armor by 20% for 30 sec. This effect stacks.
            Disrupting Roar – Warmaster Blackhorn screams fiercely, inflicting 47500 to 52500 Physical damage to all enemies on the gunship. The shout also interrupts spellcasting for players within 10 yards for 8 sec.
            Shockwave – Blackhorn strikes the ground, unleashing a wave of force that inflicts 95000 to 105000 Physical damage to enemies in an 80 yard frontal cone and stuns them for 4 sec.
            Vengeance – As Warmaster Blackhorn takes damage during the course of the fight, he inflicts 1% more damage for each percentage of his missing health.
            HeroicSiphon Vitality – When severely injured, Warmaster Blackhorn drains health directly from Goriona. The health drain inflicts damage equal to 20% of Goriona's remaining health, and heals Blackhorn accordingly.
        Goriona – After depositing her master on the deck of the Skyfire, Goriona hovers alongside the Skyfire and rains destruction down on the players. Once Goriona has suffered sufficient damage, she will break off and flee, abandoning her master on the Skyfire.
        On Heroic mode, upon reaching 90% health, Goriona will land on the Skyfire's deck to assist her master.
            Twilight Flames – Goriona launches a blast of dark energy at the location of a random player, inflicting 38000 to 42000 Shadow damage on impact to players in a 8 yard radius. Twilight Flames lingers on the deck of the Skyfire, inflicting 29250 to 30750 Shadow damage to players within 7 yards every 1 sec for 30 sec.
            HeroicTwilight Breath – Goriona unleashes a torrent of dark flame, inflicting 81600 to 88400 Shadow damage in a 45 yard cone in front of the caster.
            HeroicConsuming Shroud – Goriona envelops an enemy target in twilight energy, absorbing the next 100000 healing received. When Consuming Shroud absorbs incoming healing, unleashed energy will deal damage equal to the healing absorbed to all allies. Dying while afflicted by Consuming Shroud will release all remaining energy at once.

*/
	uint32	YellIndex;
	uint32	TalkCooldown;
	uint32	phase;
	float	DifficultyScale;
	uint32	SpawnAddTimer;
	uint32	SpawnAddType;
	uint32	DrakesRemainingMyCheck;
	uint32	DevastateTimer;
	uint32	DisruptingRoarTimer;
	uint32	ShockwaveTimer;
	uint32	SiphonHPTimer;
	int32	VengeanceEffect;

	Blackhorn(Creature* pCreature) : CreatureAIScript(pCreature)
	{
        phase = 0;
	}
    
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		//make him attack faster as difficulty increases
		uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now * DifficultyScale ) );	//attack a bit faster cause there are more healers

		VengeanceEffect = 0;

		//strange bug when he keeps respawning 
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		if( ( *EncounterStatus ) & EXTENSION_FLAG_BLACKHORN_DIED )
			return;

		RegisterAIUpdateEvent( 1000 );
	}

	void OnDied(Unit *mKiller)
	{
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		if( ( *EncounterStatus ) & EXTENSION_FLAG_BLACKHORN_DIED )
			return;
		*EncounterStatus |= EXTENSION_FLAG_BLACKHORN_DIED;
		_unit->GetMapMgr()->CreateCreature( MOB_ENTRY_DRAGONSOUL_TELEPORTER, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
		RemoveAIUpdateEvent();
		//try to see players lie about BOSS difficulty
		LOG_CREATURE_DEATH( _unit->GetEntry(), mKiller );
		if( mKiller && mKiller->IsPlayer() && SafePlayerCast( mKiller )->GetGuild() )
			SafePlayerCast( mKiller )->GetGuild()->GuildNewsAdd( GUILD_NEWS_LOG_DUNGEON_ENCOUNTER, mKiller->GetGUID(), 1298 );
    }

	void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			_unit->bInvincible = false;
			_unit->GetAIInterface()->m_canMove = true;
			_unit->GetAIInterface()->disable_melee = false;
			_unit->GetAIInterface()->disable_combat = false;
			_unit->GetAIInterface()->disable_targeting = false;
			_unit->SetScale( 1.0f );
			phase = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		// Init phase
		if( phase == 0 )
		{
			TalkCooldown = TickNow + RandomUInt() % 25000;
			SpawnAddTimer = TickNow + RandomUInt() % 25000;
			YellIndex = 0;
            phase = 1;
			_unit->bInvincible = true;
			_unit->GetAIInterface()->m_canMove = false;
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->SetScale( 0.2f );
			SpawnAddType = 1;
		}

        if( TalkCooldown < TickNow )
        {
            _unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, BlackhornYells[ YellIndex ] );
            YellIndex = ( YellIndex + 1 ) % 10;
            TalkCooldown = TickNow + (10000 + RandomUInt() % 10000);
        }

		//spawn dragons
        if( phase == 1 )
        {
			int64 *DrakesRemaining = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_ASSAULT_DRAKE );
			uint32 DrakesNeeded = 10;
			if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
				DrakesNeeded = 12;
			else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
				DrakesNeeded = 15;
			else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
				DrakesNeeded = 17;
			float AnglePerStep = (2*M_PI) / DrakesNeeded;
			float center_x = SHIP_CENTER_X;
			float center_y = SHIP_CENTER_Y;
			float center_z = SHIP_CENTER_Z;
			float radius_x = 40.0f;
			float radius_y = 60.0f;
			float radius_z = 5.0f;
			for( uint32 i=*DrakesRemaining;i<DrakesNeeded;i++)
			{
				float angle = i * AnglePerStep;
				float cur_radius_x = radius_x / 2 + RandomFloat( radius_x );
				float cur_radius_y = radius_y / 2 + RandomFloat( radius_y );
				float cur_X = center_x + (cur_radius_x*(cosf(angle)))+RandomFloat( 10.0f );
				float cur_Y = center_y + (cur_radius_y*(sinf(angle)))+RandomFloat( 10.0f );
				float DistanceToCenter = Distance2DSq( cur_X, cur_Y, center_x, center_y );
				float DistancePCT = DistanceToCenter/((radius_x+radius_y)/2);
				float cur_Z = center_z + radius_z*DistancePCT/100.0f + RandomFloat( radius_z );
				_unit->GetMapMgr()->CreateCreature( NPC_ID_ASSAULT_DRAKE, true, cur_X, cur_Y, cur_Z );
			}
			DrakesRemainingMyCheck = *DrakesRemaining = DrakesNeeded;
			phase = 2;
			int64 *CombatPhase = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_GORINA );
			*CombatPhase = 2;
		}
		//we are on a dragon and trying to reduce the health of the ship by spawning random dudes
		if( phase == 2 )
		{
			//time to move to the new phase ?
			int64 *DrakesRemaining = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_ASSAULT_DRAKE );
//			*DrakesRemaining = *DrakesRemaining - 1;
			if( *DrakesRemaining <= 0 )
			{
				phase = 3;
				int64 *CombatPhase = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_GORINA );
				*CombatPhase = 3;
				_unit->bInvincible = false;
				_unit->GetAIInterface()->m_canMove = true;
				_unit->GetAIInterface()->disable_melee = false;
				_unit->GetAIInterface()->disable_combat = false;
				_unit->SetScale( 1.0f );
				DevastateTimer = TickNow + RandomUInt() % 25000;
				DisruptingRoarTimer = TickNow + RandomUInt() % 25000;
				ShockwaveTimer = TickNow + RandomUInt() % 25000;
				SiphonHPTimer = TickNow + RandomUInt() % 25000;
				_unit->CastSpell( _unit, SPELL_ID_VENGEANCE, true );
			}
			int64 *AddsActive = _unit->GetMapMgr()->GetCreateIn64Extension( SPELL_ID_DECK_FIRE );
			if( ( SpawnAddTimer < TickNow || DrakesRemainingMyCheck != *DrakesRemaining ) && *AddsActive < BLACKHORN_MAX_ADS_ACTIVE )
			{
				*AddsActive = *AddsActive + 1;
				DrakesRemainingMyCheck = *DrakesRemaining;
				uint32	DropOffEntries[3]={56854,56848,56923};
				uint32  EnemyWeapons[3]={61370,45926,0};
				float	SpawnX[6] = {  13456.0f, 13433.0f, 13418.0f };
				float	SpawnY[6] = { -12133.0f,-12132.0f,-12131.0f };
				SpawnAddType = (SpawnAddType+1)%3;
				uint32 PickedEntry = DropOffEntries[ SpawnAddType ];
				uint32 PickedSpawnLoc = RandomUInt(2);
				Creature *c = _unit->GetMapMgr()->CreateCreature( PickedEntry, true, SpawnX[PickedSpawnLoc], SpawnY[PickedSpawnLoc], 152.0f );
				if( c )
					c->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_ID, EnemyWeapons[ SpawnAddType ] );
				SpawnAddTimer = TickNow + (SPELL_COOL_SPAWN_ADD_PHASE1 + RandomUInt() % SPELL_COOL_SPAWN_ADD_PHASE1)*DifficultyScale;
				//chance to create deck fire
//				if( Rand( 30 ) )
				{
					AI_Spell sp;
					sp.spelltargetType = TTYPE_AT_RANGE_PLAYER_RANDOM;
					sp.minrange = 0.0f;
					sp.maxrange = 60.0f;
					sp.procChance = 100;
					Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
					if( t )
					{
						SpellEntry *sp = dbcSpell.LookupEntryForced( SPELL_ID_DECK_FIRE );
						Spell *newSpell = SpellPool.PooledNew( __FILE__, __LINE__ );
						newSpell->Init( _unit, sp, true, 0 );
						SpellCastTargets targets(0);
						targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
						targets.m_destX = t->GetPositionX();
						targets.m_destY = t->GetPositionY();
						targets.m_destZ = t->GetPositionZ();
						newSpell->prepare(&targets);
					}
				}
			}
        }
		//land on ship and start fight
		if( phase == 3 )
		{
			int32 DMGScale = 101 - (int32)_unit->GetHealthPct();
			if( VengeanceEffect != DMGScale && DMGScale > 0 )
			{
				float rem = (float)VengeanceEffect/100.0f;
				float add = (float)DMGScale/100.0f;
				for(uint32 x=0;x<7;x++)
				{
					_unit->ModDamageDonePct[x] -= rem; 
					_unit->ModDamageDonePct[x] += add; 
				}
				VengeanceEffect = DMGScale;
			}

			if( DevastateTimer < TickNow && _unit->isCasting() == false )
			{
				_unit->CastSpell( tank, SPELL_ID_DEVASTATE, false );
				DevastateTimer = TickNow + (SPELL_COOL_DEVASTATE + RandomUInt() % SPELL_COOL_DEVASTATE)*DifficultyScale;
			}
			if( DisruptingRoarTimer < TickNow && _unit->isCasting() == false )
			{
				_unit->CastSpell( tank, SPELL_ID_DISRUPTING_ROAR, false );
				DisruptingRoarTimer = TickNow + (SPELL_COOL_DISRUPTING_ROAR + RandomUInt() % SPELL_COOL_DISRUPTING_ROAR)*DifficultyScale;
			}
			if( ShockwaveTimer < TickNow && _unit->isCasting() == false )
			{
				_unit->CastSpell( tank, SPELL_ID_SHOCKWAVE, false );
				ShockwaveTimer = TickNow + (SPELL_COOL_SHOCKWAVE + RandomUInt() % SPELL_COOL_SHOCKWAVE)*DifficultyScale;
			}
			if( SiphonHPTimer < TickNow && _unit->GetHealthPct() < 80 && _unit->isCasting() == false )
			{
				Creature *c = _unit->GetMapMgr()->GetCreatureByEntry( NPC_ID_GORINA );
				if( c )
				{
					SpellEntry *sp = dbcSpell.LookupEntryForced( SPELL_ID_SIPHON_HP );
					Spell *newSpell = SpellPool.PooledNew( __FILE__, __LINE__ );
					newSpell->Init( _unit, sp, true, 0 );
					newSpell->forced_basepoints[0] = 20 * _unit->GetMaxHealth() / 100;
					SpellCastTargets targets( c->GetGUID() );
					newSpell->prepare(&targets);
//					_unit->CastSpell( c, SPELL_ID_SIPHON_HP, false );
				}
				SiphonHPTimer = TickNow + (SPELL_COOL_SIPHON_HP + RandomUInt() % SPELL_COOL_SIPHON_HP)*DifficultyScale;
			}
		}
	}
};

#define SPELL_ID_TWILIGHT_ONSLAUGHT			106401
#define SPELL_COOL_TWILIGHT_ONSLAUGHT		20000
#define SPELL_ID_TWILIGHT_FLAMES			105579
#define SPELL_COOL_TWILIGHT_FLAMES			20000
#define SPELL_ID_TWILIGHT_BREATH			110212
#define SPELL_COOL_TWILIGHT_BREATH			20000
#define SPELL_ID_CONSUMING_SHROUD			110214
#define SPELL_COOL_CONSUMING_SHROUD			20000

class Goriona : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(Goriona);

	uint8 EnableLandCombat;
	float DifficultyScale;
	uint32 TwilightOnSlaughtTimer;
	uint32 TwilightFlamesTimer;
	uint32 TwilightBreathTimer;
	uint32 ConsumingShroudTimer;
    Goriona(Creature* pCreature) : CreatureAIScript(pCreature)  {   }
	void OnLoad()
	{
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_melee = true;
//		_unit->GetAIInterface()->disable_combat = true;
//		_unit->GetAIInterface()->disable_targeting = true;
		EnableLandCombat = 0;
		if( _unit->GetMapMgr()->instance_difficulty & ( INSTANCE_FLAG_RAID_10_MEN_HEROIC | INSTANCE_FLAG_RAID_25_MEN_HEROIC ) )
			EnableLandCombat = 1;
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		_unit->bInvincible = true;

		RegisterAIUpdateEvent( 2000 );

		TwilightOnSlaughtTimer = GetTickCount() + 25000 + RandomUInt() % 25000;
		TwilightFlamesTimer = 0;
		TwilightBreathTimer = 0;
		ConsumingShroudTimer = 0;
	}
    void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 10.0f;
			sp.maxrange = 80.0f;
			sp.procChance = 20;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			if( t )
				_unit->GetAIInterface()->AttackReaction( t, 10, 0 );
			return;
		}
		if( tank == NULL || _unit->isAlive() == FALSE || _unit->isCasting() )
		{
			return;
		}
		uint32 TickNow = GetTickCount();

		if( EnableLandCombat <= 1 )
		{
			int64 *CombatPhase = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_GORINA );
			if( *CombatPhase > 2 )
			{
				EnableLandCombat = 2;
				_unit->bInvincible = false;
				_unit->GetAIInterface()->m_canMove = true;
				_unit->GetAIInterface()->disable_melee = false;
				_unit->GetAIInterface()->MoveTo( SHIP_CENTER_X, SHIP_CENTER_Y, 151.0f, 0 );
				TwilightFlamesTimer = TickNow + 10000 + RandomUInt() % 25000;
				TwilightBreathTimer = TickNow + 10000 + RandomUInt() % 25000;
				ConsumingShroudTimer = TickNow + 10000 + RandomUInt() % 25000;
			}
		}
		//bombard the ship from distance
		if( EnableLandCombat <= 1 && TwilightOnSlaughtTimer < TickNow )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
			sp.minrange = 0.0f;
			sp.maxrange = 60.0f;
			sp.procChance = 50;
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			if( t )
			{
				_unit->CastSpell( t, SPELL_ID_TWILIGHT_ONSLAUGHT, false ); 
				TwilightOnSlaughtTimer = TickNow + (SPELL_COOL_TWILIGHT_ONSLAUGHT + RandomUInt() % SPELL_COOL_TWILIGHT_ONSLAUGHT)*DifficultyScale;
			}
		}
		//land on the ship and fight on heroic diff
		if( EnableLandCombat == 2 )
		{
			if( TwilightFlamesTimer < TickNow )
			{
				_unit->CastSpell( tank, SPELL_ID_TWILIGHT_FLAMES, false );
				TwilightFlamesTimer = TickNow + (SPELL_COOL_TWILIGHT_FLAMES + RandomUInt() % SPELL_COOL_TWILIGHT_FLAMES)*DifficultyScale;
			}
			else if( TwilightBreathTimer < TickNow )
			{
				_unit->CastSpell( tank, SPELL_ID_TWILIGHT_BREATH, false );
				TwilightBreathTimer = TickNow + (SPELL_COOL_TWILIGHT_BREATH + RandomUInt() % SPELL_COOL_TWILIGHT_BREATH)*DifficultyScale;
			}
			else if( ConsumingShroudTimer < TickNow )
			{
				_unit->CastSpell( tank, SPELL_ID_CONSUMING_SHROUD, false );
				ConsumingShroudTimer = TickNow + (SPELL_COOL_CONSUMING_SHROUD + RandomUInt() % SPELL_COOL_CONSUMING_SHROUD)*DifficultyScale;
			}
		}
		//cause blizz says so
		if( _unit->GetHealthPct() < 20 )
		{
			RemoveAIUpdateEvent();
			_unit->Despawn( 1, 0 );
		}
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
    }
}; 

class HarpoonGun : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HarpoonGun);

	uint32 ReloadTimer;

    HarpoonGun(Creature* pCreature) : CreatureAIScript(pCreature)   {   }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 10000 );
		ReloadTimer = 0;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_melee = true;
	}
    void AIUpdate()
    {
		if( _unit->isCasting() )
			return;

		if( ReloadTimer != 0 && ReloadTimer > GetTickCount() )
		{
//			_unit->CastSpell( _unit, 108039, false ); //reload visual
			return;
		}
		else
			ReloadTimer = 0;

		//get a non yet targeted dragon that we will harass with harpoon
		float BestDist = 999999999.0f;
		Unit *BestTarget = NULL;
		InRangeSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( (*itr)->IsUnit() == false || SafeUnitCast( *itr )->isAlive() == false )
				continue;
			if( (*itr)->GetEntry() != NPC_ID_ASSAULT_DRAKE || SafeUnitCast( *itr )->HasAura( 108038 ) )	
				continue;
			float tdist = (*itr)->GetDistance2dSq( _unit );
			if( tdist < BestDist )
			{
				BestDist = tdist;
				BestTarget = SafeUnitCast(*itr);
			}
		}
		_unit->ReleaseInrangeLock();

		if( BestTarget != NULL )
		{
			_unit->CastSpell( BestTarget, 108038, false ); //harpoon visual
			ReloadTimer = GetTickCount() + 50000; //timer to adjust fight speed ?
			//mark targets as we are forcing them to land near hapoon. Might be enough to use the aura
//			int64 *harpooned = BestTarget->GetCreateIn64Extension( NPC_ID_ASSAULT_DRAKE );
//			*harpooned = _unit->GetGUID();
		}
	}

	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
    }
}; 

class SkyfireCannon : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SkyfireCannon);

    SkyfireCannon(Creature* pCreature) : CreatureAIScript(pCreature)   {   }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 5000 );
		_unit->SetFaction( 11 ); //anything that could attack others
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_melee = true;
		for( uint32 i = 0;i<SCHOOL_COUNT;i++)
			_unit->ModGeneratedThreatModifyerPCT( i, -99 );
	}
    void AIUpdate()
    {
		if( _unit->isCasting() )
			return;

		AI_Spell sp;
		sp.spelltargetType = TTYPE_AT_RANGE;
		sp.minrange = 0.0f;
		sp.maxrange = 30;
		sp.procChance = 30;
		Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
		if( t )
		{
			_unit->GetAIInterface()->SendAIReaction( t->GetGUID(), AI_REACTION_CLIENT_AUTO_ALWAYS_FACE_TARGET );
			if( RandChance( 70 ) )
				_unit->CastSpell( t, 108010, true ); //Heavy Slug - 17k dmg
			else
				_unit->CastSpell( t, 108040, true ); //Artillery Barrage - 110k DMG
		}
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
    }
}; 

// circle around the ship and randomly spawn mobs on the ship
class TwilightAssaultDrake : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TwilightAssaultDrake);

    TwilightAssaultDrake(Creature* pCreature) : CreatureAIScript(pCreature)   {   }
	uint32 TwilightBarrageTimer;
	void OnLoad()
	{
		//find the NPC that represents ship location
//		Unit *center = _unit->GetMapMgr()->GetCreatureByEntry( NPC_ID_SHIP_LOCATION );
		//generate random waypoints around the ship
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->disable_combat = true;
			_unit->GetAIInterface()->disable_targeting = true;
			_unit->GetAIInterface()->m_moveFly = true;
			_unit->GetAIInterface()->setMoveType( MOVEMENTTYPE_CIRCLEWP );
#define STEPS_COUNT_TO_CIRCLE_SHIP	20
#define	RADIUS_SMALLEST				40.0f	//need this to know when we should cast spell at the ship
			float center_x = SHIP_CENTER_X;
			float center_y = SHIP_CENTER_Y;
			float center_z = SHIP_CENTER_Z;
			float radius_x = RADIUS_SMALLEST;
			float radius_y = 50.0f;
			float radius_z = 20.0f;
			//add waypoints so that the NPC will move in a circle
			WayPointMap WPM;
			WPM.reserve( STEPS_COUNT_TO_CIRCLE_SHIP );
			float AnglePerStep = (2*M_PI) / 16;
			//chance to fly counter clockwise
			if( RandChance( 50 ) )
				AnglePerStep = -AnglePerStep;
			for( int32 i=0;i<STEPS_COUNT_TO_CIRCLE_SHIP;i++)
			{
				float angle = i * AnglePerStep;
				float cur_radius_x = radius_x / 2 + RandomFloat( radius_x );
				float cur_radius_y = radius_y / 2 + RandomFloat( radius_y );
				float cur_X = center_x + (cur_radius_x*(cosf(angle)))+RandomFloat( 10.0f );
				float cur_Y = center_y + (cur_radius_y*(sinf(angle)))+RandomFloat( 10.0f );
				float DistanceToCenter = Distance2DSq( cur_X, cur_Y, center_x, center_y );
				float DistancePCT = DistanceToCenter/((radius_x+radius_y)/2);
				float cur_Z = center_z + radius_z*DistancePCT/100.0f + RandomFloat( radius_z );
				WayPoint *wp = new WayPoint;
				memset( wp, 0, sizeof( wp ) );
				wp->id = i;
				wp->x = cur_X;
				wp->y = cur_Y;
				wp->z = cur_Z;
				wp->flags = AI_INTERFACE_DB_FLAG_MOVE_FLY;
				WPM.push_back( wp );
			}/**/
			_unit->GetAIInterface()->SetWaypointMap( &WPM );

		RegisterAIUpdateEvent( 2000 );
		TwilightBarrageTimer = GetTickCount() + 5000;
	}

    void AIUpdate()
    {
		//harpooned yet ?
		if( _unit->HasAura( 108038 ) && _unit->GetAIInterface()->disable_melee == true )
		{
			//stop circleing
			_unit->GetAIInterface()->setMoveType( MOVEMENTTYPE_DONT_USE_WP );
			//land on ship
			_unit->GetAIInterface()->MoveTo( SHIP_CENTER_X, SHIP_CENTER_Y, 151.0f, 0 );
			//combat capable
			_unit->GetAIInterface()->disable_melee = false;
			_unit->GetAIInterface()->disable_combat = false;
			_unit->GetAIInterface()->disable_targeting = false;
			_unit->GetAIInterface()->m_moveFly = false;
		}
		//get a random location and we bombard it
		if( _unit->GetAIInterface()->disable_melee == true )
		{
			float DistanceToCenter = _unit->GetDistanceSq( SHIP_CENTER_X, SHIP_CENTER_Y, SHIP_CENTER_Z );
			if( DistanceToCenter < ( 2 * RADIUS_SMALLEST ) * ( 2 * RADIUS_SMALLEST ) )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
				sp.minrange = 0.0f;
				sp.maxrange = 2 * RADIUS_SMALLEST;
				sp.procChance = 5;
				if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
					sp.procChance = 7;
				else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
					sp.procChance = 10;
				else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
					sp.procChance = 15;
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				if( t )
				{
					_unit->GetAIInterface()->SendAIReaction( t->GetGUID(), AI_REACTION_CLIENT_AUTO_ALWAYS_FACE_TARGET );
					_unit->CastSpell( t, 107439, true ); //Twilight Barrage
					TwilightBarrageTimer = GetTickCount() + 5000;
				}
			}
		}
	}

	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		int64 *DrakesRemaining = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_ASSAULT_DRAKE );
		*DrakesRemaining = *DrakesRemaining - 1;
    }
}; 

class TwilightSapper : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TwilightSapper);

	uint32 BombArmTime;
    TwilightSapper(Creature* pCreature) : CreatureAIScript(pCreature)    {   }
	void OnLoad()
	{
		BombArmTime = GetTickCount() + 15000;
		AI_Spell sp;
		sp.spelltargetType = TTYPE_AT_RANGE_PLAYER_RANDOM;
		sp.minrange = 0.0f;
		sp.maxrange = 60.0f;
		sp.procChance = 100;
		Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
		if( t )
			_unit->GetAIInterface()->AttackReaction( t, 10, 0 );
		RegisterAIUpdateEvent( 5000 );
	}
    void AIUpdate()
    {
		if( BombArmTime > GetTickCount() )
			return;
		AI_Spell sp;
		sp.spelltargetType = TTYPE_AT_RANGE_PLAYER;
		sp.minrange = 0.0f;
		sp.maxrange = 5.0f;
		sp.procChance = 100;
		Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
		if( t )
		{
			_unit->CastSpell( t, 107518, false ); //detonate
			_unit->Despawn( 1, 0 );
		}
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
    }
};

class TwilightEliteSlayer : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TwilightEliteSlayer);

	uint32 DreadBladeTimer;
    TwilightEliteSlayer(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 2000 );
		DreadBladeTimer = 0;
	}
    void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL || _unit->isCasting() )
		{
			return;
		}
		uint32 TickNow = GetTickCount();
		if( DreadBladeTimer > TickNow )
		{
			return;
		}

		if( RandChance( 20 ) )
			_unit->CastSpell( tank, 107595, false ); //Blade Rush
		else
			_unit->CastSpell( tank, 107567, false ); //Brutal Strike

		DreadBladeTimer = TickNow + 15000;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DreadBladeTimer = TickNow + 12000;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DreadBladeTimer = TickNow + 7000;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DreadBladeTimer = TickNow + 5000;
	}
	void OnDied(Unit *mKiller)
	{
		int64 *AddsActive = _unit->GetMapMgr()->GetCreateIn64Extension( SPELL_ID_DECK_FIRE );
		*AddsActive = *AddsActive - 1;
		RemoveAIUpdateEvent();
    }
}; 
 
class TwilightEliteDreadblade : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TwilightEliteDreadblade);

	uint32 DreadBladeTimer;
    TwilightEliteDreadblade(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		RegisterAIUpdateEvent( 2000 );
		DreadBladeTimer = 0;
	}
    void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL || _unit->isCasting() )
		{
			return;
		}
		uint32 TickNow = GetTickCount();
		if( DreadBladeTimer > TickNow )
		{
			return;
		}

		if( RandChance( 20 ) )
			_unit->CastSpell( tank, 107595, false ); //Blade Rush
		else
			_unit->CastSpell( tank, 107558, false ); //Degeneration

		DreadBladeTimer = TickNow + 15000;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DreadBladeTimer = TickNow + 12000;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DreadBladeTimer = TickNow + 7000;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DreadBladeTimer = TickNow + 5000;

	}
	void OnDied(Unit *mKiller)
	{
		int64 *AddsActive = _unit->GetMapMgr()->GetCreateIn64Extension( SPELL_ID_DECK_FIRE );
		*AddsActive = *AddsActive - 1;
		RemoveAIUpdateEvent();
    }
}; 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define TEMP_DISABLE_WHILE_TESTING 
//#define TEMP_DISABLE_TIMESTALL_WHILE_TESTING
//#define TEMP_DISABLE_BARREL_ROLL_WHILE_TESTING 
//#define TEMP_DISABLE_STUN_WHILE_TESTING
//#define TEMP_DISABLE_AMALGAMATION_DESPAWN_WHILE_TESTING

#define NPC_ID_CORRUPTION				56161	//at least 1 corruption needs to be active all the time. Spawn 4 at the beggining of the fight, then 2 for each plate
#define NPC_ID_CORRUPTED_BLOOD			53889
#ifdef TEMP_DISABLE_TIMESTALL_WHILE_TESTING 
	#define NPC_COOL_CORRUPTED_BLOOD		1000 //keep on waiting
#else
	#define NPC_COOL_CORRUPTED_BLOOD		10000
#endif
#define NPC_ID_AMALGAMATION					53890	//damage dealer
#define NPC_ID_BURNING_TENDON				56575	//despawn armor plate if players kill this in time
#define NPC_COOL_BURNING_TENDON				30000	//time for players to kill the plate. Note that this should be a stacking timer !
#define MAX_CORRUPTION_COUNT				(4+2+2)
//#define SPELL_ID_BARREL_ROLL				105528	//not sure, this 105048 is just knockback, this is 105528 self knockback
#define SPELL_ID_BARREL_ROLL				48030	//defently bad, it kicks you high into the air, at least it is more visual then knockback
#define SPELL_COOL_BARREL_ROLL				15000
#define NPC_ID_GRASPING_TENDRILLS			53888		//spawned at every corruption location. It only activates when corruption is not present and will slow nearby player and avoid them to fall on barrel roll
#define SPELL_ID_GRASPING_TENDRILLS			105510
#define SPELL_ID_GRASPING_TENDRILLS_AURA	105563
//#define SPELL_ID_BLOOD_CORRUPTION_DEATH	106199
//#define SPELL_ID_BLOOD_OF_DEATHWING		106201
//#define SPELL_ID_BLOOD_CORRUPTION_EARTH	106200
//#define SPELL_ID_BLOOD_OF_NELTHARION	106213

#define SPELL_ID_SEARING_PLASMA			105479
#define SPELL_COOL_SEARING_PLASMA		20000
#define SPELL_ID_FIERY_GRIP				105490
#define SPELL_COOL_FIERY_GRIP			20000
#define SPELL_ID_ABSORB_BLOOD			105241
#define SPELL_ID_ABSORBED_BLOOD			105248
#ifdef TEMP_DISABLE_TIMESTALL_WHILE_TESTING
	#define REQUIRED_BLOOD_ABSORB_COUNT	1
#else
	#define REQUIRED_BLOOD_ABSORB_COUNT	9
#endif
#define SPELL_ID_SUPERHEATED			106264
#define SPELL_ID_NUCLEAR_BLAST			105845
#define SPELL_ID_DEGRADATION			106005
#define SPELL_ID_BURST					105219
#define SPELL_ID_RESIDUE				105223	//makes a fiery animation instead lootable corpse
#define SPELL_ID_SEAL_ARMOR_BREACH1		105847
#define SPELL_ID_SEAL_ARMOR_BREACH2		105848

void DespawnEncounterSpawns(Unit *_unit, const uint32 *DespawnOldEncounterDynamicEntries)
{
	for( uint32 i=0;i<20 && DespawnOldEncounterDynamicEntries[i]!=0;i++)
	{
		uint32 SelectedEntry = DespawnOldEncounterDynamicEntries[ i ];
		bool FoundSpawn = true;
		while( FoundSpawn )
		{
			Creature *c = _unit->GetMapMgr()->GetCreatureByEntry( SelectedEntry );
			if( c )
				c->Despawn( 0, 0 ); //insta despawn to not find this NPC again
			else
				FoundSpawn = false;
		}
	}
}

class DeathwingSpine : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(DeathwingSpine);
/* 
    Roll – Throughout the encounter, Deathwing tries to throw players from his back when he senses that they are all standing on the same side of his spine. During a roll, Deathwing throws off all players and other creatures not attached to his back.
    
    Elementium Reinforced Plates
    The glancing strike from the Dragon Soul destroyed one of the large armor plates on Deathwing's back and loosened another three. Players must remove all of these plates to give Thrall a chance to get a clear shot.
        Burning Tendons
            Players expose Burning Tendons when they pry up one of Deathwing's armor plates. Destroying these fibrous strands is the key to removing an armor plate.
            Seal Armor Breach – After the players pry up an armor plate, the Burning Tendons rapidly pull the plate closed, protecting the tendons from further damage and preventing the plate from flying off.

    Deathwing's Immune System
    The creatures that comprise Deathwing's Immune System emerge from the holes left by torn off armor plates. Deathwing defends himself with increasing intensity as more plates are torn off.
        Grasping Tendrils – These tendrils sprout from holes in Deathwing's back that aren't currently occupied by Corruption tentacles. The Tendrils grasp any player that wander too close, reducing their movement speed by 35% and inflicting 2625 to 3375 Fire damage every 5 sec. until they move away. This effect prevents players from being thrown off Deathwing's back.
        Corruption – These tentacles are dramatic physical manifestations of the Old God Corruption that runs rampant through Deathwing's body. They emerge first from the holes in his back left by armor plates, and they seal the wound completely, preventing any other creatures from emerging from it.
            Searing Plasma – The Corruption coats the victim in the searing blood of Deathwing. The thick blood absorbs up to 200000 healing done to the target and causes the victim to cough up blood. The violent coughing periodically inflicts 10000 Physical damage every 10 sec.
            Fiery Grip – The Corrupton grips victims with a whip-like cord of plasma, stunning them for up to 30 sec. and inflicting 60000 Fire damage every 3 sec. The corruption must channel to maintain this effect, and players may break the channel by inflicting significant damage to the tentacle. This effect prevents players from being thrown off Deathwing's back.
        Hideous Amalgamation – These unstable creatures are a patchwork of Elementium Armor fragments and pieces of the former Dragon's body held together by his molten blood.
            Absorb Blood – The Amalgamation absorbs nearby Corrupted Blood residue. The Amalgamation grows more unstable with each Residue absorbed, increasing its damage by 10% and attack speed by 20%. At $105248U stacks it becomes superheated.
            Superheated Nucleus – The core of the Amalgamation becomes critically unstable, causing it to inflict 13050 to 16950 Fire damage to all enemies every 3 sec. If the players destroy the Amalgamation in this state, then it will trigger a Nuclear Blast.
            Nuclear Blast – The superheated nucleus of the Amalgamation triggers a massive explosion, inflicting 350625 to 399375 Fire damage to targets within 8 yds. This powerful blast pries up one of Deathwing's Elementium plates if it is within range.
            Heroic Degradation – Destroyed Amalgamations degrade the maximum health of all enemy targets by 5%. This effect occurs regardless of the state of the nucleus and stacks up to 15 times.
            Heroic Blood Corruption: Death – Deathwing's corruption courses through your veins. This corruption takes hold if it is not removed quickly, infecting the player with the Blood of Deathwing. When dispelled this effect jumps to a new target and can mutate into Blood Corruption: Earth.
                Heroic Blood of Deathwing – The blood of Deathwing explodes violently from your veins, inflicting 787500 to 962500 Fire damage to you and several other players.
            Heroic Blood Corruption: Earth – A shadow of Neltharion courses through your veins. This corruption takes hold if it is not removed quickly, infecting the player with the Blood of Neltharion. When dispelled this effect jumps to a new target and can mutate into Blood Corruption: Death.
                Heroic Blood of Neltharion – The barest hint of a shadow of the former Earth-Warder suffuses your blood. Damage taken from all sources is reduced by 20%. This effect can stack up to $106213U times.
        Corrupted Blood – These living globs of Deathwing's blood appear fragile, but leave behind an indestructible residue when killed.
            Burst – Corrupted Blood explodes when destroyed, inflicting 9500 to 10500 Physical damage to nearby enemies within 200 yards.
            Residue – Destroying a Corrupted Blood leaves behind an indestructible residue. It slowly creeps toward the nearest hole in Deathwing's Back where it reconstitutes into a new Corrupted Blood.
*/
	uint32	phase;
	float	DifficultyScale;
	uint32	CorruptedBloodSpawnTimer[MAX_CORRUPTION_COUNT];
	uint32	BarrelRollTimerDetect,BarrelRollTimerRoll;
	uint64	ArmorPlates[3];
	uint32	ArmorPlateDespawnTimer;
	uint32	BurningTendonDespawnTimer[3];//for each plate

	DeathwingSpine(Creature* pCreature) : CreatureAIScript(pCreature)
	{
        phase = 0;
	}
    
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		//make him attack faster as difficulty increases
		uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now * DifficultyScale ) );	//attack a bit faster cause there are more healers

		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->setOutOfCombatRange( 700.0f * 700.0f );
		_unit->bInvincible = true;

		RegisterAIUpdateEvent( 1000 );
		memset( ArmorPlates, 0, sizeof( ArmorPlates ) );
	}

	void OnDied(Unit *mKiller)
	{
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		*EncounterStatus |= EXTENSION_FLAG_SPINE_DIED;
		_unit->GetMapMgr()->CreateCreature( MOB_ENTRY_DRAGONSOUL_TELEPORTER, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
		RemoveAIUpdateEvent();
		//try to see players lie about BOSS difficulty
		LOG_CREATURE_DEATH( _unit->GetEntry(), mKiller );
		//remove encounter debuffs
		InRangePlayerSetRecProt::iterator itr,itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd();)
		{
			itr = itr2;
			itr2++;
			if( !SafeUnitCast( *itr )->isAlive() ) 
				continue;
			( *itr )->RemoveAura( SPELL_ID_DEGRADATION, 0, AURA_SEARCH_NEGATIVE, MAX_AURAS );
		}
		_unit->ReleaseInrangeLock();
		if( mKiller && mKiller->IsPlayer() && SafePlayerCast( mKiller )->GetGuild() )
			SafePlayerCast( mKiller )->GetGuild()->GuildNewsAdd( GUILD_NEWS_LOG_DUNGEON_ENCOUNTER, mKiller->GetGUID(), 1291 );
    }

	void AIUpdate()
    {
		const uint32 DespawnOldEncounterDynamicEntries[] = { NPC_ID_CORRUPTION, NPC_ID_CORRUPTED_BLOOD, NPC_ID_AMALGAMATION, NPC_ID_BURNING_TENDON, NPC_ID_GRASPING_TENDRILLS, 0 };
		if( _unit->isAlive() == false )
			return;

		//!! we are only fight controller, we do not fight, beware to not loose agro !
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			phase = 0;
			return;
		}

		uint32 TickNow = GetTickCount();

		// Init phase
		float SpawnX[MAX_CORRUPTION_COUNT]={ -13868.5, -13841, -13868.5, -13842.8, -13867, -13841.2, -13870.4, -13839.3 };
		float SpawnY[MAX_CORRUPTION_COUNT]={ -13667, -13666.9, -13654, -13654, -13638.5, -13635.2, -13614.4, -13614.8 };
		int64 *ActiveCorruptions = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 );
		if( phase == 0 || phase > 4 )
		{
			//despawn old
			DespawnEncounterSpawns( _unit, DespawnOldEncounterDynamicEntries );

			//now spawn 4 corruptions
			*ActiveCorruptions = 0;
			for( int i=0;i<4;i++)
			{
				Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_CORRUPTION, true, SpawnX[i], SpawnY[i], 263.0f + i / 2 );
				//store at what location we are spawned so we can spawn corrupted bloods properly
				if( c )
				{
					int64 *CorruptionIndex = c->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 );
					*CorruptionIndex = i;
					int64 *CorruptionIndexMap = c->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 + i + 1 );
					*CorruptionIndexMap = 1;
					int64 *BloodIndexMap = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTED_BLOOD*100 + i + 1 );
					*BloodIndexMap = 0;
					*ActiveCorruptions = *ActiveCorruptions + 1;
				}
			}
			//now spawn tendrils that slow down players if they are close
			for( int i=0;i<MAX_CORRUPTION_COUNT;i++)
			{
				Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_GRASPING_TENDRILLS, true, SpawnX[i], SpawnY[i], 263.0f + i / 2 );
				if( c )
				{
					int64 *CorruptionIndex = c->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 );
					*CorruptionIndex = i;
				}
			}	
			//despawn armor plates and spawn new ones
			bool FoundPlate = true;
			GameObject *g;
//			uint32 PlateEntries[3]={ 209623, 209631, 209632 };
			uint32 PlateEntries[3]={ 209624, 209625, 209626 };
			while( FoundPlate )
			{
				FoundPlate = false;
				for( int i=0;i<3;i++)
				{
					g = _unit->GetMapMgr()->GetGameObjectByEntry( PlateEntries[i] );
					if( g )
					{
						g->Despawn( 0, 0 ); //insta despawn to not find this NPC again
						FoundPlate = true;
					}
				}
			}
			g= _unit->GetMapMgr()->CreateAndSpawnGameObject( 209624, -13855.0f, -13639.7f, 267.866f, 1.58825f, 1.0f, true);
			if( g )
				ArmorPlates[0] = g->GetGUID();
			g= _unit->GetMapMgr()->CreateAndSpawnGameObject( 209625, -13855.0f, -13619.1f, 269.869f, 1.58825f, 1.0f, true);
			if( g )
				ArmorPlates[1] = g->GetGUID();
			g= _unit->GetMapMgr()->CreateAndSpawnGameObject( 209626, -13855.4f, -13597.8f, 272.356f, 1.58825f, 1.0f, true);
			if( g )
				ArmorPlates[2] = g->GetGUID();
			ArmorPlateDespawnTimer = 0;
			//corrupted bloods only spawn where there is no corruption
			for( uint32 i=0;i<MAX_CORRUPTION_COUNT;i++)
				CorruptedBloodSpawnTimer[i]=0;
			BarrelRollTimerDetect = ArmorPlateDespawnTimer = TickNow + 30000 + RandomUInt() % 25000;
			BarrelRollTimerRoll = MAX_INT;
			memset( BurningTendonDespawnTimer, 0, sizeof( BurningTendonDespawnTimer ) );
			//move to next phase
            phase = 1;
		}

		//fight ended. Nothing to do anymore
		if( phase > 3 )
			return;
		//in case all corruptions died then spawn a new one
		if( *ActiveCorruptions <= 0 )
		{
			//pick a non taken location
			uint32 MaxCorruptionLocCount = 4 + ( phase - 1 ) * 2;
			uint32 StartAt = RandomUInt( MaxCorruptionLocCount );
			int32 RandomLoc = -1;
			for( uint32 i=0;i<MaxCorruptionLocCount;i++)
			{
				RandomLoc = ( StartAt + i ) % MAX_CORRUPTION_COUNT;
				int64 *CorruptionIndexMap = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 + RandomLoc + 1 );
				if( *CorruptionIndexMap == 0 )
					break;
			}
			if( RandomLoc != -1 )
			{
				Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_CORRUPTION, true, SpawnX[RandomLoc], SpawnY[RandomLoc], 263.0f + RandomLoc / 2 );
				*ActiveCorruptions = *ActiveCorruptions + 1;
				int64 *CorruptionIndex = c->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 );
				*CorruptionIndex = RandomLoc;
				int64 *CorruptionIndexMap = c->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 + RandomLoc + 1 );
				*CorruptionIndexMap = 1;
			}
		}

		//4,2,2,2 coruptions are spawned and no plates removed
        if( phase == 1 || phase == 2 || phase == 3 )
        {
			//spawn corrupted bloods from the locations where corruptions are dead
			for( int i=0; i<4+(phase-1)*2; i++ )
				if( CorruptedBloodSpawnTimer[i] < TickNow )
				{
					int64 *CorruptionIndexMap = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 + i + 1 );
					int64 *BloodIndexMap = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTED_BLOOD*100 + i + 1 );
					int64 *CorruptedBloodCounter = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTED_BLOOD );
					if( *CorruptionIndexMap == 0 && *BloodIndexMap == 0 && *CorruptedBloodCounter < 50 )
					{
						Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_CORRUPTED_BLOOD, true, SpawnX[i], SpawnY[i], 263.0f + i / 2 );
						CorruptedBloodSpawnTimer[i] = TickNow + (NPC_COOL_CORRUPTED_BLOOD + RandomUInt() % NPC_COOL_CORRUPTED_BLOOD)*DifficultyScale;				
						int64 *BloodIndex = c->GetCreateIn64Extension( NPC_COOL_CORRUPTED_BLOOD*100 );
						*BloodIndex = i;
						*BloodIndexMap = 1;
						*CorruptedBloodCounter = *CorruptedBloodCounter + 1;
					}
				}
        }
		
		//check if we can move to the next phase 
		int64 *NuclearBlastHapppenedMap = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_AMALGAMATION*100 );
		if( *NuclearBlastHapppenedMap )
		{
			*NuclearBlastHapppenedMap = 0;
			Creature *c;
			if( phase == 1 )
				c = _unit->GetMapMgr()->CreateCreature( NPC_ID_BURNING_TENDON, true, -13855.0f, -13639.7f-10.0f, 267.866f );
			else if( phase == 2 )
				c = _unit->GetMapMgr()->CreateCreature( NPC_ID_BURNING_TENDON, true, -13855.0f, -13619.1f-10.0f, 269.866f );
			else if( phase == 3 )
				c = _unit->GetMapMgr()->CreateCreature( NPC_ID_BURNING_TENDON, true, -13855.4f, -13597.8f-10.0f, 272.866f );
			//in case the time is too small to kill this tendon for this group, then next time it will stay more
			BurningTendonDespawnTimer[phase] += NPC_COOL_BURNING_TENDON;
			c->Despawn( BurningTendonDespawnTimer[phase], 0 );
			//try to make the anymation work. I doubt it would :(
			c->CastSpell( ArmorPlates[ phase - 1 ], SPELL_ID_SEAL_ARMOR_BREACH1, false ); //this is a channeled spell ( i think )
		}
		int64 *TendonDiedMap = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_BURNING_TENDON*100 );			
		if( *TendonDiedMap != 0 && phase < 4 && ArmorPlateDespawnTimer < TickNow )
		{
			//spawn 2 new corruptions
			uint32 DefaultCorruptionCount = 4;
			uint32 PhaseCorruptionCount = (phase-1)*2;
			uint32 CurrentCorruptionCount = DefaultCorruptionCount + PhaseCorruptionCount;
			for( int i=CurrentCorruptionCount;i<CurrentCorruptionCount+2;i++)
			{
				Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_CORRUPTION, true, SpawnX[i], SpawnY[i], 263.0f + i / 2 );
				//store at what location we are spawned so we can spawn corrupted bloods properly
				if( c )
				{
					int64 *CorruptionIndex = c->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 );
					*CorruptionIndex = i;
					int64 *CorruptionIndexMap = c->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 + i + 1 );
					*CorruptionIndexMap = 1;
					*ActiveCorruptions = *ActiveCorruptions + 1;
				}
			}
			//despawn a plate
			GameObject *GO = _unit->GetMapMgr()->GetGameObject( ArmorPlates[ phase - 1 ] );
			if( GO )
				GO->Despawn( 0, 0 );
			//avoid having insta win the fight but exploding all 3 amalgamations at once
			ArmorPlateDespawnTimer = TickNow + 20000;
			phase++;
			//we won the fight ? Not dead sure how to handle this, we will become visible and fightable
			if( phase == 4 )
			{
				_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, 35338 );
				_unit->GetAIInterface()->disable_melee = false;
				_unit->GetAIInterface()->m_canMove = true;
				_unit->bInvincible = false;
				_unit->SetScale( 5.0f );
				_unit->SetMaxHealth( 100 );
				_unit->SetHealth( 100 );
				DespawnEncounterSpawns( _unit, DespawnOldEncounterDynamicEntries );
				BarrelRollTimerDetect = MAX_INT;
				*ActiveCorruptions = MAX_CORRUPTION_COUNT;
			}
			*TendonDiedMap = 0;
		}

		//throw off players periodically that are not fasten
		uint32 MaxActiveCorruptionsInThisPhase =  4 + ( phase - 1 ) * 2;
		if( BarrelRollTimerDetect < TickNow 
			&& BarrelRollTimerRoll == MAX_INT
			&& MaxActiveCorruptionsInThisPhase > *ActiveCorruptions ) //wait until at least 2 corruptions are killed
		{
			
			//check to see if deathwing should do a barrel roll
			uint32 PlayersOnSide[2] = { 0, 0 };
			float MiddleX = -13855;
			InRangePlayerSetRecProt::iterator itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
			{
				if( (*itr2)->GetPositionX() < MiddleX )
					PlayersOnSide[0]++;
				else
					PlayersOnSide[1]++;
			}
			_unit->ReleaseInrangeLock();
			uint32 SumPlayers = PlayersOnSide[0] + PlayersOnSide[1];
			uint32 PCTOneside = SumPlayers * 20 / 100; //with 15 players that is [0,3]
			if( PlayersOnSide[0] <= PCTOneside || PlayersOnSide[1] <= PCTOneside )
			{
				BarrelRollTimerRoll = TickNow + 7000;
{
	char msg[500];
	sprintf( msg, "Debug MSG : Players on left side %d, on right side %d. Sarting a roll in %d seconds", PlayersOnSide[0], PlayersOnSide[1], ( BarrelRollTimerRoll - TickNow ) / 1000 );
	_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, msg );
}
			}
		}
#ifndef TEMP_DISABLE_BARREL_ROLL_WHILE_TESTING
		if( BarrelRollTimerRoll < TickNow )
		{
			InRangePlayerSetRecProt::iterator itr3;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr3 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr3 != _unit->GetInRangePlayerSetEnd(); itr3++)
			{
				if( (*itr3)->isAlive() == false )
					continue;
				if( (*itr3)->HasAura( SPELL_ID_GRASPING_TENDRILLS_AURA, 0, AURA_SEARCH_NEGATIVE ) )
					continue;
				if( (*itr3)->HasAura( SPELL_ID_FIERY_GRIP, 0, AURA_SEARCH_NEGATIVE ) )
					continue;
				//throw them off our back
				(*itr3)->CastSpell( (*itr3), SPELL_ID_BARREL_ROLL, true );	
				//teleport them back on the ship
				if( sEventMgr.HasEvent( (*itr3), EVENT_PLAYER_TELEPORT ) == false )
				{
					//tell them what happened to not blame it on a bug
					(*itr3)->BroadcastMessage( "You got rickrolled. Don't stand on the same side !. You will get teleported soon. I hope" );
					LocationVector v( 13485, -12134, 155, 3.0 );
					(*itr3)->SafeTeleportDelayed( (*itr3)->GetMapId(), (*itr3)->GetInstanceID(), v, 1000 );
				}
			}
			_unit->ReleaseInrangeLock();
			//unsummon all amalgamations
#ifndef TEMP_DISABLE_AMALGAMATION_DESPAWN_WHILE_TESTING
			InRangeSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( (*itr)->IsCreature() == false )
					continue;
				if( (*itr)->GetEntry() != NPC_ID_AMALGAMATION )	
					continue;
				SafeCreatureCast( *itr )->Despawn( 1, 0 );
			}
			_unit->ReleaseInrangeLock();
#endif
			BarrelRollTimerRoll = MAX_INT;
			BarrelRollTimerDetect = TickNow + SPELL_COOL_BARREL_ROLL;
		}
#endif
	}
};

class Corruption : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(Corruption);

	float	DifficultyScale;
	uint32	SearingPlasmaTimer;
	uint32	FieryGripTimer,FieryGripCounter;
	uint32	phase;

    Corruption(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		//make him attack faster as difficulty increases
		uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now * DifficultyScale ) );	//attack a bit faster cause there are more healers

		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_melee = true;
		phase = 0;
		FieryGripCounter = 0;

		RegisterAIUpdateEvent( 1000 );
	}
    void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			phase = 0;
			return;
		}
		if( phase == 0 )
		{
			SearingPlasmaTimer = GetTickCount() + 25000;
			int64 *CorruptionIndex = _unit->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 );
			FieryGripTimer = GetTickCount() + 15000 + 15000 * ( *CorruptionIndex );
			phase = 1;
		}
		uint32 TickNow = GetTickCount();
		//put a healing absorbant shield on a target
		//the other spell is channeled so we will cast this one as triggered to be able to cast it parralel
		if( SearingPlasmaTimer < TickNow )
		{
			AI_Spell sp;
			sp.spelltargetType = TTYPE_ENEMY_FOR_BUFF;
			sp.minrange = 0.0f;
			sp.maxrange = 60.0f;
			sp.procChance = 100;
			sp.spell = dbcSpell.LookupEntryForced( SPELL_ID_SEARING_PLASMA );
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			if( t )
			{
				SearingPlasmaTimer = TickNow + (SPELL_COOL_SEARING_PLASMA + RandomUInt() % SPELL_COOL_SEARING_PLASMA)*DifficultyScale;
				_unit->CastSpell( tank, SPELL_ID_SEARING_PLASMA, true );	
			}
		}
		//channeled spell - stun 3 targets
#ifndef TEMP_DISABLE_STUN_WHILE_TESTING
		if( FieryGripTimer < TickNow && _unit->isCasting() == false 
//			&& FieryGripCounter < 3 //not blizzlike at all. This spell can stun up to 12 player at start...
			) 
		{
			FieryGripTimer = TickNow + ( SPELL_COOL_FIERY_GRIP + RandomUInt() % SPELL_COOL_FIERY_GRIP ) * DifficultyScale;
			AI_Spell sp;
			sp.spelltargetType = TTYPE_ENEMY_FOR_BUFF;
			sp.minrange = 0.0f;
			sp.maxrange = 60.0f;
			sp.procChance = 100;
			sp.spell = dbcSpell.LookupEntryForced( SPELL_ID_FIERY_GRIP );
			Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
			if( t )
			{
				_unit->CastSpell( t, SPELL_ID_FIERY_GRIP, false );
				FieryGripCounter++;
			}
		}
#endif
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		//decrease corruption counter. We need to have at least one active
		int64 *ActiveCorruptions = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 );
		*ActiveCorruptions = *ActiveCorruptions - 1;
		//mark location as free to get a new spawn on it
		int64 *CorruptionIndex = _unit->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 );
		int64 *CorruptionIndexMap = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 + *CorruptionIndex + 1 );
		*CorruptionIndexMap = 0;
		//spawn an amalgamation in our location
		Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_AMALGAMATION, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
    }
}; 

class CorruptedBlood : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CorruptedBlood);

    CorruptedBlood(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		//when we die we will merge into the closest Amalgamation
	}
    void AIUpdate()   {	}
	void OnDied(Unit *mKiller)
	{
		_unit->CastSpell( _unit, SPELL_ID_BURST, true );
		RemoveAIUpdateEvent();
		int64 *BloodIndex = _unit->GetCreateIn64Extension( NPC_COOL_CORRUPTED_BLOOD*100 );
		int64 *BloodIndexMap = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTED_BLOOD*100 + *BloodIndex + 1 );
		*BloodIndexMap = 0;
		int64 *CorruptedBloodCounter = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTED_BLOOD );
		*CorruptedBloodCounter = *CorruptedBloodCounter - 1;
    }
}; 
/*
class CorruptedBloodResidue : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CorruptedBloodResidue);

    CorruptedBloodResidue(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		_unit->CastSpell( _unit, SPELL_ID_RESIDUE, true ); //we should cast this before we die or else the animation will not show
		_unit->SetDisplayId( 11686 );
	}
    void AIUpdate()   {	}
}; /**/

class HideousAmalgamation : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HideousAmalgamation);

	uint32	CorruptedBloodsAbsorbed;
    HideousAmalgamation(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		CorruptedBloodsAbsorbed = 0;
		_unit->BaseAttackType = SCHOOL_FIRE;
		float DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		//make him attack faster as difficulty increases
		uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now * DifficultyScale ) );	//attack a bit faster cause there are more healers

		RegisterAIUpdateEvent( 3000 );
	}
    void AIUpdate()
    {
		//explode and despawn
		if( CorruptedBloodsAbsorbed < REQUIRED_BLOOD_ABSORB_COUNT )
		{
			InRangeSetRecProt::iterator itr,itr2;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
			{
				itr = itr2;
				itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
				if( (*itr)->IsCreature() == false || SafeUnitCast( *itr )->isAlive() == true )
					continue;
				if( (*itr)->GetEntry() != NPC_ID_CORRUPTED_BLOOD )	
					continue;
				if( (*itr)->GetDistance2dSq( _unit ) > 10.0f * 10.0f )
					continue;
				CorruptedBloodsAbsorbed++;
				_unit->CastSpell( SafeCreatureCast(*itr), SPELL_ID_ABSORB_BLOOD, true );
				int64 *IsAbsorbed = (*itr)->GetCreateIn64Extension( NPC_ID_CORRUPTED_BLOOD*100 );
				if( *IsAbsorbed != 0 )
					continue;
				*IsAbsorbed = 1;
				SafeCreatureCast( (*itr) )->Despawn( 500, 0 );
				_unit->CastSpell( _unit, SPELL_ID_ABSORBED_BLOOD, true ); //marker
//				if( CorruptedBloodsAbsorbed >= REQUIRED_BLOOD_ABSORB_COUNT )
					break;
			}
			_unit->ReleaseInrangeLock();
		}
		//pulse of fire. Direct DMG
		if( CorruptedBloodsAbsorbed >= REQUIRED_BLOOD_ABSORB_COUNT )
			_unit->CastSpell( _unit, SPELL_ID_SUPERHEATED, true );
	}
	void OnDied(Unit *mKiller)
	{
		//pry up the nearby armor plate
		if( CorruptedBloodsAbsorbed >= REQUIRED_BLOOD_ABSORB_COUNT )
		{
			_unit->CastSpell( _unit, SPELL_ID_NUCLEAR_BLAST, true );
			int64 *NuclearBlastHapppenedMap = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_AMALGAMATION*100 );
			*NuclearBlastHapppenedMap = 1;
		}
		if( IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) )
			_unit->CastSpell( _unit, SPELL_ID_DEGRADATION, true );
		RemoveAIUpdateEvent();
    }
};  

class GraspingTendrilCaster : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GraspingTendrilCaster);

	bool CastedAura;
    GraspingTendrilCaster(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->SetDisplayId( 11686 );
		CastedAura = false;
		RegisterAIUpdateEvent( 3000 );
	}
    void AIUpdate()
    {
		//no grasping until tentacle is alive where we are spawned
		int64 *CorruptionIndex = _unit->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 );
		int64 *CorruptionIndexMap = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CORRUPTION*100 + *CorruptionIndex + 1 );
		if( *CorruptionIndexMap != 0 )
		{
			if( CastedAura == true )
			{
				_unit->RemoveAura( SPELL_ID_GRASPING_TENDRILLS );
				CastedAura = false;
			}
			return;
		}
		if( CastedAura == false )
			_unit->CastSpell( _unit, SPELL_ID_GRASPING_TENDRILLS, true );
		CastedAura = true;
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
    }
}; 

class BurningTendons : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BurningTendons);

	BurningTendons(Creature* pCreature) : CreatureAIScript(pCreature) {}
	void OnLoad()
	{
		_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, 38550 );
		_unit->SetScale( 3 );
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_melee = true;
	}
	void OnDied(Unit *mKiller)
	{
		int64 *TendonDiedMap = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_BURNING_TENDON*100 );			
		*TendonDiedMap = 1;
		RemoveAIUpdateEvent();
    }
}; 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char DeathwingMadnessYells[15][113] = {
    "Cling while you can, \"heroes.\" You and your world are doomed.",
    "Ha! I had not realized you fools were still there.",
    "I AM DEATHWING, THE DESTROYER, THE END OF ALL THINGS, INEVITABLE, INDOMITABLE; I AM THE CATACLYSM!",
    "It is good to see you again, Alexstrasza. I have been busy in my absence.",
    "Mere whelps, experiments, a means to a greater end. You will see what the research of my clutch has yielded.",
    "Nefarian, Onyxia, Sinestra... they were nothing. Now you face my ultimate creation.",
    "The Hour of Twilight is nigh; the sun sets on your pitiful mortal existence.",
    "The sea will swallow your smoldering remains.",
    "There is no shelter from my fury.",
    "Twisting your pitiful whelps into mindless abominations, bent only to my will. It was a very... painful process.",
    "You are less than dust, fit only to be brushed from my back.",
    "You have done NOTHING. I will tear your world APART.",
    "Your armor means nothing, your faith even less.",
    "Your efforts are insignificant. I carry you to your deaths.",
    "Your tenacity is admirable, but pointless. You ride into the jaws of the apocalypse.",
};
struct EncounterEventStatus
{
	uint32 Timer;			//timestamp when it will trigger again
	uint32 ProcsUntilNow;
};

bool ShouldForceProc( Unit *_unit, uint32 ProcsUntilNow, uint32 MinProcCount, int32 StartHealth = 90 ) 
{
	int32 AtHealthPCT = StartHealth / MinProcCount;
	int32 ExpectedMinHealthPCTNow = StartHealth - ProcsUntilNow * AtHealthPCT;
	int32 HealthPCTNow = _unit->GetHealthPct();
	if( HealthPCTNow < ExpectedMinHealthPCTNow )
		return true;
	return false;
}

void ConvertToStaticSpawn( Creature *c, bool RegisterUniqueGUID )
{
	if( c && c->static_object == false )
	{
		c->static_object = true; //do not go out of visual range
		c->GetMapMgr()->PushStaticObject( c );
		uint32 x = c->GetMapMgr()->GetPosX(c->GetPositionX());
		uint32 y = c->GetMapMgr()->GetPosY(c->GetPositionY());
		c->GetMapMgr()->UpdateCellActivity( x, y, 1, 2 );
		if( RegisterUniqueGUID )
		{
			int64 *GUID = c->GetMapMgr()->GetCreateIn64Extension( c->GetEntry() );
			*GUID = c->GetGUID();
		}
	}
}

#define NPC_ID_MADNESS_VISUAL			56173	// at center of the black hole. At phase 2 he will lean forward
#define NPC_ID_MADNESS_ATTACKTARGET		57962	// spawned between platform 1 and 2 and can be reached by attacks and spells
#define NPC_ID_MADNESS_FIGHT_CONTROLLER	56174	// spawned at teleport location and will spawn dynamic spawns
#define MADNESS_INDEX_GUID_VISUAL		0
#define MADNESS_INDEX_GUID_ATTACK		1
#define MADNESS_INDEX_ACTIVE_PLATFORM	2
#define MADNESS_INDEX_ACTIVE_PLATFORMS	3
#define MADNESS_LOOSE_HP_PCT_TENTACLE	15
#define SPELL_ID_ASSAULT_ASPECT			1
#define SPELL_COOL_ASSAULT_ASPECT		900000
#define	SPELL_ID_CATACLYSM				106523
#define	SPELL_COOL_CATACLYSM			900000
//#define SPELL_ID_ELEMENTIUM_BOLT		1
//#define SPELL_COOL_ELEMENTIUM_BOLT		1
#define NPC_ID_ELEMENTIUM_BOLT			56262
#define NPC_MINPROC_ELEMENTIUM_BOLT		1
#define NPC_COOL_ELEMENTIUM_BOLT		600000
#define SPELL_ID_ELEMNTIUM_BLAST		105723
#define SPELL_ID_ELEMNTIUM_BLAST_PULSE	110632
#define SPELL_COOL_ELEMNTIUM_BLAST		5000
#define SPELL_ID_CORRUPTING_PARASITE	108649	
#define SPELL_COOL_CORRUPTING_PARASITE	18000	
#define SPELL_MINPROC_CORRUPTING_PARASITE	5	
#define NPC_ID_CORRUPTING_PARASITE		57479	//auto created by spell when it ends
#define SPELL_ID_CORRUPTING_PARASITE_BACKLASH	108597
#define SPELL_ID_UNSTABLE_CORRUPTION	108813
#define SPELL_COOL_UNSTABLE_CORRUPTION	10000	//this does about 600k DMG / tick
#define NPC_ID_HEMORRHAGE_VISUAL_LOC	56359	
//#define SPELL_ID_HEMORRHAGE				105875	//or 106767 actually spawns the mobs
//#define SPELL_ID_HEMORRHAGE_VISUAL		105861
//#define SPELL_COOL_HEMORRHAGE			40000
#define NPC_ID_REGENERATIVE_BLOOD		56263	//or 56710
#define NPC_COOL_REGENERATIVE_BLOOD		37000
#define NPC_MINPROC_REGENERATIVE_BLOOD	4
#define SPELL_ID_REGENERATIVE			105937
#define SPELL_COOL_REGENERATIVE			1000	//1.4M health => needs 140k dps
#define SPELL_ID_DEGENRATIVE_BITE		105841
#define SPELL_COOL_DEGENRATIVE_BITE		1000
#define NPC_ID_LIMB0					56168
#define NPC_ID_LIMB1					56846
#define NPC_ID_LIMB2					56167
#define NPC_ID_LIMB3					56168
#define TENTACLE_START_FIGHT_HP_PCT		95
#define NPC_ID_BLISTERING_TENTACLE		56188	
#define NPC_COOL_LIMB_TENTACLE			600000	//at 25% health pct drop
#define NPC_MINPROC_LIMB_TENTACLE		3
#define SPELL_ID_BURNING_BLOOD			105408
#define SPELL_COOL_BURNING_BLOOD		2000
//#define SPELL_ID_BLISTERING_TENTACLE	1
//#define SPELL_COOL_BLISTERING_TENTACLE	1
#define SPELL_ID_BLISTERING_HEAT		105444	//aura that periodically does AOE DMG
#define SPELL_COOL_BLISTERING_HEAT		2000
#define SPELL_ID_AGONIZING_PAIN			106548
//#define SPELL_COOL_AGONIZING_PAIN		1
#define NPC_ID_MUTATED_CORRUPTION		56471
#define NPC_COOL_MUTATED_CORRUPTION		600000
#define NPC_MINPROC_MUTATED_CORRUPTION	1
#define SPELL_ID_CRUSH					106385
#define SPELL_COOL_CRUSH				8000
#define SPELL_ID_IMPALE					106400	//for full cast. Players have a chance to pop defense
#define SPELL_COOL_IMPALE				12000
#define NPC_ID_CONGEALING_BLOOD			57798
#define NPC_COOL_CONGEALING_BLOOD		37000
#define NPC_MINPROC_CONGEALING_BLOOD	5
#define SPELL_ID_CONGEALING_BLOOD_HEAL	109102
#define NPC_ID_ELEMENTIUM_FRAGMENT		56724
#define NPC_COOL_ELEMENTIUM_FRAGMENT	25000
#define NPC_MINPROC_ELEMENTIUM_FRAGMENT	3
#define SPELL_ID_SHRAPNEL_TARGETED		106794
#define SPELL_ID_SHRAPNEL_DMG			106791
#define SPELL_COOL_SHRAPNEL				15000
#define NPC_ID_ELEMNTIUM_TERROR			56710
#define NPC_COOL_ELEMNTIUM_TERROR		30000
#define NPC_MINPROC_ELEMENTIUM_TERROR	5
#define SPELL_ID_TETANUS				106730
#define SPELL_COOL_TETANUS				2000
#define SPELL_ID_CORRUPTED_BLOOD		106835
#define SPELL_COOL_CORRUPTED_BLOOD		2000
#define SPELL_ID_ALEXTRASZA_PRESENCE	106028
#define SPELL_ID_CAUTERIZE				109045
#define SPELL_COOL_CAUTERIZE			1000
#define SPELL_ID_NOZDORMU_PRESENCE		106027
#define SPELL_ID_TIME_ZONE				1
#define SPELL_ID_YSERA_PRESENCE			106457
#define SPELL_ID_ENTER_THE_DREAM		106464 //cancel dream to enter dreamer effect
#define SPELL_ID_ENTER_THE_DREAM_EFFECT	106466
#define SPELL_ID_KALECGOS_PRESENCE		106029
#define SPELL_ID_SPELLWEAVING			106040
#define SPELL_ID_CARRYING_WINDS			106664
#define NPC_ID_ASPECT_ALEXSTRASZA		56099
#define NPC_ID_ASPECT_NOZDORMU			56102
#define NPC_ID_ASPECT_KALECGOS			56101
#define NPC_ID_ASPECT_YSERA				56100
#define NPC_ID_ASPECT_THRALL			56103
#define LIMIT_SPAWN_COUNT_OF_SAME_TYPE	20

#ifdef _DEBUG
/*
	#define TEMP_DISABLE_TENTACLE_SAPWNS
	#define TEMP_DISABLE_BLOOD_SAPWNS
	#define TEMP_DISABLE_PHASE1
	#define TEMP_DISABLE_REQ_HEROIC
	*/
#endif

#define MADNESSPLATFORM_COUNT	4

#define MadnessZdifference -105.0f

const float MadnessSpawnLocation[4] = { -11903.9f, 11989.0f, -113.1f, 2.1f };
const float MadnessAttackSpawnLocation[4] = { -12063.5f, 12198.9f, -12.9f, 2.16f };
const float MadnessPhaseLocation[4] = { -12137.9f, 12079.0f, -113.1f, 2.1f };
const float CorruptionLocations[MADNESSPLATFORM_COUNT][3] = { { -11929.0f, 12297.0f, -2.0f },{ -12028.0f, 12265.5f, -6.1f },{ -12107.4f, 12201.9f, -5.2f },{ -12160.9f, 12057.0f, 2.5f} };
const float PlatformCenterLocations[MADNESSPLATFORM_COUNT][3] = { { -11956.0f, 12270.0f, 1.5f },{ -12035.6f, 12221.6f, -6.1f },{ -12097.4f, 12161.0f, -2.7f },{ -12122.4f, 12074.2f, 2.3f } };
const float LimbLocations[MADNESSPLATFORM_COUNT][5] = { { -11941.0f, 12248.9f, 12.2f, 1.98f, NPC_ID_LIMB0 },{ -12005.8f, 12190.2f, -6.4f, 2.12f, NPC_ID_LIMB1 },{ -12065.0f, 12127.2f, -3.1f, 2.33f, NPC_ID_LIMB2 },{ -12097.0f, 12067.0f, 13.5f, 2.21f, NPC_ID_LIMB3 } };
const float LimbTentacleLocations[MADNESSPLATFORM_COUNT][3] = { { -11941.0f, 12248.9f, 12.2f },{ -12025.8f, 12208.2f, -6.4f },{ -12073.0f, 12145.2f, -3.1f },{ -12097.0f, 12067.0f, 13.5f } };
const uint32 DespawnOldEncounterDynamicEntries[] = { NPC_ID_MADNESS_VISUAL, NPC_ID_MADNESS_ATTACKTARGET, NPC_ID_ELEMENTIUM_BOLT, NPC_ID_CORRUPTING_PARASITE, NPC_ID_HEMORRHAGE_VISUAL_LOC, NPC_ID_REGENERATIVE_BLOOD, NPC_ID_BLISTERING_TENTACLE, NPC_ID_LIMB0, NPC_ID_LIMB1, NPC_ID_LIMB2, NPC_ID_LIMB3, NPC_ID_MUTATED_CORRUPTION, NPC_ID_CONGEALING_BLOOD, NPC_ID_ELEMENTIUM_FRAGMENT, NPC_ID_ELEMNTIUM_TERROR, 0 };	//despawn these in case encounter resets
const float ElementiumBoltStart[MADNESSPLATFORM_COUNT][3] ={ { -11961.0f, 12228.0f, 21.0f }, { -12015.0f, 12143.0f, 21.0f }, { -12015.0f, 12143.0f, 21.0f }, { -12071.0f, 12087.0f, 21.0f } };

class DeathwingMadnessController : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(DeathwingMadnessController);
/* 
	http://www.youtube.com/watch?v=qxScIr3UlTg&feature=player_detailpage#t=278s
    Stage One: The Final Assault
    Deathwing assaults his enemies while attached to the platforms by his twisted limbs.
        Assault Aspect – Deathwing searches for his next enemy to assault. Deathwing assaults the platform with the largest number of players and an attached limb.
        Cataclysm – Deathwing attempts to finish the job he started by bringing forth a second Cataclysm, inflicting 1462500 to 1537500 Fire damage to all players.
        Elementium Bolt – Deathwing creates an Elementium Bolt, sending it to the target platform.
            Elementium Blast – If the Elementium Bolt reaches its destination, the bolt inflicts 380250 to 399750 Fire damage to all players and an additional 380250 to 399750 Fire damage every 5 sec until players destroy the Elementium Bolt. The initial damage decreases the further the player stands from the targeted location.
        Corrupting Parasite – A Corrupting Parasite inflicts increasing Shadow damage periodically. When Corrupting Parasite's duration ends, the parasite detaches from the player and causes a Parasitic Backlash. This inflicts 243750 to 256250 Fire damage to players within 10 yards of the host.
            Corrupting Parasite – After the Corrupting Parasite's duration ends, the parasite detaches from the host.
                Unstable Corruption – The Corrupting Parasite casts Unstable Corruption, inflicting 10% of the Corrupted Parasite's health as Fire damage to all players.
        Hemorrhage – Deathwing's tentacle begins to Hemorrhage, creating several Regenerative Blood at a nearby location.
            Regenerative Blood – The Regenerative Blood of Deathwing forms. Regenerative Bloods gain ten energy every 1 sec. The melee attacks of a Regenerative Blood also cause Degenerative Bite.
                Regenerative – A Regenerative Blood heals to full health when it reaches maximum energy.
                In Looking for Raid Difficulty, Regenerative Blood heals 50% of max health when it reaches maximum energy.
            Degenerative Bite – Melee attacks cause Degenerative Bite, inflicting 1462 to 1537 Shadow damage every 1 sec for 10 sec. This effect stacks.
        Limb Tentacle – Deathwing grasps onto each platform with one of his limbs.
            Burning Blood – The Burning Blood gushes from the Tentacle, inflicting Fire damage every 2 sec. The damage increases as the Limb Tentacle's health lowers.
            Blistering Tentacle – At 75%, 50%, and 25% remaining health the Limb Tentacle sprouts several Blistering Tentacles that are immune to Area of Effect abilities.
                Blistering Heat – The Blistering Heat inflicts 2500 Fire damage every 2 sec, and increases its damage by 5% for 3 sec. This effect stacks.
            Agonizing Pain – The pain from severing Deathwing's limb interrupts his concentration, stunning him and inflicting 20% of his health in damage.
        Mutated Corruption – A Mutated Corruption appears shortly after assaulting a platform.
            Crush – The crushing weight of the Mutated Corruption inflicts 100000 Physical damage to enemies in a cone in front of the Corruption.
            Impale – The Mutated Corruption impales its target, inflicting 400000 Physical damage to the target.

    Stage Two: The Last Stand
    At 20% remaining health, Deathwing falls forward onto the platform.
        Congealing Blood – Deathwing begins to Hemorrhage, creating several Congealing Blood at a nearby location. The Blood then moves towards Deathwing.
            Congealing Blood – If a Congealing Blood reaches Deathwing, it heals him for 1% of his maximum health.
        Elementium Fragment – Pieces of Deathwing's armor chip off, forming three Elementium Fragments nearby.
        In 25 player raids, eight Elementium Fragments form.
            Shrapnel – The Fragment fires a piece of Shrapnel at a random player, inflicting 195000 to 205000 unresistable Physical damage.
        Elementium Terror – Pieces of Deathwing's armor chip off, forming two Elementium Terrors nearby.
            Tetanus – The Elementium Terror's melee attacks cause Tetanus, inflicting 58500 to 61500 Physical damage and an additional 19500 to 20500 Physical damage every 1 sec. The effect stacks.
        Corrupted Blood – Corrupted Blood gushes from Deathwing, inflicting Fire damage every 2 sec. The damage increases as Deathwing's health lowers. At 15%, 10%, and 5% remaining health, Deathwing hemorrages. This increases the damage dealt by Corrupted Blood.

    The Aspects
    The Dragon Aspects assist in the fight against Deathwing. Each aspect channels Expose Weakness on their respective Limb Tentacle when Deathwing begins to cast Cataclysm. Expose Weakness increases the damage done to the Tentacle by 100%. Each Aspect also channels Concentration after the players destroy their respective tentacle, preventing the Aspect from assisting the players with their presence and their special powers. The Aspects resume assisting the players when Stage Two begins.
        Alexstrasza 
            Alexstrasza's Presence – The Presence of Alexstrasza increases the player's maximum health by 20%.
            Cauterize – Alexstrasza Cauterizes the Blistering Tentacles, inflicting lethal damage over 5 sec. Cauterize also reduces the damage inflicted by Corrupted Blood.
            In Heroic Difficulty Alexstrasza Cauterizes the Corrupting Parasite, inflicting 3% Fire damage every 0.5 sec.
        Nozdormu 
            Nozdormu's Presence – The Presence of Nozdormu increases the player's haste by 20%.
            Time Zone – Nozdormu forms a Time Zone at the target location, causing the Elementium Meteor to lapse in time and decrease its travel speed dramatically upon entering the zone. Any enemy creatures within the Time Zone have their attack speed decreased by 50%.
            In Heroic Difficulty the Time Zone slows the effects of Corrupting Parasite.
        Ysera 
            Ysera's Presence – The Presence of Ysera increases player healing done by 20%.
            Enter the Dream – The Presence of Ysera allows players to enter the Dream, decreasing damage taken by 50% for 2 sec.
        Kalecgos 
            Kalecgos' Presence – The Presence of Kalecgos increases player damage dealt by 20%.
            Spellweaving – The Presence of Kalecgos allows attacks and abilities to cause Spellweaving. Spellweaving inflicts 22425 to 23575 Arcane damage to enemies within 6 yards, excluding the current target.
        Thrall 
            Carrying Winds – The Carrying Winds take players between adjacent platforms, and increase their movement speed by 60% for 10 sec. This effect stacks.   
*/
	uint32	YellIndex;
	uint32	TalkCooldown;
	uint32	phase;
	float	DifficultyScale;
	uint32	AssaultAspectTimer;
	uint32	CataclysmTimer;
	uint64	LimbGUIDs[MADNESSPLATFORM_COUNT];
	uint32	FaintTimer;
    
	DeathwingMadnessController(Creature* pCreature) : CreatureAIScript(pCreature)
	{
        phase = 0;
	}
    
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		//make him attack faster as difficulty increases
//		uint32 attacktime_now = _unit->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME );
//		_unit->SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, MAX( 500, attacktime_now * DifficultyScale ) );	//attack a bit faster cause there are more healers

		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
		_unit->GetAIInterface()->setOutOfCombatRange( 700.0f * 700.0f );
//		_unit->bInvincible = true;
//		_unit->static_object = true;	//do not deactivate. Need to monitor the fight all the time

//		int64 *MadnessGUID = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_GUID_VISUAL );
//		*MadnessGUID = _unit->GetGUID();

		RegisterAIUpdateEvent( 1000 );

		ConvertToStaticSpawn( _unit, true );
	}

	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
    }

	void AIUpdate()
    {
		if( _unit->isAlive() == false )
			return;
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		if( *EncounterStatus & EXTENSION_FLAG_MADNESS_DIED )
			return;
//		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		InrangeLoopExitAutoCallback AutoLock;
		if( _unit->GetInRangePlayerSetBegin( AutoLock ) == _unit->GetInRangePlayerSetEnd() )
			return;
		Unit *tank = *_unit->GetInRangePlayerSetBegin( AutoLock );
		if( tank == NULL )
		{
			phase = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		// Init phase
		Creature *Madness;
		if( phase == 0 )
		{   
			TalkCooldown = TickNow + RandomUInt() % 25000;
			YellIndex = 0;
            phase = 1;
            AssaultAspectTimer = TickNow + RandomUInt() % 25000;
            CataclysmTimer = TickNow + RandomUInt() % 25000;
//            HemorrhageTimer = TickNow + RandomUInt() % 25000;
			//activate nearby mapmanager cells
			float distX = _unit->GetPositionX() - MadnessSpawnLocation[0];
			float distY = _unit->GetPositionY() - MadnessSpawnLocation[1];
			int32 dist = sqrt( distX * distX + distY * distY );
			dist += 50;
			dist = dist / _cellSize;
			uint32 x = _unit->GetMapMgr()->GetPosX(_unit->GetPositionX());
			uint32 y = _unit->GetMapMgr()->GetPosY(_unit->GetPositionY());
			_unit->GetMapMgr()->UpdateCellActivity( x, y, dist, 2 );
			//despawn all dynamic spawns
			DespawnEncounterSpawns( _unit, DespawnOldEncounterDynamicEntries );
			//spawn madness
			{
				Madness = _unit->GetMapMgr()->CreateCreature( NPC_ID_MADNESS_VISUAL, true, MadnessSpawnLocation[0], MadnessSpawnLocation[1], MadnessSpawnLocation[2], MadnessSpawnLocation[3] );
				ConvertToStaticSpawn( Madness, true );
			}
			Madness->CastSpell( Madness, SPELL_ID_CATACLYSM, false );
			{
				Creature *c;
				c = _unit->GetMapMgr()->GetCreatureByEntry( NPC_ID_ASPECT_ALEXSTRASZA );
				ConvertToStaticSpawn( c, true );
				c = _unit->GetMapMgr()->GetCreatureByEntry( NPC_ID_ASPECT_NOZDORMU );
				ConvertToStaticSpawn( c, true );
				c = _unit->GetMapMgr()->GetCreatureByEntry( NPC_ID_ASPECT_KALECGOS );
				ConvertToStaticSpawn( c, true );
				c = _unit->GetMapMgr()->GetCreatureByEntry( NPC_ID_ASPECT_YSERA );
				ConvertToStaticSpawn( c, true );
			}
			//spawn new limbs
#ifndef TEMP_DISABLE_PHASE1
			for( int i=0;i<MADNESSPLATFORM_COUNT;i++)
			{
				uint32 LimbEntry = LimbLocations[i][4];
				Creature *c = _unit->GetMapMgr()->CreateCreature( LimbEntry, true, LimbLocations[i][0], LimbLocations[i][1], LimbLocations[i][2], LimbLocations[i][3] );
				if( c )
				{
					LimbGUIDs[i] = c->GetGUID();
					int64 *LimbIndex = c->GetCreateIn64Extension( NPC_ID_LIMB0 );
					*LimbIndex = i; //self awarness for inter object comunication if required
					int64 *CanActivePlatformIndex = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_ACTIVE_PLATFORMS + *LimbIndex );
					*CanActivePlatformIndex = 1;
				}
//				c = _unit->GetMapMgr()->CreateCreature( NPC_ID_HEMORRHAGE_VISUAL_LOC, true, LimbTentacleLocations[i][0], LimbTentacleLocations[i][1], LimbTentacleLocations[i][2] );
//				if( c )
//				{
//					int64 *HemoTargetNPCGUID = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_HEMORRHAGE_VISUAL_LOC + i );
//					*HemoTargetNPCGUID = c->GetGUID();
//				}
			}
#endif
//			Madness->SetPosition( MadnessSpawnLocation[0], MadnessSpawnLocation[1], MadnessSpawnLocation[2], _unit->GetOrientation() );
//			Madness->GetAIInterface()->StopMovement( 0 );
		}

		int64 *MadnessGUID = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_GUID_VISUAL );
		Madness = _unit->GetMapMgr()->GetCreature( *MadnessGUID );
		if( Madness == NULL )
			return;

        if (phase == 1)
        {
			if( TalkCooldown < TickNow )
			{
				Madness->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, DeathwingMadnessYells[ YellIndex ] );
				YellIndex = ( YellIndex + 1 ) % 15;
				TalkCooldown = TickNow + (20000 + RandomUInt() % 10000);
			}
			//count platform players
/*			uint32 PlatformPlayerCount[MADNESSPLATFORM_COUNT];
			memset( PlatformPlayerCount, 0, sizeof( PlatformPlayerCount ) );
			float PlatformRadius = 40.0f * 40.0f;
			InRangePlayerSet::iterator itr3;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr3 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr3 != _unit->GetInRangePlayerSetEnd(); itr3++)
			{
				if( (*itr3)->isAlive() == false )
					continue;
				float BestPlatformDist = 99999999999999.0f;
				uint32 BestPlatformIndex = 0;
				for( uint32 i=0;i<MADNESSPLATFORM_COUNT;i++)
				{
					float dist = Distance2DSq( PlatformCenterLocations[ i ][0], PlatformCenterLocations[ i ][1], (*itr3)->GetPositionX(), (*itr3)->GetPositionY() );
					if( dist < BestPlatformDist )
					{
						BestPlatformDist = dist;
						BestPlatformIndex = i;
					}
				}
				PlatformPlayerCount[ BestPlatformIndex ]++;
			}
			_unit->ReleaseInrangeLock();
			//get which platform has most players on it
			uint32 BestPlayerCount = 0;
			uint32 BestPlayerCountIndex = 0;
			for( uint32 i=0;i<MADNESSPLATFORM_COUNT;i++)
			{
				int64 *CanActivePlatformIndex = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_ACTIVE_PLATFORMS + i );
				if( *CanActivePlatformIndex == 1 && PlatformPlayerCount[i] > BestPlayerCount )
				{
					BestPlayerCount = PlatformPlayerCount[i];
					BestPlayerCountIndex = i;
				}
			}
			//set as active platform
			int64 *AcctivePlatformIndex = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_ACTIVE_PLATFORM );
			*AcctivePlatformIndex = BestPlayerCountIndex;
			//get the tank of this platform
			Unit *PlatformCorruption = _unit->GetMapMgr()->GetCreature( LimbGUIDs[ BestPlayerCountIndex ] );
			Unit *PlatformTank = NULL;
			if( PlatformCorruption )
				PlatformTank = PlatformCorruption->GetAIInterface()->GetMostHated(); 

			if( PlatformTank )
			{
				if( AssaultAspectTimer < TickNow )
				{
					Madness->CastSpell( PlatformTank, SPELL_ID_ASSAULT_ASPECT, false );
					AssaultAspectTimer = TickNow + (SPELL_COOL_ASSAULT_ASPECT + RandomUInt() % SPELL_COOL_ASSAULT_ASPECT)*DifficultyScale;
				}
				if( CataclysmTimer < TickNow )
				{
					Madness->CastSpell( PlatformTank, SPELL_ID_CATACLYSM, false );
					CataclysmTimer = TickNow + (SPELL_COOL_CATACLYSM + RandomUInt() % SPELL_COOL_CATACLYSM)*DifficultyScale;
				}
			} */
			//time to enter next phase ?
			bool LimbsLeft = false;
			char LimbKilled = false;
			for( uint32 i=0;i<MADNESSPLATFORM_COUNT;i++)
			{
				int64 *CanActivePlatformIndex = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_ACTIVE_PLATFORMS + i );
				if( *CanActivePlatformIndex )
					LimbsLeft = true;
				else
					LimbKilled++;
			}
			if( Madness->GetHealthPct() <= 100 + 5 - MADNESS_LOOSE_HP_PCT_TENTACLE * MADNESSPLATFORM_COUNT || LimbsLeft == false 
//				|| LimbKilled != 0
				)
			{
				phase = 2;
				//move head between platform 2-3
				Madness->SetUInt32Value( UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_CUSTOMSPELL01 );
				FaintTimer = TickNow + 5000;
//				Madness->bInvincible = false;
				//EMOTE_ONESHOT_CUSTOMSPELL05 ->sparkling explode
				//spawn madness attack target
				{
					Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_MADNESS_ATTACKTARGET, true, MadnessAttackSpawnLocation[0], MadnessAttackSpawnLocation[1], MadnessAttackSpawnLocation[2], MadnessAttackSpawnLocation[3] );
					ConvertToStaticSpawn( c, true );
//					Madness->bInvincible = true;
					Madness->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_ATTACKABLE_9 );
				}
{
char buff[500];
sprintf( buff, "Debug MSG : You should go to the platform where taxi ported you" );
Madness->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, buff );
}
			}
        }
        else if (phase == 2)
        {
			//keep head forward
			if( FaintTimer < TickNow )
			{
				Madness->SetUInt32Value( UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_CUSTOMSPELL02 );
				for( uint32 i=0;i<MADNESSPLATFORM_COUNT;i++)
				{
					int64 *CanActivePlatformIndex = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_ACTIVE_PLATFORMS + i );
					*CanActivePlatformIndex = 1;
				}
			}
        }
	}
};

class DeathwingMadnessVisual : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DeathwingMadnessVisual);

    DeathwingMadnessVisual(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		_unit->GetAIInterface()->setOutOfCombatRange( 700.0f * 700.0f );
		_unit->m_canRegenerateHP = false;
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
		_unit->bInvincible = true;
	}
	void OnDied(Unit *mKiller)
	{
	}
};  

class DeathwingMadnessAttackTarget : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DeathwingMadnessAttackTarget);
    DeathwingMadnessAttackTarget(Creature* pCreature) : CreatureAIScript(pCreature)    {    }

	uint32	CongealingBloodTimer,CongealingBloodCounter;
	uint32	ElementiumFragmentTimer,ElementiumFragmentCounter;
	uint32	ElementiumTerrorTimer,ElementiumTerrorCounter;
	uint32	CorruptedBloodTimer;
	uint32	phase;
	float	DifficultyScale;

	void OnLoad()
	{
		_unit->GetAIInterface()->setOutOfCombatRange( 700.0f * 700.0f );
		ConvertToStaticSpawn( _unit, true );
		_unit->m_canRegenerateHP = false;
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
		//copy health from visual target
		int64 *MadnessGUID = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_GUID_VISUAL );
		Creature *Madness = _unit->GetMapMgr()->GetCreature( *MadnessGUID );
		if( Madness )
		{
			_unit->SetMaxHealth( Madness->GetMaxHealth() );
			_unit->SetHealth( Madness->GetHealth() );
		}
		RegisterAIUpdateEvent( 2000 );
		phase = 0;
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
	}
    void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			phase = 0;
			return;
		}
		uint32 TickNow = GetTickCount();
		if( phase == 0 )
		{
            phase = 1;
            CongealingBloodTimer = TickNow + 5000 + RandomUInt() % 25000;
			CongealingBloodCounter = 0;
            ElementiumFragmentTimer = TickNow + 10000 + RandomUInt() % 25000;
			ElementiumFragmentCounter = 0;
            ElementiumTerrorTimer = TickNow + 15000 + RandomUInt() % 25000;
			ElementiumTerrorCounter = 0;
            CorruptedBloodTimer = TickNow + 20000 + RandomUInt() % 25000;
		}
		if( phase == 1 )
		{
			//this happens when raid wipes :( Health resets to full. Maybe later reset the whole encounter ?
			if( _unit->GetHealthPct() > ( 100 + - MADNESS_LOOSE_HP_PCT_TENTACLE * MADNESSPLATFORM_COUNT ) )
			{
				_unit->SetHealth( _unit->GetMaxHealth() / 100 * ( 100 + - MADNESS_LOOSE_HP_PCT_TENTACLE * MADNESSPLATFORM_COUNT ) );
			}
			uint32 PlatformPlayerCount[MADNESSPLATFORM_COUNT];
			memset( PlatformPlayerCount, 0, sizeof( PlatformPlayerCount ) );
			float PlatformRadius = 40.0f * 40.0f;
			InRangePlayerSetRecProt::iterator itr3;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			for( itr3 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr3 != _unit->GetInRangePlayerSetEnd(); itr3++)
			{
				if( (*itr3)->isAlive() == false )
					continue;
				float BestPlatformDist = 99999999999999.0f;
				uint32 BestPlatformIndex = 0;
				for( uint32 i=0;i<MADNESSPLATFORM_COUNT;i++)
				{
					float dist = Distance2DSq( PlatformCenterLocations[ i ][0], PlatformCenterLocations[ i ][1], (*itr3)->GetPositionX(), (*itr3)->GetPositionY() );
					if( dist < BestPlatformDist )
					{
						BestPlatformDist = dist;
						BestPlatformIndex = i;
					}
				}
				PlatformPlayerCount[ BestPlatformIndex ]++;
			}
			_unit->ReleaseInrangeLock();
			//get which platform has most players on it
			uint32 BestPlayerCount = 0;
			uint32 BestPlayerCountIndex = 0;
			for( uint32 i=0;i<MADNESSPLATFORM_COUNT;i++)
			{
				if( PlatformPlayerCount[i] > BestPlayerCount )
				{
					BestPlayerCount = PlatformPlayerCount[i];
					BestPlayerCountIndex = i;
				}
			}
			//set as active platform
			uint32 SelectedPlatform = BestPlayerCountIndex;

			if( CongealingBloodTimer < TickNow 
#ifndef TEMP_DISABLE_REQ_HEROIC
				&& IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty )
#endif
				)
			{
				int64 *ActiveSpawnCount = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_CONGEALING_BLOOD );
				if( *ActiveSpawnCount < LIMIT_SPAWN_COUNT_OF_SAME_TYPE )
				{
//					uint32 BloodsToCreateCount = float2int32( 5 / DifficultyScale );
					uint32 BloodsToCreateCount = float2int32( 3 / DifficultyScale );
					for(uint32 i=0;i<BloodsToCreateCount;i++)
					{
						float RandX = PlatformCenterLocations[ SelectedPlatform ][0] + 5.0f - ( RandomUInt() % 100 ) / 10.0f;
						float RandY = PlatformCenterLocations[ SelectedPlatform ][1] + 5.0f - ( RandomUInt() % 100 ) / 10.0f;
						Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_CONGEALING_BLOOD, true, RandX, RandY, PlatformCenterLocations[ SelectedPlatform ][2] );
						*ActiveSpawnCount += 1;
					}
					CongealingBloodTimer = TickNow + (NPC_COOL_CONGEALING_BLOOD + RandomUInt() % NPC_COOL_CONGEALING_BLOOD)*DifficultyScale;
					CongealingBloodCounter++;
				}
			}
			else if( ShouldForceProc( _unit, CongealingBloodCounter, NPC_MINPROC_CONGEALING_BLOOD, MADNESS_LOOSE_HP_PCT_TENTACLE * MADNESSPLATFORM_COUNT ) )
				CongealingBloodTimer = 1;

			if( ElementiumFragmentTimer < TickNow )
			{
				int64 *ActiveSpawnCount = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_ELEMENTIUM_FRAGMENT );
				if( *ActiveSpawnCount < LIMIT_SPAWN_COUNT_OF_SAME_TYPE )
				{
					float RandX = PlatformCenterLocations[ SelectedPlatform ][0] + 15.0f - ( RandomUInt() % 300 ) / 10.0f;
					float RandY = PlatformCenterLocations[ SelectedPlatform ][1] + 15.0f - ( RandomUInt() % 300 ) / 10.0f;
					Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_ELEMENTIUM_FRAGMENT, true, RandX, RandY, PlatformCenterLocations[ SelectedPlatform ][2] );
					*ActiveSpawnCount += 1;
					ElementiumFragmentTimer = TickNow + (NPC_COOL_ELEMENTIUM_FRAGMENT + RandomUInt() % NPC_COOL_ELEMENTIUM_FRAGMENT)*DifficultyScale;

					ElementiumFragmentCounter++;
				}
			}
			else if( ShouldForceProc( _unit, ElementiumFragmentCounter, NPC_MINPROC_ELEMENTIUM_FRAGMENT, MADNESS_LOOSE_HP_PCT_TENTACLE * MADNESSPLATFORM_COUNT ) )
				ElementiumFragmentTimer = 1;

			if( ElementiumTerrorTimer < TickNow )
			{
				int64 *ActiveSpawnCount = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_ELEMNTIUM_TERROR );
				if( *ActiveSpawnCount < LIMIT_SPAWN_COUNT_OF_SAME_TYPE )
				{
					float RandX = PlatformCenterLocations[ SelectedPlatform ][0] + 15.0f - ( RandomUInt() % 300 ) / 10.0f;
					float RandY = PlatformCenterLocations[ SelectedPlatform ][1] + 15.0f - ( RandomUInt() % 300 ) / 10.0f;
					Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_ELEMNTIUM_TERROR, true, RandX, RandY, PlatformCenterLocations[ SelectedPlatform ][2] );
					*ActiveSpawnCount += 1;
					ElementiumTerrorTimer = TickNow + (NPC_COOL_ELEMNTIUM_TERROR + RandomUInt() % NPC_COOL_ELEMNTIUM_TERROR)*DifficultyScale;
					ElementiumTerrorCounter++;
				}
			}
			else if( ShouldForceProc( _unit, ElementiumTerrorCounter, NPC_MINPROC_ELEMENTIUM_TERROR, MADNESS_LOOSE_HP_PCT_TENTACLE * MADNESSPLATFORM_COUNT ) )
				ElementiumTerrorTimer = 1;
			if( CorruptedBloodTimer < TickNow )
			{
				//each lost health pct adds 100% dmg. Should scale up to 35000 DPS ( base is 350 )
				int32 BurningBloodDMGPCT = 100 * ( ( 101 + - MADNESS_LOOSE_HP_PCT_TENTACLE * MADNESSPLATFORM_COUNT ) - _unit->GetHealthPct() * DifficultyScale );
				InRangePlayerSetRecProt::iterator itr3;
				_unit->AquireInrangeLock(); //make sure to release lock before exit function !
				InrangeLoopExitAutoCallback AutoLock;
				for( itr3 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr3 != _unit->GetInRangePlayerSetEnd(); itr3++)
				{
					if( (*itr3)->isAlive() == false )
						continue;

					SpellEntry *spellInfo = dbcSpell.LookupEntryForced( SPELL_ID_CORRUPTED_BLOOD );
					Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
					spell->Init( _unit, spellInfo ,true, NULL);
					SpellCastTargets targets( (*itr3)->GetGUID() );
					spell->forced_pct_mod[0] = BurningBloodDMGPCT;
					spell->prepare(&targets);
				}
				_unit->ReleaseInrangeLock();
				CorruptedBloodTimer = TickNow + SPELL_COOL_CORRUPTED_BLOOD; //needs to be fixed or it will not stack
			}
		}
	}
	void OnDied(Unit *mKiller)
	{
		int64 *EncounterStatus = _unit->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		*EncounterStatus |= EXTENSION_FLAG_MADNESS_DIED;
//		_unit->GetMapMgr()->CreateCreature( MOB_ENTRY_DRAGONSOUL_TELEPORTER, true, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
		RemoveAIUpdateEvent();
		//try to see players lie about BOSS difficulty
		LOG_CREATURE_DEATH( _unit->GetEntry(), mKiller );
		int64 *MadnessGUID = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_GUID_VISUAL );
		Creature *Madness = _unit->GetMapMgr()->GetCreature( *MadnessGUID );
		if( Madness )
		{
			Madness->bInvincible = false;
			Madness->SetHealth( 1 );
			Madness->DealDamage( Madness, 100, NULL );
		}
		//spawn GO that players can loot
/*		GameObject *go = u_caster->GetMapMgr()->CreateGameObject(210220);
		if( go == NULL )
		{
			sLog.outDebug("Missing GO proto 210220");
			return;
		}
		go->CreateFromProto( 210220, _unit->GetMapId(), -12100.5f, 12173.6f, -2.73425f, 5.09636f );
		go->SetUInt64Value( OBJECT_FIELD_CREATED_BY, m_caster->GetGUID() );
		go->SetInstanceID( _unit->GetInstanceID() );
		go->PushToWorld( _unit->GetMapMgr() );
		sEventMgr.AddEvent(go, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, 5*60*1000, 1,0);
		*/
		_unit->SetUInt32Value( UNIT_FIELD_DISPLAYID, 35268 );
		_unit->SetScale( 0.5f );
		if( mKiller && mKiller->IsPlayer() && SafePlayerCast( mKiller )->GetGuild() )
			SafePlayerCast( mKiller )->GetGuild()->GuildNewsAdd( GUILD_NEWS_LOG_DUNGEON_ENCOUNTER, mKiller->GetGUID(), 1299 );
    }
};  

const uint32 DespawnOldLimbEncounterDynamicEntries[] = { NPC_ID_ELEMENTIUM_BOLT, NPC_ID_CORRUPTING_PARASITE, NPC_ID_HEMORRHAGE_VISUAL_LOC, NPC_ID_REGENERATIVE_BLOOD, NPC_ID_BLISTERING_TENTACLE, NPC_ID_MUTATED_CORRUPTION, 0 };	//despawn these in case encounter resets

class LimbGeneric : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LimbGeneric);

	uint32	phase;
	float	DifficultyScale;
	uint32	RegenerativeBloodTimer,RegenerativeBloodCounter;
	uint32	CorruptionTimer,CorruptionCounter;
	uint32	LimbTentacleTimer,LimbTentacleCounter;
	uint32	BurningBloodTimer;
	uint32	AlexStraszaCauterizeTimer;
	uint32	ElementiumBoltTimer,ElementiumBoltCounter;
	uint32	CorruptingParasiteTimer,CorruptingParasiteCounter;

    LimbGeneric(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;

		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
		_unit->GetAIInterface()->setOutOfCombatRange( 700.0f * 700.0f ); //once we start combat we should try to not reset

		RegisterAIUpdateEvent( 1000 );
		phase = 0;

		//self root to not face the tank as we gain agro
//		_unit->CastSpell( _unit, 105451, true );
	}
    void AIUpdate()
    {
		//wait for some DPS
		if( _unit->GetHealthPct() > TENTACLE_START_FIGHT_HP_PCT )
		{
			return;
		}
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		//try to get an alive player near us if we did not agro any
		if( tank == NULL )
		{
			InRangePlayerSetRecProt::iterator itr3;
			_unit->AquireInrangeLock(); //make sure to release lock before exit function !
			InrangeLoopExitAutoCallback AutoLock;
			float Radius = 70.0f * 70.0f;
			for( itr3 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr3 != _unit->GetInRangePlayerSetEnd(); itr3++)
			{
				if( (*itr3)->isAlive() == false )
					continue;
				if( Distance2DSq( (*itr3)->GetPositionX(), (*itr3)->GetPositionY(), _unit->GetPositionX(), _unit->GetPositionY() ) > Radius )
					continue;
				tank = (*itr3);
				break;
			}
			_unit->ReleaseInrangeLock();
		}
		if( tank == NULL )
		{
			//stop spamming the server and let people port to us again
			if( phase != 0 )
			{
				DespawnEncounterSpawns( _unit, DespawnOldLimbEncounterDynamicEntries );
				_unit->SetHealth( _unit->GetMaxHealth() );
			}
			phase = 0;
			return;
		}
		uint32 TickNow = GetTickCount();

		// Init phase
		if( phase == 0 )
		{   
            phase = 1;
			CorruptionTimer = TickNow + 15000 + RandomUInt() % 25000;
			CorruptionCounter = 0;
            LimbTentacleTimer = TickNow + 25000 * DifficultyScale + RandomUInt() % 25000;
			LimbTentacleCounter = 0;
            RegenerativeBloodTimer = TickNow + 35000 * DifficultyScale + RandomUInt() % 25000;
			RegenerativeBloodCounter = 0;
			BurningBloodTimer = TickNow + 35000 * DifficultyScale + RandomUInt() % 25000;
			AlexStraszaCauterizeTimer = TickNow;
			ElementiumBoltTimer = TickNow + 5000 * DifficultyScale + RandomUInt() % 25000;
			ElementiumBoltCounter = 0;
			CorruptingParasiteTimer = TickNow + 10000 * DifficultyScale + RandomUInt() % 25000;
			CorruptingParasiteCounter = 0;
			_unit->SetHealth( _unit->GetMaxHealth() );
		}
		if( phase == 1 )
		{
			//only spawn on active platform
			int64 *LimbIndex = _unit->GetCreateIn64Extension( NPC_ID_LIMB0 );
//			int64 *AcctivePlatformIndex = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_ACTIVE_PLATFORM );
//			if( *LimbIndex != *AcctivePlatformIndex )
			//////////////////////////////////////////////////
			//AOE dps
			if( BurningBloodTimer < TickNow )
			{
				//each lost health pct adds 100% dmg. Should scale up to 35000 DPS ( base is 350 )
				int32 BurningBloodDMGPCT = 100 * ( 101 - _unit->GetHealthPct() * DifficultyScale );
				float Radius = 70.0f * 70.0f;
				InRangePlayerSetRecProt::iterator itr3;
				_unit->AquireInrangeLock(); //make sure to release lock before exit function !
				InrangeLoopExitAutoCallback AutoLock;
				for( itr3 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr3 != _unit->GetInRangePlayerSetEnd(); itr3++)
				{
					if( (*itr3)->isAlive() == false )
						continue;

					if( Distance2DSq( (*itr3)->GetPositionX(), (*itr3)->GetPositionY(), _unit->GetPositionX(), _unit->GetPositionY() ) > Radius )
						continue;

					SpellEntry *spellInfo = dbcSpell.LookupEntryForced( SPELL_ID_BURNING_BLOOD );
					Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
					spell->Init( _unit, spellInfo ,true, NULL);
					SpellCastTargets targets( (*itr3)->GetGUID() );
					spell->forced_pct_mod[0] = BurningBloodDMGPCT;
					spell->prepare(&targets);
				}
				_unit->ReleaseInrangeLock();
				BurningBloodTimer = TickNow + SPELL_COOL_BURNING_BLOOD; //needs to be fixed or it will not stack
			}
	

			//////////////////////////////////////////////////
			if( CorruptionTimer!= 0 && CorruptionTimer < TickNow )
			{
				Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_MUTATED_CORRUPTION, true, CorruptionLocations[*LimbIndex][0], CorruptionLocations[*LimbIndex][1], CorruptionLocations[*LimbIndex][2] );
				if( c )
					c->GetAIInterface()->AttackReaction( tank, 10, 0 );
				CorruptionTimer = 0;
				CorruptionCounter++;
			}
			//protection against imba DPS. Will flood events also
			else if( ShouldForceProc( _unit, CorruptionCounter, NPC_MINPROC_MUTATED_CORRUPTION, TENTACLE_START_FIGHT_HP_PCT ) )
				CorruptionTimer = 1;

			//////////////////////////////////////////////////
			if( RegenerativeBloodTimer < TickNow )
			{
				int64 *ActiveBloodCount = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_REGENERATIVE_BLOOD );
				if( *ActiveBloodCount < LIMIT_SPAWN_COUNT_OF_SAME_TYPE )
				{
//					int64 *HemoTargetNPCGUID = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_HEMORRHAGE_VISUAL_LOC + *LimbIndex );
//					Creature *HemoTarget = _unit->GetMapMgr()->GetCreature( *HemoTargetNPCGUID );
//					if( HemoTarget ) 
//						HemoTarget->CastSpell( HemoTarget, SPELL_ID_HEMORRHAGE_VISUAL, true ); //we should have dedicated NPCs for these. They look like red circle on the ground
					//spawn a hemmorhage NPC that just marks the spawn location of the regenerative bloods
//					uint32 RegenerativeBloodSpawnCount = 6 / DifficultyScale; // [6,8]
					uint32 RegenerativeBloodSpawnCount = 4 / DifficultyScale; // [6,8]
					for( uint32 i=0;i<RegenerativeBloodSpawnCount;i++)
					{
						float RandX = 5.0f - ( RandomUInt() % 100 ) / 10.0f;
						float RandY = 5.0f - ( RandomUInt() % 100 ) / 10.0f;
#ifndef TEMP_DISABLE_BLOOD_SAPWNS
						Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_REGENERATIVE_BLOOD, true, LimbTentacleLocations[*LimbIndex][0] + RandX, LimbTentacleLocations[*LimbIndex][1] + RandY, PlatformCenterLocations[*LimbIndex][2] );
#endif
						*ActiveBloodCount += 1;
					}
					RegenerativeBloodCounter++;
/*{
char buff[500];
sprintf( buff, "Debug MSG : Number of bloods %d, casts %d", (int32)(*ActiveBloodCount), RegenerativeBloodCounter );
_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, buff );
}*/
				}
				RegenerativeBloodTimer = TickNow + (NPC_COOL_REGENERATIVE_BLOOD + RandomUInt() % NPC_COOL_REGENERATIVE_BLOOD)*DifficultyScale;
				//do not spawn everything at the same time pls. Extend our spawn timer
				LimbTentacleTimer = MAX( TickNow + MIN( 10000, NPC_COOL_REGENERATIVE_BLOOD / 2 ), LimbTentacleTimer );
			}
			//protection against imba DPS. Will flood events also
			else if( ShouldForceProc( _unit, RegenerativeBloodCounter, NPC_MINPROC_REGENERATIVE_BLOOD, TENTACLE_START_FIGHT_HP_PCT ) )
				RegenerativeBloodTimer = 1;

			//////////////////////////////////////////////////
			if( LimbTentacleTimer < TickNow ) //blistering tentactle
			{
				int64 *ActiveTentacleCount = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_BLISTERING_TENTACLE );
				if( *ActiveTentacleCount < LIMIT_SPAWN_COUNT_OF_SAME_TYPE )
				{
//					uint32 LimbTentacleSpawnCount = 7 / DifficultyScale; // [7,10]
					uint32 LimbTentacleSpawnCount = 5 / DifficultyScale; // [7,10]
					for( uint32 i=0;i<LimbTentacleSpawnCount;i++)
					{
						float RandX = 5.0f - ( RandomUInt() % 100 ) / 10.0f;
						float RandY = 5.0f - ( RandomUInt() % 100 ) / 10.0f;
#ifndef TEMP_DISABLE_TENTACLE_SAPWNS
						Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_BLISTERING_TENTACLE, true, LimbTentacleLocations[*LimbIndex][0] + RandX, LimbTentacleLocations[*LimbIndex][1] + RandY, PlatformCenterLocations[*LimbIndex][2] );
#endif
						*ActiveTentacleCount += 1;
					}
					LimbTentacleCounter++;
/*{
char buff[500];
sprintf( buff, "Debug MSG : Number of tentacles %d, casts %d", (int32)(*ActiveTentacleCount), LimbTentacleCounter );
_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, buff );
}*/
				}
				LimbTentacleTimer = TickNow + (NPC_COOL_LIMB_TENTACLE + RandomUInt() % NPC_COOL_LIMB_TENTACLE)*DifficultyScale;
				//do not spawn everything at the same time pls
				RegenerativeBloodTimer = MAX( TickNow + MIN( 10000, NPC_COOL_LIMB_TENTACLE / 2 ), RegenerativeBloodTimer );
			}
			//protection against imba DPS. Will flood events also
			else if( ShouldForceProc( _unit, LimbTentacleCounter, NPC_MINPROC_LIMB_TENTACLE, TENTACLE_START_FIGHT_HP_PCT ) )
				LimbTentacleTimer = 1;		

			if( ElementiumBoltTimer < TickNow )
			{
				int64 *ActiveSpawnCount = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_ELEMENTIUM_BOLT );
				if( *ActiveSpawnCount < LIMIT_SPAWN_COUNT_OF_SAME_TYPE )
				{
					Creature *c = _unit->GetMapMgr()->CreateCreature( NPC_ID_ELEMENTIUM_BOLT, true, ElementiumBoltStart[ *LimbIndex ][0], ElementiumBoltStart[ *LimbIndex ][1], ElementiumBoltStart[ *LimbIndex ][2] );
					if( c )
					{ 
						int64 *MyLimIndex = c->GetCreateIn64Extension( NPC_ID_ELEMENTIUM_BOLT );
						*MyLimIndex = *LimbIndex;
//						c->GetAIInterface()->MoveTo( PlatformCenterLocations[ *LimbIndex ][0], PlatformCenterLocations[ *LimbIndex ][1], PlatformCenterLocations[ *LimbIndex ][2], 0.0f );
					}
					ElementiumBoltTimer = TickNow + (NPC_COOL_ELEMENTIUM_BOLT + RandomUInt() % NPC_COOL_ELEMENTIUM_BOLT)*DifficultyScale;
					ElementiumBoltCounter++;
					*ActiveSpawnCount += 1;
				}
			}	
			else if( _unit->GetHealthPct() < 50 &&  ShouldForceProc( _unit, ElementiumBoltCounter, NPC_MINPROC_ELEMENTIUM_BOLT, TENTACLE_START_FIGHT_HP_PCT ) )
				ElementiumBoltTimer = 1;

			if( 
#ifndef TEMP_DISABLE_REQ_HEROIC
				IS_HEROIC_INSTANCE_DIFFICULTIE( _unit->GetMapMgr()->instance_difficulty ) && 
#endif
				CorruptingParasiteTimer < TickNow )
			{
				AI_Spell sp;
				sp.spelltargetType = TTYPE_ENEMY_FOR_BUFF;
				sp.minrange = 0.0f;
				sp.maxrange = 20.0f;
				sp.procChance = 100;
				sp.spell = dbcSpell.LookupEntryForced( SPELL_ID_CORRUPTING_PARASITE );
				Unit *t = _unit->GetAIInterface()->FindTargetForSpellTargetType( &sp );
				if( t )
				{
					_unit->CastSpell( t, SPELL_ID_CORRUPTING_PARASITE, false );
					CorruptingParasiteTimer = TickNow + (SPELL_COOL_CORRUPTING_PARASITE + RandomUInt() % SPELL_COOL_CORRUPTING_PARASITE)*DifficultyScale;
					CorruptingParasiteCounter++;
				}
			}
			else if( ShouldForceProc( _unit, CorruptingParasiteCounter, SPELL_MINPROC_CORRUPTING_PARASITE, TENTACLE_START_FIGHT_HP_PCT ) )
				CorruptingParasiteTimer = 1;

			//get the tentacle of the active platform
			if( AlexStraszaCauterizeTimer < TickNow )
			{
				AlexStraszaCauterizeTimer = TickNow + SPELL_COOL_CAUTERIZE; //3% HP every X seconds 
				uint32 RequiresPlatform = 0;
				int64 *CanActivePlatformIndex = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_ACTIVE_PLATFORMS + RequiresPlatform );
				if( *CanActivePlatformIndex == 1 )
				{
					int64 *AspectGUID = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_ASPECT_ALEXSTRASZA );
					Creature *Aspect = _unit->GetMapMgr()->GetCreature( *AspectGUID );
					if( Aspect )
					{
						//cauterize the blistering tentacles and corrupted parasites
						InRangeSetRecProt::iterator itr,itr2;
						_unit->AquireInrangeLock(); //make sure to release lock before exit function !
						InrangeLoopExitAutoCallback AutoLock;
						for( itr2 = _unit->GetInRangeSetBegin( AutoLock ); itr2 != _unit->GetInRangeSetEnd();)
						{
							itr = itr2;
							itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
							if( (*itr)->IsCreature() == false || SafeUnitCast( *itr )->isAlive() == false )
								continue;
							if( (*itr)->GetEntry() == NPC_ID_BLISTERING_TENTACLE // NPC_ID_BLISTERING_TENTACLE 
								|| ( (*itr)->GetEntry() == NPC_ID_CORRUPTING_PARASITE )	
								)
							{
								Aspect->CastSpell( SafeCreatureCast(*itr), SPELL_ID_CAUTERIZE, false );
							}
						}
						_unit->ReleaseInrangeLock();
					}
				}
			}
		}
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		//cast Agonizing Pain on deathwing
		int64 *MadnessGUID = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_GUID_VISUAL );
//		*MadnessGUID = _unit->GetGUID();
		Creature *DW = _unit->GetMapMgr()->GetCreature( *MadnessGUID );
		if( DW )
		{
			DW->InterruptSpell( ); //in case he is channeling "cataclysm"
//			DW->CastSpell( DW, SPELL_ID_AGONIZING_PAIN, false );
			//loose 20% of health
			int32 HealthLost = -(int32)DW->GetMaxHealth() / 100  * MADNESS_LOOSE_HP_PCT_TENTACLE;
			DW->ModHealth( HealthLost );
/*{
char buff[500];
sprintf( buff, "Debug MSG : Limb died. Madness GUID %d, health mod %d", (int32)(*MadnessGUID), HealthLost );
_unit->SendChatMessage( CHAT_MSG_MONSTER_PARTY, LANG_UNIVERSAL, buff );
}/**/
		}
		//mark that the limb on this platform is dead and there is no point to activate it anymore
		int64 *LimbIndex = _unit->GetCreateIn64Extension( NPC_ID_LIMB0 );
		int64 *CanActivePlatformIndex = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_ACTIVE_PLATFORMS + *LimbIndex );
		*CanActivePlatformIndex = 0;
		//no need to block vision. Just begone
		_unit->Despawn( 1, 0 );
    }
}; 

//sub spawn of limbs
class BlisteringTentacle : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BlisteringTentacle);

    BlisteringTentacle(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		_unit->GetAIInterface()->m_canMove = false;
		for(uint32 i=0;i<SCHOOL_COUNT;i++)
			_unit->AoeDamageTakenPctMod[ i ] = 0.0f;
		_unit->CastSpell( _unit, SPELL_ID_BLISTERING_HEAT, false );
	}
	void OnDied(Unit *mKiller)
	{
		//no need to block vision. Just begone
		_unit->Despawn( 1, 0 );
		int64 *ActiveSpawnCount = _unit->GetMapMgr()->GetCreateIn64Extension( _unit->GetEntry() );
		*ActiveSpawnCount -= 1;
	}
};  

class MutatedCorruption : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MutatedCorruption);

    MutatedCorruption(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	uint32 CrushTimer;
	uint32 ImpaleTimer;
	float	DifficultyScale;
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;

		_unit->GetAIInterface()->m_canMove = false;
		CrushTimer = SPELL_COOL_CRUSH;
		ImpaleTimer = SPELL_COOL_IMPALE;

		_unit->GetAIInterface()->setOutOfCombatRange( 120.0f * 120.0f ); //once we start combat we should try to not reset
		RegisterAIUpdateEvent( 5000 );
	}
    void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			return;
		}
		uint32 TickNow = GetTickCount();
		if( CrushTimer < TickNow && _unit->isCasting() == false )
		{
			_unit->CastSpell( tank, SPELL_ID_CRUSH, false );
			CrushTimer = TickNow + (SPELL_COOL_CRUSH + RandomUInt() % SPELL_COOL_CRUSH)*DifficultyScale;
		}
		if( ImpaleTimer < TickNow && _unit->isCasting() == false )
		{
			_unit->CastSpell( tank, SPELL_ID_IMPALE, false );
			ImpaleTimer = TickNow + (SPELL_COOL_IMPALE + RandomUInt() % SPELL_COOL_IMPALE)*DifficultyScale;
		}
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
    }
}; 

class RegenerativeBlood : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(RegenerativeBlood);

    RegenerativeBlood(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		//proc on attack
		SpellEntry *sp = dbcSpell.LookupEntryForced( SPELL_ID_DEGENRATIVE_BITE );	//Degenerative Bite
		ProcTriggerSpell pts( sp, NULL );
		pts.caster = _unit->GetGUID();
		pts.spellId = sp->Id;
		pts.procFlags = PROC_ON_MELEE_ATTACK;
		pts.created_with_value = sp->eff[0].EffectBasePoints + 1;
		pts.procCharges = 0;
		pts.procChance = 100;
		pts.procInterval = SPELL_COOL_DEGENRATIVE_BITE;	
		_unit->RegisterScriptProcStruct( pts );

		_unit->PctPowerRegenModifier[POWER_TYPE_MANA] = 0.0f;

		_unit->SetPower( POWER_TYPE_MANA, 0 );
		_unit->SetPowerType( POWER_TYPE_MANA );
		_unit->SetMaxPower( POWER_TYPE_MANA, 100 );
		RegisterAIUpdateEvent( SPELL_COOL_REGENERATIVE );
	}
    void AIUpdate()
    {
		_unit->ModPower( POWER_TYPE_MANA, 10 );
//		_unit->UpdatePowerAmm( true, POWER_TYPE_MANA );
		if( _unit->GetPower( POWER_TYPE_MANA ) == 100 )
		{
			_unit->SetPower( POWER_TYPE_MANA, 0 );
			_unit->CastSpell( _unit, SPELL_ID_REGENERATIVE, false );	//self heal to max
		}
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		//no need to block vision. Just begone
		_unit->Despawn( 1, 0 );
		int64 *ActiveSpawnCount = _unit->GetMapMgr()->GetCreateIn64Extension( _unit->GetEntry() );
		*ActiveSpawnCount -= 1;
    }
}; 

class CorruptingParasite : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CorruptingParasite);

    CorruptingParasite(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		//cast backlash
		_unit->CastSpell( _unit, SPELL_ID_CORRUPTING_PARASITE_BACKLASH, true );
		RegisterAIUpdateEvent( SPELL_COOL_UNSTABLE_CORRUPTION );
	}
    void AIUpdate()
    {
		SpellEntry *spellInfo = dbcSpell.LookupEntryForced( SPELL_ID_UNSTABLE_CORRUPTION );
		Spell *spell = SpellPool.PooledNew( __FILE__, __LINE__ );
		spell->Init( _unit, spellInfo ,true, NULL);
		SpellCastTargets targets( _unit->GetGUID() );
		spell->forced_basepoints[0] = spellInfo->eff[0].EffectBasePoints * _unit->GetHealth() / 100; //with 6M health this does 600k dmg / tick ? :O
		spell->prepare(&targets);
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
    }
}; 

class ElementiumBolt : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ElementiumBolt);

    ElementiumBolt(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	uint8 phase;
	uint32 StartMoveTimer;
	uint32 ElementiumPulseTick;
	void OnLoad()
	{
		_unit->m_walkSpeed = 2.5f ;
		_unit->m_runSpeed = _unit->m_walkSpeed;
		_unit->m_base_runSpeed = _unit->m_runSpeed;
		_unit->m_base_walkSpeed = _unit->m_walkSpeed;
		_unit->m_flySpeed = _unit->m_walkSpeed;
		_unit->m_backFlySpeed = 4.5f / 2;
		_unit->m_backWalkSpeed = 4.5f / 2;	// this should really be named m_backRunSpeed
		_unit->m_swimSpeed = 4.722222f / 2;
		_unit->m_backSwimSpeed = 2.5f / 2;
		_unit->m_turnRate = 3.141593f / 2;
		_unit->m_pitchRate = 3.141593f / 2; 

		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
		RegisterAIUpdateEvent( 1000 );
		StartMoveTimer = GetTickCount() + 5000;
		phase = 0;
		ElementiumPulseTick = 0;
	}
    void AIUpdate()
    {
		if( phase == 0 )
		{
			if( StartMoveTimer != 0 )
			{
				if( StartMoveTimer < GetTickCount() )
				{
					int64 *MyLimIndex = _unit->GetCreateIn64Extension( NPC_ID_ELEMENTIUM_BOLT );
					_unit->GetAIInterface()->MoveTo( PlatformCenterLocations[ *MyLimIndex ][0], PlatformCenterLocations[ *MyLimIndex ][1], PlatformCenterLocations[ *MyLimIndex ][2], 0.0f );
					StartMoveTimer = 0;
				}
				else
				{
					return;
				}
			}

			//we are getting updated but we did not even started moving
			if( _unit->GetAIInterface()->m_creatureState == STOPPED )
				phase = 1;
		}
		else if( phase == 1 )		
		{
			_unit->CastSpell( _unit, SPELL_ID_ELEMNTIUM_BLAST, false ); //AOE 
			phase = 2;
			ElementiumPulseTick = GetTickCount() + SPELL_COOL_ELEMNTIUM_BLAST;
		}
		else if( phase == 2 )
		{
			uint32 TickNow = GetTickCount();
			if( ElementiumPulseTick < TickNow )
			{
				_unit->CastSpell( _unit, SPELL_ID_ELEMNTIUM_BLAST_PULSE, false ); //AOE pulse until destroyed
				ElementiumPulseTick = TickNow + SPELL_COOL_ELEMNTIUM_BLAST;
			}
		}
			
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		//no need to block vision. Just begone
		_unit->Despawn( 1, 0 );
		int64 *ActiveSpawnCount = _unit->GetMapMgr()->GetCreateIn64Extension( _unit->GetEntry() );
		*ActiveSpawnCount -= 1;
    }
}; 

class CongealingBlood : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(CongealingBlood);

    CongealingBlood(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		_unit->m_walkSpeed = 1.2f ;
		_unit->m_runSpeed = _unit->m_walkSpeed;
		_unit->m_base_runSpeed = _unit->m_runSpeed;
		_unit->m_base_walkSpeed = _unit->m_walkSpeed;
		_unit->m_flySpeed = _unit->m_walkSpeed;
		_unit->m_backFlySpeed = 4.5f / 2;
		_unit->m_backWalkSpeed = 4.5f / 2;	// this should really be named m_backRunSpeed
		_unit->m_swimSpeed = 4.722222f / 2;
		_unit->m_backSwimSpeed = 2.5f / 2;
		_unit->m_turnRate = 3.141593f / 2;
		_unit->m_pitchRate = 3.141593f / 2; 
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
		RegisterAIUpdateEvent( 3000 );
	}
    void AIUpdate()
    {
		int64 *MadnessGUID = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_ATTACKTARGET );
		Creature *DW = _unit->GetMapMgr()->GetCreature( *MadnessGUID );
		if( DW == NULL )
			return;
		//always try to move closer
		if( _unit->GetAIInterface()->m_creatureState == STOPPED )
			_unit->GetAIInterface()->MoveTo( DW->GetPositionX(), DW->GetPositionY(), -6.1f, 0.0f );
		//if we reach owner we heal him 1% HP and suicide
		if( DW->GetDistance2dSq( _unit ) < 5.0f*5.0f )
		{
			uint32 PCT1HP = DW->GetMaxHealth() * 1 / 100;
			if( DW->isAlive() )
				_unit->Heal( DW, SPELL_ID_CONGEALING_BLOOD_HEAL, PCT1HP );
			RemoveAIUpdateEvent();
			_unit->Despawn( 1, 0 );
		}
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		int64 *ActiveSpawnCount = _unit->GetMapMgr()->GetCreateIn64Extension( _unit->GetEntry() );
		*ActiveSpawnCount -= 1;
    }
}; 

//periodically cast shrapnel at a nearby player
class ElementiumFragment : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ElementiumFragment);

	uint32	ShrapnelTimer;
	float	DifficultyScale;
    ElementiumFragment(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		DifficultyScale = 1.0f;
		if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_10_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_10HEROIC;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN )
			DifficultyScale = SPEED_SCALE_DIFF_25NORMAL;
		else if( _unit->GetMapMgr()->instance_difficulty & INSTANCE_FLAG_RAID_25_MEN_HEROIC )
			DifficultyScale = SPEED_SCALE_DIFF_25HEROIC;
		_unit->GetAIInterface()->m_canMove = false;
		ShrapnelTimer = 0;
		RegisterAIUpdateEvent( 5000 );
	}
    void AIUpdate()
    {
		Unit *tank = _unit->GetAIInterface()->GetNextTarget();
		if( tank == NULL )
		{
			ShrapnelTimer = SPELL_COOL_SHRAPNEL;
			return;
		}
		uint32 TickNow = GetTickCount();
		if( ShrapnelTimer - 7000 < TickNow && ShrapnelTimer > TickNow && _unit->isCasting() == false )
		{
			_unit->CastSpell( tank, SPELL_ID_SHRAPNEL_TARGETED, false );
		}
		else if( ShrapnelTimer < TickNow && _unit->isCasting() == false )
		{
			_unit->CastSpell( tank, SPELL_ID_SHRAPNEL_DMG, false );
			ShrapnelTimer = TickNow + (SPELL_COOL_SHRAPNEL + RandomUInt() % SPELL_COOL_SHRAPNEL)*DifficultyScale;
		}
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		int64 *ActiveSpawnCount = _unit->GetMapMgr()->GetCreateIn64Extension( _unit->GetEntry() );
		*ActiveSpawnCount -= 1;
    }
};

//large melee dmg
class ElementiumTerror : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ElementiumTerror);

    ElementiumTerror(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	void OnLoad()
	{
		//proc on attack
		SpellEntry *sp = dbcSpell.LookupEntryForced( SPELL_ID_TETANUS );	//Degenerative Bite
		ProcTriggerSpell pts( sp, NULL );
		pts.caster = _unit->GetGUID();
		pts.spellId = sp->Id;
		pts.procFlags = PROC_ON_MELEE_ATTACK;
		pts.created_with_value = sp->eff[0].EffectBasePoints + 1;
		pts.procCharges = 0;
		pts.procChance = 100;
		pts.procInterval = SPELL_COOL_TETANUS;	
		_unit->RegisterScriptProcStruct( pts );
	}
    void AIUpdate()
    {
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
		int64 *ActiveSpawnCount = _unit->GetMapMgr()->GetCreateIn64Extension( _unit->GetEntry() );
		*ActiveSpawnCount -= 1;
    }
};

class AspectAIGeneric : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(AspectAIGeneric);

    AspectAIGeneric(Creature* pCreature) : CreatureAIScript(pCreature)    {    }
	uint32 CastSpell1,CastSpell2;
	uint32 RequiresPlatform;
	void OnLoad()
	{
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->disable_combat = true;
		_unit->GetAIInterface()->disable_targeting = true;
		_unit->bInvincible = true;
		_unit->GetAIInterface()->m_moveFly = true;
		//aura that will buff nearby players
		CastSpell1 = CastSpell2 = 0;
		RequiresPlatform = MADNESSPLATFORM_COUNT + 1;
		if( _unit->GetEntry() == NPC_ID_ASPECT_ALEXSTRASZA )
		{
			CastSpell1 = SPELL_ID_ALEXTRASZA_PRESENCE;
			RequiresPlatform = 0;
		}
		else if( _unit->GetEntry() == NPC_ID_ASPECT_NOZDORMU )
		{
			CastSpell1 = SPELL_ID_NOZDORMU_PRESENCE;
			RequiresPlatform = 1;
		}
		else if( _unit->GetEntry() == NPC_ID_ASPECT_YSERA )
		{
			CastSpell1 = SPELL_ID_YSERA_PRESENCE;
			CastSpell2 = SPELL_ID_ENTER_THE_DREAM;
			RequiresPlatform = 2;
		}
		else if( _unit->GetEntry() == NPC_ID_ASPECT_KALECGOS )
		{
			CastSpell1 = SPELL_ID_KALECGOS_PRESENCE;
			CastSpell2 = SPELL_ID_SPELLWEAVING;
			RequiresPlatform = 3;
		}
		else if( _unit->GetEntry() == NPC_ID_ASPECT_THRALL ) //thrall
		{
//			CastSpell1 = SPELL_ID_CARRYING_WINDS;
			RequiresPlatform = MADNESSPLATFORM_COUNT + 1;
		}
		//do not unload us until map gets destroyed to not go out of range :(
//		_unit->GetMapMgr()->PushStaticObject( _unit );
//		_unit->static_object = true; //do not go out of visual range
		_unit->GetAIInterface()->setOutOfCombatRange( 700.0f * 700.0f );
		RegisterAIUpdateEvent( 5000 ); //less then the duration of the presence spell
	}
    void AIUpdate()
    {
		if( RequiresPlatform < MADNESSPLATFORM_COUNT )
		{
			int64 *CanActivePlatformIndex = _unit->GetMapMgr()->GetCreateIn64Extension( NPC_ID_MADNESS_VISUAL + MADNESS_INDEX_ACTIVE_PLATFORMS + RequiresPlatform );
			if( *CanActivePlatformIndex == 0 )
				return;
		}
		InRangePlayerSetRecProt::iterator itr2;
		_unit->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = _unit->GetInRangePlayerSetBegin( AutoLock ); itr2 != _unit->GetInRangePlayerSetEnd(); itr2++)
		{
			if( (*itr2)->isAlive() == false )
				continue;
			float dist =  (*itr2)->GetDistance2dSq( _unit );
			if( dist > (700.0f * 700.0f) )
				continue;
			if( CastSpell1 )
				_unit->CastSpell( (*itr2), CastSpell1, true );
			if( CastSpell2 )
				_unit->CastSpell( (*itr2), CastSpell2, true );
		}
		_unit->ReleaseInrangeLock();
	}
	void OnDied(Unit *mKiller)
	{
		RemoveAIUpdateEvent();
    }
}; 

void DeathwingMadnessPlatformAreatriggers(Player * pPlayer, uint32 AreaTrigger)
{
/*	7412 - left areatrigger fall in abyss
	7110, 7315
	7111 - back to 7110
	7112, 7317
	7113, 7318 - back to 7112
	7114, 7319
	7115, 7320 - back to 7114
	*/

	bool DoJump = false;
	float m_destX;
	float m_destY;
	float m_destZ;
	switch(AreaTrigger)
	{
	case 7412:
		{	
			pPlayer->SafeTeleport( pPlayer->GetMapId(), pPlayer->GetInstanceID(), -12128.0f, 12181.0f, 2.0f, 5.50f );
		}break;
	case 7110:
//	case 7315: //land trigger
		{	
			m_destX = -12028.0f;
			m_destY = 12229.0f;
			m_destZ = -6.0f;
			DoJump = true;
		} break;
	case 7111:
//	case 7316: //land trigger
		{	
			m_destX = -11967.0f;
			m_destY = 12262.0f;
			m_destZ = 2.0f;
			DoJump = true;
		} break;
	case 7112:
//	case 7317: //land trigger
		{	
			m_destX = -12089.0f;
			m_destY = 12163.0f;
			m_destZ = -2.0f;
			DoJump = true;
		} break;
	case 7113:
//	case 7318: //land trigger
		{	
			m_destX = -12047.0f;
			m_destY = 12210.0f;
			m_destZ = -5.0f;
			DoJump = true;
		} break;
	case 7114:
//	case 7319: //land trigger
		{	
			m_destX = -12117.0f;
			m_destY = 12085.0f;
			m_destZ = 3.0f;
			DoJump = true;
		} break;
	case 7115:
//	case 7320: //land trigger
		{	
			m_destX = -12101.0f;
			m_destY = 12149.0f;
			m_destZ = -2.0f;
			DoJump = true;
		} break;
	}
	if( DoJump == true )
	{
		float radius = sqrt( Distance2DSq( pPlayer->GetPositionX(), pPlayer->GetPositionY(), m_destX, m_destY ) );
		float speed = 137.0f / 15.0f; //well this is just 1 example from blizz :p
		uint32 timetofly = (uint32)(radius * speed);
		float raise_mob_to_height = radius * 2.5f; //no idea about this value, should be parabolic parameter some sort
		pPlayer->SpeedCheatDelay( radius * 1000 / speed  + MIN( 5000, pPlayer->GetSession()->GetLatency() ) + 2000 );	//should start sampling again after we land
		pPlayer->GetAIInterface()->SendMoveToPacket( m_destX, m_destY, m_destZ, 0.0f, timetofly, MONSTER_MOVE_FLAG_HEROIC_LEAP, raise_mob_to_height );
		pPlayer->CastSpell( pPlayer, SPELL_ID_CARRYING_WINDS, true ); //comes from Thrall
	}
}

class SCRIPT_DECL DragonSoulTeleporter : public GossipScript
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

		int64 *EncounterStatus = pObject->GetMapMgr()->GetCreateIn64Extension( EXTENSION_MAP_BOS_KILLS );
		bool AddedMenu = false;
/*		for( uint32 i=EXTENSION_FLAG_MADNESS_DIED;i>0;i/=2 )
			if( *EncounterStatus & i )
			{
				Menu->AddItem(0, "Teleport to next boss", i);
				AddedMenu = true;
				break;
			}*/
//*EncounterStatus = 0xFFFFFFFF;
		if( *EncounterStatus & EXTENSION_FLAG_MORCHOCK_DIED )
		{
			Menu->AddItem(0, "Teleport to Warlord Zon'ozz", EXTENSION_FLAG_MORCHOCK_DIED );
			Menu->AddItem(0, "Teleport to Yor'sahj", EXTENSION_FLAG_ZONOZZ_DIED );
			AddedMenu = true;
		}
		if( *EncounterStatus & (EXTENSION_FLAG_ZONOZZ_DIED | EXTENSION_FLAG_YORSAHJ_DIED) )
		{
			Menu->AddItem(0, "Teleport to Hagara the Stormbinder", EXTENSION_FLAG_YORSAHJ_DIED);
			AddedMenu = true;
		}
		if( (*EncounterStatus & (EXTENSION_FLAG_HAGARA_DIED | EXTENSION_FLAG_ZONOZZ_DIED | EXTENSION_FLAG_YORSAHJ_DIED) ) == (EXTENSION_FLAG_HAGARA_DIED | EXTENSION_FLAG_ZONOZZ_DIED | EXTENSION_FLAG_YORSAHJ_DIED) )
		{
			Menu->AddItem(0, "Teleport to Ultraxion", EXTENSION_FLAG_HAGARA_DIED);
			AddedMenu = true;
		}
		if( *EncounterStatus & (EXTENSION_FLAG_ULTRAXION_DIED | EXTENSION_FLAG_HAGARA_DIED | EXTENSION_FLAG_ZONOZZ_DIED | EXTENSION_FLAG_YORSAHJ_DIED ) == (EXTENSION_FLAG_ULTRAXION_DIED | EXTENSION_FLAG_HAGARA_DIED | EXTENSION_FLAG_ZONOZZ_DIED | EXTENSION_FLAG_YORSAHJ_DIED) )
		{
			Menu->AddItem(0, "Teleport to Warmaster Blackhorn", EXTENSION_FLAG_ULTRAXION_DIED);
			AddedMenu = true;
		}
		if( *EncounterStatus & (EXTENSION_FLAG_BLACKHORN_DIED) )
		{
			Menu->AddItem(0, "Teleport to Spine of Deathwing", EXTENSION_FLAG_BLACKHORN_DIED);
			AddedMenu = true;
		}
		if( *EncounterStatus & (EXTENSION_FLAG_SPINE_DIED) )
		{
			Menu->AddItem(0, "Teleport to Deathwing", EXTENSION_FLAG_SPINE_DIED);
			AddedMenu = true;
		}
		if( AddedMenu == false )
			Menu->AddItem(0, "You need to kill more bosses to get some options here", 3);

		Menu->AddItem(1, "Close", 3 );
	    
		if(AutoSend)
			Menu->SendTo(Plr);
	};
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char *Code)
	{
		if( IntId == EXTENSION_FLAG_SPINE_DIED )
			Plr->SafeTeleport( Plr->GetMapId(), Plr->GetInstanceID(), -12127.8, 12173, -2, 5.7 );
		else if( IntId == EXTENSION_FLAG_BLACKHORN_DIED )
		{
			//cast parachute on the player
			Plr->CastSpell( Plr, 96039, true );
			Plr->SafeTeleport( Plr->GetMapId(), Plr->GetInstanceID(), -13855, -13669.6, 265.278+20.0f, 1.59 );
		}
		else if( IntId == EXTENSION_FLAG_ULTRAXION_DIED )
		{
			Plr->BroadcastMessage("Tip : Phase 1 lasts until you kill the drakes");
			Plr->SafeTeleport( Plr->GetMapId(), Plr->GetInstanceID(), 13485, -12134, 155, 3.0 );
		}
		else if( IntId == EXTENSION_FLAG_HAGARA_DIED )
		{
			Plr->BroadcastMessage("Tip : You can cancel Twilight Shift to get Heroic Will for 5 seconds to avoid Fading Light kill on expire");
			Plr->BroadcastMessage("Tip : Hour of Twilight needs to hit max 5 players or wipes. ");
			Plr->BroadcastMessage("Tip : Talk to the aspects to get the buffs");
			Plr->SafeTeleport( Plr->GetMapId(), Plr->GetInstanceID(), -1786, -2393, 342, 0 );
		}
		else if( IntId == EXTENSION_FLAG_YORSAHJ_DIED )
			Plr->SafeTeleport( Plr->GetMapId(), Plr->GetInstanceID(), 13632, 13613, 126, 3.14 );
		else if( IntId == EXTENSION_FLAG_ZONOZZ_DIED )
			Plr->SafeTeleport( Plr->GetMapId(), Plr->GetInstanceID(), -1835, -3102, -173, 0.67 );
		else if( IntId == EXTENSION_FLAG_MORCHOCK_DIED )
		{
			Plr->BroadcastMessage("Tip : Try to not let the ball hit the edge of the room");
			Plr->SafeTeleport( Plr->GetMapId(), Plr->GetInstanceID(), -1734, -1816, -219+2, 4.38 );
		}
		else
			Plr->CloseGossip();
	};
	void GossipEnd(Object* pObject, Player* pPlayer)
	{
		GossipScript::GossipEnd(pObject, pPlayer);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetupDragonSoul(ScriptMgr* mgr)
{
	/*
	delete from ai_agents where entry in ( 55265, 57773, 58473, 55418, 57875, 55417, 55308, 55312, 55864, 55866, 55866, 55865, 55867, 55862, 55863, 56231, 56265, 55689, 55294, 56427, 53879, 56173 );
	UPDATE creature_proto SET extra_a9_flags=0,hover_height=1,auras='' WHERE entry=58473;
	UPDATE creature_proto SET minhealth = 175000, maxhealth = 175000 where entry = 57875;
	update creature_proto set auras ='' where entry in (55418,57875,55417);
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39138','5');
	UPDATE worldmap_info SET viewingdistance = 200 WHERE entry = 967;
	#no need to spawn this. They get auto spawned on boss kill
	INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(55274, 'Dragon Soul Taxi', 'Teleporter', '', 0, 0, 7, 0, 4, 0, 0, 21666, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(55274, 81, 81, 35, 350000, 350000, 1000000, 3, 12417, 1000, 0, 20100, 32562, 0, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0, '108203 108203', 1, 0, 0, 0, 3.735, 11.9, 14, 0, 1, 0, 123456, 0, 0, 0, 0, 0);
	delete from creature_spawns where entry in (58473,55418,57875,55417) and map=967;
	#UPDATE `worldmap_info` SET `repopx`='-2292',`repopy`='-2388',`repopz`='87',`repopentry`='967' WHERE `entry`='967';
	UPDATE `worldmap_info` SET `repopx`='-8269',`repopy`='-4513',`repopz`='-217',`repopentry`='1' WHERE `entry`='967';
	#INSERT INTO `graveyards`(`id`,`position_x`,`position_y`,`position_z`,`orientation`,`zoneid`,`adjacentzoneid`,`mapid`,`faction`,`name`) VALUES ( NULL,'-2292','-2388','87','0','0','0','967','3','Dragon Soul Raid inside');
	INSERT INTO `graveyards`(`id`,`position_x`,`position_y`,`position_z`,`orientation`,`zoneid`,`adjacentzoneid`,`mapid`,`faction`,`name`) VALUES ( NULL,'-8269','-4513','-217','0','0','0','1','3','Dragon Soul Raid outside');
	#INSERT INTO `creature_spawns` ( `entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes0`, `bytes1`, `bytes2`, `emote_state`, `standstate`, `mountdisplayid`, `slot1item`, `slot2item`, `slot3item`, `difficulty_mask`, `phase_mask`) VALUES
	#( 6491, 967, -2293, -2388.49, 86.2416, 6.0829, 0, 5233, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65535, 2147483647);
	INSERT INTO `creature_spawns` ( `entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes0`, `bytes1`, `bytes2`, `emote_state`, `standstate`, `mountdisplayid`, `slot1item`, `slot2item`, `slot3item`, `difficulty_mask`, `phase_mask`) VALUES
	( 6491, 1, -8269, -4513, -217, 6.0829, 0, 5233, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65535, 2147483647);


	#if zon'ozz spawn is missing then execute this
	#INSERT INTO `creature_spawns` (`entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes0`, `bytes1`, `bytes2`, `emote_state`, `standstate`, `mountdisplayid`, `slot1item`, `slot2item`, `slot3item`, `difficulty_mask`, `phase_mask`) VALUES
	#(55308, 967, -1769.33, -1916.87, -226.171, 1.29154, 12, 39138, 14, 0, 16908544, 0, 1, 0, 0, 0, 0, 0, 0, 240, 2147483647);

	#if Yor'sahj spawn is missing then execute this
	#INSERT INTO `creature_spawns` ( `entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes0`, `bytes1`, `bytes2`, `emote_state`, `standstate`, `mountdisplayid`, `slot1item`, `slot2item`, `slot3item`, `difficulty_mask`, `phase_mask`) VALUES
	#( 55312, 967, -1765.65, -3034.35, -182.276, 3.52556, 12, 39101, 14, 0, 16908544, 0, 1, 0, 0, 0, 0, 0, 0, 240, 2147483647);
	update creature_proto set auras='' where entry=55312;
	delete from creature_spawns where entry in (55864,55866,55865,55867,55862,55863,56231,56265) and position_x > -1765.65 - 300 && position_x < -1765.65 + 300 and position_y > -3034.65 - 300 && position_y < -3034.65 + 300 and map=967;
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39101','5');
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '36110','5');
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39248','5');
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39249','5');
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39628','5');
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39251','5');
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39252','5');
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39369','5');
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '27849','5');

	#if Hagara the Stormbinder spawn is missing then execute this
	#INSERT INTO `creature_spawns` ( `entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes0`, `bytes1`, `bytes2`, `emote_state`, `standstate`, `mountdisplayid`, `slot1item`, `slot2item`, `slot3item`, `difficulty_mask`, `phase_mask`) VALUES
	#( 55689, 967, 13550, 13613.2, 134.427, 0, 12, 39318, 14, 32832, 66048, 0, 1, 0, 0, 0, 75237, 75237, 0, 240, 2147483647);
	update creature_proto set auras='' where entry in ( 56165, 56700, 56136, 56104 );

	#Ultraxion : note that you need a larger patch for this
	#make sure he can fly
	UPDATE creature_proto SET extra_a9_flags = extra_a9_flags | (1<<2) | 0x80000 WHERE entry = 55294;
	#adjust position so he can be seen
	UPDATE creature_spawns SET POSITION_x = -1718, position_Y = -2387, position_z = 347 WHERE map=967 AND entry = 55294;
	#he has 25 yards size : black horse size
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39099','25');
	update creature_proto set npcflags=npcflags|1|2 where entry in (56630,56665,56664,56666,56667,56587,56781);
	update creature_proto set extra_a9_flags = extra_a9_flags | (1<<2) | 0x80000 where entry in (56249,56250,56251,57795,56668,56694,56587,56855);

	#Warmaster Blackhorn
	UPDATE creature_spawns SET POSITION_x = 13434, position_Y = -12132, position_z = 152 WHERE map=967 AND entry = 56427;
	UPDATE creature_spawns SET POSITION_x = 13430, position_Y = -12079, position_z = 148, orientation = 4.83 WHERE map=967 AND entry = 56781;
	delete from creature_waypoints where spawnid in ( select id from creature_spawns WHERE map=967 AND entry = 56781 );
	#adds should be spawned dynamically
	delete from creature_spawns where entry in ( 56854,56848,56923 ) and map=967;
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39691','10');

	#spine
	delete from gameobject_spawns where map=967 and entry in ( 209623, 209631, 209632 );
	delete from creature_spawns where entry in ( 56575, 56161, 56162, 53891, 53890, 53889 ) and map=967;
	replace into gameobject_names (`entry`,`Type`,`DisplayID`,`Name`,`info_str`,`castbarcaption`,`spellfocus`,`sound1`,`sound2`,`sound3`,`sound4`,`sound5`,`sound6`,`sound7`,`sound8`,`sound9`,`unknown1`,`unknown2`,`unknown3`,`unknown4`,`unknown5`,`unknown6`,`unknown7`,`unknown8`,`unknown9`,`unknown10`,`unknown11`,`unknown12`,`unknown13`,`unknown14`,`scale`,`RelatedItemId1`,`RelatedItemId2`,`RelatedItemId3`,`RelatedItemId4`,`RelatedItemId5`,`RelatedItemId6`) values ('209624','11','10824','Deathwing Back Plate 1 (DWR_PLATE_02)','','','0','0','0','0','0','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','0','0','0','0','0','0');
	replace into gameobject_names (`entry`,`Type`,`DisplayID`,`Name`,`info_str`,`castbarcaption`,`spellfocus`,`sound1`,`sound2`,`sound3`,`sound4`,`sound5`,`sound6`,`sound7`,`sound8`,`sound9`,`unknown1`,`unknown2`,`unknown3`,`unknown4`,`unknown5`,`unknown6`,`unknown7`,`unknown8`,`unknown9`,`unknown10`,`unknown11`,`unknown12`,`unknown13`,`unknown14`,`scale`,`RelatedItemId1`,`RelatedItemId2`,`RelatedItemId3`,`RelatedItemId4`,`RelatedItemId5`,`RelatedItemId6`) values ('209625','11','10825','Deathwing Back Plate 2 (DWR_PLATE_03)','','','0','0','0','0','0','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','0','0','0','0','0','0');
	replace into gameobject_names (`entry`,`Type`,`DisplayID`,`Name`,`info_str`,`castbarcaption`,`spellfocus`,`sound1`,`sound2`,`sound3`,`sound4`,`sound5`,`sound6`,`sound7`,`sound8`,`sound9`,`unknown1`,`unknown2`,`unknown3`,`unknown4`,`unknown5`,`unknown6`,`unknown7`,`unknown8`,`unknown9`,`unknown10`,`unknown11`,`unknown12`,`unknown13`,`unknown14`,`scale`,`RelatedItemId1`,`RelatedItemId2`,`RelatedItemId3`,`RelatedItemId4`,`RelatedItemId5`,`RelatedItemId6`) values ('209626','11','10826','Deathwing Back Plate 3 (DWR_PLATE_04)','','','0','0','0','0','0','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','0','0','0','0','0','0');

	#madness
	delete from creature_spawns where entry in ( 56173, 57962 );
	delete from creature_spawns where map=967 and  entry in ( 56263, 56188, 56724, 56710, 56262, 56581, 56699, 56307, 56332, 57788, 56519, 56311, 56471 );
	#delete from gameobject_spawns where entry in ( 210220 );
	delete from gameobject_spawns where entry in ( 210222 );
	UPDATE creature_proto SET extra_a9_flags = 0x80000 | ( 1<< 2 ) WHERE entry IN ( 56168, 56167, 56846, 56173, 56099, 56100, 56101, 56102, 56471, 57842, 56262 );
	UPDATE creature_proto SET auras='' where entry=56173;
//	replace into creature_staticspawns (`entry`,`Map`,`position_x`,`position_y`,`position_z`,`orientation`,`movetype`,`displayid`,`faction`,`flags`,`bytes0`,`bytes1`,`bytes2`,`emote_state`,`standstate`,`mountdisplayid`,`slot1item`,`slot2item`,`slot3item`,`difficulty_mask`) values ('56173','967','-11903.9','11989.1','-113.104','2.16421','12','39355','14','0','16908544','0','1','0','0','0','0','0','0','240');
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39405','15');
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39349','15');
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39509','15');
	INSERT INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39354','15');
	#replace INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39355','310');
	replace INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39355','30');
	replace INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '39947','100');
	replace INTO `unit_display_sizes`(`DisplayID`,`halfsize`) VALUES ( '35268','100');
	UPDATE creature_proto SET auras = REPLACE(auras, '4659 ', '') WHERE auras LIKE '4659 %';
	UPDATE creature_proto SET auras = REPLACE(auras, ' 4659', '') WHERE auras LIKE '% 4659';
	UPDATE creature_proto SET auras = REPLACE(auras, ' 4659 ', ' ') WHERE auras LIKE '% 4659 %';
	UPDATE creature_proto SET auras = REPLACE(auras, '109247 ', '') WHERE auras LIKE '109247 %';
	UPDATE creature_proto SET auras = REPLACE(auras, ' 109247', '') WHERE auras LIKE '% 109247';
	UPDATE creature_proto SET auras = REPLACE(auras, ' 109247 ', ' ') WHERE auras LIKE '% 109247 %';
	UPDATE creature_proto SET auras = REPLACE(auras, '91565 ', '') WHERE auras LIKE '91565 %';
	UPDATE creature_proto SET auras = REPLACE(auras, ' 91565', '') WHERE auras LIKE '% 91565';
	UPDATE creature_proto SET auras = REPLACE(auras, ' 91565 ', ' ') WHERE auras LIKE '% 91565 %';
	UPDATE creature_proto SET auras = REPLACE(auras, '5509 ', '') WHERE auras LIKE '5509 %';
	UPDATE creature_proto SET auras = REPLACE(auras, ' 5509', '') WHERE auras LIKE '% 5509';
	UPDATE creature_proto SET auras = REPLACE(auras, ' 5509 ', ' ') WHERE auras LIKE '% 5509 %';

	INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
	(56174, 'Madness Fight controller', '', '', 16778240, 0, 10, 0, 0, 0, 0, 10906, 27823, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 0);
	INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(56174, 87, 87, 35, 82994, 82994, 0, 1, 0, 2000, 0, 6348, 8252, 500, 0, 0, 600000, 0, 0, 0, 0, 0, 0, 0, 1, 0.25, '', 0, 0, 0, 0, 2.5, 10, 7, 0, 1, 0, 56174, 0, 0, 0, 0, 0);
	INSERT INTO `creature_spawns` ( `entry`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `movetype`, `displayid`, `faction`, `flags`, `bytes0`, `bytes1`, `bytes2`, `emote_state`, `standstate`, `mountdisplayid`, `slot1item`, `slot2item`, `slot3item`, `difficulty_mask`, `phase_mask`) VALUES
	(56174, 967, -12127.9, 12173.7, -2.73372, 5.71848, 0, 27823, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65535, 2147483647);

	INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(57479, 87, 87, 14, 1493892, 1493892, 100, 2, 0, 3600, 0, 19940, 30508, 500, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 3, 1, '', 0, 0, 0, 0, 2.5, 12, 7, 1, 1, 0, 57479, 0, 0, 0, 0, 0);
	INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
	(57798, 87, 87, 14, 1493892, 1493892, 100, 2, 0, 3600, 0, 19940, 30508, 500, 0, 0, 3600000, 0, 0, 0, 0, 0, 0, 0, 3, 1, '105842 3180', 0, 0, 0, 0, 2.5, 12, 7, 1, 1, 0, 56263, 0, 0, 0, 0, 0);

	UPDATE loot_creature SET entry=57962 WHERE entry=56173;

	create table `BOSSKILLS`( `Entry` int(10) , `Player` int(10) , `KillStamp` int(10), `group_size` int(10), `diff` int(10) );

	update creature_proto set modImmunities=0x7FFFFFFF,respawntime=0x00FFFFFF where entry in ( 56173,56427,56781,55294,55265,57773,55689,55308,55312,55864,55866,55865,55867,55862,55863 );
	*/

	SpellEntry *sp; 
    
	///////////////////////////////////////////////////////
	GossipScript * gs = (GossipScript*) new DragonSoulTeleporter();
	mgr->register_gossip_script(55274, gs);

	// Morchok
	mgr->register_creature_script(55265, &Morchok::Create);
	mgr->register_creature_script(57773, &Morchok::Create); //this is Kohcrom, twin of Morchok
	//Summon Kohcrom
	sp = dbcSpell.LookupEntryForced( 109017 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;	
	}
    
    // Warlord Zon'ozz
    mgr->register_creature_script(MOB_ENTRY_ZONOZZ, &Zonozz::Create);
    mgr->register_creature_script(MOB_ENTRY_VOID_OF_THE_UNMAKING, &VoidOfTheUnmaking::Create);
    mgr->register_creature_script(MOB_ENTRY_CLAW_OF_GORATH, &ClawOfGorath::Create);
    mgr->register_creature_script(MOB_ENTRY_EYE_OF_GORATH, &EyeOfGorath::Create);
    mgr->register_creature_script(MOB_ENTRY_FLAIL_OF_GORATH, &FlailOfGorath::Create);
	
	sp = dbcSpell.LookupEntryForced( SPELL_ID_INCREASE_DAMAGE_TAKEN_ZONOZZ );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->DurationIndex = 6;	//10 minutes ? This gets removed when phase 2 ends anyway
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_BLACK_BLOOD_OF_GORATH );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 )
			sp->eff[0].EffectRadiusIndex = 12;	//100 yards
		//non blizzlike, but the noobs die all the time
		sp->eff[0].EffectAmplitude *= 2;	//double the tick interval
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_PSYCHIC_DRAIN );
	//if( sp != NULL )
		sp->eff[0].EffectBasePoints = sp->eff[0].EffectBasePoints / 10; //might be too much
	sp = dbcSpell.LookupEntryForced( SPELL_ID_PSYCHIC_DRAIN_HEROIC );
	//if( sp != NULL )
		sp->eff[0].EffectBasePoints = sp->eff[0].EffectBasePoints / 10; //might be too much
    
    // Yor'sahj the Unsleeping
    mgr->register_creature_script(MOB_ENTRY_YORSAHJ, &Yorsahj::Create);
    mgr->register_creature_script(MOB_ENTRY_GLOWING_G, &GlowingGlobule::Create);
    mgr->register_creature_script(MOB_ENTRY_COBALT_G, &CobaltGlobule::Create);
    mgr->register_creature_script(MOB_ENTRY_CRIMSON_G, &CrimsonGlobule::Create);
    mgr->register_creature_script(MOB_ENTRY_DARK_G, &DarkGlobule::Create);
    mgr->register_creature_script(MOB_ENTRY_ACID_G, &AcidicGlobule::Create);
    mgr->register_creature_script(MOB_ENTRY_SHADOWED_G, &ShadowedGlobule::Create);
    mgr->register_creature_script(MOB_ENTRY_MANA_VOID, &ManaVoid::Create);
    mgr->register_creature_script(MOB_ENTRY_FORGOTTEN_ONE, &ForgottenOne::Create);
	//deep corruption explodes after 5 heal procs
	sp = dbcSpell.LookupEntryForced( SPELL_ID_DEEP_CORRUPTION );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		if( sp->eff[0].EffectRadiusIndex == 0 )
			sp->eff[0].EffectRadiusIndex = 12;	//100 yards
		sp->procFlags = PROC_ON_HEAL_SPELL;
		sp->procCharges = 1;
		sp->eff[0].EffectTriggerSpell = 105173; //smallest dmg amount after first heal instead 5 stacks
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_SEARING_BLOOD );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->rangeIndex = 13;//Anywhere
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_PSYCHIC_SLICE );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_DIGESTIVE_ACID );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->rangeIndex = 13;//Anywhere
	}
	
	// Hagara the Stormbinder
    mgr->register_creature_script(55689, &Hagara::Create);
	mgr->register_creature_script(MOB_ENTRY_ICE_TOMB, &IceTomb::Create);	
    mgr->register_creature_script(NPC_ID_BINDING_CRYSTAL, &FrozenBindingCrystal::Create);
    mgr->register_creature_script(NPC_ID_ICE_WAVE, &IceWave::Create);
    mgr->register_creature_script(NPC_ID_LIGHTNING_CONDUIT, &CrystalConductor::Create);
    mgr->register_creature_script(NPC_ID_BOUND_LIGHTNING_ELEMENTAL, &BoundLightningElemental::Create);
	sp = dbcSpell.LookupEntryForced( SPELL_ID_FROZEN_TEMPEST );
	//if( sp != NULL )
	{
		//periodically try to cast "Watery Entrenchment" to nearby players
		sp->eff[1].Effect = SPELL_EFFECT_NULL;
//		sp->eff[1].Effect = SPELL_EFFECT_APPLY_AURA;
//		sp->eff[1].EffectApplyAuraName = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
//		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_SELF;
//		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
//		sp->eff[1].EffectRadiusIndex = 20; //25 yards
//		sp->eff[1].EffectTriggerSpell = SPELL_ID_WATERY_ENCHANTMENT;
//		sp->eff[1].EffectAmplitude = 1000;	//every 5 seconds says the description
//		sp->c_is_flags2 |= SPELL_FLAG2_IS_TRIGGERED_ON_TARGET;
		sp->DurationIndex = 21; //infinit, waiting for players to destroy the pillons
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_WATERY_ENCHANTMENT );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = 20; //25 yards
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER;
		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[1].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
		sp->eff[1].EffectRadiusIndex = 20; //25 yards
		sp->eff[2].Effect = SPELL_EFFECT_NULL;
		sp->DurationIndex = 1; //10 seconds
		sp->rangeIndex = 34;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_FROZEN_TEMPEST_JUST_IMMUNE );
	//if( sp != NULL )
	{
		sp->eff[1].Effect = SPELL_EFFECT_NULL;
		sp->eff[2].Effect = SPELL_EFFECT_NULL;
		sp->DurationIndex = 21; //infinit, waiting for players to destroy the pillons
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_FROST_FLAKE );
	//if( sp != NULL )
	{
		sp->eff[1].EffectApplyAuraName = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
		sp->eff[1].EffectTriggerSpell = 110316;
		sp->eff[1].EffectAmplitude = 10000;
	}
	sp = dbcSpell.LookupEntryForced( 110316 );	//Frostflake Snare
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->DurationIndex = dbcSpell.LookupEntryForced( SPELL_ID_FROST_FLAKE )->DurationIndex;
	}
	sp = dbcSpell.LookupEntryForced( 109563 );	//Storm Pillar
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_LIGHTNING_CHAIN );
	//if( sp != NULL )
	{
		sp->eff[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SCRIPTED_TARGET;
		sp->eff[1].Effect = SPELL_EFFECT_TRIGGER_SPELL;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_SCRIPTED_TARGET;
		sp->eff[1].EffectTriggerSpell = sp->Id;
		sp->c_is_flags2 |= SPELL_FLAG2_IS_TRIGGERED_ON_TARGET;
		sp->dmg_type = SPELL_DMG_TYPE_MAGIC;
		sp->c_is_flags2 |= SPELL_FLAG2_IS_ENEMY_AOE_TARGETTING;
	}
	//this spams combat log so much you can't see anything
	sp = dbcSpell.LookupEntryForced( SPELL_ID_ICE_WAVE_VISIBLE );
	//if( sp != NULL )
	{
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_DUMMY; //no need to spam the chatlog with continues casts
	}
	//setting target type so we can manually adjust radius in case we want to make the instance more easy
	sp = dbcSpell.LookupEntryForced( SPELL_ID_ICE_WAVE_DAMAGE );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY; 
		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->rangeIndex = 3; //20 yards
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_ICE_TOMB_STUN );
	//if( sp != NULL )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
	}

    // Ultraxion
    mgr->register_creature_script(55294, &Ultraxion::Create);
/*    mgr->register_creature_script(56630, &UltraxionAlexstrasza::Create); // The Aspect
    mgr->register_creature_script(56665, &UltraxionYsera::Create);       // The Aspect
    mgr->register_creature_script(56664, &UltraxionKalecgos::Create);    // The Aspect
    mgr->register_creature_script(56666, &UltraxionNozdormu::Create);    // The Aspect
    mgr->register_creature_script(56667, &UltraxionThrall ::Create);     // The Aspect
	*/
	sp = dbcSpell.LookupEntryForced( SPELL_ID_FADING_LIGHT );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_FADED_INTO_LIGHT );
	//if( sp != NULL )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
	}
	
	sp = dbcSpell.LookupEntryForced( SPELL_ID_TWILIGHT_SHIFT );
	//if( sp != NULL )
	{
		sp->c_is_flags2 |= SPELL_FLAG2_IS_MAP_CAST_BOUND;
		sp->Attributes &= ~ATTRIBUTES_CANT_CANCEL;
	}
	sp = dbcSpell.LookupEntryForced( 106175 ); //	SPELL_ID_HEROIC_WILL
	//if( sp != NULL )
	{
		sp->maxstack = 1;
	}
	sp = dbcSpell.LookupEntryForced( 105929 ); //	SPELL_ID_HEROIC_WILL
	//if( sp != NULL )
	{
		sp->maxstack = 2;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_LOOMING_DARKNESS );
	//if( sp != NULL )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
	}

	sp = dbcSpell.LookupEntryForced( SPELL_ID_UNSTABLE_MONSTROSITY_DMG );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = 35; //25+30 yards
		sp->eff[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
		sp->rangeIndex = 170; //25+30 yards
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_TWILIGHT_BURST );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = 28; //anywhere
		sp->eff[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
		sp->rangeIndex = 13; //anywhere
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_FADING_LIGHT );
	//if( sp != NULL )
	{
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_DUMMY;
		sp->eff[1].Effect = SPELL_EFFECT_NULL;
	}
	
	UltraxionHelperGossip *gs2;
	gs2 = new UltraxionHelperGossip();
	gs2->OfferedSpell = SPELL_ID_GIFT_OF_LIFE; //Gift of Life
	mgr->register_gossip_script(56630, (GossipScript*)gs2);
	gs2 = new UltraxionHelperGossip();
	gs2->OfferedSpell = SPELL_ID_ESSENCE_OF_DREAMS; //Essence of Dreams
	mgr->register_gossip_script(56665, (GossipScript*)gs2);
	gs2 = new UltraxionHelperGossip();
	gs2->OfferedSpell = SPELL_ID_SOURCE_OF_MAGIC; //Source of Magic
	mgr->register_gossip_script(56664, (GossipScript*)gs2);
	gs2 = new UltraxionHelperGossip();
	gs2->OfferedSpell = SPELL_ID_TIMELOOP; //Timeloop
	mgr->register_gossip_script(56666, (GossipScript*)gs2);
	gs2 = new UltraxionHelperGossip();
	gs2->OfferedSpell = SPELL_ID_LAST_DEFENDER_OF_AZEROTH; //Last Defender of Azeroth
	mgr->register_gossip_script(56667, (GossipScript*)gs2);

	//do not allow players to 
	dbcSpell.LookupEntryForced( SPELL_ID_GIFT_OF_LIFE )->c_is_flags2 |= SPELL_FLAG2_IS_MAP_CAST_BOUND;
	//scripted this one in core
	dbcSpell.LookupEntryForced( SPELL_ID_ESSENCE_OF_DREAMS )->c_is_flags2 |= SPELL_FLAG2_IS_MAP_CAST_BOUND;
	dbcSpell.LookupEntryForced( SPELL_ID_SOURCE_OF_MAGIC )->c_is_flags2 |= SPELL_FLAG2_IS_MAP_CAST_BOUND;
	//scripted this one in core
	dbcSpell.LookupEntryForced( SPELL_ID_TIMELOOP )->c_is_flags2 |= SPELL_FLAG2_IS_MAP_CAST_BOUND;
	sp = dbcSpell.LookupEntryForced( SPELL_ID_LAST_DEFENDER_OF_AZEROTH );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_RAID; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_ALL_RAID; 
		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->c_is_flags2 |= SPELL_FLAG2_IS_MAP_CAST_BOUND;
	}
    
    // Warmaster Blackhorn
    mgr->register_creature_script(56427, &Blackhorn::Create);
    mgr->register_creature_script(56681, &HarpoonGun::Create);
    mgr->register_creature_script(57260, &SkyfireCannon::Create);
    mgr->register_creature_script(NPC_ID_ASSAULT_DRAKE, &TwilightAssaultDrake::Create);
    mgr->register_creature_script(NPC_ID_GORINA, &Goriona::Create);
    mgr->register_creature_script(56854, &TwilightEliteDreadblade::Create);
    mgr->register_creature_script(56848, &TwilightEliteSlayer::Create);
    mgr->register_creature_script(56923, &TwilightSapper::Create);
	sp = dbcSpell.LookupEntryForced( 108038 ); //Harpoon
	//if( sp != NULL )
	{
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_DUMMY;
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
	}    
	sp = dbcSpell.LookupEntryForced( SPELL_ID_DECK_FIRE ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectAmplitude = 2000;
		sp->eff[0].EffectTriggerSpell = 110095;
		sp->c_is_flags2 |= SPELL_FLAG2_IS_AA_CALLING_SCRIPT;
	}    
	sp = dbcSpell.LookupEntryForced( SPELL_ID_TWILIGHT_ONSLAUGHT ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
	}    
	sp = dbcSpell.LookupEntryForced( SPELL_ID_TWILIGHT_FLAMES ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
	}    
	sp = dbcSpell.LookupEntryForced( SPELL_ID_TWILIGHT_BREATH ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_IN_FRONT_OF_CASTER;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
	}    
	sp = dbcSpell.LookupEntryForced( SPELL_ID_CONSUMING_SHROUD ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
	}    
	sp = dbcSpell.LookupEntryForced( SPELL_ID_VENGEANCE ); 
	//if( sp != NULL )
	{
		sp->eff[1].Effect = SPELL_EFFECT_NULL;
	}    
	sp = dbcSpell.LookupEntryForced( SPELL_ID_SIPHON_HP ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_DUEL;
		sp->eff[0].EffectConversionCoef = 1.0f;
	}    

	// Deathwing <The Destroyer> - Spine of Deathwing
    mgr->register_creature_script(53879, &DeathwingSpine::Create);
    mgr->register_creature_script(56575, &BurningTendons::Create);
    mgr->register_creature_script(56161, &Corruption::Create);
    mgr->register_creature_script(53890, &HideousAmalgamation::Create);
    mgr->register_creature_script(53888, &GraspingTendrilCaster::Create);
    mgr->register_creature_script(53889, &CorruptedBlood::Create);
	//Grasping Tendrils SPELL_ID_GRASPING_TENDRILLS
	sp = dbcSpell.LookupEntryForced( SPELL_ID_GRASPING_TENDRILLS_AURA );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = sp->eff[0].EffectRadiusMaxIndex;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[1].EffectRadiusIndex = sp->eff[1].EffectRadiusMaxIndex;
	}
	sp = dbcSpell.LookupEntryForced( 109454 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = sp->eff[0].EffectRadiusMaxIndex;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[1].EffectRadiusIndex = sp->eff[1].EffectRadiusMaxIndex;
	}
	sp = dbcSpell.LookupEntryForced( 109455 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = sp->eff[0].EffectRadiusMaxIndex;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[1].EffectRadiusIndex = sp->eff[1].EffectRadiusMaxIndex;
	}
 	sp = dbcSpell.LookupEntryForced( 109456 );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = sp->eff[0].EffectRadiusMaxIndex;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[1].EffectRadiusIndex = sp->eff[1].EffectRadiusMaxIndex;
	}
 	sp = dbcSpell.LookupEntryForced( SPELL_ID_ABSORB_BLOOD );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_DUEL; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
	}
 	sp = dbcSpell.LookupEntryForced( SPELL_ID_SUPERHEATED );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = sp->eff[0].EffectRadiusMaxIndex;
	}
 	sp = dbcSpell.LookupEntryForced( SPELL_ID_DEGRADATION );
	//if( sp != NULL )
	{
		sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = sp->eff[0].EffectRadiusMaxIndex;
		sp->c_is_flags2 |= SPELL_FLAG2_IS_MAP_CAST_BOUND;
	}
 	sp = dbcSpell.LookupEntryForced( SPELL_ID_BURST );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = sp->eff[0].EffectRadiusMaxIndex;
	}
 	sp = dbcSpell.LookupEntryForced( SPELL_ID_RESIDUE );
	//if( sp != NULL )
	{
		sp->AttributesEx |= ATTRIBUTESEX_VISIBLE;
		sp->AttributesExB |= SPELL_ATTR2_ALLOW_DEAD_TARGET;
		sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_DUMMY; //just the visuals, no transform no nothing
		sp->eff[1].Effect = SPELL_EFFECT_NULL;
		sp->eff[2].Effect = SPELL_EFFECT_NULL;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_SEAL_ARMOR_BREACH1 );
	//if( sp != NULL )
	{
		sp->DurationIndex = 9; // 30 sec
		sp->eff[2].EffectApplyAuraName = SPELL_AURA_DUMMY; //old one was giving immunity
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_FIERY_GRIP ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS;
		sp->MaxAffectedTargets = 3;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = sp->eff[0].EffectRadiusMaxIndex;
	}    
	
    // Deathwing <The Destroyer> - Madness of Deathwing
    mgr->register_creature_script(NPC_ID_MADNESS_FIGHT_CONTROLLER, &DeathwingMadnessController::Create);
    mgr->register_creature_script(NPC_ID_MADNESS_VISUAL, &DeathwingMadnessVisual::Create);
    mgr->register_creature_script(NPC_ID_MADNESS_ATTACKTARGET, &DeathwingMadnessAttackTarget::Create);
	mgr->register_creature_script(NPC_ID_ELEMENTIUM_BOLT, &ElementiumBolt::Create);
	mgr->register_creature_script(NPC_ID_LIMB0, &LimbGeneric::Create);
	mgr->register_creature_script(NPC_ID_LIMB1, &LimbGeneric::Create);
	mgr->register_creature_script(NPC_ID_LIMB2, &LimbGeneric::Create);
	mgr->register_creature_script(NPC_ID_LIMB3, &LimbGeneric::Create);
    mgr->register_creature_script(NPC_ID_MUTATED_CORRUPTION, &MutatedCorruption::Create);
    mgr->register_creature_script(NPC_ID_BLISTERING_TENTACLE, &BlisteringTentacle::Create);
    mgr->register_creature_script(NPC_ID_REGENERATIVE_BLOOD, &RegenerativeBlood::Create);
	mgr->register_creature_script(NPC_ID_CORRUPTING_PARASITE, &CorruptingParasite::Create);
    mgr->register_creature_script(NPC_ID_CONGEALING_BLOOD, &CongealingBlood::Create);
    mgr->register_creature_script(NPC_ID_ELEMENTIUM_FRAGMENT, &ElementiumFragment::Create);
    mgr->register_creature_script(NPC_ID_ELEMNTIUM_TERROR, &ElementiumTerror::Create);
    mgr->register_creature_script(NPC_ID_ASPECT_ALEXSTRASZA, &AspectAIGeneric::Create); // The Aspect DeathwingMadnessAlexstrasza 58207
    mgr->register_creature_script(NPC_ID_ASPECT_NOZDORMU, &AspectAIGeneric::Create); // The Aspect DeathwingMadnessNozdormu 58208
    mgr->register_creature_script(NPC_ID_ASPECT_KALECGOS, &AspectAIGeneric::Create); // The Aspect DeathwingMadnessKalecgos 58210
    mgr->register_creature_script(NPC_ID_ASPECT_YSERA, &AspectAIGeneric::Create); // The Aspect DeathwingMadnessYsera 58209
    mgr->register_creature_script(NPC_ID_ASPECT_THRALL, &AspectAIGeneric::Create); // The Aspect DeathwingMadnessThrall 58232

	mgr->register_hook(SERVER_HOOK_EVENT_ON_AREATRIGGER, (void *)&DeathwingMadnessPlatformAreatriggers);

 	sp = dbcSpell.LookupEntryForced( 106444 ); //impale effect
	//if( sp != NULL )
	{
		sp->eff[1].EffectApplyAuraName = SPELL_AURA_MOD_DAMAGE_TAKEN_PCT; 
		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
	}
 	sp = dbcSpell.LookupEntryForced( SPELL_ID_REGENERATIVE );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
	}
 	sp = dbcSpell.LookupEntryForced( SPELL_ID_CORRUPTING_PARASITE );
	//if( sp != NULL )
	{
		sp->eff[0].Effect = SPELL_EFFECT_TRIGGER_SPELL; 
		sp->eff[0].EffectTriggerSpell = 108601; 
		sp->c_is_flags2 |= SPELL_FLAG2_IS_TRIGGERED_ON_TARGET;

		sp->eff[1].EffectMiscValue = NPC_ID_CORRUPTING_PARASITE;
		sp->eff[1].EffectApplyAuraName = SPELL_AURA_DUMMY;
		sp->c_is_flags2 |= SPELL_FLAG2_IS_MAP_CAST_BOUND;
	}
 	sp = dbcSpell.LookupEntryForced( SPELL_ID_CORRUPTING_PARASITE_BACKLASH );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = sp->eff[0].EffectRadiusMaxIndex;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_UNSTABLE_CORRUPTION );
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMIES_AROUND_CASTER; 
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].EffectRadiusIndex = sp->eff[0].EffectRadiusMaxIndex;

		sp->eff[1].Effect = SPELL_EFFECT_NULL;
	}
	//stun limbs to not rotate after the tank. Sadly this does not work :(
 	sp = dbcSpell.LookupEntryForced( 105451 ); 
	//if( sp != NULL )
	{
		sp->AuraInterruptFlags = 0;
		sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_MOD_ROOT;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_ALEXTRASZA_PRESENCE ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_FRIEND;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_SINGLE_FRIEND;
		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->DurationIndex = 32; //6 seconds
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_CAUTERIZE ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->eff[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
		sp->speed = 0; //takes 300 seconds to reach the target. 5 minutes ? really ?
		sp->c_is_flags2 &= ~SPELL_FLAG2_IS_ENEMY_AOE_TARGETTING;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_NOZDORMU_PRESENCE ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_FRIEND;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->rangeIndex = 13; //anywhere
		sp->DurationIndex = 32; //6 seconds
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_YSERA_PRESENCE ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_FRIEND;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->DurationIndex = 32; //6 seconds
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_ENTER_THE_DREAM ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_FRIEND;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->DurationIndex = 32; //6 seconds
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_KALECGOS_PRESENCE ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_FRIEND;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->DurationIndex = 32; //6 seconds
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_SPELLWEAVING ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_FRIEND;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
		sp->DurationIndex = 32; //6 seconds
		sp->procFlags = PROC_ON_CAST_SPELL | PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
	}
	sp = dbcSpell.LookupEntryForced( 106043 ); //the proc
	//if( sp != NULL )
	{
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT;
		sp->eff[1].EffectImplicitTargetB = EFF_TARGET_NONE;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_REGENERATIVE ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_BLISTERING_HEAT ); 
	//if( sp != NULL )
	{
		sp->eff[1].EffectApplyAuraName = SPELL_AURA_MOD_DAMAGE_TAKEN_PCT;
		sp->eff[1].EffectMechanic = 127;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_CORRUPTED_BLOOD ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
	}
	sp = dbcSpell.LookupEntryForced( SPELL_ID_BURNING_BLOOD ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[0].EffectImplicitTargetB = EFF_TARGET_NONE;
	}
	sp = dbcSpell.LookupEntryForced( 106728 ); 
	//if( sp != NULL )
	{
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_DUMMY;
	}
}