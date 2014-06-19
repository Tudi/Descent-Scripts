#include "StdAfx.h"
#include "Setup.h"
#include "globals.h"
#include "Tournament_supervisor.h"

bool safe_strcmp( const char *s1, const char *s2, uint32 maxlen )
{
	uint32 ind=0;
	while( ind < maxlen && s1[ ind ] == s2[ ind ] && s1[ ind ] != 0 && s2[ ind ] != 0 )
		ind++;
	if( s1[ ind ] == s2[ ind ] || s1[ ind ] == 0 || s2[ ind ] == 0 )
		return true;
	return false;
} 

void Event_Supervisor_AI::ReloadMaps()
{
	DespawnEventMap();
	Event_map= NULL;
	CacheMaps();
	Event.status = TS_NOT_STARTED;
}

void Event_Supervisor_AI::CacheMaps()
{
	uint32 mi = 0;
	uint32 *map_buffer = (uint32 *)malloc( sizeof( uint32 ) * MAX_STRING_LENGTH * MAX_STRING_LENGTH );
	memset( maps, 0, sizeof( maps ) );
	for(uint32 i=0;i<MAX_MAPS_CACHED;i++)
	{
		char str_buffer[MAX_STRING_LENGTH];
		char *sres;
		uint32 started_map_def = 0;
		uint32 parse_map_line = 0;
		sprintf( str_buffer,"map_%u.maze",i);
		FILE *f = fopen(str_buffer,"r");
		if( f == NULL )
			continue;
		memset( map_buffer, 0, sizeof( uint32 ) * MAX_STRING_LENGTH * MAX_STRING_LENGTH );
		sres=fgets(str_buffer,MAX_STRING_LENGTH,f);
		while( sres )
		{
			if( sres[0]=='#' )
				goto read_next_line;
			if( started_map_def == 0 )
			{
				if( sres[0] == 'N' && sres[1] == '=' )
					strcpy( maps[mi].map_info_text, &sres[2] );
				else if( sres[0] == 'n' && sres[1] == '=' )
					strcpy( maps[mi].map_name_menu, &sres[2] );
				else if( sres[0] == 'H' && sres[1] == '=' )
					maps[mi].H_GO_entry = atoi( &sres[2] );
				else if( sres[0] == 'V' && sres[1] == '=' )
					maps[mi].V_GO_entry = atoi( &sres[2] );
				else if( sres[0] == 'S' && sres[1] == '=' )
					maps[mi].GO_size = atoi( &sres[2] );
				else if( sres[0] == 'Q' && sres[1] == '=' )
					maps[mi].MOB_spawn_entries[ 0 ] = atoi( &sres[2] );
				else if( sres[0] == 'W' && sres[1] == '=' )
					maps[mi].MOB_spawn_entries[ 1 ] = atoi( &sres[2] );
				else if( sres[0] == 'E' && sres[1] == '=' )
					maps[mi].MOB_spawn_entries[ 2 ] = atoi( &sres[2] );
				else if( sres[0] == 'R' && sres[1] == '=' )
					maps[mi].MOB_spawn_entries[ 3 ] = atoi( &sres[2] );
				else if( sres[0] == 'T' && sres[1] == '=' )
					maps[mi].MOB_spawn_entries[ 4 ] = atoi( &sres[2] );
				else if( sres[0] == 'Y' && sres[1] == '=' )
					maps[mi].MOB_spawn_entries[ 5 ] = atoi( &sres[2] );
				else if( sres[0] == 'U' && sres[1] == '=' )
					maps[mi].MOB_spawn_entries[ 6 ] = atoi( &sres[2] );
				else if( sres[0] == 'I' && sres[1] == '=' )
					maps[mi].MOB_spawn_entries[ 7 ] = atoi( &sres[2] );
				else if( sres[0] == 'O' && sres[1] == '=' )
					maps[mi].MOB_spawn_entries[ 8 ] = atoi( &sres[2] );
				else if( sres[0] == 'P' && sres[1] == '=' )
					maps[mi].MOB_spawn_entries[ 9 ] = atoi( &sres[2] );
				else if( sres[0] == 'J' && sres[1] == '=' )
					maps[mi].start_buffs[ 0 ] = atoi( &sres[2] );
				else if( sres[0] == 'K' && sres[1] == '=' )
					maps[mi].start_buffs[ 1 ] = atoi( &sres[2] );
				else if( sres[0] == 'L' && sres[1] == '=' )
					maps[mi].start_buffs[ 2 ] = atoi( &sres[2] );
				else if( sres[0] == 'i' && sres[1] == 'e' && sres[2] == '1' && sres[3] == '=' )
					maps[mi].reward_item_id[0] = atoi( &sres[4] );
				else if( sres[0] == 'i' && sres[1] == 'c' && sres[2] == '1' && sres[3] == '=' )
					maps[mi].reward_item_id_count[0] = atoi( &sres[4] );
				else if( sres[0] == 'i' && sres[1] == 'e' && sres[2] == '2' && sres[3] == '=' )
					maps[mi].reward_item_id[1] = atoi( &sres[4] );
				else if( sres[0] == 'i' && sres[1] == 'c' && sres[2] == '2' && sres[3] == '=' )
					maps[mi].reward_item_id_count[1] = atoi( &sres[4] );
				else if( sres[0] == 'i' && sres[1] == 'e' && sres[2] == '3' && sres[3] == '=' )
					maps[mi].reward_item_id[2] = atoi( &sres[4] );
				else if( sres[0] == 'i' && sres[1] == 'c' && sres[2] == '3' && sres[3] == '=' )
					maps[mi].reward_item_id_count[2] = atoi( &sres[4] );
				else if( sres[0] == 'g' && sres[1] == '=' )
					maps[mi].reward_gold[0] = atoi( &sres[2] );
				else if( sres[0] == 's' && sres[1] == '1' && sres[2] == '=' )
					maps[mi].reward_buffs[0] = atoi( &sres[3] );
				else if( sres[0] == 's' && sres[1] == '2' && sres[2] == '=' )
					maps[mi].reward_buffs[1] = atoi( &sres[3] );
				else if( sres[0] == 's' && sres[1] == '3' && sres[2] == '=' )
					maps[mi].reward_buffs[2] = atoi( &sres[3] );
				else if( sres[0] == 'h' && sres[1] == '=' )
					maps[mi].H_Gate_GO_entry = atoi( &sres[2] );
				else if( sres[0] == 'v' && sres[1] == '=' )
					maps[mi].V_Gate_GO_entry = atoi( &sres[2] );
				else if( sres[0] == 'j' && sres[1] == '=' )
					maps[mi].H_Start_Gate_GO_entry = atoi( &sres[2] );
				else if( sres[0] == 'b' && sres[1] == '=' )
					maps[mi].V_Start_Gate_GO_entry = atoi( &sres[2] );
				else if( sres[0] == 'Z' && sres[1] == '=' )
					maps[mi].GO_spawn_entries[0] = atoi( &sres[2] );
				else if( sres[0] == 'X' && sres[1] == '=' )
					maps[mi].GO_spawn_entries[0] = atoi( &sres[2] );
				else if( sres[0] == 'C' && sres[1] == '=' )
					maps[mi].GO_spawn_entries[0] = atoi( &sres[2] );
				else if( sres[0] == 'M' && sres[1] == '=' )
					maps[mi].GO_spawn_entries[0] = atoi( &sres[2] );
				else if( sres[0] == 'z' && sres[1] == '=' )
					maps[mi].z_adjust = atoi( &sres[2] );
				else if( sres[0] == 'f' && sres[1] == '=' )
					maps[mi].FFA_enabled = atoi( &sres[2] );
				else if( safe_strcmp( "START_MAZE", sres, MAX_STRING_LENGTH ) == true )
					started_map_def = 1;
				else if( safe_strcmp( "MP=", sres, MAX_STRING_LENGTH ) == true )
					maps[mi].max_players_in_team = atoi( &sres[3] );
			}
			else
			{
				if( safe_strcmp( "END_MAZE", sres, MAX_STRING_LENGTH ) == true )
					started_map_def = 0;
				//try to parse this map line
				uint32 line_len = strlen( sres );
				uint32 max_valid = 0;
				for(uint32 li=0;li<line_len;li++)
				{
					if( sres[li] == 'P')
					{
						uint32 team_start_y = parse_map_line;
						uint32 team_start_x = li;
						for(uint32 ts=0;ts<MAX_TEAMS_PER_EVENT;ts++)
							if( maps[mi].team_start_x[ts] == 0.0f )
							{
								maps[mi].team_start_x[ts] = team_start_x + 0.00001f;	//make sure 0,0 can be used as starting point, this will be reindexed with map center and NPC position
								maps[mi].team_start_y[ts] = team_start_y + 0.00001f;
								break;
							}
						max_valid = li;
					}
					else if( sres[li] == 'F')
					{
						maps[mi].m_goal_x = li;
						maps[mi].m_goal_y = parse_map_line;
						max_valid = li;
					}
					else if( sres[li] == 'Q' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = -maps[mi].MOB_spawn_entries[ 0 ];
						max_valid = li;
					}
					else if( sres[li] == 'W' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = -maps[mi].MOB_spawn_entries[ 1 ];
						max_valid = li;
					}
					else if( sres[li] == 'E' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = -maps[mi].MOB_spawn_entries[ 2 ];
						max_valid = li;
					}
					else if( sres[li] == 'R' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = -maps[mi].MOB_spawn_entries[ 3 ];
						max_valid = li;
					}
					else if( sres[li] == 'T' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = -maps[mi].MOB_spawn_entries[ 4 ];
						max_valid = li;
					}
					else if( sres[li] == 'Y' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = -maps[mi].MOB_spawn_entries[ 5 ];
						max_valid = li;
					}
					else if( sres[li] == 'U' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = -maps[mi].MOB_spawn_entries[ 6 ];
						max_valid = li;
					}
					else if( sres[li] == 'I' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = -maps[mi].MOB_spawn_entries[ 7 ];
						max_valid = li;
					}
					else if( sres[li] == 'O' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = -maps[mi].MOB_spawn_entries[ 8 ];
						max_valid = li;
					}
					else if( sres[li] == 'H' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = maps[mi].H_GO_entry;
						max_valid = li;
					}
					else if( sres[li] == 'V' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = maps[mi].V_GO_entry;					
						max_valid = li;
					}
					else if( sres[li] == 'h' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = maps[mi].H_Gate_GO_entry;					
						max_valid = li;
					}
					else if( sres[li] == 'v' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = maps[mi].V_Gate_GO_entry;					
						max_valid = li;
					}
					else if( sres[li] == 'j' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = maps[mi].H_Start_Gate_GO_entry;					
						max_valid = li;
					}
					else if( sres[li] == 'b' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = maps[mi].V_Start_Gate_GO_entry;					
						max_valid = li;
					}
					else if( sres[li] == 'Z' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = maps[mi].GO_spawn_entries[0];					
						max_valid = li;
					}
					else if( sres[li] == 'X' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = maps[mi].GO_spawn_entries[1];					
						max_valid = li;
					}
					else if( sres[li] == 'C' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = maps[mi].GO_spawn_entries[2];					
						max_valid = li;
					}
					else if( sres[li] == 'M' )
					{
						map_buffer[ parse_map_line * MAX_STRING_LENGTH + li ] = maps[mi].GO_spawn_entries[3];					
						max_valid = li;
					}
				}
				parse_map_line++;
				maps[mi].map_size_x = MAX(maps[mi].map_size_x,max_valid+1);
				maps[mi].map_size_y = MAX(maps[mi].map_size_y,parse_map_line);
				if( parse_map_line >= MAX_STRING_LENGTH )
					break;
			}
read_next_line:
			sres=fgets(str_buffer,MAX_STRING_LENGTH,f);
		}
		fclose( f );
		CheckMapCorrect( mi );
		if( maps[mi].map_size_x > 1 )
		{
			if( maps[mi].entry_map != NULL )
				free( maps[mi].entry_map );
			if( maps[mi].spawn_map != NULL )
				free( maps[mi].spawn_map );
			maps[mi].entry_map = (int32*)malloc( maps[mi].map_size_x * maps[mi].map_size_y * sizeof( uint32 ) );
			maps[mi].spawn_map = (uint64*)malloc( maps[mi].map_size_x * maps[mi].map_size_y * sizeof( uint64 ) );
			memset( maps[mi].spawn_map, 0, maps[mi].map_size_x * maps[mi].map_size_y * sizeof( uint64 ) );
			for( uint32 y=0;y<maps[mi].map_size_y;y++)
				for( uint32 x=0;x<maps[mi].map_size_x;x++)
					maps[mi].entry_map[x+y*maps[mi].map_size_x] = map_buffer[ y * MAX_STRING_LENGTH + x ];
			maps[mi].goal_x =  m_Unit->GetPositionX(); 
			maps[mi].goal_y =  m_Unit->GetPositionY(); 
			maps[mi].start_x = m_Unit->GetPositionX() - maps[mi].m_goal_x * maps[mi].GO_size;
			maps[mi].start_y = m_Unit->GetPositionY() - maps[mi].m_goal_y * maps[mi].GO_size;
			maps[mi].center_x = maps[mi].start_x + maps[mi].map_size_x / 2 * maps[mi].GO_size; 
			maps[mi].center_y = maps[mi].start_y + maps[mi].map_size_y / 2 * maps[mi].GO_size; 
			for(uint32 ts=0;ts<MAX_TEAMS_PER_EVENT;ts++)
				if( maps[mi].team_start_x[ts] != 0.0f )
				{
					maps[mi].team_start_x[ts] = maps[mi].team_start_x[ts] * maps[mi].GO_size + maps[mi].start_x;	
					maps[mi].team_start_y[ts] = maps[mi].team_start_y[ts] * maps[mi].GO_size + maps[mi].start_y;
				}
			mi++;
		}
	}
	free( map_buffer );
}

