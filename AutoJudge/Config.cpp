#include "Config.h"
#include"stdafx.h"
using namespace std;
Config Config::globalsetting;
Config::Config(const string & folder):AJFile(folder,"config.ini", FileType::Setting)
{
	bool ret = ReadFile();
	if (folder == R"(.\)" && !ret)///isglobalsetting && not exist config.ini
	{
		WriteDefaultToFile();
		ReadFile();
	}
}
const string& Config::GetData(const string& segment, const string& key,const string& defaultvalue)
{
	string& d = data[segment][key];
	if (d.empty())
	{
		string& gd = GetDataGlobal(segment,key);
		if (gd.empty())
		{
			return defaultvalue;
		}
		return gd;
	}
	return d;
}
string& Config::GetDataGlobal(const string& segment, const string& key)
{
	return globalsetting.data[segment][key];
}
bool Config::ReadFile()
{
	CSimpleIniA ini;
	string input;
	if (ini.LoadFile(Path.c_str())<0)
	{
		return false;
	}
	CSimpleIniA::TNamesDepend sections, keys;
	ini.GetAllSections(sections);
	for (const auto& s : sections)
	{
		map<string, string> sdata;
		ini.GetAllKeys(s.pItem, keys);
		for (const auto& k : keys)
		{
			sdata[k.pItem] = ini.GetValue(s.pItem, k.pItem);
			//data[k.pItem] = ini.GetValue(s.pItem,k.pItem);
		}
		data[s.pItem] = move(sdata);
	}
	return true;
}

void Config::WriteDefaultToFile()
{
	ofstream out("config.ini");
	out << R"([GENERAL])" << endl
		<< R"(mode = 0		;0 = code, 1 = header)" << endl
		<< R"(cleanup = 0)" << endl
		<< R"([CODE])" << endl
		<< R"(coderegex = (\.cpp)$)" << endl
		<< R"([HEADER])" << endl
		<< R"(headerregex = (\.h)$)" << endl
		<< R"(main = main.cpp)" << endl
		<< R"(include = header.h)" << endl
		<< R"([COMPILE])" << endl
		<< R"(path = C:\Program Files(x86)\CodeBlocks\MinGW\bin)" << endl
		<< R"(options = )" << endl
		<< R"([LIMIT])" << endl
		<< R"(time = 3000)" << endl
		<< R"(memory = 32)" << endl
		<< R"([SCORE])" << endl
		<< R"(part-score = 0)" << endl
		<< R"([SEGMENT])" << endl
		<< R"(segment-score = 0)" << endl
		<< R"(token = )" << endl;
	out.close();
}


