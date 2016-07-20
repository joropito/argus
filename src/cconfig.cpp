#include "cconfig.h"
#include "ini.h"

#include <iostream>

using namespace std;

CConfig::CConfig(string filename)
{
	m_iError = ini_parse(filename.c_str(), ValueHandler, this);
}

string CConfig::MakeKey(string s, string n)
{
	string k = s + KEY_SEPARATOR + n;
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	return k;
}

int CConfig::ValueHandler(void* user, const char* section, const char* name, const char* value)
{
	CConfig* reader = (CConfig*)user;
	string key = reader->MakeKey(section, name);

#ifdef ARGUS_ALLOW_MKEYS
	if(reader->m_pValues[key].size() > 0)
		reader->m_pValues[key] += "\n";

	reader->m_pValues[key] += value;
#else
	reader->m_pValues[key] = value;
#endif

	return 1;
}

string CConfig::Get(string section, string name, string default_value) const
{
	string key = MakeKey(section, name);
	// Use m_pValues.find() here instead of m_pValues.at() to support pre C++11 compilers
	return m_pValues.count(key) ? m_pValues.find(key)->second : default_value;
}

long CConfig::GetInteger(string section, string name, long default_value) const
{
	string valstr = Get(section, name, "");
	const char* value = valstr.c_str();
	char* end;
	// This parses "1234" (decimal) and also "0x4D2" (hex)
	long n = strtol(value, &end, 0);
	return end > value ? n : default_value;
}

double CConfig::GetReal(string section, string name, double default_value) const
{
	string valstr = Get(section, name, "");
	const char* value = valstr.c_str();
	char* end;
	double n = strtod(value, &end);
	return end > value ? n : default_value;
}

bool CConfig::GetBoolean(string section, string name, bool default_value) const
{
	string valstr = Get(section, name, "");
	// Convert to lower case to make string comparisons case-insensitive
	std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
	if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
		return true;
	else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
		return false;
	else
		return default_value;
}