void Event_Supervisor_AI::CheckMapCorrect( uint32 mi )
{
	//make sure NULL terminator is present and line is not too long
	maps[mi].map_name_menu[40] = 0;	
	//if there are no walls then this is not a maze at all
	if( maps[mi].H_GO_entry == 0 || maps[mi].V_GO_entry == 0 )
	{
unrecoverable_error:
		memset( &maps[mi], 0, sizeof( Maze_Map_Info ) );
		return;
	}
	//GO size should be rational, more then visual range would get things bugged
	maps[mi].GO_size = MIN(maps[mi].GO_size,30);
	maps[mi].GO_size = MAX(maps[mi].GO_size,1);
	//check for team start positions. No teams = no map
	uint32 teams_found = 0;
	for(uint32 ts=0;ts<MAX_TEAMS_PER_EVENT;ts++)
		if( maps[mi].team_start_x[ts] != 0.0f )
			teams_found++;
	if( teams_found < 1 )
		goto unrecoverable_error;

	maps[mi].max_teams_per_event = teams_found;
	if( maps[mi].max_teams_per_event > MAX_TEAMS_PER_EVENT )
		maps[mi].max_players_in_team = MAX_TEAMS_PER_EVENT;

	//avoid imba gold ammounts ? is this a bug or a feature ?
//	for(uint32 ts=0;ts<MAX_REWARD_ITEM_IDS;ts++)
//		if( maps[mi].reward_gold[ts] > 1000000000 )
//			maps[mi].reward_gold[ts] = 0;

	if( maps[mi].max_players_in_team > MAX_PLAYERS_PER_TEAM || maps[mi].max_players_in_team == 0 )
		maps[mi].max_players_in_team = MAX_PLAYERS_PER_TEAM;
}

