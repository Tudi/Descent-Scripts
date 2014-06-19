#include "StdAfx.h"
#include "Setup.h"

class SCRIPT_DECL RealmHelperGossip : public GossipScript
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

void RealmHelperGossip::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
		return;
	GossipMenu *Menu;
	objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
	Menu->AddItem(0, "General information",17);
	Menu->AddItem(0, "I am stuck, what do I do ?",18);
	Menu->AddItem(0, "How do I get better gear (armor, weapons ) and gold ?",19);
	Menu->AddItem(0, "Arena Teams information",20);
	Menu->AddItem(0, "Guild Information",21);
	Menu->AddItem(0, "Donation Information",22);
    if(AutoSend)
        Menu->SendTo(Plr);
}
void RealmHelperGossip::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
		return;
	switch(IntId)
	{
		case 3: //Main
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->AddItem(0, "General information",17);
		  Menu->AddItem(0, "I am stuck, what do I do ?",18);
		  Menu->AddItem(0, "How do I get better gear (armor, weapons ) and gold ?",19);
		  Menu->AddItem(0, "Arena Teams information",20);
		  Menu->AddItem(0, "Guild Information",21);
		  Menu->AddItem(0, "Donation Information",22);
		  Menu->SendTo(Plr);
		}break;
		case 17: //General information
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->AddItem(0, "Rules",23);
		  Menu->AddItem(0, "TAXI NPCs",24);
		  Menu->AddItem(0, "Meeting other players",25);
		  Menu->AddItem(0, "Back",3);
		  Menu->SendTo(Plr);
		}break;
		case 18: //I am stuck, what do I do ?
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20005 );
		  Menu->AddItem(0, "Back",3);
		  Menu->AddItem(0, "Close",666);
		  Menu->SendTo(Plr);
		}break;
		case 19: //How do I get better gear (armor, weapons ) and gold ?
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20012 );
		  Menu->AddItem(0, "Back",3);
		  Menu->AddItem(0, "Close",666);
		  Menu->SendTo(Plr);
		}break;
		case 20: //Arena Teams information
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20003 );
		  Menu->AddItem(0, "Where do I get an arena charter ?",31);
		  Menu->AddItem(0, "Arena Team commands",32);
		  Menu->AddItem(0, "Back",3);
		  Menu->SendTo(Plr);
		}break;
		case 21: //Guild Information
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->AddItem(0, "How to create a Guild ?",35);
		  Menu->AddItem(0, "Guild commands",37);
		  Menu->AddItem(0, "Back",3);
		  Menu->SendTo(Plr);
		}break;
		case 22: //Donation Information
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20011 );
		  Menu->AddItem(0, "Agreement",39);
		  Menu->AddItem(0, "Problems with your donation?",40);
		  Menu->AddItem(0, "Back",3);
		  Menu->SendTo(Plr);
		}break;
		case 23: //Rules
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20002 );
		  Menu->AddItem(0, "Back",17);
		  Menu->AddItem(0, "Close",666);
		  Menu->SendTo(Plr);
		}break;
		case 24: //TAXI NPCs
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20007 );
		  Menu->AddItem(0, "Back",17);
		  Menu->AddItem(0, "Close",666);
		  Menu->SendTo(Plr);
		}break;
		case 25: //Meeting other players
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20013 );
		  Menu->AddItem(0, "Back",17);
		  Menu->AddItem(0, "Close",666);
		  Menu->SendTo(Plr);
		}break;
		case 31: //Where do I get an arena charter ?
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20010 );
		  Menu->AddItem(0, "Back",20);
		  Menu->AddItem(0, "Close",666);
		  Menu->SendTo(Plr);
		}break;
		case 32: //Arena Team commands
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20001 );
		  Menu->AddItem(0, "Back",20);
		  Menu->AddItem(0, "Close",666);
		  Menu->SendTo(Plr);
		}break;
		case 35: //How to create a Guild ?
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20006 );
		  Menu->AddItem(0, "Back",21);
		  Menu->AddItem(0, "Close",666);
		  Menu->SendTo(Plr);
		}break;
		case 37: //Guild commands
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20000 );
		  Menu->AddItem(0, "Back",21);
		  Menu->AddItem(0, "Close",666);
		  Menu->SendTo(Plr);
		}break;
		case 39: //Agreement
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20008 );
		  Menu->AddItem(0, "Back",22);
		  Menu->AddItem(0, "Close",666);
		  Menu->SendTo(Plr);
		}break;
		case 40: //Issue
		{
		  GossipMenu *Menu;
		  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		  Menu->SetTextID( 20004 );
		  Menu->AddItem(0, "Back",22);
		  Menu->AddItem(0, "Close",666);
		  Menu->SendTo(Plr);
		}break;
		case 666: Plr->CloseGossip(); break;
	};
}

