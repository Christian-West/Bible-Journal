#ifndef STR_FORMAT_H
#define STR_FORMAT_H

//***************************************************************************************************************************************

#include <string>

//***************************************************************************************************************************************

class StrFormat
{
protected:
	std::string wrongInput;
	std::string err;

public:
	StrFormat();
	bool check(const std::string& input, std::string& msg);
	std::string getErr() const;
	std::string getErrMsg() const;
	virtual void parent() = 0;
};

//***************************************************************************************************************************************

inline std::string StrFormat::getErr() const { return wrongInput; }

//***************************************************************************************************************************************

inline std::string StrFormat::getErrMsg() const { return (" may not contain \'" + err + "\'"); }

//***************************************************************************************************************************************

#endif