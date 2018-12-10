#include <iostream>
#include <string>
#include "rng.h"


class customer {
	RNG rng;
	static int last_ID; //Auto updated when constructor used
	int ID; //Auto assigned based on last_ID
	int arrival_time;
	int total_wait_time; //Just keep adding passed time OR 
	int service_wait_time; //deted by service
	char service; //Make service det by RNG. Service dets service_wait_time.
	void assign_service_type_and_time();

public:
	customer();
	customer(int);
	int get_service_wait_time();
	int get_total_wait_time();
	int get_id();
	char get_service_type();
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

char customer::get_service_type() {
	return service;
}

void customer::update_total_wait_time(int current_time) {
	total_wait_time = arrival_time - current_time + service_wait_time;
}

void customer::decr_service_wait_time() {
	service_wait_time--;
}

int customer::last_ID = 0;

void customer::assign_service_type_and_time() {
	switch (rng.roll()%4) {
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

bool customer::done() {
	return (service_wait_time == 0);
}