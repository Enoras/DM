#include "libMaze.h"
#include <iostream>
#include <fstream>
#include <exception>

using namespace std;

vector<int> Maze::Split(const char* str, char c)
{
	vector<int> result;
	do
	{
		const char* begin = str;

		while (*str != c && *str)
			str++;

		result.push_back(stoi(string(begin, str)));
	} while (0 != *str++);

	return result;
}



void Maze::Print() const {
	for (auto& stroke : _maze) {
		for (auto& val : stroke)
			cout << val << ' ';
		cout << endl;
	}
};


void Maze::LoadFile(const string& file_name) {
	fstream file(file_name);
	if (!file.is_open()) throw exception("file not found");
	_maze.clear();
	string stroke;
	while (getline(file, stroke)) {
		auto line = Split(stroke.c_str());
		if (_start.empty()) {
			for (int i = 0; i < line.size(); i++) {
				if (line[i] == START_POINT_VAL) {
					_start.push_back(_maze.size());
					_start.push_back(i);
					line[i] = PATH;
				}
	
			}
		}
		_maze.push_back(line);
	}


	size_t cur_line_len = _maze[0].size();
	for (const auto& line : _maze) {
		auto next_line_len = line.size();
		if (cur_line_len != next_line_len)
			throw exception("not rectangular maze");
		cur_line_len = next_line_len;
	}


	file.close();
}


bool Maze::InsideMaze(int x, int y) const {
	return (x < _maze.size() && x >= 0) && 
		   (y < _maze[0].size() && y >= 0);
}

bool Maze::NotWall(int x, int y) const{

	return InsideMaze(x,y) && _maze[x][y] == PATH;
	
}


bool Maze::WithinOneToExit(int x, int y) const {
	return IsExit(x + 1, y) || IsExit(x - 1, y) ||
		   IsExit(x, y + 1) || IsExit(x, y - 1);

}

bool Maze::IsExit(int x, int y) const {
	return NotWall(x, y) &&
			(x == 0 || y == 0 ||
			x == _maze.size() - 1 ||
			y == _maze[0].size() - 1);
}


std::set<std::vector<int>> Maze::UnblockedNeighbors(int x, int y) const {
	set<vector<int>> neighbors;

	if (InsideMaze(x + 2,y) && NotWall(x + 1,y) && NotWall(x + 2, y))
		neighbors.insert({x+2,y});
	if (InsideMaze(x - 2, y) && NotWall(x - 1, y) && NotWall(x - 2, y))
		neighbors.insert({ x - 2,y });
	if (InsideMaze(x, y + 2) && NotWall(x, y - 1) && NotWall(x, y - 2))
		neighbors.insert({ x ,y - 2 });
	if (InsideMaze(x, y - 2) && NotWall(x, y + 1) && NotWall(x, y + 2))
		neighbors.insert({ x ,y + 2 });

	return neighbors;
}
