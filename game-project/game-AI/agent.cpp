/* AI game project */
// by cysun
// adopted from the sample code

#include "agent.h"

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
  readState();
  chessboard = ChessBoard(board);
}

// I/O
bool isEmpty(std::ifstream& pFile) {
	return pFile.peek() == std::ifstream::traits_type::eof();
}

void Agent::readState() {
  ifstream infile(state_file);
	if (!infile.is_open()) return;
	if (isEmpty(infile)) return;

	int move;
	infile >> move;

	board.clear();
	valid_pos.clear();

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
}

int Agent::_getNextMove() {
  // int next = minimax(&ChessBoard);
  // return minimax(&ChessBoard);
  int next = 0;
  return next;
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
