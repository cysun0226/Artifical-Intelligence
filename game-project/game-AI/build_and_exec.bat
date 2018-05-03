g++ -static-libstdc++ -static-libgcc -std=c++11 main.cpp agent.cpp chessboard.cpp -o team_19.exe
copy team_19.exe ..\judge\
cd ..\judge\
rm state_19.txt
team_19.exe
cd ..\game-AI\
