#define STRICT

#include <stdio.h>

#include "Asteroids.h"

int main() {
	Asteroids* asteroid = new Asteroids(1024, 1024);
	asteroid->initiate();

	delete asteroid;

	return EXIT_SUCCESS;
};