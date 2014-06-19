/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Lhara					#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class Lhara : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Lhara);
	Lhara(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// Speaks 4:45
		RegisterAIUpdateEvent(285000);
	}

	void AIUpdate()
	{
		int RandomText;
			RandomText=rand()%4;
			switch (RandomText)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Come and take a look at my wares while you still can. We're here all week, and then we're off to some far off land." );
			break;

			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Darkmoon Faire Cards? See the Professor here for those. Come see me if you are a dealer in hard to come by antiquities and artifacts." );
			break;

			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Every item that I have for sale I acquired myself. You have my personal guarantee of authenticity." );
			break;

			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Have you any idea what I go through to bring these incomparable valuables to sell? Come see them all for yourself!" );
			break;

			}
	}

};

void SetupLhara(ScriptMgr *mgr)
{
	mgr->register_creature_script(14846, &Lhara::Create);
}