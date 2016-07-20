#ifndef __ARGUS_H__
#define __ARGUS_H__

#include "cconfig.h"

#define	ARGUS_NAME	"Argus"
#define	ARGUS_INI_FILE	"argus.ini"

#define	ARGUS_TAG_CONFIG	"config"
#define	ARGUS_TAG_PLUGINDIR	"plugindir"
#define	ARGUS_TAG_MODULEDIR	"moduledir"
#define	ARGUS_TAG_VERSION	"version"
#define	ARGUS_TAG_THREADS	"threads"
#define	ARGUS_TAG_CLEANUPTIME	"cleanuptime"
#define	ARGUS_TAG_APPS		"apps"
#define	ARGUS_TAG_TEST		"test"
#define	ARGUS_TAG_PLUGIN	"plugin"
#define	ARGUS_TAG_FRAMEFUNC	"framefunc"
#define	ARGUS_TAG_SLEEPTIME	"sleeptime"
#define	ARGUS_TAG_ACTION	"action"
#define	ARGUS_TAG_PAUSE		"pause"

#define ARGUS_DEFAULT_PLUGIN	"default.amx"
#define ARGUS_DEFAULT_FRAMEFUNC	"frame"

extern CConfig* gp_Config;

void *LoopFrame(void* p);

#endif	// __ARGUS_H__
