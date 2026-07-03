#include "DataManager.h"
using namespace std;
//***************************************************************************************************************************************

bool DataManager::_findNotebook(string& dataLine, vector<Notebook*>& notebooks, int& index)
{
	bool found = false;
	vector<Notebook*>::iterator notebookIter;

	index = 0;

	for (notebookIter = notebooks.begin(); !found && (notebookIter != notebooks.end());)
	{
		if ((*notebookIter)->getName() == dataLine)
			found = true;
		else
		{
			++notebookIter;
			++index;
		}
	}
	
	return found;
}

//***************************************************************************************************************************************

bool DataManager::_findUser(vector<string>& data, vector<User*>& users, int& id, const int index)
{
	bool found = false;
	vector<User*>::iterator userIter;

	for (userIter = users.begin(); !found && (userIter != users.end()); ++userIter)
	{
		if ((*userIter)->getName() == (data[index]))
		{
			id = (*userIter)->getID();
			found = true;
		}
	}

	return found;
}

//***************************************************************************************************************************************

bool DataManager::_findUser(string& dataLine, vector<User*>& users, int& index)
{
	bool found = false;
	vector<User*>::iterator userIter;

	for (userIter = users.begin(), index = 0; !found && (userIter != users.end());)
	{
		if ((*userIter)->getName() == dataLine)
			found = true;
		else
		{
			++userIter;
			++index;
		}
	}

	return found;
}

//***************************************************************************************************************************************

bool DataManager::_checkNotebooks()
{
	bool success = false;
	
	notebooksNotebook.open("[DATA]Notebooks.csv");
	usersNotebook.open("[DATA]Users.csv");
	hasAccessNotebook.open("[DATA]HasAccess.csv");
	updatesNotebook.open("[DATA]Updates.csv", ios::in);

	if (notebooksNotebook.is_open() && usersNotebook.is_open() && hasAccessNotebook.is_open() && updatesNotebook.is_open())
	{
		updatesNotebook.clear();

		if (updatesNotebook.get())
		{
			if (_copyNotebooks())
				success = true;
		}
		else

			success = true;
	}
	else
	{
		updatesNotebook.close();
		notebooksNotebook.close();
		usersNotebook.close();
		hasAccessNotebook.close();
	}
	
	return success;
}

//***************************************************************************************************************************************

bool DataManager::_copyNotebooks()
{
	bool success = false;
	vector<string> notebookUpdates;
	vector<string> userUpdates;
	vector<string> hasAccessUpdates;

	notebooksNotebook1.open("[DATA]Notebooks(1).csv", ios::out);
	usersNotebook1.open("[DATA]Users(1).csv", ios::out);
	hasAccessNotebook1.open("[DATA]HasAccess(1).csv", ios::out);

	if (notebooksNotebook1.is_open() && usersNotebook1.is_open() && hasAccessNotebook1.is_open())
	{
		notebooksNotebook.clear();
		usersNotebook.clear();
		hasAccessNotebook.clear();
		notebooksNotebook.seekg(0L, ios::beg);
		usersNotebook.seekg(0L, ios::beg);
		hasAccessNotebook.seekg(0L, ios::beg);
		updatesNotebook.seekg(0L, ios::beg);

		_findUniqueUpdates(notebookUpdates, userUpdates, hasAccessUpdates);
		_makeNotebookUpdates(notebookUpdates, notebooksNotebook, notebooksNotebook1);
		_makeNotebookUpdates(userUpdates, usersNotebook, usersNotebook1);
		_makeNotebookUpdates(hasAccessUpdates, hasAccessNotebook, hasAccessNotebook1);

		if (_deleteOldDataNotebooks() && _assignNewToOldDataNotebooks())
			success = true;
	}

	return success;
}

//***************************************************************************************************************************************

