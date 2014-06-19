#include "StdAfx.h"
#include <ScriptSetup.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi")

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_MISC;
}


static uint32 blocking_thread_last_update=0;
static float Last_Ram_Mb_usage = 0;
static uint32 Biggest_thread_update_lag = 0;
static uint32 kill_process_at = 0xFFFFFFFF;

#define SHUTDOWN_IF_MEMORY_USAGE_GREATER 3800 //given in MBytes
#define TEST_DEADLOCK_INTERVAL (12*60*1000)
//#define TEST_DEADLOCK_INTERVAL (0.5*60*1000)
#define RESTART_SERVER_IF_MISSING_UPDATES 4 //if the thread testing deadblock does not respond X times then it is probably blocked
#define MERCIFULL_SHUTDOWN_TIMEOUT 12*60*1000 //if it will not shutdown in 2 minutes then we shut it down the hard way
#define SHUTDOWN_IF_THREAD_LAG_IS 12*60*1000 //monitor each active thread. If they did not update thir stamp then lag aocures and we monitor that

void write_lastminute_info_to_file(char *str)
{
	FILE *crashlog=fopen("deadlockdetector_out.txt","a");
	if( crashlog )
	{
		fprintf( crashlog, "%s",str);
		fclose( crashlog );
	}
}

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	return TRUE;
}

struct TestDeadBlockThread : public ThreadBase
{
	HANDLE hEvent;
	bool running;
public:
	TestDeadBlockThread(){ };
	~TestDeadBlockThread() { CloseHandle(hEvent); }
	void OnShutdown()
	{
		running = false;
		SetEvent( hEvent );
	}
	bool run();
};

bool TestDeadBlockThread::run()
{
	hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN);

	running = true;
	for(;;)
	{
		WaitForSingleObject( hEvent, TEST_DEADLOCK_INTERVAL );
	
		//////////////////////////////////////////////////
		//let's test some basic functions : lol test
		uint32 *tp = new uint32[100];
		delete tp;
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
		Last_Ram_Mb_usage = (float)pmc.PagefileUsage;
		Last_Ram_Mb_usage = Last_Ram_Mb_usage / 1024.0f / 1024.0f;
		//we should seee if our threadpool changes in time and maybe close deadblocked threads
		Biggest_thread_update_lag = ThreadPool.GetLastThreadUpdateStamp();
		if ( Biggest_thread_update_lag && Biggest_thread_update_lag < GetTickCount() )
		{
			Biggest_thread_update_lag = GetTickCount() - Biggest_thread_update_lag;
//printf("Biggest_thread_update_lag = %u, time now %u \n",Biggest_thread_update_lag,GetTickCount());
		}
		else Biggest_thread_update_lag = 0x00;
/*		uint32 nowtime=GetTickCount();
		uint32 delayed_player_saves=0;
		uint32 total_players_found=0;
		uint32 total_delay_tim=0;
		objmgr._playerslock.AcquireReadLock();
		PlayerStorageMap::const_iterator itr;
		for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
		{
			total_players_found++;
			if(itr->second->m_nextSave < nowtime )
			{
				delayed_player_saves++;
				total_delay_tim += nowtime - itr->second->m_nextSave;
			}
		}
		bool risk_a_shutdown = false;
		if( delayed_player_saves && total_delay_tim / delayed_player_saves > 2*60*1000 )
			risk_a_shutdown = true;*/
//		printf("Supervising functionality\n");
		//////////////////////////////////////////////////

		blocking_thread_last_update = GetTickCount();
		if( !running )
			break;
	}

	return true;
}

///////////////////////////

struct SuperviseDeadBlockThread : public ThreadBase
{
	HANDLE hEvent;
	bool running;
public:
	SuperviseDeadBlockThread(){ };
	~SuperviseDeadBlockThread() { CloseHandle(hEvent); }
	void OnShutdown()
	{
		running = false;
		SetEvent( hEvent );
	}
	bool run();
};

///////////////////////////

struct BarbaricEndProcessThread : public ThreadBase
{
	HANDLE hEvent;
	bool running;
public:
	BarbaricEndProcessThread(){ };
	~BarbaricEndProcessThread(){} ;
	void OnShutdown()
	{
		running = false;
		SetEvent( hEvent );
	}
	bool run()
	{
		hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
		SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN);
		for(;;)
		{
			WaitForSingleObject( hEvent, TEST_DEADLOCK_INTERVAL );
			if( kill_process_at < GetTickCount() )
				TerminateProcess( GetCurrentProcess(), (DWORD) -1 );
			if( !running )
				break;
		}
		return true;
	}
};

bool SuperviseDeadBlockThread::run()
{
	hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN);

	running = true;
	uint32 last_update_stamp = blocking_thread_last_update;
	uint32 missign_updates_counter = 0;
	for(;;)
	{
		WaitForSingleObject( hEvent, TEST_DEADLOCK_INTERVAL );
		if( last_update_stamp == blocking_thread_last_update )
			missign_updates_counter++;
		else 
		{
			last_update_stamp = blocking_thread_last_update;
			missign_updates_counter = 0;
		}

		if( Last_Ram_Mb_usage > SHUTDOWN_IF_MEMORY_USAGE_GREATER 
			|| Biggest_thread_update_lag > SHUTDOWN_IF_THREAD_LAG_IS
			)
		{
			kill_process_at = GetTickCount() + MERCIFULL_SHUTDOWN_TIMEOUT;
			printf("!!!Ram Usage exceeded allowed amount (%f) or thread has too big lag %u. Shutting down ! \n",Last_Ram_Mb_usage,Biggest_thread_update_lag);
			PlayerStorageMap::const_iterator itr;
			objmgr._playerslock.Acquire();
			for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
				if(itr->second->GetSession())
					itr->second->SaveToDB(false);
			objmgr._playerslock.Release();
			//only send shutdown to other thread if we did not deadlock. We cannot save a deadlocked map
			//also set shutdown event after saving players to avoid concurent saving
			if( Biggest_thread_update_lag < SHUTDOWN_IF_THREAD_LAG_IS )
				sMaster.m_ShutdownEvent = true;
			char buff[500];
			sprintf(buff,"Mem usage %u and thread lag was %u\n",Last_Ram_Mb_usage,Biggest_thread_update_lag);
			write_lastminute_info_to_file(buff);
		}
		else printf("current RAM usage is %f biggest thread update lag %u\n",Last_Ram_Mb_usage,Biggest_thread_update_lag);
		if( missign_updates_counter == RESTART_SERVER_IF_MISSING_UPDATES )
		{
			kill_process_at = GetTickCount() + MERCIFULL_SHUTDOWN_TIMEOUT;
			printf("!!!detected deadblock, Will try to shutdown or end process \n");
			sMaster.m_ShutdownEvent = true;
		}

		if( !running )
			break;
	}

	return true;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
	ThreadPool.ExecuteTask( new TestDeadBlockThread() );
	ThreadPool.ExecuteTask( new SuperviseDeadBlockThread() );
	ThreadPool.ExecuteTask( new BarbaricEndProcessThread() );
}
