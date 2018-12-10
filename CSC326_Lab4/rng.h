#pragma once
#include <cstdlib>
#include <ctime>

class RNG{
public:
	int roll();
};

int RNG::roll() {
	time_t timer;
	srand(time(&timer));
	return rand();
}