#include "Config.h"
#include<fstream>
#include<map>
#include<string>
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
	ifstream in("config.ini");
	string input;
	if (!in.is_open())
	{
		in.close();
		return false;
	}
	while (getline(in, input))
	{
		size_t equal = input.find_first_of('=');
		data[input.substr(0, equal)] = input.substr(equal + 1);
	}
	in.close();
	return true;
}

void Config::WriteDefaultToFile()
{
	ofstream out("config.ini");
	out << R"(compiler=C:\Program Files (x86)\CodeBlocks\MinGW\bin)" << endl
		<< R"(time=1000)" << endl
		<< R"(headermode=0)" << endl
		<< R"(header=header.h)" << endl;
	out.close();
}
