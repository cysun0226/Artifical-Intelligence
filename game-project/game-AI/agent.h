/* AI game project */
// by cysun

#ifndef __AGENT_H__
#define __AGENT_H__


#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "chessboard.h"
#include "debug.h"

// LABEL Class Agent
class Agent {
public:
  Agent(int team_number);
	// Get the next move and write it into move file.
	void nextMove();

	// If the valid position is not empty, it is my turn.
	bool isMyTurn();

  bool isGameStop();
	int isFirstWinner();
  vector<int> get_board();
  void processStateInfo();

private:

  virtual ~Agent() {}

  // Write my move into move file.
	int _getNextMove();
	void _writeMove(int pos);

	// Get current board. 0: valid pos, 1: your pos, 2: opponent pos
	// A list of int. e.g. [0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 2, 2, 0, 2...]
	bool readState();
  int minimax(int new_pos, int depth);

	/**
	* Are you the winner of first game?
	* Return -1: the first game is still continuing.
	*         1: you are the winner of first game.
	*         0: you are not.
	*/

	int team_number;
	string state_file, move_file;
	int prev_move, cur_move, next_first_move;
	int first_winner;
	bool game_stop;
  bool new_state;

	vector<int> board, valid_pos, prev_board;
  ChessBoard chessboard;
};


#endif
