#pragma once

#include <vector>
#include <stack>

namespace Labyrinth {
	struct Node {
		int x {0};
		int y {0};
		bool visited {false};
	};

	class LabyrinthManager {
	public:
		LabyrinthManager();
		bool generateLabyrinth(std::size_t width, std::size_t height);
		void printLabyrinth();

	private:
		std::vector<int> mLabyrinth;
		std::size_t mWidth;
		std::size_t mHeight;
	};
}