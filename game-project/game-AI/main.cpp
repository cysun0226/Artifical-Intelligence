/* AI game project */
// by cysun

#include "chessboard.h"
#include "agent.h"

#define TEAM_NUMBER 19

int main() {

  Agent* agent = new Agent(TEAM_NUMBER);

	while (true) {
		agent->processStateInfo();
		if (agent->isGameStop()) break;
		if (agent->isMyTurn()) agent->nextMove();
	}

	free(agent);
	return 0;
}
