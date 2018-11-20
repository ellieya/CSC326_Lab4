#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class customer {
	static int last_ID; //Auto updated when constructor used
	int ID; //Auto assigned based on last_ID
	int arrival_time;
	int total_wait_time; //Just keep adding passed time OR 
	int service_wait_time; //deted by service
	char service; //Make service det by RNG. Service dets service_wait_time.
	bool done; //True when done with services

public:
	customer();
	customer(int);
	int get_service_wait_time();
	int get_total_wait_time();
	int get_id();
	void update_total_wait_time(int);
	void assign_service_type_and_time();
	int RNG(); //Rolls random number
};

customer::customer()
{

}

customer::customer(int time)
	:arrival_time(time), ID(++last_ID)
{
	assign_service_type_and_time();
}

int customer::get_service_wait_time() {
	return service_wait_time;
}

int customer::get_total_wait_time() {
	return total_wait_time;
}

int customer::get_id() {
	return ID;
}

void customer::update_total_wait_time(int current_time) {
	total_wait_time = current_time - arrival_time;
}

int customer::last_ID = 0;

void customer::assign_service_type_and_time() {
	switch (RNG() % 4) {
	case 0:
		service = 'D'; //Deposit
		service_wait_time = 2;
		break;
	case 1:
		service = 'W'; //Withdrawl
		service_wait_time = 5;
		break;
	case 2:
		service = 'O'; //Open account
		service_wait_time = 20;
		break;
	case 3:
		service = 'C'; //Close account
		service_wait_time = 30;
		break;
	}
	
}

int customer::RNG() {
	time_t timer;
	srand(time(&timer));
	return rand();
}

bool done() {
	//Hmm
	/*

	Ok let's think this through...
	1. Customer will come wait in line. This is unknown # and does not need to be kept track of.
	2. Customer will finish waiting and go to service. When service, known #.
	3. Customer will be done when customer timer = 0;

	How customer timer go down?
	On keypress -> Send signal that time progression to CUST and SERVER
	If CUST status = 'S', time will decr.
	On time decr, check if 0.
	If 0, DONE flag = true
	when DONE flag = true, send signal to SERVER that SERVER is free //We can also just leave wait time to server only, we will have to see as we build program
	Customer will no longer be tracked...

	Maybe currently being served customer will be stored in pointer
	Then deleted when no longer used.

	But since array probably good idea to just copy value into another var.
	
	*/
}