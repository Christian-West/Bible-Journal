#include "User.h"

//***************************************************************************************************************************************

#include <new>
#include <iostream>
using namespace std;

//***************************************************************************************************************************************

int User::allIDs = 0;

//***************************************************************************************************************************************

void User::_assign(map<Notebook*, vector<User*>>& toFill, map<Notebook*, vector<User*>>& original)
{
	toFill.clear();

	for (mapIter = original.begin(); mapIter != original.end(); ++mapIter)
		toFill.insert(make_pair(mapIter->first, mapIter->second));
}

//***************************************************************************************************************************************

User::User() 
{ 
	++allIDs; 
	id = allIDs; 
	numNotebooks = 0;
	username = ""; 
	password = "";
	userExists = true;
}

//***************************************************************************************************************************************

User::User(const string& name, const string& pass, map<Notebook*, vector<User*>>& gives, const vector<Notebook*>& has)
{
	++allIDs;
	id = allIDs;
	numNotebooks = 0;
	username = name;
	password = pass;
	userExists = true;
	_assign(givesAccess, gives);
	hasAccess.assign(has.begin(), has.end());
}

//***************************************************************************************************************************************

User::User(const string& name, const string& pass)
{
	++allIDs;
	id = allIDs;
	numNotebooks = 0;
	username = name;
	password = pass;
	userExists = true;
}

//***************************************************************************************************************************************

User::User(User& user) 
{
	id = user.id;
	numNotebooks = user.numNotebooks;
	username = user.username;
	password = user.password;
	userExists = user.userExists;
	_assign(givesAccess, user.givesAccess);
	hasAccess.assign(user.hasAccess.begin(), user.hasAccess.end());
	notebooks.assign(user.notebooks.begin(), user.notebooks.end());
}

//***************************************************************************************************************************************

void User::getGives(map<Notebook*, vector<User*>>& gives)
{
	if (userExists)
		_assign(gives, givesAccess);
	else
		cout << "\nError: Usewr does not exist\n";
}

//***************************************************************************************************************************************

bool User::findNotebook(const Notebook* l, int& index)
{
	bool found = false;
	int i = 0;

	if (userExists)
	{
		for (vectIter = notebooks.begin(); !found && vectIter != notebooks.end(); ++vectIter, ++i)
		{
			if (*l == **vectIter)
			{
				index = i;
				found = true;
			}
		}
	}
	else
		cout << "\nError: User does not exist\n";

	return found;
}

//***************************************************************************************************************************************

void User::addNotebook(vector<Notebook*>& l)
{
	if (userExists)
	{
		for (vectIter = l.begin(); vectIter != l.end(); ++vectIter)
		{
			notebooks.push_back(*vectIter);
			++numNotebooks;
		}
	}
	else
		cout << "\nError: User does not exist\n";
}

//***************************************************************************************************************************************

void User::addGives(map<Notebook*, vector<User*>>& gives)
{
	map<Notebook*, vector<User*>>::iterator givesIter;
	vector<User*>::iterator givesVectIter;

	if (userExists)
	{
		for (givesIter = gives.begin(); givesIter != gives.end(); ++givesIter)
		{
			for (givesVectIter = (givesIter->second).begin(); givesVectIter != (givesIter->second).end(); ++givesVectIter)
			{
				givesAccess[givesIter->first].push_back(*givesVectIter);
				(*givesVectIter)->addHas(givesIter->first);
			}
		}
	}
	else
		cout << "\nError: User does not exist\n";
}

//***************************************************************************************************************************************

void User::addHas(vector<Notebook*>& has)
{
	if (userExists)
	{
		for (vectIter = has.begin(); vectIter != has.end(); ++vectIter)
			hasAccess.push_back(*vectIter);
	}
	else
		cout << "\nError: User does not exist\n";
}

//***************************************************************************************************************************************

void User::display()
{
	if (userExists)
	{
		cout << "\nUser ID: " << getID() << "\n"
			<< "\tUsername: " << getName() << '\n'
			<< "\tPassword: " << getPass() << '\n'
			<< "\tNotebook(s):\n";

		for (vectIter = notebooks.begin(); vectIter != notebooks.end(); ++vectIter)
			cout << "\t\t" << (*vectIter)->getName() << " (notebookID: " << (*vectIter)->getID() << ") \n";

		cout << "\tGives Access to: \n";

		for (mapIter = givesAccess.begin(); mapIter != givesAccess.end(); ++mapIter)
		{
			cout << "\t\t" << (mapIter->first)->getName() << " (notebookID: " << (mapIter->first)->getID() << ")\n";

			for (vector<User*>::iterator vectIter = (mapIter->second).begin(); vectIter != (mapIter->second).end(); ++vectIter)
				cout << "\t\t\t" << (*vectIter)->getName() << " (userID: " << (*vectIter)->getID() << ") \n";		 
		}

		cout << "\tAllowed Access to: \n";

		for (vectIter = hasAccess.begin(); vectIter != hasAccess.end(); ++vectIter)
		{
			cout << "\t\tuserID: " << (*vectIter)->getOwner()  << '\n'
				 << "\t\t\t" << (*vectIter)->getName() << " (notebookID: " << (*vectIter)->getID() << ") \n";
		}
	}
	else
		cout << "\nError: User does not exist\n";
}

//***************************************************************************************************************************************

void User::clear() 
{ 
	id = 0;
	numNotebooks = 0;
	username = "";
	password = "";
	givesAccess.clear();
	hasAccess.clear();
	notebooks.clear();
}

//***************************************************************************************************************************************

void User::del()
{
	clear();
	userExists = false;
}

//***************************************************************************************************************************************

