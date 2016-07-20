#include "argus.h"
#include "cplugin.h"

#include <iostream>
#include <string>
#include <stdio.h>

#include <dlfcn.h>

using namespace std;

int main(int argc, char *argv[])
{
	CPlugin p(argv[1], "main", false);

	cout << "status: " << p.m_iState << endl;
	p.Run();
	return 0;
}
