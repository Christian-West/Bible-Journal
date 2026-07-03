#include "Controller.h"
#include <iostream>
#include <iomanip>
using namespace std;

//***************************************************************************************************************************************

Controller::Controller()
{
	canFunction = false;
	system = new System(canFunction);
	utl = new Utility();
	charInput = new char[50];
	option = 0;
	retry = true;
}

//***************************************************************************************************************************************

Controller::~Controller()
{
	delete system;
	delete utl;
	delete[] charInput;
	system = nullptr;
	utl = nullptr;
	charInput = nullptr;

	option = 0;
	strInput = "";
	retry = false;
	canFunction = false;
}

//***************************************************************************************************************************************

void Controller::_loginScreen()
{
	utl->moveScreen(25);

	cout << right << setw(14) << "Welcome to\n";

	/*
	Fonts to Use
		Alligator
		Alligator 2
		ASCII 12
		Big ASCII 12
		Block
		Small Braille
		Broadway
		Coinstak
		Digital
		Doh (maybe)
		Emboss
		Emboss 2
		LCD
		Letter
		Marquee
		Roman
		Thick
		Thorned
		Tiles
		Univers
		USA Flag
		Whimsy
	*/

	cout <<  // Marquee font

		"       .:            .:: .::            .::::         .::     .::     .:::     .::      .:::::\n" <<
		"     .: ::          .:    .::        .::    .::       .::     .::     .: .::   .::     .::   .::\n" <<
		"    .:  .::         .:     .::     .::        .::     .::     .::     .:: .::  .::     .::    .::\n" <<
		"   .::   .::        .::: .:        .::        .::     .::     .::     .::  .:: .::     .::    .::\n" <<
		"  .:::::: .::       .:     .::     .::        .::     .::     .::     .::   .: .::     .::    .::\n" <<
		" .::       .::      .:      .:       .::     .::      .::     .::     .::    .: ::     .::   .::\n" <<
		".::         .::     .:::: .::          .::::            .:::::        .::      .::     .:::::\n";

	cout << "Bible Lesson Planning, Notetaking, and Journaling\n";

	utl->moveScreen(7);
}

//***************************************************************************************************************************************

void Controller::menu()
{
	utl->moveScreen(40);

	cout <<  // Marquee font

	    right << setw(105) << "       .:            .:: .::            .::::         .::     .::     .:::     .::      .:::::\n" <<
		right << setw(107) << "     .: ::          .:    .::        .::    .::       .::     .::     .: .::   .::     .::   .::\n" <<
		right << setw(108) << "    .:  .::         .:     .::     .::        .::     .::     .::     .:: .::  .::     .::    .::\n" <<
		right << setw(108) << "   .::   .::        .::: .:        .::        .::     .::     .::     .::  .:: .::     .::    .::\n" <<
		right << setw(108) << "  .:::::: .::       .:     .::     .::        .::     .::     .::     .::   .: .::     .::    .::\n" <<
		right << setw(107) << " .::       .::      .:      .:       .::     .::      .::     .::     .::    .: ::     .::   .::\n" <<
		right << setw(105) << ".::         .::     .:::: .::          .::::            .:::::        .::      .::     .:::::\n\n";

	cout << right << setw(62) << "+-------+\n" <<  // Digital font
		    right << setw(62) << "|M E N U|\n" <<
		    right << setw(63) << "+-------+\n\n";

	cout << "1.  View Notebook"
		<< "\n2.  Add Notebook"
		<< "\n3.  Edit Notebook"
		<< "\n4.  Delete Notebook"
		<< "\n5.  Give Access to a Notebook"
		<< "\n6.  Remove Access to a Notebook"
		<< "\n7.  Logout and End Session"
		<< "\n8.  Delete Account\n\n";

	cout << "What would you like to do " << system->getCurrUsername() << "?";
	cout << "\n\tEnter menu option: ";
}

//***************************************************************************************************************************************

