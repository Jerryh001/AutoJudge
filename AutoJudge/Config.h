#pragma once
#include"stdafx.h"
#include"AJFile.h"
using namespace std;
class Config:protected AJFile 
{
public:
	Config(const string& folder=R"(.\)");
	const string& GetData(const string& segment, const string& key,const string& defaultvalue="");
private:
	map<string, map<string,string>> data;
	static Config globalsetting;
	bool ReadFile();
	void WriteDefaultToFile();
	string& GetDataGlobal(const string& segment,const string& key);
};
