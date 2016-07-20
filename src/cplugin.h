#ifndef __CPLUGIN_H__
#define __CPLUGIN_H__

#include "amx/amx.h"
#include "amx/amxaux.h"

#include <string>

extern "C" int AMXAPI amx_ConsoleInit(AMX *amx);
extern "C" int AMXAPI amx_CoreInit(AMX *amx);
extern "C" int AMXAPI amx_FloatInit(AMX *amx);
extern "C" int AMXAPI amx_FixedInit(AMX *amx);
extern "C" int AMXAPI amx_StringInit(AMX *amx);
extern "C" int AMXAPI amx_ArgsInit(AMX *amx);
extern "C" int AMXAPI amx_DGramInit(AMX *amx);
extern "C" int AMXAPI amx_TimeInit(AMX *amx);

enum PLUGIN_STATE { ST_NONE, ST_OK, ST_FAIL };
enum PLUGIN_ERROR { ERR_NONE, ERR_SIZE, ERR_MEMORY, ERR_LOAD, ERR_VERSION, ERR_INIT, ERR_JIT };

class CPlugin
{
public:
	PLUGIN_STATE m_iState;
	PLUGIN_ERROR m_iError;
	std::string m_sFilename;
	std::string m_sFunc;

	CPlugin(std::string filename, std::string function, bool usejit);
	~CPlugin();

	bool Run();		// calls plugin_start()

	inline AMX* GetAMX() { return &amx; };

private:
	bool LoadProgram();
	bool RegisterNatives();
	bool CallPublic(std::string);
	size_t ProgramSize(std::string filename);

	AMX amx;
	void *m_pProgram = NULL;
};

#endif // __CPLUGIN_H__
