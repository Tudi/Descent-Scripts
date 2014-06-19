/*
 * Sun++ Scripts 
 * Copyright (C) 2008 Sun++ Team <http://sunplusplus.info/>
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

// War is Hell
bool WarIsHell(uint32 i, Spell* pSpell)
{
	if( !pSpell->u_caster->IsPlayer() )
		return true;

	Player *plr = (Player*)pSpell->u_caster;
	if( plr == NULL )
		return true;

	Creature *target = (Creature*)plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( plr->GetPositionX(), plr->GetPositionY() , plr->GetPositionZ(), 24008 );
	if( target == NULL )
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(11270);
  
	if( qle == NULL )
		return true;
 
	GameObject *obj = NULL;
 
	if( qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0] )
	{
		qle->SetMobCount( 0, qle->GetMobCount( 0 ) + 1 );
		qle->SendUpdateAddKill( 0 );
		
		obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1);
		sEAS.GameobjectDelete(obj, 1*30*1000);
	}
	target->Despawn( 2000, 60*1000 );
	plr->UpdateNearbyGameObjects();
	qle->UpdatePlayerFields();
	return true;
}

// A Lesson in Fear
bool PlantForsakenBanner(uint32 i, Spell* pSpell)
{
	if(pSpell->u_caster->IsPlayer() == false)
		return true;

	Player *pPlayer = static_cast<Player*>(pSpell->u_caster);
	if( pPlayer == NULL )
		return true;

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry(11282);
	if( pQuest == NULL )
		return true;
	
	Creature*target = static_cast<Creature*>(pSpell->GetUnitTarget());
	if (target == NULL)
		return true;

	float X = target->GetPositionX();
	float Y = target->GetPositionY();
	float Z = target->GetPositionZ();

	uint32 cit = target->GetEntry();
	
	switch(cit)
	{
	case 24161:
		{
			if( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[ 0 ] )
			{
				uint32 newcount = pQuest->GetMobCount( 0 ) + 1;
				pQuest->SetMobCount( 0, newcount );
				pQuest->SendUpdateAddKill( 0 );
				pQuest->UpdatePlayerFields();
				target->Despawn(0, 3*60*1000);
			}	
		}
		break;
	case 24016:
		{
			if( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[ 1 ] )
			{
				uint32 newcount = pQuest->GetMobCount( 0 ) + 1;
				pQuest->SetMobCount( 0, newcount );
				pQuest->SendUpdateAddKill( 0 );
				pQuest->UpdatePlayerFields();
				target->Despawn(0, 3*60*1000);
			}	
		}
		break;
	case 24162:
		{
			if( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[ 2 ] )
			{
				uint32 newcount = pQuest->GetMobCount( 0 ) + 1;
				pQuest->SetMobCount( 0, newcount );
				pQuest->SendUpdateAddKill( 0 );
				pQuest->UpdatePlayerFields();
				target->Despawn(0, 3*60*1000);
			}	
		}
		break;
  }
  return true;
}

void SetupHowlingFjord(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(42793, &WarIsHell); 
	//mgr->register_dummy_spell(43178, &PlantForsakenBanner);
}
