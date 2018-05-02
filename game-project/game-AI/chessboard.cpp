/* AI game project */
// by cysun, Americano Ho and Kao Chu Ching

#include <iterator>
#include "chessboard.h"
#include "debug.h"

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

// LABEL CB::init
void ChessBoard::initialize()
{
  cout << "\ninitialize chessboard in agent...\n" << endl;

  win = false;
  lose = false;
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
  cout << "update chessboard...\n" << endl;

  for (int i = 0; i < 217; i++) {
    block[i].state = board[i];
    if (board[i] == EMPTY)
      valid_pos.push_back(i);
    if (board[i] == ME)
      my_pos.push_back(i);
    if (board[i] == OPPONENT)
      opponent_pos.push_back(i);
  }

  // count connective lines
  cout << " - count connective lines...\n" << endl;
  map<Coordinate, int>* axiz[3] = { &x_map, &y_map, &z_map};

  for (int a = 0; a < 3; a++) {
    // cout << "a = " << a << endl;
    for (int r = 0; r < 17; r++) {
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


          /* utility */
          //newly added-----------
          if(line_state == ME && win == true)
          {
          		my_lines[6]++;
          }
          else if(line_state == OPPONENT && win == true)
          {
          		opponent_lines[6]++;
          }
          else if((line_length == 1) && (head == OPEN || tail == OPEN))
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
          else if((line_length == 2) && (head == OPEN || tail == OPEN))
          {

          		if(line_state == ME)
          		{
          			if(head == OPEN && tail == OPEN) //live-2
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
					if(head == OPEN && tail == OPEN) //live-2
          			{
          			    opponent_lines[4]++; //-5000
          			}
          			else //dead-2
          			{
          				opponent_lines[1]++; // -800
          			}
          		}
          }
           else if((line_length == 3) && (head == OPEN || tail == OPEN))
          {

          		if(line_state == ME)
          		{
          			if(head == OPEN && tail == OPEN) //live-3
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
					      if(head == OPEN && tail == OPEN) //live-3
          			{
          			    opponent_lines[4]++; //-5000
          			}
          			else //dead-3
          			{
          				opponent_lines[3]++; //-2000
          			}
          		}
          }
          else if((line_length == 4) && (head == OPEN || tail == OPEN))
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

          // utility

        }
        prev_state = cur_state;
        prev_length = length;
      }
    }
  }

  // test
  // cout << "line.size() = " << line.size() << endl;
  // waitKey();
}

// LABEL put_one
void ChessBoard::put_one(int new_pos, int player)
{
  board[new_pos] = (player == ME)? 1 : 2;
  block[new_pos].state = player;
}

void ChessBoard::remove_one(int new_pos) {
  board[new_pos] = 0;
  block[new_pos].state = EMPTY;
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
    }
  }
}

// LABEL CB::utility
int ChessBoard::calculate_utility()
{
  int new_utility = 0;
  new_utility += my_lines[1];
  new_utility -= opponent_lines[1]*5;
  int temp = 1;
  if( my_lines[6] > 0 ) return INT_MAX; //WIN
  if( opponent_lines[6] > 0) return INT_MIN; // opponent WIN
  for(int i = 2; i < 6; i++ )
  {
  		temp *= 100;
  		new_utility+=my_lines[i] * temp;
  		new_utility-=opponent_lines[i] * temp * 10;
  }
 

  return new_utility;
}

int ChessBoard::get_utility() {
  return calculate_utility();
}
