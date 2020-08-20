#ifndef _GAMEMODEL_H_
#define _GAMEMODEL_H_

#include <deque>
#include <memory>
#include <cstddef> // std::size_t
//#include "subject.h"

class Head; // forward declaration
class Player; // forward declaration
class Card; // forward declaration

class GameModel { // public Subject
	private:
		std::unique_ptr<std::deque<std::shared_ptr<Head>>> heads;
		std::unique_ptr<std::deque<std::shared_ptr<Player>>> players;
		std::shared_ptr<Player> currentPlayer;
		std::size_t currentPlayerIndex;
		int getFirstHeadNum(); // used by validateHeadNum, but may have to be made public
		int getLastHeadNum(); 
	public:
		GameModel();
		std::string inspectCurrentPlayerCard();
		int getCurrentPlayerCardIntValue();
		int getHeadIntValue(int headNum);
		size_t getHowManyHeads();
		int getCurrentPlayerNum();
		void createPlayers(int n); // may throw invalid_argument
		void validateHeadNum(int headNum, int currPlayerIntValue); // may throw invalid_argument
		void distributeCards(int numPlayers, bool testing = false, unsigned int testingseed = 0); // in testing mode, 1 set of cards will be distributed among numPlayers players. otherwise, numPlayers decks will be distributed
		std::string placeOnHead(int headNum); // returns "end turn" if players turn must end, "win" if player has won, or "continue" otherwise, if throws exception, end players turn
		bool cutOffHead(); // returns false if ran out of cards, else true if cutOffHead successful
		void setJokerValue(std::string value, int headIntValue = -1);
		void useReserve(); // true if success, false 
		void switchCurrentPlayer();
		bool emptyReserve();
		void printInfo(bool printCurrentPlayer, int remaining = 0); // if currentPlayer is true, print the current player's additional information
		bool addDiscardToDraw(); // if draw size = 0, returns true if discard = 0, else adds discard to draw and returns false
};
#endif
