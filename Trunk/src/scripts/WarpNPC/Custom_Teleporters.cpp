#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
  #pragma warning(disable:4305)        // warning C4305: 'argument' : truncation from 'double' to 'float'
  #endif

class SCRIPT_DECL Warper : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
    void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
    void GossipEnd(Object * pObject, Player* Plr);
	void Destroy()	{		delete this;	}
	void resetskilland_send_menu(Object * pObject, Player* Plr,uint32 skill,uint32 maxskill);
	uint32 getmax_profession_fishing(Player* Plr);
	uint32 getmax_profession_firstaid(Player* Plr);
	uint32 getmax_profession_cooking(Player* Plr);
	uint32 getmax_profession_skinning(Player* Plr);
	uint32 getmax_profession_mining(Player* Plr);
	uint32 getmax_profession_leatherwork(Player* Plr);
	uint32 getmax_profession_herbalism(Player* Plr);
	uint32 getmax_profession_blacksmith(Player* Plr);
	uint32 getmax_profession_alchemy(Player* Plr);
	uint32 getmax_profession_jewelcrafting(Player* Plr);
	uint32 getmax_profession_engineering(Player* Plr);
	uint32 getmax_profession_tailor(Player* Plr);
	uint32 getmax_profession_enchant(Player* Plr);
	uint32 getmax_profession_inscription(Player* Plr);
	uint32 getmax_profession_archaeology(Player* Plr);
	void send_MainPage(Object * pObject, Player* Plr)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		Menu->AddItem(6, "Mall", 20);
		Menu->AddItem(2, "Alliance Cities", 2);
		Menu->AddItem(2, "Horde Cities", 1);
	//	Menu->AddItem(6, "Repair items", 1234);
		if (Plr->GetTeam() == 1)
		{
//			Menu->AddItem(9, "Mall (PvP)", 210);
			Menu->AddItem(2, "Leveling Areas 10-60", 81);
		}
		else
		{
//			Menu->AddItem(9, "Mall (PvP)", 211);
			Menu->AddItem(2, "Leveling Areas 10-60", 80);
		}
		Menu->AddItem(2, "Azeroth Instances", 30);
		Menu->AddItem(2, "Outland Locations lvl 60+", 3);
		Menu->AddItem(2, "Outland Instances", 50);
		Menu->AddItem(2, "Northrend Locations lvl 70+", 500);
		Menu->AddItem(2, "Northrend Instances", 300);
		Menu->AddItem(2, "Cataclysm Locations lvl 80+", 700);
		Menu->AddItem(2, "Cataclysm Instances", 800);
		Menu->AddItem(2, "MOP maps ( EMPTY )", 850);
		Menu->AddItem(2, "Isle of Quel'Danas", 250);
		Menu->AddItem(8, "Gurubashi Arena", 21);
		// Menu->AddItem(8, "PvP Temple", 22);
	//	Menu->AddItem(9, "Make This Place Your Home", 95);
	//	Menu->AddItem(10, "Reset Talentpoints", 220);
	//	Menu->AddItem(11, "Remove Resurrection Sickness", 97);
		Menu->AddItem(5, "Players Tools and Fixes", 369);
		Menu->AddMenuItem(5, "Advanced Bug Fixes.",0,0,"Are you sure ?",0,0,600);
		Menu->AddItem(1, "Buy Food & Water & Repair ", 200);
		Menu->SendTo(Plr);
	}
	void Cast_Reward_auras_on_active_player(Player* Plr);
};

void Warper::Cast_Reward_auras_on_active_player(Player* Plr)
{
//	return;
	uint32 kill_limits[13] = { 1,100,200,300,400,500,600,700,800,900,1500,3000,6000 };
	//bonus auras : class / auras
	uint32 n_spells[12][17] = 
	{
//		{20217,58451,58452,48104,48102,48100,58449,9885,48161,26035,47430},	//no class 0
		{20217,89345,89346,89347,1126,21562,26035,0,0,0,0,0,0,0,0},	//no class 0
		{0,0,0,0,0,0,0,0,42505,0,0,0,0,0,0,0,0},	//WARRIOR
		{0,0,0,0,0,0,0,0,42505,0,0,0,0,0,0,0,0},	//PALADIN
		{0,0,0,0,0,0,0,0,42505,0,0,0,0,0,0,0,0},	//HUNTER
		{0,0,0,0,0,0,0,0,42505,0,0,0,0,0,0,0,0},	//ROGUE
		{0,0,0,0,0,0,0,0,42505,0,0,0,0,0,0,0,0},	//PRIEST
		{0,0,0,0,0,0,0,0,42505,0,0,0,0,0,0,0,0},	//SHAMAN
		{0,0,0,0,0,0,0,0,42505,0,0,0,0,0,0,0,0},	//MAGE
		{0,0,0,0,0,0,0,0,42505,0,0,0,0,0,0,0,0},	//WARLOCK
		{0,0,0,0,0,0,0,0,42505,0,0,0,0,0,0,0,0},	//no class 10
		{0,0,0,0,0,0,0,0,42505,0,0,0,0,0,0,0,0},	//druid
	};
	uint32 kill_level = 0;
	for(uint32 i=0;i<13;i++)
		if( kill_limits[i] < Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS) )
			kill_level = i;
	
	uint32 clas = Plr->getClass();
	SpellCastTargets targets;
	targets.m_targetMask = TARGET_FLAG_UNIT;
	targets.m_unitTarget = Plr->GetGUID();
	
	for( uint32 i=0;i<kill_level;i++)
	{
		if( n_spells[clas][i] )
		{
			SpellEntry * ent = dbcSpell.LookupEntry(n_spells[clas][i]);
			Spell *newSpell = SpellPool.PooledNew( __FILE__, __LINE__ );
			newSpell->Init(Plr, ent, true, 0);
			newSpell->prepare(&targets);
		}
		if( n_spells[0][i] )
		{
			SpellEntry * ent = dbcSpell.LookupEntry(n_spells[0][i]);
			Spell *newSpell = SpellPool.PooledNew( __FILE__, __LINE__ );
			newSpell->Init(Plr, ent, true, 0);
			newSpell->prepare(&targets);
		}
	}
}

void Warper::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
	Cast_Reward_auras_on_active_player( Plr );
	if( AutoSend )
		send_MainPage(pObject,Plr);
}

