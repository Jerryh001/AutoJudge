#include "stdafx.h"
#include "AJFile.h"
using namespace std;

AJFile::AJFile()
{
}

AJFile::AJFile(const CString & path,const FileType& type)
{
	filepath = path;
	filetype = type;
	SetFilePath(filepath);
}

AJFile::AJFile(const AJFile& file)
{
	filepath = file.filepath;
	filetype = file.filetype;
	SetFilePath(file.GetFilePath());
}

void AJFile::SetFile(const CString & path, const FileType & type)
{
	SetFilePath(path);
	filetype = type;
}

CString AJFile::GetFilePath()const
{
	return CFile::GetFilePath();
}

CString AJFile::GetFileName()const
{
	return  CFile::GetFileName();
}

FileType AJFile::GetFileType()const
{
	return filetype;
}

CString AJFile::GetFileTitle() const
{
	return CFile::GetFileTitle();
}


bool operator<(const AJFile& a, const AJFile& b)
{
	return a.GetFileTitle()<b.GetFileTitle();
}
