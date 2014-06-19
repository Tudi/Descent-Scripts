#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"

bool HandleTournametSpectatorAura(uint32 i, Aura* pAura, bool apply)
{
	Unit *target = pAura->GetTarget();

	if( target == NULL )
		return true;

	if( apply ) 
	{
		if( target->IsPlayer() )
		{		
			Player *ptarget = (Player *)target;
			//remove auras to make sure they will not influence other players
			for(uint32 buffitr=0;buffitr<MAX_AURAS;buffitr++)
				if( ptarget->m_auras[ buffitr ] )
					ptarget->m_auras[ buffitr ]->Remove();

			//if he is dead then revive him
			if( ptarget->IsDead() )
				ptarget->ResurrectPlayer();

			//full health
			ptarget->SetUInt32Value(UNIT_FIELD_HEALTH, ptarget->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );

			//add flags so player can not move cast spell or do anything
			ptarget->SetUInt32Value( UNIT_FIELD_FLAGS, ptarget->GetUInt32Value( UNIT_FIELD_FLAGS ) | SET_SPECTATOR_UNIT_FLAGS );

			//make sure he could not find a way to escape lock
			ptarget->m_silenced++;
			ptarget->m_rooted++;

			//make them small to not disturb the fight
			ptarget->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f );

			//make them imune in case someone decides to attack them
			for(uint32 i=0;i<7;i++)
				ptarget->SchoolImmunityAntiEnemy[i]++;
			for(uint32 i=0;i<31;i++)
				ptarget->MechanicsDispels[i]++;

			//remove PVP just in case
			ptarget->RemovePvPFlag();
			ptarget->RemoveFFAPvPFlag();

			//make sure he understands the rulez
			ptarget->BroadcastMessage( "You are a spectator now. Please try to not disturb others. Leave the ring and after a while aura will expire" );
		}
		else if( target->GetEntry() != CREATURE_ENTRY_FOR_IGTS_SUPERVIZOR )
		{
			target->SetUInt32Value(UNIT_FIELD_HEALTH, 1);
			target->DealDamage( target, 1000, SPECTATOR_AURA_ID );
		}
	}
	//remove aura
	else
	{
		if( target->IsPlayer() )
		{		
			Player *ptarget = (Player *)target;

			//if he is dead then revive him
			if( ptarget->IsDead() )
				ptarget->ResurrectPlayer();

			//add flags so player can not move cast spell or do anything
			ptarget->SetUInt32Value( UNIT_FIELD_FLAGS, ptarget->GetUInt32Value( UNIT_FIELD_FLAGS ) & ~SET_SPECTATOR_UNIT_FLAGS );

			//make sure he could not find a way to escape lock
			if( ptarget->m_silenced > 0 )
				ptarget->m_silenced--;
			if( ptarget->m_rooted > 0 )
				ptarget->m_rooted--;

			//remove mute in case he still has it 
			if( ptarget->GetSession() )
				ptarget->GetSession()->m_muted = 0; 

			//normal penis size
			ptarget->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f );

			//remove imunity
			for(uint32 i=0;i<7;i++)
				ptarget->SchoolImmunityAntiEnemy[i] = 0;
			for(uint32 i=0;i<31;i++)
				ptarget->MechanicsDispels[i] = 0;

			//remove PVP just in case
			ptarget->RemovePvPFlag();
			ptarget->RemoveFFAPvPFlag();
			//forced remove
			ptarget->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
			ptarget->RemoveByteFlag(UNIT_FIELD_BYTES_2,	1, UNIT_BYTE2_FLAG_PVP);
			ptarget->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
			ptarget->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_FREE_FOR_ALL_PVP);

			//some get stucked as rooted
			ptarget->Unroot();
		}
	}
	return true;
}

void CreateTournametSpectatorAura(ScriptMgr * mgr)
{
	//overwrite our spell to become a spell that we like
	SpellEntry *sp = dbcSpell.LookupEntryForced( SPECTATOR_AURA_ID );
	if( sp != NULL)
	{
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;	//self cast as proc to avoid shields and resists
		sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA; 
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_DUMMY; 
		sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		sp->DurationIndex = 3; //1 minute

		sp->eff[1].Effect = SPELL_EFFECT_NULL; 
		sp->eff[2].Effect = SPELL_EFFECT_NULL; 

		sLog.outDebug("Tournament Maker : overwriting spell %u\n",SPECTATOR_AURA_ID);
	}
	//register our dummy handler
	mgr->register_dummy_aura(SPECTATOR_AURA_ID, &HandleTournametSpectatorAura);
}
