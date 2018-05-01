#include<iostream>
#include<fstream>
#include<string>
using namespace std;

#define TEAM_NUMBER 19
#define ROUND 10

enum { WHITE = 1, BLACK = 2 };
enum { CONTINUE = -1, WIN = 1, LOSE = 0 };

int main(){
    string filename ="state_" + to_string(TEAM_NUMBER) + ".txt";
    fstream fp;
    fp.open(filename, ios::out);
    if(!fp){
        cout<<"Fail to open file: "<<filename<<endl;
    }

    int state[217];

    for (int i = 0; i < 217; i++) {
      state[i] = 0;
    }

    // put chess
    state[106] = BLACK;
    state[107] = BLACK;
    state[108] = BLACK;
    state[78] = BLACK;
    state[93] = BLACK;

    state[61] = WHITE;
    state[76] = WHITE;
    state[92] = WHITE;
    state[104] = WHITE;

    // ROUND
    fp << ROUND <<endl;
    // board
    for (int i = 0; i < 216; i++)
      fp << state[i] << ' ';
    fp << state[216] << endl;
    // game_status
    fp << CONTINUE << endl;

    fp.close();//關閉檔案
    return EXIT_SUCCESS;
}
