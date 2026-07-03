#ifndef CONTROLLER_H
#define CONTROLLER_H

//***************************************************************************************************************************************

#include "System.h"
#include "Utility.h"

//***************************************************************************************************************************************

class Controller    // for input validation & most IO
{
private:
	System* system;
	Utility* utl;
	std::string strInput;
	char* charInput;
	int option;
	bool canFunction;
	bool retry;
	void _loginScreen();

public:
	Controller();
	~Controller();
	bool login();
	void menu();
	bool validate(char stringIn[], int& intOut);
	void viewNotebook();
	void addNotebook();
	void editNotebook();
	void deleteNotebook();
	void giveAccess();
	void removeAccess();
	bool deleteAccount();
};

//***************************************************************************************************************************************

inline bool Controller::validate(char stringIn[], int& intOut) { return utl->isInt(stringIn, intOut); }

//***************************************************************************************************************************************

#endif