#include <iostream>
#include <exception>
#include <string>
#include "libMaze.h"
#include "libProc.h"


int main()
{
	std::string file_name;

	std::cout << "Enter the name of file(with format), or Q to quit:\n";

	while (true) {
		std::cout << ">> ";
		std::getline(std::cin, file_name);
		if (file_name == "Q")
			exit(0);
		try {
			Maze maze(file_name);
			Solver solver(maze);
			solver.PrintSolution(solver.Solve());
		}
		catch (std::exception& exc) {
			std::cout << exc.what();
		}
		std::cout << std::endl;
	}



	return 0;
}
