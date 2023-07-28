#define STRICT

#include "Asteroids.h"

int main() {
	Asteroids* asteroids = new Asteroids();
	asteroids->initialize();

	delete asteroids;

	return EXIT_SUCCESS;
};
