/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Sylannia				#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class Sylannia : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Sylannia);
	Sylannia(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// She speaks every 2:50
		RegisterAIUpdateEvent(170000);
	}

	void AIUpdate()
	{
		int RandomText;
			RandomText=rand()%4;
			switch (RandomText)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Huzzah for the tipper! Step right up and get your delicious, cold drinks right here!" );
			break;

			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Just in... another batch of Darkmoon Special Reserve. Come get yours before they're all gone!" );
			break;

			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You there! Yes you! You look thirsty. Get over here, I have just what you need!" );
			break;

			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Cheap beer! Get your Cheap Beer right here!" );
			break;

			}
	}

};

void SetupSylannia(ScriptMgr *mgr)
{
	mgr->register_creature_script(14844, &Sylannia::Create);
}