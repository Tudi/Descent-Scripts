/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Silas Darkmoon		#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class SilasDarkmoonSpeak : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(SilasDarkmoonSpeak);
	SilasDarkmoonSpeak(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// Speaks 3:00
		RegisterAIUpdateEvent(180000);
	}

	void AIUpdate()
	{
		int RandomText;
			RandomText=rand()%6;
			switch (RandomText)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Welcome one and all to the greatest show on all of Azeroth... the Darkmoon Faire! Adventure and excitement await, don't be shy! Mysteries and prizes for both the young and the old are here for the taking. And be sure to speak with the professor about any Darkmoon Cards that you have come across during your travels!" );
			break;

			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come one, come all. Welcome to the Darkmoon Faire! Don't be shy. Step right up to Lhara and buy yourself an exotic artifact from far off lands. If you're one of the lucky few who have found Darkmoon Cards, have a word with Professor Paleo. Hungry? Thirsy? You're in luck! Refreshments are available right here from Sylannia and Stamp. I recommend the Darkmoon Special Reserve to wash down some Red Hot Wings. And if the future is what you seek, then run, don't walk, to speak with Sayge." );
			break;

			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "We're back and better than ever! It's the Darkmoon Faire friend, and it's your lucky day! Sparing no expense, we've gathered wonders and treats from around the world for your delight. Wheather you're young or old, rich or poor, the Darkmoon Faire has it all! Be sure to turn in your Darkmoon Faire Prize Tickets to Gelvas Grimegate, and choose from several exotic and wonderous prizes!" );
			break;

			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Welcome one and all to the Darkmoon Faire, the greatest event in the world! We have it all... delicious food, strong drink, exotic artifacts, fortunes read, amazing prizes and excitement without end! Don't forget to turn in your Darkmoon Faire Prize Tickets to Gelvas Grimegate! All it takes is five or more and you're on your way to the most wonderous prizes on all of Azeroth. Everybody is a winner!" );
			break;

			case 4:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Everyone enjoying themselves so far? That's great! Welcome to the Darkmoon Faire, the greatest show on all of Azeroth! Make sure you speak with Tebb and his friends here while you're taking in Neblegear's Darkmoon Zoo Bizarre." );
			break;

			case 5:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Greetings friends, and welcome to the greatest show on Azeroth!$B$BPlease, step right up and take in all we have to offer. Ride the rides and see the sights! Amaze at the wonders that the Darkmoon Faire has uncovered in this vast and mysterious world! We have spared no expense in bringing you excitement that children of all ages will delight in!" );
			break;

			}
	}

};

class SCRIPT_DECL SilasDarkmoon : public GossipScript{
public:
	void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
	void GossipEnd(Object * pObject, Player* Plr){ GossipScript::GossipEnd(pObject, Plr); }
	void Destroy(){ delete this; }
};

void SilasDarkmoon::GossipHello(Object * pObject, Player* Plr, bool AutoSend){
	GossipMenu *Menu;

	// Main text
	objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 60013, Plr);
	Menu->AddItem(0, "Silas, why is most everything at the fair free? How do you make a profit?", 1);

	if(AutoSend) 
		Menu->SendTo(Plr);
}

void SilasDarkmoon::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code){
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;

	if(pCreature==NULL)
		return;

    GossipMenu * Menu;
		switch(IntId)
		
		{
			case 1:
			// Silas, why is most everything at the fair free? How do you make a profit?
            objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 60014, Plr);
			
			Menu->SendTo(Plr);
			break;
		}
}

void SetupSilasDarkmoon(ScriptMgr * mgr)
	{
		GossipScript * gs = (GossipScript*) new SilasDarkmoon();
    	mgr->register_gossip_script(14823, gs);
	}

void SetupSilasDarkmoonSpeak(ScriptMgr *mgr)
{
	mgr->register_creature_script(14823, &SilasDarkmoonSpeak::Create);
}