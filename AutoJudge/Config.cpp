#include "Config.h"
#include"stdafx.h"
using namespace std;
map<string, string> Config::data;
void Config::Init()
{
	if (!ReadFile())
	{
		WriteDefaultToFile();
		ReadFile();
	}
}
string Config::GetData(const string& i)
{
	return Config::data[i];
}

bool Config::ReadFile()
{
	CSimpleIniA ini;
	string input;
	if (ini.LoadFile("config.ini")<0)
	{
		return false;
	}
	CSimpleIniA::TNamesDepend sections, keys;
	ini.GetAllSections(sections);
	for (const auto& s : sections)
	{
		ini.GetAllKeys(s.pItem, keys);
		for (const auto& k : keys)
		{
			data[k.pItem] = ini.GetValue(s.pItem,k.pItem);
		}
	}
	return true;
}

void Config::WriteDefaultToFile()
{
	ofstream out("config.ini");
	out <<R"([INFO])"<<endl
		<< R"(compiler=C:\Program Files (x86)\CodeBlocks\MinGW\bin)" << endl
		<< R"(time=1000)" << endl
		<< R"(headermode=0)" << endl
		<< R"(header=header.h)" << endl;
	out.close();
}
