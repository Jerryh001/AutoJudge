#pragma once
#include"stdafx.h"
using namespace std;
enum FileType
{
	Code, Header, Setting,Input,Output
};
class AJFile:protected CFile
{
	protected:
	
	//CString filename;
	CString filepath;
	FileType filetype;
	public:
	AJFile();
	AJFile(const CString& file,const FileType& type);
	AJFile(const AJFile& file);
	void SetFile(const CString& file, const FileType& type);
	CString GetFilePath()const;
	CString GetFileName()const;
	FileType GetFileType()const;
	CString GetFileTitle()const;
	friend bool operator< (const AJFile&, const AJFile&);
};

