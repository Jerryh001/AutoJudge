#include"Header.h"
#include<cstdlib>
#include<string>
#include<iostream>
using namespace std;
int Compile(const string& filename)
{
	string GppPath = R"("C:\Program Files (x86)\CodeBlocks\MinGW\bin\g++.exe")";
	string CompileOption = R"( -Wall -O2 -o a.exe > nul )";
	string CompileCommend = R"(")" + GppPath + R"( .\)" + filename + " " + CompileOption + R"(")";
	return system(CompileCommend.c_str());
}