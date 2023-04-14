#include <iostream>

#include "LabyrinthManager.h"

constexpr bool snapshots {false};

namespace Labyrinth {
	LabyrinthManager::LabyrinthManager() : mWidth{}, mHeight{} {};

	bool LabyrinthManager::generateLabyrinth(std::size_t width, std::size_t height) {
		if(width < 3 || height < 3) return false; // too small
		
		std::vector<Node> allNodes;
		std::stack<Node*> nodeStack;

		// define the position data of all the nodes
		allNodes.resize(width*height);
		for(int i = 0; i < allNodes.size(); i++) {
			int x = i % width;
			int y = i / width;
			allNodes.at(i).x = x;
			allNodes.at(i).y = y;
		}

		// take the middle most position as the starting point when generating the labyrinth
		nodeStack.push(&allNodes.at((width/2) + (height/2) * width));

		while(!nodeStack.empty()) {
			Node* current = nodeStack.top();
			current->visited = true;


			if(snapshots) {
				// For debugging purposes. Displays a snapshot after each iteration.
				// Best used with smaller labyrinths (e.g. 25x25, otherwise the loading takes a while
				static int counter = 1;
				std::cout << "Snapshot #" << counter++ << '\n';
				for(int i = 0; i < allNodes.size(); i++) {
					int x = i % width;

					if(allNodes.at(i).visited) {
						if(&allNodes.at(i) == current) {
							std::cout << "c ";
						}
						else {
							std::cout << ". ";
						}
					}
					else {
						std::cout << "# ";
					}

					if(x >= width-1) std::cout << '\n';
				}
				std::cout << '\n';
			}

			std::vector<Node*> adjacents;

			for(int y = -1; y <= 1; y++) {
				for(int x = -1; x <= 1; x++) {
					if(x == 0 && y == 0) continue; // ignore self
					if(y != 0 && x != 0) continue; // ignore diagonals

					// determine adjacent absolute grid position
					int adjacentAbsX = current->x + x;
					int adjacentAbsY = current->y + y;

					// ignore out of bounds
					if(adjacentAbsX < 0 || adjacentAbsY < 0 || adjacentAbsX >= width || adjacentAbsY >= height) continue;

					// ignore the borders of the grid
					if(adjacentAbsX == 0 || adjacentAbsX >= width-1 || adjacentAbsY == 0 || adjacentAbsY >= height-1) continue;

					Node* adjacent = &allNodes.at(adjacentAbsX + adjacentAbsY * width);
					if(adjacent->visited) continue; // ignore adjacent node if it has already been visited

					// get direction -1 to 1
					int dirX = adjacent->x - current->x;
					int dirY = adjacent->y - current->y;
					bool horizontal = dirX != 0;

					// scan to see if the adjacent node is too close to a visited node
					bool isValid = true;

					// Example of how the check works when scanning in certain directions
					// c is the current position
					// ^ > v < is the adjacent node that is being checked if it's valid, and the direction of the check
					// S are the nearby nodes being scanned. if any of those nodes are invalid (i.e., an already visited node, the adjacent node is invalid
					// 
					// Scan North  Scan East  Scan South  Scan West
					// . . . . . | . . . . . | . . . . . | . . . . .
					// . S S S . | . . S S . | . . c . . | . S S . .
					// . S ^ S . | . c > S . | . S v S . | . S < c .
					// . . c . . | . . S S . | . S S S . | . S S . .
					// . . . . . | . . . . . | . . . . . | . . . . .
					//

					if(horizontal) {
						for(int y1 = -1; y1 <= 1; y1++) {
							for(int x1 = 0; x1 <= 1; x1++) {
								// determine x and y scan position
								int xScanPos = adjacent->x + x1 * dirX;
								int yScanPos =	adjacent->y + y1 * dirX;

								// ignore checking out of bounds
								if(xScanPos < 0 || yScanPos < 0 || xScanPos >= width || yScanPos >= height) continue;

								// get the data of the node to check
								Node* check = &allNodes.at(xScanPos + yScanPos * width);

								// ignore self checking
								if(check == adjacent) continue;

								// if it has been visited, the adjacent node is invalid because it's too close to other nodes
								if(check->visited) isValid = false;
							}
						}
					}
					else {
						// same as above, except checking vertically is slightly different
						for(int y1 = 0; y1 <= 1; y1++) {
							for(int x1 = -1; x1 <= 1; x1++) {
								int xScanPos = adjacent->x + x1 * dirY;
								int yScanPos =	adjacent->y + y1 * dirY;
								if(xScanPos < 0 || yScanPos < 0 || xScanPos >= width || yScanPos >= height) continue;
								Node* check = &allNodes.at(xScanPos + yScanPos * width);
								if(check == adjacent) continue;
								if(check->visited) isValid = false;
							}
						}
					}

					// if the adjacent node pass all the checks, push it onto the stack
					if(isValid) {
						adjacents.push_back(adjacent);
					}
				}
			}

			if(adjacents.empty()) {
				// pop if there are no available edges so that the loop can restart and try the next one in the stack
				nodeStack.pop();
			}
			else {
				// pick a random edge and push it onto the stack so that it can be used as the next node
				Node* next = adjacents.at(rand() % adjacents.size());
				nodeStack.push(next);
			}
		}

		// finalize the labyrinth
		std::vector<int> result;
		result.resize(width*height);
		for(int i = 0; i < allNodes.size(); i++) {
			if(!allNodes.at(i).visited)
				result.at(i) = 1;
		}

		mLabyrinth = result; // set the labyrinth of the manager to the finalized labyrinth

		// store the width and height of the generated labyrinth
		mWidth = width;
		mHeight = height;

		return true; // return result of the labyrinth generation
	}

	void LabyrinthManager::printLabyrinth() {
		for(int i = 0; i < mLabyrinth.size(); i++) {
			int x = i % mWidth;

			if(mLabyrinth.at(i) == 0) {
				std::cout << ". ";
			}
			else {
				std::cout << "# ";
			}

			if(x >= mWidth - 1) std::cout << '\n';
		}
	}
}