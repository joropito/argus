#ifndef __CCONFIG_H__
#define __CCONFIG_H__

#include <map>
#include <string>
#include <algorithm>

#define KEY_SEPARATOR	"."

class CConfig
{
private:
	int m_iError;
	std::map<std::string, std::string> m_pValues;
	static int ValueHandler(void* user, const char* section, const char* name, const char* value);
	static std::string MakeKey(std::string s, std::string n);

public:
	CConfig(std::string filename);
	 ~CConfig() {};

	inline int ParseError() const { return m_iError; };
	std::string Get(std::string section, std::string name, std::string default_value) const;
	long GetInteger(std::string section, std::string name, long default_value) const;
	double GetReal(std::string section, std::string name, double default_value) const;
	bool GetBoolean(std::string section, std::string name, bool default_value) const;

};

#endif	// __CCONFIG_H__
