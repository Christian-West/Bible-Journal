#include "Utility.h"
#include "hasPeriod.h"
#include "hasComma.h"
#include "hasSemiColon.h"
#include "hasNULL.h"
#include "hasSpace.h"
#include <iostream>
#include <conio.h>
using namespace std;

//***************************************************************************************************************************************

bool Utility::_isTextNotebook(const string& fName)
{
	bool correctFormat = false;
	int nameSize = fName.size();
	string subStr = "",
		   e;

	if ((nameSize > 4) && (fName.find(".txt", nameSize - 4) != string::npos))
	{
		subStr = fName.substr(0, nameSize - 4);

		if (!period->check(subStr, e))
			correctFormat = true;
		else
			errMsgs.push_back(e);
	}

	return correctFormat;
}

//***************************************************************************************************************************************

Utility::Utility()
{
	period = new hasPeriod();
	comma = new hasComma();
	semiColon = new hasSemiColon();
	space = new hasSpace();
	null = new hasNULL();
	err = "";
}

//***************************************************************************************************************************************

Utility::~Utility()
{
	delete period;
	delete comma;
	delete semiColon;
	delete space;
	delete null;
	err = "";
	errMsgs.clear();
}

//***************************************************************************************************************************************

bool Utility::isInt(const char stringIn[], int& intOut)
{
	bool isInteger = true;
	int stringSize = strlen(stringIn);

	for (int i = 0; isInteger && (i < stringSize); ++i)
	{
		if (!isdigit(stringIn[i]))
			isInteger = false;
	}

	if (isInteger)
		intOut = atoi(stringIn);
	else
		err = stringIn;

	return isInteger;
}

//***************************************************************************************************************************************

bool Utility::isOption(vector<string>& options, string& input)
{
	bool correctFormat = false;
	int numOpts = options.size(),
		inputSize = input.size();

	for (int i = 0; i < inputSize; ++i)
		input[i] = tolower(input[i]);

	for (int i = 0; i < numOpts; ++i)
	{
		for (int j = 0; j < options[i].size(); ++j)
			(options[i])[j] = tolower((options[i])[j]);
	}

	for (int i = 0; !correctFormat && (i < numOpts); ++i)
	{
		if (options[i] == input)
			correctFormat = true;
	}

	return correctFormat;
}
//***************************************************************************************************************************************

bool Utility::_goodItemFormat(const string& input)
{
	bool correctFormat = false;
	string e1,
		e2;

	if ((input.size() > 0) && !comma->check(input, e1) && !semiColon->check(input, e2))
		correctFormat = true;
	else
		errMsgs.insert(errMsgs.end(), { e1, e2 });

	return correctFormat;
}

//***************************************************************************************************************************************

bool Utility::goodUserFormat(const string& input)
{
	bool correctFormat = false;
	string e1,
		   e2;

	if (_goodItemFormat(input) && !null->check(input, e1) && !period->check(input, e2))
		correctFormat = true;
	else
		errMsgs.insert(errMsgs.end(), { e1, e2 });

	return correctFormat;
}

//***************************************************************************************************************************************

bool Utility::goodNotebookFormat(const string& input)
{
	bool correctFormat = false;
	int inputSize = input.size();
	string subStr = "",
		   e;
	vector<string> invalids = {"CON", "PRN", "NUL", "COM1", "LPT1"};

	if (_goodItemFormat(input) && !space->check(input, e) && _isTextNotebook(input))
	{
		correctFormat = true;

		for (int i = 0; correctFormat && (i < (inputSize - 4)); ++i)
		{
			if (!(isalpha(input[i]) || isdigit(input[i]) || (input[i] == 95) || (input[i] == 45)))
				correctFormat = false;
		}

		if (correctFormat)
		{
			subStr = input.substr(0, inputSize - 4);

			for (int i = 0; correctFormat && (i < 5); ++i)
			{
				if (input == invalids[i])
					correctFormat = false;
			}
		}
	}
	else
		errMsgs.push_back(e);

	return correctFormat;
}

//***************************************************************************************************************************************

void Utility::displayStrFormatErrs()
{
	int msgSize = errMsgs.size();

	for (int i = 0; i < msgSize; ++i)
	{
		if (errMsgs[i] != "")
			cout << "\t" << errMsgs[i] << "\n";
	}

	errMsgs.clear();
}

//***************************************************************************************************************************************

void Utility::pressKey()
{
	char ch;

	cout << "Press any key\n";
	ch = _getch();
}

//***************************************************************************************************************************************

void Utility::moveScreen(const int numTimes) const
{
	for (int i = 0; i < numTimes; ++i)
		cout << '\n';
}