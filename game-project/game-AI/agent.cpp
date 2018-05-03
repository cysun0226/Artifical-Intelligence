/* AI game project */
// by cysun
// adopted from the sample code

#include <climits>
#include "agent.h"
#include "debug.h"
#include <sstream>

#define MINIMAX_DEPTH 5

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
    chessboard.update();
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
  if (move == 1 || move == 2) {
    cout << "\n\n=== game start ===\n" << endl;
    chessboard.initialize();
  }

  if (move == -100)
    cout << "\n\n=== game finish ===\n" << endl;

  cout << "\nstart reading new state, move = " << move <<"..." << endl;
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
  if (cur_move < 4) {
    if (board[108] == EMPTY)
      return 108;
    else {
      std::vector<Block*> n = chessboard.get_block(108).neighbors;
      for (int i = 0; i < n.size(); i++) {
        if (n[i]->state == EMPTY) {
          if (i == 0 || i == 2 || i == 4) {
            if (n[i+1]->state == EMPTY)
              return n[i]->id;
          }
          if (n[i-1]->state == EMPTY)
            return n[i]->id;
        }
      }
    }
  }

  // if (!chessboard.critical_moves.empty()) {
  //   cout << "move " << cur_move;
  //   cout << " - critical_pos[0] = " << chessboard.critical_moves.front() << endl;
  //   printVector(chessboard.critical_moves, "critical_moves");
  //   return chessboard.critical_moves.front();
  // }
  if (chessboard.critical_move != -1) {
    cout << "move " << cur_move;
    cout << " critical_pos = " << chessboard.critical_move << endl;
    return chessboard.critical_move;
  }

  return minimax(MINIMAX_DEPTH);

}

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
    s.update_one(reasonable_moves[i]);

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
    s.update_one(reasonable_moves[i]);
  }

  if (best_action == -1) {
    cout << "cur_move = " << cur_move << endl;
    cout << "depth = " << cur_move << endl;
  }
  return ActAndUtil(best_action, best_util);
}

ActAndUtil::ActAndUtil(int act, int u) : util(u), action(act) {}
