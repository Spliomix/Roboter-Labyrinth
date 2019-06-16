#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
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
	bool used{ false };

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
temp.push_back(shared_ptr<feld>(new feld(j, i, input.at(i).at(j) == '#' ? true : false)));
			}
			data.push_back(temp);
		}
	}
	void print() {
		for (const auto& elem : data) {
			for (const auto& elem2 : elem) {
				if (elem2->hash_tag)cout << '#';
				else if (elem2->used)cout << ".";
				else cout << ' ';
			}
			cout << endl;
		}
	}
	shared_ptr<feld> get(size_t x, size_t y) {		//gibt feld zurück von dem die cords eingelesen werden
		if (data.at(y).at(x)->hash_tag)return nullptr;
		return (data.at(y).at(x));
	}
};


class roboter {			//erstellen  abstrakte klasse für unter roboter
public://varibalen public ==== illegal
	size_t steps{ 0 };
	vector<string> maze;		//anfangs und Endpunktfinden nötige MAZE
	shared_ptr<Maze>maze2;		//Maze mit dem gearbeitet wird speichern und bearbeiten
	size_t max_row, max_coll{ 0 };
	roboter() {};				//default
	void constructor(vector<string> maze_in) {			//liest maze ein, erstellt, maxrow maxcol init, start function moving
		maze = maze_in;
		maze2 = shared_ptr<Maze>(new Maze(maze_in));
		max_row = maze.size() - 1;
		max_coll = maze.at(0).size() - 1;
		start_moving(find(true), find(false));
	}
	roboter(vector<string> maze_in) {			//constructor 
		constructor(maze_in);
	}
	void print() {				//steps funcition zeichnet maze
		maze2->print();
		cout << steps << "\n";
	};
	pair<size_t, size_t> find(bool start) {//wenn start true wird das erste returned
		bool flag = start;//flag wird auf true gesetzt wenn die nächste freie Stelle returend werden muss
		for (size_t i = 0; i < maze.at(0).size(); ++i) {//erste Zeile von links nach rechts
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
		feld endLoc(end.second, end.first, false);
		move(maze2->get(start.second, start.first), maze2->get(end.second, end.first), shared_ptr<used>(new used()));
	}
	virtual bool move(shared_ptr<feld> loc, shared_ptr<feld> end, shared_ptr<used>before) { return false; };
};

class homobot :public roboter {
public:
	homobot() :roboter() {};
	bool move(shared_ptr<feld> loc, shared_ptr<feld> end, shared_ptr<used>before) {//location
		shared_ptr<used> temp_used = shared_ptr<used>(new used());
		if ((loc->x == end->x && loc->y == end->y)) {
			loc->used = true;
			return true;
		}													//bewegung , schaut ob frei , markiert wenn besucht,zählt die steps hoch

		loc->used = true;

		if (!loc->left_used && !(before->right_used) && (loc->x - 1) <= max_coll) {
			loc->left_used = true;
			temp_used->left_used = true;
			++steps;
			auto field_ptr = maze2->get(loc->x - 1, loc->y);
			if (field_ptr != nullptr){//hashtag
				if (move(field_ptr, end, temp_used))return true;
				++steps;
			}
			temp_used->reset();

		}

		if (!loc->right_used && !(before->left_used) && (loc->x + 1) <= max_coll) {
			loc->right_used = true;
			temp_used->right_used = true;
			++steps;
			auto field_ptr = maze2->get(loc->x + 1, loc->y);
			if (field_ptr != nullptr) {//hashtag
				if (move(field_ptr, end, temp_used))return true;
				++steps;
			}
			temp_used->reset();
		}

		if (!loc->top_used && !(before->bot_used) && (loc->y - 1) <= max_row) {
			loc->top_used = true;
			temp_used->top_used = true;
			++steps;
			auto field_ptr = maze2->get(loc->x, loc->y-1);
			if (field_ptr != nullptr) {//hashtag
				if (move(field_ptr, end, temp_used))return true;
				++steps;
			}
			temp_used->reset();

		}

		if (!loc->bot_used && !(before->top_used) && (loc->y + 1) <= max_row) {
			loc->bot_used = true;
			temp_used->bot_used = true;
			++steps;
			auto field_ptr = maze2->get(loc->x, loc->y + 1);
			if (field_ptr != nullptr) {//hashtag
				if (move(field_ptr, end, temp_used))return true;
				++steps;
			}
			temp_used->reset();

		}
		return false;
	}
};

class homobot2 :public roboter {					//erbt vom roboter
public:												
	homobot2() :roboter() {};						//überladen durch gleichbennenung der function; parameter bleiben dieselben
	bool move(shared_ptr<feld> loc, shared_ptr<feld> end, shared_ptr<used>before) {//location
		shared_ptr<used> temp_used = shared_ptr<used>(new used());
		if ((loc->x == end->x && loc->y == end->y)) {
			loc->used = true;
			return true;
		}

		loc->used = true;
		if (!loc->right_used && !(before->left_used) && (loc->x + 1) <= max_coll) {
			loc->right_used = true;
			temp_used->right_used = true;
			++steps;
			auto field_ptr = maze2->get(loc->x+1, loc->y);
			if (field_ptr != nullptr) {//hashtag
				if (move(field_ptr, end, temp_used))return true;
				++steps;
			}
			temp_used->reset();
		}
		
		if (!loc->bot_used && !(before->top_used) && (loc->y + 1) <= max_row) {
			loc->bot_used = true;
			temp_used->bot_used = true;
			++steps;
			auto field_ptr = maze2->get(loc->x, loc->y + 1);
			if (field_ptr != nullptr) {//hashtag
				if (move(field_ptr, end, temp_used))return true;
				++steps;
			}
			temp_used->reset();

		}

		if (!loc->left_used && !(before->right_used) && (loc->x - 1) <= max_coll) {
			loc->left_used = true;
			temp_used->left_used = true;
			++steps;
			auto field_ptr = maze2->get(loc->x-1, loc->y);
			if (field_ptr != nullptr) {//hashtag
				if (move(field_ptr, end, temp_used))return true;
				++steps;
			}
			temp_used->reset();

		}

		if (!loc->top_used && !(before->bot_used) && (loc->y - 1) <= max_row) {
			loc->top_used = true;
			temp_used->top_used = true;
			++steps;
			auto field_ptr = maze2->get(loc->x, loc->y - 1);
			if (field_ptr != nullptr) {//hashtag
				if (move(field_ptr, end, temp_used))return true;
				++steps;
			}
			temp_used->reset();
		}
		return false;
	}
};

int main()
{


	homobot2 * tux2 = new homobot2();				//erstellen der objecte
	roboter * robo2 = tux2;

	homobot * tux = new homobot();
	roboter * robo = tux;

	vector<string> maze;						//init
	string line;
	ifstream myfile("../maze5_cavern.txt");		//read file and speichern myfile
	if (myfile.is_open()){
		while (getline(myfile, line)){
			maze.push_back(line);
		}
		myfile.close();
	}
	else cout << "Unable to open file";			//file error
	thread t1(&homobot::constructor, robo, maze);		//threads gleichzeitig 
	thread t2(&homobot2::constructor, robo2, maze);
	t1.join();                // pauses until first finishes	
	t2.join();               // pauses until second finishes




	robo->print();			// robo ruft maze print auf  und gibt steps aus print->printmaze
	robo2->print();		
	while (1);			//while(1) sonst beendet sich es programm zu schnell 
	delete tux;			//delet objects
	delete tux2;
	return 0;
}



