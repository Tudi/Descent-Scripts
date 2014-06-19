#include "StdAfx.h"
#include "Setup.h"

// !! there can be only one trivia game active / server but it will be visible to only some players where trivia NPC is spawned
#define INPUT_FILE_NAME "triviabot_questions.txt"
/*
specific format for us
row 1 : question
row 2 : list of hints separated by #
row 3 : list of answers separated by #
row 4 : rewards as count ( this is will be copper at this point)
row 5 : empty
*/

#define TRIVIA_MOB_ENTRY			24208			//npc entry. Bind to whatever. Only location is used from this spawn
#define TRIVIA_BOT_VIEW_RANGE		15				//the range within where trivia bot will monitor chat		
#define TRIVIA_BOT_QUESTION_TIMEOUT	10000			//interval in which mob will give next hint or answer
#define TRIVIA_BOT_NAME				"TriviaBot:"	//this will appear in chat
#define TRIVIA_BOT_REWARD_SCALEING	20				//5 points * 20 = 100 = 1 silver

#define TRIVIA_DEFAULT_STRING_LENGTH 500

//trivia bots do not have any fizical instances. Chat is monitored and in case a matching answer is received you are rewarded
struct trivia_question
{
	std::string					question;
	std::list<char *>			hints;
	std::list<char *>			answers;
	std::list<char *>::iterator	hints_remaining;
	uint32						reward_count;
};

struct trivia_bot_location
{
	float x,y;
};

static bool trivia_loaded = false;
static std::list<trivia_bot_location *>			triviabot_locations;
static std::list<trivia_question *>				triviabot_questions;
static std::list<trivia_question *>::iterator	triviabot_cur_question;
static uint32 triviabot_stampfornextupdate = 0;

//converts string to lower case
void TriviaToLowerStr(char *what)
{
	uint32 ind=0;
	while( what[ind] != 0 )
	{
		if( what[ind] >= 'A' && what[ind]<'Z' )
			what[ind] = what[ind] + 'a' - 'A';
		ind++;
	}
}

//match str1 to str2. exit as soon as possible
bool TriviaStrCmp(char *str1, char *str2)
{
	uint32 ind=0;
	while( str1[ind]==str2[ind] && str1[ind]!=0 && str2[ind]!=0 )
		ind++;
	if( str1[ind] == str2[ind] && str1[ind] == 0 )
		return true;
	return false;
}

//match str1 to str2. exit as soon as possible
void TriviaStrCpy(char *dst, const char *src,uint32 maxlen)
{
	uint32 ind=0;
	while( src[ind]!=0 && ind<maxlen )
		dst[ind]=src[ind++];
	dst[ind] = 0; 
}
void triviabot_load_questions()
{
//		specific format for us
//		row 1 : question
//		row 2 : list of hints separated by #
//		row 3 : list of answers separated by #
//		row 4 : rewards as count ( this is will be copper at this point) 
//		row 5 : empty
		FILE *conffile=fopen(INPUT_FILE_NAME,"r");
		if( conffile )
		{
			do
			{
				char readbuf[20000];
				char *res;

				//quesiton
				res = fgets(readbuf,20000,conffile);
				if(!res)  break; //end of file
				trivia_question *q = new trivia_question;
				q->question = readbuf;

				//read hints
				res = fgets(readbuf,20000,conffile);
				if(!res)  break; //end of file
				//break hint into pieces
				char tempstr[2000];
				uint32 ind=0,ind2=0;
				while( readbuf[ind] != 0 )
				{
					if( (readbuf[ind] == '#' || readbuf[ind+1] == 0) && ind2 != 0 )
					{
						tempstr[ind2++]=0;
						char *new_str = (char *)malloc( ind2 + 1 );
						strcpy( new_str, tempstr );
						q->hints.push_back( new_str );
						ind2 = 0;
					}
					if( readbuf[ind] != '#' )
						tempstr[ind2++]=readbuf[ind++];
					else
					{
						ind2=0;
						ind++;
					}
				}
				q->hints_remaining = q->hints.begin();

				//read answers
				res = fgets(readbuf,20000,conffile);
				if(!res)  break; //end of file
				//break answers into pieces
				ind=0,ind2=0;
				while( readbuf[ind] != 0 )
				{
					if( (readbuf[ind] == '#' || readbuf[ind+1] == 0) && ind2 != 0 )
					{
						tempstr[ind2++]=0;
						TriviaToLowerStr( tempstr );
						char *new_str = (char*)malloc( ind2 + 1 );
						strcpy( new_str, tempstr );
						q->answers.push_back( new_str );
						ind2 = 0;
					}
					if( readbuf[ind] != '#' )
						tempstr[ind2++]=readbuf[ind++];
					else
					{
						ind2=0;
						ind++;
					}
				}

				//read rewards
				res = fgets(readbuf,20000,conffile);
				if(!res)  break; //end of file
				q->reward_count = atoi( readbuf );

				//empty line
				res = fgets(readbuf,20000,conffile);

				//register this quizz
				triviabot_questions.push_back( q );
			}
			while (1);
			fclose(conffile);
		}
}

