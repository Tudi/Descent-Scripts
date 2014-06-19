#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"

JumpEventHolder		JumpEventHolders[ MAX_NUMBER_OF_CONCURENT_JUMP_EVENT_HOLDERS ];
Mutex				CreateLock;

bool IsInrangeSQ( Object *t1, Object *t2,float sq_r)
{
   if( !t1 || !t2 )
	  return false;

   float distx=t1->GetPositionX()-t2->GetPositionX();
   float disty=t1->GetPositionY()-t2->GetPositionY();
   float dist = distx*distx + disty*disty; 
   return dist <= sq_r;
}

bool IsInrangeSQ( float x1, float y1,float x2, float y2, float sq_r)
{
   float distx=x1-x2;
   float disty=y1-y2;
   float dist = distx*distx + disty*disty; 
   return dist <= sq_r;
}

//when player pushes gossip menu to start event
void StartJumpEvent(Player *Plr, uint64 NPCGUID)
{
	CreateLock.Acquire();
	//check if this player already has an ongoing jump event
	uint64 PlayerGUID = Plr->GetGUID();
	uint32 stampnow = GetTickCount();
	for( uint32 i=0;i<MAX_NUMBER_OF_CONCURENT_JUMP_EVENT_HOLDERS;i++)
		if( JumpEventHolders[i].PlayerGUID == PlayerGUID 
			&& stampnow < JumpEventHolders[i].EventExpireStamp
			)
		{
			Plr->BroadcastMessage("You are already participating in a Jump Event. Close that one");
			CreateLock.Release();
			return;
		}
	//check if NPC already is busy with someone
	int32 NPC_slot = -1;
	for( uint32 i=0;i<MAX_NUMBER_OF_CONCURENT_JUMP_EVENT_HOLDERS;i++)
		if( JumpEventHolders[i].SupervizorNPC == NPCGUID )
		{
			if( stampnow < JumpEventHolders[i].EventExpireStamp	)
			{
				Plr->BroadcastMessage("This NPC seems to be busy with someone. Strange.");
				CreateLock.Release();
				return;
			}
			NPC_slot = i;
		}
	//sanity check
	if( NPC_slot == -1 )
	{
		Plr->BroadcastMessage("Omg Bug.Shoot a developer pls and wait a server crash to refresh list");
		CreateLock.Release();
		return;
	}
	//ocupy this slot
	JumpEventHolders[ NPC_slot ].PlayerGUID = PlayerGUID;
	JumpEventHolders[ NPC_slot ].EventExpireStamp = MAX_EVENT_DURATION + stampnow;
	JumpEventHolders[ NPC_slot ].EventState = JES_WAITING_TO_START;
	CreateLock.Release();
	//start timer player side also
	uint32 estimated_time = Plr->getLevel() * 10;
	WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
	data << uint32( TIMER_EXHAUSTION ) << estimated_time << estimated_time << int32(-1) << uint32(0);
	Plr->GetSession()->SendPacket(&data);
	//rest will be done by the supervizor AI : generate pads, spawn them 1 by 1, monitor progress
}

//early end a jump event due to player request
void EarlyEndJumpEvent(Player *Plr, uint64 NPCGUID)
{
	CreateLock.Acquire();
	//check if this player already has an ongoing jump event
	uint64 PlayerGUID = Plr->GetGUID();
	for( uint32 i=0;i<MAX_NUMBER_OF_CONCURENT_JUMP_EVENT_HOLDERS;i++)
		if( JumpEventHolders[i].PlayerGUID == PlayerGUID 
			&& JumpEventHolders[i].SupervizorNPC == NPCGUID
			)
		{
			Plr->BroadcastMessage("Removing now");
			//stop timer player side also
			Plr->StopMirrorTimer( TIMER_EXHAUSTION );
			//clear identifyers
			JumpEventHolders[i].EventExpireStamp = 0;
			JumpEventHolders[i].PlayerGUID = 0;
			JumpEventHolders[i].EventState = JES_NOTHING_INTELIGENT;
		}
	CreateLock.Release();
}

void Event_Reward_Winner_Player(Player *Plr,uint32 intime)
{
	if( Plr == NULL )
		return;
	Plr->BroadcastMessage("Yaaay, you finished the event in %u seconds",intime);
}