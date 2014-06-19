#include "StdAfx.h"
#include "Setup.h"

/*
INSERT IGNORE INTO `creature_names` (`entry`, `name`, `subname`, `info_str`, `Flags1`, `type`, `family`, `rank`, `unk4`, `spelldataid`, `male_displayid`, `female_displayid`, `male_displayid2`, `female_displayid2`, `unknown_float1`, `unknown_float2`, `civilian`, `leader`) VALUES
(20, 'Realm Tips', 'click me', '', 0, 7, 0, 0, 0, 0, 12679, 0, 0, 0, 2, 1, NULL, 0);

INSERT IGNORE INTO `creature_proto` (`entry`, `minlevel`, `maxlevel`, `faction`, `minhealth`, `maxhealth`, `mana`, `scale`, `npcflags`, `attacktime`, `attacktype`, `mindamage`, `maxdamage`, `rangedattacktime`, `rangedmindamage`, `rangedmaxdamage`, `respawntime`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `combat_reach`, `bounding_radius`, `auras`, `boss`, `money`, `invisibility_type`, `death_state`, `walk_speed`, `run_speed`, `fly_speed`, `extra_a9_flags`, `spell1`, `spell2`, `spell3`, `spell4`, `spell_flags`, `modImmunities`) VALUES
(20, 96, 96, 35, 100000, 100000, 0, 0.5, 1, 1000, 0, 100, 200, 0, 0, 0, 360000000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 1.8, 0.5, '0', 0, 0, 0, 0, 2.5, 8, 14, 0, 0, 0, 0, 0, 0, 0);
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ANNOUNCE_INTERVAL					(2*60*1000)      //X milliseconds
//#define ANNOUNCE_INTERVAL					(10*1000)      //X milliseconds
#define INPUT_FILE_NAME						"announcer_input.txt"

#define ANNOUNCE_RANGE						10000   //players in range of the anouncer will hear announcement
#define MSG_LANG							0 
#define UNIT_ENTRY_FOR_SPAWN_AS_ANNOUNCER   20

#define MSG_TYPE							CHAT_MSG_ANNOUNCE //when not broadcast only !

/*
   CHAT_MSG_ADDON                           = -1,
   CHAT_MSG_SYSTEM                                 = 0,    //28,   CHAT_MSG_SYSTEM                 = 0x00,         0
   CHAT_MSG_SAY                           = 1,
   CHAT_MSG_PARTY                           = 2,
   CHAT_MSG_RAID                           = 3,
   CHAT_MSG_GUILD                           = 4,
   CHAT_MSG_OFFICER                        = 5,
   CHAT_MSG_YELL                           = 6,
   CHAT_MSG_WHISPER                        = 7,
   CHAT_MSG_WHISPER_MOB                     = 8,//CHAT_MSG_WHISPER_INFORM
   CHAT_MSG_WHISPER_INFORM                     = 9,//CHAT_MSG_REPLY
   CHAT_MSG_EMOTE                           = 10,
   CHAT_MSG_TEXT_EMOTE                        = 11,
   CHAT_MSG_MONSTER_SAY                     = 12,
   CHAT_MSG_MONSTER_PARTY                     = 13,
   CHAT_MSG_MONSTER_YELL                     = 14,
   CHAT_MSG_MONSTER_WHISPER                  = 15,
   CHAT_MSG_MONSTER_EMOTE                     = 16,
   CHAT_MSG_CHANNEL                        = 17,
   CHAT_MSG_CHANNEL_JOIN                     = 18,
   CHAT_MSG_CHANNEL_LEAVE                     = 19,
   CHAT_MSG_CHANNEL_LIST                     = 20,
   CHAT_MSG_CHANNEL_NOTICE                     = 21,
   CHAT_MSG_CHANNEL_NOTICE_USER               = 22,
   CHAT_MSG_AFK                           = 23,
   CHAT_MSG_DND                           = 24,
   CHAT_MSG_IGNORED                        = 25,
   CHAT_MSG_SKILL                           = 26,
   CHAT_MSG_LOOT                           = 27,
	CHAT_MSG_MONEY                                  = 28,
	CHAT_MSG_OPENING                                = 29,
	CHAT_MSG_TRADESKILLS                            = 30,
	CHAT_MSG_PET_INFO                               = 31,
	CHAT_MSG_COMBAT_MISC_INFO                       = 32,
	CHAT_MSG_COMBAT_XP_GAIN                         = 33,
	CHAT_MSG_COMBAT_HONOR_GAIN                      = 34,
	CHAT_MSG_COMBAT_FACTION_CHANGE                  = 35,
   CHAT_MSG_BG_EVENT_NEUTRAL                  = 36,
   CHAT_MSG_BG_EVENT_ALLIANCE                  = 37,
   CHAT_MSG_BG_EVENT_HORDE                     = 38,
   CHAT_MSG_RAID_LEADER                     = 39,
   CHAT_MSG_RAID_WARNING                     = 40,
   CHAT_MSG_RAID_WARNING_WIDESCREEN            = 41,
	CHAT_MSG_RAID_BOSS_EMOTE                        = 42,
   CHAT_MSG_FILTERED                        = 43,
   CHAT_MSG_BATTLEGROUND                     = 44,
   CHAT_MSG_BATTLEGROUND_LEADER               = 45,
   CHAT_MSG_RESTRICTED                        = 46,   
*/

