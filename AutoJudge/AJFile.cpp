#include "stdafx.h"
#include "AJFile.h"
using namespace std;

AJFile::AJFile()
{
}

AJFile::AJFile(const CFileFind & find)
{
	filename = CT2CA(find.GetFileName());
	filepath = CT2CA(find.GetFilePath());
}

AJFile::AJFile(const AJFile & aj)
{
	*this=aj;
}

AJFile::AJFile(const string & folder, const string & name, const FileType & type)
{
	filename = name;
	filepath = folder+ name;
	filetype = type;
}

AJFile & AJFile::operator=(const AJFile & aj)
{
	filename = aj.filename;
	filepath = aj.filepath;
	filetype = aj.filetype;
	return *this;
}


AJFile::~AJFile()
{
}

bool operator<(const AJFile& a, const AJFile& b)
{
	return a.filename<b.filename;
}
