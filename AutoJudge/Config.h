#pragma once
#include<map>
#include<string>
using namespace std;
class Config
{
public:
	static void Init();
	static string GetData(const string& i);
private:
	static map<string, string> data;
	static bool ReadFile();
	static void WriteDefaultToFile();
};
