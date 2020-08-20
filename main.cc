#include "gamemodel.h"
#include "player.h"
#include <iostream>
#include <cstddef>
#include <string>
using namespace std;

int main(int argc, char *argv[]) { // if testing, first arg is -testing, second optional arg is unsigned seed for random generation (for deck shuffling)
	bool testing = false;
	unsigned seed = 1;
	if (argc > 1) {
		string cla1 = argv[1];
		if (cla1 == "-testing") {
			testing = true;
		}
		if (argc > 2) {
			string cla2 = argv[2];
			seed = stoi(cla2);	
		}

	}

	GameModel gm;
	// read in amt of players
	int numPlayers;
	while (true) {
		cout << "How many players?" << endl;
		if (!(cin >> numPlayers)) {
			if (cin.eof()) {
				exit(0);
			} else {
				cin.clear();
				cin.ignore();
				cout << "Please enter an integer." << endl;
				continue;
			}
		}

		try {
			gm.createPlayers(numPlayers);
			break;
		} catch (const exception &ex) {
			cout << ex.what() << endl;
			continue;
		}
	}

	cout << endl;

	gm.distributeCards(numPlayers, testing, seed);

	gm.printInfo(false);

	bool winStatus= false;
	bool endTurnEarly = false;
	size_t numHeads;
	bool reserve;
	while (!winStatus) {
		cout << "Player " << gm.getCurrentPlayerNum() << ", it is your turn.\n" << endl;
		numHeads = gm.getHowManyHeads();
		for (size_t x = 0; x < numHeads; ++x) {
			reserve = false;
			gm.printInfo(true, numHeads - (x+1));
			cout << "Player " << gm.getCurrentPlayerNum() << ", you are holding a " << gm.inspectCurrentPlayerCard() << ". ";
			int move;
			while (true) { // get player's move
				cout << "Your move?" << endl;
				if (!(cin >> move)) {
					if (cin.eof()) {
						exit(0);
					} else {
						cin.clear();
						cin.ignore();
						cout << "Please enter an integer." << endl;
						continue;
					}
				}
				
				// reserve can only be used if there are more than 1 head present gm.getHowManyHeads();
				if (move == 0 && gm.getHowManyHeads() > 1 && reserve == false) {
					try {
						gm.useReserve();
						reserve = true;
						cout << "Player " << gm.getCurrentPlayerNum() << ", you have used your reserve. You are now holding a " << gm.inspectCurrentPlayerCard() << ". ";
					} catch (const exception &ex) {
						cout << ex.what() << endl;
						continue;
					}
				} else {
					try {
						gm.validateHeadNum(move, gm.getCurrentPlayerCardIntValue());
						break;
					} catch (const exception &ex) {
						cout << ex.what() << endl;
						continue;
					}
				}
			}

			cout << endl;

			if (gm.inspectCurrentPlayerCard() == "Joker") {
				string jokerValue;
				while (true) { // set joker value
					cout << "Joker value?" << endl;
					if (!(cin >> jokerValue)) {
						if (cin.eof()) {
							exit(0);
						}
					}
					
					try {
						gm.setJokerValue(jokerValue, gm.getHeadIntValue(move));
						break;
					} catch (const exception &ex) {
						cout << ex.what() << endl;
						continue;
					}
				}
			}
			
			string nextMove = gm.placeOnHead(move);
			if (nextMove == "win") {
				winStatus = true;
				break;
			} else if (nextMove == "end turn") { // in case current player's turn ends early
				endTurnEarly = true;
			}
			
			if (gm.addDiscardToDraw()) { // if there is no discard if draw left
				if (!gm.emptyReserve()) { // if there is also no reserve to empty
					break; // player has won
				}
			}

			if ((x == numHeads - 1) || (endTurnEarly == true)) {
				endTurnEarly = false;
				gm.printInfo(false);
				gm.addDiscardToDraw();
				break;
			}
		} // each head
		gm.emptyReserve();
		winStatus = gm.addDiscardToDraw(); // will add discard to draw if there are no cards in draw, and returns true if there are no cards in discard
		if (winStatus) {
			break;
		}
		gm.switchCurrentPlayer();
	}
	cout << "Player " << gm.getCurrentPlayerNum() << " has won!" << endl;
}