bool Controller::login()
{
	bool accountCreated = true;

	if (canFunction)
	{
		string username,
			password;
		vector<string> opts;

		_loginScreen();

		retry = true;
		opts.insert(opts.begin(), { "y", "n" });

		do
		{
			try
			{
				cout << "\nDo you already have an account?"
				     << "\n\tPlease enter \'Y\' or \'N\' (no quotation marks): ";
				getline(cin, strInput);

				if (!utl->isOption(opts, strInput))
					throw utl;
				else if (strInput == "n")
				{
					do
					{
					    createNewUser:
						try
						{
							cout << "\nNew Username: ";
							getline(cin, username);

							cout << "\nNew Password: ";
							getline(cin, password);

							if (utl->goodUserFormat(username) && utl->goodUserFormat(password))
							{
								if (system->createAccount(username, password))
								{
									cout << "\nAccount Created!\n";
									utl->pressKey();
									retry = false;
								}
								else
								{
									cout << '\n' << utl->invalid() << "username already exists"
									     << "\nWould you like to try again?";

									do
									{
										try
										{
											cout << "\n\tPlease enter \'Y\' or \'N\' (no quotation marks): ";
											getline(cin, strInput);

											if (!utl->isOption(opts, strInput))
												throw utl;
											else if (strInput == "y")
												goto createNewUser;
											else
											{
												retry = false;
												accountCreated = false;
											}
										}
										catch (Utility* utl)
										{
											cout << utl->invalid() << "options are \'Y\' or \'N\'\n";
											utl->pressKey();
										}
									} while (retry);
								}
							}
							else
								throw utl;
						}
						catch (Utility* utl)
						{
							cout << utl->invalid() << '\n';
							utl->displayStrFormatErrs();
							cout << "\n\n";
							goto createNewUser;
						}
					} while (retry);
				}
				else
					retry = false;
			}
			catch (Utility* utl)
			{
				cout << utl->invalid() << "options are \'Y\' or \'N\'\n";
			}
		
		} while (retry);
	
		utl->moveScreen(40);
		_loginScreen();

		retry = true;

		if (accountCreated)
		{
			do
			{
				try
				{
					cout << "\nUsername: ";
					getline(cin, username);

					cout << "\nPassword: ";
					getline(cin, password);

					if (system->login(username, password))
						retry = false;
					else
						throw utl;
				}
				catch (Utility* utl)
				{
					cout << utl->reEnter("username and password");
				}
			} while (retry);
		}
	}
	else
		cout << "\nError: system cannot function\n";

	return accountCreated;
}

//***************************************************************************************************************************************

void Controller::viewNotebook()
{
	if (canFunction)
	{
		retry = true;

		do
		{
			try
			{
				if (system->viewAllNotebooks())
				{
					cin.getline(charInput, 50);

					if (utl->isInt(charInput, option))
					{
						if (system->viewNotebook(option))
						{
							cout << "\n\n";
							utl->pressKey();
							retry = false;
						}
						else
						{
							cout << "Notebook not found";
							utl->reEnter("notebook ID");
						}
					}
					else
						throw utl;
				}
				else
				{
					cout << "\nNumber of Notebooks to View: 0\n";
					utl->pressKey();
					retry = false;
				}
			}
			catch (Utility* utl)
			{
				cout << utl->invalid() << "\"" << utl->getErr() << "\" is not an integer"
					 << "\n" << utl->reEnter("notebook ID");
			}

		} while (retry);
	}
	else
		cout << "\nError: system cannot function\n";
}

//***************************************************************************************************************************************

