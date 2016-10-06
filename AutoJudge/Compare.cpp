#ifndef _AFXDLL
#define _AFXDLL
#endif
#include<fstream>
#include<string>
#include<afx.h>
#include"Header.h"
bool Compare(const string& infilename)
{
	ifstream user(".\\temp\\output.txt");
	ifstream ans(".\\temp\\" + infilename);
	string userstr, ansstr;
	while (1)
	{
		getline(user, userstr);
		getline(ans, ansstr);
		if (user.eof() != ans.eof() || userstr != ansstr)
		{
			return 0;
		}
		if (user.eof())
		{
			return 1;
		}
	}
}