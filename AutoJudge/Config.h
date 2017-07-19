#pragma once
#include"stdafx.h"
#include"AJFile.h"
using namespace std;
class Config:protected AJFile 
{
public:
	Config(const CString& path=R"(.\config.ini)");
	const CString& GetData(const CString& segment, const CString& key,const CString& defaultvalue="");
	void SetPath(const CString& path);
	static void SetGlobalPath(const CString& path);
private:
	map<CString, map<CString,CString>> data;
	static Config globalsetting;
	bool ReadFile();
	void WriteDefaultToFile();
	CString& GetDataGlobal(const CString& segment,const CString& key);
};