void Warper::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	//can't imagine this getting NULL 
	if( Plr ) 
	{
		//note that this check is not 100% sometimes after death events put players in combat and make them get stuck.
		if( Plr->CombatStatus.IsInCombat() )
		{
			Plr->BroadcastMessage("You are not allowed to use Taxi while in combat");
			return;
		}
	}

	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
		return;

	GossipMenu *Menu;

	switch(IntId)
    {
		//main menu
		case 99:
	    case 0:	
			send_MainPage(pObject,Plr);
			break; //main
		case 1:		
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			if (Plr->GetTeam() == 1)
					Menu->AddItem(5, "Orgrimmar", 5);
			else
					Menu->AddItem(5, "Orgrimmar", 225);
			Menu->AddItem(5, "Silvermoon", 4);
			Menu->AddItem(5, "Thunderbluff", 6);
			Menu->AddItem(5, "UnderCity", 7);
			Menu->AddItem(0, "[Back]", 99);
			Menu->SendTo(Plr);
			break; //Horde cities
		case 2:		
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			if (Plr->GetTeam() == 1)
				Menu->AddItem(5, "Stormwind", 9);
			else
				Menu->AddItem(5, "Stormwind", 229);
			Menu->AddItem(5, "The Exodar", 8);
			Menu->AddItem(5, "Ironforge", 10);
			Menu->AddItem(5, "Darnassus", 11);
			Menu->AddItem(0, "[Back]", 99);
			Menu->SendTo(Plr);
			break; //Alliance Cities
		case 3:     
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem(5, "Hellfire Peninsula (58-63)", 12);
			Menu->AddItem(5, "Zangermarsh (60-64)", 13);
			Menu->AddItem(5, "Nagrand (64-67)", 14);
			Menu->AddItem(5, "Blades Edge Mountains (65-68)", 15);
			Menu->AddItem(5, "Netherstorm (67-70)", 16);
			Menu->AddItem(5, "Terokkar Forest (62-65)", 17);
			Menu->AddItem(5, "Shadowmoon Valley (67-70)", 18);
			Menu->AddItem(0, "[Back]", 99);
			Menu->SendTo(Plr);
			break; //Outland Locations
	    case 4:     Plr->EventTeleport(530, 9400.486328, -7278.376953, 14.206780);		break; //Silvermoon
		case 5:     Plr->EventTeleport(1, 1548, -4404, 19);								break;//Orgrimmar
		case 225:   Plr->EventTeleport(1, 1330, -4360, 29);								break;//Orgrimmar
		case 6:     Plr->EventTeleport(1, -1304.569946, 205.285004, 68.681396);			break;//ThunderBluff
		case 7:     Plr->EventTeleport(0, 2050.203125, 285.650604, 56.994549);			break;//UnderCity
		case 8:     Plr->EventTeleport(530, -4072.202393, -12014.337891, -1.277277);	break;//Exodar
		case 9:     Plr->EventTeleport(0, -9100.480469, 406.950745, 92.594185);			break;//Stormwind
		case 229:   Plr->EventTeleport(0, -8833.76, 623.25, 93.691803);					break;//Stormwind
		case 10:    Plr->EventTeleport(0, -5028.265137, -825.976563, 495.301575);		break;//Ironforge
		case 11:    Plr->EventTeleport(1, 9985.907227, 1971.155640, 1326.815674);		break;//Darnassus
		case 12:    Plr->EventTeleport(530, -248.160004, 922.348999, 84.379799);		break;//Hellfire Peninsula
		case 13:    Plr->EventTeleport(530, -225.863632, 5405.927246, 22.346397);		break;//Zangermarsh
		case 14:    Plr->EventTeleport(530, -468.232330, 8418.666016, 28.031298);		break;//Nagrand
		case 15:    Plr->EventTeleport(530, 1471.672852, 6828.047852, 107.759239);		break;//Blades Edge Mountains
		case 16:    Plr->EventTeleport(530, 3396.123779, 4182.208008, 137.097992);		break;//Netherstorm
		case 17:    Plr->EventTeleport(530, -1202.426636, 5313.692871, 33.774723);		break;//Terokkar Forest
		case 18:    Plr->EventTeleport(530, -2859.522461, 3182.34773, 10.008426);		break;//Shadowmoon Valley

		//MALL
		case 20:	
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem(3, "Class Trainers", 900);
			Menu->AddItem(1, "Item Vendors", 901);
			Menu->AddItem(3, "Profession Trainers", 902);
			Menu->AddItem(8, "PVP Vanity Statues", 903);
			Menu->AddItem(0, "[Back]", 99);
			Menu->SendTo(Plr);
			break; // Mall
			case 900:    Plr->EventTeleport(530, -1876.26, 5455.25, -12.427);				break;//Class Trainers		
			case 901:    Plr->EventTeleport(530, -1891.09, 5416.40, -12.426);				break;//Item Vendors
			case 902:    Plr->EventTeleport(530, -1837.70, 5448.44, -12.427);				break;//Proffesion Trainers
			case 903:    Plr->EventTeleport(530, -1845.93, 5394.61, -12.427);				break;//PVP Vanity Statues
		case 21:    Plr->EventTeleport(0, -13271.959961, 149.105515, 34.464458);			break;// Gurubsahi Arena
		// case 22:    
            // if (Plr->GetTeam() == 0)
					// Plr->EventTeleport(1, -8512.610352, 2018.642822, 104.748581);				// PvP Temple Alliance
			// else
					// Plr->EventTeleport(1, -8654.244141, 1964.294556, 106.390472);				// PvP Temple Horde
       		// break;
		case 30:	
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem(5, "Shadowfang Keep", 31);
			Menu->AddItem(5, "Scarlet Monastery", 32);
			Menu->AddItem(5, "Zul'Farrak", 33);
			Menu->AddItem(5, "Stratholme", 41);
			Menu->AddItem(5, "Scholomance", 40);
			Menu->AddItem(5, "Blackrock Spire", 42);
			Menu->AddItem(5, "Onyxia's Lair", 38);
			Menu->AddItem(5, "Molten Core", 37);
			Menu->AddItem(5, "Zul'Gurub", 34);
			Menu->AddItem(5, "Karazhan", 43);
			Menu->AddItem(5, "Ahn'Qirai 20", 35);
			Menu->AddItem(5, "Ahn'Qirai 40", 36);
			Menu->AddItem(5, "Naxxramas", 39);
			Menu->AddItem(5, "Caverns of Times", 44);
			Menu->AddItem(5, "Blackwing Lair Inside", 45);
			Menu->AddItem(5, "Zul'Aman", 100);
			Menu->AddItem(0, "[Back]", 99);
			Menu->SendTo(Plr);
			break; // Azeroth Instances
		case 31:    Plr->EventTeleport(0, -234.495087, 1561.946411, 76.892143);			break;// Shadowfang Keep
		case 32:    Plr->EventTeleport(0, 2870.442627, -819.985229, 160.331085);		break;// Scarlet Monastery
		case 33:    Plr->EventTeleport(1, -6797.278809, -2903.917969, 9.953360);		break;// Zul'Farrak
		case 34:    Plr->EventTeleport(0, -11919.073242, -1202.459374, 92.298744);		break;// Zul'Gurub
		case 35:    Plr->EventTeleport(1, -8394.730469, 1485.658447, 21.038563);		break;// Ahn'Qirai 20
		case 36:    Plr->EventTeleport(1, -8247.316406, 1970.156860, 129.071472);		break;// Ahn'Qirai 40
		case 37: 
		        // Plr->EventTeleport(0, -7515.409668, -1045.369629, 182.301208);
	            Plr->EventTeleport(409, 1089.6, -470.19, -106.41);
			break;// Molten Core
		case 38: // Onyxia
				// Plr->EventTeleport(1, -4708.491699, -3727.672363, 54.535076);
				Plr->EventTeleport(249, 30.0, -64.0, -5.0);
			break;
		case 39: // Naxxramas
				// Plr->EventTeleport(0, 3132.915283, -3731.012939, 138.658371);
				Plr->EventTeleport(533, 3006.06, -3436.72, 293.891);
			break;
		case 40: 	Plr->EventTeleport(0, 1267.468628, -2556.651367, 94.127983);		break;//Scholomance
		case 41: 	Plr->EventTeleport(0, 3359.111572, -3380.8444238, 144.781860);		break;//Stratholme
		case 42: 	Plr->EventTeleport(0, -7527.129883, -1224.997437, 285.733002);		break;// Black ROck Spire
		case 43: // Kharazan
			// Plr->EventTeleport(0, -11122.913086, -2014.498779, 47.079350);
			Plr->EventTeleport(532, -11087.3, -1977.47, 49.6135);
		break;
		case 44: 	Plr->EventTeleport(1, -8519.718750, -4297.542480, -208.441376);		break;// Caverns of Times
		case 45:    Plr->EventTeleport(469, -7664, -1101, 401);							break;// Blackwing Lair
		case 100: 	Plr->EventTeleport(530, 6848.597168, -7761.557129, 124.714592);		break;// Zul'Aman
		case 50:	
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem(0, "Outland Raids", 64);
			Menu->AddItem(5, "Hellfire Ramparts", 51);
			Menu->AddItem(5, "The Blood Furnace", 52);
			Menu->AddItem(5, "The Shattered Halls", 53);
			Menu->AddItem(5, "The Underbog", 54);
			Menu->AddItem(5, "The Slave Pens", 55);
			Menu->AddItem(5, "The Steamvault", 56);
			Menu->AddItem(5, "Mana-Tombs", 57);
			Menu->AddItem(5, "Auchenai Crypts", 58);
			Menu->AddItem(5, "Sethekk Halls", 59);
			Menu->AddItem(5, "Shadow Labyrinth", 60);
			Menu->AddItem(5, "The Mechanar", 61);
			Menu->AddItem(5, "The Botanica", 62);
			Menu->AddItem(5, "The Arcatraz", 63);
			Menu->AddItem(0, "[Back]", 99);
			Menu->SendTo(Plr);
			break;// Outland Instances
		case 51:	Plr->EventTeleport(530, -360.670990, 3071.899902, -15.097700);		break;// Hellfire Ramparts
		case 52: 	Plr->EventTeleport(530, -303.506012, 3164.820068, 31.742500);		break;// The Blood Furnace
		case 53: 	Plr->EventTeleport(530, -311.083527, 3083.291748, -3.745923);		break;// The Shattered Halls
		case 54: 	Plr->EventTeleport(530, 777.088989, 6763.450195, -72.062561);		break;// The Underbog
		case 55: 	Plr->EventTeleport(530, 719.507996, 6999.339844, -73.074303);		break;// The Slave Pens
		case 56: 	Plr->EventTeleport(530, 816.590027, 6934.669922, -80.544601);		break;// The Steamvault
		case 57: 	Plr->EventTeleport(530, -3079.810059, 4943.040039, -101.046997);	break;// Mana-Tombs
		case 58: 	Plr->EventTeleport(530, -3361.959961, 5225.770020, -101.047997);	break;// Auchenai Crypts
		case 59: 	Plr->EventTeleport(530, -3362.219971, 4660.410156, -101.049004 );	break;// Sethekk Halls
		case 60: 	Plr->EventTeleport(530, -3645.060059, 4943.620117, -101.047997);	break;// Shadow Labyrinth
		case 61: 	Plr->EventTeleport(530, 2862.409912, 1546.089966, 252.158691);		break;// The Mechanar
		case 62: 	Plr->EventTeleport(530, 3413.649902, 1483.319946, 182.837997);		break;// The Botanica
		case 63:	Plr->EventTeleport(530, 3311.598145, 1332.745117, 505.557251);		break; // The Arcatraz
		case 64: 
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(5, "Magtheridon's Lair", 65);
			Menu->AddItem(5, "Serpentshrine Cavern", 66);
            Menu->AddItem(5, "Gruul's Lair", 67);
			Menu->AddItem(5, "The Eye", 68);
			Menu->AddItem(5, "Black Temple", 69);
			Menu->AddItem(0, "[Back]", 99);
            Menu->SendTo(Plr);
		break;// Outland Raids
		case 65:	Plr->EventTeleport(530, -313.678986, 3088.350098, -116.501999);		break;// Magtheridon's Lair
		case 66:	Plr->EventTeleport(530, 830.542908, 6865.445801, -63.785503);		break;// Serpentshrine Cavern
		case 67:	Plr->EventTeleport(530, 3549.424072, 5179.854004, -4.430779);		break; // Gruul's Lair
		case 68: 	Plr->EventTeleport(530, 3087.310059, 1373.790039, 184.643005);		break;// The Eye
		case 69: // Black Temple
				// Plr->EventTeleport(530, -3609.739990, 328.252014, 37.307701 );
				Plr->EventTeleport(564, 97.0894, 1001.96, -86.8798 );
			break;
		case 80:     // Alliance
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
				Menu->AddItem(5, "Westfall(Santinel Hill) 10-20", 120);
				Menu->AddItem(5, "Redridge(Lakeshire) 15-25", 121);
				Menu->AddItem(5, "Hillsbrad(Southshore) 20-30", 122);
				Menu->AddItem(5, "Duskwood(Darkshire)) 18-30", 123);
				Menu->AddItem(5, "STV(Rebel Camp) 30-45", 124);
				Menu->AddItem(5, "Booty Bay", 125);
				Menu->AddItem(5, "The Hinterlands(Aerie Peak) 40-50", 236);
				Menu->AddItem(5, "Tanaris(Gadgetzan) 40-50", 126);
				Menu->AddItem(5, "Felwood 48-55", 127);
				Menu->AddItem(5, "Blasted Lands 45-55", 128);
				Menu->AddItem(5, "Winterspring(Everlook) 55-60", 129);
				Menu->AddItem(0, "[Back]", 99);
				Menu->SendTo(Plr);
			break;
		case 120:	Plr->EventTeleport(0, -10520.672852, 1070.661377, 54.476643);		break;//westfall
		case 121:	Plr->EventTeleport(0, -9474.256836, -2266.271484, 74.542351);		break;//redrige
		case 122:	Plr->EventTeleport(0, -691.75, -571.38, 25);			break;//southshore
		case 123:	Plr->EventTeleport(0, -10438.022461, -1169.694702, 27.711634);		break;//darkshire
		case 124:	Plr->EventTeleport(0, -11312.692383, -197.921173, 76.107666);		break;//rebel
		case 125:	Plr->EventTeleport(0, -14302, 520.451721, 8.668963);				break;//BootyBay
		case 236:	Plr->EventTeleport(0, 283.66, -2127.16, 119.962288);				break;//Aerie Peak
		case 126:	Plr->EventTeleport(1, -7164.650879, -3795.239014, 9.125962);		break;//Tanaris
		case 127:	Plr->EventTeleport(1, 6220.294922, -1957.315308, 570.255615);		break;//Felwood
		case 128:	Plr->EventTeleport(0, -11017.627930, -3320.720947, 60.680126);		break;//Blasted Lands
		case 129:	Plr->EventTeleport(1, 6718.615723, -4675.737305, 720.833557);		break;//Winterspring
		case 81:     // Maps Horde
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
				Menu->AddItem(5, "Crossroads 10-20", 130);
				Menu->AddItem(5, "Ashenvale 18-30", 131);
				Menu->AddItem(5, "Hillsbrad(Tarren Mill) 20-30", 132);
				Menu->AddItem(5, "T.Needles(Darkcloud) 25-35", 133);
				Menu->AddItem(5, "Booty Bay", 134);
				Menu->AddItem(5, "Blasted Lands 45-57", 234);
				Menu->AddItem(5, "Tanaris(Gadgetzan) 40-50", 135);
				Menu->AddItem(5, "Ungoro 48-55", 136);
				Menu->AddItem(5, "Sillithus 55-60", 137);
				Menu->AddItem(5, "Felwood 48-55", 138);
				Menu->AddItem(5, "Winterspring(Everlook) 55-60", 139);
				Menu->AddItem(0, "[Back]", 99);
				Menu->SendTo(Plr);
			break;
		case 130: 	Plr->EventTeleport(1, -442, -2632.331543, 95.534744);				break;// XR
		case 131:   Plr->EventTeleport(1, 2271.131348, -2546.675781, 98);		break;//Ashenvale
		case 132:   Plr->EventTeleport(0, -31.493298, -920.824158, 54.640732);			break;//tarren mill
		case 133:	Plr->EventTeleport(1, -5100.577148, -1945.574219, 88.742935);		break;//darkcloud
		case 134:	Plr->EventTeleport(0, -14302, 520.451721, 8.668963);				break;//BootyBay
		case 234:	Plr->EventTeleport(0, -10859, -2940, 13.294115);					break;//BLands
		case 135:	Plr->EventTeleport(1, -7164.650879, -3795.239014, 9.125962);		break;//Tanaris
		case 136:	Plr->EventTeleport(1, -6158.600098, -1093.307007, -205.555405);		break;//Ungoro
		case 137:	Plr->EventTeleport(1, -6836.545898, 757.019226, 42.706440);			break;//Sillithus
		case 138:	Plr->EventTeleport(1, 5124.594238, -338.450073, 356.489868);		break;//Felwood
		case 139:	Plr->EventTeleport(1, 6718.615723, -4675.737305, 720.833557);		break;//Winterspring
		case 85:	Plr->EventTeleport(0, 3760, -2578, 131 );							break;
        case 86:	Plr->EventTeleport(0, -13845, 2714, 53 );							break;
		case 95:    Plr->GetSession()->SendInnkeeperBind(pCreature);					break;// Bind Position
		case 97: // Remove res sickness
				Plr->addSpell(15007);			// learn res sickness
				Plr->removeSpell(15007,0,0,0);		// un learn res sickness
				pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "We have removed your Resurrection Sickness." );
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
				Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
                break;
        case 500:	 //northrend locations
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
                Menu->AddItem(5, "Borean Tundra (68-72)", 501);
				Menu->AddItem(5, "Crystalsong Forest (74-76)", 502);
				Menu->AddItem(5, "Dalaran", 503);
				Menu->AddItem(5, "Dragonblight (71-74)", 504);
				Menu->AddItem(5, "Grizzly Hills (73-75)", 505);
				Menu->AddItem(5, "Howling Fjord (68-72)", 506);
				Menu->AddItem(5, "Hrothgar's Landing (77-80)", 507);
				Menu->AddItem(5, "Icecrown (77-80)", 508);
				Menu->AddItem(5, "Sholazar Basin (75-78)", 509);
				Menu->AddItem(5, "The Storm Peaks (76-80)", 510);
				Menu->AddItem(5, "Wintergrasp (77-80)", 511);
				Menu->AddItem(5, "Zul'Drak (74-77)", 512);
                Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
            break;
			
	
		case 501:	Plr->EventTeleport(571, 2874, 5431, 57);		break; //Borean Tundra
		case 502:	Plr->EventTeleport(571, 5408, 462, 172);		break; //Crystalsong Forest
		case 503:	Plr->EventTeleport(571, 5748, 569, 651.5 );		break; //Dalaran
		case 504:	Plr->EventTeleport(571, 3575.07, 856.02, 117.44);		break; //Dragonblight
		case 505:	Plr->EventTeleport(571, 3622, -3419, 241);		break; //Grizzly Hills
		case 506:	
			if (Plr->GetTeam() == 1)
				Plr->EventTeleport(571, 432.061, -4558.01, 245.784 );	//Howling Fjord 1
			else
				Plr->EventTeleport(571, 595.124, -5081.49, 5.55386 );	//Howling Fjord 2
			break;	
		case 507:	Plr->EventTeleport(571, 10314.14, 778.87, 74.37);		break; //Hrothgar's Landing
		case 508:	Plr->EventTeleport(571, 7240.71, 2160.98, 564.96);		break; //Icecrown
		case 509:	Plr->EventTeleport(571, 5464.70, 5000.89, -131.24);		break; //Sholazar Basin
		case 510:	Plr->EventTeleport(571, 6233, -1049, 414);		break; //The Storm Peaks
		case 511:	Plr->EventTeleport(571, 4510.39, 2765.08, 389.45);		break; //Wintergrasp
		case 512:	Plr->EventTeleport(571, 5440, -1247, 249);		break; //Zul'Drak

		case 250:   Plr->EventTeleport(530, 13007.82, -6913.34, 9.583396);				break;//Quel
		case 200:	Plr->GetSession()->SendInventoryList(pCreature);					break;// Repair
		case 210:   Plr->EventTeleport(1, -2717.19, -4987.04, 26.866404);				break;//PvP Mall
		case 211:   Plr->EventTeleport(1, -2677.69, -4810.75, 18.402950);				break;//PvP Mall
		case 300:     // Nortrend Instances
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
                Menu->AddItem(5, "Utgarde Instances", 301);
				Menu->AddItem(5, "Nexus Instances", 302);
				Menu->AddItem(5, "Ulduar Instances", 303);
				Menu->AddItem(5, "Drak'theron Keep", 304);
				Menu->AddItem(5, "COT: Stratholme Past", 305);
				Menu->AddItem(5, "Azjol-Nerub Instances", 306);
