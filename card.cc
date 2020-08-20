#include "card.h"
#include <stdexcept>
using namespace std;

string Card::values[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
string Card::suits[] = {"C", "H", "D", "S", "J"}; // be careful when iterating through this

Card::Card(std::string suit, std::string value): jokerAssigned{false} {
	if (inArray(5, suits, suit)) {
		this->suit = suit;
	}
	if (inArray(13, values, value)) {
		this->value = value;
	}
}

std::string Card::getSuit() {
	return suit;
}

std::string Card::getValue() {
	return value;
}

int Card::intValue() {
	if (value == "A") {
		return 1;
	} else if (value == "2") {
		return 2;
	} else if (value == "3") {
		return 3;
	} else if (value == "4") {
		return 4;
	} else if (value == "5") {
		return 5;
	} else if (value == "6") {
		return 6;
	} else if (value == "7") {
		return 7;
	} else if (value == "8") {
		return 8;
	} else if (value == "9") {
		return 9;
	} else if (value == "10") {
		return 10;
	} else if (value == "J") {
		return 11;
	} else if (value == "Q") {
		return 12;
	} else {
		return 13;
	}
}

std::string Card::inspectCard() {
	if (suit == "J" && !jokerAssigned) {
		return "Joker";
	} else {
		std::string retval = value + suit;
		return retval;
	}
}

void Card::setSuit(std::string suit) {
	if (inArray(5, suits, suit)) {
		this->suit = suit;
	} else {
		throw invalid_argument("Invalid suit.");
	}
}

void Card::jokerUnassigned() { // should be called on all cards in a head when its discarded
	jokerAssigned = false;
}

void Card::setValue(std::string value) {
	if (inArray(13, values, value)) {
		this->value = value;
		if (suit == "J" && !jokerAssigned) {
			jokerAssigned = true;
		}
	} else {
		throw invalid_argument("Invalid value.");
	}
}

bool Card::inArray(int arraySize, std::string arr[], std::string s) {
	for (int x = 0; x < arraySize; ++x) {
		if (s == arr[x]) {
			return true;
		}
	}
	return false;
}
