#include "StrFormat.h"
using namespace std;

//***************************************************************************************************************************************

StrFormat::StrFormat()
{
	wrongInput = "";
	err = "";
}

//***************************************************************************************************************************************

bool StrFormat::check(const string& input, string& msg) 
{ 
	bool isThere = false;

	if (input.find(err) != string::npos)
	{
		wrongInput = input;
		isThere = true;
		msg = getErrMsg();
	}
	else
		msg = "";

	return isThere;
}