void DataManager::_findUniqueUpdates(vector<string>& notebookUpdates, vector<string>& userUpdates, vector<string>& hasAccessUpdates)
{
	string update,
		   change;
	vector<string> uniques;
	vector<string> uniqueUpdates;

	getline(updatesNotebook, update, ';');
	getline(updatesNotebook, change);

	uniques.push_back(update);
	uniqueUpdates.push_back(update + ";" + change);

	while (getline(updatesNotebook, update, ';'))
	{
		getline(updatesNotebook, change);

		/*for (int i = 0; i < uniques.size(); ++i)
		{
			if (uniques[i] == update)
			{
				uniques.erase(uniques.begin() + i);
				uniqueUpdates.erase(uniqueUpdates.begin() + i);
			}
		}*/

		uniques.push_back(update);
		uniqueUpdates.push_back(update + ";" + change);
	}

	while (uniqueUpdates.size() > 0)
	{
		if (uniqueUpdates[0].find("Notebooks,") != string::npos)
			notebookUpdates.push_back(uniqueUpdates[0]);
		else if (uniqueUpdates[0].find("Users,") != string::npos)
			userUpdates.push_back(uniqueUpdates[0]);
		else if (uniqueUpdates[0].find("HasAccess,") != string::npos)
			hasAccessUpdates.push_back(uniqueUpdates[0]);

		uniqueUpdates.erase(uniqueUpdates.begin());
	}

	updatesNotebook.close();
	updatesNotebook.open("[DATA]Updates.csv", ios::out | ios::trunc);
}

//***************************************************************************************************************************************

void DataManager::_makeNotebookUpdates(vector<string>& updates, fstream& fin, fstream& fout)
{
	bool found;
	string item,
		   change,
		dataLine;
	int i;
	size_t pos = 0;
	vector<string> items;
	vector<string> changes;
	vector<string>::iterator iter;

	fin.clear();
	fout.clear();
	fin.seekg(0L, ios::beg);
	fout.seekp(0L, ios::end);

	for (iter = updates.begin(); iter != updates.end(); ++iter)
	{
		item = "";
		change = "";
		i = 0;

		while ((*iter)[i] != ',')
			++i;
		++i;

		item = ";";

		while ((*iter)[i] != ',')
		{
			item += (*iter)[i];
			++i;
		}
		item += ",";
		++i;
		items.push_back(item);

		while ((*iter)[i] != ';')
			++i;
		++i;

		while ((*iter)[i] != ',')
		{
			change += (*iter)[i];
			++i;
		}
		change += ",";
		++i;
		changes.push_back(change);
	}	

	top:
	while (getline(fin, dataLine))
	{
		found = true;
		
		while (found)
		{
			found = false;
			
			for (i = 0; !found && (i < items.size());)
			{
				if (dataLine.find(items[i]) != string::npos)
					found = true;
				else
					++i;
			}

			if (found && (updates.size() > 0))
			{
				if (updates[i].find("CHANGE_NAME;") != string::npos)
					_updateData("CHANGE_NAME", items[i], changes[i], dataLine);
				else if (updates[i].find("CHANGE_PASS;") != string::npos)
					_updateData("CHANGE_PASS", items[i], changes[i], dataLine);
				else if (updates[i].find("ALLOW_ACCESS;") != string::npos)
					_updateData("ALLOW_ACCESS", items[i], changes[i], dataLine);
				else if (updates[i].find("REMOVE;") != string::npos)
					_updateData("REMOVE", items[i], changes[i], dataLine);
				else if (updates[i].find("DELETE;") != string::npos)
					goto top;

				items.erase(items.begin() + i);
				updates.erase(updates.begin() + i);
				changes.erase(changes.begin() + i);
			}
		}

		fout << dataLine << '\n';
	}
}

//***************************************************************************************************************************************

void DataManager::_updateData(const string& update, const string& item, const string& change, string& dataLine)
{
	bool found = false;
	string subString = "";
	size_t startPos = 0;
	int endPos = 0;

	if (update == "CHANGE_NAME")
	{
		startPos = dataLine.find(item);
		endPos = startPos + item.size();

		dataLine.erase(dataLine.begin() + startPos, dataLine.begin() + endPos);
		dataLine.insert(startPos, (";" + change));

	}
	else if (update == "CHANGE_PASS")
	{
		startPos = dataLine.rfind(item);
		endPos = startPos + item.size();

		dataLine.erase(dataLine.begin() + startPos, dataLine.begin() + endPos);
		dataLine.insert(startPos, (";" + change));
	}
	else if (update == "ALLOW_ACCESS")
	{
		subString = dataLine.substr(0, dataLine.size() - 2);
		subString += (change + ";,");
		dataLine = subString;
	}
	else if (update == "REMOVE")
	{
		startPos = dataLine.find(change);
		endPos = startPos + change.size();

		dataLine.erase(dataLine.begin() + startPos, dataLine.begin() + endPos);
	}
}

