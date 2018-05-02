/* AI game project */
// by cysun
// adopted from the sample code

#include <climits>
#include "agent.h"
#include "debug.h"

#define MINIMAX_DEPTH 1

Agent::Agent(int team_number):
  team_number(team_number),
  cur_move(-1),
  next_first_move(-1),
  first_winner(-1),
  game_stop(false)
  {
    this->state_file = "state_" + to_string(team_number) + ".txt";
    this->move_file = "move_" + to_string(team_number) + ".txt";
  }

void Agent::processStateInfo() {
  if(readState()) {
    chessboard.set_board(board);
    chessboard.update();
  }
}

// I/O
bool isEmpty(std::ifstream& pFile) {
	return pFile.peek() == std::ifstream::traits_type::eof();
}

bool Agent::readState() {
  ifstream infile(state_file);
	if (!infile.is_open()) return false;
	if (isEmpty(infile)) return false;

	int move;
	infile >> move;

	board.clear();
	valid_pos.clear();

	int x;
	for (int i = 0; i < 217; ++i) {
		infile >> x;
		board.push_back(x);
	}

  if (move == prev_move)
    return false;

  // waitKey();
  cout << "start reading new state..." << endl;
  prev_move = move;

	infile >> first_winner;
	infile.close();

	if (move == -100) {
		game_stop = true;
		return true;
	}

	if (move > cur_move ||
		(move == next_first_move && cur_move != next_first_move)) {
		// If we are making the first move of first game,
		// record the first move of the second game
		if (cur_move == -1) next_first_move = move == 1 ? 2 : 1;

		// Record current move
		cur_move = move;

		for (int i = 0; i < 217; ++i) {
			if (board[i] == EMPTY)
				valid_pos.push_back(i);
		}
	}

  return true;
}

int Agent::_getNextMove() {
  int next_move = -1;
  int max_utility = INT_MIN;
  for (size_t i = 0; i < valid_pos.size(); i++) {
    int new_utility = minimax(valid_pos[i], MINIMAX_DEPTH);
    if ( new_utility > max_utility) {
      max_utility = new_utility;
      next_move = valid_pos[i];
    }
  }
  return next_move;
}

void Agent::nextMove() {
  int pos = _getNextMove();
  _writeMove(pos);
}

void Agent::_writeMove(int pos) {
	ofstream outfile(move_file);
	outfile << cur_move << " " << pos;
  cout << "move " << cur_move  << ", pos = " << pos << endl;
	outfile.close();
}

// judging functions
bool Agent::isMyTurn() {
  return valid_pos.size() != 0;
}

bool Agent::isGameStop() {
  return game_stop;
}

// LABEL minimax
int Agent::minimax(int new_pos, int depth) {
  return 0;
}

// int minimax(int new_pos, int depth) {
//   int best_utility;
//   chessboard.put_one(new_pos);
//   chessboard.update();
//   // child of current start_state
//   std::vector<int> valid_pos = chessboard.get_valid_pos();
//
//   if (start_state is win or lose || depth == 0 ) {
//     chessboard.remove_one(new_pos);
//     return chessboard.get_utility();
//   }
//
//   if (state == opponent) {
//     best_utility = INT_MAX;
//     for (int i = 0; i < valid_pos.size(); i++) {
//       best_utility = min(best_utility, minimax(valid_pos[i], depth-1));
//     }
//   }
//   else { // our turn
//     best_utility = INT_MIN;
//     for (int i = 0; i < valid_pos.size(); i++) {
//       best_utility = max(best_utility, minimax(valid_pos[i], depth-1));
//     }
//   }
//
//   chessboard.remove_one(new_pos);
//   return best_utility;
// }