#define MAX_MSG_LEN 1000
#define SAY_TYPE_BROADCAST		0 //you probabaly want this ;)
#define SAY_TYPE_MONSTER		1
#define SAY_TYPE_CHAT			2
#define SAY_TYPE_COMMERCIAL		3
#define SAY_TYPE_PVP_LADDER		4
#define SAY_TYPE_TEAM2V2		5
#define SAY_TYPE_TEAM3V3		6
#define SAY_TYPE_TEAM5V5		7
#define SAY_TYPE_INGAME_TIPP	8

bool IsInrangeSQ( Object *t1, Object *t2,float sq_r)
{
   if( !t1 || !t2 )
	  return false;

   float distx=t1->GetPositionX()-t2->GetPositionX();
   float disty=t1->GetPositionY()-t2->GetPositionY();
   float dist = distx*distx + disty*disty; 
   return dist <= sq_r;
}

struct ToAnnounceStruct
{
	char msg[MAX_MSG_LEN];
	char say_type; //broadcast,monster say,personal
	char chat_type;
	char say_lang;
	int32 map_id;
	int32 char_class;
	int32 char_race;
};

ToAnnounceStruct	*msg_entrys;
uint32				msg_total;

class Announcer : public CreatureAIScript
{
public:

   //constructor
	Announcer(Creature* pCreature) : CreatureAIScript(pCreature) 
   {
	  RegisterAIUpdateEvent( ANNOUNCE_INTERVAL );
	  msg_now_index = 0;
	  msg_total = 0;
	  msg_entrys = NULL;
	  annouce_range = ANNOUNCE_RANGE;
//      _unit->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 );
   }

	void ReloadSettings()
	{
		//dump old
		msg_total = 0;
		if( msg_entrys )
		{
			free( msg_entrys );
			msg_entrys = NULL;
		}
		msg_entrys = (ToAnnounceStruct*)realloc( msg_entrys, sizeof(ToAnnounceStruct) * (msg_total+1));
		FILE *conffile=fopen(INPUT_FILE_NAME,"r");
		if( conffile )
		{
		  do
		  {
			 char readbuf[2000];
			 char *res;
			 res = fgets(readbuf,2000,conffile);
			 if(!res)
				break; //end of file
			 if(strstr(readbuf,"MSGSTRUCT_MSG="))
				 strcpy(msg_entrys[ msg_total ].msg,readbuf+strlen("MSGSTRUCT_MSG="));
			 else if(strstr(readbuf,"MSGSTRUCT_TYPE="))
				msg_entrys[ msg_total ].say_type = atoi(readbuf+strlen("MSGSTRUCT_TYPE="));
			 else if(strstr(readbuf,"MSGSTRUCT_LANG="))
				msg_entrys[ msg_total ].say_lang = atoi(readbuf+strlen("MSGSTRUCT_LANG="));
			 else if(strstr(readbuf,"MSGSTRUCT_MAPID="))
				msg_entrys[ msg_total ].map_id = atoi(readbuf+strlen("MSGSTRUCT_MAPID="));
			 else if(strstr(readbuf,"MSGSTRUCT_CHARCLASS="))
				msg_entrys[ msg_total ].char_class = atoi(readbuf+strlen("MSGSTRUCT_CHARCLASS="));
			 else if(strstr(readbuf,"MSGSTRUCT_CHARRACE="))
				msg_entrys[ msg_total ].char_race = atoi(readbuf+strlen("MSGSTRUCT_ENDSTRUCT="));
			 else if(strstr(readbuf,"MSGSTRUCT_ENDSTRUCT="))
			 {
				 msg_total++;
				 msg_entrys = (ToAnnounceStruct*)realloc( msg_entrys, sizeof(ToAnnounceStruct) * (msg_total+1));
				 msg_entrys[ msg_total ].msg[0]=0;
				 msg_entrys[ msg_total ].say_type=0;
				 msg_entrys[ msg_total ].map_id=-1;
				 msg_entrys[ msg_total ].char_class=-1;
				 msg_entrys[ msg_total ].char_race=-1;
			 }
			 else if(strstr(readbuf,"ANNOUNCE_RANGE="))
				annouce_range = atof(readbuf+strlen("ANNOUNCE_RANGE="));
		  }
		  while (1);
		  fclose(conffile);
		}
	}

