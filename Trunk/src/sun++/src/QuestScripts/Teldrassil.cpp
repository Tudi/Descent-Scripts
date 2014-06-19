#include "StdAfx.h"
#include "Setup.h"
/*
class Zenn_Foulhoof : public QuestScript 
{ 
public:
  void DemorpNPC(Creature *creat)
  {
	creat->SetUInt32Value(UNIT_FIELD_DISPLAYID,10035);
  }

  void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
  {
    if(!mTarget)
		return;

    Creature *creat = mTarget->GetMapMgr()->GetSqlIdCreature(43727);
    if(creat == NULL)
      return;

    creat->SetUInt32Value(UNIT_FIELD_DISPLAYID,901);
	creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Ribbit! No! This cannot...ribbit...be! You have duped me with...ribbit..your foul trickery! Ribbit!");

	TimedEvent *te = TimedEvent::Allocate(this, new CallbackP1<Zenn_Foulhoof, Creature*>(this, &Zenn_Foulhoof::DemorpNPC, creat), 0, 50*1000, 1);  
	creat->event_AddEvent(te);
  }
}; */

void SetupTeldrassil(ScriptMgr *mgr)
{	
//  QuestScript *Zenn_FoulhoofQuest = (QuestScript*) new Zenn_Foulhoof();
//  mgr->register_quest_script(489, Zenn_FoulhoofQuest);
}