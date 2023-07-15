#define STRICT

#include <stdio.h>

#include "Asteroid.h"


int main() {

	Asteroid* asteroid = new Asteroid();
	asteroid->run();

	delete asteroid;

	return 0;
};
