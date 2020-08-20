#include "gamemodel.h"
#include "head.h"
#include "player.h"
#include "card.h"
#include <stdexcept>
#include <iostream> // validateHeadNum()
using namespace std;

GameModel::GameModel(): currentPlayer{nullptr}, currentPlayerIndex{0} {
	heads = std::make_unique<std::deque<std::shared_ptr<Head>>>();
	players = std::make_unique<std::deque<std::shared_ptr<Player>>>();
}

string GameModel::inspectCurrentPlayerCard() {
	return currentPlayer->inspectDraw()->inspectCard();
}
			
size_t GameModel::getHowManyHeads() {
	return heads->size();
}

int GameModel::getCurrentPlayerCardIntValue() {
	return currentPlayer->inspectDraw()->intValue();
}

int GameModel::getHeadIntValue(int headNum) {
	for (auto it = heads->begin(); it != heads->end(); ++it) {
		if ((*it)->getHeadNum() == headNum) {
			return (*it)->getTopIntValue();
		}
	}
	return -1;
}

void GameModel::setJokerValue(std::string value, int headIntValue) {
	if (headIntValue != -1) {
		int val;
		if (value == "A") {
			val = 1;
		} else if (value == "J") {
			val = 11;
		} else if (value == "Q") {
			val = 12;
		} else if (value == "K") {
			val = 13;
		} else {
			val = stoi(value);
		}
		if (val > headIntValue && headIntValue != 1) {
			throw invalid_argument("Joker value cannot be greater than head value.");
		}
	}
	currentPlayer->setJokerValue(value);
}

int GameModel::getCurrentPlayerNum() {
	return currentPlayer->getPlayerNum();
}

bool GameModel::cutOffHead() { // called by placeOnHead()
	currentPlayer->addReserveToDiscard();	
	std::shared_ptr<Deck> addDeck = heads->front();
	heads->pop_front();
	currentPlayer->addDeckToDiscard(addDeck);
	if (addDiscardToDraw()) {
		return false; // false means the player has run out of cards
	}
	std::shared_ptr<Card> newhead1 = currentPlayer->drawCard(); 
	if (newhead1) {
		if (newhead1->inspectCard() == "Joker") {
			newhead1->setValue("2");
		}
		std::shared_ptr<Head> head1 = std::make_shared<Head>();
		head1->addCardToBack(newhead1);
		heads->emplace_back(head1);
	} else {
		return false;
	}
	std::shared_ptr<Card> newhead2 = currentPlayer->drawCard();
	if (newhead2) {
		if (newhead2->inspectCard() == "Joker") {
			newhead2->setValue("2");
		}
		std::shared_ptr<Head> head2 = std::make_shared<Head>();
		head2->addCardToBack(newhead2);
		heads->emplace_back(head2);
	} else {
		return false;
	}
	return true; // true means player was able to successfully cutOffHead and create 2 new ones
}

bool GameModel::addDiscardToDraw() {
	if (currentPlayer->getDrawSize() == 0) {
		if (currentPlayer->getDiscardSize() == 0) {
			return true;
		} else {
			currentPlayer->addDiscardToDraw();
			return false;
		}
	}
	return false;
}

void GameModel::switchCurrentPlayer() {
	currentPlayer->setCurrentPlayer(false);
	if (currentPlayerIndex == players->size() - 1) {
		currentPlayerIndex = 0;
	} else {
		++currentPlayerIndex;
	}
	currentPlayer = players->at(currentPlayerIndex);
	currentPlayer->setCurrentPlayer(true);
}

int GameModel::getFirstHeadNum() {
	return heads->front()->getHeadNum();
}

int GameModel::getLastHeadNum() {
	return heads->back()->getHeadNum();
}

void GameModel::validateHeadNum(int headNum, int currPlayerIntValue) {
	string error_msg;
	if ((headNum < getFirstHeadNum()) || (headNum > getLastHeadNum())) {
		if (headNum == 0) {
			if (getHowManyHeads() == 1) {
				error_msg = "Reserve is unavailable if there is only one head.";
			} else {
				error_msg = "You must play on a head, reserve is already full.";
			}
		} else {
			if (getHowManyHeads() == 1) {
				error_msg = "Invalid head. " + to_string(getFirstHeadNum()) + " is the only available head.";
			} else {
				error_msg = "Invalid head. Available heads are from " + to_string(getFirstHeadNum()) + " to " + to_string(getLastHeadNum());
			}
		}
		throw invalid_argument(error_msg);
	} else {
		if (headNum != getFirstHeadNum()) {
			if (getHeadIntValue(headNum) != 1) {
				if (getHeadIntValue(headNum) < currPlayerIntValue) {
					throw invalid_argument("You may not cut off this head.");
				}
			}
		}
	}
}

