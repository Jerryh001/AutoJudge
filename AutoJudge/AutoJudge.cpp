#include"stdafx.h"
using namespace std;

int _tmain()
{
	UI::Init();
	vector<Problem> problemlist;
	Problem::SetGlobalSetting(R"(.\config.ini)");
	CFileFind finder;
	BOOL found = finder.FindFile(TEXT(R"(*.*)"));
	while (found)
	{
		found = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots() && finder.GetFileName() != "AJ_temp")
		{
			problemlist.push_back(Problem(finder.GetFilePath()));
		}
	}
	finder.Close();
	_tsystem(TEXT(R"(rmdir .\AJ_temp /Q /S >nul 2>nul)"));
	CreateDirectory(TEXT("AJ_temp"),NULL);
	for (Problem& p : problemlist)
	{
		p.AutoJudge();
	}
	DWORD dwProcessId;
	GetWindowThreadProcessId(GetConsoleWindow(), &dwProcessId);
	if (GetCurrentProcessId() == dwProcessId)
	{
		_tsystem(TEXT("pause"));
	}
	
	return 0;
}