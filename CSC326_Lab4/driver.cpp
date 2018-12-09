#include <windows.h> //Sleep
#include <cstdlib>
#include <ctime>
#include "customer.h"
#include "teller.h"
#include "ArrayQueue.h"

using namespace std;

//void print();
//cout all relevant information

void get_user_input(int &, int &, int &, teller*);

void roll_new_customer(ArrayQueue<customer>&, int, int);
//POST: Rolls random number between X and N. If number hits a certain range, a new customer will be enqueued.

void service_next(ArrayQueue<customer>&, teller*, int, int);
//Dequeues next avaliable customer and puts them to service and calculates total wait time for the customer

int next_avaliable_teller(teller*);
//Iterates through all service desks to see if avaliable

void adjust_all_clocks(ArrayQueue<customer>&, teller*, int&);
//Updates teller clock, customer clock, and "global" clock and switches service desk statuses back to 'A' if customer is completed with service

int main() {

	int clock;
	ArrayQueue<customer> line;
	int arrival_time_distribution;
	int expected_service_time;
	teller* employees;
	int next_teller_i;


	get_user_input(arrival_time_distribution, expected_service_time, clock, employees);

	while (clock != 0) {
		print();
		roll_new_customer(line, arrival_time_distribution, clock);
		next_teller_i = next_avaliable_teller(employees);
		if (!line.isEmpty() && next_teller_i != -1) {
			service_next(line, employees, next_teller_i, clock);
		}
		adjust_all_clocks(line);
		Sleep(1000);
	}

	return 0;
}

void roll_new_customer(ArrayQueue<customer> &line, int arrival_time_distribution, int clock) {
	
	srand(clock);

	//Update value to be between 0 and 10000 to increase accuracy
	arrival_time_distribution *= (10000 / arrival_time_distribution);
	
	//Roll for new customer. If no new customer, do nothing.
	if ((rand()%10000) <= approx_time_diff_since_last) {
		customer next_customer(clock);
		line.enqueue(next_customer);
	}
}

void get_user_input(int &arrival_time_distribution, int &expected_service_time, int &clock, teller* teller_pointer) {
	int number_of_servers;
	cout << "Input number of servers: "; cin >> number_of_servers;
	//Declare & initialize tellers based on input
	cout << "Input distribution of arrival times: "; cin >> arrival_time_distribution;
	cout << "Input expected service time: "; cin >> expected_service_time;
	cout << "Input simulation time(s): "; cin >> clock;

	teller_pointer = new teller[number_of_servers];
}

void service_next(ArrayQueue<customer>& line, teller* employees, int next_teller_i, int clock) {
	line.peekFront().update_total_wait_time(clock); //We now have all the information necessary to update total wait time for customer
	employees[next_teller_i].update_currently_servicing(line.peekFront()); //Bring customer to service
	line.dequeue(); //Dequeue from line
}

void adjust_all_clocks(ArrayQueue<customer>& line, teller* employees, int& clock) {

	//Decrement timer for customers receiving service
	for (int i = 0; i < sizeof(employees) / sizeof(*employees); i++) {
		if (employees[i].get_status() == 'B') {
			employees[i].get_currently_servicing().decr_service_wait_time(); //Decrement current customer
			if (employees[i].get_currently_servicing().done()) { //See if done
				//If done,
				
					//Print customer data to file
				employees[i].switch_status();  //switch status & delete current customer
			}
		}
	}

	//Decrement "global" clock
	clock--;

}

int next_avaliable_teller(teller* employees) {
	for (int i = 0; i < sizeof(employees) / sizeof(*employees); i++) {
		if (employees[i].get_status() == 'A')
			return i; //Should short-circuit loop as soon as an avaliable teller is found
	}

	return -1; //If none return i, that means all are busy. Return -1.
}