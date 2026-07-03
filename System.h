#ifndef SYSTEM_H
#define SYSTEM_H

//***************************************************************************************************************************************

#include "DataManager.h"
#include "Utility.h"

//***************************************************************************************************************************************

class System
{
private:
	User* user;
	Notebook* currNotebook;
	DataManager* dm;
	Utility* utl;
    std::vector<Notebook*> notebooks;
	std::vector<User*> users;
	bool canFunction;
	int numNotebooks,
		numUsers;
	bool _findNotebook(const int id, int& index);
	bool _findUser(const int id, int& index);
	bool _findPassword(const int index, const std::string& password);
	bool _itemExists(const std::string& item);
	
public:
	System(bool& functional);
	~System();
	bool createAccount(const std::string& username, const std::string& password);
	bool login(const std::string& username, const std::string& password);
	bool displayUserNotebooks();
	bool displayUserHasAccess();
	bool displayUserGivesAccess(const int notebookID);
	bool viewAllNotebooks();
	bool displayAllUsers();
	bool viewNotebook(const int notebookID);
	bool createNotebook(const std::string& name);
	bool editNotebook(const int notebookID);
	bool deleteNotebook(const int notebookID);
	bool givePermission(const int notebookID, std::vector<int> userIDs);
	bool removePermission(const int notebookID, std::vector<int> userIDs);
	void deleteAccount();
	std::string getCurrUsername() const;
};

//***************************************************************************************************************************************

inline std::string System::getCurrUsername() const { return user->getName(); }

//***************************************************************************************************************************************

#endif