string GameModel::placeOnHead(int headNum) { // headNum has already been validated by validateHeadNum()
	if (currentPlayer->getDrawSize() == 0) {
		if (currentPlayer->emptyReserve()) {
			return "end turn"; // reserve is not empty and added to draw
		} else {
			return "win"; // reserve is empty
		}
	}
        std::shared_ptr<Card> add = currentPlayer->drawCard();	
	int addValue = add->intValue();
	int headValue;
	for (auto it = heads->begin(); it != heads->end(); ++it) {
		if ((*it)->getHeadNum() == headNum) {
			headValue = (*it)->getTopIntValue();
			(*it)->addCardToBack(add);
			if ((headValue != 1 && addValue == 1) || (headValue == 1 && addValue != 1)) {
				return "continue";
			} else if (headValue == 1 && addValue == 1) {
				return "end turn";
			} else if (addValue >= headValue) {
				if (addValue > headValue) {
					if (!cutOffHead()) {
						return "win";
					} 
				}
				return "end turn";
			}
			break;
		}
	}
	return "continue";
}

void GameModel::useReserve() {
	if (currentPlayer->getDrawSize() == 1 && currentPlayer->getDiscardSize() == 0) {
		throw invalid_argument("You may not use reserve if you only have 1 draw card left.");
	} else if (currentPlayer->getDrawSize() != 0) {
		currentPlayer->addToReserve(currentPlayer->drawCard());
		if (currentPlayer->getDrawSize() == 0) {
			currentPlayer->addDiscardToDraw();
		}
	} else {
		currentPlayer->emptyReserve(); // there may be nothing in the reserve
	}
}

bool GameModel::emptyReserve() {
	return currentPlayer->emptyReserve();
}

void GameModel::createPlayers(int n) {
	if (n <= 1) {
		throw invalid_argument("There must be at least 2 players."); 
	} else {
		for (int x = 1; x <= n; ++x) {
			players->emplace_back(std::make_shared<Player>(x));
		}
	}
}

void GameModel::printInfo(bool printCurrentPlayer, int remaining) {
	cout << "Heads: " << endl;
	for (auto it = heads->begin(); it != heads->end(); ++it) {
		cout << (*it)->getHeadNum() << ": " << (*it)->topCard() << " (" << (*it)->getSize() << ")" << endl;
	}
	cout << "\nPlayers: " << endl;
	for (auto it = players->begin(); it != players->end(); ++it) {
		int total = (*it)->getDiscardSize() + (*it)->getDrawSize();
		cout << "Player " << (*it)->getPlayerNum() << ": " << total << " (";
		cout << (*it)->getDrawSize() << " draw, " << (*it)->getDiscardSize() << " discard)";
		if (printCurrentPlayer) { // if it is a player's turn, print the following
			if (*it == currentPlayer) {
				cout << " + 1 in hand, " << remaining << " remaining, " << currentPlayer->isReserveInUse() << " in reserve";
			}
		}	
		cout << endl;
	}
	cout << endl;
}

void GameModel::distributeCards(int numPlayers, bool testing, unsigned testingseed) {
	Deck starter;
	int sets;
	int highestCardValue;
	if (testing) {
		sets = 1;
		highestCardValue = 6;
	} else {
		sets = numPlayers;
		highestCardValue = 13;
	}
	for (int n = 0; n < sets; ++n) {
		for (int s = 0; s < 5; ++s) {
			if (s < 4) { // create 52 cards, 13 from each suit
				for (int v = 0; v < highestCardValue; ++v) {
					starter.addCardToBack(std::make_shared<Card>(Card::suits[s], Card::values[v]));
				}
			} else { // create 2 jokers with "unassigned" values
				for (int joker = 0; joker < 2; ++joker) {
					starter.addCardToBack(std::make_shared<Card>("J", "2"));
				}
			}
		}
	}
	if (testing) {
		starter.shuffle(testingseed); // change this for the final game
	} else {
		starter.shuffle();
	}
	bool dontbreak = true;
	while (dontbreak) { // until starter deck is empty, distribute cards
		for (auto it = players->begin(); it != players->end(); ++it) {
			if (starter.isEmpty()) {
				dontbreak= false;
				break;
			} else {
				(*it)->addCardToDraw(starter.removeTopCard());
			}
		}
	}
	currentPlayer = players->at(currentPlayerIndex); // set currentPlayer to the first player
	currentPlayer->setCurrentPlayer(true);
	std::shared_ptr<Head> newhead = std::make_shared<Head>();
	if (currentPlayer->inspectDraw()->inspectCard() == "Joker") {
		setJokerValue("2");
	}
	newhead->addCardToBack(currentPlayer->drawCard());
	heads->emplace_back(newhead);
	switchCurrentPlayer(); // it is now player 2's turn
}			
