/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Gevas Grimegate		#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class GevasGrimegate : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GevasGrimegate);
	GevasGrimegate(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// Speaks 3:30
		RegisterAIUpdateEvent(210000);
	}

	void AIUpdate()
	{
		int RandomText;
			RandomText=rand()%4;
			switch (RandomText)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Tickets! Redeem your Darkmoon Faire Prize Tickets here! Wondrous and exotic prizes are waiting for you!" );
			break;

			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Five tickets or two-hundred and fifty, it doesn't matter friend, everybody is a winner!" );
			break;

			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "All it takes is five or more and you're on your way to the most wondrous prizes on all of Azeroth. Everybody is a winner!" );
			break;

			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Mysterious prizes await the adventurous. Step right up, step right up!" );
			break;

			}
	}

};

void SetupGevasGrimegate(ScriptMgr *mgr)
{
	mgr->register_creature_script(14828, &GevasGrimegate::Create);
}