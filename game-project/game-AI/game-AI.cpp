/* AI game project */
// by cysun

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
  // constructor
  Coordinate(int r, int c) : row(r), col(c) {}
} Coordinate;

// overload << (for cout)
std::ostream& operator << (std::ostream &o, const Coordinate& p) {
    return o << "(" << p.row << ", " << p.col << ")";
}


typedef enum { ME=1, OPPONENT=2, EMPTY=0 } STATE;
typedef struct Block {
  Coordinate x;
  Coordinate y;
  Coordinate z;
  STATE state;
  int id; // number of array in state.txt
  // constructor
  Block(int id, Coordinate x, Coordinate y, Coordinate z, STATE s) :
  id(id), x(x), y(y), z(z), state(s) {}
} Block;

// overload << (for cout)
std::ostream& operator << (std::ostream &o, const Block& p) {
  string s;
  s = (p.state == ME)? "ME" : (p.state == OPPONENT)? "OPPONENT" : "EMPTY";
    return o << "id: " << p.id << " | pos: " << p.x << " " << p.y << " "
     << p.z << " | state: " << s;
}


class ChessBoard {
private:
  Block blocks[217];
  int state[217]; // 1-D array

public:
  // update

  // ChessBoard ();
  // virtual ~ChessBoard ();
};

main() {
  int i = 10;
  Coordinate x(0, 5);
  Coordinate y(0, 0);
  Coordinate z(5, 7);

  Block block_1(10, x, y, z, EMPTY);

  std::cout << "point x = " << x << std::endl;
  std::cout << "block_1 = " << block_1 << std::endl;
}
