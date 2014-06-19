#include "StdAfx.h"
#include "Setup.h"
#define SKIP_ALLOCATOR_SHARING 1
#include <ScriptSetup.h>

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_MISC;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)	// Comment any script to disable it
{
	SetupBurth(mgr);
	SetupGevasGrimegate(mgr);
	SetupMaximaBlastenheimer(mgr);
	SetupMorjaTalk(mgr);
	SetupProfessorThaddeusPaleo(mgr);
	SetupProfessorThaddeusPaleoTalk(mgr);
	SetupSayge(mgr);
	SetupSaygeTalk(mgr);
	SetupSelinaDourman(mgr);
	SetupSilasDarkmoon(mgr);
	SetupSilasDarkmoonSpeak(mgr);
	SetupFlik(mgr);
	SetupFlikFrog(mgr);
	SetupLhara(mgr);
	SetupStampThunderhorn(mgr);
	SetupSylannia(mgr);
	SetupActiveCarnies(mgr);
	SetupActiveCarniesGossip(mgr);
	SetupMusicDoodad(mgr);
	SetupBlastenheimer5000(mgr);
}

#ifdef WIN32

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    return TRUE;
}

#endif