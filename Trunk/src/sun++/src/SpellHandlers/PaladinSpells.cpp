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

/*
	How to add a new paladin spell to the dummy spell handler:

		1) Add a new function to handle the spell at the end of this file but before the
		   SetupPaladinSpells() function. SetupPaladinSpells() must always be the last function.

		2) Register the dummy spell by adding a new line to the end of the list in the
		   SetupPaladinSpells() function.

	Please look at how the other spells are handled and try to use the
	same variable names and formatting style in your new spell handler.
*/

// *****************************************************************************
// -----------------------------------------------------------------------------

bool HolyShock(uint32 i, Spell *pSpell)
{
	Unit *target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target) return true;

	uint32 newspell = 0;

	if(isAttackable(pSpell->p_caster,target)) // if its an enemy
	{
		switch(pSpell->GetProto()->Id)
		{
		case 20473:
			newspell = 25912;
			break;
		case 20929:
			newspell = 25911;
			break;
		case 20930:
			newspell = 25902;
			break;
		case 27174:
			newspell = 27176;
			break;
		case 33072:
			newspell = 33073;
			break;
		}
	}
	else // if its friendly
	{
		switch(pSpell->GetProto()->Id)
		{
		case 20473:
			newspell = 25914;
			break;
		case 20929:
			newspell = 25913;
			break;
		case 20930:
			newspell = 25903;
			break;
		case 27174:
			newspell = 27175;
			break;
		case 33072:
			newspell = 33074;
			break;
		}
	}

	SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
	if(!spInfo) return true;

	pSpell->p_caster->CastSpell(target, spInfo, true);
	return true;
}



// ADD NEW FUNCTIONS ABOVE THIS LINE
// *****************************************************************************

void SetupPaladinSpells(ScriptMgr * mgr)
{
/*	mgr->register_dummy_spell(20473, &HolyShock);           // Holy Shock rank 1
	mgr->register_dummy_spell(20929, &HolyShock);           // Holy Shock rank 2
	mgr->register_dummy_spell(20930, &HolyShock);           // Holy Shock rank 3
	mgr->register_dummy_spell(27174, &HolyShock);           // Holy Shock rank 4
	mgr->register_dummy_spell(33072, &HolyShock);           // Holy Shock rank 5
*/
// REGISTER NEW DUMMY SPELLS ABOVE THIS LINE
// *****************************************************************************
}
