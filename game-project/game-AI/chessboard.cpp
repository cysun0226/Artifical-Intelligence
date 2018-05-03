/* AI game project */
// by cysun, Americano Ho and Kao Chu Ching

#include <iterator>
#include "chessboard.h"
#include "debug.h"
#include <algorithm>

/* struct */
// constructor
Coordinate::Coordinate() {}
Coordinate::Coordinate(int r, int c) : row(r), col(c) {}

// overload << (for cout)
std::ostream& operator << (std::ostream &o, const Coordinate& p) {
    return o << "(" << setw(2) << p.row << ", " << setw(2) << p.col << ")";
}

// constructor
Block::Block(){}
Block::Block(int id, Coordinate x, Coordinate y, Coordinate z, STATE s) :
id(id), x(x), y(y), z(z), state(s) {}

// overload << (for cout)
std::ostream& operator << (std::ostream &o, const Block& p) {
  string s;
  s = (p.state == ME)? "ME" : (p.state == OPPONENT)? "OPPONENT" : "EMPTY";
    return o << "id: " << setw(3) << p.id  <<
                " | pos: x" << p.x << "  y" <<  p.y << "  z" << p.z <<
                " | state: " << s;
}

// constructor LABEL Line
Line::Line() {}
Line::Line(vector<Block*> b, int l, int s, int ss) :
length(l), state(s), status(ss) {
    block.assign(b.begin(), b.end());
}
std::ostream& operator << (std::ostream &o, const Line& p) {
    string s = (p.state == ME)? "ME" : (p.state == OPPONENT)? "OPPONENT" : "EMPTY";
    string ss = (p.status == OPEN)? "OPEN" :
                (p.status == HALF_OPEN)? "HALF_OPEN" : "BLOCK";
    return o << "\nhead = " << *p.block[0] <<
                "\nlength = " << p.length << ", state = " << s <<
                ", status = " << ss << endl;
}

/* chessboard */
// LABEL Chess-Board
ChessBoard::ChessBoard() { initialize(); }
ChessBoard::ChessBoard(std::vector<int> b) {
  initialize();
}

void ChessBoard::set_board(std::vector<int> v) {
  board.assign(v.begin(), v.end());
  // cout << "set board" << endl;
}

std::vector<int> ChessBoard::get_valid_pos() {
  return valid_pos;
}

std::vector<int> ChessBoard::get_critical_moves() {
  return critical_moves;
}

Block ChessBoard::get_block(int id) {
  return block[id];
}

// LABEL CB::init
void ChessBoard::initialize()
{
  cout << "\ninitialize chessboard in agent...\n" << endl;

  win = false;
  lose = false;
  last = -1;
  second_last = -1;
  // opponent_lines = vector<int>(6,0);
  // my_lines = vector<int>(6,0);

  for (size_t i = 0; i < 217; i++) {
    block[i].state = EMPTY;
    block[i].id = i;
  }

  // x, y, z
  std::vector<int> shift, z_bound;
  // shift.push_back(0);
  z_bound.push_back(0);
  for (int i = 9; i <= 17; i++) {
    shift.push_back(i);
    z_bound.push_back(z_bound.back()+i);
  }
  for (int i = 16; i >= 9; i--){
    shift.push_back(i);
    z_bound.push_back(z_bound.back()+i);
  }

  for (size_t r = 0; r <= 8; r++) {
    for (size_t c = 0; c < shift[r]; c++) {
      int x_tmp = (c>r)? z_bound[(8-r)+c] + r : z_bound[(8-r)+c] + c;
      block[x_tmp].x = Coordinate(r, c);
      x_map[block[x_tmp].x] = x_tmp;

      int y_tmp = (c >= 9)? z_bound[c] + r - (c-8)  : r+z_bound[c];
      block[y_tmp].y = Coordinate(r, c);
      y_map[block[y_tmp].y] = y_tmp;

      block[z_bound[r]+c].z = Coordinate(r, c);
      z_map[block[z_bound[r]+c].z] = z_bound[r]+c;
      // cout << "block[" << z_bound[r]+c << "] = " << block[z_bound[r]+c].z << endl;
    }
  }

  layer.assign(shift.begin(), shift.end());
  // printVector(shift, "shift");
  // printVector(z_bound, "z_bound");

  for (size_t r = 1; r <= 8; r++) {
    for (size_t c = 0; c < shift[8-r]; c++) {
      int x_tmp = (c>=8)? z_bound[c]+r+8 : z_bound[c]+r+c;
      block[x_tmp].x = Coordinate(r+8, c);
      x_map[block[x_tmp].x] = x_tmp;
      int y_tmp = ((c+r)>=9)? z_bound[c+r+1] - (9-r) : z_bound[c+r] + (r+8);
      block[y_tmp].y = Coordinate(r+8, c);
      y_map[block[y_tmp].y] = y_tmp;

      block[z_bound[r+8]+c].z = Coordinate(r+8, c);
      z_map[block[z_bound[r+8]+c].z] = z_bound[r+8]+c;
    }
  }

  // neighbors
  for (size_t i = 0; i < 217; i++) {
    Coordinate x_tmp(block[i].x), y_tmp(block[i].y), z_tmp(block[i].z);

    if (block[i].x.col > 0)
      block[i].neighbors.push_back(& block[x_map[Coordinate(x_tmp.row, x_tmp.col-1)]]);
    if (block[i].x.col < shift[x_tmp.row]-1)
      block[i].neighbors.push_back(& block[x_map[Coordinate(x_tmp.row, x_tmp.col+1)]]);

    if (block[i].y.col > 0)
      block[i].neighbors.push_back(& block[y_map[Coordinate(y_tmp.row, y_tmp.col-1)]]);
    if (block[i].y.col < shift[y_tmp.row]-1)
      block[i].neighbors.push_back(& block[y_map[Coordinate(y_tmp.row, y_tmp.col+1)]]);

    if (block[i].z.col > 0)
      block[i].neighbors.push_back(& block[z_map[Coordinate(z_tmp.row, z_tmp.col-1)]]);
    if (block[i].z.col < shift[z_tmp.row]-1)
      block[i].neighbors.push_back(& block[z_map[Coordinate(z_tmp.row, z_tmp.col+1)]]);
  }

  // test
  // printPointerVector(block[0].neighbors, "block[0].neighbors");
  // printPointerVector(block[100].neighbors, "block[100].neighbors");
  // printPointerVector(block[108].neighbors, "block[108].neighbors");
  // printPointerVector(block[216].neighbors, "block[216].neighbors");
}

