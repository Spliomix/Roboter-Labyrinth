#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
using namespace std;


void print(vector<string> input) {
	for (const auto &elem : input) {
		cout << elem << '\n';
	}cout << "ausgabe" << '\n';

}
struct used {
	bool top_used{ false };
	bool right_used{ false };
	bool bot_used{ false };
	bool left_used{ false };
	void reset() {
		top_used = false ;
		right_used=false ;
		bot_used=false ;
		left_used=false;
	}
};
struct feld {
	size_t x;
	size_t y;
	feld(size_t a, size_t b, bool used) {
		x = a;
		y = b;
		hash_tag = used;
	}
	bool top_used{ false };
	bool right_used{ false };
	bool bot_used{ false };
	bool left_used{ false };
	bool hash_tag{ false };
	bool used() {
		return top_used && right_used && top_used && right_used;
	}

};

struct Maze {
	vector<vector<shared_ptr<feld>>> data;//inhalt des mazes
	Maze();
	size_t max_row;
	size_t max_coll;
	Maze(vector<string> input) {//erstellt alle felder für das maze und setzt die hashtags#### auf used
		max_row = input.size() - 1;
		max_coll = input.at(0).size() - 1;
		for (size_t i = 0; i < input.size(); ++i) {
			vector<shared_ptr<feld>> temp;
			for (size_t j = 0; j < input.at(i).size();++j) {
				temp.push_back(shared_ptr<feld> (new feld(j, i, input.at(i).at(j)=='#' ? true : false)));
			}
			data.push_back(temp);
		}
		print();
	}
	void print() {
		for (const auto& elem : data) {
			for (const auto& elem2 : elem) {
				if (elem2->hash_tag)cout << '#';
				else cout << ' ';
			}
			cout << '\n';
		}
	}
	feld get(size_t x, size_t y) {
		if (x > max_row)cout << "Error x";
		if (y > max_coll)cout << "Error y";
		return *(data.at(y).at(x));
	}
};


class roboter  {
public://varibalen public ==== illegal
	size_t steps{ 0 };
	vector<string> maze;
	shared_ptr<Maze>maze2;
	size_t max_row, max_coll {0};

	roboter();
	void constructor(vector<string> maze_in) {
		maze = maze_in;
		maze2 = shared_ptr<Maze>(new Maze(maze_in));
		max_row = maze.size() - 1;
		max_coll = maze.at(0).size() - 1;
		start_moving(find(true), find(false));
		cout << steps;
	}
	roboter(vector<string> maze_in) {
		constructor(maze_in);
	}
	pair<size_t, size_t> find(bool start) {//wenn start true wird das erste returned
		bool flag = start;//flag wird auf true gesetzt wenn die nächste freie Stelle returend werden muss
		for (size_t i = 0; i<maze.at(0).size(); ++i) {//erste Zeile von links nach rechts
			if (maze.at(0).at(i) == ' ') {
				if (flag)return make_pair(0, i);
				flag = true;
			}
		}

		for (size_t i = 0; i < maze.size(); ++i) {//letze Spalte von oben nach unten
			if (maze.at(i).at(max_coll) == ' ') {
				if (flag)return make_pair(i, max_coll);
				flag = true;
			}
		}

		for (size_t i = max_coll; i > 0; --i) {//letzte zeile von rechts nach links
			if (maze.at(max_row).at(i) == ' ') {
				if (flag) return make_pair(max_row, i);
				flag = true;
			}
		}

		for (size_t i = max_row; i > 0; --i) {//erste Spalte von unten nach oben
			if (maze.at(i).at(0) == ' ') {
				if (flag) return make_pair(i, 0);
				flag = true;
			}
		}
	}

	void start_moving(pair<size_t, size_t> start, pair<size_t, size_t> end) {//rechtsseitig
		size_t x = start.first;
		size_t y = start.second;
		feld startLoc(start.second, start.first, false);
		feld endLoc(end.second, end.first, false);
		cout<< move(&startLoc, endLoc, shared_ptr<used>(new used()));
	}

	virtual bool move(feld* loc, feld end, shared_ptr<used>before);
};

class homobot :public roboter{
	bool move(feld* loc, feld end, shared_ptr<used>before) {//location
		shared_ptr<used> temp_used = shared_ptr<used>(new used());
		if ((loc->x == end.x && loc->y == end.y))return true;
		if (loc->hash_tag)return false;
		++steps;
		if (!loc->right_used && !(before->left_used) && (loc->x + 1) <= max_coll) {
			loc->right_used = true;
			temp_used->right_used = true;
			if (move(&(maze2->get(loc->x + 1, loc->y)), end, temp_used))return true;
			temp_used->reset();
		}

		if (!loc->bot_used && !(before->top_used) && (loc->y + 1) <= max_row) {
			loc->bot_used = true;
			temp_used->bot_used = true;
			if (move(&(maze2->get(loc->x, loc->y + 1)), end, temp_used))return true;
			temp_used->reset();
		}

		if (!loc->left_used && !(before->right_used) && (loc->x - 1) <= max_coll) {
			loc->left_used = true;
			temp_used->left_used = true;
			if (move(&(maze2->get(loc->x - 1, loc->y)), end, temp_used))return true;
			temp_used->reset();
		}

		if (!loc->top_used && !(before->bot_used) && (loc->y - 1) <= max_row) {
			loc->top_used = true;
			temp_used->top_used = true;;
			if (move(&(maze2->get(loc->x, loc->y - 1)), end, temp_used))return true;
			temp_used->reset();
		}
		return false;
	}
};

class homobot2 :public roboter {
	homobot2(vector<string> mace) {
		auto robi = roboter();
		robi.constructor(mace);
	}
	bool move(feld* loc, feld end, shared_ptr<used>before) {//location
		shared_ptr<used> temp_used = shared_ptr<used>(new used());
		if ((loc->x == end.x && loc->y == end.y))return true;
		if (loc->hash_tag)return false;
		++steps;
		if (!loc->right_used && !(before->left_used) && (loc->x + 1) <= max_coll) {
			loc->right_used = true;
			temp_used->right_used = true;
			if (move(&(maze2->get(loc->x + 1, loc->y)), end, temp_used))return true;
			temp_used->reset();
		}

		if (!loc->bot_used && !(before->top_used) && (loc->y + 1) <= max_row) {
			loc->bot_used = true;
			temp_used->bot_used = true;
			if (move(&(maze2->get(loc->x, loc->y + 1)), end, temp_used))return true;
			temp_used->reset();
		}

		if (!loc->left_used && !(before->right_used) && (loc->x - 1) <= max_coll) {
			loc->left_used = true;
			temp_used->left_used = true;
			if (move(&(maze2->get(loc->x - 1, loc->y)), end, temp_used))return true;
			temp_used->reset();
		}

		if (!loc->top_used && !(before->bot_used) && (loc->y - 1) <= max_row) {
			loc->top_used = true;
			temp_used->top_used = true;;
			if (move(&(maze2->get(loc->x, loc->y - 1)), end, temp_used))return true;
			temp_used->reset();
		}
		return false;
	}
};

int main()
{
	vector<string> maze;
	string line;
	ifstream myfile("../maze3_braid.txt");
	if (myfile.is_open()){
		while (getline(myfile, line)){
			maze.push_back(line);
		}
		myfile.close();
		roboter robo1(maze);
	}
	else cout << "Unable to open file";
	//print(maze);
	return 0;
}



