#define STRICT

#include <stdio.h>

#include "Asteroids.h"

int main() {
	Asteroids* asteroids = new Asteroids(1024, 1024);
	asteroids->initiate();

	delete asteroids;

	return EXIT_SUCCESS;
};