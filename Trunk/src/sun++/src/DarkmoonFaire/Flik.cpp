/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Flik					#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class Flik : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Flik);
	Flik(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// Speaks 3:00
		RegisterAIUpdateEvent(180000);
	}

	void AIUpdate()
	{
		int RandomText;
			RandomText=rand()%4;
			switch (RandomText)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "It's getting away!" );
			break;

			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Get back here!" );
			break;

			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Hey, someone help me catch this thing!" );
			break;

			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Frogs and leather balls for sale!" );
			break;

			}
	}

};

void SetupFlik(ScriptMgr *mgr)
{
	mgr->register_creature_script(14860, &Flik::Create);
}