void RealmHelperGossip::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupRealmHelperGossip(ScriptMgr * mgr)
{
	GossipScript * gs = (GossipScript*) new RealmHelperGossip();
    mgr->register_gossip_script(123461, gs);     
}
/*
INSERT INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `type`, `male_displayid`) VALUES
(123461, 'Hot Chick', 'WoWBeeZ Helper', 'Directions', 7, 100);

REPLACE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`,  `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`) VALUES
(123461, 96, 96, 35, 100000, 100000, 1.25, 8195, 1000, 0, 100, 200, 0, 0, 0, 360000000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 1.8, 0.5, '0', 0, 0, 0, 0, 2.5, 8, 14, 0);

insert into npc_text (entry,text0_0,prob0) values (20000,'You can use the following commands to manage a guild: $B /gdisband – Disbands the entire guild. You must be the guild leader to use this. $B /ginfo - Gives basic information about your guild. $B /ginvite, /guildinvite [player] - Invites [player] or your current target into your guild. You must be an officer to use this command. $B /gleader, /guildleader [player] - Changes the guild leader to [player] or your current target. The player must already be a guild member. Only the guild leader can use this. $B /gquit, /guildquit - Leaves your current guild. $B /gmotd, /guildmotd - Changes the guild Message of the Day to . You must be an officer to use this command. $B /gpromote, /guildpromote [player] - Promotes [player] or your current target. $B /gdemote, /guilddemote [player] - Demotes [player] or your current target. $B /gremove, /guildremove - Removes [player] or your current target from your guild. You must be an officer to use this command. ',1);
insert into npc_text (entry,text0_0,prob0) values (20001,'You can use the following commands to manage an arena team: $B /teaminvite [2v2, 3v3, 5v5] [PLAYER] – Invites someone to your arena team. $B Example: to invite someone in a 2v2 team type: /teaminvite 2v2 Player_name_ $B /teamremove [2v2, 3v3, 5v5] [PLAYER] – Removes someone from your arena team. $B Example: to remove someone from a 2v2 team type: /teamremove 2v2 Player_name $B /teamquit [2v2, 3v3, 5v5] – Makes you leave your current arena team. $B Example: to leave a 2v2 team type: /teamquit 2v2 $B /teamdisband [2v2, 3v3, 5v5] – Disbands your arena team (you need to be the arena captain to use this command). $B Example: to disband a 2v2 team type: /teamdisband 2v2 $B /teamcaptain [2v2, 3v3, 5v5] [PLAYER] – Makes PLAYER the new team captain. $B Example: for a 2v2 team type: /teamcaptain 2v2 Player_name ',1);
insert into npc_text (entry,text0_0,prob0) values (20002,'All rules can be found at $Bhttp://forum.wowbeez.com/forumdisplay.php?fid=38 $B$B Thread: “x500 Realm Rules and Regulations” ',1);
insert into npc_text (entry,text0_0,prob0) values (20003,'Arena teams can have different sizes (2v2, 3v3 or 5v5). In order to create an Arena team you need an Arena Charter.',1);
insert into npc_text (entry,text0_0,prob0) values (20004,'I have donated but haven’t received my reward yet, what do I do ? $B In case the item hasn’t arrived within 10-15 minutes, you should create a ticket at $B https://www.wowbeez.com/contact.php $B Select subject: $ Donation Ticket. $B Please make sure to !INCLUDE! the donation information in the ticket. ',1);
insert into npc_text (entry,text0_0,prob0) values (20005,'If you are stuck you can use the “Unstuck character” tool on the website, found at: $B https://www.wowbeez.com/tools_unstuck.php ',1);
insert into npc_text (entry,text0_0,prob0) values (20006,'In order to create a Guild you need a Guild Charter. This item can be obtained at a Guild Master NPC in any major city. A Guild Charter costs 80 gold. After receiving the Guild Charter item in your inventory, talk to the Guild Master again and click on “Register a Guild Charter”. You have now created a new guild. $B $B For a complete list of Guild Masters visit: $B http://www.wowhead.com/search?q=guild+master ',1);
insert into npc_text (entry,text0_0,prob0) values (20007,'In order to shorten the travel time, you can use the TAXI to instantly teleport to various locations including major cities, instances, raids and many other places. $B The TAXI also features a “Tools” menu which you can use when you are having problems (stuck proffessions, removing bugged auras or debuffs etc) ',1);
insert into npc_text (entry,text0_0,prob0) values (20008,'Please make sure you read,understand and agree with the donation agreement. You are not buying items, we do not own you anything for helping us keep the server alive. If you do it, you do it for yourself and for others that play for free. Almost all donor gifts can be obtained ingame ! Breaking the agreement means you wish to harm us and others and we will try to make sure you will not bother the community.',1);
insert into npc_text (entry,text0_0,prob0) values (20010,'To get an Arena Charter you can go to Tanaris, Gadgetzan and talk to the Arena Organizer there. An arena charter costs 80 gold. After receiving the arena charter in your inventory, talk to the Arena organizer again and Turn in the charter. You have now created an arena team. $B $B For a complete list of Arena Organizers visit: $B http://www.wowhead.com/search?q=arena+organizer ',1);
insert into npc_text (entry,text0_0,prob0) values (20011,'Why donate ? We have to pay the server hosting ourselves and donations help us keep the server online. Each player that donates will be rewarded with gold, items or professions at his choice.',1);
insert into npc_text (entry,text0_0,prob0) values (20012,'You can get better gear by joining instances (dungeons) or raids with other players. $B Gold can easily be gained by killing mobs anywhere in the game. Because this realm multiplies the drop rates 500 times, you will find that gold gaining has never been easier. ',1);
insert into npc_text (entry,text0_0,prob0) values (20013,'You can meet up with other players at the WoWBeeZ Mall (TAXI teleport option) and also by joining the Global Chat – type /join global in the chat box.',1);

INSERT INTO gossip_menu VALUES("17", "", "20009", "General information", "3", "0", "", "0");
INSERT INTO gossip_menu VALUES("18", "", "0", "I am stuck, what do I do ?", "3", "0", "", "0");
INSERT INTO gossip_menu VALUES("19", "", "0", "How do I get better gear (armor, weapons ) and gold ?", "3", "0", "", "0");
INSERT INTO gossip_menu VALUES("20", "", "0", "Arena Teams information", "3", "0", "", "0");
INSERT INTO gossip_menu VALUES("21", "", "0", "Guild Information", "3", "0", "", "0");
INSERT INTO gossip_menu VALUES("22", "", "0", "Donation Information", "3", "0", "", "0");
INSERT INTO gossip_menu VALUES("23", "", "0", "Rules", "17", "0", "", "0");
INSERT INTO gossip_menu VALUES("24", "", "0", "TAXI NPCs", "17", "0", "", "0");
INSERT INTO gossip_menu VALUES("25", "", "0", "Meeting other players", "17", "0", "", "0");
INSERT INTO gossip_menu VALUES("26", "All rules can be found at http://forum.wowbeez.com/forumdisplay.php?fid=38 $B Thread: “x500 Realm Rules and Regulations” ", "20002", "back", "23", "0", "", "0");
INSERT INTO gossip_menu VALUES("27", "In order to shorten the travel time, you can use the TAXI to instantly teleport to various locations including major cities, instances, raids and many other places. $B The TAXI also features a “Tools” menu which you can use when you are having problems (stuck proffessions,  removing bugged auras or debuffs etc) ", "20007", "back", "24", "0", "", "0");
INSERT INTO gossip_menu VALUES("28", "You can meet up with other players at the WoWBeeZ Mall (TAXI teleport option) and also by joining the Global Chat – type /join global in the chat box.", "20013", "back", "25", "0", "", "0");
INSERT INTO gossip_menu VALUES("29", "If you are stuck you can use the “Unstuck character” tool on the website, found at: $B https://www.wowbeez.com/tools_unstuck.php ", "20005", "back", "18", "0", "", "0");
INSERT INTO gossip_menu VALUES("30", "You can get better gear by joining instances (dungeons) or raids with other players.  $B Gold can easily be gained by killing mobs anywhere in the game. Because this realm multiplies the drop rates 500 times, you will find that gold gaining has never been easier. ", "20012", "back", "19", "0", "", "0");
INSERT INTO gossip_menu VALUES("31", "Arena teams can have different sizes (2v2, 3v3 or 5v5). In order to create an Arena team you need an Arena Charter.", "20003", "Where do I get an arena charter ?", "20", "0", "", "0");
INSERT INTO gossip_menu VALUES("32", "", "10003", "Arena Team commands", "20", "0", "", "0");
INSERT INTO gossip_menu VALUES("33", "To get an Arena Charter you can go to Tanaris, Gadgetzan and talk to the Arena Organizer there. An arena charter costs 80 gold. After receiving the arena charter in your inventory, talk to the Arena organizer again and Turn in the charter. You have now created an arena team. $B $B For a complete list of Arena Organizers visit:  $B http://www.wowhead.com/search?q=arena+organizer ", "20010", "back", "31", "0", "", "0");
INSERT INTO gossip_menu VALUES("34", "/teaminvite [2v2, 3v3, 5v5] [PLAYER] – Invites someone to your arena team.  $B Example: to invite someone in a 2v2 team type: /teaminvite 2v2 Player_name_ $B /teamremove [2v2, 3v3, 5v5] [PLAYER] – Removes someone from your arena team. $B Example: to remove someone from a 2v2 team type: /teamremove 2v2 Player_name $B /teamquit [2v2, 3v3, 5v5] – Makes you leave your current arena team.  $B Example: to leave a 2v2 team type: /teamquit 2v2 $B /teamdisband [2v2, 3v3, 5v5] – Disbands your arena team (you need to be the arena captain to use this command).  $B Example: to disband a 2v2 team type: /teamdisband 2v2 $B /teamcaptain [2v2, 3v3, 5v5] [PLAYER] – Makes PLAYER the new team captain.  $B Example: for a 2v2 team type: /teamcaptain 2v2 Player_name ", "20001", "back", "32", "0", "", "0");
INSERT INTO gossip_menu VALUES("35", "", "0", "How to create a Guild ?", "21", "0", "", "0");
INSERT INTO gossip_menu VALUES("36", "In order to create a Guild you need a Guild Charter. This item can be obtained at a Guild Master NPC in any major city. A Guild Charter costs 80 gold. After receiving the Guild Charter item in your inventory, talk to the Guild Master again and click on “Register a Guild Charter”. You have now created a new guild. $B $B For a complete list of Guild Masters visit: $B http://www.wowhead.com/search?q=guild+master ", "20006", "back", "35", "0", "", "0");
INSERT INTO gossip_menu VALUES("37", "", "0", "Guild commands", "21", "0", "", "0");
INSERT INTO gossip_menu VALUES("38", "/gdisband – Disbands the entire guild. You must be the guild leader to use this. $B /ginfo - Gives basic information about your guild. $B /ginvite, /guildinvite [player] - Invites [player] or your current target into your guild. You must be an officer to use this command. $B /gleader, /guildleader [player] - Changes the guild leader to [player] or your current target. The player must already be a guild member. Only the guild leader can use this.  $B /gquit, /guildquit - Leaves your current guild. $B /gmotd, /guildmotd <message> - Changes the guild Message of the Day to <message>. You must be an officer to use this command. $B /gpromote, /guildpromote [player] - Promotes [player] or your current target. $B /gdemote, /guilddemote [player] - Demotes [player] or your current target. $B /gremove, /guildremove - Removes [player] or your current target from your guild. You must be an officer to use this command. ", "20000", "back", "37", "0", "", "0");
INSERT INTO gossip_menu VALUES("39", "Why donate ? We have to pay the server hosting ourselves and donations help us keep the server online. Each player that donates will be rewarded with gold, items or professions at his choice.", "20011", "Agreement", "22", "0", "", "0");
INSERT INTO gossip_menu VALUES("40", "", "0", "Issue", "22", "0", "", "0");
INSERT INTO gossip_menu VALUES("41", "Please make sure you read,understand and agree with the donation agreement. You are not buying items, we do not own you anything for helping us keep the server alive. If you do it, you do it for yourself and for others that play for free. Almost all donor gifts can be obtained ingame ! Breaking the agreement means you wish to harm us and others and we will try to make sure you will not bother the community.", "20008", "back", "39", "0", "", "0");
INSERT INTO gossip_menu VALUES("42", "I have donated but haven’t received my reward yet, what do I do ? $B In case the item hasn’t arrived within 10-15 minutes, you should create a ticket at  $B https://www.wowbeez.com/contact.php $B Select subject: Donation Ticket. $B Please make sure to include the donation information in the ticket. ", "20004", "back", "40", "0", "", "0");

*/