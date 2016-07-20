#include "cthreadmgr.h"
#include <iostream>

#include <pthread.h>
#include <unistd.h>

using namespace std;

void *Thread_Run(void *p)
{
	CThreadMgr::ThreadPool_t* Thread = (CThreadMgr::ThreadPool_t*)p;
	CThreadMgr *mgr = Thread->m_pThreadMgr;

	mgr->m_pFunc(p);
	sleep(rand()%10);
	Thread->m_bActive = false;
	pthread_exit(NULL);
}

CThreadMgr::CThreadMgr(int max, int cleanuptime, string appname, void *(*func)(void*))
	: m_iMax(max), m_iCleanupTime(cleanuptime), m_pFunc(func), m_sAppName(appname), m_bShutdown(false)
{ };

CThreadMgr::~CThreadMgr() { };

/*
	On step at a time
*/
int CThreadMgr::Tick()
{
	ThreadPool_t* thr;
	void *status;

	/*
		Check for finished threads
	*/
	for(list<ThreadPool_t*>::iterator it=m_pThreads.begin(); it != m_pThreads.end(); ++it)
	{
		thr = (*it);
		if(!thr->m_bActive)
		{
			it++;
			FinishThread(thr->m_iThreadId);
		}
	}
	int total = ActiveThreads();
	
	/*
		Start/Restart threads for available slots
	*/
	while(total < m_iMax)
		total += StartThread(total);

}

void CThreadMgr::SetShutdown()
{
	m_bShutdown = true;
}

/*
	Shutdown the whole thread system
*/
int CThreadMgr::Shutdown()
{
	ThreadPool_t* thr;
	void *status;

	m_bShutdown = true;
	while(CountThreads() > 0)
	{
		for(list<ThreadPool_t*>::iterator it=m_pThreads.begin(); it != m_pThreads.end(); ++it)
		{
			thr = (*it);
			if(!thr->m_bActive)
			{
				it++;
				FinishThread(thr->m_iThreadId);
			}
		}
	}
}

/*
	Wait for a thread to finish
*/
int CThreadMgr::FinishThread(int iThreadId)
{
	ThreadPool_t* thr;
	void *status;

	for(list<ThreadPool_t*>::iterator it=m_pThreads.begin(); it != m_pThreads.end(); ++it)
	{
		thr = (*it);
		if(thr->m_iThreadId == iThreadId && !thr->m_bActive)
		{
			pthread_join(thr->m_pThread, &status);
			it = m_pThreads.erase(it);
			delete thr;
			return 0;
		}
	}
	return 1;
}

/*
	Start a thread
*/
int CThreadMgr::StartThread(int iThreadId)
{
	ThreadPool_t *thr = (new ThreadPool_t);

	thr->m_iThreadId = iThreadId;
	thr->m_bActive = true;
	thr->m_bHandled = false;
	thr->m_pThreadMgr = this;
	m_pThreads.push_back(thr);

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&thr->m_pThread, NULL, Thread_Run, (void*)thr);

	return 1;
}

int CThreadMgr::CountThreads()
{
	return m_pThreads.size();
}

int CThreadMgr::ActiveThreads()
{
	int total = 0;
	ThreadPool_t* thr;
	for(list<ThreadPool_t*>::iterator it=m_pThreads.begin(); it != m_pThreads.end(); ++it)
	{
		thr = (*it);
		if(thr->m_bActive)
			total++;
	}
	return total;
}

int CThreadMgr::FinishedThreads()
{
	int total = 0;
	ThreadPool_t* thr;
	for(list<ThreadPool_t*>::iterator it=m_pThreads.begin(); it != m_pThreads.end(); ++it)
	{
		thr = (*it);
		if(!thr->m_bActive)
			total++;
	}
	return total;
}
