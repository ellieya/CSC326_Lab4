#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "teller.h"
#include "ArrayQueue.h"

/*
Ayy we basically done!
Add-ons:
-If keep dequeueing and searching for avaliable if !empty
-Print service name
-When closed, dequeue all and continue to service until no more people to service

also tidy the code it looks fucking disgusting
*/

using namespace std;

void print(ArrayQueue<customer>, int, teller*, int);
//cout all relevant information

void get_user_input(int &, int &, int&);

void roll_new_customer(ArrayQueue<customer>&, int, int, int);
//POST: Rolls random number between X and N. If number hits a certain range, a new customer will be enqueued.

void service_next(ArrayQueue<customer>&, teller*, int, int);
//Dequeues next avaliable customer and puts them to service and calculates total wait time for the customer

int next_avaliable_teller(teller*, int);
//Iterates through all service desks to see if avaliable

void adjust_all_clocks(ArrayQueue<customer>&, teller*, int&, ofstream&, int);
//Updates teller clock, customer clock, and "global" clock and switches service desk statuses back to 'A' if customer is completed with service

int main() {

	int clock;
	int initial_clock;
	ArrayQueue<customer> line;
	int arrival_time_distribution;
	teller* employees = nullptr;
	int next_teller_i;
	int number_of_tellers;
    ofstream output_file;
    
    output_file.open("output_log.txt");

	get_user_input(arrival_time_distribution, clock, number_of_tellers);
	initial_clock = clock;
	employees = new teller[number_of_tellers];

	while (clock != 0) {
		print(line, clock, employees, number_of_tellers);
		roll_new_customer(line, arrival_time_distribution, initial_clock, clock);
		next_teller_i = next_avaliable_teller(employees, number_of_tellers);
		if (!line.isEmpty() && next_teller_i != -1) {
			service_next(line, employees, next_teller_i, clock);
		}
		adjust_all_clocks(line, employees, clock, output_file, number_of_tellers);
		Sleep(1000);
		system("cls");
	}
    
    output_file.close();

	return 0;
}

void print(ArrayQueue<customer> line, int clock, teller* employees, int number_of_tellers) {

	//Print clock
	cout << "----------" << '\n' << "CLOCK" << '\n' << "----------" << endl;
	cout << clock << " minutes until closing" << endl;

	//Print tellers
	cout << "\n----------" << '\n' << "TELLERS" << '\n' << "----------" << endl;
	for (int i = 0; i < number_of_tellers; i++) {
		cout << "Teller #" << i << ": " << employees[i].get_status();
		if (employees[i].get_status() == 'B') {
			cout << " | Serving ID #" << employees[i].get_currently_servicing()->get_id() << " | Remaining Time: " << employees[i].get_currently_servicing()->get_service_wait_time();
		}
		cout << endl;
	}


	//Print queue if queue is not empty
	cout << "\n----------" << '\n' << "QUEUE" << '\n' << "----------" << endl;
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
	
	srand(clock);

	//Roll for new customer. If no new customer, do nothing.
	if ((rand()%initial_clock) <= (initial_clock / static_cast<double>(arrival_time_distribution))) {
		customer next_customer(clock);
		line.enqueue(next_customer);
	}
}

void get_user_input(int &arrival_time_distribution, int &clock, int& number_of_tellers) {
	cout << "Input number of servers: "; cin >> number_of_tellers;
	//Declare & initialize tellers based on input
	cout << "Input distribution of arrival times: "; cin >> arrival_time_distribution;
	cout << "Input simulation time(s): "; cin >> clock;
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
				output_file << "Customer " << employees[i].get_currently_servicing()->get_id() << " has departed at " << clock << " after waiting "	<< employees[i].get_currently_servicing()->get_total_wait_time() << " minutes." << endl; 
                //switch status & delete current customer
				employees[i].switch_status();  
			}
		}
	}

	//Decrement "global" clock
	clock--;

}

int next_avaliable_teller(teller* employees, int number_of_tellers) {
	for (int i = 0; i < number_of_tellers; i++) {
		if (employees[i].get_status() == 'A')
			return i; //Should short-circuit loop as soon as an avaliable teller is found
	}

	return -1; //If none return i, that means all are busy. Return -1.
}