#include "argus.h"
#include "cthreadmgr.h"
#include "cplugin.h"
#include <sstream>
#include <iostream>

#include <unistd.h>
#include <stdio.h>

using namespace std;

CConfig* gp_Config;
CThreadMgr* gp_Threads;
int g_iSleepTime;

template <typename T>
string ToString(T val)
{
    stringstream stream;
    stream << val;
    return stream.str();
}

int main()
{
	CConfig cfg(ARGUS_INI_FILE);
	gp_Config = &cfg;

	g_iSleepTime = gp_Config->GetInteger(ARGUS_TAG_CONFIG, ARGUS_TAG_SLEEPTIME, 1000000);

	int apps = gp_Config->GetInteger(ARGUS_TAG_CONFIG, ARGUS_TAG_APPS, 0);

	string mdir = gp_Config->Get(ARGUS_TAG_CONFIG, ARGUS_TAG_MODULEDIR, "./");
	setenv("AMXLIB", mdir.c_str(), true);

	if(!apps)
		return 1;

	CThreadMgr *thr;
	list<CThreadMgr*>appthreads;

	for(int i=0; i < apps; i++)
	{
		string appname = "APP_" + ToString(i);
		if(gp_Config->GetInteger(appname, ARGUS_TAG_THREADS, -1) > 0)
		{
			thr = new CThreadMgr(
				gp_Config->GetInteger(appname, ARGUS_TAG_THREADS, 10),
				gp_Config->GetInteger(appname, ARGUS_TAG_CLEANUPTIME, 900),
				appname,
				LoopFrame
			);
			appthreads.push_back(thr);
		}
	}

	cout << "Argus version " << gp_Config->Get(ARGUS_TAG_CONFIG, ARGUS_TAG_VERSION, "") << endl;

	int test = gp_Config->GetInteger(ARGUS_TAG_CONFIG, ARGUS_TAG_TEST, 0);

	while(1)
	{
		static int i = 0;
		for(list<CThreadMgr*>::iterator it=appthreads.begin(); it!= appthreads.end(); ++it)
		{
			thr = (CThreadMgr*)(*it);
			thr->Tick();
		}
//		gp_Threads->Tick();
		usleep(g_iSleepTime);
		i++;
		if(test > 0 && i > test)
		{
			for(list<CThreadMgr*>::iterator it=appthreads.begin(); it!= appthreads.end(); ++it)
			{
				thr = (CThreadMgr*)(*it);
				thr->SetShutdown();
			}
			for(list<CThreadMgr*>::iterator it=appthreads.begin(); it!= appthreads.end(); ++it)
			{
				thr = (CThreadMgr*)(*it);
				thr->Shutdown();
				delete thr;
			}
			break;
		}
	}

}

void *LoopFrame(void *p)
{
	CThreadMgr::ThreadPool_t* Thread = (CThreadMgr::ThreadPool_t*)p;
	CThreadMgr *mgr = Thread->m_pThreadMgr;

	string appname = mgr->GetAppName();
	bool loop = !(gp_Config->Get(appname, ARGUS_TAG_ACTION, "")).compare("loop");
	int pause = gp_Config->GetInteger(appname, ARGUS_TAG_PAUSE, 0);

	string plugin = gp_Config->Get(ARGUS_TAG_CONFIG, ARGUS_TAG_PLUGINDIR, "./") + gp_Config->Get(appname, ARGUS_TAG_PLUGIN, ARGUS_DEFAULT_PLUGIN);
	string framefunc = gp_Config->Get(appname, ARGUS_TAG_FRAMEFUNC, ARGUS_DEFAULT_FRAMEFUNC);
	CPlugin pl(plugin, framefunc, false);

	while(1)
	{
		if(!mgr->m_bShutdown)
			pl.Run();

		if(!loop || mgr->m_bShutdown)
		{
			break;
		}
		else
		{
			sleep(pause);
		}
	}
//	cout << appname << " process shut down" << endl;
}
