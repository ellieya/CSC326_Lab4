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
	void assign_service_type_and_time();
	int RNG(); //Rolls random number

public:
	customer();
	customer(int);
	int get_service_wait_time();
	int get_total_wait_time();
	int get_id();
//	string get_service_type();
	void update_total_wait_time(int);
	void decr_service_wait_time();
	bool done(); //True when done with services
	
};

customer::customer() {

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

/*
string customer::get_service_type() {
	switch (service) {
	case 'D':
		return "Deposit";
		break;
	case 'W':
		return "Withdrawl";
		break;
	case 'O':
		return "Open Account";
		break;
	case 'C':
		return "Close Account";
		break;
	}
}
*/

void customer::update_total_wait_time(int current_time) {
	total_wait_time = arrival_time - current_time + service_wait_time;
}

void customer::decr_service_wait_time() {
	service_wait_time--;
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

bool customer::done() {
	return (service_wait_time == 0);
}