/* AI game project */
// by cysun
// adopted from the sample code

#include <climits>
#include "agent.h"
#include "debug.h"
#include <sstream>

#define MINIMAX_DEPTH 1

Agent::Agent(int team_number):
  team_number(team_number),
  cur_move(-1),
  next_first_move(-1),
  first_winner(-1),
  game_stop(false)
  {
    stringstream ss;
    ss << team_number;
    string convert_str;
    ss >>  convert_str;

    this->state_file = "state_" + convert_str + ".txt";
    this->move_file = "move_" + convert_str + ".txt";
    // this->state_file = "state_" + to_string(team_number) + ".txt";
    // this->move_file = "move_" + to_string(team_number) + ".txt";
  }

void Agent::processStateInfo() {
  if(readState()) {
    chessboard.set_board(board);
    cout << "update chessboard...\n" << endl;
    cout << " - count connective lines..." << endl;
    chessboard.update();
    cout << " - finish analyze lines.\n" << endl;
  }
}

// I/O
bool isEmpty(std::ifstream& pFile) {
	return pFile.peek() == std::ifstream::traits_type::eof();
}

bool Agent::readState() {
  ifstream infile(state_file);
	if (!infile.is_open())
    return false;
  // cout << "file exist" << endl;
  // waitKey();
	if (isEmpty(infile))
    return false;
  // cout << "file not empty" << endl;
  // waitKey();

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
  cout << "\nstart reading new state, move = " << move <<"..." << endl;
  if (move == 1 || move == 2) {
    chessboard.initialize();
  }
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
  return minimax(MINIMAX_DEPTH);
  // int next_move = -1;
  // int max_utility = INT_MIN;
  // for (size_t i = 0; i < valid_pos.size(); i++) {
  //   int new_utility = minimax(valid_pos[i], MINIMAX_DEPTH);
  //   if ( new_utility > max_utility) {
  //     max_utility = new_utility;
  //     next_move = valid_pos[i];
  //   }
  // }
  // return next_move;
  // return valid_pos[0];
}

void Agent::nextMove() {
  int pos = _getNextMove();
  _writeMove(pos);
}

void Agent::_writeMove(int pos) {
	ofstream outfile(move_file);
	outfile << cur_move << " " << pos;
  cout << " > move " << cur_move  << ", pos = " << pos << endl;
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
int Agent::minimax(int depth) {
  ActAndUtil result = _minimax_impl(chessboard,
                                    depth,
                                    INT_MIN+1,
                                    INT_MAX,
                                    ME);

  return result.action;
}

ActAndUtil Agent::_minimax_impl(ChessBoard& s, int depth, float alpha, float beta, int color) {
  /* Implementation of negamax with alphe-beta pruning
   * Args:
   *   s: current board state
   *   depth: remained search depth
   *   alpha: lower bound for pruning
   *   beta: upper bound for pruning
   *   color: 1: player max, -1: player min
   *
   * Return:
   *   A struct of type `ActAndUtil` containing the best possible move
   *   and the heuristic board utility following this move
   */

  if (depth == 0 || s.win || s.lose) {
    return ActAndUtil(-1, color*s.get_utility());
  }

  // float best_util = std::numeric_limits<float>::min();
  int best_util = INT_MIN+1;
  int best_action = -1;
  int player = (color == 1)? ME : OPPONENT;

  // iterate through reasonable moves
  std::vector<int> reasonable_moves = s.get_valid_pos();
  // printVector(reasonable_moves, "reasonable_moves");
  // waitKey();
  for (int i = 0; i < reasonable_moves.size(); i++) {
    // cout << "put_one" << endl;
    s.put_one(reasonable_moves[i], player);
    // cout << "update_one" << endl;
    s.update();

    int return_util = -1 * _minimax_impl(s, depth-1, -beta, -alpha, -color).util;
    if (return_util > best_util) {
      best_util = return_util;
      best_action = reasonable_moves[i];
    }
    alpha = return_util > alpha? return_util : alpha;
    if (alpha >= beta)
      break;

    // cout << "remove_one" << endl;
    s.remove_one(reasonable_moves[i]);
    // cout << "update_one" << endl;
    s.update();
  }

  return ActAndUtil(best_action, best_util);
}

ActAndUtil::ActAndUtil(int act, int u) : util(u), action(act) {}

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
