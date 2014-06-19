/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Morha					#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class MorjaTalk : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MorjaTalk);
	MorjaTalk(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		// Speaks 4:25
		RegisterAIUpdateEvent(265000);
	}

	void AIUpdate()
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Jubjub? Where are you, Jubjub? Oh no! Where did you go this time!" );
		//_unit->Emote(5);
	}

};

void SetupMorjaTalk(ScriptMgr *mgr)
{
	mgr->register_creature_script(14871, &MorjaTalk::Create);
}