//				Menu->AddItem(5, "Northrend Battleground", 307);
				Menu->AddItem(5, "The Violet Hold", 308);
				Menu->AddItem(5, "The Obsidian Sanctum", 309);
				Menu->AddItem(5, "Trial of the Champion", 310);
				Menu->AddItem(5, "Trial of the Crusader", 313);
				Menu->AddItem(5, "Icecrown Instances", 314);
				Menu->AddItem(5, "Icecrown Raid", 315);
				Menu->AddItem(5, "Vault of Archavon", 400);
				Menu->AddItem(5, "Ruby Sanctum", 317);
                Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
            break;

		case 313:                Plr->EventTeleport(571, 8516, 731, 558.25);			break;//Trial of Crusader
		case 314:                Plr->EventTeleport(571, 5628, 2035, 798.28);			break;//Icecrown Instances
		case 315:                Plr->EventTeleport(571, 6118.17, 2223.15, 515.34);		break;//Icecrown Raid
		case 317:                Plr->EventTeleport(571, 3597, 201, -114);				break;//Ruby Sanctum
		
		case 301:     // Utgarde Instances
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
                Menu->AddItem(5, "Utgarde Keep", 311);
				Menu->AddItem(5, "Utgarde Pinnacle", 312);
				Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
            break;
		case 311:                Plr->EventTeleport(571, 1220, -4864, 41.248);			break;//Utgarde Keep
		case 312:                Plr->EventTeleport(571, 1252, -4854, 215.80);			break;//Utgarde Pinnacle
		case 302:     // Nexus Instances
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
                Menu->AddItem(5, "The Nexus", 321);
				Menu->AddItem(5, "The Oculus", 322);
				Menu->AddItem(5, "The Eye of eternity", 323);
				Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
            break;
		case 321:                Plr->EventTeleport(571, 3891, 6985, 69.49);			break;//The Nexus
		case 322:                Plr->EventTeleport(571, 3866, 6986, 153);				break;//The Occulus
					//the eye of eternity
		case 323:				Plr->EventTeleport( 571, 3864, 6984, 107);				break; 
		case 303:     //Ulduar Instances
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
                Menu->AddItem(5, "Halls of Stone", 331);
				Menu->AddItem(5, "Halls of Lightning", 332);
				Menu->AddItem(5, "Ulduar Raid", 333);
				Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
            break;
		case 331:                Plr->EventTeleport(599, 1153, 811, 196);				break;//Halls of Stone
		case 332:                Plr->EventTeleport(602, 1333, -237, 41);				break;//Halls of Lightning
		case 333:                Plr->EventTeleport(571, 9276.140, -1116.917, 1216.117);				break;//Uldar Raid
		case 304:     // Drak'theron Keep Instances
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
                Menu->AddItem(5, "Hall of Departure", 341);
				Menu->AddItem(5, "Gun'Drak", 342);
				Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
            break;
		case 341:                Plr->EventTeleport(571, 4774, -2038, 230);				break;//Hall of Departure
		case 342:                Plr->EventTeleport(604, 1947.18, 682.82, 135.433853);	break;//Gun'Drak
		case 305:     // Caverns of Time: Stratholme Past
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
                Menu->AddItem(5, "Outer Instance Entrance", 351);
				Menu->AddItem(5, "Bridge to Stratholme", 352);
				Menu->AddItem(5, "Crusaders Square", 353);
				Menu->AddItem(5, "Festival Lane", 354);
				Menu->AddItem(5, "Inn", 355);
				Menu->AddItem(5, "Plaguewood Tower", 356);

				Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
            break;
		case 351:                Plr->EventTeleport(1, -8638, -4382, -207);				break;//Outer Instance Entrance
		case 352:                Plr->EventTeleport(595, 1967, 1287, 146);				break;//Bridge to Stratholme
		case 353:                Plr->EventTeleport(595, 2300, 1495, 129);				break;//Crusaders Square
		case 354:                Plr->EventTeleport(595, 2258, 1153, 139);				break;//Festival Lane
		case 355:                Plr->EventTeleport(595, 1560, 603, 100);				break;//Inn
		case 356:                Plr->EventTeleport(595, 1654, 1611, 117);				break;//Plaguewood Tower
		case 306:     //Azjol-Nerub Instances
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
                Menu->AddItem(5, "Azjol-Nerub", 361);
				Menu->AddItem(5, "Ahn'kahet: The Old Kingdom", 362);
				Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
            break;
		case 700:	//cataclysm locations
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
				if (Plr->GetTeam() == 1)
				{
					Menu->AddItem(5, "Kezan", 702);
					Menu->AddItem(5, "Lost Island", 709);
				}
				else
					Menu->AddItem(5, "Gilneas", 701);
				Menu->AddItem(5, "Tol Barad (84-85)", 703);
				Menu->AddItem(5, "Deepholm (82-83)", 704);
				Menu->AddItem(5, "Uldum (83-84)", 705);
				Menu->AddItem(5, "Vashj'ir (80-82)", 706);
				Menu->AddItem(5, "Mount Hyjal (80-82)", 707);
				Menu->AddItem(5, "Twilight Highlands (84-85)",708);
				Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
			}break;
		case 800:	//cataclysm locations
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
                Menu->AddItem(5, "Deadmines(Heroic)", 811);
                Menu->AddItem(5, "Shadowfang Keep(Heroic)", 31);
				Menu->AddItem(5, "Zul'Aman(Heroic)", 100);
				Menu->AddItem(5, "Zul'Gurub(Heroic)", 34);
                Menu->AddItem(5, "The Stonecore", 801);
                Menu->AddItem(5, "The Vortex Pinacle", 802);
                Menu->AddItem(5, "Throne of the Tides", 803);
                Menu->AddItem(5, "Blackrock Caverns", 804);
                Menu->AddItem(5, "Lost City of the Tol'vir", 805);
                Menu->AddItem(5, "Grim Batol", 806);
                Menu->AddItem(5, "Halls of Origination", 807);
                Menu->AddItem(5, "Raid - The Bastion of Twilight", 808);
                Menu->AddItem(5, "Raid - Blackwing Descent", 809);
                Menu->AddItem(5, "Raid - Throne of the Four Winds", 810);
                Menu->AddItem(5, "Raid - Baradin Hold", 813);
                Menu->AddItem(5, "Raid - Firelands", 812);
                Menu->AddItem(5, "Molten Front", 814);
                Menu->AddItem(5, "End Time(Heroic)", 815);
                Menu->AddItem(5, "Well of Eternity(Heroic)", 816);
                Menu->AddItem(5, "Hour of Twilight(Heroic)", 817);
                Menu->AddItem(5, "Raid - Dragon Soul", 819);
                Menu->AddItem(5, "Raid - Halls of Mortality", 820);
                Menu->AddItem(5, "-DarkMoon Faire-", 818);
                Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
			}break;
		case 801:		Plr->EventTeleport(646, 1025, 636, 158);				break; //the stonecore
		case 802:		Plr->EventTeleport(1, -11524, -2318, 627);				break; //the vortex pinnacle
		case 803:		Plr->EventTeleport(0, -5628, 5311, -1260);				break; //Throne of the Tides
		case 804:		Plr->EventTeleport(0, -7572, -1313, 246);				break; //Blackrock Caverns
		case 805:		Plr->EventTeleport(1, -10702, -1312, 18);				break; //Lost City of the Tol'vir
		case 806:		Plr->EventTeleport(0, -4062, -3450, 281);				break; //Grim Batol
		case 807:		Plr->EventTeleport(1, -10204, -1837, 21);				break; //Halls of Origination
		case 808:		Plr->EventTeleport(0, -4891, -4238, 828);				break; //The Bastion of Twilight
		case 809:		Plr->EventTeleport(0, -7537, -1199, 479);				break; //Blackwing Descent
		case 810:		Plr->EventTeleport(1, -11355, 57, 726);					break; //Throne of the Four Winds
		case 811:		Plr->EventTeleport(0, -11209, 1668, 25);				break; //Deadmines
		case 812:		Plr->EventTeleport(1, 3955, -2832, 619);				break; //Firelands
		case 813:		Plr->EventTeleport(732, -1260, 1050, 107);				break; //baradin hold
		case 814:		Plr->EventTeleport(861, 1031, 318, 42);					break; //Molten Front
		case 815:		Plr->EventTeleport(1, -8301, -4457, -206);				break; //End Time
		case 816:		Plr->EventTeleport(1, -8600, -4016, -204);				break; //Well of Eternity
		case 817:		Plr->EventTeleport(1, -8296, -4568, -222);				break; //Hour of Twilight
		case 818:		Plr->EventTeleport(974, -3621, 6317, 114);				break; //Darkmoon Faire
		case 819:		Plr->EventTeleport(1, -8272, -4513, -217);				break; //Dragon Soul
		case 820:		Plr->EventTeleport(994, -3969.67, -2542.71, 29);		break; //Halls of mortality
		case 850:		//MOP maps
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
//                Menu->AddItem(5, "Mogu",851);	//renaming it to halls of mortality
//                Menu->AddItem(5, "Scarlet Sancuary", 852);
				Menu->AddItem(5, "New Scholomance", 853);
				Menu->AddItem(5, "Mogu Crypt", 854);
                Menu->AddItem(5, "Proving Grounds", 855);
                Menu->AddItem(5, "Ring of valor", 856);
                Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
			}break;