void Controller::addNotebook()
{
	vector<string> opts;
	bool reValidate;

	if (canFunction)
	{
		opts.insert(opts.begin(), { "y", "n" });

		do
		{
			utl->moveScreen(45);

			retry = true;
			reValidate = false;

			cout << "What would you like the name to be? (must end in \'.txt\' [no quotation marks])"
			     << "\n\n\tEnter notebook name: ";

			try
			{
				getline(cin, strInput);

				if (utl->goodNotebookFormat(strInput))
				{
					if (system->createNotebook(strInput))
					{
						cout << "\nNew Notebook Added!\n";
						utl->pressKey();
						retry = false;
					}
					else
					{
						cout << utl->invalid() << "Notebook already exists. Notebook names must be unique"
							<< "\n\nWould you like to enter a differnt name?";
						do
						{
							try
							{
								cout << "\n\tPlease enter \'Y\' or \'N\' (no quotation marks): ";
								getline(cin, strInput);

								if (!utl->isOption(opts, strInput))
									throw utl;
								else if (strInput == "n")
									retry = false;
								else if (strInput == "y")
								{
									retry = false;
									reValidate = true;
								}

							}
							catch (Utility* utl)
							{
								cout << utl->invalid() << "options are \'Y\' or \'N\'\n\n";
								utl->pressKey();
							}
						} while (retry);
					}		
				}
				else
					throw utl;
			}
			catch (Utility* utl)
			{
				cout << utl->invalid() << '\n';
				utl->displayStrFormatErrs();
				cout << "\nWoud you like to retry?";

				do
				{
					try
					{
						cout << "\n\tPlease enter \'Y\' or \'N\' (no quotation marks): ";
						getline(cin, strInput);

						if (!utl->isOption(opts, strInput))
							throw utl;
						else if (strInput == "n")
							retry = false;
						else if (strInput == "y")
						{
							retry = false;
							reValidate = true;
						}
					}
					catch (Utility* utl)
					{
						cout << utl->invalid() << "options are \'Y\' or \'N\'\n\n";
						utl->pressKey();
					}
				} while (retry);
			}
		} while (retry || reValidate);
	}
	else
		cout << "\nError: system cannot function\n";
}

//***************************************************************************************************************************************

void Controller::editNotebook()
{
	if (canFunction)
	{
		retry = true;

		do
		{
			try
			{
				utl->moveScreen(45);

				if (system->displayUserNotebooks())
				{
					cout << "\nWhich notebook would you like to edit?"
						<< "\n\tEnter Notebook ID: ";

					cin.getline(charInput, 50);

					if (utl->isInt(charInput, option))
					{
						if (system->editNotebook(option))
							retry = false;
						else
						{
							cout << "\nNotebook not found";
							cout << '\n';
							utl->pressKey();
						}
					}
					else
						throw utl;
				}
				else
				{
					cout << "\nCurrent Number of Notebooks: 0\n";
					utl->pressKey();
					retry = false;
				}
			}
			catch (Utility* utl)
			{
				cout << utl->invalid() << "\"" << utl->getErr() << "\" is not an integer"
					<< "\n" << utl->reEnter("notebook ID");
			}

		} while (retry);
	}
	else
		cout << "\nError: system cannot function\n";
}

//***************************************************************************************************************************************

void Controller::deleteNotebook()
{
	vector<string> opts;
	bool stay = true;

	if (canFunction)
	{
		opts.insert(opts.begin(), { "y", "n", "exit"});

		do
		{
			utl->moveScreen(45);

			retry = true;

			if (system->viewAllNotebooks())
			{
				cin.getline(charInput, 50);

				try
				{
					if (utl->isInt(charInput, option) && (strlen(charInput) > 0))
					{
						cout << "\nAre you sure you want to delete this notebook?";

						do
						{
							try
							{
								cout << "\n\tPlease enter \'Y\', \'N\', or \"EXIT\" (no quotation marks): ";
								getline(cin, strInput);

								if (!utl->isOption(opts, strInput))
									throw utl;
								else if (strInput == "n")
									retry = false;
								else if (strInput == "y")
								{
									if (system->deleteNotebook(option))
									{
										cout << "\n\nNotebook deleted!\n";
										utl->pressKey();
										retry = false;
										stay = false;
									}
									else
									{
										utl->moveScreen(45);
										cout << "Notebook does not exit\n";
										utl->pressKey();
										retry = false;
									}
								}
								else if (strInput == "exit")
								{
									retry = false;
									stay = false;
								}
							}
							catch (Utility* utl)
							{
								cout << '\n' << utl->invalid() << "options are \'Y\', \'N\', or \"EXIT\"\n";
								utl->pressKey();
							}
						} while (retry);
					}
					else
						throw utl;
				}
				catch (Utility* utl)
				{
					cout << utl->invalid() << "\"" << utl->getErr() << "\" is not an integer"
						<< "\n" << utl->reEnter("notebook ID");
				}		
			}
			else
			{
				cout << "\nCurrent Number of Notebooks: 0\n";
				utl->pressKey();
				stay = false;
			}
		} while (stay);
	}
	else
		cout << "\nError: system cannot function\n";
}

