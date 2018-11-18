#include <iostream>
#include <string>

class customer {
	static int last_ID;
	int ID;
	int wait_time;
	int total_wait_time;
	char status;

public:
	customer();
	customer(int);
	char get_status();
	int get_wait_time();
	int get_id();
	void decr_wait_time();
};

customer::customer()
	:wait_time(0)
{

}

customer::customer(int time)
	: wait_time(time), total_wait_time(time), status('A'), ID(last_ID + 1)
{

}

char customer::get_status() {
	return status;
}

int customer::get_wait_time() {
	return wait_time;
}

int customer::get_id() {
	return ID;
}

void customer::decr_wait_time() {
	wait_time--;
}

int customer::last_ID = 0;