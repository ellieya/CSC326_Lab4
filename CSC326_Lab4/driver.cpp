#include <Windows.h> //Sleep
#include <fstream> //For output log
#include "teller.h" //contains customer.h, rng.h, iostream, and string
#include "ArrayQueue.h"

/*
Ayy we basically done!
Add-ons:
-When closed, dequeue all and continue to service until no more people to service
*/

using namespace std;

/*
	PROTOTYPES
*/
void print(ArrayQueue<customer>, int, teller*, int);
//PRE: var/objs line, clock, employees, and num_of_tellers must be initialized
//Prints current time, tellers, and the line.

teller* get_user_input(int &, int &, int &, int &);
//PRE: vars arrival_time_distrubution, initial_clock, clock, and number_of_tellers must be declared.
//Gets the initial user inputs necessary for the program to work and returns teller*.

void roll_new_customer(ArrayQueue<customer>&, int, int, int);
//Rolls random number between X and initial_clock, using time as rand seed. If rolled number is less than or equal to initial_clock/arrival_time_distribution, a new customer will be enqueued.
//PRE: var/objs line, arrival_time_distribution, initial_clock, and clock must be initialized

void service_next(ArrayQueue<customer>&, teller*, int, int);
//Dequeues next avaliable customer and puts them to service and calculates total wait time for the customer
//PRE: var/objs line, employees, next_teller_i, and clock must be initalized

int next_avaliable_teller(teller*, int);
//Iterates through all service desks to see if there any avaliable tellers
//PRE: var/objs employees and number_of_tellers must be initialized
//POST: Returns index of the next avaliable teller. Otherwise, if all tellers are busy, returns -1.

void adjust_all_clocks(ArrayQueue<customer>&, teller*, int&, ofstream&, int);
//PRE: var/objs line, employees, clock, output_file, and number_of_tellers must be initialized. output_file must have an opened file.
//POST: Updates teller clock, customer clock, and "global" clock and switches service desk statuses back to 'A' if customer is completed with service

int main() {

	/*
		VARIABLE DECLARATION
	*/

	//User-inputted data
	int clock;
	int initial_clock;
	int arrival_time_distribution;

	//Line
	ArrayQueue<customer> line;
	
	//Teller
	teller* employees = nullptr;
	int next_teller_i;
	int number_of_tellers;
	bool service_end = false;

	//File
    ofstream output_file;
    output_file.open("output_log.txt");

	//Get user input
	employees = get_user_input(arrival_time_distribution, initial_clock, clock, number_of_tellers);
	system("cls");
	/*
		CODE
	*/

	while (!service_end) {
		
		if (clock != 0 )
			roll_new_customer(line, arrival_time_distribution, initial_clock, clock);

		//Condition placed in do-while loop to cover cases where 2 tellers are free at the same time.
		//do-while required to avoid next_teller_i staying as -1 forever
		do {
			next_teller_i = next_avaliable_teller(employees, number_of_tellers);
			if (!line.isEmpty() && next_teller_i != -1) {
				service_next(line, employees, next_teller_i, clock);
			}
		} while (!line.isEmpty() && next_teller_i != -1);

		print(line, clock, employees, number_of_tellers);
		adjust_all_clocks(line, employees, clock, output_file, number_of_tellers);
		Sleep(1000);
		system("cls");

		//Update service_end flag
		if (clock == 0 && line.isEmpty() && /* NEED TO GET CONDITION FOR NO EMPLOYEES ARE STILL BUSY WITH CUSTOMER THAT DOESN'T HAVE INSANE OVERHEAD */)
			//When clock = 0, line is empty, and the no employees are still not busy with a customer, service ends.
			service_end = true;
		else
			service_end = false;
		
	}
    
    output_file.close();

	return 0;
}

