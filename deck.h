#ifndef _DECK_H_
#define _DECK_H_

#include <deque>
#include <string>
#include <memory>

class Card;

class Deck {
        protected:
		std::shared_ptr<std::deque<std::shared_ptr<Card>>> cards;
        public:
		Deck();
                int getSize();
                void addCardToBack(std::shared_ptr<Card> card);
		void unassignJokers();
		void addDeck(std::shared_ptr<Deck> deck); // called when a deck is added to another, e.g. Head is discarded
		void addCardToFront(std::shared_ptr<Card> card);
		std::shared_ptr<Card> removeTopCard();
                std::shared_ptr<Card> inspectTopCard();
                void shuffle(unsigned int defaultseed = 1);
		bool isEmpty();
};

#endif
