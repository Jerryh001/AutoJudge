#ifndef _AFXDLL
#define _AFXDLL
#endif
#include<fstream>
#include<string>
#include<afx.h>
#include"Header.h"
int Compare(const string& infilename)
{
	ifstream user(R"(.\AJ_temp\)"+ infilename);
	ifstream ans(R"(.\output\)" + infilename);
	string userstr, ansstr;
	for (int line=1;;line++)
	{
		getline(user, userstr);
		getline(ans, ansstr);
		if (user.eof() != ans.eof() || userstr != ansstr)
		{
			return line;
		}
		if (user.eof())
		{
			return 0;
		}
	}
}