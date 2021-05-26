#include "libProc.h"
#include <iostream>
#include <exception>

using namespace std;

Solver::Solver(const Maze& maze) {
	_grid = maze;
	auto grid = _grid.GetGrid();
	_x_len = grid.size();
	_y_len = grid[0].size();
	_solution.resize(_x_len, std::vector<int>(_y_len));
	
}



bool Solver::IsDeadEnd(const std::vector<int>& prev, const std::vector<int>& cur) const{
	return abs(prev[0] - cur[0]) > 1 || abs(prev[1] - cur[1]) > 1
		|| (abs(prev[0] - cur[0]) == 1 && abs(prev[1] - cur[1]));
}


void Solver::PrintSolution(const std::vector<std::vector<int>>& solution) const {

	for (int i = 0; i < solution.size(); i++) {
		if (i > 0 && IsDeadEnd(solution[i],solution[i-1]))
			cout << "dead end" << endl;
		cout << '(' << solution[i][0] << ',' << solution[i][1] << ')' << endl;
		
	}

}


bool Solver::RecurSolve(int x, int y, vector<vector<int>>& path) {


	if (_grid.IsExit(x,y)) {
		_solution[x][y] = PATH;
		path.push_back({ x, y });
		return true;
	}

	// Check if maze[x][y] is valid
	if (_grid.NotWall(x, y) == true) {
		// Check if the current block is already part of solution path.
		if (_solution[x][y] == PATH)
			return false;


		_solution[x][y] = PATH;
		path.push_back({ x, y });

		if (RecurSolve(x + 1, y,path) == true)
			return true;

		if (RecurSolve(x, y + 1,path) == true)
			return true;
	
		if (RecurSolve(x - 1, y,path) == true)
			return true;

		if (RecurSolve(x, y - 1,path) == true)
			return true;


		// backtracking if stuck
		_solution[x][y] = WALL;
		return false;
	}

	return false;


}

void Solver::SetMaze(const Maze& maze) {
	_solution.clear();
	_grid = maze;
	auto grid = _grid.GetGrid();
	_x_len = grid.size();
	_y_len = grid[0].size();
	_solution.resize(_x_len, std::vector<int>(_y_len));

}



vector<vector<int>> Solver::Solve() {
	auto start = _grid.GetStart();
	if (_grid.GetGrid().empty())
		throw exception("Maze not set\n");
	if (!_grid.InsideMaze(start[0], start[1]))
		throw exception("Start position outside maze\n");
	

	vector<vector<int>> path;

	if (RecurSolve(start[0], start[1], path) == false) {
		throw exception("No solution\n");
	}
	
	return path;

	
}