// LABEL CB::update
void ChessBoard::update()
{
  valid_pos.clear();
  my_pos.clear();
  opponent_pos.clear();
  line.clear();
  critical_moves.clear();
  win = false;
  lose = false;
  critical_move = -1;



  for (int i = 0; i < 217; i++) {
    block[i].state = board[i];
    if (board[i] == EMPTY)
      valid_pos.push_back(i);
    if (board[i] == ME)
      my_pos.push_back(i);
    if (board[i] == OPPONENT)
      opponent_pos.push_back(i);
  }

  // printVector(valid_pos, "valid_pos");
  // cout << "valid_pos = " << valid_pos.size() << endl;
  // waitKey();

  // count connective lines
  // cout << " - count connective lines..." << endl;
  map<Coordinate, int>* axiz[3] = { &x_map, &y_map, &z_map};

  for (int a = 0; a < 3; a++) {
    // cout << "a = " << a << endl;
    for (int r = 0; r < 17; r++) {
      int length = 1, prev_state = -1, prev_length = 1;
      int head, tail, status;
      int prev_line_row = -1, prev_line_tail = -1;
      for (int c = 0; c < layer[r]; c++) {
        int cur_state = block[(*axiz[a])[Coordinate(r, c)]].state;
        if ( cur_state != EMPTY && cur_state == prev_state)
          length++;
        else
          length = 1;

        // connective
        if (prev_length > length || (c == layer[r]-1 && length>1)) {
          std::vector<Block*> block_list;
          int start = (c == layer[r]-1 && length>1)?  c-(length-1) : c-prev_length;
          int end = (c == layer[r]-1 && length>1)?  c+1 : c;
          int line_length = (c == layer[r]-1 && length>1)?  length : prev_length;
          int line_state = (c == layer[r]-1 && length>1)?  cur_state : prev_state;
          int head_line_pos = (*axiz[a])[Coordinate(r, start-1)];

          for (int i = start; i < end; i++)
            block_list.push_back(&block[(*axiz[a])[Coordinate(r, i)]]);

          if (start > 0) {
            int head_neighbor = block[(*axiz[a])[Coordinate(r, start-1)]].state;
            head = (head_neighbor == EMPTY)? OPEN : BLOCK;
          }
          else
            head = BLOCK;

          if (c != layer[c]-1)
            tail = (cur_state == EMPTY)? OPEN : BLOCK;
          else
            tail = BLOCK;

          status = (head==OPEN && tail==OPEN)? OPEN :
                   (head==OPEN || tail==OPEN)? HALF_OPEN : BLOCK;

          line.push_back(Line(block_list, line_length, line_state, status));

          if(line_length >= 5 && line_state == ME)
            win = true;
          else if (line_length >= 5 && line_state == OPPONENT)
            lose = true;

          // critical_moves
          int tail_line_pos = (*axiz[a])[Coordinate(r, c)];
          int c_move = (head == OPEN)? head_line_pos : tail_line_pos;

          if (line_length == 3 && status == OPEN) {
            put_one(head_line_pos, ME);
            update_one(head_line_pos);
            int h_util = calculate_utility();
            remove_one(head_line_pos);
            update_one(tail_line_pos);
            put_one(tail_line_pos, ME);
            update_one(tail_line_pos);
            int t_util = calculate_utility();
            remove_one(tail_line_pos);
            update_one(tail_line_pos);

            c_move = (h_util > t_util)? head_line_pos : tail_line_pos;

            critical_moves.push_back(c_move);
            critical_move = c_move;
          }

          if (prev_line_row == r && prev_line_tail == start-2 && head==OPEN) {
            // cout << "-- inside --" << endl;
            // cout << "r = " << prev_line_row << endl;
            // cout << "prev_line_tail = " << prev_line_tail << endl;
            // cout << "start = " << "start" << endl;
            critical_moves.push_back(head_line_pos);
            critical_move = head_line_pos;
          }

          if (start>1 && line_length>2 &&
              block[(*axiz[a])[Coordinate(r, start-2)]].state == line_state &&
              head==OPEN ) {
            critical_move = head_line_pos;
          }

          if (line_length == 4 && line_state == OPPONENT && status == HALF_OPEN) {
            cout << "Oops!" << endl;
            critical_move = c_move;
            critical_moves.push_back(c_move);
          }

          if (line_length == 4 && line_state == ME && status != BLOCK) {
            critical_moves.push_back(c_move);
            critical_move = c_move;
          }

          prev_line_row = r;
          prev_line_tail = c-1;

          // test
          // cout << " == line == " << endl;
          // cout << line.back() << endl;
          // printPointerVector(block_list, "block_list");
          // string s = (head == OPEN)? "OPEN" : "BLOCK";
          // cout << "head = " << s;
          // s = (tail == BLOCK)? "BLOCK" : "OPEN";
          // cout << ", tail = " << s << endl << endl;
        }
        prev_state = cur_state;
        prev_length = length;
      }
    }
  }

  // test
  // cout << "line.size() = " << line.size() << endl;
  // waitKey();
  // cout << " - finish analyze lines.\n" << endl;
}