//		case 851:		Plr->EventTeleport(994, -3969.67, -2542.71, 29);		break; //Mogu
		case 852:		Plr->EventTeleport(1001, 820.7, 607.8, 14);				break; //Scarlet Sancuary
		case 853:		Plr->EventTeleport(1007, 284.73, 90.19, 114);			break; //New Scholomance
		case 854:		Plr->EventTeleport(1030, 905.2, 2354.08, 98);			break; //Mogu Crypt
		case 855:		Plr->EventTeleport(1031, 3795.64, 533.61, 641);			break; //Proving Grounds
		case 856:		Plr->EventTeleport(1043, 2077.03, -4760.9, 86.7759);	break; //Ring of valor

		case 701:		Plr->EventTeleport(654, -1451, 1403, 36);				break; //gilneas
		case 702:		Plr->EventTeleport(648, -8423, 1361, 105);				break; //echo islands
		case 709:		Plr->EventTeleport(648, 603, 3128, 2);					break; //Lost islands
		case 703:	//TolBarad 
			{
				if( Plr->GetTeam() == 1)
					Plr->EventTeleport(732, -610, 1403, 22);			
				else
					Plr->EventTeleport(732, -357, 1046, 22);
			}break;
		case 704:		Plr->EventTeleport(646, 931, 506, -48);					break;//deepholm
		case 705:		Plr->EventTeleport(1, -9431, -959, 112);				break; //Uldum 
		case 706:	//Vashj'ir 
			{
				if( Plr->GetTeam() == 1)
					Plr->EventTeleport(0, -5668, 3721, 3);			
				else
					Plr->EventTeleport(0, -5314, 3917, 5);
			}break;
		case 707:		Plr->EventTeleport(1, 4634, -3823, 944);				break;//Mount Hyjal
		case 708:		Plr->EventTeleport(0, -4615, -6434, 5);					break;//Twilight Highlands

		case 361:                Plr->EventTeleport(571, 3739, 2163, 37.2);				break;//Azjol-Nerub
		case 362:                Plr->EventTeleport(571, 3647, 2045, 1.79);				break;//Ahn'kahet: The Old Kingdom
					//The Violet Hold
		case 308:              Plr->EventTeleport( 608, 1838, 804, 44.23);				break; 
					//The Obsidian Sanctum
		case 309:              Plr->EventTeleport( 571, 3457,262,-112);					break; 
					//Trial of the Champion
		case 310:              Plr->EventTeleport( 571, 8571,792,559);					break; 
					//Vault of Archavon
		case 400:              Plr->EventTeleport( 571, 5441,2840,421);					break;  
		case 220://Reset Talentpoints
			{
				uint32 price = 10000;
				uint32 currentgold = Plr->GetGold();
				if (currentgold>=price)
				{
					int32 newgold = currentgold - price;
					Plr->SetGold(newgold);
					Plr->BroadcastMessage("Your talentpoints are reset!");
					Plr->Reset_Talents();
				}
				else 
					Plr->BroadcastMessage("You don't have enough money to afford a talent reset!");
			}
			break;
		case 556://Repair Items
			{
			uint64 price = 20000;
			uint64 currentgold = Plr->GetGold();
			if (currentgold>=price)
			{
				Player *p_caster = Plr;
				for( int i = 0; i < EQUIPMENT_SLOT_END; i++ )
				{
					Item *pItem = p_caster->GetItemInterface()->GetInventoryItem( i );
					if( pItem != NULL )
					{
						if( pItem->IsContainer() )
						{
							Container *pContainer = static_cast<Container*>( pItem );
							for( unsigned int j = 0; j < pContainer->GetProto()->ContainerSlots; ++j )
							{
								pItem = pContainer->GetItem( j );
								if( pItem != NULL )
								{
		//							int32 cost = 0;
		//							if( cost <= (int32)p_caster->GetUInt32Value( PLAYER_FIELD_COINAGE ) )
									{
		//								p_caster->ModUnsigned32Value( PLAYER_FIELD_COINAGE, -cost );
										pItem->SetDurabilityToMax();
										pItem->m_isDirty = true;
									}
								}
							}
						}
						else
						{
							if( pItem->GetProto()->MaxDurability > 0 && i < INVENTORY_SLOT_BAG_END && pItem->GetDurability() <= 0 )
							{
		//						int32 cost = 0;
		//						if( cost <= (int32)p_caster->GetUInt32Value( PLAYER_FIELD_COINAGE ) )
								{
		//							p_caster->ModUnsigned32Value( PLAYER_FIELD_COINAGE, -cost );
									pItem->SetDurabilityToMax();
									pItem->m_isDirty = true;
								}
								p_caster->ApplyItemMods( pItem, i, true );
							}
							else
							{
								int64 cost = 0;
								if( cost <= p_caster->GetGold( ) )
								{
									p_caster->ModGold( -cost );
									pItem->SetDurabilityToMax();
									pItem->m_isDirty = true;
								}
							}
						}
					}
				}
				Plr->BroadcastMessage("Your items are repaired!");
			}
			else
			{
				Plr->BroadcastMessage("You need 2 gold to repair your items!");
			}
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem(0, "[Back]", 369);
		    Menu->SendTo(Plr);
			}
		break;
	case 369: //players tools and fixes menu
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem(0, "Fix Professions", 555);
//			Menu->AddItem(0, "Learn prof specialization", 557);
			Menu->AddItem(0, "Repair all my Items", 556);
			Menu->AddItem(0, "Remove shaman totems", 558);
			Menu->AddItem(11, "Remove Resurrection Sickness", 97);
