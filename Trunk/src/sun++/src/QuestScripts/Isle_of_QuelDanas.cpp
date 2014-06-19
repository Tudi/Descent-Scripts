/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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



/*--------------------------------------------------------------------------------------------------------*/
// Crisis at the Sunwell

class ScryingOrb : public GameObjectAIScript
{
public:
	ScryingOrb(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new ScryingOrb(GO); }

	void OnActivate(Player * pPlayer)
	{
		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(11490);
  		if(qle)
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			
			GameObject* Orb = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( SSX, SSY, SSZ, 187578);
			if (Orb)
			{
				Orb->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
  				qle->SetMobCount(0, 1);
  				qle->SendUpdateAddKill(0);
  				qle->UpdatePlayerFields();
			}
			return;
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required quest : The Scryer's Scryer");
		}
	}
};



/*--------------------------------------------------------------------------------------------------------*/
// Erratic Behavior

bool ConvertingSentry(uint32 i, Spell* pSpell)
{
  Player *caster = pSpell->p_caster;
  if(caster == NULL)
    return true;

  Creature *target = static_cast<Creature*>(pSpell->GetUnitTarget());
  if(target == NULL)
    return true;

  // Erratic Sentry: 24972
  if(target->GetEntry() != 24972)
    return true;

  if(target->isAlive())
    return true;

  QuestLogEntry *qle = NULL;
  qle = caster->GetQuestLogForEntry(11525);
  if(qle == NULL)
  {
	qle = caster->GetQuestLogForEntry(11524);
	if(qle == NULL)
	{
       return true;
    }
  }

  if(qle->GetMobCount(0) == qle->GetQuest()->required_mobcount[0])
    return true;

  qle->SetMobCount(0, qle->GetMobCount(0)+1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();

  target->Despawn(500, 2*60*1000);

  return true;
}

bool OrbOfMurlocControl(uint32 i, Spell* pSpell)
{
	if(pSpell->m_caster->IsPlayer() == false)
		return true;

	Player *Plr = (Player*)pSpell->u_caster;

	QuestLogEntry *pQuest = Plr->GetQuestLogForEntry(11541);
	if(pQuest == NULL)
		return true;

	Unit *mTarget;
	Creature *cTarget;

	InrangeLoopExitAutoCallback AutoLock;
	for(InRangeSetRecProt::iterator itr = pSpell->m_caster->GetInRangeSetBegin( AutoLock ); itr != pSpell->m_caster->GetInRangeSetEnd(); ++itr)
	{
		if((*itr)->IsUnit())
			mTarget = (Unit*)*itr;
		else
			continue;

		if(pSpell->m_caster->CalcDistance(mTarget) > 5 )
			continue;

		cTarget = static_cast<Creature*>(mTarget);

		if(cTarget->GetEntry() == 25084)
		{
		  if(pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0])
		  {
			pQuest->SetMobCount(0, pQuest->GetMobCount(0) + 1);
			pQuest->SendUpdateAddKill(0);		
			Creature *FreedGreengill = sEAS.SpawnCreature(Plr, 25085, cTarget->GetPositionX(), cTarget->GetPositionY(), cTarget->GetPositionZ(), cTarget->GetOrientation(), 0);
			FreedGreengill->Despawn(6*60*1000, 0);
			cTarget->Despawn(0, 6*60*1000);
			pQuest->UpdatePlayerFields();
			return true;
		  }  
		}
  }
  return true;
}

#define GO_FIRE 183816

struct Coords
{
	float x;
	float y;
	float z;
	float o;
};
static Coords BloodoathFire[] =
{
	{13329.4f, -6994.70f, 14.5219f, 1.38938f},
 	{13315.4f, -6990.72f, 14.7647f, 1.25979f}
};
static Coords SinlorenFire[] =
{
	{13214.3f, -7059.19f, 17.5717f, 1.58573f},
	{13204.2f, -7059.38f, 17.5717f, 1.57787f}
};
static Coords DawnchaserFire[] =
{
	{13284.1f, -7152.65f, 15.9774f, 1.44828f},
	{13273.0f, -7151.21f, 15.9774f, 1.39723f}
};

bool ShipBombing(uint32 i, Spell* pSpell)
{
	Player *plr = (Player*)pSpell->u_caster;
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = NULL;
	qle = plr->GetQuestLogForEntry(11542);
	if(qle == NULL)
	{
		qle = plr->GetQuestLogForEntry(11543);
		if(qle == NULL)
		{
			return true;
		}
	}
	
	GameObject *Sinloren = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(13200.232422, -7049.176270, 3.838517, 550000);
	GameObject *Bloodoath = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(13319.419922, -6988.779785, 4.002993, 550000);
	GameObject *Dawnchaser = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(13274.51625, -7145.434570, 4.770292, 550000);
	
	GameObject *obj = NULL;

	if(Sinloren != NULL)
	{
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			if(plr->CalcDistance(plr, Sinloren) < 15)
			{
				qle->SetMobCount(0, qle->GetMobCount(0)+1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();
				for(uint8 i = 0; i < 2; i++)
				{
					obj = sEAS.SpawnGameobject(plr, GO_FIRE, SinlorenFire[i].x, SinlorenFire[i].y, SinlorenFire[i].z, SinlorenFire[i].o, 1);
					sEAS.GameobjectDelete(obj, 2*60*1000);
				}
			}
		}
	}
	if(Bloodoath != NULL)
	{
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			if(plr->CalcDistance(plr, Bloodoath) < 15)
			{
				qle->SetMobCount(1, qle->GetMobCount(1)+1);
				qle->SendUpdateAddKill(1);
				qle->UpdatePlayerFields();
				for(uint8 i = 0; i < 2; i++)
				{
					obj = sEAS.SpawnGameobject(plr, GO_FIRE, BloodoathFire[i].x, BloodoathFire[i].y, BloodoathFire[i].z, BloodoathFire[i].o, 1);
					sEAS.GameobjectDelete(obj, 2*60*1000);
				}
			}
		}
	}
	if(Dawnchaser != NULL)
	{
		if(plr->CalcDistance(plr, Dawnchaser) < 15)
		{
			if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
			{
				qle->SetMobCount(2, qle->GetMobCount(2)+1);
				qle->SendUpdateAddKill(2);
				qle->UpdatePlayerFields();
				for(uint8 i = 0; i < 2; i++)
				{
					obj = sEAS.SpawnGameobject(plr, GO_FIRE, DawnchaserFire[i].x, DawnchaserFire[i].y, DawnchaserFire[i].z, DawnchaserFire[i].o, 1);
					sEAS.GameobjectDelete(obj, 2*60*1000);
				}
			}
		}
	}
	return true;
}

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
    Menu->SendTo(Plr);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Ayren Cloudbreaker Gossip