// LABEL put_one
void ChessBoard::put_one(int new_pos, int player)
{
  board[new_pos] = (player == ME)? 1 : 2;
  block[new_pos].state = player;
  valid_pos.erase(std::remove(valid_pos.begin(), valid_pos.end(), new_pos), valid_pos.end());
}

void ChessBoard::remove_one(int new_pos) {
  board[new_pos] = 0;
  block[new_pos].state = EMPTY;
  valid_pos.push_back(new_pos);
}

void ChessBoard::update_one(int new_pos) {
  // three axiz
  map<Coordinate, int>* axiz[3] = { &x_map, &y_map, &z_map};
  for (int a = 0; a < 3; a++) {
    // row
    int r = (a == 0)? block[new_pos].x.row :
            (a == 1)? block[new_pos].y.row :
            block[new_pos].z.row;

    // remove old lines that are on the same line as new_pos
    vector<Line>::iterator iter = line.begin();
    while (iter != line.end()) {
      int line_row = (a == 0)? iter->block.front()->x.row :
                     (a == 1)? iter->block.front()->y.row :
                     iter->block.front()->z.row;
      if (line_row == r)
        iter = line.erase(iter);
      else
        ++iter;

    }

    // update line counting
    int length = 1, prev_state = -1, prev_length = 1;
    int head, tail, status;
    for (int c = 0; c < layer[r]; c++) {
      int cur_state = block[(*axiz[a])[Coordinate(r, c)]].state;
      if ( cur_state != EMPTY && cur_state == prev_state)
        length++;
      else
        length = 1;

      // connective
      if (prev_length > length || (c == layer[r]-1 && length>1)) {
        std::vector<Block*> block_list;

        int start = (c == layer[r]-1 && length>1)?  c-(length-1) : c-prev_length;
        int end = (c == layer[r]-1 && length>1)?  c+1 : c;
        int line_length = (c == layer[r]-1 && length>1)?  length : prev_length;
        int line_state = (c == layer[r]-1 && length>1)?  cur_state : prev_state;

        for (int i = start; i < end; i++)
          block_list.push_back(&block[(*axiz[a])[Coordinate(r, i)]]);

        if (start > 0) {
          int head_neighbor = block[(*axiz[a])[Coordinate(r, start-1)]].state;
          head = (head_neighbor == EMPTY)? OPEN : BLOCK;
        }
        else
          head = BLOCK;

        if (c != layer[c]-1)
          tail = (cur_state == EMPTY)? OPEN : BLOCK;
        else
          tail = BLOCK;

        status = (head==OPEN && tail==OPEN)? OPEN :
                 (head==OPEN || tail==OPEN)? HALF_OPEN : BLOCK;

        line.push_back(Line(block_list, line_length, line_state, status));

        if(line_length >= 5 && line_state == ME)
          win = true;
        else if (line_length >= 5 && line_state == OPPONENT)
          lose = true;

        // test
        // cout << " == line == " << endl;
        // cout << line.back() << endl;
        // printPointerVector(block_list, "block_list");
        // string s = (head == OPEN)? "OPEN" : "BLOCK";
        // cout << "head = " << s;
        // s = (tail == BLOCK)? "BLOCK" : "OPEN";
        // cout << ", tail = " << s << endl << endl;
      }

      prev_state = cur_state;
      prev_length = length;
    }
  }
}