//			Menu->AddItem(9, "Make This Place Your Home", 95);
//			Menu->AddItem(10, "Reset Talentpoints", 220);
			Menu->AddItem(0, "Learn class missing Spells and Proficiencies", 603);
			Menu->AddItem(0, "Disband Arena Team 2v2", 606);
			Menu->AddItem(0, "Disband Arena Team 3v3", 607);
			Menu->AddItem(0, "Disband Arena Team 5v5", 608);
			Menu->AddItem(0, "Reset Daily quests", 609);
			Menu->AddItem(0, "Watch Wow Movies", 610);
			Menu->AddItem(0, "[Back]", 99);
			Menu->SendTo(Plr);
		break;
	case 606:
		{
			if( Plr->m_arenaTeams[ ARENA_TEAM_TYPE_2V2 ] && Plr->m_arenaTeams[ ARENA_TEAM_TYPE_2V2 ]->m_leader == Plr->GetLowGUID() )
			{
				Plr->m_arenaTeams[ ARENA_TEAM_TYPE_2V2 ]->Destroy();
				Plr->BroadcastMessage("Your Arena team has been disbanded!");
			}
			else
				Plr->BroadcastMessage("Could not find arena team to disband!");
			Plr->CloseGossip();
		}break;
	case 607:
		{
			if( Plr->m_arenaTeams[ ARENA_TEAM_TYPE_3V3 ] && Plr->m_arenaTeams[ ARENA_TEAM_TYPE_3V3 ]->m_leader == Plr->GetLowGUID() )
			{
				Plr->m_arenaTeams[ ARENA_TEAM_TYPE_3V3 ]->Destroy();
				Plr->BroadcastMessage("Your Arena team has been disbanded!");
			}
			else
				Plr->BroadcastMessage("Could not find arena team to disband!");
			Plr->CloseGossip();
		}break;
	case 608:
		{
			if( Plr->m_arenaTeams[ ARENA_TEAM_TYPE_5V5 ] && Plr->m_arenaTeams[ ARENA_TEAM_TYPE_5V5 ]->m_leader == Plr->GetLowGUID() )
			{
				Plr->m_arenaTeams[ ARENA_TEAM_TYPE_5V5 ]->Destroy();
				Plr->BroadcastMessage("Your Arena team has been disbanded!");
			}
			else
				Plr->BroadcastMessage("Could not find arena team to disband!");
			Plr->CloseGossip();
		}break;
	case 609:	//check player quests and see which ones should be moved to the repeatable quests list
		{
			bool found_any = false;
			std::set<uint32>::iterator itr,itr2;
			for(itr = Plr->m_finishedQuests.begin(); itr != Plr->m_finishedQuests.end(); )
			{
				uint32 questid = *itr;
				itr2 = itr;
				itr++;
				Quest *q = QuestStorage.LookupEntry( questid );
				if( q )
				{
					if( q->is_repeatable == arcemu_QUEST_REPEATABLE_DAILY || q->is_repeatable == arcemu_QUEST_REPEATABLE_WEEKLY )
					{
						if( q->is_repeatable == arcemu_QUEST_REPEATABLE_DAILY )
							Plr->PushToFinishedDailies( questid );
						if( q->is_repeatable == arcemu_QUEST_REPEATABLE_DAILY )
							Plr->PushToFinishedWeeklies( questid );
						Plr->BroadcastMessage("Force finished quest : %s",q->title);
						Plr->m_finishedQuests.erase( itr2 );
						found_any = true;
					}
				}
			}
			if( found_any )
				Plr->BroadcastMessage("You should be able to finish these repeatable quest after next reset interval");
			else
				Plr->BroadcastMessage("Could not find any quests that need to forced set as repeatable");
			Plr->CloseGossip();
		}break;
	case 610: //players tools and fixes menu
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem(0, "Intro", 630);
			Menu->AddItem(0, "Burning Crusade", 631);
			Menu->AddItem(0, "WOTLK", 632);
			Menu->AddItem(0, "At the wrath gate", 633);
			Menu->AddItem(0, "Fall of the Lich King", 634);
			Menu->AddItem(0, "Cataclysm", 635);
			Menu->AddItem(0, "Worgen outro", 636);
			Menu->AddItem(0, "Goblin Outro", 637);
			Menu->SendTo(Plr);
		break;
	case 630: Plr->CloseGossip(); Plr->SendTriggerMovie(2);  break;
	case 631: Plr->CloseGossip(); Plr->SendTriggerMovie(27); break;
	case 632: Plr->CloseGossip(); Plr->SendTriggerMovie(18); break;
	case 633: Plr->CloseGossip(); Plr->SendTriggerMovie(14); break;
	case 634: Plr->CloseGossip(); Plr->SendTriggerMovie(16); break;
	case 635: Plr->CloseGossip(); Plr->SendTriggerMovie(23); break;
	case 636: Plr->CloseGossip(); Plr->SendTriggerMovie(21); break;
	case 637: Plr->CloseGossip(); Plr->SendTriggerMovie(22); break;
	case 555: //Profession Fixes menu
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem(8, "Fix enchanting", 596);
			Menu->AddItem(8, "Fix tailoring", 595);
			Menu->AddItem(8, "Fix engineering", 594);
			Menu->AddItem(8, "Fix jewelcrafting", 593);
			Menu->AddItem(8, "Fix alchemy", 592);
			Menu->AddItem(8, "Fix black smith",591);
			Menu->AddItem(8, "Fix herbalism", 590);
			Menu->AddItem(8, "Fix leather working", 589);
			Menu->AddItem(8, "Fix mining", 588);
			Menu->AddItem(8, "Fix skinning", 587);
			Menu->AddItem(8, "Fix cooking", 586);
			Menu->AddItem(8, "Fix first aid", 585);
			Menu->AddItem(8, "Fix fishing", 584);
			Menu->AddItem(8, "Fix inscription", 597);
			Menu->AddItem(8, "Fix archaeology", 598);
			Menu->AddItem(0, "[Back]", 99);
			Menu->SendTo(Plr);
		break;
	case 600:
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem(0, "Reset Faction Standings to new char", 559);
			Menu->AddItem(0, "Remove profession skills and give 2 slots back(in case dual spec bug)", 601);
//			Menu->AddItem(0, "Reset all spells to as new char(including professions). In case missing racial or buged chars.", 602);
			Menu->AddMenuItem(0, "Reset all spells to as new char(including professions). In case missing racial or buged chars.",0,0,"This will reset mounts and professions. You will not get a refund! Continue ?",0,0,602);
			Menu->AddItem(0, "Remove all auras(even ones that are invisible).Strange bugs", 604);
			Menu->AddItem(0, "Resend achievement rewards.(10k gold antispam prot)", 605);
			Menu->AddItem(0, "Remove A Profession", 560);
			Menu->AddItem(0, "[Back]", 99);
			Menu->SendTo(Plr);
		break;
	case 560: //Profession Fixes menu
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem(8, "Remove enchanting", 561);
			Menu->AddItem(8, "Remove tailoring", 562);
			Menu->AddItem(8, "Remove engineering", 563);
			Menu->AddItem(8, "Remove jewelcrafting", 564);
			Menu->AddItem(8, "Remove alchemy", 565);
			Menu->AddItem(8, "Remove black smith",566);
			Menu->AddItem(8, "Remove herbalism", 567);
			Menu->AddItem(8, "Remove leather working", 568);
			Menu->AddItem(8, "Remove mining", 569);
			Menu->AddItem(8, "Remove skinning", 570);
			Menu->AddItem(8, "Remove cooking", 571);
			Menu->AddItem(8, "Remove first aid", 572);
			Menu->AddItem(8, "Remove fishing", 573);
			Menu->AddItem(8, "Remove inscription", 574);
			Menu->AddItem(8, "Remove archaeology", 575);
			Menu->AddItem(0, "[Back]", 99);
			Menu->SendTo(Plr);
		break;
	case 559: //Reset Faction Standing to new char
		{
			Plr->ClearReputations();
//			Plr->reputationByListId.empty();
			Plr->_InitialReputation();
//			Plr->smsg_InitialFactions();
			Plr->BroadcastMessage("Your faction standing has been reseted.Needs a relog!");
			Plr->CloseGossip();
		}break;
	case 605:
		{
			if( Plr->GetGold() < 100000000 )
			{
				Plr->BroadcastMessage("you need 10k gold to get rewards again for achievements!");
				Plr->CloseGossip();
				return;
			}
			Plr->ModGold( -100000000 );
			std::map<uint32,AchievementVal*>::iterator itr;
			for( itr = Plr->m_sub_achievement_criterias.begin(); itr != Plr->m_sub_achievement_criterias.end(); itr++ )
				if( itr->second && itr->second->completed_at_stamp != 0 )
				{
					AchievementCriteriaEntry *achic = dbcAchievementCriteriaStore.LookupEntry( itr->first );
					AchievementEntry *achi = dbcAchievementStore.LookupEntry( achic->referredAchievement );
					Plr->GiveAchievementReward( achi );
				}
			Plr->BroadcastMessage("Your achievements have been reawarded!");
			Plr->CloseGossip();
		}break;
	//make sure player has 2 profession points and does not have any professions learned. Does not delete recepies
