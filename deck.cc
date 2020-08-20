#include "deck.h"
#include "card.h"
#include <algorithm> // std::shuffle
#include <random> // std::default_random_engine
#include <chrono> // std::chrono::system_clock (for time-based seed generation)
#include <memory>

Deck::Deck() {
	this->cards = std::make_shared<std::deque<std::shared_ptr<Card>>>();
}

int Deck::getSize() {
	return cards->size();
}

void Deck::addCardToBack(std::shared_ptr<Card> card) {
	if (card) {
		cards->emplace_back(card);
	}
}

void Deck::addCardToFront(std::shared_ptr<Card> card) {
	if (card) {
		cards->emplace_front(card);
	}
}

void Deck::addDeck(std::shared_ptr<Deck> deck) {
	if (deck) {
		while (!deck->isEmpty()) {
			addCardToBack(deck->removeTopCard());
		}
	}
}

bool Deck::isEmpty() {
	return cards->empty();
}

std::shared_ptr<Card> Deck::removeTopCard() {
	if (!cards->empty()) {
		std::shared_ptr<Card> retval = cards->front();
		cards->pop_front();
		return retval;
	} else {
		return nullptr;
	}
}

std::shared_ptr<Card> Deck::inspectTopCard() {
	return cards->front();

}

void Deck::unassignJokers() { // will be called when discard pile is transferred to draw and all jokers must be unassigned their values
	for (auto it = cards->begin(); it != cards->end(); ++it) {
		(*it)->jokerUnassigned();
	}
}

void Deck::shuffle(unsigned defaultseed) {
	if (getSize() > 1) {
		unsigned seed;
		if (defaultseed == 1) {
			// time-based seed
			seed = std::chrono::system_clock::now().time_since_epoch().count();
		} else {
			seed = defaultseed;
		}
		std::shuffle(cards->begin(), cards->end(), std::default_random_engine(seed));
	}
}
