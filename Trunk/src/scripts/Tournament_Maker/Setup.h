#ifndef TOURNAMENT_SCRIPTS_SETUP_H
#define TOURNAMENT_SCRIPTS_SETUP_H

//should let GMs supervise the battles
void SetupTournamentSupervisor(ScriptMgr * mgr);
void SetupTournamentSupervisorGossip(ScriptMgr * mgr);

//players can port to the the actual tournament but make no actions
void SetupTournamentSpectatorSupervisor(ScriptMgr * mgr);
void SetupTournamentSpectatorSupervisorGossip(ScriptMgr * mgr);

void OnChat(Player *pPlayer, uint32 type, uint32 lang, const char* message, const char * target);

#endif