//***************************************************************************************************************************************

bool DataManager::_deleteOldDataNotebooks()
{
	bool success = true;
	int numSuccess = 0;
	string oldNames[3] = { "[DATA]Notebooks.csv", "[DATA]Users.csv", "[DATA]HasAccess.csv" };

	notebooksNotebook.close();
	usersNotebook.close();
	hasAccessNotebook.close();

	for (int i = 0; i < 3; ++i)
	{
		if ((fopen_s(&remover, oldNames[i].c_str(), "r") == 0) && (fclose(remover) == 0) && (remove(oldNames[i].c_str()) == 0))
			++numSuccess;
		else
			perror("\nError");
	}

	if (numSuccess == 3)
		success = true;

	return success;
}

//***************************************************************************************************************************************

bool DataManager::_assignNewToOldDataNotebooks()
{
	bool success = false;
	int numSuccess = 0;
	string oldNames[3] = { "[DATA]Notebooks.csv", "[DATA]Users.csv", "[DATA]HasAccess.csv" };
	string updated[3] = { "[DATA]Notebooks(1).csv", "[DATA]Users(1).csv", "[DATA]HasAccess(1).csv" };
	
	notebooksNotebook1.close();
	usersNotebook1.close();
	hasAccessNotebook1.close();

	for (int i = 0; i < 3; ++i)
	{
		if (rename(updated[i].c_str(), oldNames[i].c_str()) == 0)
			++numSuccess;
	}

	if (numSuccess == 3)
		success = true;

	return success;
}

//***************************************************************************************************************************************

DataManager::DataManager(bool& functional)
{
	notebooksNotebook.close();
	usersNotebook.close();
	hasAccessNotebook.close();
	notebooksNotebook1.close();
	usersNotebook1.close();
	hasAccessNotebook1.close();
	updatesNotebook.close();
	remover = nullptr;

	if (_bootUp())
	{
		functional = true;
		canFunction = true;
	}
	else
	{
		functional = false;
		canFunction = false;
	}
}

//***************************************************************************************************************************************

DataManager::~DataManager()
{
	notebooksNotebook.close();
	usersNotebook.close();
	hasAccessNotebook.close();
	notebooksNotebook1.close();
	usersNotebook1.close();
	hasAccessNotebook1.close();
	updatesNotebook.close();
	remover = nullptr;
	canFunction = false;
}

//***************************************************************************************************************************************