//***************************************************************************************************************************************

void Controller::giveAccess()
{
	int notebookOption;
	vector<int> userIDs;
	vector<string> opts;

	if (canFunction)
	{
		retry = true;
		opts.insert(opts.begin(), { "y", "n", "exit" });

		do
		{
			try
			{
				utl->moveScreen(45);

				if (system->viewAllNotebooks())
				{
					cin.getline(charInput, 50);

					if (utl->isInt(charInput, notebookOption))
					{
						enterUsers:
						utl->moveScreen(45);

						if (system->displayAllUsers())
						{
							cout << "\n\nPress the \"ENTER\" key after inputing an ID"
								<< "\nWhen finished, input \"DONE\" (no quotation marks)\n";
							do
							{
								try
								{
									cout << "-> ";

									cin.getline(charInput, 50);

									if (!utl->isInt(charInput, option))
									{
										for (int i = 0; i < strlen(charInput); ++i)
											charInput[i] = tolower(charInput[i]);

										if (strcmp(charInput, "done"))
											throw utl;
									}
									else if (option == 0)
										throw (utl->invalid());
									else
										userIDs.push_back(option);
								}
								catch (Utility* utl)
								{
									cout << '\n' << utl->invalid() << "\"" << utl->getErr() << "\" is not an integer"
										<< "\n\"" << utl->getErr() << "\" does not = \"DONE\""
										<< "\n" << utl->reEnter("user ID");
								}
								catch (string s)
								{
									cout << '\n' << s << "ID cannot be \'0\' or empty"
										<< "\n" << utl->reEnter("user ID");
								}
							} while (strcmp(charInput, "done"));

							if (userIDs.size() > 0)
							{
								for (int i = 0; i < userIDs.size(); ++i)
									cout << "\n\tID: " << userIDs[i];

								cout << "\nAre these the correct user ID(s)?";
							}
							else
							{
								cout << "\n\nNo users were entered\n";
								utl->pressKey();
								break;
							}

							try
							{
								cout << "\n\tPlease enter \'Y\', \'N\', or \"EXIT\" (no quotation marks): ";
								getline(cin, strInput);

								if (!utl->isOption(opts, strInput))
									throw utl;
								else if (strInput == "n")
								{
									userIDs.clear();
									goto enterUsers;
								}
								else if (strInput == "exit")
								{
									retry = false;
									break;
								}

								if (system->givePermission(notebookOption, userIDs))
								{
									cout << "\nUsers Were Given Permission!\n";
									utl->pressKey();
									retry = false;
								}
								else
								{
									utl->moveScreen(45);
									cout << "\nNotebook or User not found\n";
									utl->pressKey();
								}
							}
							catch (Utility* utl)
							{
								cout << utl->invalid() << "options are \'Y\' or \'N\'\n";
								utl->pressKey();
							}
						}
						else
						{
							cout << "\nNumber of Other Users: 0\n";
							utl->pressKey();
							retry = false;
						}
					}
					else
						throw utl;
				}
				else
				{
					cout << "\nCurrent Number of Notebooks: 0\n";
					utl->pressKey();
					retry = false;
				}
			}
			catch (Utility* utl)
			{
				cout << utl->invalid() << "\"" << utl->getErr() << "\" is not an integer"
					<< "\n" << utl->reEnter("notebook ID");
			}
		} while (retry);
	}
	else
		cout << "\nError: system cannot function\n";
}

//***************************************************************************************************************************************

