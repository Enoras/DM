#pragma once
#include <vector>
#include <string>
#include <set>

#define START_POINT_VAL 2
#define WALL 0
#define PATH 1

class Maze {
private:
	
	std::vector<int> _start;
private:
	std::vector<int> Split(const char* str, char c = ' ');

public:
	std::vector<std::vector<int>> _maze;
	Maze() {};
	Maze(const std::string& file_name) { LoadFile(file_name); };
	Maze(const std::vector<std::vector<int>>& maze, const std::vector<int>& start) :_maze(maze), _start(start) {};
	void LoadFile(const std::string& file_name);
	void Print() const;
	bool NotWall(int x, int y) const;
	bool IsExit(int x, int y) const;
	bool WithinOneToExit(int x, int y) const;
	std::set<std::vector<int>> UnblockedNeighbors(int x, int y) const;
	std::vector<int> GetStart() const { return _start; };
	bool InsideMaze(int x, int y) const;
	std::vector<std::vector<int>> GetGrid() const { return _maze; };
};