   //this will be called with ANNOUNCE_INTERVAL interval
   void AIUpdate()
   {
		ReloadSettings();
	   if( !msg_total )
		   return;
	   //let's reload the settings
	  MapMgr *mgr = _unit->GetMapMgr();
	  if( mgr )
	  {
		 PlayerStorageMap::iterator itr;
		 WorldPacket *data;
		 msg_now_index = ( msg_now_index ) % msg_total; //if length changed since last reload then we should use correct setting anyway

		 //special case is the PVP ladder
		 if( msg_entrys[ msg_now_index ].say_type == SAY_TYPE_PVP_LADDER )
		 {
			 AnnounceRealmPVPBests();
			 msg_now_index = ( msg_now_index + 1 ) % msg_total;
			 return;
		 }

		 //special case is the PVP ladder
		 if( msg_entrys[ msg_now_index ].say_type == SAY_TYPE_TEAM2V2 || msg_entrys[ msg_now_index ].say_type == SAY_TYPE_TEAM3V3 || msg_entrys[ msg_now_index ].say_type == SAY_TYPE_TEAM5V5 )
		 {
			 AnnounceRealmPVPBestArena( msg_entrys[ msg_now_index ].say_type );
			 msg_now_index = ( msg_now_index + 1 ) % msg_total;
			 return;
		 }

		 if( msg_entrys[ msg_now_index ].say_type == SAY_TYPE_INGAME_TIPP )
		 {
			 msg_now_index = ( msg_now_index + 1 ) % msg_total;
			 return;
		 }

		 for( itr = mgr->m_PlayerStorage.begin(); itr!=mgr->m_PlayerStorage.end();itr++)
			if( itr->second //can't imagine this getting to null though :P
			   && itr->second->isAlive() 
			   && itr->second->IsInWorld()
			   && IsInrangeSQ( _unit, itr->second, annouce_range*annouce_range)
			   && ( msg_entrys[ msg_now_index ].char_class == -1 || msg_entrys[ msg_now_index ].char_class == itr->second->getClass() )
			   && ( msg_entrys[ msg_now_index ].char_race == -1 || msg_entrys[ msg_now_index ].char_race == itr->second->getRace() )
			   && ( msg_entrys[ msg_now_index ].map_id == -1 || msg_entrys[ msg_now_index ].map_id == itr->second->GetMapId() )
			   )
			{
			   //no sender just the message
				if( msg_entrys[ msg_now_index ].say_type == SAY_TYPE_BROADCAST )
					itr->second->BroadcastMessage( msg_entrys[ msg_now_index ].msg );
			    //normal monster yell but only if player is in visual range
				else if( msg_entrys[ msg_now_index ].say_type == SAY_TYPE_MONSTER )
	               _unit->SendChatMessageToPlayer( msg_entrys[ msg_now_index ].chat_type , msg_entrys[ msg_now_index ].say_lang, msg_entrys[ msg_now_index ].msg ,itr->second);
			   //to make text appear on middle of monitor
				else if( msg_entrys[ msg_now_index ].say_type == SAY_TYPE_COMMERCIAL )		
	               itr->second->SendAreaTriggerMessage( msg_entrys[ msg_now_index ].msg );
			   //can have multiple message types like self wisper :D
				else if( msg_entrys[ msg_now_index ].say_type == SAY_TYPE_CHAT )						
				{
	               data = sChatHandler.FillMessageData(msg_entrys[ msg_now_index ].chat_type , msg_entrys[ msg_now_index ].say_lang, msg_entrys[ msg_now_index ].msg, itr->second->GetGUID(), itr->second->bGMTagOn ? 4 : 0  );
	               itr->second->GetSession()->SendPacket(data);
				}
			}
		msg_now_index = ( msg_now_index + 1 ) % msg_total;
	  }
   }