/*	case 604:
		uint32 possible_professions[] = {SKILL_INSCRIPTION,SKILL_ENCHANTING,SKILL_TAILORING,SKILL_ENGINEERING,SKILL_JEWELCRAFTING,SKILL_ALCHEMY,SKILL_BLACKSMITHING,SKILL_HERBALISM,SKILL_LEATHERWORKING,SKILL_MINING,SKILL_SKINNING};
		uint32 index_count = sizeof( possible_professions ) / sizeof( uint32 );
		break;*/
	//make sure player has 2 profession points and does not have any professions learned
	case 601:
		{
			//still need to add here the profession specializations 
			uint32 possible_professions[] = {56273,9787,10660,28672,26797,2656,26801,31252,28675,59390,26798,53042,51005,17039,17040,17041,20222,20219,55534,2580,2383,28677,10658,10656,13262,52175,9788,8613,8617,8618,10768,32678,50305,2575,2576,3564,10248,29354,50310,2108,3104,3811,10662,32549,51302,2366,2368,3570,11993,28695,50300,2018,3100,3538,9785,29844,51300,2259,3101,3464,11611,28596,51304,25229,25230,28894,28895,28897,51311,4036,4037,4038,12656,30350,51306,3908,3909,3910,12180,26790,51309,7411,7412,7413,13920,28029,51313,45357,45358,45359,45360,45361,45363};
			uint32 index_count = sizeof( possible_professions ) / sizeof( uint32 );
			for(uint32 i=0;i<index_count;i++)
			{
				Plr->removeSpell( possible_professions[i], false, false, 0 );
				Plr->removeDeletedSpell( possible_professions[i] );
			}
			Plr->removeSpellByHashName( SPELL_HASH_MASTER_OF_ANATOMY );
			Plr->removeSpellByHashName( SPELL_HASH_TOUGHNESS );
			Plr->_RemoveSkillLine( SKILL_INSCRIPTION, true );
			Plr->_RemoveSkillLine( SKILL_ENCHANTING, true );
			Plr->_RemoveSkillLine( SKILL_TAILORING, true );
			Plr->_RemoveSkillLine( SKILL_ENGINEERING, true );
			Plr->_RemoveSkillLine( SKILL_JEWELCRAFTING, true );
			Plr->_RemoveSkillLine( SKILL_ALCHEMY, true );
			Plr->_RemoveSkillLine( SKILL_BLACKSMITHING, true );
			Plr->_RemoveSkillLine( SKILL_HERBALISM, true );
			Plr->_RemoveSkillLine( SKILL_LEATHERWORKING, true );
			Plr->_RemoveSkillLine( SKILL_MINING, true );
			Plr->_RemoveSkillLine( SKILL_SKINNING, true );
			Plr->SetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1,0);
			Plr->SetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1+1,0);
			send_MainPage(pObject,Plr);
		}
		break;
	//remove all spells including talents from spellbook in case some spells are bugging professions menu
	case 602:
		Plr->Reset_Talents();
		Plr->Reset_Spells();
		Plr->SetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1,0);
		Plr->SetUInt32Value(PLAYER_PROFESSION_SKILL_LINE_1+1,0);
		send_MainPage(pObject,Plr);
		break;
	//like rogue dismantle,paladin resurrect,warrior stances..
	case 603:
		//armor specializations
		if( Plr->getClass() == WARRIOR )
			Plr->addSpell( 86526, false );		
		else if( Plr->getClass() == PALADIN )
			Plr->addSpell( 86525, false );		
		else if( Plr->getClass() == DEATHKNIGHT )
			Plr->addSpell( 86524, false );		
		else if( Plr->getClass() == ROGUE )
			Plr->addSpell( 86531, false );		
		else if( Plr->getClass() == HUNTER )
			Plr->addSpell( 86528, false );		
		else if( Plr->getClass() == DRUID )
			Plr->addSpell( 86530, false );		
		else if( Plr->getClass() == SHAMAN )
			Plr->addSpell( 86529, false );		
		else if( Plr->getClass() == WARLOCK )
			Plr->addSpell( 86091, false );		
		else if( Plr->getClass() == PRIEST )
			Plr->addSpell( 89745, false );		
		else if( Plr->getClass() == MAGE )
			Plr->addSpell( 89744, false );

		if( Plr->getClass() == WARRIOR )
		{
			Plr->addSpell( 750, false );		//Plate Proficiencies
			Plr->addSpell( 8737, false );		//Mail Proficiencies
			Plr->addSpell( 9077, false );		//Leather Proficiencies
			Plr->addSpell( 9116, false );		//Shield Proficiencies
			Plr->addSpell( 202, false );		//Two-Handed Swords Proficiencies
			Plr->addSpell( 199, false );		//Two-Handed Maces Proficiencies
			Plr->addSpell( 197, false );		//Two-Handed Axes Proficiencies
			Plr->addSpell( 227, false );		//Staves Proficiencies
			Plr->addSpell( 3018, false );		//Shoot Proficiencies
			Plr->addSpell( 200, false );		//Polearms Proficiencies
			Plr->addSpell( 201, false );		//One-Handed Swords Proficiencies
			Plr->addSpell( 198, false );		//One-Handed Maces Proficiencies
			Plr->addSpell( 196, false );		//One-Handed Axes Proficiencies
			Plr->addSpell( 266, false );		//Guns Proficiencies
			Plr->addSpell( 15590, false );		//Fist Weapons Proficiencies
			Plr->addSpell( 1180, false );		//Daggers Proficiencies
			Plr->addSpell( 5011, false );		//Crossbows Proficiencies
			Plr->addSpell( 264, false );		//Bows Proficiencies
//			Plr->addSpell( 86110, false );		//Plate Specialization
			if( Plr->getLevel() >= 10 )
			{
				Plr->addSpell( 71, false );		//Defensive Stance
				Plr->addSpell( 7386, false );	//Sunder Armor
				Plr->addSpell( 355, false );	//Taunt
				Plr->addSpell( 12678, false );	//Stance Mastery
			}
			if( Plr->getLevel() >= 30 )
			{
				Plr->addSpell( 2458, false );	//Berserker Stance
				Plr->addSpell( 20252, false );	//Intercept
			}
		}
		else if( Plr->getClass() == PALADIN )
		{
			Plr->addSpell( 750, false );		//Plate Proficiencies
			Plr->addSpell( 8737, false );		//Mail Proficiencies
			Plr->addSpell( 9077, false );		//Leather Proficiencies
			Plr->addSpell( 9116, false );		//Shield Proficiencies
			Plr->addSpell( 27762, false );		//Relic Proficiencies
			Plr->addSpell( 202, false );		//Two-Handed Swords Proficiencies
			Plr->addSpell( 199, false );		//Two-Handed Maces Proficiencies
			Plr->addSpell( 197, false );		//Two-Handed Axes Proficiencies
			Plr->addSpell( 200, false );		//Polearms Proficiencies
			Plr->addSpell( 201, false );		//One-Handed Swords Proficiencies
			Plr->addSpell( 198, false );		//One-Handed Maces Proficiencies
			Plr->addSpell( 196, false );		//One-Handed Axes Proficiencies
			Plr->addSpell( 86525, false );		//Plate Specialization
			
			if( Plr->getLevel() >= 20 )
			{
				Plr->addSpell( 5502, false );	//Sense Undead
				Plr->addSpell( 13819, false );	//Warhorse
				Plr->addSpell( 23214, false );	//Charger
			}
			if( Plr->getLevel() >= 12 && !Plr->HasSpell( 7328 ) )
			{
				Plr->addSpell( 7328, false );	//Redemption
				Plr->removeDeletedSpell( 10322 );//Redemption
				Plr->removeDeletedSpell( 10324 );//Redemption
				Plr->removeDeletedSpell( 20772 );//Redemption
				Plr->removeDeletedSpell( 20773 );//Redemption
				Plr->removeDeletedSpell( 48949 );//Redemption
				Plr->removeDeletedSpell( 48950 );//Redemption
			}
		}
		else if( Plr->getClass() == HUNTER )
		{
			Plr->addSpell( 8737, false );		//Mail Proficiencies
			Plr->addSpell( 9077, false );		//Leather Proficiencies
			Plr->addSpell( 202, false );		//Two-Handed Swords Proficiencies
			Plr->addSpell( 197, false );		//Two-Handed Axes Proficiencies
			Plr->addSpell( 227, false );		//Staves Proficiencies
			Plr->addSpell( 200, false );		//Polearms Proficiencies
			Plr->addSpell( 201, false );		//One-Handed Swords Proficiencies
			Plr->addSpell( 196, false );		//One-Handed Axes Proficiencies
			Plr->addSpell( 266, false );		//Guns Proficiencies
			Plr->addSpell( 15590, false );		//Fist Weapons Proficiencies
			Plr->addSpell( 1180, false );		//Daggers Proficiencies
			Plr->addSpell( 5011, false );		//Crossbows Proficiencies
			Plr->addSpell( 264, false );		//Bows Proficiencies
			Plr->addSpell( 674, false );		//dual weild
//			Plr->addSpell( 86538, false );		//Mail Specialization
			if( Plr->getLevel() >= 20 )
			{
				Plr->addSpell( 1515, false );	//Tame Beast
				Plr->addSpell( 883, false );	//Call Pet
				Plr->addSpell( 2641, false );	//Dismiss Pet
				Plr->addSpell( 6991, false );	//Feed Pet
				Plr->addSpell( 982, false );	//Revive Pet
			}
		}
		else if( Plr->getClass() == ROGUE )
		{
			Plr->addSpell( 9077, false );		//Leather Proficiencies
			Plr->addSpell( 3018, false );		//Shoot Proficiencies
			Plr->addSpell( 201, false );		//One-Handed Swords Proficiencies
			Plr->addSpell( 198, false );		//One-Handed Maces Proficiencies
			Plr->addSpell( 196, false );		//One-Handed Axes Proficiencies
			Plr->addSpell( 266, false );		//Guns Proficiencies
			Plr->addSpell( 15590, false );		//Fist Weapons Proficiencies
			Plr->addSpell( 1180, false );		//Daggers Proficiencies
			Plr->addSpell( 5011, false );		//Crossbows Proficiencies
			Plr->addSpell( 264, false );		//Bows Proficiencies
//			Plr->addSpell( 86092, false );		//Leather Specialization
			if( Plr->getLevel() >= 20 )
				Plr->addSpell( 51722, false );	//dismantle
		}
		else if( Plr->getClass() == PRIEST )
		{
			Plr->addSpell( 5009, false );		//Wands Proficiencies
			Plr->addSpell( 227, false );		//Staves Proficiencies
			Plr->addSpell( 5019, false );		//Shoot Proficiencies
			Plr->addSpell( 198, false );		//One-Handed Maces Proficiencies
			Plr->addSpell( 1180, false );		//Daggers Proficiencies
			if( Plr->getLevel() >= 20 )
			{
				Plr->addSpell( 2944, false );	//Devouring Plague
				Plr->addSpell( 6346, false );	//Fear Ward
			}
		}
		else if( Plr->getClass() == DEATHKNIGHT )
		{
			Plr->addSpell( 48778, false );	//Acherus Deathcharger
			Plr->addSpell( 53428, false );	//Runeforging
			Plr->addSpell( 50977, false );	//Death Gate
			Plr->addSpell( 52665, false );		//Relic Proficiencies
			Plr->addSpell( 8737, false );		//Mail Proficiencies
			Plr->addSpell( 9077, false );		//Leather Proficiencies
			Plr->addSpell( 202, false );		//Two-Handed Swords Proficiencies
			Plr->addSpell( 199, false );		//Two-Handed Maces Proficiencies
			Plr->addSpell( 197, false );		//Two-Handed Axes Proficiencies
			Plr->addSpell( 200, false );		//Polearms Proficiencies
			Plr->addSpell( 201, false );		//One-Handed Swords Proficiencies
			Plr->addSpell( 198, false );		//One-Handed Maces Proficiencies
			Plr->addSpell( 196, false );		//One-Handed Axes Proficiencies
		}
		else if( Plr->getClass() == SHAMAN )
		{
			Plr->addSpell( 8737, false );		//Mail Proficiencies
			Plr->addSpell( 9077, false );		//Leather Proficiencies
			Plr->addSpell( 9116, false );		//Shield Proficiencies
			Plr->addSpell( 27763, false );		//Relic Proficiencies
			Plr->addSpell( 199, false );		//Two-Handed Maces Proficiencies
			Plr->addSpell( 197, false );		//Two-Handed Axes Proficiencies
			Plr->addSpell( 227, false );		//Staves Proficiencies
			Plr->addSpell( 198, false );		//One-Handed Maces Proficiencies
			Plr->addSpell( 196, false );		//One-Handed Axes Proficiencies
			Plr->addSpell( 15590, false );		//Fist Weapons Proficiencies
			Plr->addSpell( 1180, false );		//Daggers Proficiencies
//			Plr->addSpell( 86099, false );		//Mail Specialization or 86100 or 86108
			if( Plr->getLevel() >= 10 )
				Plr->addSpell( 3599, false );	//Searing Totem
			if( Plr->getLevel() >= 20 )
				Plr->addSpell( 5394, false );	//Healing Stream Totem
			if( Plr->getLevel() >= 4 )
				Plr->addSpell( 8071, false );	//Stoneskin Totem
		}
		else if( Plr->getClass() == MAGE )
		{
			Plr->addSpell( 5009, false );		//Wands Proficiencies
			Plr->addSpell( 227, false );		//Staves Proficiencies
			Plr->addSpell( 5019, false );		//Shoot Proficiencies
			Plr->addSpell( 201, false );		//One-Handed Swords Proficiencies
			Plr->addSpell( 1180, false );		//Daggers Proficiencies
			if( Plr->getLevel() >= 71 )
				Plr->addSpell( 53140, false );	//Teleport: Dalaran
			if( Plr->getLevel() >= 60 )
			{
				Plr->addSpell( 28272, false );	//Polymorph
			}
		}
		else if( Plr->getClass() == WARLOCK )
		{
			Plr->addSpell( 5009, false );		//Wands Proficiencies
			Plr->addSpell( 227, false );		//Staves Proficiencies
			Plr->addSpell( 688, false );		//Summon Imp
			Plr->addSpell( 5019, false );		//Shoot Proficiencies
			Plr->addSpell( 201, false );		//One-Handed Swords Proficiencies
			Plr->addSpell( 1180, false );		//Daggers Proficiencies
			if( Plr->getLevel() >= 10 )
				Plr->addSpell( 697, false );	//Summon Voidwalker
			if( Plr->getLevel() >= 20 )
			{
				Plr->addSpell( 5784, false );	//Felsteed
				Plr->addSpell( 712, false );	//Summon Succubus
			}
			if( Plr->getLevel() >= 30 )
				Plr->addSpell( 691, false );	//Summon Felhunter
			if( Plr->getLevel() >= 40 )
				Plr->addSpell( 23161, false );	//Dreadsteed
			if( Plr->getLevel() >= 50 )
				Plr->addSpell( 1122, false );	//Inferno
			if( Plr->getLevel() >= 60 )
				Plr->addSpell( 18540, false );	//Ritual of Doom
		}
		else if( Plr->getClass() == DRUID )
		{
//			Plr->addSpell( 84738, false );		//Celestial Focus
			Plr->addSpell( 9077, false );		//Leather Proficiencies
			Plr->addSpell( 27764, false );		//Relic Proficiencies
			Plr->addSpell( 199, false );		//Two-Handed Maces Proficiencies
			Plr->addSpell( 227, false );		//Staves Proficiencies
			Plr->addSpell( 200, false );		//Polearms Proficiencies
			Plr->addSpell( 198, false );		//One-Handed Maces Proficiencies
			Plr->addSpell( 15590, false );		//Fist Weapons Proficiencies
			Plr->addSpell( 1180, false );		//Daggers Proficiencies
//			Plr->addSpell( 86104, false );		//Leather Specialization - normal form
//			Plr->addSpell( 86097, false );		//Leather Specialization - cat form
//			Plr->addSpell( 86096, false );		//Leather Specialization - bear form
			if( Plr->getLevel() >= 16 )
				Plr->addSpell( 1066, false );	//Aquatic Form
			if( Plr->getLevel() >= 14 )
				Plr->addSpell( 8946, false );	//Cure Poison
			if( Plr->getLevel() >= 10 )
			{
				Plr->addSpell( 5487, false );	//Bear Form
				Plr->addSpell( 6795, false );	//Growl
				Plr->addSpell( 6807, false );	//Maul
			}
			if( Plr->getLevel() >= 71 )
				Plr->addSpell( 40120, false );	//Swift Flight Form
			if( Plr->getLevel() >= 85 )
			{
				Plr->addSpell( 88747, false );	//Wild Mushroom
				Plr->addSpell( 88751, false );	//Wild Mushroom: Detonate
			}
			//only need swift flight form
			if( Plr->HasSpell( 40120 ) )
				Plr->removeSpell( 33943, true, 0, 0 );
		}
		if( Plr->getRace() == RACE_TROLL )
		{
			Plr->addSpell( 26297, false );	//Berserking
			Plr->addSpell( 20557, false );	//Beast Slaying
			Plr->addSpell( 26290, false );	//Bow Specialization
		}
		else if( Plr->getRace() == RACE_BLOODELF )
		{
			Plr->removeSpell( 28734, false, false, 0 );	//mana tap
			if( Plr->getClass() == DEATHKNIGHT )
				Plr->addSpell( 50613, false );	//arcane torrent
			if( Plr->getClass() == DEATHKNIGHT )
				Plr->addSpell( 50613, false );	//arcane torrent
		}
		send_MainPage(pObject,Plr);
		break;
	//there are some strange invisible auras that remain on chars and make them bugged
	case 604:
		for(uint32 i = 0; i < MAX_PASSIVE_AURAS1(Plr); ++i)
			if( Plr->m_auras[i] != 0 ) 
				Plr->m_auras[i]->Remove();
		break;
