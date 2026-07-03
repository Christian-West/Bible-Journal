#include "Notebook.h"
#include <iostream>
#include <conio.h>
#include <iomanip>
using namespace std;

//***************************************************************************************************************************************

int Notebook::allIDs = 0;

//***************************************************************************************************************************************

void Notebook::_options(const char ch, int& opt)
{
    string line;
    bool exit = false;
    char confirm = 'n';

    line.assign(80, '*');

    do
    {
        utl->moveScreen(45);

        cout << right << setw(98) << line << '\n'
            << right << setw(numMoveType) << " OPTIONS" << '\n'
            << right << setw(98) << line << "\n\n";

        //cout << right << setw(19) << '|' << right << setw(47) << "Add Today's Date" << right << setw(32) << '|' << '\n';

        //cout << '\n' << right << setw(98) << line << '\n';

        cout << "1. Type \"<<>>" << ch << "\"\n"
            << "2. Exit OPTIONS\n"
            << "3. Save and Exit Editing\n"        
            << "4. Delete Current Notebook \"" << name << "\"";
        utl->moveScreen(13);

        cout << "\n\tChoice: ";

        cin >> opt;

        cout << "\nYou chose option " << opt
            << "\n\tIs this correct? (Y/N): ";
        cin >> confirm;
    } while (confirm == 'n' || confirm == 'N');
}

//***************************************************************************************************************************************

bool Notebook::_save()
{
    std::vector<char>::iterator iter;
    bool success = false;
    string line;

    if (notebookExists)
    {
        for (iter = editor.begin(); iter != editor.end(); ++iter)
            notebook << *iter;

       /* notebook.clear();
        notebook.seekg(0L, ios::beg);

        while (getline(notebook, line))   in case I want to work on the typing system
            ++numLines;

        notebook.clear();
        notebook.seekg(0L, ios::end);*/

        success = true;
    }
    else
        perror("\nError");
    
    return success;
}

//***************************************************************************************************************************************

bool Notebook::_checkCharCount(const char ch)
{
    editor.push_back(ch);
    
    return (++_numChars == 120);
}

//***************************************************************************************************************************************

Notebook::Notebook(const string& fName, const int owner)
{
    ++allIDs;
    id = allIDs;
    ownerID = owner;
    _numChars = 0;
    numMoveType = 0;
    //numLines = 0;
    saved = false;
    remover = nullptr;
    notebookExists = false;
    name = fName;
    utl = new Utility();
    notebook.open(name, ios::in | ios::out | ios::app);

    if (notebook.is_open())
        notebookExists = true;
    else
        perror("\nError");
}

//***************************************************************************************************************************************

Notebook::Notebook(const Notebook& notebook)
{
    id = notebook.id;
    ownerID = notebook.ownerID;
    _numChars = notebook._numChars;
    numMoveType = notebook.numMoveType;
    //numLines = notebook.numLines;
    utl = notebook.utl;
    saved = notebook.saved;
    remover = notebook.remover;
    notebookExists = notebook.notebookExists;
    name = notebook.name;
}

//***************************************************************************************************************************************

Notebook::~Notebook()
{
   
    notebook.close();
    id = -1;
    ownerID = -1;
    _numChars = 0;
    numMoveType = 0;
    editor.clear();
    //numLines = 0;
    delete remover;
    remover = nullptr;
    delete utl;
    utl = nullptr;
	name = "";
    notebookExists = false;
}

//***************************************************************************************************************************************

bool Notebook::setName(const string& newName)
{
    bool success = false;

    if (notebookExists)
    {
        notebook.close();

        if (rename(name.c_str(), newName.c_str()) == 0)
        {
            name = newName;
            success = true;
        }

        notebook.open(name, ios::in | ios::out | ios::app);
    }
    else
        perror("\nError");

    return success;
}

//***************************************************************************************************************************************

void Notebook::setOwner(const int id)
{
    if (notebookExists)
        ownerID = id;
}

//***************************************************************************************************************************************

