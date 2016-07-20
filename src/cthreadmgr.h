#ifndef __CTHREADMGR_H__
#define __CTHREADMGR_H__

#include "argus.h"
#include <list>

void *Thread_Run(void *p);

class CThreadMgr
{
public:
	struct ThreadPool_t
	{
		pthread_t m_pThread;
		CThreadMgr *m_pThreadMgr;
		int m_iThreadId;
		bool m_bActive;
		bool m_bHandled;
	};
private:
	std::list <ThreadPool_t*> m_pThreads;
	std::string m_sAppName;

	int m_iMax;
	int m_iCleanupTime;

public:
	CThreadMgr(int max, int cleanuptime, std::string appname, void *(*func)(void*));
	~CThreadMgr();

	void *(*m_pFunc)(void*);

	int Tick();
	void SetShutdown();
	int Shutdown();
	int StartThread(int iThreadId);
	int FinishThread(int iThreadId);
	std::string GetAppName() { return m_sAppName; };
	int ActiveThreads();
	int CountThreads();
	int FinishedThreads();

	bool m_bShutdown;

};

#endif	// __CTHREADMGR_H__