void User::setName(const string& name) 
{ 
	if (userExists)
		username = name; 
	else
		cout << "\nError: User does not exist\n";
}

//***************************************************************************************************************************************

void User::setPass(const string& pass) 
{ 
	if (userExists)
		password = pass;
	else
		cout << "\nError: User does not exist\n";
}

//***************************************************************************************************************************************

void User::addNotebook(Notebook* notebook) 
{ 
	if (userExists)
	{
		notebooks.push_back(notebook);
		++numNotebooks;
	}
	else
		cout << "\nError: User does not exist\n";
}

//***************************************************************************************************************************************

void User::addGives(Notebook* notebook, User* user) 
{ 
	if (userExists)
	{
		givesAccess[notebook].push_back(user);
		user->addHas(notebook);
	}

	else
		cout << "\nError: User does not exist\n";
}

//***************************************************************************************************************************************

void User::addHas(Notebook* has) 
{ 
	if (userExists)
		hasAccess.push_back(has); 
	else
		cout << "\nError: User does not exist\n";
}

//***************************************************************************************************************************************

bool User::removeNotebook(Notebook* l)
{
	bool success = false;
	int index;

	if (userExists)
	{
		if (findNotebook(l, index))
		{
			notebooks.erase(notebooks.begin() + index);
			--numNotebooks;
			success = true;
		}
	}
	else
		cout << "\nError: User does not exist\n";


	return success;
}

//***************************************************************************************************************************************

bool User::removeNotebook(vector<Notebook*>& l)
{
	bool success = false;
	int index,
		numRemoved = 0;

	if (userExists)
	{
		for (int i = 0; i < l.size(); ++i)
		{
			if (findNotebook(l[i], index))
			{
				notebooks.erase(notebooks.begin() + index);
				--numNotebooks;
				++numRemoved;
			}
		}

		if (numRemoved == l.size())
			success = true;
	}
	else
		cout << "\nError: User does not exist\n";


	return success;
}

//***************************************************************************************************************************************

bool User::removeGives(Notebook* notebook, User* user)
{
	bool found = false;
	int index = 0;

	if (userExists)
	{ 
		if (givesAccess.count(notebook) != 0)
		{	
			mapIter = givesAccess.find(notebook);
			
			for (int i = 0; !found && (i < (mapIter->second).size()); ++i)
			{
				if (*user == (mapIter->second)[i])
				{
					(mapIter->second).erase((mapIter->second).begin() + i);
					user->removeHas(notebook);
					found = true;
				}
			}
		}
	}
	else
		cout << "\nError: User does not exist\n";

	return found;
}

//***************************************************************************************************************************************

bool User::removeGives(map<Notebook*, vector<User*>>& gives)
{
	bool success = false,
		 found;
	int index,
		numFound = 0,
		notebooks = 0,
	    totalUsers = 0;
	map<Notebook*, vector<User*>>::iterator givesIter;
	map<Notebook*, vector<User*>>::iterator givesAccessIter;
	vector<User*>::iterator givesVectIter;

	if (userExists)
	{
		for (givesIter = gives.begin(); givesIter != gives.end(); ++givesIter)
		{
			++notebooks;

			if (givesAccess.count(givesIter->first) != 0)
			{
				givesAccessIter = givesAccess.find(givesIter->first);

				for (givesVectIter = (givesIter->second).begin(); givesVectIter != (givesIter->second).end(); ++givesVectIter)
				{
					found = false;
					index = 0;
					++totalUsers;
					for (int i = 0; !found && (i < (givesAccessIter->second).size()); ++i)
					{
						if (**givesVectIter == (givesAccessIter->second)[i])
						{
							(givesAccessIter->second).erase((givesAccessIter->second).begin() + i);
							(*givesVectIter)->removeHas(givesIter->first);
							found = true;
							++numFound;
						}
					}
				}
			}
		}

		if (notebooks == gives.size() && (totalUsers == numFound))
			success = true;
	}
	else
		cout << "\nError: User does not exist\n";

	return success;
}

//***************************************************************************************************************************************

bool User::removeGivesNotebook(Notebook* notebook)
{
	bool success = false;

	if (userExists)
	{
		if (givesAccess.count(notebook) != 0)
		{
			mapIter = givesAccess.find(notebook);
		
			for (vector<User*>::iterator vectIter = (mapIter->second).begin(); vectIter != (mapIter->second.end()); ++vectIter)
				(*vectIter)->removeHas(mapIter->first);

			givesAccess.erase(notebook);
				
			success = true;
		}

	}
	else
		cout << "\nError: User does not exist\n";

	return success;
}

//***************************************************************************************************************************************

bool User::removeHas(Notebook* notebook)
{
	bool found = false;

	if (userExists)
	{
		for (int i = 0; !found && (i < hasAccess.size()); ++i)
		{
			if (notebook == (hasAccess[i]))
			{
				hasAccess.erase(hasAccess.begin() + i);
				found = true;
			}
		}
	}
	else
		cout << "\nError: User does not exist\n";

	return found;
}

//***************************************************************************************************************************************

bool User::removeHas(vector<Notebook*>& has)
{
	bool success = false,
		 found = false;
	int	numRemoved = 0;

	if (userExists)
	{
		for (vector<Notebook*>::iterator outerIter = has.begin(); outerIter != has.end(); ++outerIter)
		{
			for (int i = 0; !found && (i < hasAccess.size()); ++i)
			{
				if (**outerIter == *(hasAccess[i]))
				{
					hasAccess.erase(hasAccess.begin() + i);
					found = true;
					++numRemoved;
				}
			}	
		}
	}
	else
		cout << "\nError: User does not exist\n";

	if (numRemoved == has.size())
		success = true;

	return success;
}