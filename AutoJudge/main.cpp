#define _AFXDLL
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
	string filename;
	CFileFind finder;
	BOOL bWorking = finder.FindFile(_T("*.cpp"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		filename = CT2A(finder.GetFileName());
		cout << filename << endl;
		if (0&&Compile(filename))
		{
			cout << "Compile Error" << endl;
			continue;
		}
		SHELLEXECUTEINFO info = { sizeof(SHELLEXECUTEINFO) ,SEE_MASK_NOCLOSEPROCESS ,NULL,TEXT("open"),TEXT("cmd"),TEXT("/c a > output.txt"),NULL ,SW_HIDE };
		ShellExecuteEx(&info);
		Sleep(1000);
		system(("taskkill /F /PID " + to_string(GetProcessId(info.hProcess))).c_str());
	}
	return EXIT_SUCCESS;
}