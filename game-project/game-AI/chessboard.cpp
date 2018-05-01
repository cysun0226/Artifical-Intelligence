/* AI game project */
// by cysun

# include "chessboard.h"

// for debug
void waitKey() {
  cout << "\nPress any key to continue ...";
  cin.get();
  cout << endl;
}

void printVector(std::vector<int> v, string name) {
  cout << "\n -- " << name << " -- \n" << endl;
  for (size_t i = 0; i < v.size(); i++) {
    cout << name << "[" << i << "] = " << v[i] << endl;
  }
  cout << endl;
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
  }

  // x, y, z
  map<Coordinate, int> x_map, y_map, z_map;
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
      y_map[block[y_tmp].x] = y_tmp;

      block[z_bound[r]+c].z = Coordinate(r, c);
      z_map[block[z_bound[r]+c].z] = z_bound[r]+c;
      // cout << "block[" << z_bound[r]+c << "] = " << block[z_bound[r]+c].z << endl;
    }
  }

  for (size_t r = 1; r <= 8; r++) {
    for (size_t c = 0; c < shift[8-r]; c++) {
      int x_tmp = (c>=8)? z_bound[c]+r+8 : z_bound[c]+r+c;
      block[x_tmp].x = Coordinate(r+8, c);
      x_map[block[x_tmp].x] = x_tmp;
      int y_tmp = ((c+r)>=9)? z_bound[c+r+1] - (9-r) : z_bound[c+r] + (r+8);
      block[y_tmp].y = Coordinate(r+8, c);
      y_map[block[y_tmp].y] = y_tmp;

      block[z_bound[r+8]+c].z = Coordinate(r, c);
      z_map[block[z_bound[r+8]+c].z] = z_bound[r+8]+c;
    }
  }

  // neighbors
  for (size_t i = 0; i < 217; i++) {
    Coordinate tmp;
    // if (block[i].x.row > 0) { block[i].neighbors.push_back(&x_map[Coordinate(block[i].x.row-1, block[i].x.col)]); }
    // if (block[i].x.row <= 16) { tmp = block[i].x; tmp.row++; block[i].neighbors.push_back(&block[x_map[tmp]]); }
    // if (block[i].x.col > 0) { tmp = block[i].x; tmp.col--; block[i].neighbors.push_back(&x_map[tmp]); }
    // if (block[i].x.col <= 16) { tmp = block[i].x; tmp.col++; block[i].neighbors.push_back(&x_map[tmp]); }
    //
    // if (block[i].y.row > 0) { tmp = block[i].y; tmp.row--; block[i].neighbors.push_back(&y_map[tmp]); }
    // if (block[i].y.row <= 16) { tmp = block[i].y; tmp.row++; block[i].neighbors.push_back(&y_map[tmp]); }
    // if (block[i].y.col > 0) { tmp = block[i].y; tmp.col--; block[i].neighbors.push_back(&y_map[tmp]); }
    // if (block[i].y.col <= 16) { tmp = block[i].y; tmp.col++; block[i].neighbors.push_back(&y_map[tmp]); }
    //
    // if (block[i].z.row > 0) { tmp = block[i].z; tmp.row--; block[i].neighbors.push_back(&z_map[tmp]); }
    // if (block[i].z.row <= 16) { tmp = block[i].z; tmp.row++; block[i].neighbors.push_back(&z_map[tmp]); }
    // if (block[i].z.col > 0) { tmp = block[i].z; tmp.col--; block[i].neighbors.push_back(&z_map[tmp]); }
    // if (block[i].z.col <= 16) { tmp = block[i].z; tmp.col++; block[i].neighbors.push_back(&z_map[tmp]); }

  }

}
