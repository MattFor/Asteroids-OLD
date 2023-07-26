#define STRICT

#include <stdio.h>

#include "Asteroids.h"

int main() {
	Asteroids* asteroids = new Asteroids(1440, 2560, true);
	asteroids->initiate();

	delete asteroids;

	return EXIT_SUCCESS;
};