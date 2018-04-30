/* AI game project */
// by cysun

# include "chessboard.h"
//
// using namespace std;

/* struct */
// constructor
Coordinate::Coordinate() {}
Coordinate::Coordinate(int r, int c) : row(r), col(c) {}

// overload << (for cout)
std::ostream& operator << (std::ostream &o, const Coordinate& p) {
    return o << "(" << p.row << ", " << p.col << ")";
}

// constructor
Block::Block(){}
Block::Block(int id, Coordinate x, Coordinate y, Coordinate z, STATE s) :
id(id), x(x), y(y), z(z), state(s) {}

// overload << (for cout)
std::ostream& operator << (std::ostream &o, const Block& p) {
  string s;
  s = (p.state == ME)? "ME" : (p.state == OPPONENT)? "OPPONENT" : "EMPTY";
    return o << "id: " << p.id << " | pos: " << p.x << " " << p.y << " "
     << p.z << " | state: " << s;
}

// Coordinate x(0, 5);
// Coordinate y(0, 0);
// Coordinate z(5, 7);
//
// Block block_1(10, x, y, z, EMPTY);
//
// std::cout << "point x = " << x << std::endl;
// std::cout << "block_1 = " << block_1 << std::endl;

/* chessboard */
ChessBoard::ChessBoard()
{
  cout << "build chessboard" << endl;

}
