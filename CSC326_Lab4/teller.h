#pragma once
#include "customer.h"

class teller {
	char status = 'A';
	customer* currently_servicing;

public:
	void switch_status();
	//Changes status and updates currently_servicing if switched from B to A
	void update_currently_servicing(customer);
	customer get_currently_servicing();
	char get_status();
};

void teller::switch_status() {
	if (status == 'A') {
		status = 'B';
	}
	else {
		status = 'A';
		delete currently_servicing;
	}
}

void teller::update_currently_servicing(customer incoming_customer) {
	currently_servicing = &incoming_customer;
	switch_status();
}

customer teller::get_currently_servicing() {
	return *currently_servicing;
}

char teller::get_status() {
	return status;
}