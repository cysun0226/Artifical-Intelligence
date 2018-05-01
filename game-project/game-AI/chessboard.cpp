/* AI game project */
// by cysun

# include "chessboard.h"
# include "debug.h"

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


/* chessboard */
// LABEL Chess-Board
ChessBoard::ChessBoard() {}
ChessBoard::ChessBoard(std::vector<int> board) {
  state.assign(board.begin(), board.end());
  initialize();
}

// LABEL CB::init
void ChessBoard::initialize()
{
  cout << "\ninitialize chessboard in agent..." << endl;

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
  for (int i = 16; i >= 10; i--){
    shift.push_back(i);
    z_bound.push_back(z_bound.back()+i);
  }
  z_bound.push_back(217);

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

  printPointerVector(block[100].neighbors, "block[100].neighbors");
  printPointerVector(block[108].neighbors, "block[108].neighbors");

}