class SCRIPT_DECL AyrenCloudbreaker_Gossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 12252, Plr);
		if (Plr->GetQuestLogForEntry(11532) || Plr->GetQuestLogForEntry(11533))
			Menu->AddItem( 0, "Speaking of action, I've been ordered to undertake an air strike.", 1);
		if (Plr->GetQuestLogForEntry(11543) || Plr->GetQuestLogForEntry(11542))
			Menu->AddItem( 0, "I need to intercept the Dawnblade reinforcements.", 2);
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
		case 1:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 779 );
				Plr->TaxiStart( path, 22840, 0 );
				Plr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
			}
			break;
		case 2:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 784 );
				Plr->TaxiStart( path, 22840, 0 );
				Plr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
			}
			break;
		}
	}
	void Destroy()
	{
		delete this;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Unrestrained Dragonhawk Gossip

class SCRIPT_DECL UnrestrainedDragonhawk_Gossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 12371, Plr);
		if (Plr->GetQuestLogForEntry(11543) || Plr->GetQuestLogForEntry(11542))
			Menu->AddItem( 0, "<Ride the dragonhawk to Sun's Reach>", 1);
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
		case 1:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 788 );
				Plr->TaxiStart( path, 22840, 0 );
				Plr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
			}
			break;
		}
	}
	void Destroy()
	{
		delete this;
	}
};

// The Battle for the Sun's Reach Armory
class TheBattleForTheSunReachArmory : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TheBattleForTheSunReachArmory);
    TheBattleForTheSunReachArmory(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if (mKiller->IsPlayer())
		{
			QuestLogEntry *qle = ((Player*)mKiller)->GetQuestLogForEntry(11537);
			if( qle == NULL )
			{
				qle = ((Player*)mKiller)->GetQuestLogForEntry(11538);
				if( qle == NULL )
					return;
			}

			if( qle->GetMobCount( 1 ) < qle->GetQuest()->required_mobcount[ 1 ] )
			{
				uint32 newcount = qle->GetMobCount( 1 ) + 1;
				qle->SetMobCount( 1, newcount );
				qle->SendUpdateAddKill( 1 );
				qle->UpdatePlayerFields();
				return;
			}
		}
	}
};

bool ImpaleEmissary(uint32 i, Spell* pSpell)
{
	if(pSpell->u_caster->IsPlayer() == false)
	return true;

	Player *pPlayer = static_cast<Player*>(pSpell->u_caster);
	if( pPlayer == NULL )
		return true;

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry(11537);
	if( pQuest == NULL )
	{
		pQuest = pPlayer->GetQuestLogForEntry(11538);
		if( pQuest == NULL )
			return true;
	}
	
	Creature *mEmissary = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 25003);
	if( mEmissary == NULL )
		return true;
		
	if( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[ 0 ] )
	{
		uint32 newcount = pQuest->GetMobCount( 0 ) + 1;
		pQuest->SetMobCount( 0, newcount );
		pQuest->SendUpdateAddKill( 0 );
		pQuest->UpdatePlayerFields();
		mEmissary->Despawn(0, 3*60*1000);
	}
	return true;
}

void SetupIsleOfQuelDanas(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(187578, &ScryingOrb::Create);

	mgr->register_dummy_spell(45109, &OrbOfMurlocControl);
	mgr->register_dummy_spell(44997, &ConvertingSentry);
	mgr->register_dummy_spell(45115, &ShipBombing);
	
	mgr->register_creature_script(24999, &TheBattleForTheSunReachArmory::Create);
	mgr->register_creature_script(25001, &TheBattleForTheSunReachArmory::Create);
	mgr->register_creature_script(25002, &TheBattleForTheSunReachArmory::Create);

	mgr->register_dummy_spell(45030, &ImpaleEmissary);

	//GOSSIP
	GossipScript * AyrenCloudbreakerGossip = (GossipScript*) new AyrenCloudbreaker_Gossip;
	mgr->register_gossip_script(25059, AyrenCloudbreakerGossip);
	GossipScript * UnrestrainedDragonhawkGossip = (GossipScript*) new UnrestrainedDragonhawk_Gossip;
	mgr->register_gossip_script(25236, UnrestrainedDragonhawkGossip);
}
