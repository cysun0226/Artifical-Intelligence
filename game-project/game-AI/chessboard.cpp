/* AI game project */
// by cysun

# include "chessboard.h"

// for debug
void waitKey() {
  cout << "\nPress any key to continue ...";
  cin.get();
}

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
ChessBoard::ChessBoard() {}
ChessBoard::ChessBoard(std::vector<int> board) {
  state.assign(board.begin(), board.end());
  initialize();
}

void ChessBoard::initialize()
{
  cout << "\ninitialize chessboard in agent..." << endl;

  for (size_t i = 0; i < 217; i++) {
    block[i].state = EMPTY;

    block[i].id = i;
    // blocks[i].x = Coordinate(, );


  }

  // x, y, z
  map<int, int> x_row, y_row;
  std::vector<int> shift, z_bound;
  // shift.push_back(0);
  z_bound.push_back(0);
  for (int i = 9; i <= 17; i++) {
    shift.push_back(i);
    z_bound.push_back(z_bound.back()+i);
  }
  for (int i = 16; i >= 10; i--){
    shift.push_back(i);
    z_bound.push_back(z_bound.back()+i);
  }

  for (size_t i = 0; i < z_bound.size(); i++) {
    cout << "z_bound[" << i << "] = " << z_bound[i] << endl;
  }

  waitKey();

  // x
  for (size_t r = 0; r <= 8; r++) {
    for (size_t c = 0; c < shift[r]; c++) {
      int tmp = (c >= 9)? z_bound[c] + r - (c-8)  : r+z_bound[c];
      block[tmp].x = Coordinate(r, c);
      cout << "block[" << tmp << "] = " << Coordinate(r, c) << endl;

    }
  }

  // for (size_t r = 0; r < 8; r++) {
  //   for (size_t c = 0; c < shift[r]; c++) {
  //     int tmp = (c >= 9)? z_bound[c] + r - (c-8)  : r+z_bound[c];
  //     block[tmp].x = Coordinate(r, c);
  //     cout << "block[" << tmp << "] = " << Coordinate(r, c) << endl;
  //
  //   }
  // }


  cout << "block[100].x = " << block[100].x << endl;
  cout << "block[108].x = " << block[108].x << endl;
  cout << "block[215].x = " << block[215].x << endl;
  // system("pause");
  cout << "\nPress any key to continue ..." << endl;
  cin.get();



}