void Controller::removeAccess()
{
	int notebookOption;
	vector<int> userIDs;
	vector<string> opts;

	if (canFunction)
	{
		retry = true;
		opts.insert(opts.begin(), { "y", "n", "exit" });

		do
		{
			try
			{
				utl->moveScreen(45);

				if (system->viewAllNotebooks())
				{
					cin.getline(charInput, 50);

					if (utl->isInt(charInput, notebookOption))
					{
					enterUsers:
						utl->moveScreen(45);

						if (system->displayUserGivesAccess(notebookOption))
						{
							cout << "\n\nPress the \"ENTER\" key after inputing an ID"
								<< "\nWhen finished, input \"DONE\" (no quotation marks)\n";
							do
							{
								try
								{
									cout << "-> ";

									cin.getline(charInput, 50);

									if (!utl->isInt(charInput, option))
									{
										for (int i = 0; i < strlen(charInput); ++i)
											charInput[i] = tolower(charInput[i]);

										if (strcmp(charInput, "done"))
											throw utl;
									}
									else if (option == 0)
										throw (utl->invalid());
									else
										userIDs.push_back(option);
								}
								catch (Utility* utl)
								{
									cout << '\n' << utl->invalid() << "\"" << utl->getErr() << "\" is not an integer"
										<< "\n\"" << utl->getErr() << "\" does not = \"DONE\""
										<< "\n" << utl->reEnter("user ID");
								}
								catch (string s)
								{
									cout << '\n' << s << "ID cannot be \'0\' or empty"
										<< "\n" << utl->reEnter("user ID");
								}
							} while (strcmp(charInput, "done"));

							if (userIDs.size() > 0)
							{
								for (int i = 0; i < userIDs.size(); ++i)
									cout << "\n\tID: " << userIDs[i];

								cout << "\nAre these the correct user ID(s) to remove access?";
							}
							else
							{
								cout << "\n\nNo users were entered\n";
								utl->pressKey();
								break;
							}

							try
							{
								cout << "\n\tPlease enter \'Y\', \'N\', or \"EXIT\" (no quotation marks): ";
								getline(cin, strInput);

								if (!utl->isOption(opts, strInput))
									throw utl;
								else if (strInput == "n")
								{
									userIDs.clear();
									goto enterUsers;
								}
								else if (strInput == "exit")
								{
									retry = false;
									break;
								}

								if (system->removePermission(notebookOption, userIDs))
								{
									cout << "\nUser(s) Access to Notebook was Removed!\n";
									utl->pressKey();
									retry = false;
								}
								else
								{
									utl->moveScreen(45);
									cout << "\nUser not Found\n";
									utl->pressKey();
								}
							}
							catch (Utility* utl)
							{
								cout << utl->invalid() << "options are \'Y\' or \'N\'\n";
								utl->pressKey();
							}
						}
						else
						{
							cout << "\nNotebook or Other Users with Access not Found\n";
							utl->pressKey();
							retry = false;
						}
					}
					else
						throw utl;
				}
				else
				{
					cout << "\nCurrent Number of Notebooks: 0\n";
					utl->pressKey();
					retry = false;
				}
			}
			catch (Utility* utl)
			{
				cout << utl->invalid() << "\"" << utl->getErr() << "\" is not an integer"
					<< "\n" << utl->reEnter("notebook ID");
			}
		} while (retry);
	}
	else
		cout << "\nError: system cannot function\n";
}

//***************************************************************************************************************************************

bool Controller::deleteAccount()
{
	bool deleteAccount = false;
	vector<string> opts;

	if (canFunction)
	{
		opts.insert(opts.begin(), { "y", "n" });

		retry = true;

		do
		{
			try
			{
				cout << "\n\nAre you sure you want to delete your account?"
				     << "\n\tPlease enter \'Y\' or \'N\' (no quotation marks): ";
				getline(cin, strInput);

				if (!utl->isOption(opts, strInput))
					throw utl;
				else if (strInput == "n")
					retry = false;
				else if (strInput == "y")
				{
					system->deleteAccount();
					cout << "\n\nAccount Succesfully Deleted\n";
					utl->pressKey();
					deleteAccount = true;
					retry = false;
				}
			}
			catch (Utility* utl)
			{
				cout << '\n' << utl->invalid() << "options are \'Y\' or \'N\'\n";
				utl->pressKey();
			}
		} while (retry);
	}
	else
		cout << "\nError: system cannot function\n";

		
	return deleteAccount;
}