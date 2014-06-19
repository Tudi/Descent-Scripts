/*
 * WEmu Scripts for WEmu MMORPG Server
 * Copyright (C) 2008 WEmu Team
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
#include "../EAS/EasyFunctions.h"


#include "StdAfx.h"
#include "Setup.h"
#include "../EAS/EasyFunctions.h"

class SCRIPT_DECL Lady_Jaina : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(plr->GetQuestLogForEntry(558))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7012, plr);
			Menu->AddItem(0, "I know this is rather silly but i have a young ward who is a bit shy and would like your autograph.", 1);
			Menu->SendTo(plr);
		}
	}

	void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pObject==NULL)
			return;

		switch(IntId)
		{
		case 0: // Return to start
			GossipHello(pCreature, plr, true);
			break;
		case 1: // Give Item
			{
				plr->CastSpell(plr, dbcSpell.LookupEntry(23122),true);
				plr->Gossip_Complete();
				break;
			}
			break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class SCRIPT_DECL Cairne : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(plr->GetQuestLogForEntry(925))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7013, plr);
			Menu->AddItem(0, "Give me hoofprint.", 1);
			Menu->SendTo(plr);
		}
	}

	void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		GossipMenu *Menu;
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pObject==NULL)
			return;

		switch(IntId)
		{
		case 0: // Return to start
			GossipHello(pCreature, plr, true);
			break;
		case 1: // Give Item
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7014, plr);
				Menu->SendTo(plr);
				plr->CastSpell(plr, dbcSpell.LookupEntry(23123),true);
				break;
			}
			break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class SCRIPT_DECL NoXPGain : public GossipScript
{
public:

	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60150, plr);
		if( plr->HasFlag( PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN ) )
			Menu->AddMenuItem(0, "Enable XP Gain.", 0,0, "You will be able to level again",0,false,1);
		else
			Menu->AddMenuItem(0, "Disable XP Gain.", 0,0, "You will NOT be able to level until manual enable",0,false,2);
		Menu->AddItem(0, "Close", 3 );
		Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pObject==NULL)
			return;

		switch(IntId)
		{
		case 0: // Return to start
			GossipHello(pCreature, plr, true);
			break;
		case 1: 
			{
				plr->RemoveFlag( PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN );
				plr->CloseGossip();
				break;
			}break;
		case 2: 
			{
				plr->SetFlag( PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN );
				plr->CloseGossip();
				break;
			}break;
		default:
			plr->CloseGossip();
			break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class SCRIPT_DECL BGRealmDescribe : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 68, plr);
		Menu->AddItem( 0, "All you can do on this realm is PVP.", 1);
		Menu->AddItem( 0, "Next", 2);
		Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pObject==NULL)
			return;

		switch(IntId)
		{
		case 0: // Return to start
			GossipHello(pCreature, plr, true);
			break;
		case 1: 
		case 2: 
			{
				GossipMenu *Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 68, plr);
				Menu->AddItem( 0, "Join Battlegrounds or Arenas to gain XP or gold.", 3);
				Menu->AddItem( 0, "Next", 4);
				Menu->SendTo(plr);
			}break;
		case 3: 
		case 4: 
			{
				GossipMenu *Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 68, plr);
				Menu->AddItem( 0, "Outside of this building there are vendors and trainer. You can spend your gold there", 5);
				Menu->AddItem( 0, "Next", 6);
				Menu->SendTo(plr);
			}break;
		case 5: 
		case 6: 
			{
				GossipMenu *Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 68, plr);
				Menu->AddItem( 0, "There are no raids or spawns apart from this area. Try to enjoy PVP at every level", 7);
				Menu->AddItem( 0, "Next", 8);
				Menu->SendTo(plr);
			}break;
		case 7: 
		case 8: 
			{
				GossipMenu *Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 68, plr);
				Menu->AddItem( 0, "You can disable your XP gain if you enjoy a specific level group or you feel undergeared to progress", 9);
				Menu->AddItem( 0, "Next", 10);
				Menu->SendTo(plr);
			}break;
		case 9: 
		case 10: 
			{
				GossipMenu *Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 68, plr);
				Menu->AddItem( 0, "You earn gold for participating in a kill. You get extra gold if you make the killer blow. You get some gold when you get ganked.", 11);
				Menu->AddItem( 0, "Next", 12);
				Menu->SendTo(plr);
			}break;
		case 11: 
		case 12: 
			{
				GossipMenu *Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 68, plr);
				Menu->AddItem( 0, "Gold is continuesly earned while inside BG(invisible). But it only gets added to you if you make a kill. Sometimes you will earn more gold then other times.", 13);
				Menu->AddItem( 0, "Next", 14);
				Menu->SendTo(plr);
			}break;
		case 13: 
		case 14: 
			{
				GossipMenu *Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 68, plr);
				Menu->AddItem( 0, "To avoid farming exploit. Your gold does not get updated only every few minutes even if you continuesly make killing blows.", 15);
				Menu->AddItem( 0, "Next", 16);
				Menu->SendTo(plr);
			}break;
		case 15: 
		case 16: 
			{
				GossipMenu *Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 68, plr);
				Menu->AddItem( 0, "Try to have. It's all about that.", 13);
				Menu->AddItem( 0, "Next", 14);
				Menu->SendTo(plr);
			}break;
		default:
				plr->Gossip_Complete();
			break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

void SetupQuestGossip(ScriptMgr * mgr)
{
	GossipScript * LJ = (GossipScript*) new Lady_Jaina();
	GossipScript * CB = (GossipScript*) new Cairne();

	mgr->register_gossip_script(4968, LJ);
	mgr->register_gossip_script(3057, CB);
/*
INSERT INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `hover_height`, `modImmunities`, `LootId`, `PickpocketingId`, `skinId`, `heroic_entry1`, `heroic_entry2`, `heroic_entry3`) VALUES
(100166, 85, 85, 35, 1337, 1337, 1, 1.2, 3, 1500, 0, 41, 61, 500, 0, 0, 900000, 0, 0, 0, 0, 0, 0, 0, 1.5, 0.306, '', 0, 0, 0, 0, 2.5, 10.24, 13, 0, 1, 0, 240, 0, 0, 0, 0, 0);
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `unk420`, `type`, `family`, `rank`, `CreatureRelation1`, `CreatureRelation2`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `hp_modifier`, `mana_modifier`, `leader`, `RelatedItemId1`, `RelatedItemId2`, `RelatedItemId3`, `RelatedItemId4`, `RelatedItemId5`, `RelatedItemId6`, `MovementInfo`, `unk403`) VALUES
(100166, 'XP Gain Disabler', 'Srsly it disables XP gain', '', 0, 0, 7, 0, 0, 0, 0, 1297, 0, 0, 0, 1.02, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
*/
	GossipScript * noXP = (GossipScript*) new NoXPGain();
	mgr->register_gossip_script(100166, noXP);
	mgr->register_gossip_script(35364, noXP);
	mgr->register_gossip_script(35365, noXP);

	GossipScript *BGrealmDetails = (GossipScript*) new BGRealmDescribe();
	mgr->register_gossip_script(100167, BGrealmDetails);
}