bool IsInrangeToBots( Object *t1 )
{
   if( !t1 )
     return false;

	bool already_registered = false;
	std::list<trivia_bot_location *>::iterator itr;
	for( itr=triviabot_locations.begin(); itr != triviabot_locations.end(); itr++)
	{
		float distx=t1->GetPositionX()-(*itr)->x;
		float disty=t1->GetPositionY()-(*itr)->y;
		float dist = distx*distx + disty*disty; 
		if( dist < TRIVIA_BOT_VIEW_RANGE * TRIVIA_BOT_VIEW_RANGE )
			return true;
	}
	return false;
}

void TriviaBotSendMsg( char *msg1, char *msg2 )
{
	//send this message to players
/*	PlayerStorageMap::iterator itr;
	for( itr = mgr->m_PlayerStorage.begin(); itr!=mgr->m_PlayerStorage.end();itr++)
		if( itr->second //can't imagine this getting to null though :P
			&& itr->second->isAlive() 
			&& itr->second->IsInWorld()
			&& IsInrangeToBots( itr->second )
			)
		{
			itr->second->BroadcastMessage( next_string );
			if( next_question )
				itr->second->BroadcastMessage( next_question );
		}
		*/
	objmgr._playerslock.AcquireReadLock();
	PlayerStorageMap::const_iterator itr;
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		if( itr->second //can't imagine this getting to null though :P
			&& itr->second->isAlive() 
			&& itr->second->IsInWorld()
			&& IsInrangeToBots( itr->second )
			)
		{
			itr->second->BroadcastMessage( msg1 );
			if( msg2 )
				itr->second->BroadcastMessage( msg2 );
		}
	}
	objmgr._playerslock.ReleaseReadLock();
}

void TriviaBotNextQuestion()
{
	uint32 inc_count = RandomUInt() % 4 + 1;
	for(uint32 i=0;i<inc_count;i++)
	{
		triviabot_cur_question++;
		//in case we finished putting all our questions
		if( triviabot_cur_question == triviabot_questions.end() )
			triviabot_cur_question = triviabot_questions.begin();
	}
	(*triviabot_cur_question)->hints_remaining = (*triviabot_cur_question)->hints.begin();
}

void TriviaBotSendQuestion(uint32 new_question )
{
	//no function call spams
	if( GetTickCount() < triviabot_stampfornextupdate && new_question == 0 )
		return;

	//avoid call spam please
	triviabot_stampfornextupdate = GetTickCount() + TRIVIA_BOT_QUESTION_TIMEOUT;

	sLog.outDebug("Triviabot: trying to put next question. List size is %u\n",triviabot_questions.size());
	//omg not questions: QQ
	if( triviabot_questions.size() == 0 )
	   return;

	//in case we finished putting all our questions
	if( triviabot_cur_question == triviabot_questions.end() )
		triviabot_cur_question = triviabot_questions.begin();

	char next_string[2000];
	char next_string2[2000];
	char *next_question = NULL;

	sLog.outDebug("Triviabot: hints size %u new question %u \n",(*triviabot_cur_question)->hints.size(),new_question != 0);
	//if we used up all our hints then give answer
	if( ((*triviabot_cur_question)->hints_remaining == (*triviabot_cur_question)->hints.end() 
			|| (*triviabot_cur_question)->hints.size() == 0 ) 
		&& new_question == 0
		)
	{
		sprintf(next_string,"%s The answer was : %s",TRIVIA_BOT_NAME,(*((*triviabot_cur_question)->answers.begin())));
		(*triviabot_cur_question)->hints_remaining == (*triviabot_cur_question)->hints.begin();
		TriviaBotNextQuestion();
		sprintf(next_string2,"%s Question : %s",TRIVIA_BOT_NAME,(char*)(*triviabot_cur_question)->question.c_str());
		next_question = next_string2;
	}
	else if( new_question == 0 )
	{
		sprintf(next_string,"%s Hint : %s",TRIVIA_BOT_NAME,(*((*triviabot_cur_question)->hints_remaining)));
		(*triviabot_cur_question)->hints_remaining++;
	}
	else
	{
		sprintf(next_string,"%s Question : %s",TRIVIA_BOT_NAME,(char*)(*triviabot_cur_question)->question.c_str());
	}

	//send messages to our players
	TriviaBotSendMsg( next_string, next_question );
}