   void AnnounceRealmPVPBests()
   {
		uint32 announce_first_N_players = 8;
		if( announce_first_N_players > 10 )
		   announce_first_N_players = 10;
		static char class_names[12][20]=
		{
		   "",
		   "Warrior",
		   "Paladin",
		   "Hunter",
		   "Rogue",
		   "Priest",
		   "DeathKnight",
		   "Shaman",
		   "Mage",
		   "Warlock",
		   "",
		   "Druid",
		};
		char msg_buff[20][200];
		uint32 fetched_rows=0;
//		QueryResult * result = CharacterDatabase.Query( "Select name,class,honorPoints/100*level*killsLifeTime/100/100 from characters order by honorPoints*level*killsLifeTime desc limit 0,%u ", announce_first_N_players );
		QueryResult * result = CharacterDatabase.Query( "Select name,class,(SUBSTR( currency_list,instr(currency_list,',392,')+5,LOCATE(',',currency_list,instr(currency_list,',392,')+4)-instr(currency_list,',392,'))+1)*level*killsLifeTime/100/100 as score from characters order by score desc limit 0,%u ", announce_first_N_players );
		if( result )
		{
			do 
			{
				Field *fields = result->Fetch();
				char *name = (char*)fields[0].GetString();
				uint8 pclass = fields[1].GetInt8();
				uint32 score = fields[2].GetInt32();
				if( pclass > 11 )
					continue;
				fetched_rows++;
				sprintf(msg_buff[fetched_rows],"%-6u %-15s %-15s %u",fetched_rows,name,class_names[pclass],score);
			} while(result->NextRow());
			delete result;
			result = NULL;
		}

		//wow, no results at all ?
		if( fetched_rows <= 1 )
			return;

		//the default message 
		sprintf(msg_buff[0],"%-5s %-12s %-12s %s","Rank","Name","Class","Score");

		//we already checked and unit has a map
		MapMgr *mgr = _unit->GetMapMgr();
		PlayerStorageMap::iterator itr;
		for( itr = mgr->m_PlayerStorage.begin(); itr!=mgr->m_PlayerStorage.end();itr++)
		if( itr->second //can't imagine this getting to null though :P
		   && itr->second->isAlive() 
		   && itr->second->IsInWorld()
		   && IsInrangeSQ( _unit, itr->second, annouce_range*annouce_range)
		   )
		{
			for(int i=0;i<fetched_rows;i++)
				itr->second->BroadcastMessage( msg_buff[i] );
		}
   }

