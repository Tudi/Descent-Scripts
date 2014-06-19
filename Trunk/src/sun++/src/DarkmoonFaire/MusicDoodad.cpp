/* 
#################################
#  The Darkmoon Faire Project	#
# ------------------------------#
# Author: Nexis					#
# Script: Music Doodad			#
#################################
*/

#include "StdAfx.h"
#include "Setup.h"

class DarkmoonFaireMusic : public CreatureAIScript
{
public:
  ADD_CREATURE_FACTORY_FUNCTION(DarkmoonFaireMusic);
  DarkmoonFaireMusic(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnLoad()
		{
			RegisterAIUpdateEvent(1000);
		}

		void AIUpdate()
		{
			_unit->PlaySoundToSet(8440);
		}

};

void SetupMusicDoodad(ScriptMgr *mgr)
{
	mgr->register_creature_script(500000, &DarkmoonFaireMusic::Create);
}