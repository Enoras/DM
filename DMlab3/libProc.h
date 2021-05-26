#pragma once
#include "libMaze.h"


class Solver {
private:
	
	Maze _grid;
	std::vector<std::vector<int>> _solution; // contains only final version of path
	size_t _x_len;
	size_t _y_len;

private:
	bool RecurSolve(int x, int y, std::vector<std::vector<int>>& path);
	bool IsDeadEnd(const std::vector<int>& prev, const std::vector<int>& cur) const;
public:
	Solver() = default;
	Solver(const Maze& maze);
	void SetMaze(const Maze& maze);
	std::vector<std::vector<int>> Solve();
	void PrintSolution(const std::vector<std::vector<int>>& solution) const;


};