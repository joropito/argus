#include "argus.h"
#include "cscript.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

CScript * createScript(char * filename)
{
	std::cout << "Loading Script\nVersion: " << pCore->version << std::endl;
	long memsize = aux_ProgramSize(filename);
	if(memsize==0)
	{
		printf("Script file not found or corrupted\n");
		return NULL;
	}

	void * program = malloc(memsize);

	if (program == NULL)
	{
		printf("Memory allocation for script failed\n");
		return NULL;
	}

	CScript * my_script= new CScript();
	if(!my_script->LoadProgram(filename,program))
	{
		printf("Loading script into Abstract Machine failed\n");
		delete my_script;
		return NULL;
	}

	return my_script;

}

CScript::CScript()
{
	program=NULL;
}

CScript::~CScript()
{
	amx_Cleanup(&amx);
	if (program != NULL)
		free(this->program);

}

void CScript::drop()
{
	delete this;
}


void CScript::Error(int error)
{
	printf("Run time error %d: \"%s\"\n",
			error, aux_StrError(error));
}

bool CScript::LoadProgram(char*file,void * _program)
{
	program = _program;
	this->err = aux_LoadProgram(&amx, file, program);
	if (err != AMX_ERR_NONE)
	{
	printf("LoadProgram\n");
		Error(this->err);
		return false;
	}
	return true;
}

int CScript::ExecMain()
{
	cell ret;
	this->err = amx_Exec(&amx, &ret, AMX_EXEC_MAIN);
	if (err != AMX_ERR_NONE)
	{
	printf("ExecMain\n");
		Error( this->err);
	}

	if (ret != 0)
		printf("Returns %ld\n", (long)ret);
	return ret;
}

bool CScript::registerNatives(const AMX_NATIVE_INFO *list)
{
	amx_ConsoleInit(&amx);
	err=amx_Register(&amx,list,-1);
	if (err != AMX_ERR_NONE)
	{
		Error( this->err);
		return false;
	}

	cell num=-1;
	amx_NumNatives(&amx,&num);

	printf("Registered %i native functions.\n",num);
	return true;
}
