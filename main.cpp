#include <iostream>

#include "LabyrinthManager.h"

int main() {
	Labyrinth::LabyrinthManager labyrinth;
	labyrinth.generateLabyrinth(100, 100);
	labyrinth.printLabyrinth();

	return 0;
}