/*	
	//the actions are not made yet
	case 557: //learn profession specialization
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem(8, "Armorsmithing", 5571);
			Menu->AddItem(8, "Weaponsmithing", 5572);
			Menu->AddItem(8, "Master Axesmithing", 5573);
			Menu->AddItem(8, "Master Hamersmithing", 5574);
			Menu->AddItem(8, "Master Swordsmithing", 5575);
			Menu->AddItem(8, "Gnomish engineering",5576);
			Menu->AddItem(8, "Goblin engineering", 5577);
			Menu->AddItem(8, "Dragonscale Leatherworking", 5578);
			Menu->AddItem(8, "Elemental leatherworking", 5579);
			Menu->AddItem(8, "Tribal Leatherworking", 5580);
			Menu->AddItem(8, "Mooncloth tailoring", 5581);
			Menu->AddItem(8, "Shadoweave tailoring", 5582);
			Menu->AddItem(8, "SpellFire Tailoring", 5583);
			Menu->AddItem(0, "[Back]", 99);
			Menu->SendTo(Plr);
		break;*/
	case 558: //Remove shaman totems from players since in 3.3 there are totems that replace them all
		if( Plr->GetItemInterface() )
		{
			Plr->GetItemInterface()->RemoveItemAmt(5175,1);
			Plr->GetItemInterface()->RemoveItemAmt(5176,1);
			Plr->GetItemInterface()->RemoveItemAmt(5177,1);
			Plr->GetItemInterface()->RemoveItemAmt(5178,1);
			Plr->CloseGossip();
		}
		break;
	case 598: 
		resetskilland_send_menu(pObject,Plr,SKILL_ARCHAEOLOGY,getmax_profession_archaeology(Plr));		
		break;
	case 597: 
		resetskilland_send_menu(pObject,Plr,SKILL_INSCRIPTION,getmax_profession_inscription(Plr));		
		break;
	case 596: 
		resetskilland_send_menu(pObject,Plr,SKILL_ENCHANTING,getmax_profession_enchant(Plr));		
		break;
	case 595: 
		resetskilland_send_menu(pObject,Plr,SKILL_TAILORING,getmax_profession_tailor(Plr));			
		break;
	case 594: 
		resetskilland_send_menu(pObject,Plr,SKILL_ENGINEERING,getmax_profession_engineering(Plr));		
		break;
	case 593: 
		resetskilland_send_menu(pObject,Plr,SKILL_JEWELCRAFTING,getmax_profession_jewelcrafting(Plr));		
		break;
	case 592: 
		{
			resetskilland_send_menu(pObject,Plr,SKILL_ALCHEMY,getmax_profession_alchemy(Plr));			
			uint32 currentskill = Plr->_GetSkillLineCurrent(SKILL_ALCHEMY,false);
			if( currentskill > 0 )
			{
				Plr->addSpell( 53042 );	//right now Mixology as perk can't be learned from trainers
				Plr->CastSpell( Plr, 53042, true );
			}
		}break;
	case 591: 
		resetskilland_send_menu(pObject,Plr,SKILL_BLACKSMITHING,getmax_profession_blacksmith(Plr));		
		break;
	case 590: 
		resetskilland_send_menu(pObject,Plr,SKILL_HERBALISM,getmax_profession_herbalism(Plr));			
		break;
	case 589: 
		resetskilland_send_menu(pObject,Plr,SKILL_LEATHERWORKING,getmax_profession_leatherwork(Plr));	
		break;
	case 588: 
		resetskilland_send_menu(pObject,Plr,SKILL_MINING,getmax_profession_mining(Plr));			
		break;
	case 587: 
		resetskilland_send_menu(pObject,Plr,SKILL_SKINNING,getmax_profession_skinning(Plr));			
		break;
	case 586: 
		resetskilland_send_menu(pObject,Plr,SKILL_COOKING,getmax_profession_cooking(Plr));			
		break;
	case 585: 
		resetskilland_send_menu(pObject,Plr,SKILL_FIRST_AID,getmax_profession_firstaid(Plr));			
		break;
	case 584: 
		resetskilland_send_menu(pObject,Plr,SKILL_FISHING,getmax_profession_fishing(Plr));			
		break;
	case 561:
			Plr->_RemoveSkillLine( SKILL_ENCHANTING, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_ENCHANTING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_ENCHANTING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 562:
			Plr->_RemoveSkillLine( SKILL_TAILORING, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_TAILORING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_TAILORING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 563:
			Plr->_RemoveSkillLine( SKILL_ENGINEERING, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_ENGINEERING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_ENGINEERING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 564:
			Plr->_RemoveSkillLine( SKILL_JEWELCRAFTING, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_JEWELCRAFTING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_JEWELCRAFTING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 565:
			Plr->_RemoveSkillLine( SKILL_ALCHEMY, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_ALCHEMY )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_ALCHEMY )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 566:
			Plr->_RemoveSkillLine( SKILL_BLACKSMITHING, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_BLACKSMITHING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_BLACKSMITHING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 568:
			Plr->_RemoveSkillLine( SKILL_LEATHERWORKING, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_LEATHERWORKING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_LEATHERWORKING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 569:
			Plr->_RemoveSkillLine( SKILL_MINING, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_MINING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_MINING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 570:
			Plr->_RemoveSkillLine( SKILL_SKINNING, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_SKINNING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_SKINNING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 571:
			Plr->_RemoveSkillLine( SKILL_COOKING, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_COOKING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_COOKING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 572:
			Plr->_RemoveSkillLine( SKILL_FIRST_AID, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_FIRST_AID )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_FIRST_AID )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 573:
			Plr->_RemoveSkillLine( SKILL_FISHING, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_FISHING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_FISHING )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 574:
			Plr->_RemoveSkillLine( SKILL_INSCRIPTION, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_INSCRIPTION )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_INSCRIPTION )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	case 575:
			Plr->_RemoveSkillLine( SKILL_ARCHAEOLOGY, true );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 ) == SKILL_ARCHAEOLOGY )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1, 0 );
			if( Plr->GetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1 ) == SKILL_ARCHAEOLOGY )
				Plr->SetUInt32Value( PLAYER_PROFESSION_SKILL_LINE_1 + 1, 0 );
			Plr->CloseGossip();
		break;
	}
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_fishing(Player* Plr)
{
	uint32 spells[]={0,7620,7731,7732,18248,33095,51294,88868};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_firstaid(Player* Plr)
{
	uint32 spells[]={0,3273,3274,7924,10846,27028,45542,74559};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_cooking(Player* Plr)
{
	uint32 spells[]={0,2550,3102,3413,18260,33359,51296,88053};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_skinning(Player* Plr)
{
	uint32 spells[]={0,8613,8617,8618,10768,32678,50305,74522};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_mining(Player* Plr)
{
	uint32 spells[]={0,2575,2576,3564,10248,29354,50310,74517};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_leatherwork(Player* Plr)
{
	uint32 spells[]={0,2108,3104,3811,10662,32549,51302,81199};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_herbalism(Player* Plr)
{
	uint32 spells[]={0,2366,2368,3570,11993,28695,50300,74519};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_blacksmith(Player* Plr)
{
	uint32 spells[]={0,2018,3100,3538,9785,29844,51300,76666};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_alchemy(Player* Plr)
{
	uint32 spells[]={0,2259,3101,3464,11611,28596,51304,80731};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_jewelcrafting(Player* Plr)
{
	uint32 spells[]={0,25229,25230,28894,28895,28897,51311,73318};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_engineering(Player* Plr)
{
	uint32 spells[]={0,4036,4037,4038,12656,30350,51306,82774};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_tailor(Player* Plr)
{
	uint32 spells[]={0,3908,3909,3910,12180,26790,51309,75156};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_enchant(Player* Plr)
{
	uint32 spells[]={0,7411,7412,7413,13920,28029,51313,74258};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_archaeology(Player* Plr)
{
	uint32 spells[]={0,78670,88961,89718,89719,89720,89721,89722};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

//in case we leaned next level of a profession but our skill maxed out then we advance max skill too
uint32 Warper::getmax_profession_inscription(Player* Plr)
{
	uint32 spells[]={0,45357,45358,45359,45360,45361,45363,86008};
	for(int32 i=7;i>0;i--)
		if( Plr->HasSpell( spells[ i ] ) ) 
		{
			//remove other ranks cause in 403 client freaks out from more then 2 professions
			for(int32 j=i-1;j>0;j--)
				Plr->removeSpell( spells[ j ], true, false, 0 );
			return 75*i;
		}
	return 0;
}

void Warper::resetskilland_send_menu(Object * pObject, Player* Plr,uint32 skill, uint32 maxskill)
{
	GossipMenu *Menu;
	objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
	Menu->AddItem(0, "[Back]", 99);
	Menu->SendTo(Plr);

	if ( maxskill != 0 )
	{
		uint32 currentskill = Plr->_GetSkillLineCurrent(skill,false);
		if( currentskill > maxskill )
			currentskill = maxskill; //should not happen but let us be safe
		Plr->_RemoveSkillLine(skill);
		Plr->_AddSkillLine(skill, currentskill, maxskill);
		Creature * pCreature = (Creature *)pObject; // we already checked if he is creature in main menu
		pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Profession has been fixed" );
	}
	else
	{
		Plr->_RemoveSkillLine( skill ); //maybe he got stuck with the skill and he is missing the spell
		Plr->BroadcastMessage("You don't have this proffesion learned!");
	}
}

void Warper::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupWarper(ScriptMgr * mgr)
{
	GossipScript * gs = (GossipScript*) new Warper();
    /* Teleporter List */
    mgr->register_gossip_script(123456, gs);          // Nega

//	replace INTO items (`entry`,`class`,`subclass`,`field4`,`name1`,`name2`,`name3`,`name4`,`displayid`,`quality`,`flags`,`buyprice`,`sellprice`,`inventorytype`,`allowableclass`,`allowablerace`,`itemlevel`,`requiredlevel`,`RequiredSkill`,`RequiredSkillRank`,`RequiredPlayerRank1`,`RequiredPlayerRank2`,`RequiredFaction`,`RequiredFactionStanding`,`Unique`,`maxcount`,`ContainerSlots`,`stat_type1`,`stat_value1`,`stat_type2`,`stat_value2`,`stat_type3`,`stat_value3`,`stat_type4`,`stat_value4`,`stat_type5`,`stat_value5`,`stat_type6`,`stat_value6`,`stat_type7`,`stat_value7`,`stat_type8`,`stat_value8`,`stat_type9`,`stat_value9`,`stat_type10`,`stat_value10`,`dmg_min1`,`dmg_max1`,`dmg_type1`,`dmg_min2`,`dmg_max2`,`dmg_type2`,`armor`,`holy_res`,`fire_res`,`nature_res`,`frost_res`,`shadow_res`,`arcane_res`,`delay`,`ammo_type`,`range`,`spellid_1`,`spelltrigger_1`,`spellcharges_1`,`spellcooldown_1`,`spellcategory_1`,`spellcategorycooldown_1`,`spellid_2`,`spelltrigger_2`,`spellcharges_2`,`spellcooldown_2`,`spellcategory_2`,`spellcategorycooldown_2`,`spellid_3`,`spelltrigger_3`,`spellcharges_3`,`spellcooldown_3`,`spellcategory_3`,`spellcategorycooldown_3`,`spellid_4`,`spelltrigger_4`,`spellcharges_4`,`spellcooldown_4`,`spellcategory_4`,`spellcategorycooldown_4`,`spellid_5`,`spelltrigger_5`,`spellcharges_5`,`spellcooldown_5`,`spellcategory_5`,`spellcategorycooldown_5`,`bonding`,`description`,`page_id`,`page_language`,`page_material`,`quest_id`,`lock_id`,`lock_material`,`sheathID`,`randomprop`,`unk203_1`,`block`,`itemset`,`MaxDurability`,`ZoneNameID`,`mapid`,`bagfamily`,`TotemCategory`,`socket_color_1`,`unk201_3`,`socket_color_2`,`unk201_5`,`socket_color_3`,`unk201_7`,`socket_bonus`,`GemProperties`,`ReqDisenchantSkill`,`unk2`) VALUES ('66000','0','-1','-1','Taxi','','','','1102','6','0','100000','0','0','-1','-1','0','0','0','0','0','0','0','0','0','500','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41616','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','Use to open Taxi menu','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','','0','0','0','0','0','0','0','','-1','0');
//	mgr->register_item_gossip_script(66000,gs);
}