   void AnnounceRealmPVPBestArena( uint32 Type )
   {
		char msg_buff[20][200];

#define SELECT_BEST_N_TEAMS 3

		uint32 MyTeamType;
		msg_buff[0][0] = 0;
		if( Type == SAY_TYPE_TEAM2V2 )
		{
			MyTeamType = ARENA_TEAM_TYPE_2V2;
		}
		else if( Type == SAY_TYPE_TEAM3V3 )
		{
			MyTeamType = ARENA_TEAM_TYPE_3V3;
		}
		else if( Type == SAY_TYPE_TEAM5V5 )
		{
			MyTeamType = ARENA_TEAM_TYPE_5V5;
		}

		{
			ArenaTeam *BestTeams[SELECT_BEST_N_TEAMS];
			memset( BestTeams, 0, sizeof( BestTeams ) );
			
			for(HM_NAMESPACE::hash_map<uint32,ArenaTeam*>::iterator itr = objmgr.m_arenaTeamMap[MyTeamType].begin(); itr != objmgr.m_arenaTeamMap[MyTeamType].end(); ++itr)
			{
				if( itr->second->m_memberCount <= 1 )
					continue;

				uint32 MinIndex = 0, MinVal = 99999999;
				for( uint32 i=0;i<SELECT_BEST_N_TEAMS;i++)
					if( BestTeams[i] == NULL )
					{
						MinIndex = i;
						MinVal = 99999999;
						break;
					}
					else if( BestTeams[i]->m_stat_rating < MinVal )
					{
						MinVal = BestTeams[i]->m_stat_rating;
						MinIndex = i;
					}

				if( itr->second->m_stat_rating > MinVal || MinVal == 99999999 )
					BestTeams[ MinIndex ] = itr->second;
			}
			for( uint32 i=0;i<SELECT_BEST_N_TEAMS;i++)
			{
				msg_buff[i][0] = 0;
				if( BestTeams[i] != NULL )
				{
					sprintf( &msg_buff[i][0], "Team '%s'(%d) : ", BestTeams[i]->m_name.c_str(), BestTeams[i]->m_stat_rating );
					for( uint32 j=0;j < MIN( BestTeams[i]->m_memberCount, 10 ); j++ )
						if( BestTeams[i]->m_members[j].Info )
							sprintf( &msg_buff[i][0], "%s %s(%d) ", msg_buff[i], BestTeams[i]->m_members[j].Info->name, BestTeams[i]->m_members[j].PersonalRating );
				}
			}
		}

		//nothing to send in this bracket
		if( msg_buff[0][0] == 0 )
			return;

		//we already checked and unit has a map
		MapMgr *mgr = _unit->GetMapMgr();
		PlayerStorageMap::iterator itr;
		for( itr = mgr->m_PlayerStorage.begin(); itr!=mgr->m_PlayerStorage.end();itr++)
		if( itr->second //can't imagine this getting to null though :P
		   && itr->second->isAlive() 
		   && itr->second->IsInWorld()
		   && IsInrangeSQ( _unit, itr->second, annouce_range*annouce_range)
		   )
		{
			for( uint32 i=0;i<SELECT_BEST_N_TEAMS;i++)
				if( msg_buff[i][0] != 0 )
					itr->second->BroadcastMessage( msg_buff[i] );
		}
   }
   //create an anouncer object for this creature
	static CreatureAIScript *Create(Creature * c) { return new Announcer(c); }

   //destroy the anouncer object
	void Destroy() { delete this; }

private:
	uint32				msg_now_index;
	float				annouce_range;
};

class SCRIPT_DECL SerInfoTipps : public GossipScript
{
public:
	SerInfoTipps()
	{
	}
    void GossipHello( Object * pObject, Player* Plr, bool AutoSend )
	{
		if( msg_entrys == NULL || msg_total <= 0 )
			return;

		uint32 TippCount = 0;
		for( uint32 i=0;i<msg_total;i++)
			if( msg_entrys[ i ].say_type == SAY_TYPE_INGAME_TIPP )
				TippCount++;
		if( TippCount == 0 )
			return;

		int64 *Index = Plr->GetCreateIn64Extension( EXTENSION_ID_CUSTOM_BG_TIP_INDEX );
		*Index = *Index % TippCount;
		uint32 SearchFor = *Index;
		*Index = *Index + 1;
		for( uint32 i=0;i<msg_total;i++)
			if( msg_entrys[ i ].say_type == SAY_TYPE_INGAME_TIPP )
			{
				if( SearchFor == 0 )
				{
					Plr->BroadcastMessage( msg_entrys[ i ].msg );
					break;
				}
				SearchFor--;
			}
	}
	void Destroy()	{		delete this;	}
};

void SetupAnnouncer(ScriptMgr * mgr)
{
	msg_entrys = NULL;
	msg_total = 0;

   //this is to register the anouncer object to our creatures
	mgr->register_creature_script(UNIT_ENTRY_FOR_SPAWN_AS_ANNOUNCER, &Announcer::Create);

	GossipScript *gs = (GossipScript*) new SerInfoTipps();
	mgr->register_gossip_script( UNIT_ENTRY_FOR_SPAWN_AS_ANNOUNCER , gs ); 

}