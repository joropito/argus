
#include "cplugin.h"

#include <string>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <malloc.h>
#include <sys/mman.h>

using namespace std;

CPlugin::~CPlugin()
{
	amx_Cleanup(&amx);
	if(m_pProgram != NULL)
		free(m_pProgram);
}

CPlugin::CPlugin(string filename, string function, bool usejit = false)
: m_iState(ST_NONE), m_iError(ERR_NONE), m_sFilename(filename), m_sFunc(function)
{
	long memsize = ProgramSize(filename.c_str());
	if(memsize==0)
	{
		cout << "Script file not found or corrupted" << endl;
		m_iState = ST_FAIL;
		m_iError = ERR_SIZE;
	}
	else
	{
		m_pProgram = malloc(memsize);
		if(m_pProgram == NULL)
		{
			cout << "Malloc error" << endl;
			m_iState = ST_FAIL;
			m_iError = ERR_MEMORY;
		}
		else
		{
			LoadProgram();
		}
	}
}

size_t CPlugin::ProgramSize(string filename)
{
	FILE *fp;
	AMX_HEADER hdr;

	if ((fp=fopen(filename.c_str(),"rb")) == NULL)
		return 0;
	fread(&hdr, sizeof hdr, 1, fp);
	fclose(fp);

	amx_Align16(&hdr.magic);
	amx_Align32((uint32_t *)&hdr.stp);
	return (hdr.magic==AMX_MAGIC) ? (size_t)hdr.stp : 0;
}

bool CPlugin::LoadProgram()
{
	FILE *fp;
	AMX_HEADER hdr;
	int err, didalloc;

	if ((fp = fopen(m_sFilename.c_str(), "rb")) == NULL)
	{
		m_iState = ST_FAIL;
		m_iError = ERR_LOAD;
		return false;
	}

	fread(&hdr, sizeof hdr, 1, fp);
	amx_Align16(&hdr.magic);
	amx_Align32((uint32_t *)&hdr.size);
	amx_Align32((uint32_t *)&hdr.stp);
	if (hdr.magic != AMX_MAGIC) {
		fclose(fp);
		m_iState = ST_FAIL;
		m_iError = ERR_VERSION;
		return false;
	}

	didalloc = 0;
	if (m_pProgram == NULL) {
		if ((m_pProgram = malloc(hdr.stp)) == NULL) {
			fclose(fp);
			m_iState = ST_FAIL;
			m_iError = ERR_MEMORY;
			return false;
		} /* if */
		didalloc = 1;
	}

	rewind(fp);
	fread(m_pProgram, 1, (size_t)hdr.size, fp);
	fclose(fp);

	memset(&amx, 0, sizeof amx);

	err = amx_Init(&amx, m_pProgram);

	if(err != AMX_ERR_NONE && didalloc)
	{
		free(m_pProgram);
		amx.base = NULL;
		m_iState = ST_FAIL;
		m_iError = ERR_INIT;
		return false;
	}

	m_iState = ST_OK;
	RegisterNatives();
	return true;
}

bool CPlugin::RegisterNatives()
{
	amx_ConsoleInit(&amx);
	amx_CoreInit(&amx);
	amx_FloatInit(&amx);
	amx_FixedInit(&amx);
	amx_StringInit(&amx);
	amx_ArgsInit(&amx);
	amx_DGramInit(&amx);
	amx_TimeInit(&amx);
}

bool CPlugin::Run()
{
	CallPublic(m_sFunc);
}

bool CPlugin::CallPublic(string function)
{
	int index;
	int err = amx_FindPublic(&amx, function.c_str(), &index);

	if(err)
		return false;

	err =  amx_Exec(&amx, NULL, index);
	if(err != AMX_ERR_NONE)
		return false;

	return true;
}

