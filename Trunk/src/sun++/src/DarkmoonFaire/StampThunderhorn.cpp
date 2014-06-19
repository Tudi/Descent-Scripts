/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Stamp Thunderhorn		#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class StampThunderhorn : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(StampThunderhorn);
	StampThunderhorn(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// She speaks every 2:55
		RegisterAIUpdateEvent(175000);
	}

	void AIUpdate()
	{
		int RandomText;
			RandomText=rand()%4;
			switch (RandomText)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Darkmoon Dogs fresh off the grill. Come and get em while they're hot!" );
			break;

			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Step right up! Get your Red Hot Wings right here!" );
			break;

			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I recommend the Darkmoon Special Reserve to wash down some Red Hot Wings. And if the future is what you seek, then run, don't walk, to speak with Sayge." );
			break;

			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "For a limited time only, Crunchy Frog available exclusively here at the Darkmoon Faire." );
			break;

			}
	}

};

void SetupStampThunderhorn(ScriptMgr *mgr)
{
	mgr->register_creature_script(14845, &StampThunderhorn::Create);
}