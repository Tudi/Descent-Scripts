#ifndef TOURNAMENT_SCRIPTS_SETUP_H
#define TOURNAMENT_SCRIPTS_SETUP_H

//monitor the chat
void OnChatForTrivia(Player *pPlayer, uint32 type, uint32 lang, const char* message, const char * target);

//in case a trivia bot is spawned he will register itself into triviabot global list to be able to monitor chat
void SetupTriviaBot(ScriptMgr * mgr);

#endif