#ifndef USERS_H
#define USERS_H

//***************************************************************************************************************************************

#include "Notebook.h"
#include <map>

//***************************************************************************************************************************************

class User
{
private:
	static int allIDs;
	int id,
		numNotebooks;
	bool userExists;
	std::string username,
		        password;
	std::map<Notebook*, std::vector<User*>> givesAccess;
	std::vector<Notebook*> hasAccess;
	std::vector<Notebook*> notebooks;
	std::vector<Notebook*>::iterator vectIter;
	std::map<Notebook*, std::vector<User*>>::iterator mapIter;
	void _assign(std::map<Notebook*, std::vector<User*>>& toFill, std::map<Notebook*, std::vector<User*>>& original);

public:
	User();
	User(const std::string& name, const std::string& pass, std::map<Notebook*, std::vector<User*>>& gives, const std::vector<Notebook*>& has);
	User(const std::string& name, const std::string& pass);
	User(User& user);
	int getID() const;
	std::string getName() const;
	std::string getPass() const;
	void getGives(std::map<Notebook*, std::vector<User*>>& gives);
	void getHas(std::vector<Notebook*>& has) const;
	bool findNotebook(const Notebook* l, int& index);
	void setName(const std::string& name);
	void setPass(const std::string& pass);
	void addNotebook(Notebook* notebook);
	void addNotebook(std::vector<Notebook*>& l);
	void addGives(Notebook* notebook, User* user);
	void addGives(std::map<Notebook*, std::vector<User*>>& gives);
	void addHas(Notebook* notebook);
	void addHas(std::vector<Notebook*>& has);
	bool removeNotebook(Notebook* l);
	bool removeNotebook(std::vector<Notebook*>& l);
	bool removeGives(Notebook* notebook, User* user);
	bool removeGives(std::map<Notebook*, std::vector<User*>>& gives);
	bool removeGivesNotebook(Notebook* notebook);
	bool removeHas(Notebook* notebook);
	bool removeHas(std::vector<Notebook*>& has);
	void getNotebooks(std::vector<Notebook*>& l);
	int getNumNotebooks() const;
	bool exists() const;
	void display();
	void clear();
	void del();
	bool operator==(const User* rhs) const;
	bool operator!=(const User* rhs) const;
};

//***************************************************************************************************************************************

inline int User::getID() const { return id; }

//***************************************************************************************************************************************

inline std::string User::getName() const { return username; }

//***************************************************************************************************************************************

inline std::string User::getPass() const { return password; }

//***************************************************************************************************************************************

inline void User::getHas(std::vector<Notebook*>& has) const { has.assign(hasAccess.begin(), hasAccess.end()); }

//***************************************************************************************************************************************

inline void User::getNotebooks(std::vector<Notebook*>& l) { l.assign(notebooks.begin(), notebooks.end()); }

//***************************************************************************************************************************************

inline int User::getNumNotebooks() const { return numNotebooks; }

//***************************************************************************************************************************************

inline bool User::exists() const { return userExists; }

//***************************************************************************************************************************************

inline bool User::operator==(const User* rhs) const { return (id == rhs->id); }

//***************************************************************************************************************************************

inline bool User::operator!=(const User* rhs) const { return !(*this == rhs); }

//***************************************************************************************************************************************

#endif