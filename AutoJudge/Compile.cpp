#include"Header.h"
#include<cstdlib>
#include<string>
#include<iostream>
using namespace std;
int Compile(const string& filename)
{
	string GppPath = Config::GetData("compiler");
	string CompileOption = R"( -Wall -O2 -o .\AJ_temp\a.exe >nul)";
	string CompileCommend = R"(cmd /C "set "PATH=)"+ GppPath +R"(;%PATH%" && g++ ".\)" + filename + R"(" )" + CompileOption + R"(")";
	//cout << CompileCommend << endl;
	return system(CompileCommend.c_str());
	
}
