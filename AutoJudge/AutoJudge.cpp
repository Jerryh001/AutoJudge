#include"stdafx.h"
using namespace std;

int main()
{
	Config::Init();
	UI::GetColor();
	Problem p;
	p.ReadFile(Code);
	p.Judge();
	return 0;
}