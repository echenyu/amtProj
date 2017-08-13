//Budget visualizer from ledger
//Given a simple dated list of all payments/purchases, this program will
//1) Show total and categorized spending over a desired time period
//2) Show average monthly spending for a desired category
//3) Average monthly/yearly spending trends to suggest budget plan
//4) Show balances of each bank account

/* TO DO:
- FIX - calculations in custom time frame seem wrong
- FIX - average monthly spending should divide by current month, not 12?
- write tests for edge cases
- eg empty csv, invalid dates
- add in valid date check?
- translate from excel date punctuation to int MMDDYYYY
- translate from excel D/W to sign 1/-1
---> generalize repetitions into single functions
	- consolidate date reading functions
- list category options
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//Translate from MM/DD/YYYY to MMDDYYYY
int interpretDate(string origDate) {
	int date = 0;

	//-48 to get integer values from ASCII assignments
	char charM1 = (int)origDate[0] - 48;
	char charM2 = (int)origDate[1] - 48;

	char charD1 = (int)origDate[3] - 48;
	char charD2 = (int)origDate[4] - 48;

	char charY1 = (int)origDate[6] - 48;
	char charY2 = (int)origDate[7] - 48;
	char charY3 = (int)origDate[8] - 48;
	char charY4 = (int)origDate[9] - 48;

	date = (charM1 * 10E6) + (charM2 * 10E5) + (charD1 * 10E4) + (charD2 * 10E3)
		+ (charY1 * 10E2) + (charY2 * 100) + (charY3 * 10) + charY4;

	return date;
}

//Translate from D/W to +1/-1
int interpretSign(char origDW) {
	int sign = 0;
	if (origDW == 'D') {
		sign = 1;
	}
	else if (origDW == 'W') {
		sign = -1;
	}
	return sign;
}

ifstream ins;

//Print menu
void printMenu() {
	cout << "*******************************" << endl
		<< "      Budget Visualizer      " << endl
		<< "*******************************" << endl
		<< endl
		<< "To see spending in a specific month, press 1." << endl
		<< "To see spending in a specific year, press 2." << endl
		<< "To see spending in a custom time frame, press 3." << endl
		<< "To see average monthly spending, press 4." << endl
		<< "To see average spending in a specific category, press 5." << endl
		<< "To see current bank account balance(s), press 6." << endl
		<< "To see bank account balance(s) at a specific date, press 7." << endl
		<< "To see spending categories, press 8." << endl
		<< "To quit, press 0." << endl << endl;
}

//Get user choice from menu
int getUserChoice() {
	int userChoice = 0;
	printMenu();
	cin >> userChoice;
	while (userChoice < 0 || userChoice > 8) {
		cout << endl << "Invalid menu choice.  Choose again.";
		printMenu();
		cin >> userChoice;
	}
	return userChoice;
}

//Read in entry as date, sign, amt, bank, cat, subcat
void readEntry(int row);

//Extract month, day, and year from MMDDYYYY
void distillDate(int date) {

	int mn = 0;
	int day = 0;
	int yr = 0;

	//Extract month from date by dividing by 10E6 and truncating (int division)
	mn = date / 1000000;

	//Extract day from date by subtracting mn from fullDate, dividing by 10E4 and truncating
	day = (date - (1000000 * mn)) / 10000;

	//Extract year from date by subtracting mn and day from fullDate
	yr = date - ((1000000 * mn) + (10000 * day));

	return;
}


int main()
{
	readEntry(5);

	int userChoice = 1;
	while (userChoice != 0) {

		//Open document "ledger.csv" wtih input stream ins
		ins.open("ledger_for_L.csv");

		//Check to see if file has opened
		if (!ins.is_open()) {
			cout << "NO FILE HAS BEEN OPENED" << endl;
		} else {}

		//Count rows in .csv file
		int end = 0;
		string line;
		while (getline(ins, line)) {
			++end;
		}

		//Clear ins state
		ins.clear();
		ins.seekg(0, ios::beg);

		//Print menu and get user choice
		userChoice = getUserChoice();

		//Variables for entry components
		int date = 0, sign = 0;
		double amt = 0;
		string bank = "", cat = "", subcat = "";

		//Variables for date components
		int mn = 0, day = 0, yr = 0;

		int userMn = 0, userYr = 0, startDay = 0, startMonth = 0;
		int startYear = 0, endDay = 0, endMonth = 0, endYear = 0;

		//To read over punctuation in inputted dates
		char skip;

		//Monthly
		if (userChoice == 1) {

			//User inputs desired month as MM / YYYY
			cout << "Input the month you would like to see as MM/YYYY" << endl;
			cin >> userMn >> skip >> userYr;

			cout << endl;

			//Calculate total monthly income
			double mnInTotal = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				if (yr == userYr && mn == userMn && sign == 1) {
					mnInTotal += amt;
				}
			}

			cout << "Your total income for " << userMn << "/" << userYr << " is $" << mnInTotal << endl;

			ins.clear();
			ins.seekg(0, ios::beg);

			//Calculate total monthly spending
			double mnOutTotal = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				if (yr == userYr && mn == userMn && sign == -1) {
					mnOutTotal += amt;
				}
			}

			cout << "Your total spending for " << userMn << "/" << userYr << " is $" << mnOutTotal << endl;

			ins.clear();
			ins.seekg(0, ios::beg);

			//Calculate monthly spending by category
			//Variables for monthly category totals
			//To add a new category, add a variable mnCategory here and add an else if below.
			double mnHousing = 0;
			double mnFood = 0;
			double mnUtilities = 0;
			double mnHousehold = 0;
			double mnMedical = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				if (yr == userYr && mn == userMn && sign == -1) {
					if (cat == "Housing") {
						mnHousing += amt;
					}
					else if (cat == "Food") {
						mnFood += amt;
					}
					else if (cat == "Utilities") {
						mnUtilities += amt;
					}
					else if (cat == "Household") {
						mnHousehold += amt;
					}
					else if (cat == "Medical") {
						mnMedical += amt;
					}
				}
			}

			cout << "Monthly spending by category is: " << endl;
			cout << "Housing: $" << mnHousing << endl
				<< "Food: $" << mnFood << endl
				<< "Utilities: $" << mnUtilities << endl
				<< "Household: $" << mnHousehold << endl
				<< "Medical: $" << mnMedical << endl;

			ins.clear();
			ins.seekg(0, ios::beg);
		}


		//Yearly
		if (userChoice == 2) {

			//User inputs desired year as YYYY
			cout << "Input the year you would like to see as YYYY" << endl;
			cin >> userYr;

			cout << endl;

			//Calculate total yearly income
			double yrInTotal = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				if (yr == userYr && sign == 1) {
					yrInTotal += amt;
				}
			}

			cout << "Your total income for " << userYr << " is $" << yrInTotal << endl;

			ins.clear();
			ins.seekg(0, ios::beg);

			//Calculate total yearly spending
			double yrOutTotal = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				if (yr == userYr && sign == -1) {
					yrOutTotal += amt;
				}
			}

			cout << "Your total spending for " << userYr << " is $" << yrOutTotal << endl;

			ins.clear();
			ins.seekg(0, ios::beg);

			//Calculate yearly spending by category
			//Variables for yearly category totals; to add a new category, add a variable yrCategory here and add an else if below.
			double yrHousing = 0;
			double yrFood = 0;
			double yrUtilities = 0;
			double yrHousehold = 0;
			double yrMedical = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				if (yr == userYr && sign == -1) {
					if (cat == "Housing") {
						yrHousing += amt;
					}
					else if (cat == "Food") {
						yrFood += amt;
					}
					else if (cat == "Utilities") {
						yrUtilities += amt;
					}
					else if (cat == "Household") {
						yrHousehold += amt;
					}
					else if (cat == "Medical") {
						yrMedical += amt;
					}
				}
			}

			cout << "Yearly spending by category is: " << endl;
			cout << "Housing: $" << yrHousing << endl
				<< "Food: $" << yrFood << endl
				<< "Utilities: $" << yrUtilities << endl
				<< "Household: $" << yrHousehold << endl
				<< "Medical: $" << yrMedical << endl;

			ins.clear();
			ins.seekg(0, ios::beg);
		}


		//Custom
		if (userChoice == 3) {

			//User inputs desired time frame
			cout << "Input your desired starting date as MM/DD/YYYY" << endl;
			cin >> startMonth >> skip >> startDay >> skip >> startYear;
			cout << "Input your desired ending date as MM/DD/YYYY" << endl;
			cin >> endMonth >> skip >> endDay >> skip >> endYear;

			cout << endl;

			//Calculate total custom income
			double tsInTotal = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				if (yr >= startYear && yr <= endYear && sign == 1) {
					if (mn >= startMonth && mn <= endMonth) {
						if (day >= startDay && day <= endDay) {
							tsInTotal += amt;
						}
					}
				}
			}

			cout << "Your total income from " << startMonth << "/" << startDay << "/" << startYear
				<< " to " << endMonth << "/" << endDay << "/" << endYear << " is $" << tsInTotal << endl;

			ins.clear();
			ins.seekg(0, ios::beg);

			//Calculate total custom spending
			double tsOutTotal = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				if (yr >= startYear && yr <= endYear && sign == -1) {
					if (mn >= startMonth && mn <= endMonth) {
						if (day >= startDay && day <= endDay) {
							tsOutTotal += amt;
						}
					}
				}
			}

			cout << "Your total spending from " << startMonth << "/" << startDay << "/" << startYear
				<< " to " << endMonth << "/" << endDay << "/" << endYear << " is $" << tsOutTotal << endl;

			ins.clear();
			ins.seekg(0, ios::beg);

			//Calculate custom spending by category
			//Variables for custom category totals; to add a new category, add a variable yrCategory here and add an else if below.
			double tsHousing = 0;
			double tsFood = 0;
			double tsUtilities = 0;
			double tsHousehold = 0;
			double tsMedical = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				if (yr >= startYear && yr <= endYear && sign == -1) {
					if (mn >= startMonth && mn <= endMonth) {
						if (day >= startDay && day <= endDay) {
							if (cat == "Housing") {
								tsHousing += amt;
							}
							else if (cat == "Food") {
								tsFood += amt;
							}
							else if (cat == "Utilities") {
								tsUtilities += amt;
							}
							else if (cat == "Household") {
								tsHousehold += amt;
							}
							else if (cat == "Medical") {
								tsMedical += amt;
							}
						}
					}
				}
			}

			cout << "Your spending by category is: " << endl;
			cout << "Housing: $" << tsHousing << endl
				<< "Food: $" << tsFood << endl
				<< "Utilities: $" << tsUtilities << endl
				<< "Household: $" << tsHousehold << endl
				<< "Medical: $" << tsMedical << endl;

			ins.clear();
			ins.seekg(0, ios::beg);

		}

		//Calculate average monthly spending in each category
		if (userChoice == 4) {

			cout << endl;

			//Housing
			double mnOutAvg_Housing = 0;
			int counter = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				for (int month = 01; month <= 12; ++month) {
					if (mn == month && sign == -1) {
						if (cat == "Housing") {
							mnOutAvg_Housing += amt;
							counter += 1;
							mnOutAvg_Housing = mnOutAvg_Housing / counter;
						}
						else {}
					}
					else {}
				}
			}

			cout << "Average monthly spending on Housing is " << mnOutAvg_Housing << endl;

			ins.clear();
			ins.seekg(0, ios::beg);

			//Utilities
			double mnOutAvg_Utilities = 0;
			counter = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				for (int month = 01; month <= 12; ++month) {
					if (mn == month && sign == -1) {
						if (cat == "Utilities") {
							mnOutAvg_Utilities += amt;
							counter += 1;
							mnOutAvg_Utilities = mnOutAvg_Utilities / counter;
						}
						else {}
					}
					else {}
				}
			}

			cout << "Average monthly spending on Utilities is " << mnOutAvg_Utilities << endl;

			ins.clear();
			ins.seekg(0, ios::beg);

			//Food
			double mnOutAvg_Food = 0;
			counter = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				for (int month = 01; month <= 12; ++month) {
					if (mn == month && sign == -1) {
						if (cat == "Food") {
							mnOutAvg_Food += amt;
							counter += 1;
							mnOutAvg_Food = mnOutAvg_Food / counter;
						}
						else {}
					}
					else {}
				}
			}

			cout << "Average monthly spending on Food is " << mnOutAvg_Food << endl;

			ins.clear();
			ins.seekg(0, ios::beg);

			//Household
			double mnOutAvg_Household = 0;
			counter = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				for (int month = 01; month <= 12; ++month) {
					if (mn == month && sign == -1) {
						if (cat == "Household") {
							mnOutAvg_Household += amt;
							counter += 1;
							mnOutAvg_Household = mnOutAvg_Household / counter;
						}
						else {}
					}
					else {}
				}
			}

			cout << "Average monthly spending on Household is " << mnOutAvg_Household << endl;

			ins.clear();
			ins.seekg(0, ios::beg);

			//Medical
			double mnOutAvg_Medical = 0;
			counter = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				for (int month = 01; month <= 12; ++month) {
					if (mn == month && sign == -1) {
						if (cat == "Medical") {
							mnOutAvg_Medical += amt;
							counter += 1;
							mnOutAvg_Medical = mnOutAvg_Medical / counter;
						}
						else {}
					}
					else {}
				}
			}

			cout << "Average monthly spending on Medical is is " << mnOutAvg_Medical << endl;
		}

		// Average spending by category
		if (userChoice == 5) {
			string userCat;

			cout << "Input name of category." << endl;
			cin >> userCat;

			double yrOutAvg_userCat = 0;
			int counter = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				if (cat == userCat && sign == -1) {
					yrOutAvg_userCat += amt;
					counter += 1;
					yrOutAvg_userCat = yrOutAvg_userCat / counter;
				}
				else {}
			}
			cout << "Average annual spending in " << userCat << " is $" << yrOutAvg_userCat << endl;

			double mnOutAvg_userCat = 0;
			counter = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				for (int month = 01; month <= 12; ++month) {
					if (mn == month && sign == -1) {
						if (cat == userCat) {
							mnOutAvg_userCat += amt;
							counter += 1;
							mnOutAvg_userCat = mnOutAvg_userCat / counter;
						}
						else {}
					}
					else {}
				}
			}

			cout << "Average monthly spending on " << userCat << " is $" << mnOutAvg_userCat << endl;
		}

		//Calculate current bank account balance(s)
		//To add another bank add another double variable below
		if (userChoice == 6) {
			double balance_Citi = 0;
			double balance_WF = 0;
			double balance_TCF = 0;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				if (bank == "Citi") {
					balance_Citi += (sign * amt);
				}
				else if (bank == "WF") {
					balance_WF += (sign * amt);
				}
				else if (bank == "TCF") {
					balance_TCF += (sign * amt);
				}
			}

			cout << "Current Citi bank balance is $" << balance_Citi << endl;
			cout << "Current Wells Fargo bank balance is $" << balance_WF << endl;
			cout << "Current TCF bank balance is $" << balance_TCF << endl;
		}

		//Calculate current bank account balance(s)
		//To add another bank add another double variable below
		if (userChoice == 7) {
			double balance_Citi = 0;
			double balance_WF = 0;
			double balance_TCF = 0;

			double userMonth;
			double userDay;
			double userYear;

			cout << "Input your desired date as MM/DD/YYYY" << endl;
			cin >> userMonth >> skip >> userDay >> skip >> userYear;

			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				distillDate(date);
				if (mn <= userMonth && yr <= userYear && day <= userDay) {
					if (bank == "Citi") {
						balance_Citi += (sign * amt);
					}
					else if (bank == "WF") {
						balance_WF += (sign * amt);
					}
					else if (bank == "TCF") {
						balance_TCF += (sign * amt);
					}
				}
				else {}
			}

			cout << "Citi bank balance as of " << userMonth << "/" << userDay << "/" << userYear << " is $" << balance_Citi << endl;
			cout << "Wells Fargo bank balance as of " << userMonth << "/" << userDay << "/" << userYear << " is $" << balance_WF << endl;
			cout << "TCF bank balance as of " << userMonth << "/" << userDay << "/" << userYear << " is $" << balance_TCF << endl;
		}

		ins.clear();
		ins.seekg(0, ios::beg);

		//Print categories
		if (userChoice == 8) {
			cout << "Categories are: " << endl;
			for (int row = 1; row <= end; ++row) {
				readEntry(row);
				string cat2 = "cat2";
				if (cat != cat2) {
					cout << "    - " << cat << endl;
				}
				cat2 = cat;
			}
		}


		ins.close();

		cout << endl << endl << endl;
	}
	return 0;
}

void readEntry(int row) {
	string origDate, bank, cat, subcat;
	int date, sign;
	char origDW = 0;
	double amt = 0;

	//To skip over commas in .csv
	char comma;

	getline(ins, origDate, ',');
	ins >> origDW >> comma;
	ins >> amt >> comma;
	getline(ins, bank, ',');
	getline(ins, cat, ',');
	getline(ins, subcat);

	date = interpretDate(origDate);
	sign = interpretSign(origDW);
	return;
}