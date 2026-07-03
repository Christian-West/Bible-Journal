#ifndef NOTEBOOK_H
#define NOTEBOOK_H

//***************************************************************************************************************************************

#include "Utility.h"
#include <string>
#include <cstdio>
#include <fstream>
#include <vector>

//***************************************************************************************************************************************

class Notebook
{
protected:
	static int allIDs;
	int id,
		ownerID,
		_numChars,
		numMoveType;
	std::string name;
	std::FILE* remover;
	std::fstream notebook;
	Utility* utl;
	bool saved,
		notebookExists;
	std::vector<char> editor;
	void _options(const char ch, int& optz);
	bool _save();
	bool _checkCharCount(const char ch);

public:
	Notebook(const std::string& fName, const int owner = -1);
	Notebook(const Notebook& notebook);
	~Notebook();
	int getID() const;
	int getOwner() const;
	std::string getName() const;
	bool setName(const std::string& newName);
	void setOwner(const int id);
	void edit();
	bool del();
	void displayContent(const bool inEditSession = false);
	void display();
	bool hasContent();
	bool exists() const;
	bool operator==(const Notebook& rhs) const;
	bool operator!=(const Notebook& rhs) const;
};

//***************************************************************************************************************************************

inline int Notebook::getID() const { return id; }

//***************************************************************************************************************************************

inline int Notebook::getOwner() const { return ownerID; }

//***************************************************************************************************************************************

inline std::string Notebook::getName() const { return name; }

//***************************************************************************************************************************************

inline bool Notebook::exists() const { return notebookExists; }

//***************************************************************************************************************************************

inline bool Notebook::operator==(const Notebook& rhs) const { return (id == rhs.id); }

//***************************************************************************************************************************************

inline bool Notebook::operator!=(const Notebook& rhs) const { return !(*this == rhs); }

//***************************************************************************************************************************************

#endif