void Event_Supervisor_AI::DespawnEventMap()
{
	Event.status = TS_NOT_STARTED;
	Event.is_spawned = false;

	if( Event_map == NULL || m_Unit->GetMapMgr() == NULL )
		return;

	for( uint32 y=0;y<Event_map->map_size_y;y++)
		for( uint32 x=0;x<Event_map->map_size_x;x++)
			if( Event_map->spawn_map[x+y*Event_map->map_size_x] )
			{
				if( Event_map->entry_map[x+y*Event_map->map_size_x] < 0 )
				{
					Unit *u = m_Unit->GetMapMgr()->GetUnit( Event_map->spawn_map[x+y*Event_map->map_size_x] );
					if( u && u->IsInWorld() && u->IsCreature() )
					{
						SafeCreatureCast( u )->Despawn( 1, 0 );
						continue;
					}
				}
				else if( Event_map->entry_map[x+y*Event_map->map_size_x] > 0 )
				{
					GameObject *g = m_Unit->GetMapMgr()->GetGameObject( Event_map->spawn_map[x+y*Event_map->map_size_x] );
					if( g && g->IsInWorld() && g->IsGameObject() )
					{
						SafeGOCast( g )->Despawn( 1, 0 );
						continue;
					}
				}
			}
	memset( Event_map->spawn_map, 0, Event_map->map_size_x * Event_map->map_size_y * sizeof( uint64 ) );
}

