/*
 * OpenarcemuScripts for Openarcemu MMORPG Server
 * Copyright (C) 2008 OpenarcemuScripts Team
 * Copyright (C) 2008 WEmu Team
 * Based on Moon++ Scripts for arcemu MMORPG Server
 * Copyright (C) 2007-2008 Moon++ Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"
#include "EAS/EasyFunctions.h"

class DraeneiFishingNet : public GossipScript
{
public:
	void GossipHello(Object *pObject, Player *pPlayer, bool AutoSend)
	{
		if ( pObject == NULL || pObject->GetTypeId() != TYPEID_ITEM || pPlayer == NULL )
			return;

		Item *pItem = static_cast< Item* >( pObject );
		uint32 SpellId = pItem->GetProto()->Spells.Id[0];
		pPlayer->CastSpell( pPlayer, dbcSpell.LookupEntry( SpellId ), false );

		QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 9452 );
		if ( pQuest == NULL )
			return;

		uint32 Chance = RandomUInt( 10 );
		if ( Chance <= 3 )
		{
			if ( pPlayer->GetMapMgr() == NULL || pPlayer->GetMapMgr()->GetInterface() == NULL )
				return;

			sEAS.SpawnCreature( pPlayer, 17102, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 3 * 60 * 1000 );
			return;
		}

		sEAS.AddItem( 23614, pPlayer );

		pQuest->SendUpdateAddKill( 1 );
		pQuest->UpdatePlayerFields();

		pPlayer->Gossip_Complete();
	}
};

typedef std::pair< uint64, Creature* > QuestDefinition;
typedef std::vector< QuestDefinition > QuestCreature;

class TotemofCoo : public QuestScript 
{ 
public:
	void OnQuestStart(Player *pPlayer, QuestLogEntry *pQuest)
	{
		if ( pPlayer == NULL || pPlayer->GetMapMgr() == NULL || pPlayer->GetMapMgr()->GetInterface() == NULL )
			return;

		Creature *pAkida = sEAS.SpawnCreature( pPlayer, 17379, -4183.043457f, -12511.419922f, 44.361786f, 6.05629f, 0 );
		if ( pAkida == NULL )
			return;

		pAkida->m_escorter = pPlayer;   
		pAkida->GetAIInterface()->setMoveType( RUN );
		pAkida->GetAIInterface()->StopMovement( 1000 );
		pAkida->GetAIInterface()->SetAllowedToEnterCombat( false );
		pAkida->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Follow me I shall result you on a place!" );
		pAkida->SetUInt32Value( UNIT_NPC_FLAGS, 0 );
		pAkida->CastSpell( pAkida, 25035, true ); // Apparition Effect

		sEAS.WaypointCreate( pAkida, -4174.025879f, -12512.800781f, 44.361458f, 2.827430f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -4078.135986f, -12535.500977f, 43.066765f, 5.949394f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -4040.495361f, -12565.537109f, 43.698250f, 5.592041f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -4009.526367f, -12598.929688f, 53.168480f, 5.434962f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -3981.581543f, -12635.541602f, 63.896046f, 5.332861f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -3953.170410f, -12680.391602f, 75.433006f, 5.218981f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -3924.324951f, -12741.846680f, 95.187035f, 5.124734f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -3920.791260f, -12746.218750f, 96.887978f, 3.271200f, 0, 256, 16995 );
		mAkidas.push_back( std::make_pair( pPlayer->GetGUID(), pAkida ) );
	}

	void OnQuestComplete(Player *pPlayer, QuestLogEntry *pQuest)
	{
		uint64 PlayerGuid = pPlayer->GetGUID();
		for ( QuestCreature::iterator itr = mAkidas.begin(); itr != mAkidas.end(); ++itr )
		{
			if ( itr->first == PlayerGuid )
			{
				Creature *pAkida = itr->second;
				if ( pAkida != NULL )							// Can't happen, but whatever :)
				{
					pAkida->CastSpell( pAkida, 30428, true );	// Disparition Effect
					pAkida->Despawn( 5000, 0 );
				}

				itr = mAkidas.erase( itr );
			}
		}
	}

	void OnQuestCancel(Player *pPlayer)
	{
		uint64 PlayerGuid = pPlayer->GetGUID();
		QuestCreature::iterator itr,itr2;
		for ( itr2 = mAkidas.begin(); itr2 != mAkidas.end();  )
		{
			itr = itr2;
			++itr;
			if ( itr->first == PlayerGuid )
			{
				Creature *pAkida = itr->second;
				if ( pAkida != NULL )							// Can't happen, but whatever :)
				{
					pAkida->CastSpell( pAkida, 30428, true );	// Disparition Effect
					pAkida->Despawn( 5000, 0 );
				}

				mAkidas.erase( itr );
			}
		}
	}

	QuestCreature	mAkidas;
};
/*
class TotemofTikti : public QuestScript 
{ 
public:

	void OnQuestStart( Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( !mTarget || !mTarget->GetMapMgr() || !mTarget->GetMapMgr()->GetInterface() )
			return;

		Coo = sEAS.SpawnCreature( mTarget, 17391, -3926.974365f, -12752.285156f, 97.672722f, 4.926801f, 0 );
		if( !Coo )
			return;

		Coo->CastSpell( Coo, 25035, true); // Apparition Effect

		Coo->m_escorter = mTarget;   
		Coo->GetAIInterface()->setMoveType( 11 );
		Coo->GetAIInterface()->StopMovement( 3000 );
		Coo->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Follow me!" );
		Coo->SetUInt32Value( UNIT_NPC_FLAGS, 0 );

		sEAS.CreateCustomWaypointMap( Coo );
		sEAS.WaypointCreate( Coo, -3926.076660f, -12755.158203f, 99.080429f, 5.031188f, 0, 256, 16993 );
		sEAS.WaypointCreate( Coo, -3924.019043f, -12763.895508f, 101.547874f, 5.212689f, 0, 256, 16993 );

		TimedEvent * time1 = TimedEvent::Allocate( this, new CallbackP1<TotemofTikti, Creature*>( this, &TotemofTikti::CooAI1, Coo ), 0, 6000, 1);
		TimedEvent * time2 = TimedEvent::Allocate( this, new CallbackP1<TotemofTikti, Creature*>( this, &TotemofTikti::CooAI2, Coo ), 0, 8750, 1);
		TimedEvent * time3 = TimedEvent::Allocate( this, new CallbackP1<TotemofTikti, Creature*>( this, &TotemofTikti::CooAI3, Coo ), 0, 9000, 1);
		TimedEvent * time4 = TimedEvent::Allocate( this, new CallbackP1<TotemofTikti, Creature*>( this, &TotemofTikti::CooAI4, Coo ), 0, 12750, 1);
		TimedEvent * time5 = TimedEvent::Allocate( this, new CallbackP1<TotemofTikti, Creature*>( this, &TotemofTikti::CooAI5, Coo ), 0, 15200, 1);
		Coo->event_AddEvent( time1 );
		Coo->event_AddEvent( time2 );
		Coo->event_AddEvent( time3 );
		Coo->event_AddEvent( time4 );
		Coo->event_AddEvent( time5 );
	}

	void CooAI1( Creature * Coo )
	{
		Coo->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Ritk kin'chikx azul azure summit..." );
	}

	void CooAI2( Creature * Coo )
	{
		Player * plr = Coo->m_escorter;
		if( plr->CalcDistance( Coo, plr ) <= 10 )
			Coo->CastSpell( plr, 30424, true );
	}

	void CooAI3( Creature * Coo )
	{
		Coo->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Coo xip fly... Jump ilos river. Find Tikti." );
	}

	void CooAI4( Creature * Akida )
	{
		Coo->CastSpell( Akida, 30473, true);
	}

	void CooAI5( Creature * Akida )
	{
		Coo->Despawn( 0, 0 );
	}

private:
	Creature * Coo;
};

class TotemofYor : public QuestScript 
{ 
public:

	void OnQuestStart( Player * mTarget, QuestLogEntry * qLogEntry )
	{
		if( !mTarget || !mTarget->GetMapMgr() || !mTarget->GetMapMgr()->GetInterface() )
			return;

		Tikti = sEAS.SpawnCreature( mTarget, 17392, -3875.430664f, -13125.011719f, 6.822148f, 2.020735f, 0 );
		if( !Tikti )
			return;

		mTarget->CastSpell( Tikti, 25035, true); // Apparition Effect

		Tikti->m_escorter = mTarget;   
		Tikti->GetAIInterface()->setMoveType( WALK );
		Tikti->GetAIInterface()->StopMovement( 3000 );
		Tikti->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Follow me!" );
		Tikti->SetUInt32Value( UNIT_NPC_FLAGS, 0 );

		sEAS.CreateCustomWaypointMap( Tikti );
		sEAS.WaypointCreate( Tikti, -3881.700928f, -13111.898438f, 5.814010f, 1.855801f, 0, 256, 16999 );
		sEAS.WaypointCreate( Tikti, -3886.341553f, -13098.914063f, 3.964841f, 1.855801f, 0, 256, 16999 );

		TimedEvent * time1 = TimedEvent::Allocate( this, new CallbackP1<TotemofYor, Creature*>( this, &TotemofYor::TiktiAI1, Tikti ), 0, 15000, 1);
		TimedEvent * time2 = TimedEvent::Allocate( this, new CallbackP1<TotemofYor, Creature*>( this, &TotemofYor::TiktiAI2, Tikti ), 0, 18000, 1);
		TimedEvent * time3 = TimedEvent::Allocate( this, new CallbackP1<TotemofYor, Creature*>( this, &TotemofYor::TiktiAI3, Tikti ), 0, 20000, 1);
		TimedEvent * time4 = TimedEvent::Allocate( this, new CallbackP1<TotemofYor, Creature*>( this, &TotemofYor::TiktiAI4, Tikti ), 0, 24000, 1);
		Tikti->event_AddEvent( time1 );
		Tikti->event_AddEvent( time2 );
		Tikti->event_AddEvent( time3 );
		Tikti->event_AddEvent( time4 );
	}

	void TiktiAI1(Creature * Tikti)
	{
		Tikti->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "[Furbolg] Far you mixik tak come. Gaze upon the kitch'kal river. Follow. Ilog to Yor." );
	}

	void TiktiAI2(Creature * Tikti)
	{
		Player * plr = Tikti->m_escorter;
		if( plr->CalcDistance( Tikti, plr ) <= 10 )
			Tikti->CastSpell( plr, 30430, true );
	}

	void TiktiAI3(Creature * Tikti)
	{
		Tikti->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "[Furbolg] Go... Search yitix'kil bottom river. South!" );
		Tikti->CastSpell( Tikti, 30431, true );
	}

	void TiktiAI4(Creature * Tikti)
	{
		Tikti->Despawn( 0, 0 );
	}

private:
	Creature * Tikti;
}; 

class TotemofVark : public QuestScript 
{ 
public:

	void OnQuestStart(Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( !mTarget || !mTarget->GetMapMgr() || !mTarget->GetMapMgr()->GetInterface() )
			return;

		Yor = sEAS.SpawnCreature( mTarget, 17393, -4634.246582f, -13071.686523f, -14.755350f, 1.569997f, 0 );
		if( !Yor )
			return;
	
		Yor->CastSpell( Yor, 25035, true ); // Apparition Effect

		Yor->m_escorter = mTarget;   
		Yor->GetAIInterface()->StopMovement( 1000 );
		Yor->SetUInt32Value( UNIT_NPC_FLAGS, 0 );
		char msg[256];
		snprintf((char*)msg, 256, "Come, %s . Let us leave the water together, purified.", mTarget->GetName() );
		Yor->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg );

		sEAS.CreateCustomWaypointMap( Yor );
		sEAS.WaypointCreate( Yor, -4650.081055f, -13016.692383f, 1.776296f, 2.021601f, 0, 256, 16393 );
		sEAS.WaypointCreate( Yor, -3886.341553f, -13098.914063f, 3.964841f, 1.855801f, 1000, 256, 16393 );
		sEAS.WaypointCreate( Yor, -4677.421387f, -12983.874023f, 0.833827f, 2.335760f, 0, 256, 16393 ); // Should look player

		TimedEvent * time1 = TimedEvent::Allocate( this, new CallbackP1<TotemofVark, Creature*>( this, &TotemofVark::YorAI1, Yor ), 0, 15500, 1);
		TimedEvent * time2 = TimedEvent::Allocate( this, new CallbackP1<TotemofVark, Creature*>( this, &TotemofVark::YorAI2, Yor ), 0, 26500, 1);
		TimedEvent * time3 = TimedEvent::Allocate( this, new CallbackP1<TotemofVark, Creature*>( this, &TotemofVark::YorAI3, Yor ), 0, 30000, 1);
		TimedEvent * time4 = TimedEvent::Allocate( this, new CallbackP1<TotemofVark, Creature*>( this, &TotemofVark::YorAI4, Yor ), 0, 31000, 1);
		Yor->event_AddEvent( time1 );
		Yor->event_AddEvent( time2 );
		Yor->event_AddEvent( time3 );
		Yor->event_AddEvent( time4 );
	}

	void YorAI1( Creature * Yor )
	{
		Yor->CastSpell( Yor, 30446, true ); // Change to Stillpine form
	}

	void YorAI2( Creature * Yor )
	{
		Yor->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "[Furbolg] We go now, together. We will seek Vark." );
	}

	void YorAI3( Creature * Yor )
	{
		Yor->CastSpell( Yor, 30448, true ); // Change to nightsaber form
	}

	void YorAI4( Creature * Yor )
	{
		Yor->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "[Furbolg] Follow me to Vark." );
		Player * plr = Yor->m_escorter;
		if( plr->CalcDistance( Yor, plr ) <= 10 )
			Yor->CastSpell( plr, 30448, true ); // Change to nightsaber form
	}
	
	// NEED TO MAKE THE PATH TO THE TOTEM OF VARK
private:
	Creature * Yor;
}; */

