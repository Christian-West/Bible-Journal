#include "Controller.h"
#include <iostream>
using namespace std;

//***************************************************************************************************************************************

void handler(Controller& c);

//***************************************************************************************************************************************

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	Controller c;

	handler(c);

	return 0;
}

//***************************************************************************************************************************************

void handler(Controller& c)
{
	char input[50] = "";
	int option = 0;
	bool delAccount = false;

	if (c.login())
	{
		do
		{
			c.menu();
			cin.getline(input, 50);

			if (c.validate(input, option))
			{
				switch (option)
				{
				case 1:
					c.viewNotebook();
					break;
				case 2:
					c.addNotebook();
					break;
				case 3:
					c.editNotebook();
					break;
				case 4:
					c.deleteNotebook();
					break;
				case 5:
					c.giveAccess();
					break;
				case 6:
					c.removeAccess();
					break;
				case 8:
					delAccount = c.deleteAccount();
					break;
				}
			}
		} while ((option != 7) && !delAccount);
	}

	cout << "\nSession Ended. Have a Good Day!\n";
}