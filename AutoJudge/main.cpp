#ifndef _AFXDLL
#define _AFXDLL
#endif
#include<cstdlib>
#include<string>
#include<iostream>
#include<fstream>
#include<afx.h>
#include<shellapi.h>
#include"Header.h"
using namespace std;
int main(int argc, char* argv[])
{
	string filename, infilename;
	CFileFind finder,infinder;
	BOOL bWorking = finder.FindFile(_T("*.cpp"));
	
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		filename = CT2A(finder.GetFileName());
		cout << filename << endl;
		if (Compile(filename))
		{
			cout << "Compile Error" << endl;
			continue;
		}
		BOOL infile = infinder.FindFile(TEXT(".\\input\\*.*"));
		bool AC = true;
		while (infile)
		{
			infile = infinder.FindNextFile();
			if (infinder.IsDots() || infinder.IsDirectory()) continue;
			infilename = CT2A(infinder.GetFilePath());
			CA2T parameter = ("/c a > .\\temp\\output.txt < "+infilename).c_str();
			SHELLEXECUTEINFO info = { sizeof(SHELLEXECUTEINFO) ,SEE_MASK_NOCLOSEPROCESS ,NULL,TEXT("open"),TEXT("cmd"),parameter,NULL ,SW_HIDE };
			ShellExecuteEx(&info);
			Sleep(1000);
			if (system(("taskkill  /F /T /PID " + to_string(GetProcessId(info.hProcess)) + " >nul 2> nul").c_str()))
			{
				if (Compare(infilename))
				{
					AC = false;
					cout << "Wrong Answer" << endl;
					break;
				}
			}
			else
			{
				AC = false;
				cout << "Time Limit Exceeded" << endl;
				break;
			}
		}
		if (AC)
		{
			cout << "Answer Correct" << endl;
		}
	}
	return EXIT_SUCCESS;
}