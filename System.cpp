#include "System.h"
#include <iostream>
#include <iomanip>
using namespace std;

//***************************************************************************************************************************************

bool System::_findNotebook(const int id, int& index)
{
	bool found = false;

	for (int i = 0; !found && (i < numNotebooks); ++i)
	{
		if (id == notebooks[i]->getID())
		{
			index = i;
			found = true;
		}
	}

	return found;
}

//***************************************************************************************************************************************

bool System::_findUser(const int id, int& index)
{
	bool found = false;

	for (int i = 0; !found && (i < numUsers); ++i)
	{
		if (id == users[i]->getID())
		{
			index = i;
			found = true;
		}
	}

	return found;
}

//***************************************************************************************************************************************

bool System::_itemExists(const string& name)
{
	bool found = false;

	if (name.find(".txt") == string::npos)
	{
		for (int i = 0; !found && (i < numUsers); ++i)
		{
			if (name == users[i]->getName())
				found = true;
		}
	}
	else
	{
		for (int i = 0; !found && (i < numNotebooks); ++i)
		{
			if (name == notebooks[i]->getName())
				found = true;
		}
	}

	return found;
}

//***************************************************************************************************************************************

bool System::displayUserNotebooks()
{
	bool success = false;
	int numNotebooks = 0;
	vector<Notebook*> notebooks;

	if (canFunction)
	{
		cout << "\nNOTEBOOKS\n";
		user->getNotebooks(notebooks);

		if (notebooks.size() > 0)
		{
			for (int i = 0; i < notebooks.size(); ++i)
				cout << "\tID: " << notebooks[i]->getID() << ") " << notebooks[i]->getName() << '\n';

			success = true;
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return success;
}

//***************************************************************************************************************************************

bool System::displayUserHasAccess()
{
	bool success = false;
	vector<Notebook*> notebooks;

	if (canFunction)
	{
		cout << "\nAllowed Access to:\n";
		user->getHas(notebooks);

		if (notebooks.size() > 0)
		{
			for (int i = 0; i < notebooks.size(); ++i)
				cout << "\tID: " << notebooks[i]->getID() << ") " << notebooks[i]->getName() << '\n';

			success = true;
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return success;
}

//***************************************************************************************************************************************

bool System::displayUserGivesAccess(const int notebookID)
{
	bool success = false;
	int index;
	map<Notebook*, vector<User*>> gives;
	map<Notebook*, vector<User*>>::iterator givesIter;

	if (canFunction)
	{
		if (_findNotebook(notebookID, index))
		{
			cout << "\nUsers Allowed Access to " << notebooks[index]->getName() << ":\n";
			user->getGives(gives);

			givesIter = gives.find(notebooks[index]);

			if (givesIter != gives.end())
			{
				for (vector<User*>::iterator userVect = givesIter->second.begin(); userVect != givesIter->second.end(); ++userVect)
					cout << "\tID: " << (*userVect)->getID() << ") " << (*userVect)->getName() << '\n';

				success = true;
			}
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return success;
}

//***************************************************************************************************************************************

bool System::_findPassword(int index, const string& password)
{
	bool found = false;

	if (users[index]->getPass() == password)
		found = true;

	return found;
}

//***************************************************************************************************************************************

System::System(bool& functional)
{
	canFunction = false;

	dm = new(std::nothrow) DataManager(canFunction);

	if (canFunction)
	{
		dm->populate(notebooks, users);
		numNotebooks = notebooks.size();
		numUsers = users.size();
		functional = true;
	}
	else
	{
		cout << "\nError: data wasn't loaded properly, system cannot function\n";
		numNotebooks = 0;
		numUsers = 0;
		functional = false;
	}

	user = nullptr;
	currNotebook = nullptr;
	utl = new Utility();
}

//***************************************************************************************************************************************

System::~System()
{
	canFunction = false;
	numNotebooks = 0;
	numUsers = 0;
	delete user;
	delete currNotebook;
	delete utl;
	user = nullptr;
	currNotebook = nullptr;
	utl = nullptr;
	notebooks.clear();
	users.clear();
}

//***************************************************************************************************************************************

bool System::createAccount(const std::string& username, const std::string& password)
{
	bool success = true;

	if (canFunction)
	{
		if (_itemExists(username))
			success = false;
		else
		{
			user = new User(username, password);

			if (user)
			{
				users.push_back(user);
				dm->addUser(username, password);
				++numUsers;
			}
			else
			{
				success = false;
				cout << "\nError: not enough memory\n";
			}
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return success;
}

//***************************************************************************************************************************************

bool System::login(const string& username, const string& password)
{
	bool found = false;
	int index = 0;

	if (canFunction)
	{
		for (int i = 0; !found && (i < users.size()); ++i)
		{
			if (users[i]->getName() == username)
			{
				index = i;
				found = true;	
			}
		}

		if (found)
		{
			if (_findPassword(index, password))
				user = users[index];
			else
			{
				cout << "\nUsername found, password was not found";
				found = false;
			}
		}
		else
			cout << "\nUsername was not found";
	}
	else
		cout << "\nError: system cannot function\n";

	return found;
}

//***************************************************************************************************************************************

bool System::viewAllNotebooks()
{
	bool success = false;
	vector<Notebook*> hasAccessNotebooks;

	if (canFunction)
	{
		utl->moveScreen(40);

		user->getHas(hasAccessNotebooks);

		if ((user->getNumNotebooks() > 0) || (hasAccessNotebooks.size() > 0))
		{
			displayUserNotebooks();
			displayUserHasAccess();
			
			success = true;

			cout << "\nWhich notebook would you like?"
				<< "\n\tEnter Notebook ID: ";
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return success;
}

//***************************************************************************************************************************************

bool System::displayAllUsers()
{
	bool success = false;

	if (canFunction)
	{
		utl->moveScreen(40);

		if (users.size() > 0)
		{
			for (int i = 0; i < users.size(); ++i)
				cout << "\tID: " << users[i]->getID() << ") " << users[i]->getName() << '\n';

			success = true;

			cout << "\nWhich users would you like to give access to?" 
				<< "\n\tEnter User IDs ";
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return success;
}

//***************************************************************************************************************************************

bool System::viewNotebook(const int notebookID)
{
	bool found = false;
	int index;

	if (canFunction)
	{
		if (_findNotebook(notebookID, index))
		{
			utl->moveScreen(45);
			notebooks[index]->displayContent();
			found = true;
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return found;
}

//***************************************************************************************************************************************

bool System::createNotebook(const string& name)
{
	bool success = false;

	if (canFunction)
	{
		if (!_itemExists(name))
		{
			currNotebook = new(nothrow) Notebook(name, user->getID());

			if (currNotebook)
			{
				user->addNotebook(currNotebook);
				dm->addNotebook(name, user->getName());
				notebooks.push_back(currNotebook);
				notebooks.push_back(currNotebook);

				++numNotebooks;
				success = true;
			}
			else
				cout << "\nError: not enough memory\n";
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return true;
}

//***************************************************************************************************************************************

bool System::editNotebook(const int id)
{
	bool success = false;
	int index;

	if (canFunction)
	{
		if (_findNotebook(id, index))
		{
			success = true;
			notebooks[index]->edit();
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return success;
}

//***************************************************************************************************************************************

bool System::givePermission(const int notebookID, vector<int> userIDs)
{
	bool success = false;
	int userIndex,
		notebookIndex,
		numIDs = userIDs.size();
	vector<User*> userVect;
	map<Notebook*, vector<User*>> gives;

	if (canFunction)
	{
		if (_findNotebook(notebookID, notebookIndex))
		{
			success = true;

			for (int i = 0; success && (i < numIDs); ++i)
			{
				if (_findUser(userIDs[i], userIndex))
					userVect.push_back(users[userIndex]);
				else
					success = false;
			}

			if (success)
			{
				gives[notebooks[notebookIndex]] = userVect;
				user->addGives(gives);
				
				for (int i = 0; i < numIDs; ++i)
					dm->update("HasAccess", notebooks[notebookIndex]->getName(), "ALLOW_ACCESS", userVect[i]->getName());

				dm->addHANotebook(notebooks[notebookIndex]->getName(), user->getName());
			}
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return success;
}

//***************************************************************************************************************************************

bool System::removePermission(const int notebookID, vector<int> userIDs)
{
	bool success = false;
	int userIndex,
		notebookIndex,
		numIDs = userIDs.size();
	vector<User*> userVect;
	map<Notebook*, vector<User*>> gives;

	if (canFunction)
	{
		if (_findNotebook(notebookID, notebookIndex))
		{
			success = true;

			for (int i = 0; success && (i < numIDs); ++i)
			{
				if (_findUser(userIDs[i], userIndex))
					userVect.push_back(users[userIndex]);
				else
					success = false;
			}

			if (success)
			{
				gives[notebooks[notebookIndex]] = userVect;
				user->removeGives(gives);

				for (int i = 0; i < numIDs; ++i)
					dm->update("HasAccess", notebooks[notebookIndex]->getName(), "REMOVE", userVect[i]->getName());
			}
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return success;
}

//***************************************************************************************************************************************

bool System::deleteNotebook(const int notebookID)
{
	bool success = false;
	int index;

	if (canFunction)
	{
		if (_findNotebook(notebookID, index) && notebooks[index]->exists())
		{
			user->removeNotebook(notebooks[index]);
			notebooks[index]->del();
			notebooks.erase(notebooks.begin() + index);
			dm->update("Notebooks", notebooks[index]->getName(), "DELETE");
			dm->update("HasAccess", notebooks[index]->getName(), "DELETE");
			--numNotebooks;
			success = true;
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return success;
}

//***************************************************************************************************************************************

void System::deleteAccount()
{
	vector<Notebook*> notebooks;


	user->getHas(notebooks);

	for (int i = 0; i < notebooks.size(); ++i)
		dm->update("HasAccess", notebooks[i]->getName(), "REMOVE", user->getName());

	notebooks.clear();

	user->getNotebooks(notebooks);

	for (int i = 0; i < notebooks.size(); ++i)
		dm->update("Notebooks", user->getName(), "CHANGE");

	notebooks.clear();

	dm->update("Users", user->getName(), "DELETE");
	user->del();
	--numUsers;
}