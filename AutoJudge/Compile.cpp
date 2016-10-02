#include"Header.h"
#include<cstdlib>
#include<string>
#include<iostream>
using namespace std;
int Compile(const string& filename)
{
	string GppPath = "\"C:\\Program Files (x86)\\CodeBlocks\\MinGW\\bin\\g++.exe\"";
	//string testcpp = u8"test.cpp";
	string CompileOption = "-Wall -O2";
	string CompileCommend = GppPath + " " + filename + " " + CompileOption;
	return system(CompileCommend.c_str());
}