void print(ArrayQueue<customer> line, int clock, teller* employees, int number_of_tellers) {

	//Print clock
	cout << "----------" << '\n'
		<< "CLOCK" << '\n'
		<< "----------" << endl;
	
	if (clock != 0)
		cout << clock << " minutes until closing" << endl;
	else
		cout << "CLOSED!" << endl;

	//Print tellers
	cout << "\n----------" << '\n'
		<< "TELLERS" << '\n'
		<< "----------" << endl;
	
	for (int i = 0; i < number_of_tellers; i++) {
		cout << "Teller #" << i+1 << ": " << employees[i].get_status();
		if (employees[i].get_status() == 'B') {
			cout << " | Serving ID #" << employees[i].get_currently_servicing()->get_id() << " | Remaining Time: " << employees[i].get_currently_servicing()->get_service_wait_time();
		}
		cout << endl;
	}

	//Print queue if queue is not empty
	cout << "\n----------" <<'\n'
		<< "QUEUE" << '\n'
		<< "----------" << endl;

	if (!line.isEmpty()) {
		ArrayQueue<customer> line_copy = line;
		customer* holder = nullptr;
		int count = 0;

		//Get count first
		while (!line_copy.isEmpty()) {
			line_copy.dequeue();
			count++;
		}

		holder = new customer[count];

		//Store data into holder array
		for (int i = 0; i < count; i++) {
			holder[i] = line.peekFront();
			line.dequeue();
		}

		//Print holder array data
		for (int i = 0; i < count; i++) {
			cout << "Position #" << i+1 << " ID: " << holder[i].get_id() << endl;
		}
	}
	else {
		cout << "QUEUE IS EMPTY!" << endl;
	}

}

void roll_new_customer(ArrayQueue<customer> &line, int arrival_time_distribution, int initial_clock, int clock) {
	
	RNG rng;
	//Roll for new customer. If no new customer, do nothing.
	if (((rng.roll()%initial_clock)) <= (initial_clock / static_cast<double>(arrival_time_distribution))) {
		customer next_customer(clock);
		line.enqueue(next_customer);
	}
}

teller* get_user_input(int &arrival_time_distribution, int &initial_clock, int &clock, int& number_of_tellers) {
	cout << "Input number of servers: "; cin >> number_of_tellers;
	//Declare & initialize tellers based on input
	cout << "Input distribution of arrival times: "; cin >> arrival_time_distribution;
	cout << "Input simulation time(s): "; cin >> clock;

	initial_clock = clock;
	return new teller[number_of_tellers];
}

void service_next(ArrayQueue<customer>& line, teller* employees, int next_teller_i, int clock) {
	
	employees[next_teller_i].update_currently_servicing(line.peekFront()); //Bring customer to service
	employees[next_teller_i].get_currently_servicing()->update_total_wait_time(clock); //We now have all the information necessary to update total wait time for customer
	line.dequeue(); //Dequeue from line
}

void adjust_all_clocks(ArrayQueue<customer>& line, teller* employees, int& clock, ofstream &output_file, int number_of_tellers) {

	//Decrement timer for customers receiving service
	for (int i = 0; i < number_of_tellers; i++) {
		if (employees[i].get_status() == 'B') {
			employees[i].get_currently_servicing()->decr_service_wait_time(); //Decrement current customer's timer
			if (employees[i].get_currently_servicing()->done()) { //See if done
				//If done,
				//Print customer data to file
				output_file << "Customer " << employees[i].get_currently_servicing()->get_id() << " has departed at " << clock << " after waiting "	<< employees[i].get_currently_servicing()->get_total_wait_time() << " minutes. (Service type: " << employees[i].get_currently_servicing()->get_service_type() << ")." << endl; 
                //switch status & delete current customer
				employees[i].switch_status();  
			}
		}
	}

	//Decrement "global" clock, unless clock is already at 0
	if (clock != 0) {
		clock--;
	}

}

int next_avaliable_teller(teller* employees, int number_of_tellers) {
	for (int i = 0; i < number_of_tellers; i++) {
		if (employees[i].get_status() == 'A')
			return i; //Should short-circuit loop as soon as an avaliable teller is found
	}

	return -1; //If none return i, that means all are busy. Return -1.
}