// Made bij Tegno alias Mordryon.
// free to use, only leave my mane in the script.

#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305)		// warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

class SCRIPT_DECL DalaranGuard : public GossipScript
{
public:
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
    void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
    void GossipEnd(Object * pObject, Player* Plr);
	void Destroy()
	{
		delete this;
	}
};
void DalaranGuard::GossipHello(Object* pObject, Player* Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
  		Menu->AddItem(0, "Arena", 2);
		Menu->AddItem(0, "Auction House", 3);
		Menu->AddItem(0, "Bank", 4);
		Menu->AddItem(0, "Barber", 5);
		Menu->AddItem(0, "Battlemasters", 6);
		Menu->AddItem(0, "Capital Portals", 7);
		Menu->AddItem(0, "Flight Master", 8);
		Menu->AddItem(0, "Guild Master", 9);
		Menu->AddItem(0, "Inn", 10);
		Menu->AddItem(0, "Mailbox", 11);
		Menu->AddItem(0, "Points of Interest", 12);
		Menu->AddItem(0, "Stable Master", 13);
		Menu->AddItem(0, "Trainers", 14);
		Menu->AddItem(0, "Vendors", 15);
        if(AutoSend)
            Menu->SendTo(Plr);
    };

void DalaranGuard::GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
		{
      //  case 0:     // Return to start
      //      GossipHello(pObject, Plr, true);
      //      break;

//////////////////////
// Menus
/////


//arena
case 2:
	{
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
		Menu->AddItem(3, "Dalaran's arena and those who service it are found beneath the city, in the sewers.", 2);
		Menu->AddItem(3, "       ", 2);
		Menu->AddItem(3, "There are at least three ways in: Entrances in the east and west, and a well in the north.", 2);
		Menu->AddItem(3, "       ", 2);
		Menu->AddItem(0, "Eastern Sewer Entrance", 18);
		Menu->AddItem(0, "Western Sewer Entrance", 19);
		Menu->AddItem(0, "Well Entrance", 20);
		Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

case 18:
	{
Plr->Gossip_SendPOI(5801.77, 551.07, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 19:
	{
Plr->Gossip_SendPOI(5815.21, 762.65, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 20:
	{
Plr->Gossip_SendPOI(5791.60, 561.82, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//auction house
case 3:
	{
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(3, "Dalaran has no auction house of its own. you must take a portal back to one of your capitals.", 3);
Menu->AddItem(3, "       ", 3);
Menu->AddItem(3, "They can be found in Dalaran's Alliance and Horde quarters: the Silver Enclave and the Sunreaver's Sanctuary.", 3);
Menu->AddItem(3, "       ", 3);
Menu->AddItem(0, "The Alliance Quarter", 22);
Menu->AddItem(0, "The Horde Quarter", 23);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

case 22:
	{
Plr->Gossip_SendPOI(5763.20, 711.77, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 23:
	{
Plr->Gossip_SendPOI(5860.35, 592.97, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//bank
case 4:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(3, "There are three banks in the city: The Bank of Dalaran in the north, the Dalaran Merchant's Bank in the south, and one more beneath the city, in the sewers.", 4);
Menu->AddItem(3, "       ", 4);
Menu->AddItem(0, "Northern Bank", 25);
Menu->AddItem(0, "Southern Bank", 26);
Menu->AddItem(0, "Sewers", 27);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

case 25:
	{
Plr->Gossip_SendPOI(5979.58, 608.16, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 26:
	{
Plr->Gossip_SendPOI(5631.20, 694.15, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 27:
	{
Plr->Gossip_SendPOI(5766.31, 731.66, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//barber
case 5:
	{
Plr->Gossip_SendPOI(5889.67, 621.21, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//battlemasters
case 6:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(3, "Portals to the various capitals and battlegrounds can be found in Dalaran's Alliance and Horde quarters: the Silver Enclave and the Sunreaver's Sanctuary.", 6);
Menu->AddItem(3, "       ", 6);
Menu->AddItem(0, "The Alliance Quarter", 22);
Menu->AddItem(0, "The Horde Quarter", 23);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

//capital portals
case 7:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(3, "Portals to the various capitals and battlegrounds can be found in Dalaran's Alliance and Horde quarters: the Silver Enclave and the Sunreaver's Sanctuary.", 7);
Menu->AddItem(3, "       ", 7);
Menu->AddItem(0, "The Alliance Quarter", 22);
Menu->AddItem(0, "The Horde Quarter", 23);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

//flight master
case 8:
	{
Plr->Gossip_SendPOI(5815.91, 453.64, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//guild master
case 9:
	{
Plr->Gossip_SendPOI(5762.21, 622.84, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//inn
case 10:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(3, "The most popular inn in Dalaran is the Legerdemain Lounge, just north of the city,s center.", 10);
Menu->AddItem(3, "       ", 10);
Menu->AddItem(3, "The Alliance and Horde Quarters each have their own inns as well, and i am told there is one more beneath the city, in the sewers.", 10);
Menu->AddItem(3, "       ", 10);
Menu->AddItem(0, "Legerdemain Lounge", 31);
Menu->AddItem(0, "Alliance Inn", 32);
Menu->AddItem(0, "Horde Inn", 33);
Menu->AddItem(0, "Sewers", 34);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

case 31:
	{
Plr->Gossip_SendPOI(5845.40, 647.37, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 32:
	{
Plr->Gossip_SendPOI(5718.01, 689.28, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 33:
	{
Plr->Gossip_SendPOI(5890.22, 500.58, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 34:
	{
Plr->Gossip_SendPOI(5761.06, 714.45, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//mailbox
case 11:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(3, "There are many mailboxes in Dalaran: On many corners, outside nearly every inn and bank, even upon Krasus' Landing.", 11);
Menu->AddItem(3, "       ", 11);
Menu->AddItem(0, "Inn", 10);
Menu->AddItem(0, "Bank", 4);
Menu->AddItem(0, "Krasus' Landing", 8);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

//points of interest
case 12:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(3, "There are many places of interest in Dalaran. Which do you seek?", 12);
Menu->AddItem(3, "       ", 12);
Menu->AddItem(0, "The Alliance Quarter", 22);
Menu->AddItem(0, "The Horde Quarter", 23);
Menu->AddItem(0, "The Violet Citadel", 37);
Menu->AddItem(0, "The Violet Hold", 38);
Menu->AddItem(0, "Sewers", 27);
Menu->AddItem(0, "Trade District", 39);
Menu->AddItem(0, "Krasus'Landing", 8);
Menu->AddItem(0, "Antonidas Memorial", 40);
Menu->AddItem(0, "Runeweaver Square", 41);
Menu->AddItem(0, "The Eventide", 42);
Menu->AddItem(0, "Cemetary", 43);
Menu->AddItem(0, "Lexicon of Power", 44);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

case 37:
	{
Plr->Gossip_SendPOI(5795.11, 769.15, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 38:
	{
Plr->Gossip_SendPOI(5720.40, 537.46, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 39:
	{
Plr->Gossip_SendPOI(5900.88, 726.65, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 40:
	{
Plr->Gossip_SendPOI(5951.74, 683.53, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 41:
	{
Plr->Gossip_SendPOI(5810.96, 632.72, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 42:
	{
Plr->Gossip_SendPOI(5694.66, 650.82, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 43:
	{
Plr->Gossip_SendPOI(5853.35, 769.06, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 44:
	{
Plr->Gossip_SendPOI(5860.99, 707.23, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//stable master
case 13:
	{
Plr->Gossip_SendPOI(5859.68, 557.57, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//trainers
case 14:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(0, "Class Trainer", 46);
Menu->AddItem(0, "Cold Weather Flying Trainer", 8);
Menu->AddItem(0, "Portal Trainer", 47);
Menu->AddItem(0, "Profession Trainer", 48);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

//class trainer
case 46:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(3, "Dalaran's only class trainers are mages. You must take a portal back to one of your capitals for others.", 46);
Menu->AddItem(3, "       ", 46);
Menu->AddItem(3, "They can be found in Dalaran's Alliance and Horde quarters: the Silver Enclave and the Sunreaver's Sanctuary.", 46);
Menu->AddItem(3, "       ", 46);
Menu->AddItem(0, "The Alliance Quarter", 22);
Menu->AddItem(0, "The Horde Quarter", 23);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

//portal trainer
case 47:
	{
Plr->Gossip_SendPOI(5810.07, 581.00, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//profession trainer
case 48:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(0, "Alchemy", 51);
Menu->AddItem(0, "Blacksmithing", 52);
Menu->AddItem(0, "Cooking", 53);
Menu->AddItem(0, "Enchanting", 54);
Menu->AddItem(0, "engineering", 55);
Menu->AddItem(0, "First Aid", 56);
Menu->AddItem(0, "Fishing", 57);
Menu->AddItem(0, "Herbalism", 58);
Menu->AddItem(0, "Inscription", 59);
Menu->AddItem(0, "Jewelcrafting", 60);
Menu->AddItem(0, "Leatherworking", 61);
Menu->AddItem(0, "Mining", 62);
Menu->AddItem(0, "Skinning", 63);
Menu->AddItem(0, "Tailoring", 64);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

case 51:
	{
Plr->Gossip_SendPOI(5888.05, 702.44, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 52:
	{
Plr->Gossip_SendPOI(5904.86, 680.12, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 53:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(3, "Cooking trainers are located in the kitchens of the Alliance and Horde inns in theyr respective quarters.", 53);
Menu->AddItem(3, "       ", 53);
Menu->AddItem(0, "Alliance Inn", 32);
Menu->AddItem(0, "Horde Inn", 33);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

case 54:
	{
Plr->Gossip_SendPOI(5840.01, 726.53, 7, 6, 0, "Enchanting profession trainer");
                Plr->Gossip_Complete();
}break;

case 55:
	{
Plr->Gossip_SendPOI(5922.55, 727.05, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 56:
	{
Plr->Gossip_SendPOI(5862.76, 743.71, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 57:
	{
Plr->Gossip_SendPOI(5707.45, 614.57, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 58:
	{
Plr->Gossip_SendPOI(5873.99, 689.44, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 59:
	{
Plr->Gossip_SendPOI(5861.80, 704.30, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 60:
	{
Plr->Gossip_SendPOI(5874.27, 719.18, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 61:
	{
Plr->Gossip_SendPOI(5903.90, 751.97, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 62:
	{
Plr->Gossip_SendPOI(5923.27, 709.86, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 63:
	{
Plr->Gossip_SendPOI(5903.90, 751.97, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 64:
	{
Plr->Gossip_SendPOI(5881.78, 746.64, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//vendors
case 15:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(0, "Armor", 66);
Menu->AddItem(0, "Clothing", 82);
Menu->AddItem(0, "Emblem Gear", 68);
Menu->AddItem(0, "Flowers", 69);
Menu->AddItem(0, "Fruit", 70);
Menu->AddItem(0, "General Goods", 71);
Menu->AddItem(0, "Jewelry", 60);
Menu->AddItem(0, "Pet Supplies & Exotic Mounts", 73);
Menu->AddItem(0, "Pie, Pastry & Cakes", 74);
Menu->AddItem(0, "Reagents & Magical Goods", 75);
Menu->AddItem(0, "Toys", 76);
Menu->AddItem(0, "Trade Supplies", 71);
Menu->AddItem(0, "Trinkets. Relics & Off-hand items", 78);
Menu->AddItem(0, "Weapons", 79);
Menu->AddItem(0, "Wine & Cheese", 80);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

case 69:
	{
Plr->Gossip_SendPOI(5772.27, 576.61, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 70:
	{
Plr->Gossip_SendPOI(5754.02, 694.18, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 71:
	{
Plr->Gossip_SendPOI(5681.22, 616.57, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 73:
	{
Plr->Gossip_SendPOI(5833.13, 572.72, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 74:
	{
Plr->Gossip_SendPOI(5905.86, 629.29, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 75:
	{
Plr->Gossip_SendPOI(5758.70, 737.24, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 76:
	{
Plr->Gossip_SendPOI(5813.48, 688.49, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 78:
	{
Plr->Gossip_SendPOI(5755.53, 642.03, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 80:
	{
Plr->Gossip_SendPOI(5885.66, 606.89, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//armor
case 66:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(3, "What sort of armor?", 66);
Menu->AddItem(3, "       ", 66);
Menu->AddItem(0, "Cloth Armor", 82);
Menu->AddItem(0, "Leather Armor", 83);
Menu->AddItem(0, "Mail Armor", 84);
Menu->AddItem(0, "Plate Armor", 85);
Menu->AddItem(0, "Shields", 86);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

case 82:
	{
Plr->Gossip_SendPOI(5793.11, 686.44, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 83:
	{
Plr->Gossip_SendPOI(5667.76, 627.16, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 84:
	{
Plr->Gossip_SendPOI(5667.76, 627.16, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 85:
	{
Plr->Gossip_SendPOI(5914.85, 667.22, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 86:
	{
Plr->Gossip_SendPOI(5914.85, 667.22, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

//emblem gear
case 68:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(3, "Adventurers turn in their Emblems of Heroism or Valor in the Silver Enclave or Sunreaver's Sactuary", 68);
Menu->AddItem(3, "       ", 68);
Menu->AddItem(0, "The Alliance Quarter", 22);
Menu->AddItem(0, "The Horde Quarter", 23);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

//weapons
case 79:
	{
objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
Menu->AddItem(3, "What sorts of weapons?", 79);
Menu->AddItem(3, "       ", 79);
Menu->AddItem(0, "Melee Weapons", 89);
Menu->AddItem(0, "Ranged & Thrown Weapons", 90);
Menu->AddItem(0, "Staves & Wands", 91);
Menu->AddItem(0, "[Back]", 1);
Menu->SendTo(Plr);
}break;

case 89:
	{
Plr->Gossip_SendPOI(5725.11, 602.57, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 90:
	{
Plr->Gossip_SendPOI(5778.50, 556.18, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 91:
	{
Plr->Gossip_SendPOI(5665.40, 644.91, 7, 6, 0, "name");
                Plr->Gossip_Complete();
}break;

case 1:
	{
	objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
  		Menu->AddItem(0, "Arena", 2);
		Menu->AddItem(0, "Auction House", 3);
		Menu->AddItem(0, "Bank", 4);
		Menu->AddItem(0, "Barber", 5);
		Menu->AddItem(0, "Battlemasters", 6);
		Menu->AddItem(0, "Capital Portals", 7);
		Menu->AddItem(0, "Flight Master", 8);
		Menu->AddItem(0, "Guild Master", 9);
		Menu->AddItem(0, "Inn", 10);
		Menu->AddItem(0, "Mailbox", 11);
		Menu->AddItem(0, "Points of Interest", 12);
		Menu->AddItem(0, "Stable Master", 13);
		Menu->AddItem(0, "Trainers", 14);
		Menu->AddItem(0, "Vendors", 15);
        Menu->SendTo(Plr);
		}break;
}
};

void DalaranGuard::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}
void SetupDalaranGuardGossip(ScriptMgr * mgr)
{
	GossipScript * daralan = (GossipScript*) new DalaranGuard();
	mgr->register_gossip_script(32675,daralan);	
	mgr->register_gossip_script(32676,daralan);
	mgr->register_gossip_script(32677,daralan);
	mgr->register_gossip_script(32678,daralan);
	mgr->register_gossip_script(32679,daralan);
	mgr->register_gossip_script(32680,daralan);
	mgr->register_gossip_script(32681,daralan);
	mgr->register_gossip_script(32683,daralan);
	mgr->register_gossip_script(32684,daralan);
	mgr->register_gossip_script(32685,daralan);
	mgr->register_gossip_script(32686,daralan);
	mgr->register_gossip_script(32687,daralan);
	mgr->register_gossip_script(32688,daralan);
	mgr->register_gossip_script(32689,daralan);
	mgr->register_gossip_script(32690,daralan);
	mgr->register_gossip_script(32691,daralan);
	mgr->register_gossip_script(32692,daralan);
	mgr->register_gossip_script(32693,daralan);
}