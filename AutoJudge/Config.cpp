#include "Config.h"
#include"stdafx.h"
using namespace std;
Config Config::globalsetting;
Config::Config(const CString &path) :AJFile(path, FileType::Setting)
{
	bool ret = ReadFile();
	if (this == &globalsetting && !ret)///isglobalsetting && not exist config.ini
	{
		WriteDefaultToFile();
		ReadFile();
	}
}
const CString& Config::GetData(const CString& segment, const CString& key, const CString& defaultvalue)
{
	CString& d = data[segment][key];
	if (d.IsEmpty())
	{
		CString& gd = GetDataGlobal(segment, key);
		if (gd.IsEmpty())
		{
			return defaultvalue;
		}
		return gd;
	}
	return d;
}
void Config::SetPath(const CString & path)
{
	SetFile(path, FileType::Setting);
	ReadFile();
}
void Config::SetGlobalPath(const CString & path)
{
	globalsetting.SetFile(path,FileType::Setting);
	bool ret = globalsetting.ReadFile();
	if (!ret)///not exist config.ini
	{
		globalsetting.WriteDefaultToFile();
		globalsetting.ReadFile();
	}
}
CString& Config::GetDataGlobal(const CString& segment, const CString& key)
{
	return globalsetting.data[segment][key];
}
bool Config::ReadFile()
{
	CSimpleIni ini;
	CString input;
	if (ini.LoadFile(GetFilePath()) < 0)
	{
		return false;
	}
	CSimpleIni::TNamesDepend sections, keys;
	ini.GetAllSections(sections);
	for (const auto& s : sections)
	{
		map<CString, CString> sdata;
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
		<< R"(mode = 0)" << endl
		<< R"(;0 = code, 1 = header)" << endl
		<< R"(cleanup = 0)" << endl
		<< R"([CODE])" << endl
		<< R"(coderegex = (\.cpp)$)" << endl
		<< R"([HEADER])" << endl
		<< R"(headerregex = (\.h)$)" << endl
		<< R"(main = main.cpp)" << endl
		<< R"(include = header.h)" << endl
		<< R"([COMPILE])" << endl
		<< R"(path = C:\Program Files (x86)\CodeBlocks\MinGW\bin)" << endl
		<< R"(options = )" << endl
		<< R"([LIMIT])" << endl
		<< R"(time = 3000)" << endl
		<< R"(memory = 32)" << endl
		<< R"([SCORE])" << endl
		<< R"(part-score = 0)" << endl
		<< R"(allowPE = 0)" << endl
		<< R"([SEGMENT])" << endl
		<< R"(segment-score = 0)" << endl
		<< R"(token = )" << endl;
	out.close();
}


