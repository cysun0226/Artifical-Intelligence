// This is a template helping you to do program assignment2.
// We implemented the file I/O part.
// You may focus on the method, _getNextMove(),
// which is a method to decide where to place your stone based on the current
// game state, including (1) valid position, (2) your position, (3) your
// opponent position, (4) board and (5) the winner of first game.
// !! Remember to change the team number in main() !!

//#include "stdafx.h"
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Agent {
	/**
	* Game agent.
	* The procedure:
	* 1. process_state_info: check game state
	* 2. is_my_turn: if it is my turn, decide a position to put stone
	* 3. step: call get_next_move and write the result to move file
	*/

public:
	Agent(int team_number)
		: team_number(team_number),
		cur_move(-1),
		next_first_move(-1),
		first_winner(-1),
		game_stop(false)
	{
		this->state_file = "state_" + to_string(team_number) + ".txt";
		this->move_file = "move_" + to_string(team_number) + ".txt";
	}

	/**
	* Read state file and get valid position.
	* If not my turn to make move, return an empty list.
	*/
	void processStateInfo();

	/**
	*  Get the next move and write it into move file.
	*/
	void step();

	/**
	* If the valid position is not empty, it is my turn.
	*/
	bool isMyTurn();

	bool isGameStop();

	int isFirstWinner();

private:
	/**
	* Get a position from valid_pos randomly.
	* You should implement your algorithm here.
	*
	* These utilities may be helpful:
	* getValidPos()
	* getMyPos()
	* getOpponentPos()
	* getBoard()
	*
	* Check them below for more detail
	*/
	int _getNextMove();

	/**
	* Write my move into move file.
	*/
	void _writeMove(int pos);

	/**
	* Get the valid position where you can put your stone.
	* A list of int. e.g. [0, 1, 3, 5, 6, 9, 12,...]
	*/
	vector<int> getValidPos();

	/**
	* Get the position where you have put.
	* A list of int. e.g. [2, 4, 7, 8, ...]
	*/
	vector<int> getMyPos();

	/**
	* Get the position where your opponent have put.
	* A list of int. e.g. [10, 11, 13, ...]
	*/
	vector<int> getOpponentPos();

	/**
	* Get current board. 0: valid pos, 1: your pos, 2: opponent pos
	* A list of int. e.g. [0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 2, 2, 0, 2...]
	*/
	vector<int> getBoard();

	/**
	* Are you the winner of first game?
	* Return -1: the first game is still continuing.
	*         1: you are the winner of first game.
	*         0: you are not.
	*/

	int team_number;
	string state_file, move_file;
	int cur_move, next_first_move;
	int first_winner;

	bool game_stop;

	vector<int> valid_pos, my_pos, opponent_pos, board;
};

int main() {
	//srand(time(NULL));

	// change the number to your team number.
	Agent* agent = new Agent(2);

	while (true) {
		agent->processStateInfo();
		if (agent->isGameStop()) break;
		if (agent->isMyTurn()) agent->step();
	}

	free(agent);
	return 0;
}

bool isEmpty(std::ifstream& pFile) {
	return pFile.peek() == std::ifstream::traits_type::eof();
}

void Agent::processStateInfo() {
	ifstream infile(state_file);
	if (!infile.is_open()) return;
	if (isEmpty(infile)) return;

	int move;
	infile >> move;

	board.clear();
	valid_pos.clear();
	my_pos.clear();
	opponent_pos.clear();

	int x;
	for (int i = 0; i < 217; ++i) {
		infile >> x;
		board.push_back(x);
	}

	infile >> first_winner;
	infile.close();

	if (move == -100) {
		game_stop = true;
		return;
	}

	// The only condition of move read from state file being less than our
	// record move (cur_move) is that the second game starts.
	// So, if move is less than cur_move and is not next_first_move,
	// just skip it
	if (move > cur_move ||
		(move == next_first_move && cur_move != next_first_move)) {
		// If we are making the first move of first game,
		// record the first move of the second game
		if (cur_move == -1) next_first_move = move == 1 ? 2 : 1;

		// Record current move
		cur_move = move;

		for (int i = 0; i < 217; ++i) {
			if (board[i] == 0)
				valid_pos.push_back(i);
			else if (board[i] == 1)
				my_pos.push_back(i);
			else if (board[i] == 2)
				opponent_pos.push_back(i);
		}
	}
}

void Agent::step() {
	int pos = _getNextMove();
	_writeMove(pos);
}

int Agent::_getNextMove() { return valid_pos[rand() % valid_pos.size()]; }

void Agent::_writeMove(int pos) {
	ofstream outfile(move_file);
	outfile << cur_move << " " << pos;
	outfile.close();
}

bool Agent::isMyTurn() { return valid_pos.size() != 0; }

bool Agent::isGameStop() { return game_stop; }

vector<int> Agent::getValidPos() { return valid_pos; }

vector<int> Agent::getMyPos() { return my_pos; }

vector<int> Agent::getOpponentPos() { return opponent_pos; }

vector<int> Agent::getBoard() { return board; }

int Agent::isFirstWinner() { return first_winner; }