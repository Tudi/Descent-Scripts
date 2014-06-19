#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"

uint32				global_update_next_stamp = 0;		//do not spam updates in case there are multiple supervizor spawns
Mutex				UpdateMutex;
uint32				debug_commands = SUPERVIZOR_COMMAND_NONE;

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
