/****************************************************************************
 *
 * SpellHandler Plugin
 * Copyright (c) 2007 Team Ascent
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons,
 * 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
 *
 * EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU
 * ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES
 * ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 */

#include "StdAfx.h"
#include "Setup.h"

class LadyMalandeCircleOfHealing : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(LadyMalandeCircleOfHealing);
	LadyMalandeCircleOfHealing(Spell* pSpell) : SpellScript(pSpell) {}
	bool IsValidTarget(Object* obj)
	{
		if (obj == NULL || !obj->IsCreature())
			return false;
		Creature* c=static_cast<Creature*>(obj);
		switch (c->GetEntry())
		{
		case 22949:
		case 22950:
		case 22951:
		case 22952:
			return true;
		default: return false;
		}
	}
};

void SetupRaidBlackTempleSpells(ScriptMgr* mgr)
{
	mgr->register_spell_script(41455, &LadyMalandeCircleOfHealing::Create);
}