void Notebook::edit()
{
    char ch = '-';
    int option = 0;
    bool deleteNotebook = false;
    //int i = 0;

    if (notebookExists)
    {
        utl->moveScreen(45);
        displayContent();

        saved = false;

        cout << "\nPLEASE READ: Type \"<<>>o\" (no quotation marks) for options\n";

        while (true)
        {
            if (_kbhit())
            {
                ch = _getch();

                if (ch == '\r')
                {
                    cout << '\n'; //<< ++i << "\t";
                    editor.push_back(ch);
                    _numChars = 0;
                }
                else if (ch == '\b')
                {
                    cout << "\b \b";

                    if (editor.size() != 0)
                        editor.pop_back();
                }
                else if (editor.size() >= 4 &&
                    *(editor.end() - 1) == '>' &&
                    *(editor.end() - 2) == '>' &&
                    *(editor.end() - 3) == '<' &&
                    *(editor.end() - 4) == '<' &&
                    (ch == 'o'))
                {
                    _options(ch, option);

                    if (option == 3)
                        break;                             
                    else if (option == 1)
                    {
                        utl->moveScreen(45);
                        displayContent(true);
                    
                        cout << ch;

                        if (_checkCharCount(ch))
                        {
                            cout << '\n';
                            _numChars = 0;
                        }
                    }
                    else if (option == 2)
                    {
                        for (int i = 0; i < 4; ++i)
                            editor.pop_back();

                        utl->moveScreen(45);
                        displayContent();
                    }
                    else if (option == 4)
                    {
                        deleteNotebook = true;
                        goto deleteNotebook;
                    }
                }
                else
                {
                    cout << ch;

                    if (_checkCharCount(ch))
                    {
                        cout << '\n';
                        _numChars = 0;
                    }
                }
            }
        }

        for (int i = 0; i < 4; ++i)
            editor.pop_back();

        if (_save())
        {
            saved = true;
            editor.clear();
        }

    deleteNotebook: 
        if (deleteNotebook)
            del();
    }
    else
        perror("\nError");
}

//***************************************************************************************************************************************
    
bool Notebook::del()
{
    bool success = false;

    if (notebookExists)
    {
        notebook.close();

        if (fopen_s(&remover, name.c_str(), "r") == 0)
        {
            if (fclose(remover) == 0)
            {
                if (remove(name.c_str()) == 0)
                {
                    success = true;
                    saved = false;
                    remover = nullptr;
                    notebookExists = false;
                    name = "";
                    editor.clear();
                }
            }
            else
                perror("\nError");
        }
        else
            perror("\nError");
    }
    else
        perror("\nError");

    return success;
}

//***************************************************************************************************************************************

bool Notebook::hasContent()
{
    bool isContent = false;
    string line;

    if (notebookExists)
    {
        notebook.clear();
        notebook.seekg(0L, ios::beg);

        getline(notebook, line);

        if (line != "")
            isContent = true;

        notebook.clear();
        notebook.seekg(0L, ios::end);
    }
    else
        perror("\nError");

    return isContent;
}

//***************************************************************************************************************************************

void Notebook::displayContent(const bool inEditSession)
{
    string line;
    std::vector<char>::iterator iter;

    
    if (notebookExists)
    {
        if (hasContent())
        {
            notebook.clear();
            notebook.seekg(0L, ios::beg);

            while (getline(notebook, line))
                cout << line << '\n';
            /*to work on typing system:
            *   use numLines - 1 in a for loop to output the notebook execpt for the last line
            *   output each character of the last line using notebook.get([char])
            */
               
            notebook.clear();
            notebook.seekp(0L, ios::end);
        }
        else
        {
            if (!inEditSession)
                cout << "\nEmpty Notebook\n";
        }

        for (iter = editor.begin(); iter != editor.end(); ++iter)
            cout << *iter;
    }
    else
        perror("\nError");
}

//***************************************************************************************************************************************

void Notebook::display()
{
    string answer = "No"; 

    if (hasContent())
        answer = "Yes";



    cout << "\nNotebook ID: " << getID() << "\n"
        << "\tName: " << getName() << '\n'
        << "\tOwnerID: " << getOwner() << '\n'
        << "\tHas Content: " << answer << '\n';
}