void OnChatForTrivia(Player *pPlayer, uint32 type, uint32 lang, const char* message, const char * target)
{

   if ( pPlayer == NULL || message == NULL )
      return;   // wtf error

   //omg not questions: QQ
   if( triviabot_questions.size() == 0 )
	   return;

   //check if we can consider this player still playing for trivia
	if( !IsInrangeToBots( pPlayer ) )
		return;

	//in case we finished putting all our questions
	if( triviabot_cur_question == triviabot_questions.end() )
		triviabot_cur_question = triviabot_questions.begin();

	//tolower the string
	char local_string[2000];
	TriviaStrCpy(local_string, message, 2000 );
	TriviaToLowerStr( local_string );

    sLog.outDebug("Triviabot: checking answer %s\n",local_string);
	//check for answer match
	std::list<char *>::iterator itr;
	for( itr=(*triviabot_cur_question)->answers.begin(); itr != (*triviabot_cur_question)->answers.end();itr++)
	{
		sLog.outDebug("Triviabot: checking answer %s - %s\n",local_string,(*itr ));
		if( TriviaStrCmp( local_string, (*itr ) ) )
		{
			char local_string2[2000];
			sprintf(local_string2,"%s `%s` gave corect answer : %s",TRIVIA_BOT_NAME,pPlayer->GetName(),(*itr ));
			TriviaBotSendMsg(local_string2,NULL);
			WorldPacket pkt;
			pkt.SetOpcode(SMSG_LOOT_MONEY_NOTIFY);
			pkt << (*triviabot_cur_question)->reward_count * TRIVIA_BOT_REWARD_SCALEING;
			pPlayer->ModGold( (*triviabot_cur_question)->reward_count * TRIVIA_BOT_REWARD_SCALEING );
			pPlayer->GetSession()->SendPacket(&pkt);
			TriviaBotNextQuestion();
			TriviaBotSendQuestion( 1 );
			break;
		}
	}
}

class TriviaBotNPC : public CreatureAIScript
{
public:
   ADD_CREATURE_FACTORY_FUNCTION(TriviaBotNPC);

   //constructor
    TriviaBotNPC(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		float x = _unit->GetPositionX();
		float y = _unit->GetPositionY();
		//register our location as a monitored one
		bool already_registered = false;
		std::list<trivia_bot_location *>::iterator itr;
		for( itr=triviabot_locations.begin(); itr != triviabot_locations.end(); itr++)
			if( (*itr)->x == x && (*itr)->y == y )
			{
				already_registered = true;
				break;
			}
		if( already_registered == false )
		{
			trivia_bot_location *t = new trivia_bot_location;
			t->x = x;
			t->y = y;
			triviabot_locations.push_back( t );
		}

		//see if triviabot is loaded. If not then load it
		if( trivia_loaded == false )
		{
		   triviabot_load_questions();
		   trivia_loaded = true;
		   triviabot_cur_question = triviabot_questions.begin();
		   TriviaBotNextQuestion();
		   sLog.outDebug("Triviabot: Initialized triviabot with question size %u\n",triviabot_questions.size());
		   TriviaBotSendQuestion( 1 );
		}

		RegisterAIUpdateEvent( TRIVIA_BOT_QUESTION_TIMEOUT );
    }
    void AIUpdate()
    {
//		sLog.outDebug("TriviaBot : Mob is getting undated");
		//watch it, as more spawns are made this will be called more frecvently. Target function will filter out call spam
		TriviaBotSendQuestion( 0 );
	}
};

void SetupTriviaBot(ScriptMgr * mgr)
{
	//every time a mob is spawned he registers itself into global 
    mgr->register_creature_script(TRIVIA_MOB_ENTRY, &TriviaBotNPC::Create);

}