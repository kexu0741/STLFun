#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct date{ // preparation for date and time conversion
	int day;
	int month;
	int year;
};

class Customer{
	public:
		Customer(){} // default constructor
		Customer(string _firstName, string _lastName, string _birthDay, 
			string _email, string _street, string _city, string _state, string _zipCode){ // params are read in from file
			firstName = _firstName;
			lastName = _lastName;
			birthDay = _birthDay;
			email = _email;
			street = _street;
			city = _city;
			state = _state;
			zipCode = _zipCode;
		}
		Customer(string _lastName){ // unique constructor needed for dummy object creation for searching
			lastName = _lastName;
		}
		Customer(string _lastName, string _state){ // unique constructor need for dummy object creation for searching
			state = _state;
		}

		string getFirstName(){
			return firstName;
		}

		string getLastName() const{
			return lastName;
		}

		string getState() const{
			return state;
		}

		string getBirthDay() const{
			return birthDay;
		}

		string getEmail(){
			return email;
		}

		string getStreet(){
			return street;
		}

		string getCity(){
			return city;
		}

		string getZipCode(){
			return zipCode;
		}

		date convertDOB(int _month, int _day, int _year){
			DOB.month = _month;
			DOB.day = _day;
			DOB.year = _year;
		}

		date getDOB() const{
			return DOB;
		}

	private:
		string firstName;
		string lastName;
		string birthDay;
		string email;
		string street;
		string city;
		string state;
		string zipCode;
		date DOB;
};
// functor initialization
struct compareByLast{
	bool operator()(const Customer &c1, const Customer &c2){
		return (c1.getLastName() < c2.getLastName()); // functor allows for sort by last name, (A-Z)
	}
};

struct compareByState{
	bool operator()(const Customer &c1, const Customer &c2){
		return (c1.getState() < c2.getState()); // allows for sort by state, (A-Z)
	}
};

struct compareByBirthday{
	bool operator()(const Customer &c1, const Customer &c2){ // allows for sort by age/DOB
		if (c1.getDOB().year == c2.getDOB().year){ // checking need for month comparison
			if (c1.getDOB().month == c2.getDOB().month){ // checking need for day comparison
				return (c1.getDOB().day < c2.getDOB().day);
			}
			else {
				return (c1.getDOB().month < c2.getDOB().month);
			}	
		}
		else {
			return (c1.getDOB().year < c2.getDOB().year);
		}
	}
};

void printCustomer(Customer c){
	cout << c.getFirstName() << " " << c.getLastName() << " " << c.getEmail() << " " 
	<< c.getState() << " "<< c.getBirthDay() << endl;
}

void sortByLast(vector<Customer> &V){
	compareByLast comparer; // instantiates compareByLast functor
	sort(V.begin(), V.end(), comparer); // begin(), end() iterators passed in
}

void sortByState(vector<Customer> &V){
	compareByState comparer; // instanstiates compareByState functor
	sort(V.begin(), V.end(), comparer);
}

void searchByLast(vector<Customer> &V, string ln){ //create person with that last name, see where it would be slotted
	sortByLast(V); // sorted vector required for use of upper_bound, lower_bound

	vector<Customer>::iterator low, up; // iterators used for keeping track of range of results
	Customer c(ln); // dummy object, 'if object were to have name ln, where would it be
	compareByLast comparer;

	// use of lower_bound and upper_bound allows us to get a all possible search results
	low = lower_bound(V.begin(), V.end(), c, comparer); 
	up = upper_bound(V.begin(), V.end(), c, comparer);

	if (low == up){ // iterators point to same thing if object is not found
		cout << ln << " not found." << endl;
	}
	else{
		for (auto it = low; it != up; it++){
			printCustomer(*it);
		}
	}
}

void sortByBirthday(vector<Customer> &V){
	compareByBirthday comparer;
	sort(V.begin(), V.end(), comparer);
}

void searchByState(vector<Customer> &V, string st){
	sortByState(V);

	vector<Customer>::iterator low, up;
	Customer c(" ", st);
	compareByState comparer;

	low = lower_bound(V.begin(), V.end(), c, comparer); //ln needs to be a customer somehow......
	up = upper_bound(V.begin(), V.end(), c, comparer);

	if (low == up){
		cout << st << " not found." << endl;
	}
	else{
		cout << distance(low, up) << " customers in " << st << endl;
	}
}

int main(int argc, char* argv[]){ // command line argument: fileName
	string fileName = argv[1]; 

	ifstream in;
	in.open(fileName);
	if(!in){ // checks if file open
		cout << "invalid file" << endl;
		return 0;
	}

	string line;
	string token;
	char delim = ',';

	string components[9]; // array used to store the delimited components of a line read in
	int cellCnt;

	vector<Customer> customers;
	customers.reserve(100001); //assuming we know how many customers we have

	string dummy;
	getline(in, dummy); //ignores first line 
	while (getline(in, line)){
		cellCnt = 0;
		stringstream ss(line);
		while (getline(ss, token, delim)){
			components[cellCnt] = token;
			cellCnt++;
		}

		Customer created(components[1], components[2], components[3], components[4], components[5], components[6],
		components[7], components[8]);

		stringstream ss2(components[3]); //converting birthday into date format
		string token2;
		char delim2 = '/';
		int dateComponents[3]; //stores delimited components
		int dateCnt = 0;
		while (getline(ss2, token2, delim2)){
			dateComponents[dateCnt] = stoi(token2);
			dateCnt++;
		}

		created.convertDOB(dateComponents[0], dateComponents[1], dateComponents[2]); //month, day, year

		customers.push_back(created);
	}

	bool openMenu = true;
	int userInput;
	string name;
	string state;
	while (openMenu){ // initializes menu for user input
		cout << "-----------------------------------" << endl;
		cout << "1. Sort by last name" << endl;
		cout << "2. Search by last name" << endl;
		cout << "3. Sort by state" << endl;
		cout << "4. Find number of customers in a given state" << endl;
		cout << "5. Sort by birthday" << endl;
		cout << "6. Quit" << endl;
		cout << "-----------------------------------" << endl;

		cin >> userInput;

		switch(userInput){
			case 1:
				sortByLast(customers);
				for (auto it : customers){
					printCustomer(it);
				}
			break;

			case 2:
				cout << "Enter Name: " << endl;
				cin >> name;
				searchByLast(customers, name);
			break;

			case 3:
				sortByState(customers);
				for (auto it : customers){
					printCustomer(it);
				}
			break;

			case 4:
				cout << "Enter State: " << endl;
				cin >> state;
				searchByState(customers, state);
			break;

			case 5:
				sortByBirthday(customers);
				for (auto it : customers){
					printCustomer(it);
				}
			break;

			case 6:
				openMenu = false;
			break;
		}
	}

}