// Chieftain Oomooroo

class ChieftainOomoorooQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ChieftainOomoorooQAI);
    ChieftainOomoorooQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if ( mKiller->IsPlayer() )
		{
			QuestLogEntry *pQuest = static_cast< Player* >( mKiller )->GetQuestLogForEntry( 9573 );
			if ( pQuest != NULL && pQuest->GetMobCount( 1 ) < pQuest->GetQuest()->required_mobcount[1] )
			{
				pQuest->SetMobCount( 1, pQuest->GetMobCount( 1 ) + 1 );
				pQuest->SendUpdateAddKill( 1 );
				pQuest->UpdatePlayerFields();
			}
		}
	}
};

// Healing The Lake

bool HealingTheLake(uint32 i, Spell* pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;

	Player *pPlayer = static_cast< Player* >( pSpell->u_caster );

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 9294 );
	if ( pQuest == NULL )
		return true;

	if ( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
	{
		pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
		pQuest->SendUpdateAddKill( 0 );
		pQuest->UpdatePlayerFields();
		
		return true;
	}
	
	return true;
}

void SetupAzuremystIsle( ScriptMgr * mgr )
{
	mgr->register_item_gossip_script( 23654, CREATE_GOSSIPSCRIPT( DraeneiFishingNet ) );
//	mgr->register_quest_script( 9539, CREATE_QUESTSCRIPT( TotemofCoo ) );
	/*mgr->register_quest_script( 9540, CREATE_QUESTSCRIPT( TotemofTikti ));
	mgr->register_quest_script( 9541, CREATE_QUESTSCRIPT( TotemofYor ) );
	mgr->register_quest_script( 9542, CREATE_QUESTSCRIPT( TotemofVark ) );*/
	mgr->register_creature_script(17189, &ChieftainOomoorooQAI::Create);
	mgr->register_dummy_spell(28700, &HealingTheLake);
}
