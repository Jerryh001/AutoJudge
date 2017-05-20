#include"stdafx.h"
using namespace std;

int main()
{
	UI::Init();
	vector<Problem> problemlist;
	CFileFind finder;
	BOOL found = finder.FindFile(TEXT(R"(*.*)"));
	while (found)
	{
		found = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots() && strcmp(CT2CA(finder.GetFileName()), "AJ_temp"))
		{
			problemlist.push_back(Problem(string(CT2CA(finder.GetFilePath())),true));
		}
	}
	system(R"(rmdir .\AJ_temp /Q /S >nul 2>nul)");
	CreateDirectory(CA2CT("AJ_temp"),NULL);
	//system(R"(mkdir AJ_temp  >nul 2>nul)");
	//atexit([]() {system("taskkill /F /T /IM a.exe > nul 2> nul"); });
	for (Problem& p : problemlist)
	{
		p.ReadFile();
		p.Judge();
	}
	system("pause");
	return 0;
}