// LABEL CB::utility
int ChessBoard::calculate_utility()
{
  /* utility */
  opponent_lines.clear();
  my_lines.clear();
  for (size_t i = 0; i < 7; i++) {
    opponent_lines.push_back(0);
    my_lines.push_back(0);
  }

  // my_lines = vector<int>(6,0);
  bool line_win = false;
  bool line_lose = false;

  for (int i = 0; i < line.size(); i++) {
    int line_state = line[i].state;
    int line_length = line[i].length;
    int line_status = line[i].status;

    if(line_length >= 5 && line_state == ME)
      line_win = true;
    else if (line_length >= 5 && line_state == OPPONENT)
      line_lose = true;

    if(line_state == ME && line_win == true)
    {
        my_lines[6]++;
    }
    if(line_state == OPPONENT && line_lose == true)
    {
        opponent_lines[6]++;
    }
    if((line_length == 1) && (line_status == OPEN))
    {
        if(line_state == ME)
        {
          my_lines[2]++; //150
        }
        else
        {
          opponent_lines[2]++; //-1000
        }
    }
    if((line_length == 2) && (line_status != BLOCK))
    {

        if(line_state == ME)
        {
          if(line_status == OPEN) //live-2
          {
              my_lines[3]++; //600
          }
          else //dead-2
          {
            my_lines[1]++; //120
          }
        }
        else
        {
          if(line_status == OPEN) //live-2
          {
              opponent_lines[4]++; //-5000
          }
          else //dead-2
          {
            opponent_lines[1]++; // -800
          }
        }
    }
    if((line_length == 3) && (line_status != BLOCK))
    {

        if(line_state == ME)
        {
          if(line_status == OPEN) //live-3
          {
              my_lines[4]++; //3500
          }
          else //dead-3
          {
            my_lines[2]++; //600
          }
        }
        else
        {
          if(line_status == OPEN) //live-3
          {
              opponent_lines[4]++; //-5000
          }
          else //dead-3
          {
            opponent_lines[3]++; //-2000
          }
        }
    }
    if((line_length == 4) && (line_status != BLOCK))
    {
        if(line_state == ME)
        {
           my_lines[5]++;
        }
        else
        {
           opponent_lines[5]++;
        }
    }
  }

  // sum up utility
  // printVector(my_lines, "my_lines");
  // printVector(my_lines, "opponent_lines");


  int new_utility = 0;
  new_utility += my_lines[1];
  new_utility -= opponent_lines[1]*5;
  int temp = 1;
  if( my_lines[6] > 0 ) return INT_MAX; //WIN
  if( opponent_lines[6] > 0) return INT_MIN+1; // opponent WIN
  for(int i = 2; i < 6; i++)
  {
  		temp *= 100;
  		new_utility+=my_lines[i] * temp;
  		new_utility-=opponent_lines[i] * temp;
  }


  return new_utility;
}

int ChessBoard::get_utility() {
  return calculate_utility();
}

vector<int> ChessBoard::get_reasonable_moves() {
  vector<int> reasonables;

  if (valid_pos.size() <= 215) {
    if (board[108] == EMPTY) {
      reasonables.push_back(108);
      return reasonables;
    }

  }
  // else {
  //   // extend my connective line
  //   if (second_last != -1) {
  //     add_neighbor(reasonables, second_last);
  //   }
  //
  //   // block opponent's line
  //   if (last != -1) {
  //     add_neighbor(reasonables, last);
  //   }
  // }

  return reasonables;
}
