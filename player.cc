#include "player.h"
#include "card.h"
#include "deck.h"
#include <iostream>
using namespace std;

Player::Player(int playerNum): playerNum{playerNum}, reserve{nullptr}, currentPlayer{false} {
	this->discard = std::make_shared<Deck>();
	this->draw = std::make_shared<Deck>();
}

int Player::getPlayerNum() {
	return playerNum;
}

bool Player::emptyReserve() {
	if (reserve) {
		draw->addCardToFront(popReserve());
		return true;
	}
	return false;
}

std::shared_ptr<Card> Player::popReserve() { // may return nullptr
	std::shared_ptr<Card> retval = reserve;
	reserve = nullptr;
	return retval;
}

void Player::addReserveToDiscard() {
	if (reserve) {
		discard->addCardToBack(popReserve());
	}
}

void Player::addToReserve(std::shared_ptr<Card> card) {
	if (card) {
		if (reserve) { // if reserve already contains a card, empty it first
			emptyReserve();
		}
		reserve = card;
	}
	// handle the case where there is no card to add -> could have to transfer discard or the player has won
}

void Player::setCurrentPlayer(bool b) {
	currentPlayer = b;
}

int Player::isReserveInUse() {
	if (reserve) {
		return 1;
	} else {
		return 0;
	}
}

void Player::addCardToDraw(std::shared_ptr<Card> card) {
	draw->addCardToBack(card);
}

void Player::addDeckToDiscard(std::shared_ptr<Deck> deck) {
	discard->addDeck(deck);
}

void Player::addDeckToDraw(std::shared_ptr<Deck> deck) {
	draw->addDeck(deck);
}

void Player::addDiscardToDraw() {
	discard->unassignJokers(); // all jokers are unassigned values 
	discard->shuffle(); // discard is shuffled
	addDeckToDraw(discard);
}

void Player::setJokerValue(std::string value) { // sets the value of the top draw card
	if (draw->inspectTopCard() != nullptr) {
		if (draw->inspectTopCard()->getSuit() == "J") {
			draw->inspectTopCard()->setValue(value);
		}
	}
	// handle the scenario where the card doesnt exist?? is nullptr?	
}

std::shared_ptr<Card> Player::inspectDraw() {
	return draw->inspectTopCard();
}

std::shared_ptr<Card> Player::drawCard() {
	return draw->removeTopCard();
}

int Player::getDiscardSize() {
	return discard->getSize();
}

int Player::getDrawSize() {
	return draw->getSize();
}
