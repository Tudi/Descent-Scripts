#ifndef _EVENT_SUPERVISOR_H_
#define _EVENT_SUPERVISOR_H_

class Event_Supervisor_AI : public AIInterface
{
public:
	void					Init(Unit *un, AIType at, MovementType mt);
	void					Update(uint32 p_time);
	void					CacheMaps();
	void					DespawnEventMap();
	void					SpawnEventMap();
	void					ResetEventForNew( );
	void					ReloadMaps();
	Selected_Event_info		Event;
	Maze_Map_Info			*Event_map;
	uint32					debug_commands;
	Maze_Map_Info			maps[MAX_MAPS_CACHED];
	void					BroadcastToTournamentSafe( const char *what );
private:
	uint32					global_update_next_stamp;		//do not spam updates in case there are multiple supervizor spawns
	uint32					forced_reset_next_stamp;		//in case some state gets stuck then we use this to reset the whole NPC

//	Mutex					UpdateMutex;
		
	void					UpdateEventProgress( );
	void					BroadcastToTournament( const char *what );
	void					CheckMapCorrect( uint32 map_ind );	
	void					UpdateGates();				//despawns gates that do not have some MOB near them
	void					SendGoalPositionToPlayers();

	void					RemoveEventStatesFromPlayers( );
	void					HandleEventWin( uint32 team_id );
	void					AssignTeamStartPossitions();
	void					DespawnStartGates();
};

#endif