#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

//***************************************************************************************************************************************

#include "User.h"

//***************************************************************************************************************************************

class DataManager
{
private:
	std::fstream notebooksNotebook;
	std::fstream usersNotebook;
	std::fstream hasAccessNotebook;
	std::fstream notebooksNotebook1;
	std::fstream usersNotebook1;
	std::fstream hasAccessNotebook1;
	std::fstream updatesNotebook;
	std::FILE* remover;
	bool canFunction;
	bool _bootUp();
	bool _findNotebook(std::string& dataLine, std::vector<Notebook*>& notebooks, int& index);
	bool _findUser(std::vector<std::string>&, std::vector<User*>& users, int& id, const int index);
	bool _findUser(std::string& dataLine, std::vector<User*>& users, int& index);
	bool _checkNotebooks();
	bool _copyNotebooks();
	void _findUniqueUpdates(std::vector<std::string>& notebookUpdates, std::vector<std::string>& userUpdates,
		std::vector<std::string>& hasAccessUpdates);
	void _makeNotebookUpdates(std::vector<std::string>& updates, std::fstream& fin, std::fstream& fout);
	void _updateData(const std::string& update, const std::string& item, const std::string& change, std::string& dataLine);
	bool _deleteOldDataNotebooks();
	bool _assignNewToOldDataNotebooks();

public:
	DataManager(bool& functional);
	~DataManager();
	bool populate(std::vector<Notebook*>& notebooks, std::vector<User*>& users);
	bool update(const std::string& dataNotebook, const std::string& item, const std::string& update, 
		              const std::string& change = "NULL");
	bool addNotebook(const std::string& name, const std::string& owner = "NULL");
	bool addUser(const std::string& username, const std::string& password);
	bool addHANotebook(const std::string& notebookName, const std::string& owner);
};

//***************************************************************************************************************************************

inline bool DataManager::_bootUp() { return _checkNotebooks(); }

//***************************************************************************************************************************************

#endif