void Event_Supervisor_AI::SpawnEventMap()
{
	if( Event_map == NULL || m_Unit->GetMapMgr() == NULL )
		return;

	//make sure. 99% this is already a despawned map
	DespawnEventMap();
	Event.is_spawned = true;

	//now spawn objects on it
	for( uint32 y=0;y<Event_map->map_size_y;y++)
		for( uint32 x=0;x<Event_map->map_size_x;x++)
		{
			float sx = Event_map->goal_x - Event_map->m_goal_x * Event_map->GO_size + x * Event_map->GO_size;
			float sy = Event_map->goal_y - Event_map->m_goal_y * Event_map->GO_size + y * Event_map->GO_size;
			float o = 0.0f;
			if( Event_map->entry_map[x+y*Event_map->map_size_x] > 0 )
			{
				uint32 entry = Event_map->entry_map[x+y*Event_map->map_size_x];
				//go oirentation is messed up. somehow they are relativ to each other
//				if( entry == Event_map->H_Gate_GO_entry || entry == Event_map->H_GO_entry )
					o = 0.0f;
//				else
//					o = 1.57f;	//this is good
				GameObject *g = m_Unit->GetMapMgr()->CreateGameObject( entry );
				if( g )
				{
					// Setup game object
					g->SetInstanceID(m_Unit->GetInstanceID());
					if( g->CreateFromProto( entry, m_Unit->GetMapId(),sx, sy, m_Unit->GetPositionZ() + Event_map->z_adjust, o) == false )
					{
						g->Despawn( 0 );
						continue;
					}
					g->InitAI();
					g->SetOrientation( o );
//					g->UpdateGoOrientation();
					g->PushToWorld( m_Unit->GetMapMgr() );
					Event_map->spawn_map[x+y*Event_map->map_size_x] = g->GetGUID();
				}
			}
			else if( Event_map->entry_map[x+y*Event_map->map_size_x] < 0 )
			{
				uint32 entry = -Event_map->entry_map[x+y*Event_map->map_size_x];
				Creature *c = m_Unit->GetMapMgr()->CreateCreature( entry );
				if( c )
				{
					CreatureProto * cp = CreatureProtoStorage.LookupEntry( entry );
					if( cp == NULL )
						continue;
					c->Load( cp, sx, sy, m_Unit->GetPositionZ(), o );
				    c->_setFaction();
				    c->GetAIInterface()->Init(c,AITYPE_AGRO,MOVEMENTTYPE_NONE);
					c->GetAIInterface()->agro_range_mod = 0.1f;
					c->PushToWorld( m_Unit->GetMapMgr() );
					Event_map->spawn_map[x+y*Event_map->map_size_x] = c->GetGUID();
				}
			}
		}
}
