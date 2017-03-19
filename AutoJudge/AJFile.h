#pragma once
#include"stdafx.h"
enum FileType
{
	Code, Header
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
	AJFile& operator=(const AJFile& aj);
	~AJFile();

	const string& Path = filepath;
	const string& Name = filename;
	const FileType& Type = filetype;
	friend bool operator< (const AJFile&, const AJFile&);
};

