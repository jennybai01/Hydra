#ifndef _PLAYER_H_
#define _PLAYER_H

#include <memory>
class Deck; // forward declaration
class Card; // forward declaration;

class Player {
	private:
		int playerNum;
		std::shared_ptr<Card> reserve;
		std::shared_ptr<Deck> discard;
		std::shared_ptr<Deck> draw;
		bool currentPlayer;
	public:
		explicit Player(int playerNum);
		int isReserveInUse(); // returns 1 if in use, else 0
		int getPlayerNum();
		std::shared_ptr<Card> popReserve();
		void addToReserve(std::shared_ptr<Card> card);
		bool emptyReserve(); // true if successful, false if reserve is empty, adds reserve to top of draw
		void setJokerValue(std::string value);
		void addCardToDraw(std::shared_ptr<Card> card);
		void addDeckToDiscard(std::shared_ptr<Deck> deck);
		void addReserveToDiscard();
		void addDeckToDraw(std::shared_ptr<Deck> deck);
		void addDiscardToDraw();
		std::shared_ptr<Card> inspectDraw();
		std::shared_ptr<Card> drawCard();
		void setCurrentPlayer(bool b);
		int getDiscardSize();
		int getDrawSize();
};
#endif