bool DataManager::populate(vector<Notebook*>& notebooks, 
	                       vector<User*>& users)
{
	if (canFunction)
	{
		Notebook* notebook;
		vector<string> data;
		vector<User*> accessUsers;
		map<Notebook*, vector<User*>> givesAccessMap;
		string dataLine;
		int id = 0,
			userIndex,
			notebookIndex;

		notebooksNotebook.close();
		usersNotebook.close();
		hasAccessNotebook.close();

		notebooksNotebook.open("[DATA]Notebooks.csv", ios::in);
		usersNotebook.open("[DATA]Users.csv", ios::in);
		hasAccessNotebook.open("[DATA]HasAccess.csv", ios::in);

		notebooksNotebook.clear();
		usersNotebook.clear();
		hasAccessNotebook.clear();
		notebooksNotebook.seekg(0L, ios::beg);
		usersNotebook.seekg(0L, ios::beg);
		hasAccessNotebook.seekg(0L, ios::beg);

		notebooks.clear();
		users.clear();

		while (!usersNotebook.eof())
		{        
			data.clear();

			for (int i = 0; i < 2; i++)
			{
				getline(usersNotebook, dataLine, ',');

				if (dataLine[0] == '\n')
					dataLine = dataLine.substr(1);

				data.push_back(dataLine);
			}

			if (data[0].size() > 0)
				data[0] = data[0].substr(1);

			if (data[1].size() > 0)
				data[1] = data[1].substr(1);

			if (data[0] != "")
				users.push_back(new User(data[0], data[1]));
		}		

		while (!notebooksNotebook.eof())
		{
			data.clear();

			for (int i = 0; i < 2; i++)
			{
				getline(notebooksNotebook, dataLine, ',');

				if (dataLine[0] == '\n')
					dataLine = dataLine.substr(1);

				data.push_back(dataLine);
			}

			if (data[0].size() > 0)
				data[0] = data[0].substr(1);

			_findUser(data, users, id, 1);
			_findUser(data[1], users, userIndex);

			if (data[0] != "")
			{
				if (data[1] != "NULL")
				{
					notebook = new(nothrow) Notebook(data[0], id);

					if (userIndex < users.size())
						users[userIndex]->addNotebook(notebook);
				}
				else
					notebook = new(nothrow) Notebook(data[0]);

				notebooks.push_back(notebook);
			}
		}

		while (getline(hasAccessNotebook, dataLine, ','))
		{ 
			data.clear();
			accessUsers.clear();
			givesAccessMap.clear();

			if (dataLine[0] == '\n')
				dataLine = dataLine.substr(1);

			if (dataLine.size() > 0)
				dataLine = dataLine.substr(1); 

			data.push_back(dataLine);
			getline(hasAccessNotebook, dataLine, ',');

			if (dataLine[0] == '\n')
				dataLine = dataLine.substr(1);

			data.push_back(dataLine);
				
			if (_findNotebook(data[0], notebooks, notebookIndex))
			{
				while (getline(hasAccessNotebook, dataLine, ',') && (dataLine != ";"))
				{
					if (dataLine[0] == '\n')
						dataLine = dataLine.substr(1);

					if (_findUser(dataLine, users, userIndex) && (dataLine != ""))
						accessUsers.push_back(users[userIndex]);
				}	

				givesAccessMap.emplace(make_pair(notebooks[notebookIndex], accessUsers));

				if (_findUser(data[1], users, userIndex))
					users[userIndex]->addGives(givesAccessMap);
			}
		}

		notebooksNotebook.close();
		usersNotebook.close();
		hasAccessNotebook.close();

		notebooksNotebook.open("[DATA]Notebooks.csv", ios::out | ios::app);
		usersNotebook.open("[DATA]Users.csv", ios::out | ios::app);
		hasAccessNotebook.open("[DATA]HasAccess.csv", ios::out | ios::app);
	}

	return canFunction;
}

//***************************************************************************************************************************************

bool DataManager::update(const string& dataNotebook, const string& item, const string& update, const string& change)
{
	if (canFunction)
	{
		string newUpdate = "";

		updatesNotebook.clear();
		updatesNotebook.seekp(0L, ios::end);

		newUpdate = (dataNotebook + "," + item + "," + update + ";" + change + ",");

		updatesNotebook << newUpdate << '\n';
	}
	
	return canFunction;
}

//***************************************************************************************************************************************

bool DataManager::addNotebook(const string& name, const string& owner)
{
	if (canFunction)
	{
		string newUpdate = "";

		notebooksNotebook.clear();
		notebooksNotebook.seekp(0L, ios::end);

		newUpdate = (";" + name + "," + owner + ",");
		
		notebooksNotebook << newUpdate << '\n';
	}

	return canFunction;
}

//***************************************************************************************************************************************

bool DataManager::addUser(const string& username, const string& password)
{
	if (canFunction)
	{
		string newUpdate = "";

		usersNotebook.clear();
		usersNotebook.seekp(0L, ios::end);

		newUpdate = (";" + username + ",;" + password + ",");

		usersNotebook << newUpdate << '\n';
	}

	return canFunction;
}

//***************************************************************************************************************************************

bool DataManager::addHANotebook(const string& notebookName, const string& owner)
{
	if (canFunction)
	{
		string newUpdate = "";
		bool found = false;

		hasAccessNotebook.clear();
		hasAccessNotebook.seekp(0L, ios::beg);

		while (!found && (getline(hasAccessNotebook, newUpdate)))
		{
			if (newUpdate.find(notebookName) != string::npos)
				found = true;
		}

		if (!found)
		{
			hasAccessNotebook.clear();
			hasAccessNotebook.seekp(0L, ios::end);

			newUpdate = (";" + notebookName + "," + owner + ",;,");

			hasAccessNotebook << newUpdate << '\n';
		}
	}

	return canFunction;
}