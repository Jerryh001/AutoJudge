#pragma once
#include"stdafx.h"
using namespace std;
enum FileType
{
	Code, Header, Setting
};
class AJFile
{
	protected:
	
	string filename;
	string filepath;
	FileType filetype;
	public:
	AJFile();
	AJFile(const CFileFind& find);
	AJFile(const AJFile& aj);
	AJFile(const string& folder, const string& name, const FileType& type = Setting);
	AJFile& operator=(const AJFile& aj);
	~AJFile();

	const string& Path = filepath;
	const string& Name = filename;
	const FileType& Type = filetype;
	friend bool operator< (const AJFile&, const AJFile&);
};

