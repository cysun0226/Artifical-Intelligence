/* AI game project */
// by cysun

#ifndef __CHESSBOARD_H__
#define __CHESSBOARD_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <math.h>

using namespace std;

/* struct and class */
typedef struct Coordinate {
  int row;
  int col;
  Coordinate();
  Coordinate(int r, int c);

  // operator
  Coordinate& operator = (Coordinate const &c) {
    if (this == &c) return *this; //self assignment
    this->row = c.row;
    this->col = c.col;
    return *this;
  }

  friend bool operator < (const Coordinate& l, const Coordinate& r) {
        return (l.row != r.row)? l.row < r.row : l.col < r.col;
  }

} Coordinate;

std::ostream& operator << (std::ostream &o, const Coordinate& p);

// Block
typedef enum { ME=1, OPPONENT=2, EMPTY=0 } STATE;
typedef struct Block {
  Coordinate x;
  Coordinate y;
  Coordinate z;
  int state;
  int id; // number of array in state.txt
  std::vector<Block*> neighbors;

  Block(int id, Coordinate x, Coordinate y, Coordinate z, STATE s);
  Block();
} Block;

std::ostream& operator << (std::ostream &o, const Block& p);

// LABEL struct Line
typedef enum { BLOCK, HALF_OPEN, OPEN } STATUS;
// typedef enum { BOTH_BLOCK, HALF_OPEN, BOTH_OPEN } STATUS;

typedef struct Line {
  std::vector<Block*> block;
  int length;
  int state; // ME or OPPONENT?
  int status; // BLOCK, HALF_OPEN, OPEN

  Line();
  Line(vector<Block*> b, int l, int s, int ss);
} Line;

std::ostream& operator << (std::ostream &o, const Line& p);


// LABEL Class Chess-Board
class ChessBoard {
private:
  std::vector<int> board;
  Block block[217];
  bool win, lose;
  int utility;

  vector<Line> line;
  vector<int> layer;
  vector<int> valid_pos, my_pos, opponent_pos;
  map<Coordinate, int> x_map, y_map, z_map;

  void initialize();

public:
  // update
  void update();
  void set_board(std::vector<int> v);
  int calculate_utility();
  int get_utility();
  std::vector<int> get_valid_pos();
  

  ChessBoard();
  ChessBoard(std::vector<int> board);
};


#endif
