#ifndef UTILITY_H
#define UTILITY_H

//***************************************************************************************************************************************

#include "StrFormat.h"
#include <vector>

//***************************************************************************************************************************************

class Utility
{
private:
	StrFormat* period;
	StrFormat* comma;
	StrFormat* semiColon;
	StrFormat* space;
	StrFormat* null;
	std::string err;
	std::vector<std::string> errMsgs;
	bool _isTextNotebook(const std::string& input);
	bool _goodItemFormat(const std::string& input);

public:
	Utility();
	~Utility();
	bool isInt(const char stringIn[], int& intOut);
	bool isOption(std::vector<std::string>& options, std::string& input);
	bool goodUserFormat(const std::string& input);
	bool goodNotebookFormat(const std::string& input);
	std::string getErr() const;
	void displayStrFormatErrs();
	std::string invalid() const;
	std::string reEnter(const std::string& item) const;
	void pressKey();
	void moveScreen(const int numTimes) const;
	//tree for finding name?
};

//***************************************************************************************************************************************

inline std::string Utility::getErr() const { return err; }

//***************************************************************************************************************************************

inline std::string Utility::invalid() const { return "Invalid: "; }

//***************************************************************************************************************************************

inline std::string Utility::reEnter(const std::string& item) const { return ("\nPlease re-enter " + item + "\n"); }

//***************************************************************************************************************************************

#endif