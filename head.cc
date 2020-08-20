#include "head.h"
#include "deck.h"
#include "card.h"

int Head::instances = 0;

Head::Head(): Deck() {
	++instances;
	this->headNum = instances;
}

int Head::getHeadNum() {
	return headNum;
}

int Head::getTopIntValue() {
	return cards->back()->intValue();
}

std::string Head::topCard() { 
	return cards->back()->inspectCard();
}	
